#pragma code_seg("C69")
#pragma data_seg("D69")
#pragma bss_seg("B69")
#pragma const_seg("K69")
#pragma comment(linker, "/merge:D69=69")
#pragma comment(linker, "/merge:C69=69")
#pragma comment(linker, "/merge:B69=69")
#pragma comment(linker, "/merge:K69=69")
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
