#pragma code_seg("C143")
#pragma data_seg("D143")
#pragma bss_seg("B143")
#pragma const_seg("K143")
#pragma comment(linker, "/merge:D143=143")
#pragma comment(linker, "/merge:C143=143")
#pragma comment(linker, "/merge:B143=143")
#pragma comment(linker, "/merge:K143=143")
#include "driver.h"
#include "vidhrdw/generic.h"

static struct tilemap *bg_tilemap;

WRITE_HANDLER( blockade_videoram_w )
{
	if (videoram[offset] != data)
	{
		videoram[offset] = data;
		tilemap_mark_tile_dirty(bg_tilemap, offset);
	}
}

static void get_bg_tile_info(int tile_index)
{
	int code = videoram[tile_index];

	SET_TILE_INFO(0, code, 0, 0)
}

VIDEO_START( blockade )
{
	bg_tilemap = tilemap_create(get_bg_tile_info, tilemap_scan_rows, 
		TILEMAP_OPAQUE, 8, 8, 32, 32);

	if ( !bg_tilemap )
		return 1;

	return 0;
}

VIDEO_UPDATE( blockade )
{
	tilemap_draw(bitmap, &Machine->visible_area, bg_tilemap, 0, 0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
