#pragma code_seg("C129")
#pragma data_seg("D129")
#pragma bss_seg("B129")
#pragma const_seg("K129")
#pragma comment(linker, "/merge:D129=129")
#pragma comment(linker, "/merge:C129=129")
#pragma comment(linker, "/merge:B129=129")
#pragma comment(linker, "/merge:K129=129")
/***************************************************************************

	Atari Avalanche hardware

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "artwork.h"
#include "avalnche.h"


WRITE_HANDLER( avalnche_videoram_w )
{
	videoram[offset] = data;

	if (offset >= 0x200)
	{
		int x,y,i;

		x = 8 * (offset % 32);
		y = offset / 32;

		for (i = 0;i < 8;i++)
			plot_pixel(tmpbitmap,x+7-i,y,Machine->pens[(data >> i) & 1]);
	}
}


VIDEO_UPDATE( avalnche )
{
	if (get_vh_global_attribute_changed())
	{
		int offs;

		for (offs = 0;offs < videoram_size; offs++)
			avalnche_videoram_w(offs,videoram[offs]);
	}

	/* copy the character mapped graphics */
	copybitmap(bitmap,tmpbitmap,0,0,0,0,&Machine->visible_area,TRANSPARENCY_NONE,0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
