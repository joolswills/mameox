#pragma code_seg("CC44")
#pragma data_seg("CD44")
#pragma bss_seg("CB44")
#pragma const_seg("CK44")
#pragma comment(linker, "/merge:CD44=CPU44")
#pragma comment(linker, "/merge:CC44=CPU44")
#pragma comment(linker, "/merge:CB44=CPU44")
#pragma comment(linker, "/merge:CK44=CPU44")
#include "g65816cm.h"
#define EXECUTION_MODE EXECUTION_MODE_M1X1
#include "g65816op.h"
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
