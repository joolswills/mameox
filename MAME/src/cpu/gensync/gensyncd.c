#pragma code_seg("CC49")
#pragma data_seg("CD49")
#pragma bss_seg("CB49")
#pragma const_seg("CK49")
#pragma comment(linker, "/merge:CD49=CPU49")
#pragma comment(linker, "/merge:CC49=CPU49")
#pragma comment(linker, "/merge:CB49=CPU49")
#pragma comment(linker, "/merge:CK49=CPU49")
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
