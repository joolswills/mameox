#pragma code_seg("C274")
#pragma data_seg("D274")
#pragma bss_seg("B274")
#pragma const_seg("K274")
#pragma comment(linker, "/merge:D274=274")
#pragma comment(linker, "/merge:C274=274")
#pragma comment(linker, "/merge:B274=274")
#pragma comment(linker, "/merge:K274=274")
/******************************************************************************

	Game Driver for Video System Mahjong series. (Preliminary driver)

	Taisen Idol-Mahjong Final Romance 2
	(c)1995 Video System Co.,Ltd.

	Taisen Mahjong FinalRomance R
	(c)1995 Video System Co.,Ltd.

	Taisen Mahjong FinalRomance 4
	(c)1998 Video System Co.,Ltd.

	Driver by Takahiro Nogi <nogi@kt.rim.or.jp> 2001/02/28 -
	Special very thanks to Uki.

******************************************************************************/
/******************************************************************************
Memo:

******************************************************************************/

#include "driver.h"
#include "cpu/m68000/m68000.h"
#include "cpu/z80/z80.h"
#include "vidhrdw/generic.h"
#include "machine/eeprom.h"


VIDEO_UPDATE( fromanc2 );
VIDEO_START( fromanc2 );
VIDEO_START( fromancr );
VIDEO_START( fromanc4 );

READ16_HANDLER( fromanc2_paletteram_0_r );
READ16_HANDLER( fromanc2_paletteram_1_r );
WRITE16_HANDLER( fromanc2_paletteram_0_w );
WRITE16_HANDLER( fromanc2_paletteram_1_w );
READ16_HANDLER( fromancr_paletteram_0_r );
READ16_HANDLER( fromancr_paletteram_1_r );
WRITE16_HANDLER( fromancr_paletteram_0_w );
WRITE16_HANDLER( fromancr_paletteram_1_w );
READ16_HANDLER( fromanc4_paletteram_0_r );
READ16_HANDLER( fromanc4_paletteram_1_r );
WRITE16_HANDLER( fromanc4_paletteram_0_w );
WRITE16_HANDLER( fromanc4_paletteram_1_w );
WRITE16_HANDLER( fromanc2_videoram_0_w );
WRITE16_HANDLER( fromanc2_videoram_1_w );
WRITE16_HANDLER( fromanc2_videoram_2_w );
WRITE16_HANDLER( fromanc2_videoram_3_w );
WRITE16_HANDLER( fromancr_videoram_0_w );
WRITE16_HANDLER( fromancr_videoram_1_w );
WRITE16_HANDLER( fromancr_videoram_2_w );
WRITE16_HANDLER( fromanc4_videoram_0_w );
WRITE16_HANDLER( fromanc4_videoram_1_w );
WRITE16_HANDLER( fromanc4_videoram_2_w );
WRITE16_HANDLER( fromanc2_gfxreg_0_w );
WRITE16_HANDLER( fromanc2_gfxreg_1_w );
WRITE16_HANDLER( fromanc2_gfxreg_2_w );
WRITE16_HANDLER( fromanc2_gfxreg_3_w );
WRITE16_HANDLER( fromancr_gfxreg_0_w );
WRITE16_HANDLER( fromancr_gfxreg_1_w );
WRITE16_HANDLER( fromanc2_gfxbank_0_w );
WRITE16_HANDLER( fromanc2_gfxbank_1_w );
void fromancr_gfxbank_w(int data);
WRITE16_HANDLER( fromanc4_gfxreg_0_w );
WRITE16_HANDLER( fromanc4_gfxreg_1_w );
WRITE16_HANDLER( fromanc4_gfxreg_2_w );

void fromanc2_set_dispvram_w(int vram);


static int fromanc2_playerside;
static int fromanc2_portselect;
static UINT16 fromanc2_datalatch1;
static UINT8 fromanc2_datalatch_2h, fromanc2_datalatch_2l;
static UINT8 fromanc2_subcpu_int_flag;
static UINT8 fromanc2_subcpu_nmi_flag;
static UINT8 fromanc2_sndcpu_nmi_flag;


// ----------------------------------------------------------------------------
// 	MACHINE INITIALYZE
// ----------------------------------------------------------------------------

static MACHINE_INIT( fromanc2 )
{
	//
}

static MACHINE_INIT( fromancr )
{
	//
}

static MACHINE_INIT( fromanc4 )
{
	//
}


static DRIVER_INIT( fromanc2 )
{
	fromanc2_subcpu_nmi_flag = 1;
	fromanc2_subcpu_int_flag = 1;
	fromanc2_sndcpu_nmi_flag = 1;

	fromanc2_playerside = 0;
}

static DRIVER_INIT( fromancr )
{
	fromanc2_subcpu_nmi_flag = 1;
	fromanc2_subcpu_int_flag = 1;
	fromanc2_sndcpu_nmi_flag = 1;

	fromanc2_playerside = 0;
}

static DRIVER_INIT( fromanc4 )
{
	fromanc2_sndcpu_nmi_flag = 1;

	fromanc2_playerside = 0;
}


// ----------------------------------------------------------------------------
// 	MAIN CPU Interrupt (fromanc2, fromancr, fromanc4)	TEST ROUTINE
// ----------------------------------------------------------------------------

static INTERRUPT_GEN( fromanc2_interrupt )
{
	static int fromanc2_playerside_old = -1;
	static int key_F1_old = 0;

	if (keyboard_pressed(KEYCODE_F1))
	{
		if (key_F1_old != 1)
		{
			key_F1_old = 1;
			fromanc2_playerside ^= 1;
		}
	} else key_F1_old = 0;

	if (fromanc2_playerside_old != fromanc2_playerside)
	{
		fromanc2_playerside_old = fromanc2_playerside;

		usrintf_showmessage("PLAYER-%01X SIDE", (fromanc2_playerside + 1));

		if (!fromanc2_playerside)
		{
			mixer_set_stereo_volume(3, 75, 75);	// 1P (LEFT)
			mixer_set_stereo_volume(4,  0,  0);	// 2P (RIGHT)
		}
		else
		{
			mixer_set_stereo_volume(3,  0,  0);	// 1P (LEFT)
			mixer_set_stereo_volume(4, 75, 75);	// 2P (RIGHT)
		}

		fromanc2_set_dispvram_w(fromanc2_playerside);
	}

	cpu_set_irq_line(0, 1, HOLD_LINE);
}


// ----------------------------------------------------------------------------
// 	Sound Command Interface (fromanc2, fromancr, fromanc4)
// ----------------------------------------------------------------------------

static WRITE16_HANDLER( fromanc2_sndcmd_w )
{
	soundlatch_w(offset, ((data >> 8) & 0xff));	// 1P (LEFT)
	soundlatch2_w(offset, (data & 0xff));		// 2P (RIGHT)

	cpu_set_irq_line(1, IRQ_LINE_NMI, PULSE_LINE);
	fromanc2_sndcpu_nmi_flag = 0;
}

// ----------------------------------------------------------------------------
// 	Input Port Interface (COIN, TEST, KEY MATRIX, EEPROM)
// ----------------------------------------------------------------------------

static WRITE16_HANDLER( fromanc2_portselect_w )
{
	fromanc2_portselect = data;
}

static READ16_HANDLER( fromanc2_keymatrix_r )
{
	UINT16 ret;

	switch (fromanc2_portselect)
	{
		case	0x01:	ret = readinputport(1);	break;
		case	0x02:	ret = readinputport(2); break;
		case	0x04:	ret = readinputport(3); break;
		case	0x08:	ret = readinputport(4); break;
		default:	ret = 0xffff;
				logerror("PC:%08X unknown %02X\n", activecpu_get_pc(), fromanc2_portselect);
				break;
	}

	if (fromanc2_playerside) ret = (((ret & 0xff00) >> 8) | ((ret & 0x00ff) << 8));

	return ret;
}

static READ16_HANDLER( fromanc2_input_r )
{
	UINT16 cflag, coinsw, eeprom;

	cflag = (((fromanc2_subcpu_int_flag & 1) << 4) |
		 ((fromanc2_subcpu_nmi_flag & 1) << 6) |
		 ((fromanc2_sndcpu_nmi_flag & 1) << 5));
	eeprom = ((EEPROM_read_bit() & 1) << 7);	// EEPROM DATA
	coinsw = (readinputport(0) & 0x030f);		// COIN, TEST

	return (cflag | eeprom | coinsw);
}

static READ16_HANDLER( fromanc4_input_r )
{
	UINT16 cflag, coinsw, eeprom;

	cflag = ((fromanc2_sndcpu_nmi_flag & 1) << 5);
	eeprom = ((EEPROM_read_bit() & 1) << 7);	// EEPROM DATA
	coinsw = (readinputport(0) & 0x001f);		// COIN, TEST

	return (cflag | eeprom | coinsw);
}

static WRITE16_HANDLER( fromanc2_eeprom_w )
{
	if (ACCESSING_MSB)
	{
		// latch the bit
		EEPROM_write_bit(data & 0x0100);

		// reset line asserted: reset.
		EEPROM_set_cs_line((data & 0x0400) ? CLEAR_LINE : ASSERT_LINE);

		// clock line asserted: write latch or select next bit to read
		EEPROM_set_clock_line((data & 0x0200) ? ASSERT_LINE : CLEAR_LINE);
	}
}

static WRITE16_HANDLER( fromancr_eeprom_w )
{
	if (ACCESSING_LSB)
	{
		fromancr_gfxbank_w(data & 0xfff8);

		// latch the bit
		EEPROM_write_bit(data & 0x0001);

		// reset line asserted: reset.
		EEPROM_set_cs_line((data & 0x0004) ? CLEAR_LINE : ASSERT_LINE);

		// clock line asserted: write latch or select next bit to read
		EEPROM_set_clock_line((data & 0x0002) ? ASSERT_LINE : CLEAR_LINE);
	}
}

static WRITE16_HANDLER( fromanc4_eeprom_w )
{
	if (ACCESSING_LSB)
	{
		// latch the bit
		EEPROM_write_bit(data & 0x0004);

		// reset line asserted: reset.
		EEPROM_set_cs_line((data & 0x0001) ? CLEAR_LINE : ASSERT_LINE);

		// clock line asserted: write latch or select next bit to read
		EEPROM_set_clock_line((data & 0x0002) ? ASSERT_LINE : CLEAR_LINE);
	}
}

// ----------------------------------------------------------------------------
// 	MAIN CPU, SUB CPU Communication Interface (fromanc2, fromancr)
// ----------------------------------------------------------------------------

static WRITE16_HANDLER( fromanc2_subcpu_w )
{
	fromanc2_datalatch1 = data;

	cpu_set_irq_line(2, 0, HOLD_LINE);
	fromanc2_subcpu_int_flag = 0;
}

static READ16_HANDLER( fromanc2_subcpu_r )
{
	cpu_set_irq_line(2, IRQ_LINE_NMI, PULSE_LINE);
	fromanc2_subcpu_nmi_flag = 0;

	return ((fromanc2_datalatch_2h << 8) | fromanc2_datalatch_2l);
}

static READ_HANDLER( fromanc2_maincpu_r_l )
{
	return (fromanc2_datalatch1 & 0x00ff);
}

static READ_HANDLER( fromanc2_maincpu_r_h )
{
	fromanc2_subcpu_int_flag = 1;

	return ((fromanc2_datalatch1 & 0xff00) >> 8);
}

static WRITE_HANDLER( fromanc2_maincpu_w_l )
{
	fromanc2_datalatch_2l = data;
}

static WRITE_HANDLER( fromanc2_maincpu_w_h )
{
	fromanc2_datalatch_2h = data;
}

static WRITE_HANDLER( fromanc2_subcpu_nmi_clr )
{
	fromanc2_subcpu_nmi_flag = 1;
}

static READ_HANDLER( fromanc2_sndcpu_nmi_clr )
{
	fromanc2_sndcpu_nmi_flag = 1;

	return 0xff;
}

static WRITE_HANDLER( fromanc2_subcpu_rombank_w )
{
	UINT8 *RAM = memory_region(REGION_CPU3);
	int rombank = data & 0x03;
	int rambank = (data & 0x0c) >> 2;

	// Change ROM BANK
	cpu_setbank(1, &RAM[rombank * 0x4000]);

	// Change RAM BANK
	if (rambank != 0) cpu_setbank(2, &RAM[0x10000 + (rambank * 0x4000)]);
	else cpu_setbank(2, &RAM[0x8000]);
}


// ----------------------------------------------------------------------------
//	MAIN Program (fromanc2, fromancr, fromanc4)
// ----------------------------------------------------------------------------

static MEMORY_READ16_START( fromanc2_readmem_main )
	{ 0x000000, 0x07ffff, MRA16_ROM },		// MAIN ROM

	{ 0x802000, 0x802fff, MRA16_NOP },		// ???

	{ 0xa00000, 0xa00fff, fromanc2_paletteram_0_r },// PALETTE (1P)
	{ 0xa80000, 0xa80fff, fromanc2_paletteram_1_r },// PALETTE (2P)

	{ 0xd01100, 0xd01101, fromanc2_input_r },	// INPUT COMMON, EEPROM
	{ 0xd01300, 0xd01301, fromanc2_subcpu_r },	// SUB CPU READ
	{ 0xd01800, 0xd01801, fromanc2_keymatrix_r },	// INPUT KEY MATRIX

	{ 0xd80000, 0xd8ffff, MRA16_RAM },		// WORK RAM
MEMORY_END

static MEMORY_WRITE16_START( fromanc2_writemem_main )
	{ 0x000000, 0x07ffff, MWA16_ROM },		// MAIN ROM

	{ 0x800000, 0x803fff, fromanc2_videoram_0_w },	// VRAM 0, 1 (1P)
	{ 0x880000, 0x883fff, fromanc2_videoram_1_w },	// VRAM 2, 3 (1P)
	{ 0x900000, 0x903fff, fromanc2_videoram_2_w },	// VRAM 0, 1 (2P)
	{ 0x980000, 0x983fff, fromanc2_videoram_3_w },	// VRAM 2, 3 (2P)

	{ 0xa00000, 0xa00fff, fromanc2_paletteram_0_w },// PALETTE (1P)
	{ 0xa80000, 0xa80fff, fromanc2_paletteram_1_w },// PALETTE (2P)

	{ 0xd00000, 0xd00023, fromanc2_gfxreg_0_w },	// SCROLL REG (1P/2P)
	{ 0xd00100, 0xd00123, fromanc2_gfxreg_2_w },	// SCROLL REG (1P/2P)
	{ 0xd00200, 0xd00223, fromanc2_gfxreg_1_w },	// SCROLL REG (1P/2P)
	{ 0xd00300, 0xd00323, fromanc2_gfxreg_3_w },	// SCROLL REG (1P/2P)

	{ 0xd00400, 0xd00413, MWA16_NOP },		// ???
	{ 0xd00500, 0xd00513, MWA16_NOP },		// ???

	{ 0xd01000, 0xd01001, fromanc2_sndcmd_w },	// SOUND REQ (1P/2P)
	{ 0xd01200, 0xd01201, fromanc2_subcpu_w },	// SUB CPU WRITE
	{ 0xd01400, 0xd01401, fromanc2_gfxbank_0_w },	// GFXBANK (1P)
	{ 0xd01500, 0xd01501, fromanc2_gfxbank_1_w },	// GFXBANK (2P)
	{ 0xd01600, 0xd01601, fromanc2_eeprom_w },	// EEPROM DATA
	{ 0xd01a00, 0xd01a01, fromanc2_portselect_w },	// PORT SELECT (1P/2P)

	{ 0xd80000, 0xd8ffff, MWA16_RAM },		// WORK RAM
MEMORY_END

static MEMORY_READ16_START( fromancr_readmem_main )
	{ 0x000000, 0x07ffff, MRA16_ROM },		// MAIN ROM

	{ 0xa00000, 0xa00fff, fromancr_paletteram_0_r },// PALETTE (1P)
	{ 0xa80000, 0xa80fff, fromancr_paletteram_1_r },// PALETTE (2P)

	{ 0xd01100, 0xd01101, fromanc2_input_r },	// INPUT COMMON, EEPROM
	{ 0xd01300, 0xd01301, fromanc2_subcpu_r },	// SUB CPU READ
	{ 0xd01800, 0xd01801, fromanc2_keymatrix_r },	// INPUT KEY MATRIX

	{ 0xd80000, 0xd8ffff, MRA16_RAM },		// WORK RAM
MEMORY_END

static MEMORY_WRITE16_START( fromancr_writemem_main )
	{ 0x000000, 0x07ffff, MWA16_ROM },		// MAIN ROM

	{ 0x800000, 0x803fff, fromancr_videoram_0_w },	// VRAM BG (1P/2P)
	{ 0x880000, 0x883fff, fromancr_videoram_1_w },	// VRAM FG (1P/2P)
	{ 0x900000, 0x903fff, fromancr_videoram_2_w },	// VRAM TEXT (1P/2P)
	{ 0x980000, 0x983fff, MWA16_NOP },		// VRAM Unused ?

	{ 0xa00000, 0xa00fff, fromancr_paletteram_0_w },// PALETTE (1P)
	{ 0xa80000, 0xa80fff, fromancr_paletteram_1_w },// PALETTE (2P)

	{ 0xd00000, 0xd00023, fromancr_gfxreg_1_w },	// SCROLL REG (1P/2P)
	{ 0xd00100, 0xd00123, fromancr_gfxreg_0_w },	// SCROLL REG (1P/2P)

	{ 0xd00200, 0xd002ff, MWA16_NOP },		// ?

	{ 0xd00400, 0xd00413, MWA16_NOP },		// ???
	{ 0xd00500, 0xd00513, MWA16_NOP },		// ???

	{ 0xd01000, 0xd01001, fromanc2_sndcmd_w },	// SOUND REQ (1P/2P)
	{ 0xd01200, 0xd01201, fromanc2_subcpu_w },	// SUB CPU WRITE
	{ 0xd01400, 0xd01401, MWA16_NOP },		// COIN COUNTER ?
	{ 0xd01600, 0xd01601, fromancr_eeprom_w },	// EEPROM DATA, GFXBANK (1P/2P)
	{ 0xd01a00, 0xd01a01, fromanc2_portselect_w },	// PORT SELECT (1P/2P)

	{ 0xd80000, 0xd8ffff, MWA16_RAM },		// WORK RAM
MEMORY_END

static MEMORY_READ16_START( fromanc4_readmem_main )
	{ 0x000000, 0x07ffff, MRA16_ROM },		// MAIN ROM
	{ 0x400000, 0x7fffff, MRA16_ROM },		// DATA ROM

	{ 0x800000, 0x81ffff, MRA16_RAM },		// WORK RAM

	{ 0xdb0000, 0xdb0fff, fromanc4_paletteram_0_r },// PALETTE (1P)
	{ 0xdc0000, 0xdc0fff, fromanc4_paletteram_1_r },// PALETTE (2P)

	{ 0xd10000, 0xd10001, fromanc2_keymatrix_r },	// INPUT KEY MATRIX
	{ 0xd20000, 0xd20001, fromanc4_input_r },	// INPUT COMMON, EEPROM DATA

	{ 0xe5000c, 0xe5000d, MRA16_NOP },		// EXT-COMM PORT ?
MEMORY_END

static MEMORY_WRITE16_START( fromanc4_writemem_main )
	{ 0x000000, 0x07ffff, MWA16_ROM },		// MAIN ROM
	{ 0x400000, 0x7fffff, MWA16_ROM },		// DATA ROM

	{ 0x800000, 0x81ffff, MWA16_RAM },		// WORK RAM

	{ 0xd00000, 0xd00001, fromanc2_portselect_w },	// PORT SELECT (1P/2P)

	{ 0xd10000, 0xd10001, MWA16_NOP },		// ?
	{ 0xd30000, 0xd30001, MWA16_NOP },		// ?
	{ 0xd50000, 0xd50001, fromanc4_eeprom_w },	// EEPROM DATA

	{ 0xd70000, 0xd70001, fromanc2_sndcmd_w },	// SOUND REQ (1P/2P)

	{ 0xd80000, 0xd8ffff, fromanc4_videoram_0_w },	// VRAM FG (1P/2P)
	{ 0xd90000, 0xd9ffff, fromanc4_videoram_1_w },	// VRAM BG (1P/2P)
	{ 0xda0000, 0xdaffff, fromanc4_videoram_2_w },	// VRAM TEXT (1P/2P)

	{ 0xdb0000, 0xdb0fff, fromanc4_paletteram_0_w },// PALETTE (1P)
	{ 0xdc0000, 0xdc0fff, fromanc4_paletteram_1_w },// PALETTE (2P)

	{ 0xe00000, 0xe0001d, fromanc4_gfxreg_0_w },	// SCROLL, GFXBANK (1P/2P)
	{ 0xe10000, 0xe1001d, fromanc4_gfxreg_1_w },	// SCROLL, GFXBANK (1P/2P)
	{ 0xe20000, 0xe2001d, fromanc4_gfxreg_2_w },	// SCROLL, GFXBANK (1P/2P)

	{ 0xe30000, 0xe30013, MWA16_NOP },		// ???
	{ 0xe40000, 0xe40013, MWA16_NOP },		// ???

	{ 0xe50000, 0xe50009, MWA16_NOP },		// EXT-COMM PORT ?
MEMORY_END


// ----------------------------------------------------------------------------
// 	Z80 SUB Program (fromanc2, fromancr)
// ----------------------------------------------------------------------------

static MEMORY_READ_START( fromanc2_readmem_sub )
	{ 0x0000, 0x3fff, MRA_ROM },			// ROM
	{ 0x4000, 0x7fff, MRA_BANK1 },			// ROM(BANK)
	{ 0x8000, 0xbfff, MRA_RAM },			// RAM(WORK)
	{ 0xc000, 0xffff, MRA_BANK2 },			// RAM(BANK)
MEMORY_END

static MEMORY_WRITE_START( fromanc2_writemem_sub )
	{ 0x0000, 0x3fff, MWA_ROM },			// ROM
	{ 0x4000, 0x7fff, MWA_BANK1 },			// ROM(BANK)
	{ 0x8000, 0xbfff, MWA_RAM },			// RAM(WORK)
	{ 0xc000, 0xffff, MWA_BANK2 },			// RAM(BANK)
MEMORY_END

static PORT_READ_START( fromanc2_readport_sub )
	{ 0x02, 0x02, fromanc2_maincpu_r_l },		// to MAIN CPU
	{ 0x04, 0x04, fromanc2_maincpu_r_h },		// to MAIN CPU
PORT_END

static PORT_WRITE_START( fromanc2_writeport_sub )
	{ 0x00, 0x00, fromanc2_subcpu_rombank_w },
	{ 0x02, 0x02, fromanc2_maincpu_w_l },		// from MAIN CPU
	{ 0x04, 0x04, fromanc2_maincpu_w_h },		// from MAIN CPU
	{ 0x06, 0x06, fromanc2_subcpu_nmi_clr },
PORT_END


// ----------------------------------------------------------------------------
// 	Z80 Sound Program (fromanc2, fromancr, fromanc4)
// ----------------------------------------------------------------------------

static MEMORY_READ_START( fromanc2_readmem_sound )
	{ 0x0000, 0xdfff, MRA_ROM },
	{ 0xe000, 0xffff, MRA_RAM },
MEMORY_END

static MEMORY_WRITE_START( fromanc2_writemem_sound )
	{ 0x0000, 0xdfff, MWA_ROM },
	{ 0xe000, 0xffff, MWA_RAM },
MEMORY_END

static PORT_READ_START( fromanc2_readport_sound )
	{ 0x00, 0x00, soundlatch_r },				// snd cmd (1P)
	{ 0x04, 0x04, soundlatch2_r },				// snd cmd (2P)
	{ 0x09, 0x09, IORP_NOP },				// ?
	{ 0x08, 0x08, YM2610_status_port_0_A_r },
	{ 0x0a, 0x0a, YM2610_status_port_0_B_r },
	{ 0x0c, 0x0c, fromanc2_sndcpu_nmi_clr },
PORT_END

static PORT_WRITE_START( fromanc2_writeport_sound )
	{ 0x00, 0x00, IOWP_NOP },				// ?
	{ 0x08, 0x08, YM2610_control_port_0_A_w },
	{ 0x09, 0x09, YM2610_data_port_0_A_w },
	{ 0x0a, 0x0a, YM2610_control_port_0_B_w },
	{ 0x0b, 0x0b, YM2610_data_port_0_B_w },
PORT_END


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

#define VSYSMJCTRL_PORT1 \
	PORT_START	/* (1) PORT 0 */ \
	PORT_BITX( 0x0001, IP_ACTIVE_LOW, 0, "P1 A", KEYCODE_A, IP_JOY_NONE ) \
	PORT_BITX( 0x0002, IP_ACTIVE_LOW, 0, "P1 E", KEYCODE_E, IP_JOY_NONE ) \
	PORT_BITX( 0x0004, IP_ACTIVE_LOW, 0, "P1 I", KEYCODE_I, IP_JOY_NONE ) \
	PORT_BITX( 0x0008, IP_ACTIVE_LOW, 0, "P1 M", KEYCODE_M, IP_JOY_NONE ) \
	PORT_BITX( 0x0010, IP_ACTIVE_LOW, 0, "P1 Kan", KEYCODE_LCONTROL, IP_JOY_NONE ) \
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_START1 ) \
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BITX( 0x0100, IP_ACTIVE_LOW, 0, "P2 A", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0200, IP_ACTIVE_LOW, 0, "P2 E", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0400, IP_ACTIVE_LOW, 0, "P2 I", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0800, IP_ACTIVE_LOW, 0, "P2 M", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x1000, IP_ACTIVE_LOW, 0, "P2 Kan", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_START2 ) \
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

#define VSYSMJCTRL_PORT2 \
	PORT_START	/* (2) PORT 1 */ \
	PORT_BITX( 0x0001, IP_ACTIVE_LOW, 0, "P1 B", KEYCODE_B, IP_JOY_NONE ) \
	PORT_BITX( 0x0002, IP_ACTIVE_LOW, 0, "P1 F", KEYCODE_F, IP_JOY_NONE ) \
	PORT_BITX( 0x0004, IP_ACTIVE_LOW, 0, "P1 J", KEYCODE_J, IP_JOY_NONE ) \
	PORT_BITX( 0x0008, IP_ACTIVE_LOW, 0, "P1 N", KEYCODE_N, IP_JOY_NONE ) \
	PORT_BITX( 0x0010, IP_ACTIVE_LOW, 0, "P1 Reach", KEYCODE_LSHIFT, IP_JOY_NONE ) \
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BITX( 0x0100, IP_ACTIVE_LOW, 0, "P2 B", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0200, IP_ACTIVE_LOW, 0, "P2 F", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0400, IP_ACTIVE_LOW, 0, "P2 J", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0800, IP_ACTIVE_LOW, 0, "P2 N", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x1000, IP_ACTIVE_LOW, 0, "P2 Reach", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

#define VSYSMJCTRL_PORT3 \
	PORT_START	/* (3) PORT 2 */ \
	PORT_BITX( 0x0001, IP_ACTIVE_LOW, 0, "P1 C", KEYCODE_C, IP_JOY_NONE ) \
	PORT_BITX( 0x0002, IP_ACTIVE_LOW, 0, "P1 G", KEYCODE_G, IP_JOY_NONE ) \
	PORT_BITX( 0x0004, IP_ACTIVE_LOW, 0, "P1 K", KEYCODE_K, IP_JOY_NONE ) \
	PORT_BITX( 0x0008, IP_ACTIVE_LOW, 0, "P1 Chi", KEYCODE_SPACE, IP_JOY_NONE ) \
	PORT_BITX( 0x0010, IP_ACTIVE_LOW, 0, "P1 Ron", KEYCODE_Z, IP_JOY_NONE ) \
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BITX( 0x0100, IP_ACTIVE_LOW, 0, "P2 C", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0200, IP_ACTIVE_LOW, 0, "P2 G", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0400, IP_ACTIVE_LOW, 0, "P2 K", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0800, IP_ACTIVE_LOW, 0, "P2 Chi", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x1000, IP_ACTIVE_LOW, 0, "P2 Ron", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

#define VSYSMJCTRL_PORT4 \
	PORT_START	/* (4) PORT 3 */ \
	PORT_BITX( 0x0001, IP_ACTIVE_LOW, 0, "P1 D", KEYCODE_D, IP_JOY_NONE ) \
	PORT_BITX( 0x0002, IP_ACTIVE_LOW, 0, "P1 H", KEYCODE_H, IP_JOY_NONE ) \
	PORT_BITX( 0x0004, IP_ACTIVE_LOW, 0, "P1 L", KEYCODE_L, IP_JOY_NONE ) \
	PORT_BITX( 0x0008, IP_ACTIVE_LOW, 0, "P1 Pon", KEYCODE_LALT, IP_JOY_NONE ) \
	PORT_BIT ( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BITX( 0x0100, IP_ACTIVE_LOW, 0, "P2 D", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0200, IP_ACTIVE_LOW, 0, "P2 H", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0400, IP_ACTIVE_LOW, 0, "P2 L", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BITX( 0x0800, IP_ACTIVE_LOW, 0, "P2 Pon", IP_KEY_DEFAULT, IP_JOY_NONE ) \
	PORT_BIT ( 0x1000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN ) \
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

INPUT_PORTS_START( fromanc2 )
	PORT_START	/* (0) COIN SW, TEST SW, EEPROM DATA, etc */
	PORT_BIT ( 0x0001, IP_ACTIVE_LOW, IPT_COIN1 )		// COIN1 (1P)
	PORT_BIT ( 0x0002, IP_ACTIVE_LOW, IPT_COIN2 )		// COIN2 (1P)
	PORT_BIT ( 0x0004, IP_ACTIVE_LOW, IPT_COIN3 )		// COIN1 (2P)
	PORT_BIT ( 0x0008, IP_ACTIVE_LOW, IPT_COIN4 )		// COIN2 (2P)
	PORT_BIT ( 0x0010, IP_ACTIVE_LOW, IPT_UNUSED )		// SUBCPU INT FLAG
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_UNUSED )		// SNDCPU NMI FLAG
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )		// SUBCPU NMI FLAG
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )		// EEPROM READ
	PORT_BITX( 0x0100, IP_ACTIVE_LOW, IPT_SERVICE, DEF_STR( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )	// TEST (1P)
	PORT_BITX( 0x0200, IP_ACTIVE_LOW, IPT_SERVICE, DEF_STR( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )	// TEST (2P)
	PORT_BIT ( 0x0400, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x0800, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x1000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )

	VSYSMJCTRL_PORT1	/* (1) PORT 1-0 */
	VSYSMJCTRL_PORT2	/* (2) PORT 1-1 */
	VSYSMJCTRL_PORT3	/* (3) PORT 1-2 */
	VSYSMJCTRL_PORT4	/* (4) PORT 1-3 */
INPUT_PORTS_END

INPUT_PORTS_START( fromanc4 )
	PORT_START	/* (0) COIN SW, TEST SW, EEPROM DATA, etc */
	PORT_BITX( 0x0001, IP_ACTIVE_LOW, IPT_SERVICE, DEF_STR( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )	// TEST (1P)
	PORT_BIT ( 0x0002, IP_ACTIVE_LOW, IPT_COIN1 )		// COIN1 (1P)
	PORT_BIT ( 0x0004, IP_ACTIVE_LOW, IPT_COIN2 )		// COIN2 (1P)
	PORT_BIT ( 0x0008, IP_ACTIVE_LOW, IPT_COIN3 )		// COIN3 (2P)
	PORT_BIT ( 0x0010, IP_ACTIVE_LOW, IPT_COIN4 )		// COIN4 (2P)
	PORT_BIT ( 0x0020, IP_ACTIVE_LOW, IPT_UNUSED )		// SNDCPU NMI FLAG
	PORT_BIT ( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )		// EEPROM READ
	PORT_BIT ( 0x0100, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x0200, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x0400, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x0800, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x1000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x2000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT ( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )

	VSYSMJCTRL_PORT1	/* (1) PORT 1-0 */
	VSYSMJCTRL_PORT2	/* (2) PORT 1-1 */
	VSYSMJCTRL_PORT3	/* (3) PORT 1-2 */
	VSYSMJCTRL_PORT4	/* (4) PORT 1-3 */
INPUT_PORTS_END


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

static struct GfxLayout fromanc2_tilelayout =
{
	8, 8,
	RGN_FRAC(1, 1),
	4,
	{ 0, 1, 2, 3 },
	{ 4, 0, 12, 8, 20, 16, 28, 24 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8
};

static struct GfxDecodeInfo fromanc2_gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &fromanc2_tilelayout, (  0 * 2), (256 * 2) },
	{ REGION_GFX2, 0, &fromanc2_tilelayout, (256 * 2), (256 * 2) },
	{ REGION_GFX3, 0, &fromanc2_tilelayout, (512 * 2), (256 * 2) },
	{ REGION_GFX4, 0, &fromanc2_tilelayout, (768 * 2), (256 * 2) },
	{ -1 } /* end of array */
};

static struct GfxLayout fromancr_tilelayout =
{
	8, 8,
	RGN_FRAC(1, 1),
	8,
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 1*8, 0*8, 3*8, 2*8, 5*8, 4*8, 7*8, 6*8 },
	{ 0*64, 1*64, 2*64, 3*64, 4*64, 5*64, 6*64, 7*64 },
	64*8
};

static struct GfxDecodeInfo fromancr_gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &fromancr_tilelayout, (512 * 2), 2 },
	{ REGION_GFX2, 0, &fromancr_tilelayout, (256 * 2), 2 },
	{ REGION_GFX3, 0, &fromancr_tilelayout, (  0 * 2), 2 },
	{ -1 } /* end of array */
};


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

static void irqhandler(int irq)
{
	cpu_set_irq_line(1, 0, irq ? ASSERT_LINE : CLEAR_LINE);
}

static struct YM2610interface ym2610_interface =
{
	1,
	8000000,
	{ 50 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ irqhandler },
	{ 0 },
	{ REGION_SOUND1 },
	{ YM3012_VOL(75, MIXER_PAN_CENTER, 75, MIXER_PAN_CENTER) }
};


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

static MACHINE_DRIVER_START( fromanc2 )

	/* basic machine hardware */
	MDRV_CPU_ADD(M68000,32000000/2)		/* 16.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_main,fromanc2_writemem_main)
	MDRV_CPU_VBLANK_INT(fromanc2_interrupt,1)

	MDRV_CPU_ADD(Z80,32000000/4)
	MDRV_CPU_FLAGS(CPU_AUDIO_CPU)		/* 8.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_sound,fromanc2_writemem_sound)
	MDRV_CPU_PORTS(fromanc2_readport_sound,fromanc2_writeport_sound)

	MDRV_CPU_ADD(Z80,32000000/4)		/* 8.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_sub,fromanc2_writemem_sub)
	MDRV_CPU_PORTS(fromanc2_readport_sub,fromanc2_writeport_sub)

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	MDRV_MACHINE_INIT(fromanc2)
	MDRV_NVRAM_HANDLER(93C46)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(512, 512)
	MDRV_VISIBLE_AREA(0, 352-1, 0, 240-1)
	MDRV_GFXDECODE(fromanc2_gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(4096)

	MDRV_VIDEO_START(fromanc2)
	MDRV_VIDEO_UPDATE(fromanc2)

	/* sound hardware */
	MDRV_SOUND_ATTRIBUTES(SOUND_SUPPORTS_STEREO)
	MDRV_SOUND_ADD(YM2610, ym2610_interface)
MACHINE_DRIVER_END

static MACHINE_DRIVER_START( fromancr )

	/* basic machine hardware */
	MDRV_CPU_ADD(M68000,32000000/2)		/* 16.00 MHz */
	MDRV_CPU_MEMORY(fromancr_readmem_main,fromancr_writemem_main)
	MDRV_CPU_VBLANK_INT(fromanc2_interrupt,1)

	MDRV_CPU_ADD(Z80,32000000/4)
	MDRV_CPU_FLAGS(CPU_AUDIO_CPU)		/* 8.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_sound,fromanc2_writemem_sound)
	MDRV_CPU_PORTS(fromanc2_readport_sound,fromanc2_writeport_sound)

	MDRV_CPU_ADD(Z80,32000000/4)		/* 8.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_sub,fromanc2_writemem_sub)
	MDRV_CPU_PORTS(fromanc2_readport_sub,fromanc2_writeport_sub)

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	MDRV_MACHINE_INIT(fromancr)
	MDRV_NVRAM_HANDLER(93C46)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(512, 512)
	MDRV_VISIBLE_AREA(0, 352-1, 0, 240-1)
	MDRV_GFXDECODE(fromancr_gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(4096)

	MDRV_VIDEO_START(fromancr)
	MDRV_VIDEO_UPDATE(fromanc2)

	/* sound hardware */
	MDRV_SOUND_ATTRIBUTES(SOUND_SUPPORTS_STEREO)
	MDRV_SOUND_ADD(YM2610, ym2610_interface)
MACHINE_DRIVER_END

static MACHINE_DRIVER_START( fromanc4 )

	/* basic machine hardware */
	MDRV_CPU_ADD(M68000,32000000/2)		/* 16.00 MHz */
	MDRV_CPU_MEMORY(fromanc4_readmem_main,fromanc4_writemem_main)
	MDRV_CPU_VBLANK_INT(fromanc2_interrupt,1)

	MDRV_CPU_ADD(Z80,32000000/4)
	MDRV_CPU_FLAGS(CPU_AUDIO_CPU)		/* 8.00 MHz */
	MDRV_CPU_MEMORY(fromanc2_readmem_sound,fromanc2_writemem_sound)
	MDRV_CPU_PORTS(fromanc2_readport_sound,fromanc2_writeport_sound)

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	MDRV_MACHINE_INIT(fromanc4)
	MDRV_NVRAM_HANDLER(93C46)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(2048, 256)
	MDRV_VISIBLE_AREA(0, 352-1, 0, 240-1)
	MDRV_GFXDECODE(fromancr_gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(4096)

	MDRV_VIDEO_START(fromanc4)
	MDRV_VIDEO_UPDATE(fromanc2)

	/* sound hardware */
	MDRV_SOUND_ATTRIBUTES(SOUND_SUPPORTS_STEREO)
	MDRV_SOUND_ADD(YM2610, ym2610_interface)
MACHINE_DRIVER_END


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------

ROM_START( fromanc2 )
	ROM_REGION( 0x0080000, REGION_CPU1, 0 )			// MAIN CPU
	ROM_LOAD16_WORD_SWAP( "4-ic23.bin", 0x000000, 0x080000, CRC(96c90f9e) SHA1(c233e91d6967ef05cf14923273be84b17fce200f) )

	ROM_REGION( 0x0010000, REGION_CPU2, 0 )			// SOUND CPU
	ROM_LOAD( "5-ic85.bin",  0x00000, 0x10000, CRC(d8f19aa3) SHA1(f980c2a021fa1995bc18b6427b361506ca8d9bf2) )

	ROM_REGION( 0x0020000, REGION_CPU3, 0 )			// SUB CPU + BANK RAM
	ROM_LOAD( "3-ic1.bin",   0x00000, 0x10000, CRC(6d02090e) SHA1(08a538f3a578adbf83718e5e592c457b2ad841a6) )

	ROM_REGION( 0x0480000, REGION_GFX1, ROMREGION_DISPOSE )	// LAYER4 DATA
	ROM_LOAD( "124-121.bin", 0x000000, 0x200000, CRC(0b62c9c5) SHA1(1e82398a34fb69bf2a82ef1af79dcc6a50ee53e9) )
	ROM_LOAD( "125-122.bin", 0x200000, 0x200000, CRC(1d6dc86e) SHA1(31804465fd9a7c8a20a4bc2217a70bda7963e0ae) )
	ROM_LOAD( "126-123.bin", 0x400000, 0x080000, CRC(9c0f7abc) SHA1(0b69d72e50e64bf02fed4a11cdf10db547953074) )

	ROM_REGION( 0x0480000, REGION_GFX2, ROMREGION_DISPOSE )	// LAYER3 DATA
	ROM_LOAD( "35-47.bin",   0x000000, 0x200000, CRC(97ff0ad6) SHA1(eefa13ef07d6f665a641464089345f1e0ffa7b56) )
	ROM_LOAD( "161-164.bin", 0x200000, 0x200000, CRC(eedbc4d1) SHA1(2f882c5a2a0311bc1fca7b8569621ffee8cdbc82) )
	ROM_LOAD( "162-165.bin", 0x400000, 0x080000, CRC(9b546e59) SHA1(69a2fad9aa87fd07e59fed2fb19c5533a9176bb5) )

	ROM_REGION( 0x0200000, REGION_GFX3, ROMREGION_DISPOSE )	// LAYER2 DATA
	ROM_LOAD( "36-48.bin",   0x000000, 0x200000, CRC(c8ee7f40) SHA1(3f043e4d93dd20f0bfb56b6345d8d60c884547db) )

	ROM_REGION( 0x0100000, REGION_GFX4, ROMREGION_DISPOSE )	// LAYER1 DATA
	ROM_LOAD( "40-52.bin",   0x000000, 0x100000, CRC(dbb5062d) SHA1(d1be4d675b36ea6ebd602d5c990adcf3c029485e) )

	ROM_REGION( 0x0400000, REGION_SOUND1, 0 )		// SOUND DATA
	ROM_LOAD( "ic96.bin",    0x000000, 0x200000, CRC(2f1b394c) SHA1(d95dd8231d7873328f2253eaa27374c79d87e21b) )
	ROM_LOAD( "ic97.bin",    0x200000, 0x200000, CRC(1d1377fc) SHA1(0dae5dfcbcf4ed6662522e9404fcac0236dce04d) )
ROM_END

ROM_START( fromancr )
	ROM_REGION( 0x0080000, REGION_CPU1, 0 )			// MAIN CPU
	ROM_LOAD16_WORD_SWAP( "2-ic20.bin", 0x000000, 0x080000, CRC(378eeb9c) SHA1(c1cfc7440590a229b3cdc1114428a473fea15b63) )

	ROM_REGION( 0x0010000, REGION_CPU2, 0 )			// SOUND CPU
	ROM_LOAD( "5-ic73.bin",  0x0000000, 0x010000, CRC(3e4727fe) SHA1(816c0c2cd2e349900fb9cd63cbced4c621017f37) )

	ROM_REGION( 0x0020000, REGION_CPU3, 0 )			// SUB CPU + BANK RAM
	ROM_LOAD( "4-ic1.bin",   0x0000000, 0x010000, CRC(6d02090e) SHA1(08a538f3a578adbf83718e5e592c457b2ad841a6) )

	ROM_REGION( 0x0800000, REGION_GFX1, ROMREGION_DISPOSE )	// BG DATA
	ROM_LOAD( "ic1-3.bin",   0x0000000, 0x400000, CRC(70ad9094) SHA1(534f10478a929e9e0cc4e01573a68474fe696099) )
	ROM_LOAD( "ic2-4.bin",   0x0400000, 0x400000, CRC(c6c6e8f7) SHA1(315e4e8ae9d1e3d68f4b2cff723d78652dc74e57) )

	ROM_REGION( 0x2400000, REGION_GFX2, ROMREGION_DISPOSE )	// FG DATA
	ROM_LOAD( "ic28-13.bin", 0x0000000, 0x400000, CRC(7d7f9f63) SHA1(fe7b7a6bd9610d953f109b5ff8e38aab1c4ffac1) )
	ROM_LOAD( "ic29-14.bin", 0x0400000, 0x400000, CRC(8ec65f31) SHA1(9b63b18d5ad8f7ec37fa950b21d547fec559d5fa) )
	ROM_LOAD( "ic31-16.bin", 0x0800000, 0x400000, CRC(e4859534) SHA1(91fbbe0ab8119a954d76d33134290a7f7640e4ba) )
	ROM_LOAD( "ic32-17.bin", 0x0c00000, 0x400000, CRC(20d767da) SHA1(477d86538e95583238c50e11acee3ed9ed17b75a) )
	ROM_LOAD( "ic34-19.bin", 0x1000000, 0x400000, CRC(d62a383f) SHA1(0b11a97fa11a0b9657219d70a2ba26843b37d285) )
	ROM_LOAD( "ic35-20.bin", 0x1400000, 0x400000, CRC(4e697f38) SHA1(66b2e9ecedfcf878defb31528611574c1711e831) )
	ROM_LOAD( "ic37-22.bin", 0x1800000, 0x400000, CRC(6302bf5f) SHA1(bac8bead71e25e060bc75abd428dce97e5d51ef2) )
	ROM_LOAD( "ic38-23.bin", 0x1c00000, 0x400000, CRC(c6cffa53) SHA1(41a1c31d921fa92aa285e0a874565e929dba80dc) )
	ROM_LOAD( "ic40-25.bin", 0x2000000, 0x400000, CRC(af60bd0e) SHA1(0dc3a2e9b06626b3891b60368c3ef4d7ce1bdc6a) )

	ROM_REGION( 0x0200000, REGION_GFX3, ROMREGION_DISPOSE )	// TEXT DATA
	ROM_LOAD( "ic28-29.bin", 0x0000000, 0x200000, CRC(f5e262aa) SHA1(35464d059f4814832bf5cb3bede4b8a600bc8a84) )

	ROM_REGION( 0x0400000, REGION_SOUND1, 0 )		// SOUND DATA
	ROM_LOAD( "ic81.bin",    0x0000000, 0x200000, CRC(8ab6e343) SHA1(5ae28e6944edb0a4b8d0071ce48e348b6e927ca9) )
	ROM_LOAD( "ic82.bin",    0x0200000, 0x200000, CRC(f57daaf8) SHA1(720eadf771c89d8749317b632bbc5e8ff1f6f520) )
ROM_END

ROM_START( fromanc4 )
	ROM_REGION( 0x0800000, REGION_CPU1, 0 )			// MAIN CPU + DATA
	ROM_LOAD16_WORD_SWAP( "ic18.bin",    0x0000000, 0x080000, CRC(46a47839) SHA1(f1ba47b193e7e4b1c0fe8d67a76a9c452989885c) )
	ROM_LOAD16_WORD_SWAP( "em33-m00.19", 0x0400000, 0x400000, CRC(6442534b) SHA1(a504d5cdd569ad4301f9917247531d4fdb807c76) )

	ROM_REGION( 0x0020000, REGION_CPU2, 0 )			// SOUND CPU
	ROM_LOAD( "ic79.bin", 0x0000000, 0x020000, CRC(c9587c09) SHA1(e04ee8c3f8519c2b2d3c2bdade1e142974b7fcb1) )

	ROM_REGION( 0x1000000, REGION_GFX1, ROMREGION_DISPOSE )	// BG DATA
	ROM_LOAD16_WORD_SWAP( "em33-c00.59", 0x0000000, 0x400000, CRC(7192bbad) SHA1(d9212860a516106c64e348c78e03091ee766ab23) )
	ROM_LOAD16_WORD_SWAP( "em33-c01.60", 0x0400000, 0x400000, CRC(d75af19a) SHA1(3a9c4ccf1f832d0302fe115d336e33e006910a8a) )
	ROM_LOAD16_WORD_SWAP( "em33-c02.61", 0x0800000, 0x400000, CRC(4f4d2735) SHA1(d0b59c8ed285ec9120a89b0198e414e33567729a) )
	ROM_LOAD16_WORD_SWAP( "em33-c03.62", 0x0c00000, 0x400000, CRC(7ece6ad5) SHA1(c506fc4ea68abf57009d524a17ca487f9c568abd) )

	ROM_REGION( 0x3000000, REGION_GFX2, ROMREGION_DISPOSE )	// FG DATA
	ROM_LOAD16_WORD_SWAP( "em33-b00.38", 0x0000000, 0x400000, CRC(10b8f90d) SHA1(68b8f197c7be70082f61016824098c1ae3a76b38) )
	ROM_LOAD16_WORD_SWAP( "em33-b01.39", 0x0400000, 0x400000, CRC(3b3ea291) SHA1(bb80070a19bb1a1febda612ef260f895a8b65ce2) )
	ROM_LOAD16_WORD_SWAP( "em33-b02.40", 0x0800000, 0x400000, CRC(de88f95b) SHA1(d84a1896a1ef3d9b7fa7de23771168e17c7a450a) )
	ROM_LOAD16_WORD_SWAP( "em33-b03.41", 0x0c00000, 0x400000, CRC(35c1b398) SHA1(b2141cdd3b7f9e2cbfb0a048c440979b59149be5) )
	ROM_LOAD16_WORD_SWAP( "em33-b04.42", 0x1000000, 0x400000, CRC(84b8d5db) SHA1(5999a12c24c01ee8673c2c0a9193c8800a490e6f) )
	ROM_LOAD16_WORD_SWAP( "em33-b05.43", 0x1400000, 0x400000, CRC(b822b57c) SHA1(b50f3b73239a688101027f1c4247fed5ae59b064) )
	ROM_LOAD16_WORD_SWAP( "em33-b06.44", 0x1800000, 0x400000, CRC(8f1b2b19) SHA1(1e08908758fed104d114fecc9977a4a0eb93fe9b) )
	ROM_LOAD16_WORD_SWAP( "em33-b07.45", 0x1c00000, 0x400000, CRC(dd4ddcb7) SHA1(0145afa70c1a6f59eec65cf4d8572f2c00cd04a5) )
	ROM_LOAD16_WORD_SWAP( "em33-b08.46", 0x2000000, 0x400000, CRC(3d8ce018) SHA1(43c3cb4d6c26a8209fc290fcac56297fe66209e3) )
	ROM_LOAD16_WORD_SWAP( "em33-b09.47", 0x2400000, 0x400000, CRC(4ad79143) SHA1(9240ee46fff8f4a400a2bddaedb9acd258f37e1d) )
	ROM_LOAD16_WORD_SWAP( "em33-b10.48", 0x2800000, 0x400000, CRC(d6ab74b2) SHA1(1dbff7e997869a00922f6471afbd76d383ec0e2c) )
	ROM_LOAD16_WORD_SWAP( "em33-b11.49", 0x2c00000, 0x400000, CRC(4aa206b1) SHA1(afee0d8fc02e4f673ecccb9786c6d502dea5cb70) )

	ROM_REGION( 0x0400000, REGION_GFX3, ROMREGION_DISPOSE )	// TEXT DATA
	ROM_LOAD16_WORD_SWAP( "em33-a00.37", 0x0000000, 0x400000, CRC(a3bd4a34) SHA1(78bd5298e83f89c738c18105c8bc809fa6a35206) )

	ROM_REGION( 0x0800000, REGION_SOUND1, 0 )		// SOUND DATA
	ROM_LOAD16_WORD_SWAP( "em33-p00.88", 0x0000000, 0x400000, CRC(1c6418d2) SHA1(c66d6b35f342fcbeca5414dbb2ac038d8a2ec2c4) )
	ROM_LOAD16_WORD_SWAP( "em33-p01.89", 0x0400000, 0x400000, CRC(615b4e6e) SHA1(a031773ed27de2263e32422a3d11118bdcb2c197) )
ROM_END


GAME( 1995, fromanc2, 0, fromanc2, fromanc2, fromanc2, ROT0, "Video System Co.", "Taisen Idol-Mahjong Final Romance 2 (Japan)" )
GAME( 1995, fromancr, 0, fromancr, fromanc2, fromancr, ROT0, "Video System Co.", "Taisen Mahjong FinalRomance R (Japan)" )
GAME( 1998, fromanc4, 0, fromanc4, fromanc4, fromanc4, ROT0, "Video System Co.", "Taisen Mahjong FinalRomance 4 (Japan)" )
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
