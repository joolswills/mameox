#pragma code_seg("C379")
#pragma data_seg("D379")
#pragma bss_seg("B379")
#pragma const_seg("K379")
#pragma comment(linker, "/merge:D379=379")
#pragma comment(linker, "/merge:C379=379")
#pragma comment(linker, "/merge:B379=379")
#pragma comment(linker, "/merge:K379=379")
/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"



/* palette colors (see drivers/8080bw.c) */
enum { BLACK, WHITE };


static unsigned char mask = 0;

WRITE_HANDLER( ramtek_mask_w )
{
	mask = data;
}

WRITE_HANDLER( ramtek_videoram_w )
{
	data = data & ~mask;

	if (videoram[offset] != data)
	{
		int i,x,y;

		videoram[offset] = data;

		y = offset / 32;
		x = 8 * (offset % 32);

		for (i = 0; i < 8; i++)
		{
			plot_pixel(tmpbitmap, x, y, Machine->pens[(data & 0x80) ? WHITE : BLACK]);

			x++;
			data <<= 1;
		}
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
