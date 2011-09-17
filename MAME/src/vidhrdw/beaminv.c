#pragma code_seg("C157")
#pragma data_seg("D157")
#pragma bss_seg("B157")
#pragma const_seg("K157")
#pragma comment(linker, "/merge:D157=157")
#pragma comment(linker, "/merge:C157=157")
#pragma comment(linker, "/merge:B157=157")
#pragma comment(linker, "/merge:K157=157")
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

WRITE8_HANDLER( beaminv_videoram_w )
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
