#pragma code_seg("C285")
#pragma data_seg("D285")
#pragma bss_seg("B285")
#pragma const_seg("K285")
#pragma comment(linker, "/merge:D285=285")
#pragma comment(linker, "/merge:C285=285")
#pragma comment(linker, "/merge:B285=285")
#pragma comment(linker, "/merge:K285=285")
/***************************************************************************

	Kyugo hardware games

***************************************************************************/

#include "driver.h"
#include "kyugo.h"


data8_t *kyugo_sharedram;


/*************************************
 *
 *	Machine initialization
 *
 *************************************/

MACHINE_INIT( kyugo )
{
	// must start with interrupts and sub CPU disabled
	cpu_interrupt_enable(0, 0);
	kyugo_sub_cpu_control_w(0, 0);
}


WRITE_HANDLER( kyugo_sub_cpu_control_w )
{
	cpu_set_halt_line(1, data ? CLEAR_LINE : ASSERT_LINE);
}


/*************************************
 *
 *	Shared RAM handlers
 *
 *************************************/

WRITE_HANDLER( kyugo_sharedram_w )
{
	kyugo_sharedram[offset] = data;
}


READ_HANDLER( kyugo_sharedram_r )
{
	return kyugo_sharedram[offset];
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
