#pragma code_seg("C133")
#pragma data_seg("D133")
#pragma bss_seg("B133")
#pragma const_seg("K133")
#pragma comment(linker, "/merge:D133=133")
#pragma comment(linker, "/merge:C133=133")
#pragma comment(linker, "/merge:B133=133")
#pragma comment(linker, "/merge:K133=133")
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
