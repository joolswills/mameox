#pragma code_seg("C8")
#pragma bss_seg("B8")
#pragma data_seg("D8")
#pragma const_seg("K8")
/*******************************************************************************

	actfancr - Bryan McPhail, mish@tendril.co.uk

*******************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "state.h"


static UINT8 actfancr_control_1[0x20],actfancr_control_2[0x20];
unsigned char *actfancr_pf1_data,*actfancr_pf2_data,*actfancr_pf1_rowscroll_data;
static struct tilemap *pf1_tilemap,*pf1_alt_tilemap;
static int flipscreen;

static UINT32 actfancr_scan(UINT32 col,UINT32 row,UINT32 num_cols,UINT32 num_rows)
{
	/* logical (col,row) -> memory offset */
	return (col & 0x0f) + ((row & 0x0f) << 4) + ((col & 0xf0) << 4);
}

static UINT32 actfancr_scan2(UINT32 col,UINT32 row,UINT32 num_cols,UINT32 num_rows)
{
	/* logical (col,row) -> memory offset */
	return (col & 0x0f) + ((row & 0x0f) << 4) + ((row & 0x10) << 4) + ((col & 0x70) << 5);
}

static void get_tile_info(int tile_index)
{
	int tile,color;

	tile=actfancr_pf1_data[2*tile_index]+(actfancr_pf1_data[2*tile_index+1]<<8);
	color=tile >> 12;
	tile=tile&0xfff;

	SET_TILE_INFO(
			2,
			tile,
			color,
			0)
}

static UINT32 triothep_scan(UINT32 col,UINT32 row,UINT32 num_cols,UINT32 num_rows)
{
	/* logical (col,row) -> memory offset */
	return (col & 0x0f) + ((row & 0x0f) << 4) + ((row & 0x10) << 4) + ((col & 0x10) << 5);
}

static void get_trio_tile_info(int tile_index)
{
	int tile,color;

	tile=actfancr_pf1_data[2*tile_index]+(actfancr_pf1_data[2*tile_index+1]<<8);
	color=tile >> 12;
	tile=tile&0xfff;

	SET_TILE_INFO(
			2,
			tile,
			color,
			0)
}

/******************************************************************************/

static void register_savestate(void)
{
	state_save_register_UINT8("video", 0, "control_1", actfancr_control_1, 0x20);
	state_save_register_UINT8("video", 0, "control_2", actfancr_control_2, 0x20);
}

VIDEO_START( actfancr )
{
	pf1_tilemap = tilemap_create(get_tile_info,actfancr_scan,TILEMAP_OPAQUE,16,16,256,16);
	pf1_alt_tilemap = tilemap_create(get_tile_info,actfancr_scan2,TILEMAP_OPAQUE,16,16,128,32);

	if (!pf1_tilemap || !pf1_alt_tilemap)
		return 1;

	register_savestate();

	return 0;
}

VIDEO_START( triothep )
{
	pf1_tilemap = tilemap_create(get_trio_tile_info,triothep_scan,TILEMAP_OPAQUE,16,16,32,32);

	if (!pf1_tilemap)
		return 1;

	pf1_alt_tilemap=NULL;

	register_savestate();

	return 0;
}

/******************************************************************************/

WRITE_HANDLER( actfancr_pf1_control_w )
{
	actfancr_control_1[offset]=data;
}

WRITE_HANDLER( actfancr_pf2_control_w )
{
	actfancr_control_2[offset]=data;
}

WRITE_HANDLER( actfancr_pf1_data_w )
{
	actfancr_pf1_data[offset]=data;
	tilemap_mark_tile_dirty(pf1_tilemap,offset/2);
	if (pf1_alt_tilemap) tilemap_mark_tile_dirty(pf1_alt_tilemap,offset/2);
}

READ_HANDLER( actfancr_pf1_data_r )
{
	return actfancr_pf1_data[offset];
}

WRITE_HANDLER( actfancr_pf2_data_w )
{
	actfancr_pf2_data[offset]=data;
}

READ_HANDLER( actfancr_pf2_data_r )
{
	return actfancr_pf2_data[offset];
}

/******************************************************************************/

VIDEO_UPDATE( actfancr )
{
	int my,mx,offs,color,tile,mult;
	int scrollx=(actfancr_control_1[0x10]+(actfancr_control_1[0x11]<<8));
	int scrolly=(actfancr_control_1[0x12]+(actfancr_control_1[0x13]<<8));

	/* Draw playfield */
	flipscreen=actfancr_control_2[0]&0x80;
	tilemap_set_flip(ALL_TILEMAPS,flipscreen ? (TILEMAP_FLIPY | TILEMAP_FLIPX) : 0);

	tilemap_set_scrollx( pf1_tilemap,0, scrollx );
	tilemap_set_scrolly( pf1_tilemap,0, scrolly );
	tilemap_set_scrollx( pf1_alt_tilemap,0, scrollx );
	tilemap_set_scrolly( pf1_alt_tilemap,0, scrolly );

	if (actfancr_control_1[6]==1)
		tilemap_draw(bitmap,cliprect,pf1_alt_tilemap,0,0);
	else
		tilemap_draw(bitmap,cliprect,pf1_tilemap,0,0);

	/* Sprites */
	for (offs = 0;offs < 0x800;offs += 8)
	{
		int x,y,sprite,colour,multi,fx,fy,inc,flash;

		y=buffered_spriteram[offs]+(buffered_spriteram[offs+1]<<8);
 		if ((y&0x8000) == 0) continue;
		x = buffered_spriteram[offs+4]+(buffered_spriteram[offs+5]<<8);
		colour = ((x & 0xf000) >> 12);
		flash=x&0x800;
		if (flash && (cpu_getcurrentframe() & 1)) continue;

		fx = y & 0x2000;
		fy = y & 0x4000;
		multi = (1 << ((y & 0x1800) >> 11)) - 1;	/* 1x, 2x, 4x, 8x height */

											/* multi = 0   1   3   7 */
		sprite = buffered_spriteram[offs+2]+(buffered_spriteram[offs+3]<<8);
		sprite &= 0x0fff;

		x = x & 0x01ff;
		y = y & 0x01ff;
		if (x >= 256) x -= 512;
		if (y >= 256) y -= 512;
		x = 240 - x;
		y = 240 - y;

		sprite &= ~multi;
		if (fy)
			inc = -1;
		else
		{
			sprite += multi;
			inc = 1;
		}

		if (flipscreen) {
			y=240-y;
			x=240-x;
			if (fx) fx=0; else fx=1;
			if (fy) fy=0; else fy=1;
			mult=16;
		}
		else mult=-16;

		while (multi >= 0)
		{
			drawgfx(bitmap,Machine->gfx[1],
					sprite - multi * inc,
					colour,
					fx,fy,
					x,y + mult * multi,
					cliprect,TRANSPARENCY_PEN,0);
			multi--;
		}
	}

	/* Draw character tiles */
	for (offs = 0x800 - 2;offs >= 0;offs -= 2) {
		tile=actfancr_pf2_data[offs]+(actfancr_pf2_data[offs+1]<<8);
		if (!tile) continue;
		color=tile>>12;
		tile=tile&0xfff;
		mx = (offs/2) % 32;
		my = (offs/2) / 32;
		if (flipscreen) {mx=31-mx; my=31-my;}
		drawgfx(bitmap,Machine->gfx[0],
			tile,color,flipscreen,flipscreen,8*mx,8*my,
			cliprect,TRANSPARENCY_PEN,0);
	}
}

VIDEO_UPDATE( triothep )
{
	int my,mx,offs,color,tile,i,mult;
	int scrollx=(actfancr_control_1[0x10]+(actfancr_control_1[0x11]<<8));
	int scrolly=(actfancr_control_1[0x12]+(actfancr_control_1[0x13]<<8));

	/* Draw playfield */
	flipscreen=actfancr_control_2[0]&0x80;
	tilemap_set_flip(ALL_TILEMAPS,flipscreen ? (TILEMAP_FLIPY | TILEMAP_FLIPX) : 0);

	if (actfancr_control_2[0]&0x4) {
		tilemap_set_scroll_rows(pf1_tilemap,32);
		tilemap_set_scrolly( pf1_tilemap,0, scrolly );
		for (i=0; i<32; i++)
			tilemap_set_scrollx( pf1_tilemap,i, scrollx+(actfancr_pf1_rowscroll_data[i*2] | actfancr_pf1_rowscroll_data[i*2+1]<<8) );
	}
	else {
		tilemap_set_scroll_rows(pf1_tilemap,1);
		tilemap_set_scrollx( pf1_tilemap,0, scrollx );
		tilemap_set_scrolly( pf1_tilemap,0, scrolly );
	}

	tilemap_draw(bitmap,cliprect,pf1_tilemap,0,0);

	/* Sprites */
	for (offs = 0;offs < 0x800;offs += 8)
	{
		int x,y,sprite,colour,multi,fx,fy,inc,flash;

		y=buffered_spriteram[offs]+(buffered_spriteram[offs+1]<<8);
 		if ((y&0x8000) == 0) continue;
		x = buffered_spriteram[offs+4]+(buffered_spriteram[offs+5]<<8);
		colour = ((x & 0xf000) >> 12);
		flash=x&0x800;
		if (flash && (cpu_getcurrentframe() & 1)) continue;

		fx = y & 0x2000;
		fy = y & 0x4000;
		multi = (1 << ((y & 0x1800) >> 11)) - 1;	/* 1x, 2x, 4x, 8x height */

											/* multi = 0   1   3   7 */
		sprite = buffered_spriteram[offs+2]+(buffered_spriteram[offs+3]<<8);
		sprite &= 0x0fff;

		x = x & 0x01ff;
		y = y & 0x01ff;
		if (x >= 256) x -= 512;
		if (y >= 256) y -= 512;
		x = 240 - x;
		y = 240 - y;

		sprite &= ~multi;
		if (fy)
			inc = -1;
		else
		{
			sprite += multi;
			inc = 1;
		}

		if (flipscreen) {
			y=240-y;
			x=240-x;
			if (fx) fx=0; else fx=1;
			if (fy) fy=0; else fy=1;
			mult=16;
		}
		else mult=-16;

		while (multi >= 0)
		{
			drawgfx(bitmap,Machine->gfx[1],
					sprite - multi * inc,
					colour,
					fx,fy,
					x,y + mult * multi,
					cliprect,TRANSPARENCY_PEN,0);
			multi--;
		}
	}

	/* Draw character tiles */
	for (offs = 0x800 - 2;offs >= 0;offs -= 2) {
		tile=actfancr_pf2_data[offs]+(actfancr_pf2_data[offs+1]<<8);
		if (!tile) continue;
		color=tile>>12;
		tile=tile&0xfff;
		mx = (offs/2) % 32;
		my = (offs/2) / 32;
		if (flipscreen) {mx=31-mx; my=31-my;}
		drawgfx(bitmap,Machine->gfx[0],
			tile,color,flipscreen,flipscreen,8*mx,8*my,
			cliprect,TRANSPARENCY_PEN,0);
	}
}
#pragma data_seg()
#pragma code_seg()
#pragma bss_seg()
#pragma const_seg()
