#pragma code_seg("C81")
#pragma data_seg("D81")
#pragma bss_seg("B81")
#pragma const_seg("K81")
#pragma comment(linker, "/merge:D81=81")
#pragma comment(linker, "/merge:C81=81")
#pragma comment(linker, "/merge:B81=81")
#pragma comment(linker, "/merge:K81=81")
/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/
#include "driver.h"
#include "vidhrdw/generic.h"
#include "ctype.h"



unsigned char *bublbobl_objectram;
size_t bublbobl_objectram_size;
int bublbobl_video_enable;


/***************************************************************************

  Draw the game screen in the given mame_bitmap.
  Do NOT call osd_update_display() from this function, it will be called by
  the main emulation engine.

***************************************************************************/
VIDEO_UPDATE( bublbobl )
{
	int offs;
	int sx,sy,xc,yc;
	int gfx_num,gfx_attr,gfx_offs;
	const UINT8 *prom_line;


	/* Bubble Bobble doesn't have a real video RAM. All graphics (characters */
	/* and sprites) are stored in the same memory region, and information on */
	/* the background character columns is stored in the area dd00-dd3f */

	/* This clears & redraws the entire screen each pass */
	fillbitmap(bitmap,Machine->pens[255],&Machine->visible_area);

	if (!bublbobl_video_enable) return;

	sx = 0;

	for (offs = 0;offs < bublbobl_objectram_size;offs += 4)
    {
		/* skip empty sprites */
		/* this is dword aligned so the UINT32 * cast shouldn't give problems */
		/* on any architecture */
		if (*(UINT32 *)(&bublbobl_objectram[offs]) == 0)
			continue;

		gfx_num = bublbobl_objectram[offs + 1];
		gfx_attr = bublbobl_objectram[offs + 3];
		prom_line = memory_region(REGION_PROMS) + 0x80 + ((gfx_num & 0xe0) >> 1);

		gfx_offs = ((gfx_num & 0x1f) * 0x80);
		if ((gfx_num & 0xa0) == 0xa0)
			gfx_offs |= 0x1000;

		sy = -bublbobl_objectram[offs + 0];

		for (yc = 0;yc < 32;yc++)
		{
			if (prom_line[yc/2] & 0x08)	continue;	/* NEXT */

			if (!(prom_line[yc/2] & 0x04))	/* next column */
			{
				sx = bublbobl_objectram[offs + 2];
				if (gfx_attr & 0x40) sx -= 256;
			}

			for (xc = 0;xc < 2;xc++)
			{
				int goffs,code,color,flipx,flipy,x,y;

				goffs = gfx_offs + xc * 0x40 + (yc & 7) * 0x02 +
						(prom_line[yc/2] & 0x03) * 0x10;
				code = videoram[goffs] + 256 * (videoram[goffs + 1] & 0x03) + 1024 * (gfx_attr & 0x0f);
				color = (videoram[goffs + 1] & 0x3c) >> 2;
				flipx = videoram[goffs + 1] & 0x40;
				flipy = videoram[goffs + 1] & 0x80;
				x = sx + xc * 8;
				y = (sy + yc * 8) & 0xff;

				if (flip_screen)
				{
					x = 248 - x;
					y = 248 - y;
					flipx = !flipx;
					flipy = !flipy;
				}

				drawgfx(bitmap,Machine->gfx[0],
						code,
						color,
						flipx,flipy,
						x,y,
						&Machine->visible_area,TRANSPARENCY_PEN,15);
			}
		}

		sx += 16;
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
