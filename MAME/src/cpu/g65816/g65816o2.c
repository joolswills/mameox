#pragma code_seg("CC48")
#pragma data_seg("CD48")
#pragma bss_seg("CB48")
#pragma const_seg("CK48")
#pragma comment(linker, "/merge:CD48=CPU48")
#pragma comment(linker, "/merge:CC48=CPU48")
#pragma comment(linker, "/merge:CB48=CPU48")
#pragma comment(linker, "/merge:CK48=CPU48")
#include "g65816cm.h"
#define EXECUTION_MODE EXECUTION_MODE_M1X0
#include "g65816op.h"
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
