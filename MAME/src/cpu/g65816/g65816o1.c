#pragma code_seg("CC88")
#pragma data_seg("CD88")
#pragma bss_seg("CB88")
#pragma const_seg("CK88")
#pragma comment(linker, "/merge:CD88=CPU88")
#pragma comment(linker, "/merge:CC88=CPU88")
#pragma comment(linker, "/merge:CB88=CPU88")
#pragma comment(linker, "/merge:CK88=CPU88")
#include "g65816cm.h"
#define EXECUTION_MODE EXECUTION_MODE_M0X1
#include "g65816op.h"
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
