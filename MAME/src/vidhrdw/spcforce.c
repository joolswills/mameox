#pragma code_seg("C592")
#pragma data_seg("D592")
#pragma bss_seg("B592")
#pragma const_seg("K592")
#pragma comment(linker, "/merge:D592=592")
#pragma comment(linker, "/merge:C592=592")
#pragma comment(linker, "/merge:B592=592")
#pragma comment(linker, "/merge:K592=592")
/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"


unsigned char *spcforce_scrollram;


WRITE_HANDLER( spcforce_flip_screen_w )
{
	flip_screen_set(~data & 0x01);
}

/***************************************************************************

  Draw the game screen in the given mame_bitmap.
  Do NOT call osd_update_display() from this function, it will be called by
  the main emulation engine.

***************************************************************************/
VIDEO_UPDATE( spcforce )
{
	int offs;


	/* draw the characters as sprites because they could be overlapping */

	fillbitmap(bitmap,Machine->pens[0],&Machine->visible_area);


	for (offs = 0; offs < videoram_size; offs++)
	{
		int code,sx,sy,col;


		sy = 8 * (offs / 32) -  (spcforce_scrollram[offs]       & 0x0f);
		sx = 8 * (offs % 32) + ((spcforce_scrollram[offs] >> 4) & 0x0f);

		code = videoram[offs] + ((colorram[offs] & 0x01) << 8);
		col  = (~colorram[offs] >> 4) & 0x07;

		if (flip_screen)
		{
			sx = 248 - sx;
			sy = 248 - sy;
		}

		drawgfx(bitmap,Machine->gfx[0],
				code, col,
				flip_screen, flip_screen,
				sx, sy,
				&Machine->visible_area,TRANSPARENCY_PEN,0);
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
