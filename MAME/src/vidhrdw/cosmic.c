#pragma code_seg("C119")
#pragma bss_seg("B119")
#pragma data_seg("D119")
#pragma const_seg("K119")
/***************************************************************************

 COSMIC.C

 emulation of video hardware of cosmic machines of 1979-1980(ish)

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"


static int (*map_color)(int x, int y);

static int color_registers[3];
static int color_base = 0;
static int nomnlnd_background_on=0;


/* No Mans Land - I don't know if there are more screen layouts than this */
/* this one seems to be OK for the start of the game       */

static const signed short nomnlnd_tree_positions[2][2] =
{
	{66,63},{66,159}
};

static const signed short nomnlnd_water_positions[4][2] =
{
	{160,32},{160,96},{160,128},{160,192}
};


WRITE_HANDLER( panic_color_register_w )
{
	/* 7c0c & 7c0e = Rom Address Offset
 	   7c0d        = high / low nibble */

	set_vh_global_attribute(&color_registers[offset], data & 0x80);

   	color_base = (color_registers[0] << 2) + (color_registers[2] << 3);
}

WRITE_HANDLER( cosmicg_color_register_w )
{
	set_vh_global_attribute(&color_registers[offset], data);

   	color_base = (color_registers[0] << 8) + (color_registers[1] << 9);
}


static int panic_map_color(int x, int y)
{
	/* 8 x 16 coloring */
	unsigned char byte = memory_region(REGION_USER1)[color_base + (x / 16) * 32 + (y / 8)];

	if (color_registers[1])
		return byte >> 4;
	else
		return byte & 0x0f;
}

static int cosmicg_map_color(int x, int y)
{
	unsigned char byte;

	/* 16 x 16 coloring */
	byte = memory_region(REGION_USER1)[color_base + (y / 16) * 16 + (x / 16)];

	/* the upper 4 bits are for cocktail mode support */

	return byte & 0x0f;
}

static int magspot2_map_color(int x, int y)
{
	unsigned char byte;

	/* 16 x 8 coloring */

	// Should the top line of the logo be red or white???

	byte = memory_region(REGION_USER1)[(x / 8) * 16 + (y / 16)];

	if (color_registers[1])
		return byte >> 4;
	else
		return byte & 0x0f;
}


static const unsigned char panic_remap_sprite_code[64][2] =
{
{0x00,0},{0x26,0},{0x25,0},{0x24,0},{0x23,0},{0x22,0},{0x21,0},{0x20,0}, /* 00 */
{0x00,0},{0x26,0},{0x25,0},{0x24,0},{0x23,0},{0x22,0},{0x21,0},{0x20,0}, /* 08 */
{0x00,0},{0x16,0},{0x15,0},{0x14,0},{0x13,0},{0x12,0},{0x11,0},{0x10,0}, /* 10 */
{0x00,0},{0x16,0},{0x15,0},{0x14,0},{0x13,0},{0x12,0},{0x11,0},{0x10,0}, /* 18 */
{0x00,0},{0x06,0},{0x05,0},{0x04,0},{0x03,0},{0x02,0},{0x01,0},{0x00,0}, /* 20 */
{0x00,0},{0x06,0},{0x05,0},{0x04,0},{0x03,0},{0x02,0},{0x01,0},{0x00,0}, /* 28 */
{0x07,2},{0x06,2},{0x05,2},{0x04,2},{0x03,2},{0x02,2},{0x01,2},{0x00,2}, /* 30 */
{0x07,2},{0x06,2},{0x05,2},{0x04,2},{0x03,2},{0x02,2},{0x01,2},{0x00,2}, /* 38 */
};

/*
 * Panic Color table setup
 *
 * Bit 0 = RED, Bit 1 = GREEN, Bit 2 = BLUE
 *
 * First 8 colors are normal intensities
 *
 * But, bit 3 can be used to pull Blue via a 2k resistor to 5v
 * (1k to ground) so second version of table has blue set to 2/3
 *
 */

PALETTE_INIT( panic )
{
	int i;
	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int r = 0xff * ((i >> 0) & 1);
		int g = 0xff * ((i >> 1) & 1);
		int b;
		if ((i & 0x0c) == 0x08)
			b = 0xaa;
		else
			b = 0xff * ((i >> 2) & 1);
		palette_set_color(i,r,g,b);
	}


	for (i = 0;i < TOTAL_COLORS(0);i++)
		COLOR(0,i) = *(color_prom++) & 0x0f;


    map_color = panic_map_color;
}


/*
 * Cosmic Alien Color table setup
 *
 * 8 colors, 16 sprite color codes
 *
 * Bit 0 = RED, Bit 1 = GREEN, Bit 2 = BLUE
 *
 */

PALETTE_INIT( cosmica )
{
	int i;

	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int r = 0xff * ((i >> 0) & 1);
		int g = 0xff * ((i >> 1) & 1);
		int b = 0xff * ((i >> 2) & 1);
		palette_set_color(i,r,g,b);
	}


	for (i = 0;i < TOTAL_COLORS(0)/2;i++)
	{
		COLOR(0,i)                     =  * color_prom          & 0x07;
		COLOR(0,i+(TOTAL_COLORS(0)/2)) = (*(color_prom++) >> 4) & 0x07;
	}


    map_color = panic_map_color;
}


/*
 * Cosmic guerilla table setup
 *
 * Use AA for normal, FF for Full Red
 * Bit 0 = R, bit 1 = G, bit 2 = B, bit 4 = High Red
 *
 * It's possible that the background is dark gray and not black, as the
 * resistor chain would never drop to zero, Anybody know ?
 *
 */

PALETTE_INIT( cosmicg )
{
	int i;

	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int r,g,b;
		
    	if (i > 8) r = 0xff;
        else r = 0xaa * ((i >> 0) & 1);

		g = 0xaa * ((i >> 1) & 1);
		b = 0xaa * ((i >> 2) & 1);
		palette_set_color(i,r,g,b);
	}


    map_color = cosmicg_map_color;
}

/**************************************************/
/* Magical Spot 2/Devil Zone specific routines    */
/*												  */
/* 16 colors, 8 sprite color codes				  */
/**************************************************/

PALETTE_INIT( magspot2 )
{
	int i;
	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])


	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int r,g,b;
		
		if ((i & 0x09) == 0x08)
			r = 0xaa;
	 	else
			r = 0xff * ((i >> 0) & 1);

		g = 0xff * ((i >> 1) & 1);
		b = 0xff * ((i >> 2) & 1);
		palette_set_color(i,r,g,b);
	}


	for (i = 0;i < TOTAL_COLORS(0);i++)
	{
		COLOR(0,i) = *(color_prom++) & 0x0f;
	}


    map_color = magspot2_map_color;
}


WRITE_HANDLER( nomnlnd_background_w )
{
	nomnlnd_background_on = data;
}


WRITE_HANDLER( cosmica_videoram_w )
{
    int i,x,y,col;

    videoram[offset] = data;

	y = offset / 32;
	x = 8 * (offset % 32);

    col = Machine->pens[map_color(x, y)];

    for (i = 0; i < 8; i++)
    {
		if (flip_screen)
			plot_pixel(tmpbitmap, 255-x, 255-y, (data & 0x80) ? col : Machine->pens[0]);
		else
			plot_pixel(tmpbitmap,     x,     y, (data & 0x80) ? col : Machine->pens[0]);

	    x++;
	    data <<= 1;
    }
}


VIDEO_UPDATE( cosmicg )
{
	if (get_vh_global_attribute_changed())
	{
		int offs;

		for (offs = 0; offs < videoram_size; offs++)
		{
			cosmica_videoram_w(offs, videoram[offs]);
		}
	}

	copybitmap(bitmap,tmpbitmap,0,0,0,0,&Machine->visible_area,TRANSPARENCY_NONE,0);
}


VIDEO_UPDATE( panic )
{
	int offs;


	video_update_cosmicg(bitmap, 0);


    /* draw the sprites */

	for (offs = spriteram_size - 4;offs >= 0;offs -= 4)
	{
		if (spriteram[offs] != 0)
		{
			int code,bank,flipy;

			/* panic_remap_sprite_code sprite number to my layout */

			code = panic_remap_sprite_code[(spriteram[offs] & 0x3F)][0];
			bank = panic_remap_sprite_code[(spriteram[offs] & 0x3F)][1];
			flipy = spriteram[offs] & 0x40;

			if((code==0) && (bank==0))
				logerror("remap failure %2x\n",(spriteram[offs] & 0x3F));

			/* Switch Bank */

			if(spriteram[offs+3] & 0x08) bank=1;

			if (flip_screen)
			{
				flipy = !flipy;
			}

			drawgfx(bitmap,Machine->gfx[bank],
					code,
					7 - (spriteram[offs+3] & 0x07),
					flip_screen,flipy,
					256-spriteram[offs+2],spriteram[offs+1],
					&Machine->visible_area,TRANSPARENCY_PEN,0);
		}
	}
}


VIDEO_UPDATE( cosmica )
{
	int offs;


	video_update_cosmicg(bitmap, 0);


    /* draw the sprites */

	for (offs = spriteram_size - 4;offs >= 0;offs -= 4)
	{
		if (spriteram[offs] != 0)
        {
			int code, color;

			code  = ~spriteram[offs  ] & 0x3f;
			color = ~spriteram[offs+3] & 0x0f;

            if (spriteram[offs] & 0x80)
            {
                /* 16x16 sprite */

			    drawgfx(bitmap,Machine->gfx[0],
					    code,
					    color,
					    0,0,
				    	256-spriteram[offs+2],spriteram[offs+1],
				        &Machine->visible_area,TRANSPARENCY_PEN,0);
            }
            else
            {
                /* 32x32 sprite */

			    drawgfx(bitmap,Machine->gfx[1],
					    code >> 2,
					    color,
					    0,0,
				    	256-spriteram[offs+2],spriteram[offs+1],
				        &Machine->visible_area,TRANSPARENCY_PEN,0);
            }
        }
	}
}


VIDEO_UPDATE( magspot2 )
{
	int offs;


	video_update_cosmicg(bitmap, 0);


    /* draw the sprites */

	for (offs = spriteram_size - 4;offs >= 0;offs -= 4)
	{
		if (spriteram[offs] != 0)
        {
			int code, color;

			code  = ~spriteram[offs  ] & 0x3f;
			color = ~spriteram[offs+3] & 0x07;

            if (spriteram[offs] & 0x80)
            {
                /* 16x16 sprite */

			    drawgfx(bitmap,Machine->gfx[0],
					    code,
					    color,
					    0,0,
				    	256-spriteram[offs+2],spriteram[offs+1],
				        &Machine->visible_area,TRANSPARENCY_PEN,0);
            }
            else
            {
                /* 32x32 sprite */

			    drawgfx(bitmap,Machine->gfx[1],
					    code >> 2,
					    color,
					    0,0,
				    	256-spriteram[offs+2],spriteram[offs+1],
				        &Machine->visible_area,TRANSPARENCY_PEN,0);
            }
        }
	}
}


VIDEO_UPDATE( nomnlnd )
{
	int offs;


	video_update_magspot2(bitmap, 0);


    if (nomnlnd_background_on)
    {
		// draw trees

        static UINT8 water_animate=0;

        water_animate++;

    	for(offs=0;offs<2;offs++)
        {
			int code,x,y;

			x = nomnlnd_tree_positions[offs][0];
			y = nomnlnd_tree_positions[offs][1];

			if (flip_screen)
			{
				x = 223 - x;
				y = 223 - y;
				code = 2 + offs;
			}
			else
			{
				code = offs;
			}

    		drawgfx(bitmap,Machine->gfx[2],
					code,
					8,
					0,0,
					x,y,
					&Machine->visible_area,TRANSPARENCY_PEN,0);
        }

		// draw water

    	for(offs=0;offs<4;offs++)
        {
			int x,y;

			x = nomnlnd_water_positions[offs][0];
			y = nomnlnd_water_positions[offs][1];

			if (flip_screen)
			{
				x = 239 - x;
				y = 223 - y;
			}

    		drawgfx(bitmap,Machine->gfx[3],
					water_animate >> 3,
					9,
					0,0,
					x,y,
					&Machine->visible_area,TRANSPARENCY_NONE,0);
        }
    }
}
#pragma data_seg()
#pragma code_seg()
#pragma bss_seg()
#pragma const_seg()
