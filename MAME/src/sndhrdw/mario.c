#pragma code_seg("C413")
#pragma data_seg("D413")
#pragma bss_seg("B413")
#pragma const_seg("K413")
#pragma comment(linker, "/merge:D413=413")
#pragma comment(linker, "/merge:C413=413")
#pragma comment(linker, "/merge:B413=413")
#pragma comment(linker, "/merge:K413=413")
#include "driver.h"
#include "cpu/i8039/i8039.h"



WRITE_HANDLER( mario_sh_w )
{
	if (data)
		cpu_set_irq_line(1,0,ASSERT_LINE);
	else
		cpu_set_irq_line(1,0,CLEAR_LINE);
}


/* Mario running sample */
WRITE_HANDLER( mario_sh1_w )
{
	static int last;

	if (last!= data)
	{
		last = data;
                if (data && sample_playing(0) == 0) sample_start (0, 3, 0);
	}
}

/* Luigi running sample */
WRITE_HANDLER( mario_sh2_w )
{
	static int last;

	if (last!= data)
	{
		last = data;
                if (data && sample_playing(1) == 0) sample_start (1, 4, 0);
	}
}

/* Misc samples */
WRITE_HANDLER( mario_sh3_w )
{
	static int state[8];

	/* Don't trigger the sample if it's still playing */
	if (state[offset] == data) return;

	state[offset] = data;
	if (data)
	{
		switch (offset)
		{
			case 2: /* ice */
				sample_start (2, 0, 0);
				break;
			case 6: /* coin */
				sample_start (2, 1, 0);
				break;
			case 7: /* skid */
				sample_start (2, 2, 0);
				break;
		}
	}
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
