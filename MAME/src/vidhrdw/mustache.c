#pragma code_seg("C357")
#pragma data_seg("D357")
#pragma bss_seg("B357")
#pragma const_seg("K357")
#pragma comment(linker, "/merge:D357=357")
#pragma comment(linker, "/merge:C357=357")
#pragma comment(linker, "/merge:B357=357")
#pragma comment(linker, "/merge:K357=357")
/***************************************************************************

	Mustache Boy
	(c)1987 March Electronics

***************************************************************************/


#include "driver.h"
#include "vidhrdw/generic.h"

static struct tilemap *bg_tilemap;
static int control_byte;

PALETTE_INIT(mustache)
{
  int i;

  for (i = 0;i < 256;i++)
  {
	int bit0,bit1,bit2,bit3,r,g,b;

	/* red component */
	bit0 = (color_prom[i] >> 0) & 0x01;
	bit1 = (color_prom[i] >> 1) & 0x01;
	bit2 = (color_prom[i] >> 2) & 0x01;
	bit3 = (color_prom[i] >> 3) & 0x01;
	r = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

	/* green component */
	bit0 = (color_prom[i + 256] >> 0) & 0x01;
	bit1 = (color_prom[i + 256] >> 1) & 0x01;
	bit2 = (color_prom[i + 256] >> 2) & 0x01;
	bit3 = (color_prom[i + 256] >> 3) & 0x01;
	g = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

	/* blue component */
	bit0 = (color_prom[i + 512] >> 0) & 0x01;
	bit1 = (color_prom[i + 512] >> 1) & 0x01;
	bit2 = (color_prom[i + 512] >> 2) & 0x01;
	bit3 = (color_prom[i + 512] >> 3) & 0x01;
	b = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

	palette_set_color(i,r,g,b);
  }
}


WRITE_HANDLER (mustache_video_control_w)
{
	if((control_byte^data)&8) /* tile bank  */
	{
		control_byte=data;
		tilemap_mark_all_tiles_dirty(bg_tilemap);
	}
}

WRITE_HANDLER( mustache_scroll_w )
{
	tilemap_set_scrollx(bg_tilemap,0,0x100-data);
	tilemap_set_scrollx(bg_tilemap,1,0x100-data);
	tilemap_set_scrollx(bg_tilemap,2,0x100-data);
	tilemap_set_scrollx(bg_tilemap,3,0x100);
}

WRITE_HANDLER( mustache_videoram_w )
{
	if (videoram[offset] != data)
	{
		videoram[offset] = data;
		tilemap_mark_tile_dirty(bg_tilemap,offset/2);
	}
}


static UINT32 scan_rows_flipx( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return row*num_cols + (num_cols-1) - col;
}

static void get_bg_tile_info(int tile_index)
{
	int attr = videoram[2*tile_index+1];
	int code = videoram[2*tile_index] + ((attr&0xe0)<<3)+ ( (control_byte&8) << 7);

	SET_TILE_INFO(
			0,
			code,
			attr & 0xf,0)
}


VIDEO_START( mustache )
{
	bg_tilemap = tilemap_create(get_bg_tile_info,scan_rows_flipx,TILEMAP_OPAQUE,8,8,64,32);
	tilemap_set_scroll_rows(bg_tilemap,4);

	return 0;
}

static void draw_sprites( struct mame_bitmap *bitmap, const struct rectangle *cliprect )
{
	struct rectangle clip = *cliprect;
	const struct GfxElement *gfx = Machine->gfx[1];
	int offs;

	for (offs = 0;offs < spriteram_size;offs += 4)
	{
		int sy = 240-spriteram[offs];
		int sx = 240-spriteram[offs+3];
		int code = spriteram[offs+2];
		int attr = spriteram[offs+1];
		int color = (attr & 0xe0)>>5;

		if (sy == 240) continue;

		code+=(attr&0x0c)<<6;

		if((control_byte & 0xa))
			clip.max_y = Machine->visible_area.max_y;
		else
			clip.max_y = Machine->visible_area.max_y - 56;

		drawgfx(bitmap,gfx,
				code,
				color,
				0,0,
				sx,sy,
				&clip,TRANSPARENCY_PEN,0);
	}
}


VIDEO_UPDATE( mustache )
{
	tilemap_draw(bitmap,cliprect,bg_tilemap,0,0);
	draw_sprites(bitmap,cliprect);
}


#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
