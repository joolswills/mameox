#pragma code_seg("C57")
#pragma data_seg("D57")
#pragma bss_seg("B57")
#pragma const_seg("K57")
#pragma comment(linker, "/merge:D57=57")
#pragma comment(linker, "/merge:C57=57")
#pragma comment(linker, "/merge:B57=57")
#pragma comment(linker, "/merge:K57=57")
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
