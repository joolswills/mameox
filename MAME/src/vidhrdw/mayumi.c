#pragma code_seg("C453")
#pragma data_seg("D453")
#pragma bss_seg("B453")
#pragma const_seg("K453")
#pragma comment(linker, "/merge:D453=453")
#pragma comment(linker, "/merge:C453=453")
#pragma comment(linker, "/merge:B453=453")
#pragma comment(linker, "/merge:K453=453")
/******************************************************************************

Kikiippatsu Mayumi-chan (c) 1988 Victory L.L.C.

Video hardware
	driver by Uki

******************************************************************************/

#include "vidhrdw/generic.h"

static data8_t *mayumi_videoram;
static struct tilemap *mayumi_tilemap;

static void get_tile_info(int tile_index)
{
	int code = mayumi_videoram[tile_index] + (mayumi_videoram[tile_index+0x800] & 0x1f)*0x100 ;
	int col = (mayumi_videoram[tile_index+0x1000] >> 3) & 0x1f;

	SET_TILE_INFO(0, code, col, 0)
}

VIDEO_START( mayumi )
{
	mayumi_videoram = auto_malloc(0x1800);

	mayumi_tilemap = tilemap_create( get_tile_info,tilemap_scan_rows,TILEMAP_OPAQUE,8,8,64,32 );

	if (!mayumi_videoram || !mayumi_tilemap )
		return 1;

	return 0;
}

WRITE8_HANDLER( mayumi_videoram_w )
{
	mayumi_videoram[offset] = data;

	tilemap_mark_tile_dirty(mayumi_tilemap, offset & 0x7ff );
}

READ8_HANDLER( mayumi_videoram_r )
{
	return mayumi_videoram[offset];
}

VIDEO_UPDATE( mayumi )
{
	tilemap_draw(bitmap, cliprect, mayumi_tilemap, 0, 0);
}

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
