#pragma code_seg("C211")
#pragma data_seg("D211")
#pragma bss_seg("B211")
#pragma const_seg("K211")
#pragma comment(linker, "/merge:D211=211")
#pragma comment(linker, "/merge:C211=211")
#pragma comment(linker, "/merge:B211=211")
#pragma comment(linker, "/merge:K211=211")
/***************************************************************************

Ninja Gaiden memory map (preliminary)

000000-03ffff ROM
060000-063fff RAM
070000-070fff Video RAM (text layer)
072000-075fff VRAM (backgrounds)
076000-077fff Sprite RAM
078000-079fff Palette RAM

07a100-07a1ff Unknown

memory mapped ports:

read:
07a001    IN0
07a002    IN2
07a003    IN1
07a004    DWSB
07a005    DSWA
see the input_ports definition below for details on the input bits

write:
07a104-07a105 text  layer Y scroll
07a10c-07a10d text  layer X scroll
07a204-07a205 front layer Y scroll
07a20c-07a20d front layer X scroll
07a304-07a305 back  layer Y scroll
07a30c-07a30d back  layer X scroll

Notes:
- The sprite Y size control is slightly different from gaiden/wildfang to
  raiga. In the first two, size X and Y change together, while in the latter
  they are changed independently. This is handled with a variable set in
  DRIVER_INIT, but it might also be a selectable hardware feature, since
  the two extra bits used by raiga are perfectly merged with the rest.
  Raiga also uses more sprites than the others, but there's no way to tell
  if hardware is more powerful or the extra sprites were just not needed
  in the earlier games.

todo:

- finish raiga (need to trojanize the mcu).

***************************************************************************/
/***************************************************************************

Strato Fighter (US version)
Tecmo, 1991


PCB Layout
----------

Top Board
---------
0210-A
MG-Y.VO
-----------------------------------------------
|         MN50005XTA         4MHz  DSW2 DSW1  |
|                    6264 6264       8049     |
|           IOP8     1.3S 2.4S                |
|24MHz                                        |
|                                             |
|18.432MHz                                   J|
|                                             |
|              68000P10                      A|
|                                             |
|          6116                              M|
|          6116                               |
|          6116                              M|
|                                             |
|                                            A|
|                                             |
|              6264    YM2203  YM3014         |
|       Z80    3.4B                           |
| 4MHz  6295   4.4A    YM2203  YM3014         |
-----------------------------------------------

Bottom Board
------------
0210-B
MG-Y.VO
-----------------------------------------------
|                TECMO-5                      |
|               -----------                   |
|               | TECMO-06|                   |
| ROM.M1 ROM.M3 | YM6048  |     6264          |
|               -----------     6264          |
|   4164  4164  4164  4164                    |
|   4164  4164  4164  4164                    |
|   4164  4164  4164  4164                    |
|                                             |
|                                             |
|        TECMO-3      TECMO-3      TECMO-3    |
| TECMO-4      TECMO-4      TECMO-4           |
|                                             |
|                                             |
|                                             |
|  6264  6264   6116   6116   6264            |
| ROM.1B        ROM.4B        6116            |
|                             ROM.7A          |
-----------------------------------------------

Notes:
      68k clock: 9.216MHz (18.432 / 2)
      Z80 clock: 4.000MHz
      IOP8 manufactured by Ricoh. Full part number: RICOH EPLIOP8BP (PAL or PIC?)

***************************************************************************/


#include "driver.h"
#include "vidhrdw/generic.h"
#include "cpu/m68000/m68000.h"
#include "cpu/z80/z80.h"

extern data16_t *gaiden_videoram,*gaiden_videoram2,*gaiden_videoram3;
extern int gaiden_sprite_sizey;

VIDEO_UPDATE( gaiden );

WRITE16_HANDLER( gaiden_videoram_w );
WRITE16_HANDLER( gaiden_videoram2_w );
READ16_HANDLER( gaiden_videoram2_r );
WRITE16_HANDLER( gaiden_videoram3_w );
READ16_HANDLER( gaiden_videoram3_r );

WRITE16_HANDLER( gaiden_txscrollx_w );
WRITE16_HANDLER( gaiden_txscrolly_w );
WRITE16_HANDLER( gaiden_fgscrollx_w );
WRITE16_HANDLER( gaiden_fgscrolly_w );
WRITE16_HANDLER( gaiden_bgscrollx_w );
WRITE16_HANDLER( gaiden_bgscrolly_w );
WRITE16_HANDLER( gaiden_flip_w );

VIDEO_START( gaiden );
VIDEO_UPDATE( gaiden );



static WRITE16_HANDLER( gaiden_sound_command_w )
{
	if (ACCESSING_LSB) soundlatch_w(0,data & 0xff);	/* Ninja Gaiden */
	if (ACCESSING_MSB) soundlatch_w(0,data >> 8);	/* Tecmo Knight */
	cpu_set_irq_line(1,IRQ_LINE_NMI,PULSE_LINE);
}



/* Wild Fang / Tecmo Knight has a simple protection. It writes codes to 0x07a804, */
/* and reads the answer from 0x07a007. The returned values contain the address of */
/* a function to jump to. */

static int prot;

static WRITE16_HANDLER( wildfang_protection_w )
{
	if (ACCESSING_MSB)
	{
		static int jumpcode;
		static int jumppoints[] =
		{
			0x0c0c,0x0cac,0x0d42,0x0da2,0x0eea,0x112e,0x1300,0x13fa,
			0x159a,0x1630,0x109a,0x1700,0x1750,0x1806,0x18d6,0x1a44,
			0x1b52
		};

		data >>= 8;

//		logerror("PC %06x: prot = %02x\n",activecpu_get_pc(),data);

		switch (data & 0xf0)
		{
			case 0x00:	/* init */
				prot = 0x00;
				break;
			case 0x10:	/* high 4 bits of jump code */
				jumpcode = (data & 0x0f) << 4;
				prot = 0x10;
				break;
			case 0x20:	/* low 4 bits of jump code */
				jumpcode |= data & 0x0f;
				if (jumpcode >= sizeof(jumppoints)/sizeof(jumppoints[0]))
				{
					logerror("unknown jumpcode %02x\n",jumpcode);
					jumpcode = 0;
				}
				prot = 0x20;
				break;
			case 0x30:	/* ask for bits 12-15 of function address */
				prot = 0x40 | ((jumppoints[jumpcode] >> 12) & 0x0f);
				break;
			case 0x40:	/* ask for bits 8-11 of function address */
				prot = 0x50 | ((jumppoints[jumpcode] >> 8) & 0x0f);
				break;
			case 0x50:	/* ask for bits 4-7 of function address */
				prot = 0x60 | ((jumppoints[jumpcode] >> 4) & 0x0f);
				break;
			case 0x60:	/* ask for bits 0-3 of function address */
				prot = 0x70 | ((jumppoints[jumpcode] >> 0) & 0x0f);
				break;
		}
	}
}

static READ16_HANDLER( wildfang_protection_r )
{
//	logerror("PC %06x: read prot %02x\n",activecpu_get_pc(),prot);
	return prot;
}



/* raiga, incomplete. MCU read routine is at D9CE */
/* on startup it reads 00/36/0e/11/33/34/2d, fetching some code copied to RAM,
   and a value which is used as an offset to point to the code in RAM.
   19/12/31/28 are read repeatedly, from the interrupt, and the returned value
   changes.
   Others polled: 2b (fbi/japan screen) 15 (first boss) 23 25 13 1e 1c
  */

static WRITE16_HANDLER( raiga_protection_w )
{
	if (ACCESSING_MSB)
	{
		static int jumpcode;
		static int jumppoints[] =
		{
			0x6669,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
			    -1,    -1,    -1,    -1,    -1,    -1,0x4a46,    -1,
			    -1,0x6704,    -2,    -1,    -1,    -1,    -1,    -1,
			    -1,    -2,    -1,    -1,    -1,    -1,    -1,    -1,
			    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
			    -2,    -1,    -1,    -1,    -1,0x4e75,    -1,    -1,
			    -1,    -2,    -1,0x4e71,0x60fc,    -1,0x7288,    -1,
			    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1
		};

		data >>= 8;

//		logerror("PC %06x: prot = %02x\n",activecpu_get_pc(),data);

		switch (data & 0xf0)
		{
			case 0x00:	/* init */
				prot = 0x00;
				break;
			case 0x10:	/* high 4 bits of jump code */
				jumpcode = (data & 0x0f) << 4;
				prot = 0x10;
				break;
			case 0x20:	/* low 4 bits of jump code */
				jumpcode |= data & 0x0f;
				logerror("requested protection jumpcode %02x\n",jumpcode);
//				jumpcode = 0;
				if (jumpcode >= sizeof(jumppoints)/sizeof(jumppoints[0]) ||
						jumppoints[jumpcode] == -1)
				{
					logerror("unknown jumpcode %02x\n",jumpcode);
					usrintf_showmessage("unknown jumpcode %02x",jumpcode);
					jumpcode = 0;
				}
				prot = 0x20;
				break;
			case 0x30:	/* ask for bits 12-15 of function address */
				prot = 0x40 | ((jumppoints[jumpcode] >> 12) & 0x0f);
				break;
			case 0x40:	/* ask for bits 8-11 of function address */
				prot = 0x50 | ((jumppoints[jumpcode] >> 8) & 0x0f);
				break;
			case 0x50:	/* ask for bits 4-7 of function address */
				prot = 0x60 | ((jumppoints[jumpcode] >> 4) & 0x0f);
				break;
			case 0x60:	/* ask for bits 0-3 of function address */
				prot = 0x70 | ((jumppoints[jumpcode] >> 0) & 0x0f);
				break;
		}
	}
}

static READ16_HANDLER( raiga_protection_r )
{
//	logerror("PC %06x: read prot %02x\n",activecpu_get_pc(),prot);
	return prot;
}

static MEMORY_READ16_START( readmem )
	{ 0x000000, 0x03ffff, MRA16_ROM },
	{ 0x060000, 0x063fff, MRA16_RAM },
	{ 0x070000, 0x070fff, MRA16_RAM },
	{ 0x072000, 0x073fff, gaiden_videoram2_r },
	{ 0x074000, 0x075fff, gaiden_videoram3_r },
	{ 0x076000, 0x077fff, MRA16_RAM },
	{ 0x078000, 0x0787ff, MRA16_RAM },
	{ 0x078800, 0x079fff, MRA16_NOP },   /* extra portion of palette RAM, not really used */
	{ 0x07a000, 0x07a001, input_port_0_word_r },
	{ 0x07a002, 0x07a003, input_port_1_word_r },
	{ 0x07a004, 0x07a005, input_port_2_word_r },
MEMORY_END

static MEMORY_WRITE16_START( writemem )
	{ 0x000000, 0x03ffff, MWA16_ROM },
	{ 0x060000, 0x063fff, MWA16_RAM },
	{ 0x070000, 0x070fff, gaiden_videoram_w, &gaiden_videoram },
	{ 0x072000, 0x073fff, gaiden_videoram2_w, &gaiden_videoram2 },
	{ 0x074000, 0x075fff, gaiden_videoram3_w, &gaiden_videoram3 },
	{ 0x076000, 0x077fff, MWA16_RAM, &spriteram16 },
	{ 0x078000, 0x0787ff, paletteram16_xxxxBBBBGGGGRRRR_word_w, &paletteram16 },
	{ 0x078800, 0x079fff, MWA16_NOP },   /* extra portion of palette RAM, not really used */
	{ 0x07a104, 0x07a105, gaiden_txscrolly_w },
	{ 0x07a10c, 0x07a10d, gaiden_txscrollx_w },
	{ 0x07a204, 0x07a205, gaiden_fgscrolly_w },
	{ 0x07a20c, 0x07a20d, gaiden_fgscrollx_w },
	{ 0x07a304, 0x07a305, gaiden_bgscrolly_w },
	{ 0x07a30c, 0x07a30d, gaiden_bgscrollx_w },
	{ 0x07a800, 0x07a801, watchdog_reset16_w },
	{ 0x07a802, 0x07a803, gaiden_sound_command_w },
	{ 0x07a806, 0x07a807, MWA16_NOP },
	{ 0x07a808, 0x07a809, gaiden_flip_w },
MEMORY_END

static MEMORY_READ_START( sound_readmem )
	{ 0x0000, 0xdfff, MRA_ROM },
	{ 0xf000, 0xf7ff, MRA_RAM },
	{ 0xf800, 0xf800, OKIM6295_status_0_r },
	{ 0xfc00, 0xfc00, MRA_NOP },	/* ?? */
	{ 0xfc20, 0xfc20, soundlatch_r },
MEMORY_END

static MEMORY_WRITE_START( sound_writemem )
	{ 0x0000, 0xdfff, MWA_ROM },
	{ 0xf000, 0xf7ff, MWA_RAM },
	{ 0xf800, 0xf800, OKIM6295_data_0_w },
	{ 0xf810, 0xf810, YM2203_control_port_0_w },
	{ 0xf811, 0xf811, YM2203_write_port_0_w },
	{ 0xf820, 0xf820, YM2203_control_port_1_w },
	{ 0xf821, 0xf821, YM2203_write_port_1_w },
	{ 0xfc00, 0xfc00, MWA_NOP },	/* ?? */
MEMORY_END



INPUT_PORTS_START( shadoww )
	PORT_START	/* System Inputs */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START	/* Players Inputs */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER1 )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER1 )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER2 )
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER2 )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	/* Dip Switches order fits the first screen */

	PORT_START	/* DSW */
	PORT_DIPNAME( 0x00e0, 0x00e0, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x00e0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0060, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x00a0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x00c0, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x001c, 0x001c, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x001c, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x000c, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0014, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x0002, 0x0002, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0001, 0x0001, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( On ) )
	PORT_DIPNAME( 0xc000, 0xc000, DEF_STR( Lives ) )
	PORT_DIPSETTING(      0x0000, "1" )
	PORT_DIPSETTING(      0xc000, "2" )
	PORT_DIPSETTING(      0x4000, "3" )
	PORT_DIPSETTING(      0x8000, "4" )
	PORT_DIPNAME( 0x3000, 0x3000, "Energy" )
	PORT_DIPSETTING(      0x0000, "2" )
	PORT_DIPSETTING(      0x3000, "3" )
	PORT_DIPSETTING(      0x1000, "4" )
	PORT_DIPSETTING(      0x2000, "5" )
	PORT_DIPNAME( 0x0800, 0x0800, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0800, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0400, 0x0400, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0400, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0200, 0x0200, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0100, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0100, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
INPUT_PORTS_END

INPUT_PORTS_START( wildfang )
	PORT_START	/* System Inputs */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START	/* Players Inputs */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER1 )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER1 )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER2 )
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER2 )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	/* Dip Switches order fits the first screen */

	PORT_START	/* DSW */
	PORT_DIPNAME( 0x00e0, 0x00e0, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x00e0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0060, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x00a0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x00c0, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x001c, 0x001c, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x001c, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x000c, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0014, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x0002, 0x0002, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0001, 0x0001, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( On ) )
	PORT_DIPNAME( 0xc000, 0xc000, DEF_STR( Lives ) )
	PORT_DIPSETTING(      0x8000, "1" )
	PORT_DIPSETTING(      0xc000, "2" )
	PORT_DIPSETTING(      0x4000, "3" )
/*	PORT_DIPSETTING(      0x0000, "2" ) */
	/* When bit 0 is On,  use bits 4 and 5 for difficulty */
	PORT_DIPNAME( 0x3000, 0x3000, "Difficulty (Tecmo Knight)" )
	PORT_DIPSETTING(      0x3000, "Easy" )
	PORT_DIPSETTING(      0x1000, "Normal" )
	PORT_DIPSETTING(      0x2000, "Hard" )
	PORT_DIPSETTING(      0x0000, "Hardest" )
	/* When bit 0 is 0ff, use bits 2 and 3 for difficulty */
	PORT_DIPNAME( 0x0c00, 0x0c00, "Difficulty (Wild Fang)" )
	PORT_DIPSETTING(      0x0c00, "Easy" )
	PORT_DIPSETTING(      0x0400, "Normal" )
	PORT_DIPSETTING(      0x0800, "Hard" )
	PORT_DIPSETTING(      0x0000, "Hardest" )
	PORT_DIPNAME( 0x0200, 0x0200, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0100, "Title" )	// also affects Difficulty Table (see above)
	PORT_DIPSETTING(      0x0100, "Wild Fang" )
	PORT_DIPSETTING(      0x0000, "Tecmo Knight" )
INPUT_PORTS_END

INPUT_PORTS_START( tknight )
	PORT_START	/* System Inputs */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START	/* Players Inputs */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER1 )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER1 )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER2 )
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 | IPF_PLAYER2 )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	/* Dip Switches order fits the first screen */

	PORT_START	/* DSW */
	PORT_DIPNAME( 0x00e0, 0x00e0, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x00e0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0060, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x00a0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x00c0, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x001c, 0x001c, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x001c, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x000c, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0014, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( 1C_4C ) )
	PORT_DIPNAME( 0x0002, 0x0002, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0001, 0x0001, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( On ) )
	PORT_DIPNAME( 0xc000, 0xc000, DEF_STR( Lives ) )
	PORT_DIPSETTING(      0x8000, "1" )
	PORT_DIPSETTING(      0xc000, "2" )
	PORT_DIPSETTING(      0x4000, "3" )
/*	PORT_DIPSETTING(      0x0000, "2" ) */
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x2000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x1000, 0x1000, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x1000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0c00, 0x0c00, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(      0x0c00, "Easy" )
	PORT_DIPSETTING(      0x0400, "Normal" )
	PORT_DIPSETTING(      0x0800, "Hard" )
	PORT_DIPSETTING(      0x0000, "Hardest" )
	PORT_DIPNAME( 0x0200, 0x0200, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0100, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0100, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
INPUT_PORTS_END

INPUT_PORTS_START( raiga )
	PORT_START	/* System Inputs */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START	/* Players Inputs */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER1 | IPF_8WAY )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER1 )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_PLAYER2 | IPF_8WAY )
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_PLAYER2 )
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	/* Dip Switches order fits the first screen */

	PORT_START	/* DSW */
	PORT_DIPNAME( 0x00f0, 0x00f0, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x00a0, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 3C_2C ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( 4C_3C ) )
	PORT_DIPSETTING(      0x00f0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x00c0, DEF_STR( 3C_4C ) )
	PORT_DIPSETTING(      0x00e0, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(      0x0070, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0060, DEF_STR( 2C_5C ) )
	PORT_DIPSETTING(      0x00b0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0030, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x00d0, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(      0x0050, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(      0x0090, DEF_STR( 1C_7C ) )
	PORT_DIPNAME( 0x000f, 0x000f, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x000a, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( 3C_2C ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( 4C_3C ) )
	PORT_DIPSETTING(      0x000f, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x000c, DEF_STR( 3C_4C ) )
	PORT_DIPSETTING(      0x000e, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(      0x0007, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0006, DEF_STR( 2C_5C ) )
	PORT_DIPSETTING(      0x000b, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0003, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x000d, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(      0x0005, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(      0x0009, DEF_STR( 1C_7C ) )
	PORT_DIPNAME( 0x8000, 0x8000, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x8000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x4000, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x4000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x3000, 0x3000, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(      0x3000, "Easy" )
	PORT_DIPSETTING(      0x1000, "Normal" )
	PORT_DIPSETTING(      0x2000, "Hard" )
	PORT_DIPSETTING(      0x0000, "Hardest" )
	PORT_DIPNAME( 0x0c00, 0x0c00, DEF_STR( Lives ) )
	PORT_DIPSETTING(      0x0000, "2" )
	PORT_DIPSETTING(      0x0c00, "3" )
	PORT_DIPSETTING(      0x0400, "4" )
	PORT_DIPSETTING(      0x0800, "5" )
	PORT_DIPNAME( 0x0300, 0x0300, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(      0x0300, "50k 200k" )
	PORT_DIPSETTING(      0x0100, "100k 300k" )
	PORT_DIPSETTING(      0x0200, "50k only" )
	PORT_DIPSETTING(      0x0000, "None" )
INPUT_PORTS_END



static struct GfxLayout tilelayout =
{
	8,8,	/* tile size */
	RGN_FRAC(1,1),	/* number of tiles */
	4,	/* 4 bits per pixel */
	{ 0, 1, 2, 3 },
	{ 0*4, 1*4, 2*4, 3*4, 4*4, 5*4, 6*4, 7*4 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8	/* offset to next tile */
};

static struct GfxLayout tile2layout =
{
	16,16,	/* tile size */
	RGN_FRAC(1,1),	/* number of tiles */
	4,	/* 4 bits per pixel */
	{ 0, 1, 2, 3 },	/* the bitplanes are packed in one nibble */
	{ 0*4, 1*4, 2*4, 3*4, 4*4, 5*4, 6*4, 7*4,
	  32*8+0*4, 32*8+1*4, 32*8+2*4, 32*8+3*4,
	  32*8+4*4, 32*8+5*4, 32*8+6*4, 32*8+7*4,},
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32,
	  16*32, 17*32, 18*32, 19*32, 20*32, 21*32, 22*32, 23*32},
	128*8	/* offset to next tile */
};

static struct GfxLayout spritelayout =
{
	8,8,	/* sprites size */
	RGN_FRAC(1,2),	/* number of sprites */
	4,	/* 4 bits per pixel */
	{ 0, 1, 2, 3 },	/* the bitplanes are packed in one nibble */
	{ 0,4,RGN_FRAC(1,2),4+RGN_FRAC(1,2),8,12,8+RGN_FRAC(1,2),12+RGN_FRAC(1,2) },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16 },
	16*8	/* offset to next sprite */
};

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &tilelayout,        256, 16 },	/* tiles 8x8 */
	{ REGION_GFX2, 0, &tile2layout,       768, 16 },	/* tiles 16x16 */
	{ REGION_GFX3, 0, &tile2layout,       512, 16 },	/* tiles 16x16 */
	{ REGION_GFX4, 0, &spritelayout,        0, 16 },	/* sprites 8x8 */
	{ -1 } /* end of array */
};



/* handler called by the 2203 emulator when the internal timers cause an IRQ */
static void irqhandler(int irq)
{
	cpu_set_irq_line(1,0,irq ? ASSERT_LINE : CLEAR_LINE);
}

static struct YM2203interface ym2203_interface =
{
	2,			/* 2 chips */
	4000000,	/* 4 MHz ? (hand tuned) */
	{ YM2203_VOL(60,15), YM2203_VOL(60,15) },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ irqhandler }
};


static struct OKIM6295interface okim6295_interface =
{
	1,                  /* 1 chip */
	{ 7576 },			/* 7576Hz frequency */
	{ REGION_SOUND1 },	/* memory region */
	{ 20 }
};


static MACHINE_DRIVER_START( shadoww )

	/* basic machine hardware */
	MDRV_CPU_ADD(M68000, 18432000/2)	/* 9.216 MHz */
	MDRV_CPU_MEMORY(readmem,writemem)
	MDRV_CPU_VBLANK_INT(irq5_line_hold,1)

	MDRV_CPU_ADD(Z80, 4000000)	/* 4 MHz */
	MDRV_CPU_FLAGS(CPU_AUDIO_CPU)
	MDRV_CPU_MEMORY(sound_readmem,sound_writemem)
								/* IRQs are triggered by the YM2203 */
	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(32*8, 32*8)
	MDRV_VISIBLE_AREA(0*8, 32*8-1, 2*8, 30*8-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(1024)

	MDRV_VIDEO_START(gaiden)
	MDRV_VIDEO_UPDATE(gaiden)

	/* sound hardware */
	MDRV_SOUND_ADD(YM2203, ym2203_interface)
	MDRV_SOUND_ADD(OKIM6295, okim6295_interface)
MACHINE_DRIVER_END



/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( shadoww )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "shadowa.1",     0x00000, 0x20000, CRC(8290d567) SHA1(1e2f80c1548c853ec1127e79438f62eda6592a07) )
	ROM_LOAD16_BYTE( "shadowa.2",     0x00001, 0x20000, CRC(f3f08921) SHA1(df6bb7302714e0eab12cbd0a7f2a4ca751a600e1) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "gaiden.3",     0x0000, 0x10000, CRC(75fd3e6a) SHA1(3333e84ed4983caa133e60a8e8895fa897ab4949) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.5",     0x000000, 0x10000, CRC(8d4035f7) SHA1(3473456cdd24e312e3073586d7e8f24eb71bbea1) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "14.bin",       0x000000, 0x20000, CRC(1ecfddaa) SHA1(e71d60ae1a98fe8512498f91cce01c16be9f0871) )
	ROM_LOAD( "15.bin",       0x020000, 0x20000, CRC(1291a696) SHA1(023b05260214adc39bdba81d5e2aa246b6d74a6a) )
	ROM_LOAD( "16.bin",       0x040000, 0x20000, CRC(140b47ca) SHA1(6ffd9b7116658a46a124f9085602d88aa143d829) )
	ROM_LOAD( "17.bin",       0x060000, 0x20000, CRC(7638cccb) SHA1(780d47d3aa248346e0e7abc6e6284542e7392919) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "18.bin",       0x000000, 0x20000, CRC(3fadafd6) SHA1(0cb5387a354c631d5c6aca8f77ecbbc0d175a574) )
	ROM_LOAD( "19.bin",       0x020000, 0x20000, CRC(ddae9d5b) SHA1(108b202ae7ae124a32400a0a404c7d2b614c60bd) )
	ROM_LOAD( "20.bin",       0x040000, 0x20000, CRC(08cf7a93) SHA1(fd3278c3fb3ef30ed03c8a95656d86ba82a163d8) )
	ROM_LOAD( "21.bin",       0x060000, 0x20000, CRC(1ac892f5) SHA1(28364266ca9d1955fb7953f5c2d6f35e114beec6) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.6",     0x000000, 0x20000, CRC(e7ccdf9f) SHA1(80ffcefc95660471124898a9c2bee55df36bda13) )	/* sprites A1 */
	ROM_LOAD( "gaiden.8",     0x020000, 0x20000, CRC(7ef7f880) SHA1(26ba9a76adce24beea3cffa1cb95aeafe6f82f96) )	/* sprites B1 */
	ROM_LOAD( "gaiden.10",    0x040000, 0x20000, CRC(a6451dec) SHA1(553e7a1453b59055fa0b10ca04125543d9f8987c) )	/* sprites C1 */
	ROM_LOAD( "shadoww.12a",  0x060000, 0x10000, CRC(9bb07731) SHA1(b799b1958dc9b84797fdab2591e33bd5d28884a3) )	/* sprites D1 */
	ROM_LOAD( "shadoww.12b",  0x070000, 0x10000, CRC(a4a950a2) SHA1(9766b5e88edd16554e59179a37cca49d29f83367) )	/* sprites D1 */
	ROM_LOAD( "gaiden.7",     0x080000, 0x20000, CRC(016bec95) SHA1(6a6757c52ca9a2398ea43d1af4a8d5adde6f4cd2) )	/* sprites A2 */
	ROM_LOAD( "gaiden.9",     0x0a0000, 0x20000, CRC(6e9b7fd3) SHA1(c86ff61844fc94c02625bb812b9062d0649c8fdf) )	/* sprites B2 */
	ROM_LOAD( "gaiden.11",    0x0c0000, 0x20000, CRC(7fbfdf5e) SHA1(ab67b72dcadb5f2236d29de751de5bf890a9e423) )	/* sprites C2 */
	ROM_LOAD( "shadoww.13a",  0x0e0000, 0x10000, CRC(996d2fa5) SHA1(a32526949af3635914927ebbbe684c3de9562a9d) )	/* sprites D2 */
	ROM_LOAD( "shadoww.13b",  0x0f0000, 0x10000, CRC(b8df8a34) SHA1(6810f7961052a983b8c78b42d550038051012c6d) )	/* sprites D2 */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "gaiden.4",     0x0000, 0x20000, CRC(b0e0faf9) SHA1(2275d2ef5eee356ccf80b9e9644d16fc30a4d107) ) /* samples */
ROM_END

ROM_START( shadowwa )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "shadoww.1",    0x00000, 0x20000, CRC(fefba387) SHA1(20ce28da5877009494c3f3f67488bbe805d91340) )
	ROM_LOAD16_BYTE( "shadoww.2",    0x00001, 0x20000, CRC(9b9d6b18) SHA1(75068611fb1de61120be8bf840f61d90c0dc86ca) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "gaiden.3",     0x0000, 0x10000, CRC(75fd3e6a) SHA1(3333e84ed4983caa133e60a8e8895fa897ab4949) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.5",     0x000000, 0x10000, CRC(8d4035f7) SHA1(3473456cdd24e312e3073586d7e8f24eb71bbea1) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "14.bin",       0x000000, 0x20000, CRC(1ecfddaa) SHA1(e71d60ae1a98fe8512498f91cce01c16be9f0871) )
	ROM_LOAD( "15.bin",       0x020000, 0x20000, CRC(1291a696) SHA1(023b05260214adc39bdba81d5e2aa246b6d74a6a) )
	ROM_LOAD( "16.bin",       0x040000, 0x20000, CRC(140b47ca) SHA1(6ffd9b7116658a46a124f9085602d88aa143d829) )
	ROM_LOAD( "17.bin",       0x060000, 0x20000, CRC(7638cccb) SHA1(780d47d3aa248346e0e7abc6e6284542e7392919) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "18.bin",       0x000000, 0x20000, CRC(3fadafd6) SHA1(0cb5387a354c631d5c6aca8f77ecbbc0d175a574) )
	ROM_LOAD( "19.bin",       0x020000, 0x20000, CRC(ddae9d5b) SHA1(108b202ae7ae124a32400a0a404c7d2b614c60bd) )
	ROM_LOAD( "20.bin",       0x040000, 0x20000, CRC(08cf7a93) SHA1(fd3278c3fb3ef30ed03c8a95656d86ba82a163d8) )
	ROM_LOAD( "21.bin",       0x060000, 0x20000, CRC(1ac892f5) SHA1(28364266ca9d1955fb7953f5c2d6f35e114beec6) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.6",     0x000000, 0x20000, CRC(e7ccdf9f) SHA1(80ffcefc95660471124898a9c2bee55df36bda13) )	/* sprites A1 */
	ROM_LOAD( "gaiden.8",     0x020000, 0x20000, CRC(7ef7f880) SHA1(26ba9a76adce24beea3cffa1cb95aeafe6f82f96) )	/* sprites B1 */
	ROM_LOAD( "gaiden.10",    0x040000, 0x20000, CRC(a6451dec) SHA1(553e7a1453b59055fa0b10ca04125543d9f8987c) )	/* sprites C1 */
	ROM_LOAD( "shadoww.12a",  0x060000, 0x10000, CRC(9bb07731) SHA1(b799b1958dc9b84797fdab2591e33bd5d28884a3) )	/* sprites D1 */
	ROM_LOAD( "shadoww.12b",  0x070000, 0x10000, CRC(a4a950a2) SHA1(9766b5e88edd16554e59179a37cca49d29f83367) )	/* sprites D1 */
	ROM_LOAD( "gaiden.7",     0x080000, 0x20000, CRC(016bec95) SHA1(6a6757c52ca9a2398ea43d1af4a8d5adde6f4cd2) )	/* sprites A2 */
	ROM_LOAD( "gaiden.9",     0x0a0000, 0x20000, CRC(6e9b7fd3) SHA1(c86ff61844fc94c02625bb812b9062d0649c8fdf) )	/* sprites B2 */
	ROM_LOAD( "gaiden.11",    0x0c0000, 0x20000, CRC(7fbfdf5e) SHA1(ab67b72dcadb5f2236d29de751de5bf890a9e423) )	/* sprites C2 */
	ROM_LOAD( "shadoww.13a",  0x0e0000, 0x10000, CRC(996d2fa5) SHA1(a32526949af3635914927ebbbe684c3de9562a9d) )	/* sprites D2 */
	ROM_LOAD( "shadoww.13b",  0x0f0000, 0x10000, CRC(b8df8a34) SHA1(6810f7961052a983b8c78b42d550038051012c6d) )	/* sprites D2 */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "gaiden.4",     0x0000, 0x20000, CRC(b0e0faf9) SHA1(2275d2ef5eee356ccf80b9e9644d16fc30a4d107) ) /* samples */
ROM_END

ROM_START( gaiden )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "gaiden.1",     0x00000, 0x20000, CRC(e037ff7c) SHA1(5418bcb80d4c52f05e3c26668193452fd51f1283) )
	ROM_LOAD16_BYTE( "gaiden.2",     0x00001, 0x20000, CRC(454f7314) SHA1(231296423870f00ea2e545faf0fbb37577430a4f) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "gaiden.3",     0x0000, 0x10000, CRC(75fd3e6a) SHA1(3333e84ed4983caa133e60a8e8895fa897ab4949) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.5",     0x000000, 0x10000, CRC(8d4035f7) SHA1(3473456cdd24e312e3073586d7e8f24eb71bbea1) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "14.bin",       0x000000, 0x20000, CRC(1ecfddaa) SHA1(e71d60ae1a98fe8512498f91cce01c16be9f0871) )
	ROM_LOAD( "15.bin",       0x020000, 0x20000, CRC(1291a696) SHA1(023b05260214adc39bdba81d5e2aa246b6d74a6a) )
	ROM_LOAD( "16.bin",       0x040000, 0x20000, CRC(140b47ca) SHA1(6ffd9b7116658a46a124f9085602d88aa143d829) )
	ROM_LOAD( "17.bin",       0x060000, 0x20000, CRC(7638cccb) SHA1(780d47d3aa248346e0e7abc6e6284542e7392919) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "18.bin",       0x000000, 0x20000, CRC(3fadafd6) SHA1(0cb5387a354c631d5c6aca8f77ecbbc0d175a574) )
	ROM_LOAD( "19.bin",       0x020000, 0x20000, CRC(ddae9d5b) SHA1(108b202ae7ae124a32400a0a404c7d2b614c60bd) )
	ROM_LOAD( "20.bin",       0x040000, 0x20000, CRC(08cf7a93) SHA1(fd3278c3fb3ef30ed03c8a95656d86ba82a163d8) )
	ROM_LOAD( "21.bin",       0x060000, 0x20000, CRC(1ac892f5) SHA1(28364266ca9d1955fb7953f5c2d6f35e114beec6) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.6",     0x000000, 0x20000, CRC(e7ccdf9f) SHA1(80ffcefc95660471124898a9c2bee55df36bda13) )	/* sprites A1 */
	ROM_LOAD( "gaiden.8",     0x020000, 0x20000, CRC(7ef7f880) SHA1(26ba9a76adce24beea3cffa1cb95aeafe6f82f96) )	/* sprites B1 */
	ROM_LOAD( "gaiden.10",    0x040000, 0x20000, CRC(a6451dec) SHA1(553e7a1453b59055fa0b10ca04125543d9f8987c) )	/* sprites C1 */
	ROM_LOAD( "gaiden.12",    0x060000, 0x20000, CRC(90f1e13a) SHA1(3fe9fe62aa9e92c871c791a3b11f96c9a48099a9) )	/* sprites D1 */
	ROM_LOAD( "gaiden.7",     0x080000, 0x20000, CRC(016bec95) SHA1(6a6757c52ca9a2398ea43d1af4a8d5adde6f4cd2) )	/* sprites A2 */
	ROM_LOAD( "gaiden.9",     0x0a0000, 0x20000, CRC(6e9b7fd3) SHA1(c86ff61844fc94c02625bb812b9062d0649c8fdf) )	/* sprites B2 */
	ROM_LOAD( "gaiden.11",    0x0c0000, 0x20000, CRC(7fbfdf5e) SHA1(ab67b72dcadb5f2236d29de751de5bf890a9e423) )	/* sprites C2 */
	ROM_LOAD( "gaiden.13",    0x0e0000, 0x20000, CRC(7d9f5c5e) SHA1(200102532ea9a88c7c708e03f8893c46dff827d1) )	/* sprites D2 */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "gaiden.4",     0x0000, 0x20000, CRC(b0e0faf9) SHA1(2275d2ef5eee356ccf80b9e9644d16fc30a4d107) ) /* samples */
ROM_END

ROM_START( ryukendn )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "ryukendn.1",  0x00000, 0x20000, CRC(6203a5e2) SHA1(8cfe05c483a351e938b067ffa642d515e28605a3) )
	ROM_LOAD16_BYTE( "ryukendn.2",  0x00001, 0x20000, CRC(9e99f522) SHA1(b2277d8934b5e6e2f556aee5092f5d1050774a34) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "ryukendn.3",   0x0000, 0x10000, CRC(6b686b69) SHA1(f0fa553acb3945f8dbbf466073c8bae35a0375ef) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "ryukendn.5",   0x000000, 0x10000, CRC(765e7baa) SHA1(4d0a50f091b284739b6d9a8ceb4f81999da445fc) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "14.bin",       0x000000, 0x20000, CRC(1ecfddaa) SHA1(e71d60ae1a98fe8512498f91cce01c16be9f0871) )
	ROM_LOAD( "15.bin",       0x020000, 0x20000, CRC(1291a696) SHA1(023b05260214adc39bdba81d5e2aa246b6d74a6a) )
	ROM_LOAD( "16.bin",       0x040000, 0x20000, CRC(140b47ca) SHA1(6ffd9b7116658a46a124f9085602d88aa143d829) )
	ROM_LOAD( "17.bin",       0x060000, 0x20000, CRC(7638cccb) SHA1(780d47d3aa248346e0e7abc6e6284542e7392919) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "18.bin",       0x000000, 0x20000, CRC(3fadafd6) SHA1(0cb5387a354c631d5c6aca8f77ecbbc0d175a574) )
	ROM_LOAD( "19.bin",       0x020000, 0x20000, CRC(ddae9d5b) SHA1(108b202ae7ae124a32400a0a404c7d2b614c60bd) )
	ROM_LOAD( "20.bin",       0x040000, 0x20000, CRC(08cf7a93) SHA1(fd3278c3fb3ef30ed03c8a95656d86ba82a163d8) )
	ROM_LOAD( "21.bin",       0x060000, 0x20000, CRC(1ac892f5) SHA1(28364266ca9d1955fb7953f5c2d6f35e114beec6) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "gaiden.6",     0x000000, 0x20000, CRC(e7ccdf9f) SHA1(80ffcefc95660471124898a9c2bee55df36bda13) )	/* sprites A1 */
	ROM_LOAD( "gaiden.8",     0x020000, 0x20000, CRC(7ef7f880) SHA1(26ba9a76adce24beea3cffa1cb95aeafe6f82f96) )	/* sprites B1 */
	ROM_LOAD( "gaiden.10",    0x040000, 0x20000, CRC(a6451dec) SHA1(553e7a1453b59055fa0b10ca04125543d9f8987c) )	/* sprites C1 */
	ROM_LOAD( "shadoww.12a",  0x060000, 0x10000, CRC(9bb07731) SHA1(b799b1958dc9b84797fdab2591e33bd5d28884a3) )	/* sprites D1 */
	ROM_LOAD( "ryukendn.12b", 0x070000, 0x10000, CRC(1773628a) SHA1(e7eacc880f2a4174f17b263bedf8c1bc64007dbd) )	/* sprites D1 */
	ROM_LOAD( "gaiden.7",     0x080000, 0x20000, CRC(016bec95) SHA1(6a6757c52ca9a2398ea43d1af4a8d5adde6f4cd2) )	/* sprites A2 */
	ROM_LOAD( "ryukendn.9a",  0x0a0000, 0x10000, CRC(c821e200) SHA1(5867e5055e16c8739d7699ede5e4a708e4c48895) )	/* sprites B2 */
	ROM_LOAD( "ryukendn.9b",  0x0b0000, 0x10000, CRC(6a6233b3) SHA1(21b8693335496b851628e6b62c6012e6624d13bf) )	/* sprites B2 */
	ROM_LOAD( "gaiden.11",    0x0c0000, 0x20000, CRC(7fbfdf5e) SHA1(ab67b72dcadb5f2236d29de751de5bf890a9e423) )	/* sprites C2 */
	ROM_LOAD( "shadoww.13a",  0x0e0000, 0x10000, CRC(996d2fa5) SHA1(a32526949af3635914927ebbbe684c3de9562a9d) )	/* sprites D2 */
	ROM_LOAD( "ryukendn.13b", 0x0f0000, 0x10000, CRC(1f43c507) SHA1(29f655442c16677855073284c7ab41059c99c497) )	/* sprites D2 */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "gaiden.4",     0x0000, 0x20000, CRC(b0e0faf9) SHA1(2275d2ef5eee356ccf80b9e9644d16fc30a4d107) ) /* samples */
ROM_END

ROM_START( tknight )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "tkni1.bin",    0x00000, 0x20000, CRC(9121daa8) SHA1(06ba7779602df8fae32e859371d27c0dbb8d3430) )
	ROM_LOAD16_BYTE( "tkni2.bin",    0x00001, 0x20000, CRC(6669cd87) SHA1(8888522a3aef76a979ffc80ba457dd49f279abf1) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "tkni3.bin",    0x0000, 0x10000, CRC(15623ec7) SHA1(db43fe6c417117d7cd90a26e12a52efb0e1a5ca6) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni5.bin",    0x000000, 0x10000, CRC(5ed15896) SHA1(87bdddb26934af0b2c4e704e6d85c69a7531aeb1) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni7.bin",    0x000000, 0x80000, CRC(4b4d4286) SHA1(d386aa223eb288ea829c98d3f39279a75dc66b71) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni6.bin",    0x000000, 0x80000, CRC(f68fafb1) SHA1(aeca38eaea2f6dfc484e48ac1114c0c4abaafb9c) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni9.bin",    0x000000, 0x80000, CRC(d22f4239) SHA1(360a9a821faabe911eef407ef85452d8b706538f) )	/* sprites */
	ROM_LOAD( "tkni8.bin",    0x080000, 0x80000, CRC(4931b184) SHA1(864e827ac109c0ee52a898034c021cd5e92ff000) )	/* sprites */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "tkni4.bin",    0x0000, 0x20000, CRC(a7a1dbcf) SHA1(2fee1d9745ce2ab54b0b9cbb6ab2e66ba9677245) ) /* samples */
ROM_END

ROM_START( wildfang )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )	/* 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "1.3st",    0x00000, 0x20000, CRC(ab876c9b) SHA1(b02c822f107df4c9c4f0024998f225c1ddbbd496) )
	ROM_LOAD16_BYTE( "2.5st",    0x00001, 0x20000, CRC(1dc74b3b) SHA1(c99051ebefd6ce666b13ab56c0a10b188f15ec28) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "tkni3.bin",    0x0000, 0x10000, CRC(15623ec7) SHA1(db43fe6c417117d7cd90a26e12a52efb0e1a5ca6) )   /* Audio CPU is a Z80  */

	ROM_REGION( 0x010000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni5.bin",    0x000000, 0x10000, CRC(5ed15896) SHA1(87bdddb26934af0b2c4e704e6d85c69a7531aeb1) )	/* 8x8 tiles */

	ROM_REGION( 0x080000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "14.3a",        0x000000, 0x20000, CRC(0d20c10c) SHA1(209ca4e166d0b91ff99a338e135e5388af2c51f5) )
	ROM_LOAD( "15.3b",        0x020000, 0x20000, CRC(3f40a6b4) SHA1(7486ddfe4b0ac4198512548b74402f4194c804f1) )
	ROM_LOAD( "16.1a",        0x040000, 0x20000, CRC(0f31639e) SHA1(e150db4f617c5fcf505e5ca95d94073c1f6b7d0d) )
	ROM_LOAD( "17.1b",        0x060000, 0x20000, CRC(f32c158e) SHA1(2861754bda37e30799151b5ca73771937edf38a9) )

	ROM_REGION( 0x080000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni6.bin",    0x000000, 0x80000, CRC(f68fafb1) SHA1(aeca38eaea2f6dfc484e48ac1114c0c4abaafb9c) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "tkni9.bin",    0x000000, 0x80000, CRC(d22f4239) SHA1(360a9a821faabe911eef407ef85452d8b706538f) )	/* sprites */
	ROM_LOAD( "tkni8.bin",    0x080000, 0x80000, CRC(4931b184) SHA1(864e827ac109c0ee52a898034c021cd5e92ff000) )	/* sprites */

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )	/* 128k for ADPCM samples - sound chip is OKIM6295 */
	ROM_LOAD( "tkni4.bin",    0x0000, 0x20000, CRC(a7a1dbcf) SHA1(2fee1d9745ce2ab54b0b9cbb6ab2e66ba9677245) ) /* samples */
ROM_END

ROM_START( stratof )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )
	ROM_LOAD16_BYTE( "1.3s",        0x00000, 0x20000, CRC(060822a4) SHA1(82abf6ea64695d2f7b5934ad2487e857648aeecf) )
	ROM_LOAD16_BYTE( "2.4s",        0x00001, 0x20000, CRC(339358fa) SHA1(b662bccc2206ae888ea36f355d44bf98fcd2ee2c) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )
	ROM_LOAD( "a-4b.3",           0x00000, 0x10000, CRC(18655c95) SHA1(8357e0520565a201bb930cadffc759463931ec41) )

	ROM_REGION( 0x1000, REGION_CPU3, 0 )	/* protection NEC D8749 */
	ROM_LOAD( "a-6v.mcu",         0x00000, 0x1000, NO_DUMP )

	ROM_REGION( 0x10000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "b-7a.5",           0x00000, 0x10000, CRC(6d2e4bf1) SHA1(edcf96bbcc109da71e3adbb37d119254d3873b29) )

	ROM_REGION( 0x100000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "b-1b",  0x00000, 0x80000, CRC(781d1bd2) SHA1(680d91ea02f1e9cb911501f595008f46ad77ded4) )

	ROM_REGION( 0x80000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "b-4b",  0x00000, 0x80000, CRC(89468b84) SHA1(af60fe957c98fa3f00623d420a0941a941f5bc6b) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "b-2m",  0x00000, 0x80000, CRC(5794ec32) SHA1(07e78d8bcb2373da77ef9f8cde6a01f384f8bf7e) )
	ROM_LOAD( "b-1m",  0x80000, 0x80000, CRC(b0de0ded) SHA1(45c74d0c58e3e73c79e587722d9fea9f7ba9cb0a) )

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )
	ROM_LOAD( "a-4a.4", 0x00000, 0x20000, CRC(ef9acdcf) SHA1(8d62a666843f0cb22e8926ae18a961052d4f9ed5) )
ROM_END

ROM_START( raiga )
	ROM_REGION( 0x40000, REGION_CPU1, 0 )
	ROM_LOAD16_BYTE( "a-3s.1",      0x00000, 0x20000, CRC(303c2a6c) SHA1(cd825329fd1f7d87661114f07cc87e43fd34e251) )
	ROM_LOAD16_BYTE( "a-4s.2",      0x00001, 0x20000, CRC(5f31fecb) SHA1(b0c88d260d0108100c157ea92f7defdc3cbb8933) )

	ROM_REGION( 0x10000, REGION_CPU2, 0 )
	ROM_LOAD( "a-4b.3",           0x00000, 0x10000, CRC(18655c95) SHA1(8357e0520565a201bb930cadffc759463931ec41) )

	ROM_REGION( 0x1000, REGION_CPU3, 0 )	/* protection NEC D8749 */
	ROM_LOAD( "a-6v.mcu",         0x00000, 0x1000, NO_DUMP )

	ROM_REGION( 0x10000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "b-7a.5",           0x00000, 0x10000, CRC(6d2e4bf1) SHA1(edcf96bbcc109da71e3adbb37d119254d3873b29) )

	ROM_REGION( 0x100000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "b-1b",  0x00000, 0x80000, CRC(781d1bd2) SHA1(680d91ea02f1e9cb911501f595008f46ad77ded4) )

	ROM_REGION( 0x80000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "b-4b",  0x00000, 0x80000, CRC(89468b84) SHA1(af60fe957c98fa3f00623d420a0941a941f5bc6b) )

	ROM_REGION( 0x100000, REGION_GFX4, ROMREGION_DISPOSE )
	ROM_LOAD( "b-2m",  0x00000, 0x80000, CRC(5794ec32) SHA1(07e78d8bcb2373da77ef9f8cde6a01f384f8bf7e) )
	ROM_LOAD( "b-1m",  0x80000, 0x80000, CRC(b0de0ded) SHA1(45c74d0c58e3e73c79e587722d9fea9f7ba9cb0a) )

	ROM_REGION( 0x20000, REGION_SOUND1, 0 )
	ROM_LOAD( "a-4a.4", 0x00000, 0x20000, CRC(ef9acdcf) SHA1(8d62a666843f0cb22e8926ae18a961052d4f9ed5) )
ROM_END



static DRIVER_INIT( shadoww )
{
	gaiden_sprite_sizey = 0;	// sprite size Y = sprite size X
}

static DRIVER_INIT( wildfang )
{
	gaiden_sprite_sizey = 0;	// sprite size Y = sprite size X

	install_mem_read16_handler (0, 0x07a006, 0x07a007, wildfang_protection_r);
	install_mem_write16_handler(0, 0x07a804, 0x07a805, wildfang_protection_w);
}

static DRIVER_INIT( raiga )
{
#if 0
data16_t *rom = (data16_t *)memory_region(REGION_CPU1);
int i;
static data16_t tr[] =
{
	0x7000,0x7200,0x740F,0x47F9,0x0000,0x06B4,0x7E00,0x1E33,
	0x1000,0x4EB9,0x0000,0xD9CE,0x3007,0xE848,0xE848,0xE848,
	0x0240,0x000F,0x0600,0x0030,0x0C00,0x003A,0x6D04,0x0600,
	0x0007,0x3340,0x0800,0x4259,0x3007,0xE848,0xE848,0x0240,
	0x000F,0x0600,0x0030,0x0C00,0x003A,0x6D04,0x0600,0x0007,
	0x3340,0x0800,0x4259,0x3007,0xE848,0x0240,0x000F,0x0600,
	0x0030,0x0C00,0x003A,0x6D04,0x0600,0x0007,0x3340,0x0800,
	0x4259,0x3007,0x0240,0x000F,0x0600,0x0030,0x0C00,0x003A,
	0x6D04,0x0600,0x0007,0x3340,0x0800,0x4259,0xD0FC,0x0010,
	0x2248,0x5201,0x51CA,0xFF76,0x60FE,
	0x2828,0x282b,0x2815,0x281c,0x281e,0x2813,0x2823,0x2825
};
for (i = 0;i < sizeof(tr)/sizeof(tr[0]);i++)
	rom[0x61a/2 + i] = tr[i];
#endif

	gaiden_sprite_sizey = 2;	// sprite size Y *independent* from sprite size X

	install_mem_read16_handler (0, 0x07a006, 0x07a007, raiga_protection_r);
	install_mem_write16_handler(0, 0x07a804, 0x07a805, raiga_protection_w);
}



GAME( 1988, shadoww,  0,        shadoww, shadoww,  shadoww,  ROT0, "Tecmo", "Shadow Warriors (World set 1)" )
GAME( 1988, shadowwa, shadoww,  shadoww, shadoww,  shadoww,  ROT0, "Tecmo", "Shadow Warriors (World set 2)" )
GAME( 1988, gaiden,   shadoww,  shadoww, shadoww,  shadoww,  ROT0, "Tecmo", "Ninja Gaiden (US)" )
GAME( 1989, ryukendn, shadoww,  shadoww, shadoww,  shadoww,  ROT0, "Tecmo", "Ninja Ryukenden (Japan)" )
GAME( 1989, wildfang, 0,        shadoww, wildfang, wildfang, ROT0, "Tecmo", "Wild Fang / Tecmo Knight" )
GAME( 1989, tknight,  wildfang, shadoww, tknight,  wildfang, ROT0, "Tecmo", "Tecmo Knight" )
GAMEX(1991, stratof,  0,        shadoww, raiga,    raiga,    ROT0, "Tecmo", "Raiga - Strato Fighter (US)", GAME_UNEMULATED_PROTECTION )
GAMEX(1991, raiga,    stratof,  shadoww, raiga,    raiga,    ROT0, "Tecmo", "Raiga - Strato Fighter (Japan)", GAME_UNEMULATED_PROTECTION )
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
