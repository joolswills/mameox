#pragma code_seg("C87")
#pragma data_seg("D87")
#pragma bss_seg("B87")
#pragma const_seg("K87")
#pragma comment(linker, "/merge:D87=87")
#pragma comment(linker, "/merge:C87=87")
#pragma comment(linker, "/merge:B87=87")
#pragma comment(linker, "/merge:K87=87")
#include "driver.h"

/* macro to convert 4-bit unsigned samples to 8-bit signed samples */
#define SAMPLE_CONV4(a) (0x11*((a&0x0f))-0x80)


static signed char *speech;	/* 24k for speech */
static int channel;



int bosco_sh_start(const struct MachineSound *msound)
{
	int i;
	unsigned char bits;

	channel = mixer_allocate_channel(25);
	mixer_set_name(channel,"Samples");

	speech = auto_malloc(2*memory_region_length(REGION_SOUND2));
	if (!speech)
		return 1;

	/* decode the rom samples */
	for (i = 0;i < memory_region_length(REGION_SOUND2);i++)
	{
		bits = memory_region(REGION_SOUND2)[i] & 0x0f;
		speech[2*i] = SAMPLE_CONV4(bits);

		bits = (memory_region(REGION_SOUND2)[i] & 0xf0) >> 4;
		speech[2*i + 1] = SAMPLE_CONV4(bits);
	}

	return 0;
}


void bosco_sh_stop (void)
{
}


void bosco_sample_play(int offset, int length)
{
	if (Machine->sample_rate == 0)
		return;

	mixer_play_sample(channel,speech + offset,length,4000,0);
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
