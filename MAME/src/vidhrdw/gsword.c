#pragma code_seg("C227")
#pragma data_seg("D227")
#pragma bss_seg("B227")
#pragma const_seg("K227")
#pragma comment(linker, "/merge:D227=227")
#pragma comment(linker, "/merge:C227=227")
#pragma comment(linker, "/merge:B227=227")
#pragma comment(linker, "/merge:K227=227")
/***************************************************************************
  Great Swordsman

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"

size_t gs_videoram_size;
size_t gs_spritexy_size;

unsigned char *gs_videoram;
unsigned char *gs_scrolly_ram;
unsigned char *gs_spritexy_ram;
unsigned char *gs_spritetile_ram;
unsigned char *gs_spriteattrib_ram;

static struct mame_bitmap 	*bitmap_bg;
static unsigned char 	 	*dirtybuffer;
static int charbank,charpalbank;
static int flipscreen;


PALETTE_INIT( josvolly )
{
	/* sprite lookup table is not original but it is almost 98% correct */

	int sprite_lookup_table[16] = { 0x00,0x02,0x05,0x8C,0x49,0xDD,0xB7,0x06,
					0xD5,0x7A,0x85,0x8D,0x27,0x1A,0x03,0x0F };
	int i;

	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int bit0,bit1,bit2,bit3,r,g,b;


		/* red component */
		bit0 = (color_prom[0] >> 0) & 0x01;
		bit1 = (color_prom[0] >> 1) & 0x01;
		bit2 = (color_prom[0] >> 2) & 0x01;
		bit3 = (color_prom[0] >> 3) & 0x01;
		r = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;
		/* green component */
		bit0 = (color_prom[Machine->drv->total_colors] >> 0) & 0x01;
		bit1 = (color_prom[Machine->drv->total_colors] >> 1) & 0x01;
		bit2 = (color_prom[Machine->drv->total_colors] >> 2) & 0x01;
		bit3 = (color_prom[Machine->drv->total_colors] >> 3) & 0x01;
		g = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;
		/* blue component */
		bit0 = (color_prom[2*Machine->drv->total_colors] >> 0) & 0x01;
		bit1 = (color_prom[2*Machine->drv->total_colors] >> 1) & 0x01;
		bit2 = (color_prom[2*Machine->drv->total_colors] >> 2) & 0x01;
		bit3 = (color_prom[2*Machine->drv->total_colors] >> 3) & 0x01;
		b = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

		palette_set_color(i,r,g,b);
		color_prom++;
	}

	color_prom += 2*Machine->drv->total_colors;
	/* color_prom now points to the beginning of the sprite lookup table */

	/* characters */
	for (i = 0;i < TOTAL_COLORS(0);i++)
		COLOR(0,i) = i;

	/* sprites */
	for (i = 0;i < TOTAL_COLORS(1);i++)
		COLOR(1,i) = sprite_lookup_table[*(color_prom++)];
}


PALETTE_INIT( gsword )
{
	/* sprite lookup table is not original but it is almost 98% correct */

	int sprite_lookup_table[16] = { 0x00,0x02,0x05,0x8C,0x49,0xDD,0xB7,0x06,
					0xD5,0x7A,0x85,0x8D,0x27,0x1A,0x03,0x0F };
	int i;

	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int bit0,bit1,bit2,r,g,b;

		/* red component */
		bit0 = (color_prom[Machine->drv->total_colors] >> 0) & 1;
		bit1 = (color_prom[Machine->drv->total_colors] >> 1) & 1;
		bit2 = (color_prom[Machine->drv->total_colors] >> 2) & 1;
		r = 0x21 * bit0 + 0x47 * bit1 + 0x97 * bit2;
		/* green component */
		bit0 = (color_prom[Machine->drv->total_colors] >> 3) & 1;
		bit1 = (color_prom[0] >> 0) & 1;
		bit2 = (color_prom[0] >> 1) & 1;
		g = 0x21 * bit0 + 0x47 * bit1 + 0x97 * bit2;
		/* blue component */
		bit0 = 0;
		bit1 = (color_prom[0] >> 2) & 1;
		bit2 = (color_prom[0] >> 3) & 1;
		b = 0x21 * bit0 + 0x47 * bit1 + 0x97 * bit2;

		palette_set_color(i,r,g,b);
		color_prom++;
	}

	color_prom += Machine->drv->total_colors;
	/* color_prom now points to the beginning of the sprite lookup table */

	/* characters */
	for (i = 0;i < TOTAL_COLORS(0);i++)
		COLOR(0,i) = i;

	/* sprites */
	for (i = 0;i < TOTAL_COLORS(1);i++)
		COLOR(1,i) = sprite_lookup_table[*(color_prom++)];
}


VIDEO_START( gsword )
{
	if ((dirtybuffer = auto_malloc(gs_videoram_size)) == 0) return 1;
	if ((bitmap_bg = auto_bitmap_alloc(Machine->drv->screen_width,2*Machine->drv->screen_height)) == 0)
		return 1;
	memset(dirtybuffer,1,gs_videoram_size);
	return 0;
}

WRITE_HANDLER( gs_charbank_w )
{
	if (charbank != data)
	{
		charbank = data;
		memset(dirtybuffer,1,gs_videoram_size);
	}
}

WRITE_HANDLER( gs_videoctrl_w )
{
	if (data & 0x8f)
	{
		usrintf_showmessage("videoctrl %02x",data);
	}
	/* bits 5-6 are char palette bank */
	if (charpalbank != ((data & 0x60) >> 5))
	{
		charpalbank = (data & 0x60) >> 5;
		memset(dirtybuffer,1,gs_videoram_size);
	}
	/* bit 4 is flip screen */
	if (flipscreen != (data & 0x10))
	{
		flipscreen = data & 0x10;
	        memset(dirtybuffer,1,gs_videoram_size);
	}

	/* bit 0 could be used but unknown */

	/* other bits unused */
}

WRITE_HANDLER( gs_videoram_w )
{
	if (gs_videoram[offset] != data)
	{
		dirtybuffer[offset] = 1;
		gs_videoram[offset] = data;
	}
}

void render_background(struct mame_bitmap *bitmap)
{
	int offs;

	/* for every character in the Video RAM, check if it has been modified */
	/* since last time and update it accordingly. */

	for (offs = 0; offs < gs_videoram_size ;offs++)
	{
		if (dirtybuffer[offs])
		{
			int sx,sy,tile,flipx,flipy;

			dirtybuffer[offs] = 0;

			sx = offs % 32;
			sy = offs / 32;
			flipx = 0;
			flipy = 0;

			if (flipscreen)
			{
				flipx = !flipx;
				flipy = !flipy;
			}

			tile = gs_videoram[offs] + ((charbank & 0x03) << 8);

			drawgfx(bitmap_bg,Machine->gfx[0],
					tile,
					((tile & 0x3c0) >> 6) + 16 * charpalbank,
					flipx,flipy,
					8*sx,8*sy,
					0,TRANSPARENCY_NONE,0);
		}
	}
}


void render_sprites(struct mame_bitmap *bitmap)
{
	int offs;

	for (offs = 0; offs < gs_spritexy_size - 1; offs+=2)
	{
		int sx,sy,flipx,flipy,spritebank,tile;

		if (gs_spritexy_ram[offs]!=0xf1)
		{
			spritebank = 0;
			tile = gs_spritetile_ram[offs];
			sy = 241-gs_spritexy_ram[offs];
			sx = gs_spritexy_ram[offs+1]-56;
			flipx = gs_spriteattrib_ram[offs] & 0x02;
			flipy = gs_spriteattrib_ram[offs] & 0x01;

			// Adjust sprites that should be far far right!
			if (sx<0) sx+=256;

			// Adjuste for 32x32 tiles(#128-256)
			if (tile > 127)
			{
				spritebank = 1;
				tile -= 128;
				sy-=16;
			}
			if (flipscreen)
			{
				flipx = !flipx;
				flipy = !flipy;
			}
			drawgfx(bitmap,Machine->gfx[1+spritebank],
					tile,
					gs_spritetile_ram[offs+1] & 0x3f,
					flipx,flipy,
					sx,sy,
					&Machine->visible_area,TRANSPARENCY_COLOR, 15);
		}
	}
}

VIDEO_UPDATE( gsword )
{
	int scrollx=0, scrolly=-(*gs_scrolly_ram);

	render_background(bitmap_bg);
	copyscrollbitmap(bitmap,bitmap_bg,1,&scrollx,1,&scrolly,&Machine->visible_area,TRANSPARENCY_NONE,0);
	render_sprites(bitmap);
}

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
