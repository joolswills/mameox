#pragma code_seg("C577")
#pragma data_seg("D577")
#pragma bss_seg("B577")
#pragma const_seg("K577")
#pragma comment(linker, "/merge:D577=577")
#pragma comment(linker, "/merge:C577=577")
#pragma comment(linker, "/merge:B577=577")
#pragma comment(linker, "/merge:K577=577")
/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"

PALETTE_INIT( truco )
{
	int i;

	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int	r = ( i & 0x8 ) ? 0xff : 0x00;
		int g = ( i & 0x4 ) ? 0xff : 0x00;
		int b = ( i & 0x2 ) ? 0xff : 0x00;

		int dim = ( i & 0x1 );

		if ( dim ) {
			r >>= 1;
			g >>= 1;
			b >>= 1;
		}

		palette_set_color(i,r,g,b);
	}
}

VIDEO_UPDATE( truco )
{
	data8_t		*vid = memory_region( REGION_CPU1 ) + 0x1800;
	int x, y;

	for( y = 0; y < 192; y++ ) {
		for( x = 0; x < 256; x++ ) {
			int		pixel;

			if ( x & 1 ) {
				pixel = vid[x>>1] & 0x0f;
			} else {
				pixel = ( vid[x>>1] >> 4 ) & 0x0f;
			}

			plot_pixel(bitmap,x,y,Machine->pens[pixel]);
		}

		vid += 0x80;
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
