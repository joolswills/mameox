#pragma code_seg("C640")
#pragma data_seg("D640")
#pragma bss_seg("B640")
#pragma const_seg("K640")
#pragma comment(linker, "/merge:D640=640")
#pragma comment(linker, "/merge:C640=640")
#pragma comment(linker, "/merge:B640=640")
#pragma comment(linker, "/merge:K640=640")
/******************************************************************************************

Sengoku Mahjong Video Hardware section

*******************************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"

static struct tilemap *bg_tilemap,*md_tilemap,*tx_tilemap,*fg_tilemap;
data8_t *sm_bgvram;
data8_t *md_vram,*tx_vram,*fg_vram;

READ8_HANDLER( sengoku_sm_bgvram_r )
{
	return sm_bgvram[offset];
}

READ8_HANDLER( sengoku_md_vram_r )
{
	return md_vram[offset];
}

READ8_HANDLER( sengoku_tx_vram_r )
{
	return tx_vram[offset];
}

READ8_HANDLER( sengoku_fg_vram_r )
{
	return fg_vram[offset];
}

WRITE8_HANDLER( sengoku_sm_bgvram_w )
{
	sm_bgvram[offset] = data;
	tilemap_mark_tile_dirty(bg_tilemap,offset/2);
}

WRITE8_HANDLER( sengoku_md_vram_w )
{
	md_vram[offset] = data;
	tilemap_mark_tile_dirty(md_tilemap,offset/2);
}

WRITE8_HANDLER( sengoku_tx_vram_w )
{
	tx_vram[offset] = data;
	tilemap_mark_tile_dirty(tx_tilemap,offset/2);
}

WRITE8_HANDLER( sengoku_fg_vram_w )
{
	fg_vram[offset] = data;
	tilemap_mark_tile_dirty(fg_tilemap,offset/2);
}

static void sengoku_bg_tile_info(int tile_index)
{
	int tile = sm_bgvram[tile_index*2] + (sm_bgvram[2*tile_index+1] << 8);
	int color = (tile >> 12) & 0x0f;

	tile &= 0xfff;

	SET_TILE_INFO(
			1,
			tile,
			color+0x40,
			0)
}

static void sengoku_md_tile_info(int tile_index)
{
	int tile = md_vram[tile_index*2] + (md_vram[2*tile_index+1] << 8);
	int color = (tile >> 12) & 0x0f;

	tile &= 0xfff;

	SET_TILE_INFO(
			1,
			tile + 0x1000,
			color+0x60,
			0)
}

static void sengoku_fg_tile_info(int tile_index)
{
	int tile = fg_vram[tile_index*2] + (fg_vram[2*tile_index+1] << 8);
	int color = (tile >> 12) & 0x0f;

	tile &= 0xfff;

	SET_TILE_INFO(
			1,
			tile + 0x2000,
			color+0x50,
			0)
}

static void sengoku_tx_tile_info(int tile_index)
{
	int tile = tx_vram[tile_index*2] + (tx_vram[2*tile_index+1] << 8);
	int color = (tile >> 12) & 0x0f;

	tile &= 0xfff;

	SET_TILE_INFO(
			2,
			tile + 0x3000,
			color + 0x70,
			0)
}

static void draw_sprites(struct mame_bitmap *bitmap,const struct rectangle *cliprect,int pri)
{
	int offs,fx,fy,x,y,color,sprite;
	int dx,dy,ax,ay;

	for (offs = 0x800-8;offs >= 0;offs -= 8)
	{
		if ((spriteram[offs+1]&0x80)!=0x80) continue;
		sprite = (spriteram[offs+2])+(spriteram[offs+3]<<8);
		if ((sprite>>14)!=pri) continue;
		sprite &= 0x1fff;

		y = (spriteram[offs+6])+(spriteram[offs+7]<<8);
		x = 128 + (spriteram[offs+4])+(spriteram[offs+5]<<8);

		if (x&0x8000) x=0-(0x200-(x&0x1ff));
		//else x&=0x1ff;
		if (y&0x8000) y=0-(0x200-(y&0x1ff));
		//else y&=0x1ff;

		color = (spriteram[offs+0]&0x3f);
		fx = spriteram[offs+0]&0x40;
		fy = 0; /* To do - probably 0x2000 */
		dy=((((spriteram[offs+0])+(spriteram[offs+1]<<8))&0x0380)>>7)+1;
		dx=((((spriteram[offs+0])+(spriteram[offs+1]<<8))&0x1c00)>>10)+1;

		for (ax=0; ax<dx; ax++)
			for (ay=0; ay<dy; ay++) {
				if (!fx)
					drawgfx(bitmap,Machine->gfx[0],
						sprite++,
						color,fx,fy,x+ax*16,y+ay*16,
						cliprect,TRANSPARENCY_PEN,15);
				else
					drawgfx(bitmap,Machine->gfx[0],
						sprite++,
						color,fx,fy,x+(dx-1-ax)*16,y+ay*16,
						cliprect,TRANSPARENCY_PEN,15);
			}
	}
}

VIDEO_START( sengokmj )
{
	bg_tilemap = tilemap_create(sengoku_bg_tile_info,tilemap_scan_rows,TILEMAP_TRANSPARENT,16,16,32,16);
	md_tilemap = tilemap_create(sengoku_md_tile_info,tilemap_scan_rows,TILEMAP_TRANSPARENT,16,16,32,16);
	fg_tilemap = tilemap_create(sengoku_fg_tile_info,tilemap_scan_rows,TILEMAP_TRANSPARENT,16,16,32,16);
	tx_tilemap = tilemap_create(sengoku_tx_tile_info,tilemap_scan_rows,TILEMAP_TRANSPARENT,8,8,64,32);

	if(!bg_tilemap || !md_tilemap || !fg_tilemap || !tx_tilemap)
		return 1;

	tilemap_set_transparent_pen(md_tilemap,15);
	tilemap_set_transparent_pen(fg_tilemap,15);
	tilemap_set_transparent_pen(tx_tilemap,15);

	return 0;
}

VIDEO_UPDATE( sengokmj )
{
	fillbitmap(bitmap, get_black_pen(), cliprect);
	tilemap_draw(bitmap,cliprect,bg_tilemap,0,0);
	draw_sprites(bitmap,cliprect, 2);
	draw_sprites(bitmap,cliprect, 1);
	tilemap_draw(bitmap,cliprect,md_tilemap,0,0);
	tilemap_draw(bitmap,cliprect,fg_tilemap,0,0);
	draw_sprites(bitmap,cliprect, 0);
	draw_sprites(bitmap,cliprect, 3);
	tilemap_draw(bitmap,cliprect,tx_tilemap,0,0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
