#pragma code_seg("C156")
#pragma data_seg("D156")
#pragma bss_seg("B156")
#pragma const_seg("K156")
#pragma comment(linker, "/merge:D156=156")
#pragma comment(linker, "/merge:C156=156")
#pragma comment(linker, "/merge:B156=156")
#pragma comment(linker, "/merge:K156=156")
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
