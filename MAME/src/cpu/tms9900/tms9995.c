#pragma code_seg("CC20")
#pragma data_seg("CD20")
#pragma bss_seg("CB20")
#pragma const_seg("CK20")
#pragma comment(linker, "/merge:CD20=CPU20")
#pragma comment(linker, "/merge:CC20=CPU20")
#pragma comment(linker, "/merge:CB20=CPU20")
#pragma comment(linker, "/merge:CK20=CPU20")
/*
	generate the tms9995 emulator
*/

#include "tms9900.h"

#define TMS99XX_MODEL TMS9995_ID

#include "99xxcore.h"
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
