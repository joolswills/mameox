#pragma code_seg("CC47")
#pragma data_seg("CD47")
#pragma bss_seg("CB47")
#pragma const_seg("CK47")
#pragma comment(linker, "/merge:CD47=CPU47")
#pragma comment(linker, "/merge:CC47=CPU47")
#pragma comment(linker, "/merge:CB47=CPU47")
#pragma comment(linker, "/merge:CK47=CPU47")
#include "g65816cm.h"
#define EXECUTION_MODE EXECUTION_MODE_M1X1
#include "g65816op.h"
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
