#pragma code_seg("CC89")
#pragma data_seg("CD89")
#pragma bss_seg("CB89")
#pragma const_seg("CK89")
#pragma comment(linker, "/merge:CD89=CPU89")
#pragma comment(linker, "/merge:CC89=CPU89")
#pragma comment(linker, "/merge:CB89=CPU89")
#pragma comment(linker, "/merge:CK89=CPU89")
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
