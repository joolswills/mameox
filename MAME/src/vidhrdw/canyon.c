#pragma code_seg("C86")
#pragma data_seg("D86")
#pragma bss_seg("B86")
#pragma const_seg("K86")
#pragma comment(linker, "/merge:D86=86")
#pragma comment(linker, "/merge:C86=86")
#pragma comment(linker, "/merge:B86=86")
#pragma comment(linker, "/merge:K86=86")
/***************************************************************************

	Atari Canyon Bomber hardware

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "canyon.h"

/***************************************************************************

  Draw the game screen in the given mame_bitmap.
  Do NOT call osd_update_display() from this function, it will be called by
  the main emulation engine.

***************************************************************************/

VIDEO_UPDATE( canyon )
{
    int offs;
    
    if (get_vh_global_attribute_changed())
    	memset(dirtybuffer, 1, videoram_size);

    /* for every character in the Video RAM, check if it has been modified */
	/* since last time and update it accordingly. */
	for (offs = videoram_size - 1;offs >= 0;offs--)
	{
		if (dirtybuffer[offs])
		{
			int charcode;
			int sx,sy;

			dirtybuffer[offs]=0;

			charcode = videoram[offs] & 0x3F;

			sx = 8 * (offs % 32);
			sy = 8 * (offs / 32);
			drawgfx(tmpbitmap,Machine->gfx[0],
					charcode, (videoram[offs] & 0x80)>>7,
					0,0,sx,sy,
					&Machine->visible_area,TRANSPARENCY_NONE,0);
		}
	}

	/* copy the character mapped graphics */
	copybitmap(bitmap,tmpbitmap,0,0,0,0,&Machine->visible_area,TRANSPARENCY_NONE,0);

	for (offs=0; offs<2; offs++)
	{
		int sx, sy;
		int pic;
		int flipx;

		sx = 27*8 - spriteram[offs*2+1];
		sy = 30*8 - spriteram[offs*2+8];
		pic = spriteram[offs*2+9];
		if (pic & 0x80)
			flipx=0;
		else
			flipx=1;

        drawgfx(bitmap,Machine->gfx[1],
                (pic & 0x18) >> 3, offs,
				flipx,0,sx,sy,
				&Machine->visible_area,TRANSPARENCY_PEN,0);
	}

	/* Draw the bombs as 2x2 squares */
	for (offs=2; offs<4; offs++)
	{
		int sx, sy;

		sx = 31*8 - spriteram[offs*2+1];
		sy = 31*8 - spriteram[offs*2+8];

        drawgfx(bitmap,Machine->gfx[2],
                0, offs,
				0,0,sx,sy,
				&Machine->visible_area,TRANSPARENCY_PEN,0);
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
