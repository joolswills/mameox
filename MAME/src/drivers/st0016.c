#pragma code_seg("C699")
#pragma data_seg("D699")
#pragma bss_seg("B699")
#pragma const_seg("K699")
#pragma comment(linker, "/merge:D699=699")
#pragma comment(linker, "/merge:C699=699")
#pragma comment(linker, "/merge:B699=699")
#pragma comment(linker, "/merge:K699=699")
/*******************************************
      Seta custom ST-0016 chip based games.
	driver by Tomasz Slanina	
********************************************


Todo:
- sound
   Sound registers - $100 bytes @ $e900 ($20x8)
   8 channels ??
   ---------------------------------
  00-03 | start address  
  04-07 | loop start
  08-0b | loop end
  0c-0f | end address
  10-11 | freq (looks like 16.16 fixed point where 0x10000 = 44100, 0x08000 = 22050, 0x04000 = 11025)
  12-13 | unknown (0)
  14-15 | 2x 8bit channel volumes
  16    | flags.  bit 0 = loop, bits 1 + 2 = active (0x06 written to key on one-shot sounds, 0x07 for loops)
  17-1f | unknown
  ----------------------------------
  The last reg ($ff) is probably sound enable/mode

- find NMI source, and NMI enable/disable (timer ? video hw ?)
- nerate chu - missing text (sprites) in test mode (press down+button for video test)
- nerate chu - dip switches

Not working games :
- Super Real Mahjong P5	
- Super Eagle Shot
- Mayjinsen
- Mayjinsen 2

--
Renjyu Kizoku
Visco, 1994

PCB Layout

E51-00001-A
|------------------------------------|
|AMP       UPD6376    424400  62256  |
|    VOL      RESET   424400  62256  |
|                                    |
| TD62064 74273                      |
|J        74273                      |
|A        74273            42.9545MHz|
|M                                   |
|M        74245         UNKNOWN      |
|A        74245          QFP208      |
|         74245                      |
|         74245                 48MHz|
|                                    |
|  74138  74138                      |
|  74253  74253       RNJ2           |
|                                    |
|  DSW1   DSW2        RENJYU-1  6264 |
|------------------------------------|
*/

#include "driver.h"
#include "cpu/z80/z80.h" 
#include "cpu/mips/r3000.h"

WRITE8_HANDLER (st0016_sprite_bank_w);
WRITE8_HANDLER (st0016_palette_bank_w);
WRITE8_HANDLER (st0016_character_bank_w);
READ8_HANDLER  (st0016_sprite_ram_r);
WRITE8_HANDLER (st0016_sprite_ram_w);
READ8_HANDLER  (st0016_sprite2_ram_r);
WRITE8_HANDLER (st0016_sprite2_ram_w);
READ8_HANDLER  (st0016_palette_ram_r);
WRITE8_HANDLER (st0016_palette_ram_w);
READ8_HANDLER  (st0016_character_ram_r);
WRITE8_HANDLER (st0016_character_ram_w);
READ8_HANDLER(st0016_vregs_r);
WRITE8_HANDLER(st0016_vregs_w);

VIDEO_START(st0016);
VIDEO_UPDATE(st0016);
extern int st0016_game;
void st0016_save_init(void);
static int mux_port;
int st0016_rom_bank;
static data32_t *rom_base;

static ADDRESS_MAP_START( st0016_mem, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK(1)
	AM_RANGE(0xc000, 0xcfff) AM_READ(st0016_sprite_ram_r) AM_WRITE(st0016_sprite_ram_w)
	AM_RANGE(0xd000, 0xdfff) AM_READ(st0016_sprite2_ram_r) AM_WRITE(st0016_sprite2_ram_w)
	AM_RANGE(0xe000, 0xe7ff) AM_RAM
	AM_RANGE(0xe800, 0xe87f) AM_RAM /* common ram */
	AM_RANGE(0xe900, 0xe9ff) AM_RAM AM_WRITE(st0016_snd_w) AM_BASE(&st0016_sound_regs) /* sound regs 8 x $20 bytes, see notes */
	AM_RANGE(0xea00, 0xebff) AM_READ(st0016_palette_ram_r) AM_WRITE(st0016_palette_ram_w)
	AM_RANGE(0xec00, 0xec1f) AM_READ(st0016_character_ram_r) AM_WRITE(st0016_character_ram_w)
	AM_RANGE(0xf000, 0xffff) AM_RAM /* work ram */
ADDRESS_MAP_END

static READ8_HANDLER(mux_r)
{
/* 
	76543210
	    xxxx - input port #2
	xxxx     - dip switches (2x8 bits) (multiplexed)
*/	    
	int retval=input_port_2_r(0)&0x0f;
	switch(mux_port&0x30)
	{
		case 0x00: retval|=((input_port_4_r(0)&1)<<4)|((input_port_4_r(0)&0x10)<<1)|((input_port_5_r(0)&1)<<6)|((input_port_5_r(0)&0x10)<<3);break;
		case 0x10: retval|=((input_port_4_r(0)&2)<<3)|((input_port_4_r(0)&0x20)   )|((input_port_5_r(0)&2)<<5)|((input_port_5_r(0)&0x20)<<2);break;
		case 0x20: retval|=((input_port_4_r(0)&4)<<2)|((input_port_4_r(0)&0x40)>>1)|((input_port_5_r(0)&4)<<4)|((input_port_5_r(0)&0x40)<<1);break;
		case 0x30: retval|=((input_port_4_r(0)&8)<<1)|((input_port_4_r(0)&0x80)>>2)|((input_port_5_r(0)&8)<<3)|((input_port_5_r(0)&0x80)   );break;
	}
	return retval;
}

static WRITE8_HANDLER(mux_select_w)
{
	mux_port=data;
}

WRITE8_HANDLER(st0016_rom_bank_w)
{
	cpu_setbank( 1, memory_region(REGION_CPU1) + (data* 0x4000) + 0x10000 );	
	st0016_rom_bank=data;
}

READ8_HANDLER(st0016_dma_r);

static ADDRESS_MAP_START( st0016_io, ADDRESS_SPACE_IO, 8 )
	AM_RANGE(0x00, 0xbf) AM_READ(st0016_vregs_r) AM_WRITE(st0016_vregs_w) /* video/crt regs ? */
	AM_RANGE(0xc0, 0xc0) AM_READ(input_port_0_r) AM_WRITE(mux_select_w) 
	AM_RANGE(0xc1, 0xc1) AM_READ(input_port_1_r) AM_WRITENOP
	AM_RANGE(0xc2, 0xc2) AM_READ(mux_r) AM_WRITENOP
	AM_RANGE(0xc3, 0xc3) AM_READ(input_port_1_r) AM_WRITENOP
	AM_RANGE(0xe0, 0xe0) AM_WRITENOP /* renju = $40, neratte = 0 */
	AM_RANGE(0xe1, 0xe1) AM_WRITE(st0016_rom_bank_w)
	AM_RANGE(0xe2, 0xe2) AM_WRITE(st0016_sprite_bank_w)
	AM_RANGE(0xe3, 0xe4) AM_WRITE(st0016_character_bank_w)
	AM_RANGE(0xe5, 0xe5) AM_WRITE(st0016_palette_bank_w)
	AM_RANGE(0xe6, 0xe6) AM_WRITENOP /* banking ? ram bank ? shared rambank ? */
	AM_RANGE(0xe7, 0xe7) AM_WRITENOP /* watchdog */
	AM_RANGE(0xf0, 0xf0) AM_READ(st0016_dma_r)
ADDRESS_MAP_END


INPUT_PORTS_START( renju )
	PORT_START		
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP | IPF_PLAYER1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN | IPF_PLAYER1 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT | IPF_PLAYER1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x60, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* buttons ? */
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_START1 )
	
	PORT_START	
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP | IPF_PLAYER2 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN | IPF_PLAYER2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT | IPF_PLAYER2 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x60, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* buttons ? */
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_START2 )
		
	PORT_START
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BITX( 0x04, IP_ACTIVE_LOW, IPT_SERVICE | IPF_TOGGLE, DEF_STR ( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	
	PORT_START
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* unused ? */		

	PORT_START		/* DSW1 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNKNOWN ) 	
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "Test mode" )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x08, DEF_STR( On ) )
	PORT_DIPNAME( 0x30, 0x30, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(      0x20, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x30, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x00, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(      0x10, DEF_STR( 1C_2C ) )
	PORT_DIPNAME( 0xc0, 0xc0, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(      0x80, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0xc0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x00, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(      0x80, DEF_STR( 1C_2C ) )

	PORT_START		/* DSW2 */
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(      0x00, "Very Hard" )
	PORT_DIPSETTING(      0x01, "Hard" )
	PORT_DIPSETTING(      0x02, "Easy" )
	PORT_DIPSETTING(      0x03, "Normal" )
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNKNOWN ) 	
INPUT_PORTS_END


INPUT_PORTS_START( nratechu )
	PORT_START		
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP | IPF_PLAYER1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN | IPF_PLAYER1 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT | IPF_PLAYER1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER1 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x60, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* buttons ? */
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_START1 )
	
	PORT_START		
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP | IPF_PLAYER2 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN | IPF_PLAYER2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT | IPF_PLAYER2 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_PLAYER2 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x60, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* buttons ? */
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_START2 )
		
	PORT_START		
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BITX( 0x04, IP_ACTIVE_LOW, IPT_SERVICE | IPF_TOGGLE, DEF_STR ( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN ) 

	PORT_START		
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) /* read in test mode */
	
	PORT_START		/* DSW1 */
	PORT_BIT( 0x7f, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	PORT_DIPNAME( 0x80, 0x00, "Language" )
	PORT_DIPSETTING(    0x00, "English" )
	PORT_DIPSETTING(    0x80, "Japanese" )
		
	PORT_START		/* DSW2 */
	PORT_BIT( 0x7f, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	PORT_DIPNAME( 0x80, 0x80, "Test mode" )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
//	{ 0, 0, &charlayout,      0, 16*4  },
	{ -1 }	/* end of array */
};

INTERRUPT_GEN(st0016_int)
{
	if(!cpu_getiloops())
		cpunum_set_input_line(0,0,HOLD_LINE);
	else
		if(activecpu_get_reg(Z80_IFF1)) /* dirty hack ... */
			cpunum_set_input_line(0, INPUT_LINE_NMI, PULSE_LINE );
}

static struct ST0016interface st0016_interface =
{
	YM3012_VOL(100, MIXER_PAN_LEFT, 100, MIXER_PAN_RIGHT),
};

static MACHINE_DRIVER_START( st0016 )
	/* basic machine hardware */
	MDRV_CPU_ADD_TAG("main",Z80,8000000) /* 8 MHz ? */
	MDRV_CPU_PROGRAM_MAP(st0016_mem,0)
	MDRV_CPU_IO_MAP(st0016_io,0)

	MDRV_CPU_VBLANK_INT(st0016_int,5) /*  4*nmi + int0 */

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(48*8, 48*8)
	MDRV_VISIBLE_AREA(0*8, 48*8-1, 0*8, 48*8-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(16*16*4+1)

	MDRV_VIDEO_START(st0016)
	MDRV_VIDEO_UPDATE(st0016)

	MDRV_SOUND_ATTRIBUTES(SOUND_SUPPORTS_STEREO)
	MDRV_SOUND_ADD(ST0016, st0016_interface)
MACHINE_DRIVER_END

/*
Super Real Mahjong P5
(c)199x SETA

CPU   : R3560(custom?)
SOUND : TC6210AF(custom?)
OSC.  : 50.0000MHz 42.9545MHz

SX008-01.BIN ; CHR ROM
SX008-02.BIN ;  |
SX008-03.BIN ;  |
SX008-04.BIN ;  |
SX008-05.BIN ;  |
SX008-06.BIN ;  |
SX008-07.BIN ; /
SX008-08.BIN ; SOUND DATA
SX008-09.BIN ; /
SX008-11.BIN ; MAIN PRG
SX008-12.BIN ;  |
SX008-13.BIN ;  |
SX008-14.BIN ; /

*/

static struct r3000_config config =
{
	0,	/* 1 if we have an FPU, 0 otherwise */
	4096,	/* code cache size */
	4096	/* data cache size */
};

static ADDRESS_MAP_START( srmp5_mem, ADDRESS_SPACE_PROGRAM, 32 )
	AM_RANGE(0x00000000, 0x001fffff) AM_RAM
	AM_RANGE(0x002f0000, 0x002f7fff) AM_RAM
	AM_RANGE(0x01000000, 0x01000003) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01800000, 0x01800003) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01800004, 0x01800007) AM_READ(MRA32_RAM)
	AM_RANGE(0x01800008, 0x0180000b) AM_READ(MRA32_RAM)
	AM_RANGE(0x0180000c, 0x0180000f) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01800014, 0x01800017) AM_READ(MRA32_RAM)
	AM_RANGE(0x0180001c, 0x0180001f) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01800200, 0x01800203) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01802000, 0x01802003) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01802004, 0x01802007) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01802008, 0x0180200b) AM_READ(MRA32_RAM)
	AM_RANGE(0x01a00200, 0x01a00223) AM_READ(MRA32_RAM)
	AM_RANGE(0x01c00000, 0x01c00003) AM_READ(MRA32_RAM)
	AM_RANGE(0x0a000000, 0x0a00ffff) AM_RAM
	AM_RANGE(0x0a100000, 0x0a105fff) AM_RAM
	AM_RANGE(0x0a180000, 0x0a1801ff) AM_RAM
	AM_RANGE(0x0a200000, 0x0a3fffff) AM_RAM
	AM_RANGE(0x1eff0000, 0x1eff0013) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x1fc00000, 0x1fdfffff) AM_READ(MRA32_ROM) AM_WRITE(MWA32_ROM) AM_BASE(&rom_base)
ADDRESS_MAP_END

static MACHINE_DRIVER_START( srmp5 )
	MDRV_IMPORT_FROM(st0016) 
	MDRV_CPU_ADD(R3000BE, 16000000)
	MDRV_CPU_CONFIG(config) 
	MDRV_CPU_PROGRAM_MAP(srmp5_mem,0)
MACHINE_DRIVER_END

INPUT_PORTS_START( srmp5 )
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BITX( 0x04, IP_ACTIVE_LOW, IPT_SERVICE | IPF_TOGGLE, DEF_STR ( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN ) 

	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
INPUT_PORTS_END	

/*
Super Eagle Shot
(c)1993 Seta (distributed by Visco)
E30-001A

CPU  : Integrated Device IDT79R3051-25J 9407C (R3000A)
Sound: ST-0016
OSC  : 50.0000MHz (X1) 42.9545MHz (X3)

ROMs:
SX004-01.PR0 - Main programs (MX27C4000)
SX004-02.PR1 |
SX004-03.PR2 |
SX004-04.PR3 /

SX004-05.RD0 - Graphics? (D23C8000SCZ)
SX004-06.RD1 /

SX004-07.ZPR - Program? (16M mask, read as 27c160)

GALs (not dumped):
SX004-08.27 (16V8B)
SX004-09.46 (16V8B)
SX004-10.59 (16V8B)
SX004-11.61 (22V10B)
SX004-12.62 (22V10B)
SX004-13.63 (22V10B)

Custom chips:
SETA ST-0015 60EN502F12 JAPAN 9415YAI (U18, 208pin PQFP, system controller)
SETA ST-0016 TC6187AF JAPAN 9348YAA (U68, 208pin PQFP, sound & object)
                                                
   ?      ST-0015              SX004-01   49.9545MHz    ST-0016       5588-25
                               SX004-02      52B256-70  514256 514256 
 50MHz                         SX004-03      52B256-70  SX004-07
 528257-70 514256-70 514256-70 SX004-04
 528257-70 514256-70 514256-70 SX004-05
 528257-70 514256-70 514256-70 SX004-06
           514256-70 514256-70
                                                NEC D6376
                                                
*/

static ADDRESS_MAP_START( speglsht_mem, ADDRESS_SPACE_PROGRAM, 32 )
	AM_RANGE(0x00000000, 0x000fffff) AM_RAM
	AM_RANGE(0x01800200, 0x01800203) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x01a00000, 0x01afffff) AM_RAM
	AM_RANGE(0x01c00000, 0x01c00003) AM_READ(MRA32_RAM)
	AM_RANGE(0x01ca5000, 0x01ca6fff) AM_RAM /* i/o ?? */
	AM_RANGE(0x0a000000, 0x0a003fff) AM_RAM
	AM_RANGE(0x1eff0000, 0x1eff001f) AM_WRITE(MWA32_RAM)
	AM_RANGE(0x1fc00000, 0x1fdfffff) AM_READ(MRA32_ROM) AM_WRITE(MWA32_ROM) AM_BASE(&rom_base)
ADDRESS_MAP_END

static MACHINE_DRIVER_START( speglsht )
	MDRV_IMPORT_FROM(st0016) 
	MDRV_CPU_ADD(R3000BE, 16000000)
	MDRV_CPU_CONFIG(config) 
	MDRV_CPU_PROGRAM_MAP(speglsht_mem,0)
MACHINE_DRIVER_END

INPUT_PORTS_START( speglsht )
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BITX( 0x04, IP_ACTIVE_LOW, IPT_SERVICE | IPF_TOGGLE, DEF_STR ( Service_Mode ), KEYCODE_F2, IP_JOY_NONE )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN ) 

	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0x7f, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	PORT_DIPNAME( 0x80, 0x80, "Test mode" )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END	

/*

Mayjinsen (JPN Ver.)
(c)1994 Seta

CPU:	UPD70732-25 V810 ?
Sound:	Custom (ST-0016 ?)

sx003.01	main prg
sx003.02
sx003.03
sx003.04	/

sx003.05d	chr
sx003.06
sx003.07d	/

-----------

Mayjinsen II
Seta, 1994

This game runs on Seta hardware. The game is similar to Shougi.

PCB Layout
----------

E52-00001
|----------------------------------------------------|
|                  62256    62256    62256    62256  |
| D70732GD-25                                        |
| NEC 1991 V810    62256    62256    62256    62256  |
|                                                    |
|                  62256    62256    62256    62256  |
|                                                    |
|                SX007-01 SX007-02  SX007-03 SX007-04|
|                                                    |
|                                   6264             |
|                                                    |
|                   62256      42.9545MHz  48MHz     |
|      PAL                                           |
|                   62256                            |
| 46MHz                          ST-0016   SX007-05  |
|                                TC6210AF            |
|                                                    |
|                   TC514800                         |
|                                                    |
|                                           DSW1-8   |
|                                                    |
|                                           DSW2-8   |
|                           JAMMA                    |
|----------------------------------------------------|

*/

INPUT_PORTS_START( mayjinsn )
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_VBLANK ) /* hack to pass some checks */
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_VBLANK )
	
	PORT_BIT( 0x0c, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
INPUT_PORTS_END	

INPUT_PORTS_START( mayjisn2 )
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_VBLANK ) /* hack to pass some checks */
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_VBLANK )
	PORT_BIT( 0x0c, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
 
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
	
	PORT_START	
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNKNOWN ) 
INPUT_PORTS_END	

/**********************************************************/

ROM_START( renju )
	ROM_REGION( 0x290000, REGION_CPU1, 0 )
	ROM_LOAD( "renjyu-1.u31",0x010000, 0x200000, CRC(e0fdbe9b) SHA1(52d31024d1a88b8fcca1f87366fcaf80e3c387a1) )
	ROM_LOAD( "rnj2.u32",    0x210000, 0x080000, CRC(2015289c) SHA1(5223b6d3dbe4657cd63cf5b527eaab84cf23587a ) )
	ROM_COPY( REGION_CPU1,   0x210000, 0x000000, 0x08000 )
ROM_END

ROM_START( nratechu )
	ROM_REGION( 0x210000, REGION_CPU1, 0 )
	ROM_LOAD( "sx012-01",   0x10000, 0x80000,   CRC(6ca01d57) SHA1(065848f19ecf2dc1f7bbc7ddd87bca502e4b8b16) )
	ROM_LOAD( "sx012-02",   0x110000, 0x100000, CRC(40a4e354) SHA1(8120ce8deee6805050a5b083a334c3743c09566b) )
	ROM_COPY( REGION_CPU1,  0x10000, 0x00000, 0x08000 )
ROM_END				   

ROM_START( srmp5 )
	ROM_REGION( 0x210000, REGION_CPU1, 0 )
	ROM_LOAD( "sx008-08.bin",   0x10000, 0x200000,   CRC(d4ac54f4) SHA1(c3dc76cd71485796a0b6a960294ea96eae8c946e) )
	ROM_COPY( REGION_CPU1,  0x10000, 0x00000, 0x08000 )
	
	ROM_REGION( 0x200000, REGION_CPU2, 0 )
	
	ROM_REGION32_BE( 0x200000, REGION_USER1, ROMREGION_DISPOSE )
	ROM_LOAD32_BYTE( "sx008-14.bin",   0x00000, 0x80000,   CRC(b5c55120) SHA1(0a41351c9563b2c6a00709189a917757bd6e0a24) )
	ROM_LOAD32_BYTE( "sx008-13.bin",   0x00001, 0x80000,   CRC(0af475e8) SHA1(24cddffa0f8c81832ae8870823d772e3b7493194) )
	ROM_LOAD32_BYTE( "sx008-12.bin",   0x00002, 0x80000,   CRC(43e9bb98) SHA1(e46dd98d2e1babfa12ddf2fa9b31377e8691d3a1) )
	ROM_LOAD32_BYTE( "sx008-11.bin",   0x00003, 0x80000,   CRC(ca15ff45) SHA1(5ee610e0bb835568c36898210a6f8394902d5b54) )
	
	ROM_REGION( 0x200000*8, REGION_USER2,0) /* gfx ? */
	ROM_LOAD( "sx008-01.bin",   0x000000, 0x200000,   CRC(82dabf48) SHA1(c53e9ed0056c431eab13ab362936c25d3cc5abba) )
	ROM_LOAD( "sx008-02.bin",   0x200000, 0x200000,   CRC(cfd2be0f) SHA1(a21f2928e08047c97443123aceba7ff4e95c6d3d) )
	ROM_LOAD( "sx008-03.bin",   0x400000, 0x200000,   CRC(d7323b10) SHA1(94ecc17b6b8b071cf2c61bbef4aec2c6c7693c62) )
	ROM_LOAD( "sx008-04.bin",   0x600000, 0x200000,   CRC(b10d3067) SHA1(21c36307780d4f38ec54d87cd222d65e4f8c00a5) )
	ROM_LOAD( "sx008-05.bin",   0x800000, 0x200000,   CRC(0ff5e6f5) SHA1(ab7d021757f341d28db6d7d009c20ec9d7bd83c1) )
	ROM_LOAD( "sx008-06.bin",   0xa00000, 0x200000,   CRC(ba6fd7c4) SHA1(f086195c5c647e07e77ce2a23e94d28e6ad9ff4f) )
	ROM_LOAD( "sx008-07.bin",   0xc00000, 0x200000,   CRC(3564485d) SHA1(12464de4e2b6c4df1595183996d1987f0ecffb01) )
	ROM_LOAD( "sx008-09.bin",   0xe00000, 0x200000,   CRC(5a3e6560) SHA1(92ea398f3c5e3035869f0ca5dfe7b05c90095318) )
ROM_END

ROM_START( speglsht )
	ROM_REGION( 0x210000, REGION_CPU1, 0 )
	ROM_LOAD( "sx004-07",   0x10000, 0x200000,   CRC(2d759cc4) SHA1(9fedd829190b2aab850b2f1088caaec91e8715dd) )
	ROM_COPY( REGION_CPU1,  0x10000, 0x00000, 0x08000 )
	
	ROM_REGION( 0x200000, REGION_CPU2, 0 )
	
	ROM_REGION32_BE( 0x200000, REGION_USER1, ROMREGION_DISPOSE )
	ROM_LOAD32_BYTE( "sx004-04.u33",   0x00000, 0x80000,   CRC(e46d2e57) SHA1(b1fb836ab2ce547dc2e8d1046d7ef835b87bb04e) )
	ROM_LOAD32_BYTE( "sx004-03.u32",   0x00001, 0x80000,   CRC(c6ffb00e) SHA1(f57ef45bb5c690c3e63101a36835d2687abfcdbd) )
	ROM_LOAD32_BYTE( "sx004-02.u31",   0x00002, 0x80000,   CRC(21eb46e4) SHA1(0ab21ed012c9a76e01c83b60c6f4670836dfa718) )
	ROM_LOAD32_BYTE( "sx004-01.u30",   0x00003, 0x80000,   CRC(65646949) SHA1(74931c230f4e4b1008fbc5fba169292e216aa23b) )
	
	ROM_REGION( 0x200000, REGION_USER2,0) 
	ROM_LOAD( "sx004-05",   0x000000, 0x100000,   CRC(4b8ae72b) SHA1(261495edf41d7797d2ea8d442ac6a8362bafadf2) )
	ROM_LOAD( "sx004-06",   0x100000, 0x100000,   CRC(5af78e44) SHA1(0131d50348fef80c2b100d74b7c967c6a710d548) )
ROM_END

ROM_START( speglsha )
	ROM_REGION( 0x210000, REGION_CPU1, 0 )
	ROM_LOAD( "sx004-07",   0x10000, 0x200000,   CRC(2d759cc4) SHA1(9fedd829190b2aab850b2f1088caaec91e8715dd) )
	ROM_COPY( REGION_CPU1,  0x10000, 0x00000, 0x08000 )
	
	ROM_REGION( 0x200000, REGION_CPU2, 0 )
	
	ROM_REGION32_BE( 0x200000, REGION_USER1, ROMREGION_DISPOSE )
	ROM_LOAD32_BYTE( "sx004-04.u33",   0x00000, 0x80000,   CRC(e46d2e57) SHA1(b1fb836ab2ce547dc2e8d1046d7ef835b87bb04e) )
	ROM_LOAD32_BYTE( "sx004-03.u32",   0x00001, 0x80000,   CRC(c6ffb00e) SHA1(f57ef45bb5c690c3e63101a36835d2687abfcdbd) )
	ROM_LOAD32_BYTE( "sx004-02.u31",   0x00002, 0x80000,   CRC(21eb46e4) SHA1(0ab21ed012c9a76e01c83b60c6f4670836dfa718) )
	ROM_LOAD32_BYTE( "sx004-01.u30",   0x00003, 0x80000,   CRC(65646949) SHA1(74931c230f4e4b1008fbc5fba169292e216aa23b) )
	
	ROM_REGION( 0x200000, REGION_USER2,0)  
	ROM_LOAD( "sx004-05.rd0",   0x000000, 0x100000,   CRC(f3c69468) SHA1(81daef6d0596cb67bb6f87b39874aae1b1ffe6a6) )
	ROM_LOAD( "sx004-06",       0x100000, 0x100000,   CRC(5af78e44) SHA1(0131d50348fef80c2b100d74b7c967c6a710d548) )
ROM_END

ROM_START(mayjinsn )
	ROM_REGION( 0x190000, REGION_CPU1, 0 )
	ROM_LOAD( "sx003.05d",   0x010000, 0x80000,  CRC(2be6d620) SHA1(113db888fb657d45be55708bbbf9a9ac159a9636) )
	ROM_LOAD( "sx003.06",    0x090000, 0x80000,  CRC(f0553386) SHA1(8915cb3ce03b9a12612694caec9bbec6de4dd070) )
	ROM_LOAD( "sx003.07d",   0x110000, 0x80000, CRC(8db281c3) SHA1(f8b488dd28010f01f789217a4d62ba2116e06e94) )
	ROM_COPY( REGION_CPU1,   0x010000, 0x00000, 0x08000 )

	ROM_REGION32_BE( 0x20000*4, REGION_USER1, 0 ) /* V810 code */
	ROM_LOAD32_BYTE( "sx003.04",   0x00000, 0x20000,   CRC(fa15459f) SHA1(4163ab842943705c550f137abbdd2cb51ba5390f) )
	ROM_LOAD32_BYTE( "sx003.03",   0x00001, 0x20000,   CRC(71a438ea) SHA1(613bab6a59aa1bced2ab37177c61a0fd7ce7e64f) )
	ROM_LOAD32_BYTE( "sx003.02",   0x00002, 0x20000,   CRC(61911eed) SHA1(1442b3867b85120ba652ec8205d74332addffb67) )
	ROM_LOAD32_BYTE( "sx003.01",   0x00003, 0x20000,   CRC(d210bfe5) SHA1(96d9f2b198d98125df4bd6b15705646d472a8a87) )
ROM_END

ROM_START(mayjisn2 )
	ROM_REGION( 0x110000, REGION_CPU1, 0 )
	ROM_LOAD( "sx007-05.8b",   0x10000, 0x100000,  CRC(b13ea605) SHA1(75c067df02c988f170c24153d3852c472355fc9d) )
	ROM_COPY( REGION_CPU1,  0x10000, 0x00000, 0x08000 )

	ROM_REGION32_BE( 0x20000*4, REGION_USER1, 0 ) /* V810 code */
	ROM_LOAD32_BYTE( "sx007-04.4b",   0x00000, 0x20000,   CRC(fa15459f) SHA1(4163ab842943705c550f137abbdd2cb51ba5390f) )
	ROM_LOAD32_BYTE( "sx007-03.4j",   0x00001, 0x20000,   CRC(71a438ea) SHA1(613bab6a59aa1bced2ab37177c61a0fd7ce7e64f) )
	ROM_LOAD32_BYTE( "sx007-02.4m",   0x00002, 0x20000,   CRC(61911eed) SHA1(1442b3867b85120ba652ec8205d74332addffb67) )
	ROM_LOAD32_BYTE( "sx007-01.4s",   0x00003, 0x20000,   CRC(d210bfe5) SHA1(96d9f2b198d98125df4bd6b15705646d472a8a87) )
ROM_END


static DRIVER_INIT(renju)
{
	st0016_game=0;
}

static DRIVER_INIT(nratechu)
{
	st0016_game=1;
}

static DRIVER_INIT(srmp5)
{
	st0016_game=2;
	memcpy(rom_base, memory_region(REGION_USER1), memory_region_length(REGION_USER1));
}

static DRIVER_INIT(speglsht)
{
	st0016_game=3;
	memcpy(rom_base, memory_region(REGION_USER1), memory_region_length(REGION_USER1));
}

static DRIVER_INIT(mayjinsn)
{
	st0016_game=4;
}


GAME( 1994, renju,     0,     	  st0016,   renju,     renju,    ROT0, "Visco", "Renju Kizoku")
GAME( 1996, nratechu,  0,     	  st0016,   nratechu,  nratechu, ROT0, "Seta",  "Neratte Chu")
/* Not working */
GAMEX( 199?, srmp5,  	0,     	  srmp5,    srmp5,     srmp5,    ROT0, "Seta",  "Super Real Mahjong P5",GAME_NOT_WORKING)
GAMEX( 1994, speglsht,	0,     	  speglsht, speglsht,  speglsht, ROT0, "Seta",  "Super Eagle Shot (set 1)",GAME_NOT_WORKING)
GAMEX( 1994, speglsha,	speglsht, speglsht, speglsht,  speglsht, ROT0, "Seta",  "Super Eagle Shot (set 2)",GAME_NOT_WORKING)
GAMEX( 1994, mayjinsn,	0, 	  st0016,   mayjinsn,  mayjinsn, ROT0, "Seta",  "Mayjinsen",GAME_NOT_WORKING)
GAMEX( 1994, mayjisn2,	0, 	  st0016,   mayjisn2,  mayjinsn, ROT0, "Seta",  "Mayjinsen 2",GAME_NOT_WORKING)
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
