#pragma code_seg("C589")
#pragma data_seg("D589")
#pragma bss_seg("B589")
#pragma const_seg("K589")
#pragma comment(linker, "/merge:D589=589")
#pragma comment(linker, "/merge:C589=589")
#pragma comment(linker, "/merge:B589=589")
#pragma comment(linker, "/merge:K589=589")
/***************************************************************************

  vidhrdw.c

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "vidhrdw/konamiic.h"

int qdrmfgp_get_palette(void);



static void tile_callback(int layer, int *code, int *color)
{
	*color = ((*color>>2) & 0x0f) | qdrmfgp_get_palette();
}

static void gp2_tile_callback(int layer, int *code, int *color)
{
	*color = (*color>>1) & 0x7f;
}

/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

VIDEO_START( qdrmfgp )
{
	if (K056832_vh_start(REGION_GFX1, K056832_BPP_4dj, 1, NULL, tile_callback))
		return 1;

	K056832_set_LayerAssociation(0);

	K056832_set_LayerOffset(0, 2, 0);
	K056832_set_LayerOffset(1, 4, 0);
	K056832_set_LayerOffset(2, 6, 0);
	K056832_set_LayerOffset(3, 8, 0);

	return 0;
}

VIDEO_START( qdrmfgp2 )
{
	if (K056832_vh_start(REGION_GFX1, K056832_BPP_4dj, 1, NULL, gp2_tile_callback))
		return 1;

	K056832_set_LayerAssociation(0);

	K056832_set_LayerOffset(0, 3, 1);
	K056832_set_LayerOffset(1, 5, 1);
	K056832_set_LayerOffset(2, 7, 1);
	K056832_set_LayerOffset(3, 9, 1);

	return 0;
}

/***************************************************************************

  Display refresh

***************************************************************************/

VIDEO_UPDATE( qdrmfgp )
{
	fillbitmap(bitmap, get_black_pen(), cliprect);

	K056832_tilemap_draw(bitmap,cliprect, 3, 0, 1);
	K056832_tilemap_draw(bitmap,cliprect, 2, 0, 2);
	K056832_tilemap_draw(bitmap,cliprect, 1, 0, 4);
	K056832_tilemap_draw(bitmap,cliprect, 0, 0, 8);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
