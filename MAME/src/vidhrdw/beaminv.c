#pragma code_seg("C143")
#pragma data_seg("D143")
#pragma bss_seg("B143")
#pragma const_seg("K143")
#pragma comment(linker, "/merge:D143=143")
#pragma comment(linker, "/merge:C143=143")
#pragma comment(linker, "/merge:B143=143")
#pragma comment(linker, "/merge:K143=143")
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
