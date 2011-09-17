#pragma code_seg("C558")
#pragma data_seg("D558")
#pragma bss_seg("B558")
#pragma const_seg("K558")
#pragma comment(linker, "/merge:D558=558")
#pragma comment(linker, "/merge:C558=558")
#pragma comment(linker, "/merge:B558=558")
#pragma comment(linker, "/merge:K558=558")
/*

Pinball Champ '95

rom will be banked

seems to have some gambling elements to it - should i care?

*/

#include "driver.h"


static ADDRESS_MAP_START( readmem, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_READ(MRA8_ROM)

	AM_RANGE(0x8000, 0xafff) AM_READ(MRA8_RAM) //?

	AM_RANGE(0xc000, 0xcfff) AM_READ(MRA8_RAM)
	AM_RANGE(0xd000, 0xdfff) AM_READ(MRA8_RAM)
	AM_RANGE(0xe000, 0xefff) AM_READ(MRA8_RAM)
	AM_RANGE(0xf000, 0xffff) AM_READ(MRA8_RAM)
ADDRESS_MAP_END

static ADDRESS_MAP_START( writemem, ADDRESS_SPACE_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_WRITE(MWA8_ROM)

	AM_RANGE(0x8000, 0xafff) AM_WRITE(MWA8_RAM) //?

	AM_RANGE(0xc000, 0xcfff) AM_WRITE(MWA8_RAM)
	AM_RANGE(0xd000, 0xdfff) AM_WRITE(MWA8_RAM)
	AM_RANGE(0xe000, 0xefff) AM_WRITE(MWA8_RAM)
	AM_RANGE(0xf000, 0xffff) AM_WRITE(MWA8_RAM)
ADDRESS_MAP_END


INPUT_PORTS_START( pbchmp95 )
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
	RGN_FRAC(1,2),
	4,
	{ 0, 1, 2,3 },
	{ 0, 4, RGN_FRAC(1,2)+0, RGN_FRAC(1,2)+4, 8, 12, RGN_FRAC(1,2)+8, RGN_FRAC(1,2)+12},
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16 },
	16*8
};

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &tiles8x8_layout, 0, 16 },
	{ REGION_GFX2, 0, &tiles8x8_layout, 0, 16 },
	{ -1 }
};



static struct OKIM6295interface okim6295_interface =
{
	1,                  /* 1 chip */
	{ 8000 },           /* ? frequency */
	{ REGION_SOUND1 },	/* memory region */
	{ 100 }
};

VIDEO_START(pbchmp95)
{
	return 0;
}

VIDEO_UPDATE(pbchmp95)
{

}

static MACHINE_DRIVER_START( pbchmp95 )
	/* basic machine hardware */
	MDRV_CPU_ADD(Z80,8000000)		 /* ? MHz */
	MDRV_CPU_PROGRAM_MAP(readmem,writemem)
	MDRV_CPU_VBLANK_INT(irq0_line_hold,1)

	/* 2nd z80 */

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER )
	MDRV_SCREEN_SIZE(256, 256)
	MDRV_VISIBLE_AREA(0, 256-1, 16, 256-16-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(0x100)

	MDRV_VIDEO_START(pbchmp95)
	MDRV_VIDEO_UPDATE(pbchmp95)

	/* sound hardware */
	MDRV_SOUND_ADD(OKIM6295, okim6295_interface)
MACHINE_DRIVER_END


ROM_START( pbchmp95 )
	ROM_REGION( 0x28000, REGION_CPU1, 0 )
	ROM_LOAD( "3.bin", 0x00000, 0x08000, CRC(e881aa05) SHA1(10d259396cac4b9a1b72c262c11ffa5efbdac433)  )
	ROM_CONTINUE ( 0x10000, 0x18000)

	ROM_REGION( 0x20000, REGION_GFX1, 0 )
	ROM_LOAD( "2.bin", 0x00000, 0x20000,  CRC(7007ced4) SHA1(6a0aac3ff9a4d5360c8ba1142f010add1b430ada)  )

	ROM_REGION( 0x40000, REGION_GFX2, 0 )
	ROM_LOAD( "1.bin", 0x00000, 0x40000,  CRC(f6cf7ed6) SHA1(327580a17eb2740fad974a01d97dad0a4bef9881)  )

	ROM_REGION( 0x20000, REGION_CPU2, 0 )
	ROM_LOAD( "4.bin", 0x00000, 0x08000, CRC(82460b82) SHA1(d85a9d77edaa67dfab8ff6ac4cb6273f0904b3c0)  )

	ROM_REGION( 0x40000, REGION_SOUND1, 0 )
	ROM_LOAD( "5.bin", 0x00000, 0x40000, CRC(62e42371) SHA1(5042abc2176d0c35fd6b698eca4145f93b0a3944) )
ROM_END


GAMEX( 1995, pbchmp95, 0, pbchmp95, pbchmp95, 0, ROT0, "Veltmeijer Automaten", "Pinball Champ '95", GAME_NOT_WORKING )
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
