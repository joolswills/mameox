#pragma code_seg("C109")
#pragma data_seg("D109")
#pragma bss_seg("B109")
#pragma const_seg("K109")
#pragma comment(linker, "/merge:D109=109")
#pragma comment(linker, "/merge:C109=109")
#pragma comment(linker, "/merge:B109=109")
#pragma comment(linker, "/merge:K109=109")
/*
 *	Chack'n Pop (C) 1983 TAITO Corp.
 *	simulate 68705 MCU
 */

#include "driver.h"
#include "cpu/z80/z80.h"
#include "state.h"

#define MCU_INITIAL_SEED	0x81

static data8_t mcu_seed;
static data8_t mcu_select;
static data8_t mcu_result;
static data8_t mcu_wait;

/* mcu data that is extracted from the real board! */
/* updated on 2st Jun 2003 */
static data8_t mcu_data[256] = {
	0x3a, 0xe6, 0x80, 0xc6, 0x0e, 0xdd, 0x77, 0xfd,
	0x7e, 0xfe, 0x10, 0x38, 0x10, 0xdd, 0x7e, 0x03,
	0xc6, 0x08, 0xdd, 0x77, 0x03, 0xdd, 0x7e, 0xff,
	0xc6, 0x08, 0xdd, 0x77, 0xff, 0x34, 0x23, 0xc9,
	0x06, 0x10, 0xdd, 0xe5, 0xe1, 0x23, 0x23, 0x23,
	0x4e, 0x23, 0x5e, 0x23, 0x56, 0x23, 0x23, 0x7b,
	0xb2, 0x28, 0x02, 0x79, 0x12, 0x10, 0xf1, 0xc9,
	0xe6, 0x03, 0x87, 0x87, 0x6f, 0x26, 0x00, 0x29,
	0x29, 0x29, 0xe5, 0x29, 0x29, 0x29, 0xc1, 0x09,
	0x01, 0xc3, 0x81, 0x09, 0xe5, 0xdd, 0xe1, 0xc9,
	0x10, 0x00, 0x08, 0x10, 0x20, 0x64, 0x50, 0x00,
	0x08, 0x50, 0xb0, 0x01, 0x34, 0xa0, 0x13, 0x34,
	0xb0, 0x05, 0x34, 0xc0, 0x04, 0x34, 0xd0, 0x02,
	0x34, 0xf0, 0x02, 0x34, 0x00, 0x60, 0x00, 0x00,
	0x3f, 0x00, 0x0c, 0x1f, 0xa0, 0x3f, 0x1e, 0xa2,
	0x01, 0x1e, 0xa1, 0x0a, 0x1e, 0xa2, 0x07, 0x1e,
	0x92, 0x05, 0x1e, 0x02, 0x04, 0x1e, 0x12, 0x09,
	0x3f, 0x22, 0x06, 0x3f, 0x21, 0x03, 0x3f, 0x20,
	0x02, 0x00, 0x00, 0x3f, 0x00, 0x04, 0x02, 0xa0,
	0x40, 0x12, 0xa1, 0x06, 0x12, 0xa2, 0x02, 0x12,
	0xa1, 0x0a, 0x12, 0xa2, 0x07, 0x10, 0x92, 0x05,
	0x10, 0x02, 0x04, 0x12, 0x12, 0x09, 0x12, 0x22,
	0x06, 0x12, 0x21, 0x03, 0x12, 0x20, 0x02, 0x26,
	0x00, 0x14, 0x12, 0x00, 0x00, 0x00, 0x3f, 0x00,
	0x04, 0x1a, 0xa0, 0x40, 0x3f, 0x00, 0x00, 0x00,
	0x3e, 0x3a, 0x87, 0x83, 0x3c, 0x32, 0x87, 0x83,
	0x0f, 0x0f, 0xe6, 0x07, 0xfe, 0x02, 0x20, 0x01,
	0xaf, 0x11, 0x40, 0x98, 0x1d, 0x12, 0x1d, 0x20,
	0xfb, 0x2a, 0x89, 0x83, 0x2b, 0x22, 0x89, 0x83,
	0xc9, 0x3a, 0x5b, 0x81, 0xa7, 0xc0, 0x21, 0x80,
	0x81, 0x11, 0x04, 0x00, 0x06, 0x09, 0x34, 0x19,
	0x10, 0xfc, 0x3e, 0x01, 0x32, 0x5b, 0x81, 0xc9
};

static void mcu_update_seed(data8_t data)
{
	if (!(data & 0x80))
	{
		mcu_seed += 0x83;
		mcu_seed = (mcu_seed & 0x80) | (mcu_seed >> 1);
	}

	mcu_seed += 0x19;
	//logerror("New seed: 0x%02x\n", mcu_seed);
}


/***************************************************************************
  Memory handlers
***************************************************************************/

READ_HANDLER( chaknpop_mcu_portA_r )
{
	//logerror("%04x: MCU portA read\n", activecpu_get_pc());
	return mcu_result;
}


READ_HANDLER( chaknpop_mcu_portB_r )
{
	//logerror("%04x: MCU portB read\n", activecpu_get_pc());

	if (--mcu_wait)
		return 0x00;

	return 0xff;
}

READ_HANDLER( chaknpop_mcu_portC_r )
{
	//logerror("%04x: MCU portC read\n", activecpu_get_pc());
	return 0x00;
}

WRITE_HANDLER( chaknpop_mcu_portA_w )
{
	data8_t *RAM = memory_region(REGION_CPU1);
	data8_t mcu_command;

	mcu_command = data + mcu_seed;
	mcu_result = 0;

	if (mcu_command < 0x08)
	{
		mcu_update_seed(data);

		mcu_result = mcu_data[mcu_select * 8 + mcu_command];
		mcu_result -=  mcu_seed;

		mcu_update_seed(mcu_result);

		logerror("%04x: MCU command 0x%02x, result 0x%02x\n", activecpu_get_pc(), mcu_command, mcu_result);
	}
	else if (mcu_command >= 0x28 && mcu_command <= 0x2a)
	{
		mcu_update_seed(data);

		mcu_result = RAM[0x8380 + mcu_command];
		mcu_result -=  mcu_seed;

		mcu_update_seed(mcu_result);

		logerror("%04x: MCU command 0x%02x, result 0x%02x\n", activecpu_get_pc(), mcu_command, mcu_result);
	}
	else if (mcu_command < 0x80)
	{
		mcu_update_seed(data);

		if (mcu_command >= 0x40 && mcu_command < 0x60)
		{
			mcu_select = mcu_command - 0x40;

			logerror("%04x: MCU select 0x%02x\n", activecpu_get_pc(), mcu_select);
		}
	}
	else if (mcu_command == 0x9c|| mcu_command == 0xde)
	{
		mcu_update_seed(data);

		logerror("%04x: MCU command 0x%02x\n", activecpu_get_pc(), mcu_command);
	}
}

WRITE_HANDLER( chaknpop_mcu_portB_w )
{
	//logerror("%04x: MCU portB write 0x%02x\n", activecpu_get_pc(), data);
}

WRITE_HANDLER( chaknpop_mcu_portC_w )
{
	//logerror("%04x: MCU portC write 0x%02x\n", activecpu_get_pc(), data);
}


/***************************************************************************
  Initialize mcu emulation
***************************************************************************/

DRIVER_INIT( chaknpop )
{
	state_save_register_UINT8("chankpop", 0, "mcu_seed",    &mcu_seed,    1);
	state_save_register_UINT8("chankpop", 0, "mcu_result",  &mcu_result,  1);
	state_save_register_UINT8("chankpop", 0, "mcu_select",  &mcu_select,  1);
	state_save_register_UINT8("chankpop", 0, "mcu_wait",    &mcu_wait,    1);
}

MACHINE_INIT( chaknpop )
{
	mcu_seed = MCU_INITIAL_SEED;
	mcu_wait = 0;
}
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
