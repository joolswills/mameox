#pragma code_seg("C168")
#pragma data_seg("D168")
#pragma bss_seg("B168")
#pragma const_seg("K168")
#pragma comment(linker, "/merge:D168=168")
#pragma comment(linker, "/merge:C168=168")
#pragma comment(linker, "/merge:B168=168")
#pragma comment(linker, "/merge:K168=168")
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

	if (input_port_3_r(0) & 0x80)
	{
		logerror("blockade_videoram_w: scanline %d\n", cpu_getscanline());
		cpu_spinuntil_int();
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
