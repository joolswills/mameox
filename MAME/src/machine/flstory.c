#pragma code_seg("C199")
#pragma data_seg("D199")
#pragma bss_seg("B199")
#pragma const_seg("K199")
#pragma comment(linker, "/merge:D199=199")
#pragma comment(linker, "/merge:C199=199")
#pragma comment(linker, "/merge:B199=199")
#pragma comment(linker, "/merge:K199=199")
/***************************************************************************

  machine.c

  Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
  I/O ports)

***************************************************************************/

#include "driver.h"


static unsigned char from_main,from_mcu;
static int mcu_sent = 0,main_sent = 0;


/***************************************************************************

 Fairy Land Story 68705 protection interface

 The following is ENTIRELY GUESSWORK!!!

 It seems, however, to be identical to Buggy Challenge.

***************************************************************************/

static unsigned char portA_in,portA_out,ddrA;

READ_HANDLER( flstory_68705_portA_r )
{
//logerror("%04x: 68705 port A read %02x\n",activecpu_get_pc(),portA_in);
	return (portA_out & ddrA) | (portA_in & ~ddrA);
}

WRITE_HANDLER( flstory_68705_portA_w )
{
//logerror("%04x: 68705 port A write %02x\n",activecpu_get_pc(),data);
	portA_out = data;
}

WRITE_HANDLER( flstory_68705_ddrA_w )
{
	ddrA = data;
}



/*
 *  Port B connections:
 *
 *  all bits are logical 1 when read (+5V pullup)
 *
 *  1   W  when 1->0, enables latch which brings the command from main CPU (read from port A)
 *  2   W  when 0->1, copies port A to the latch for the main CPU
 */

static unsigned char portB_in,portB_out,ddrB;

READ_HANDLER( flstory_68705_portB_r )
{
	return (portB_out & ddrB) | (portB_in & ~ddrB);
}

WRITE_HANDLER( flstory_68705_portB_w )
{
//logerror("%04x: 68705 port B write %02x\n",activecpu_get_pc(),data);

	if ((ddrB & 0x02) && (~data & 0x02) && (portB_out & 0x02))
	{
		portA_in = from_main;
		if (main_sent) cpu_set_irq_line(2,0,CLEAR_LINE);
		main_sent = 0;
logerror("read command %02x from main cpu\n",portA_in);
	}
	if ((ddrB & 0x04) && (data & 0x04) && (~portB_out & 0x04))
	{
logerror("send command %02x to main cpu\n",portA_out);
		from_mcu = portA_out;
		mcu_sent = 1;
	}

	portB_out = data;
}

WRITE_HANDLER( flstory_68705_ddrB_w )
{
	ddrB = data;
}


static unsigned char portC_in,portC_out,ddrC;

READ_HANDLER( flstory_68705_portC_r )
{
	portC_in = 0;
	if (main_sent) portC_in |= 0x01;
	if (!mcu_sent) portC_in |= 0x02;
//logerror("%04x: 68705 port C read %02x\n",activecpu_get_pc(),portC_in);
	return (portC_out & ddrC) | (portC_in & ~ddrC);
}

WRITE_HANDLER( flstory_68705_portC_w )
{
logerror("%04x: 68705 port C write %02x\n",activecpu_get_pc(),data);
	portC_out = data;
}

WRITE_HANDLER( flstory_68705_ddrC_w )
{
	ddrC = data;
}

WRITE_HANDLER( flstory_mcu_w )
{
logerror("%04x: mcu_w %02x\n",activecpu_get_pc(),data);
	from_main = data;
	main_sent = 1;
	cpu_set_irq_line(2,0,ASSERT_LINE);
}

READ_HANDLER( flstory_mcu_r )
{
logerror("%04x: mcu_r %02x\n",activecpu_get_pc(),from_mcu);
	mcu_sent = 0;
	return from_mcu;
}

READ_HANDLER( flstory_mcu_status_r )
{
	int res = 0;

	/* bit 0 = when 1, mcu is ready to receive data from main cpu */
	/* bit 1 = when 1, mcu has sent data to the main cpu */
//logerror("%04x: mcu_status_r\n",activecpu_get_pc());
	if (!main_sent) res |= 0x01;
	if (mcu_sent) res |= 0x02;

	return res;
}

WRITE_HANDLER( onna34ro_mcu_w )
{
	data8_t *RAM = memory_region(REGION_CPU1);
	UINT16 score_adr = RAM[0xe29e]*0x100 + RAM[0xe29d];

	switch (data)
	{
		case 0x0e:
			from_mcu = 0xff;
			break;
		case 0x01:
			from_mcu = 0x6a;
			break;
		case 0x40:
			from_mcu = RAM[score_adr];			/* score l*/
			break;
		case 0x41:
			from_mcu = RAM[score_adr+1];		/* score m*/
			break;
		case 0x42:
			from_mcu = RAM[score_adr+2] & 0x0f;	/* score h*/
			break;
		default:
			from_mcu = 0x80;
	}
}

READ_HANDLER( onna34ro_mcu_r )
{
	return from_mcu;
}

READ_HANDLER( onna34ro_mcu_status_r )
{
	int res = 3;

	return res;
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
