#pragma code_seg("C142")
#pragma data_seg("D142")
#pragma bss_seg("B142")
#pragma const_seg("K142")
#pragma comment(linker, "/merge:D142=142")
#pragma comment(linker, "/merge:C142=142")
#pragma comment(linker, "/merge:B142=142")
#pragma comment(linker, "/merge:K142=142")
/***************************************************************************

	Tekunon Kougyou Beam Invader hardware

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"


/*************************************
 *
 *	Memory handlers
 *
 *************************************/

WRITE_HANDLER( beaminv_videoram_w )
{
	UINT8 x,y;
	int i;


	videoram[offset] = data;

	y = ~(offset >> 8 << 3);
	x = offset;

	for (i = 0; i < 8; i++)
	{
		plot_pixel(tmpbitmap, x, y, data & 0x01);

		y--;
		data >>= 1;
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
