#pragma code_seg("CC48")
#pragma data_seg("CD48")
#pragma bss_seg("CB48")
#pragma const_seg("CK48")
#pragma comment(linker, "/merge:CD48=CPU48")
#pragma comment(linker, "/merge:CC48=CPU48")
#pragma comment(linker, "/merge:CB48=CPU48")
#pragma comment(linker, "/merge:CK48=CPU48")
#include <stdio.h>
#include <string.h>
#include "gensync.h"

unsigned gensyncd(char *buffer, unsigned PC)
{
	int hblank = activecpu_get_reg(GS_HBLANK);
	int hsync = activecpu_get_reg(GS_HSYNC);
	int vblank = activecpu_get_reg(GS_VBLANK);
	int vsync = activecpu_get_reg(GS_VSYNC);

	if( hblank )
	{
		buffer += sprintf(buffer, " HB");
		if( hsync )
			buffer += sprintf(buffer, " HS");
    }
	else
		buffer += sprintf(buffer, " X:%03X", activecpu_get_reg(GS_X));

    if( vblank )
    {
		buffer += sprintf(buffer, " VB");
		if( vsync )
			buffer += sprintf(buffer, " VS");
    }
	else
		buffer += sprintf(buffer, " Y:%03X", activecpu_get_reg(GS_Y));

    return 1;
}

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
