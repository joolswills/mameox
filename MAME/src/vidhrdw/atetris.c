#pragma code_seg("C119")
#pragma data_seg("D119")
#pragma bss_seg("B119")
#pragma const_seg("K119")
#pragma comment(linker, "/merge:D119=119")
#pragma comment(linker, "/merge:C119=119")
#pragma comment(linker, "/merge:B119=119")
#pragma comment(linker, "/merge:K119=119")
/***************************************************************************

	Atari Tetris hardware

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "atetris.h"


static struct tilemap *tilemap;


/*************************************
 *
 *	Tilemap callback
 *
 *************************************/

static void get_tile_info(int tile_index)
{
	int code = videoram[tile_index * 2] | ((videoram[tile_index * 2 + 1] & 7) << 8);
	int color = (videoram[tile_index * 2 + 1] & 0xf0) >> 4;

	SET_TILE_INFO(0, code, color, 0);
}



/*************************************
 *
 *	Video RAM write
 *
 *************************************/

WRITE_HANDLER( atetris_videoram_w )
{
	videoram[offset] = data;
	tilemap_mark_tile_dirty(tilemap, offset / 2);
}



/*************************************
 *
 *	Video system start
 *
 *************************************/

VIDEO_START( atetris )
{
	tilemap = tilemap_create(get_tile_info, tilemap_scan_rows, TILEMAP_OPAQUE, 8,8, 64,32);
	if (!tilemap)
		return 1;
	return 0;
}



/*************************************
 *
 *	Main refresh
 *
 *************************************/

VIDEO_UPDATE( atetris )
{
	tilemap_draw(bitmap, cliprect, tilemap, 0,0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
