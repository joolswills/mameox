#pragma code_seg("C489")
#pragma data_seg("D489")
#pragma bss_seg("B489")
#pragma const_seg("K489")
#pragma comment(linker, "/merge:D489=489")
#pragma comment(linker, "/merge:C489=489")
#pragma comment(linker, "/merge:B489=489")
#pragma comment(linker, "/merge:K489=489")
/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"


data8_t *mosaic_fgvideoram;
data8_t *mosaic_bgvideoram;

static struct tilemap *bg_tilemap,*fg_tilemap;



/***************************************************************************

  Callbacks for the TileMap code

***************************************************************************/

static void get_fg_tile_info(int tile_index)
{
	tile_index *= 2;
	SET_TILE_INFO(
			0,
			mosaic_fgvideoram[tile_index] + (mosaic_fgvideoram[tile_index+1] << 8),
			0,
			0)
}

static void get_bg_tile_info(int tile_index)
{
	tile_index *= 2;
	SET_TILE_INFO(
			1,
			mosaic_bgvideoram[tile_index] + (mosaic_bgvideoram[tile_index+1] << 8),
			0,
			0)
}



/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

VIDEO_START( mosaic )
{
	fg_tilemap = tilemap_create(get_fg_tile_info,tilemap_scan_rows,TILEMAP_TRANSPARENT,8,8,64,32);
	bg_tilemap = tilemap_create(get_bg_tile_info,tilemap_scan_rows,TILEMAP_OPAQUE,     8,8,64,32);

	if (!fg_tilemap || !bg_tilemap)
		return 1;

	tilemap_set_transparent_pen(fg_tilemap,0xff);

	return 0;
}


/***************************************************************************

  Memory handlers

***************************************************************************/

WRITE8_HANDLER( mosaic_fgvideoram_w )
{
	mosaic_fgvideoram[offset] = data;
	tilemap_mark_tile_dirty(fg_tilemap,offset/2);
}

WRITE8_HANDLER( mosaic_bgvideoram_w )
{
	mosaic_bgvideoram[offset] = data;
	tilemap_mark_tile_dirty(bg_tilemap,offset/2);
}



VIDEO_UPDATE( mosaic )
{
	tilemap_draw(bitmap,cliprect,bg_tilemap,0,0);
	tilemap_draw(bitmap,cliprect,fg_tilemap,0,0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
