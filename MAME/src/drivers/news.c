#pragma code_seg("C454")
#pragma data_seg("D454")
#pragma bss_seg("B454")
#pragma const_seg("K454")
#pragma comment(linker, "/merge:D454=454")
#pragma comment(linker, "/merge:C454=454")
#pragma comment(linker, "/merge:B454=454")
#pragma comment(linker, "/merge:K454=454")
/*

News

from the program ROM:
PROGRAMED BY KWANG-HO CHO
COPYRIGHT(C) 1993
ALL RIGHTS RESERVED BY POBY
Hi-tel ID:poby:


driver by David Haywood

Notes:
- The gfx data cointains pictures for both women and girls, however only the women
  seem to be used. Different ROM set, probably (there's a table at 0x253 containing
  the numbers of the pictures to use).
*/

#include "driver.h"
#include "news.h"


static ADDRESS_MAP_START( readmem, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_READ(MRA8_ROM)
	AM_RANGE(0x8000, 0x8fff) AM_READ(MRA8_RAM)
	AM_RANGE(0xc000, 0xc000) AM_READ(input_port_0_r)
	AM_RANGE(0xc001, 0xc001) AM_READ(input_port_1_r)
	AM_RANGE(0xc002, 0xc002) AM_READ(OKIM6295_status_0_r)
	AM_RANGE(0xe000, 0xffff) AM_READ(MRA8_RAM)
ADDRESS_MAP_END

static ADDRESS_MAP_START( writemem, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_WRITE(MWA8_ROM)	/* 4000-7fff is written to during startup, probably leftover code */
	AM_RANGE(0x8000, 0x87ff) AM_WRITE(news_fgram_w) AM_BASE(&news_fgram)
	AM_RANGE(0x8800, 0x8fff) AM_WRITE(news_bgram_w) AM_BASE(&news_bgram)
	AM_RANGE(0x9000, 0x91ff) AM_WRITE(paletteram_xxxxRRRRGGGGBBBB_swap_w) AM_BASE(&paletteram)
	AM_RANGE(0xc002, 0xc002) AM_WRITE(OKIM6295_data_0_w) /* ?? */
	AM_RANGE(0xc003, 0xc003) AM_WRITE(news_bgpic_w)
	AM_RANGE(0xe000, 0xffff) AM_WRITE(MWA8_RAM)
ADDRESS_MAP_END


INPUT_PORTS_START( news )
	PORT_START	/* DSW */
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPNAME( 0x0c, 0x0c, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(    0x0c, "Easy" )
	PORT_DIPSETTING(    0x08, "Medium" )
	PORT_DIPSETTING(    0x04, "Hard" )
	PORT_DIPSETTING(    0x00, "Hardest" )
	PORT_DIPNAME( 0x10, 0x10, "Helps" )
	PORT_DIPSETTING(    0x10, "1" )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPNAME( 0x20, 0x00, "Copyright" )
	PORT_DIPSETTING(    0x00, "Poby" )
	PORT_DIPSETTING(    0x20, "Virus" )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unused ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unused ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    | IPF_8WAY )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  | IPF_8WAY )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  | IPF_8WAY )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT | IPF_8WAY )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON2 )
INPUT_PORTS_END



static struct GfxLayout tiles8x8_layout =
{
	8,8,
	RGN_FRAC(1,1),
	4,
	{ 0, 1, 2, 3 },
	{ 0, 4, 8, 12, 16, 20, 24, 28 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8
};

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &tiles8x8_layout, 0, 16 },
	{ -1 }
};



static struct OKIM6295interface okim6295_interface =
{
	1,                  /* 1 chip */
	{ 8000 },           /* ? frequency */
	{ REGION_SOUND1 },	/* memory region */
	{ 100 }
};



static MACHINE_DRIVER_START( news )
	/* basic machine hardware */
	MDRV_CPU_ADD(Z80,8000000)		 /* ? MHz */
	MDRV_CPU_PROGRAM_MAP(readmem,writemem)
	MDRV_CPU_VBLANK_INT(irq0_line_hold,1)

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER )
	MDRV_SCREEN_SIZE(256, 256)
	MDRV_VISIBLE_AREA(0, 256-1, 16, 256-16-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(0x100)

	MDRV_VIDEO_START(news)
	MDRV_VIDEO_UPDATE(news)

	/* sound hardware */
	MDRV_SOUND_ADD(OKIM6295, okim6295_interface)
MACHINE_DRIVER_END



ROM_START( news )
	ROM_REGION( 0x10000, REGION_CPU1, 0 )
	ROM_LOAD( "virus.4", 0x00000, 0x08000, BAD_DUMP CRC(aa005dfb) SHA1(52f4dd399a30568851d43d052b83cfaa6682665d)  ) /* The Original was too short, I padded it with 0xFF */

	ROM_REGION( 0x80000, REGION_GFX1, 0 )
	ROM_LOAD16_BYTE( "virus.2", 0x00000, 0x40000, CRC(b5af58d8) SHA1(5dd8c6ab8b53df695463bd0c3620adf8c08daaec) )
	ROM_LOAD16_BYTE( "virus.3", 0x00001, 0x40000, CRC(a4b1c175) SHA1(b1ac0da4d91bc3a3454ea80aa4cdbbc68bbdf7f1) )

	ROM_REGION( 0x40000, REGION_SOUND1, 0 )
	ROM_LOAD( "virus.1", 0x00000, 0x40000, CRC(41f5935a) SHA1(1566d243f165019660cd4dd69df9f049e0130f15) )
ROM_END


GAME( 1993, news, 0, news, news, 0, ROT0, "Poby / Virus", "News" )
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
