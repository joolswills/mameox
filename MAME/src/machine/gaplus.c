#pragma code_seg("C289")
#pragma data_seg("D289")
#pragma bss_seg("B289")
#pragma const_seg("K289")
#pragma comment(linker, "/merge:D289=289")
#pragma comment(linker, "/merge:C289=289")
#pragma comment(linker, "/merge:B289=289")
#pragma comment(linker, "/merge:K289=289")
/***************************************************************************

  machine.c

  Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
  I/O ports)

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "machine/namcoio.h"


data8_t *gaplus_customio_3;

/************************************************************************************
*																					*
*           Gaplus custom I/O chips (preliminary)									*
*																					*
************************************************************************************/

WRITE8_HANDLER( gaplus_customio_3_w )
{
	if ((offset == 0x09) && (data >= 0x0f))
		sample_start(0,0,0);
    gaplus_customio_3[offset] = data;
}


READ8_HANDLER( gaplus_customio_3_r )
{
    int mode;

    mode = gaplus_customio_3[8];
    if (mode == 2)
    {
        switch (offset)
        {
            case 0:
                return readinputport( 4 ); /* cabinet & test mode */
                break;
            case 2:
                return 0x0f;
                break;
            default:
                return gaplus_customio_3[offset];
        }
    }
    else
    {
        switch (offset)
        {
            case 0:
                return readinputport( 4 ); /* cabinet & test mode */
                break;
            case 1:
                return 0x0f;
                break;
            case 2:
                return 0x0e;
                break;
            case 3:
                return 0x01;
                break;
            default:
                return gaplus_customio_3[offset];
        }
    }
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
