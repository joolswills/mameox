#pragma code_seg("C483")
#pragma data_seg("D483")
#pragma bss_seg("B483")
#pragma const_seg("K483")
#pragma comment(linker, "/merge:D483=483")
#pragma comment(linker, "/merge:C483=483")
#pragma comment(linker, "/merge:B483=483")
#pragma comment(linker, "/merge:K483=483")
/*******************************************************************************

	Shoot Out (USA) 			(c) 1985 Data East USA		DE-0219
	Shoot Out (Japan)			(c) 1985 Data East USA		DE-0203
	Shoot Out (Korean bootleg)	(c) 1985 Data East USA		DE-0203 bootleg

	Shoot Out (Japan) is an interesting board, it runs on an earlier PCB design
	than the USA version, has no sound CPU, uses half as many sprites and
	unusually for a Deco Japanese game it is credited to 'Data East USA'.
	Perhaps the USA arm of Deco designed this game rather than the Japanese
	arm?

	Shoot Out (Japan) uses the YM2203 ports for CPU bankswitching so it does
	not work with sound turned off.

	Shoot Out (Korean bootleg) is based on the earlier DE-0203 board but
	strangely features the same encryption as used on the DE-0219 board.  It
	also has some edited graphics.

	Driver by:
		Ernesto Corvi (ernesto@imagina.com)
		Phil Stroffolino
		Shoot Out (Japan) and fixes added by Bryan McPhail (mish@tendril.co.uk)

	Todo:
	- Add cocktail support to shootoub/shootouj.

*******************************************************************************/

/*
	
	2003-06-01	Added cocktail support to shootout

*/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "cpu/m6502/m6502.h"

/* externals: from vidhrdw */
unsigned char *shootout_textram;
WRITE_HANDLER( shootout_videoram_w );
WRITE_HANDLER( shootout_textram_w );

VIDEO_START( shootout );
VIDEO_UPDATE( shootout );
VIDEO_UPDATE( shootouj );

PALETTE_INIT( shootout );

/*******************************************************************************/

static WRITE_HANDLER( shootout_bankswitch_w )
{
	int bankaddress;
	unsigned char *RAM;

	RAM = memory_region(REGION_CPU1);
	bankaddress = 0x10000 + ( 0x4000 * (data & 0x0f) );

	cpu_setbank(1,&RAM[bankaddress]);
}

static WRITE_HANDLER( sound_cpu_command_w )
{
	soundlatch_w( offset, data );
	cpu_set_irq_line( 1, IRQ_LINE_NMI, PULSE_LINE );
}

static WRITE_HANDLER( shootout_flipscreen_w )
{
	flip_screen_set(data);
}

/* stub for reading input ports as active low (makes building ports much easier) */
static READ_HANDLER( low_input_r )
{
	return ~readinputport( offset );
}

static WRITE_HANDLER( shootout_coin_counter_w )
{
	coin_counter_w( offset, data );
}

/*******************************************************************************/

static MEMORY_READ_START( readmem )
	{ 0x0000, 0x0fff, MRA_RAM },
	{ 0x1000, 0x1003, low_input_r },
	{ 0x2000, 0x27ff, MRA_RAM },	/* foreground */
	{ 0x2800, 0x2fff, MRA_RAM },	/* background */
	{ 0x4000, 0x7fff, MRA_BANK1 },
	{ 0x8000, 0xffff, MRA_ROM },
MEMORY_END

static MEMORY_WRITE_START( writemem )
	{ 0x0000, 0x0fff, MWA_RAM },
	{ 0x1000, 0x1000, shootout_bankswitch_w },
	{ 0x1001, 0x1001, shootout_flipscreen_w },
	{ 0x1002, 0x1002, shootout_coin_counter_w },
	{ 0x1003, 0x1003, sound_cpu_command_w },
	{ 0x1004, 0x17ff, MWA_RAM },
	{ 0x1800, 0x19ff, MWA_RAM, &spriteram, &spriteram_size },
	{ 0x2000, 0x27ff, shootout_textram_w, &shootout_textram },
	{ 0x2800, 0x2fff, shootout_videoram_w, &videoram },
	{ 0x4000, 0xffff, MWA_ROM },
MEMORY_END

static MEMORY_READ_START( readmem_alt )
	{ 0x0000, 0x0fff, MRA_RAM },
	{ 0x1000, 0x1003, low_input_r },
	{ 0x2000, 0x21ff, MRA_RAM },
	{ 0x2800, 0x2800, YM2203_status_port_0_r },
	{ 0x3000, 0x37ff, MRA_RAM },	/* foreground */
	{ 0x3800, 0x3fff, MRA_RAM },	/* background */
	{ 0x4000, 0x7fff, MRA_BANK1 },
	{ 0x8000, 0xffff, MRA_ROM },
MEMORY_END

static MEMORY_WRITE_START( writemem_alt )
	{ 0x0000, 0x0fff, MWA_RAM },
	{ 0x1800, 0x1800, shootout_coin_counter_w },
	{ 0x2000, 0x21ff, MWA_RAM, &spriteram, &spriteram_size },
	{ 0x2800, 0x2800, YM2203_control_port_0_w },
	{ 0x2801, 0x2801, YM2203_write_port_0_w },
	{ 0x3000, 0x37ff, shootout_textram_w, &shootout_textram },
	{ 0x3800, 0x3fff, shootout_videoram_w, &videoram },
	{ 0x4000, 0xffff, MWA_ROM },
MEMORY_END

/*******************************************************************************/

static MEMORY_READ_START( sound_readmem )
	{ 0x0000, 0x07ff, MRA_RAM },
	{ 0x4000, 0x4000, YM2203_status_port_0_r },
	{ 0xa000, 0xa000, soundlatch_r },
	{ 0xc000, 0xffff, MRA_ROM },
MEMORY_END

static MEMORY_WRITE_START( sound_writemem )
	{ 0x0000, 0x07ff, MWA_RAM },
	{ 0x4000, 0x4000, YM2203_control_port_0_w },
	{ 0x4001, 0x4001, YM2203_write_port_0_w },
	{ 0xd000, 0xd000, interrupt_enable_w },
	{ 0xc000, 0xffff, MWA_ROM },
MEMORY_END

/*******************************************************************************/

INPUT_PORTS_START( shootout )
	PORT_START	/* DSW1 */
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(	0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(	0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(	0x01, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(	0x02, DEF_STR( 1C_3C ) )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(	0x0c, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(	0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(	0x04, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(	0x08, DEF_STR( 1C_3C ) )
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(	0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(	0x10, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(	0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(	0x20, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(	0x40, DEF_STR( Upright ) )
	PORT_DIPSETTING(	0x00, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x80, 0x00, "Freeze" )
	PORT_DIPSETTING(	0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(	0x80, DEF_STR( On ) )

	PORT_START
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_8WAY )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  | IPF_8WAY )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP    | IPF_8WAY )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN  | IPF_8WAY )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_BUTTON2 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_START2 )

	PORT_START
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_COCKTAIL )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  | IPF_COCKTAIL )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP    | IPF_COCKTAIL )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN  | IPF_COCKTAIL )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 | IPF_COCKTAIL )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_BUTTON2 | IPF_COCKTAIL )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_COIN2 )

	PORT_START	/* DSW2 */
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(	0x02, "1" )
	PORT_DIPSETTING(	0x00, "3" )
	PORT_DIPSETTING(	0x01, "5" )
	PORT_BITX(0,		0x03, IPT_DIPSWITCH_SETTING | IPF_CHEAT, "Infinite", IP_KEY_NONE, IP_JOY_NONE )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(	0x00, "20k 70k" )
	PORT_DIPSETTING(	0x04, "30k 80k" )
	PORT_DIPSETTING(	0x08, "40k 90k" )
	PORT_DIPSETTING(	0x0c, "70k" )
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(	0x00, "Easy" )
	PORT_DIPSETTING(	0x10, "Medium" )
	PORT_DIPSETTING(	0x20, "Hard" )
	PORT_DIPSETTING(	0x30, "Hardest" )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNKNOWN ) /* this is set when either coin is inserted */
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_VBLANK )
INPUT_PORTS_END


static struct GfxLayout char_layout =
{
	8,8,	/* 8*8 characters */
	0x400,	/* 1024 characters */
	2,	/* 2 bits per pixel */
	{ 0,4 },	/* the bitplanes are packed in the same byte */
	{ (0x2000*8)+0, (0x2000*8)+1, (0x2000*8)+2, (0x2000*8)+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8 /* every char takes 8 consecutive bytes */
};
static struct GfxLayout sprite_layout =
{
	16,16,	/* 16*16 sprites */
	0x800,	/* 2048 sprites */
	3,	/* 3 bits per pixel */
	{ 0*0x10000*8, 1*0x10000*8, 2*0x10000*8 },	/* the bitplanes are separated */
	{ 128+0, 128+1, 128+2, 128+3, 128+4, 128+5, 128+6, 128+7, 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
	32*8	/* every char takes 32 consecutive bytes */
};
static struct GfxLayout tile_layout =
{
	8,8,	/* 8*8 characters */
	0x800,	/* 2048 characters */
	2,	/* 2 bits per pixel */
	{ 0,4 },	/* the bitplanes are packed in the same byte */
	{ (0x4000*8)+0, (0x4000*8)+1, (0x4000*8)+2, (0x4000*8)+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8 /* every char takes 8 consecutive bytes */
};

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ REGION_GFX1, 0, &char_layout,   16*4+8*8, 16 }, /* characters */
	{ REGION_GFX2, 0, &sprite_layout, 16*4, 	 8 }, /* sprites */
	{ REGION_GFX3, 0, &tile_layout,   0,		16 }, /* tiles */
	{ -1 } /* end of array */
};

static void shootout_snd_irq(int linestate)
{
	cpu_set_irq_line(1,0,linestate);
}

static void shootout_snd2_irq(int linestate)
{
	cpu_set_irq_line(0,0,linestate);
}

static struct YM2203interface ym2203_interface =
{
	1,	/* 1 chip */
	1500000,	/* 1.5 MHz */
	{ YM2203_VOL(50,50) },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ shootout_snd_irq },
};

static struct YM2203interface ym2203_interface2 =
{
	1,	/* 1 chip */
	1500000,	/* 1.5 MHz */
	{ YM2203_VOL(50,50) },
	{ 0 },
	{ 0 },
	{ shootout_bankswitch_w },
	{ 0 }, /* Todo:  Port B write is flipscreen */
	{ shootout_snd2_irq },
};

static INTERRUPT_GEN( shootout_interrupt )
{
	static int coin = 0;

	if ( readinputport( 2 ) & 0xc0 ) {
		if ( coin == 0 ) {
			coin = 1;
			nmi_line_pulse();
		}
	} else
		coin = 0;
}

static MACHINE_DRIVER_START( shootout )

	/* basic machine hardware */
	MDRV_CPU_ADD(M6502, 2000000)	/* 2 MHz? */
	MDRV_CPU_MEMORY(readmem,writemem)
	MDRV_CPU_VBLANK_INT(shootout_interrupt,1) /* nmi's are triggered at coin up */

	MDRV_CPU_ADD(M6502, 1500000)
	MDRV_CPU_FLAGS(CPU_AUDIO_CPU)
	MDRV_CPU_MEMORY(sound_readmem,sound_writemem)

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_REAL_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(32*8, 32*8)
	MDRV_VISIBLE_AREA(0*8, 32*8-1, 1*8, 31*8-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(256)

	MDRV_PALETTE_INIT(shootout)
	MDRV_VIDEO_START(shootout)
	MDRV_VIDEO_UPDATE(shootout)

	/* sound hardware */
	MDRV_SOUND_ADD(YM2203, ym2203_interface)
MACHINE_DRIVER_END


static MACHINE_DRIVER_START( shootouj )

	/* basic machine hardware */
	MDRV_CPU_ADD(M6502, 2000000)	/* 2 MHz? */
	MDRV_CPU_MEMORY(readmem_alt,writemem_alt)
	MDRV_CPU_VBLANK_INT(shootout_interrupt,1) /* nmi's are triggered at coin up */

	MDRV_FRAMES_PER_SECOND(60)
	MDRV_VBLANK_DURATION(DEFAULT_REAL_60HZ_VBLANK_DURATION)

	/* video hardware */
	MDRV_VIDEO_ATTRIBUTES(VIDEO_TYPE_RASTER)
	MDRV_SCREEN_SIZE(32*8, 32*8)
	MDRV_VISIBLE_AREA(0*8, 32*8-1, 1*8, 31*8-1)
	MDRV_GFXDECODE(gfxdecodeinfo)
	MDRV_PALETTE_LENGTH(256)

	MDRV_PALETTE_INIT(shootout)
	MDRV_VIDEO_START(shootout)
	MDRV_VIDEO_UPDATE(shootouj)

	/* sound hardware */
	MDRV_SOUND_ADD(YM2203, ym2203_interface2)
MACHINE_DRIVER_END


ROM_START( shootout )
	ROM_REGION( 2*0x20000, REGION_CPU1, 0 )	/* 128k for code + 128k for decrypted opcodes */
	ROM_LOAD( "cu00.b1",        0x08000, 0x8000, CRC(090edeb6) SHA1(ab849d123dacf3947b1ebd29b70a20e066911a60) ) /* opcodes encrypted */
	/* banked at 0x4000-0x8000 */
	ROM_LOAD( "cu02.c3",        0x10000, 0x8000, CRC(2a913730) SHA1(584488278d58c4d34a2eebeaf39518f87cf5eecd) ) /* opcodes encrypted */
	ROM_LOAD( "cu01.c1",        0x18000, 0x4000, CRC(8843c3ae) SHA1(c58ed4acac566f890cadf62bcbcced07a59243fc) ) /* opcodes encrypted */

	ROM_REGION( 0x10000, REGION_CPU2, 0 ) /* 64k for code */
	ROM_LOAD( "cu09.j1",        0x0c000, 0x4000, CRC(c4cbd558) SHA1(0e940ae99febc1161e5f35550aa75afca88cb5e9) ) /* Sound CPU */

	ROM_REGION( 0x04000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "cu11.h19",       0x00000, 0x4000, CRC(eff00460) SHA1(15daaa3d3125a981a26f31d43283faa5be26e96b) ) /* foreground characters */

	ROM_REGION( 0x30000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "cu04.c7",        0x00000, 0x8000, CRC(ceea6b20) SHA1(9fe363668db2e2759b3c531b4d7f23c65f2e8035) )   /* sprites */
	ROM_LOAD( "cu03.c5",        0x08000, 0x8000, CRC(b786bb3e) SHA1(5a209f01914ca4b206138d738a34640e0bcb3185) )
	ROM_LOAD( "cu06.c10",       0x10000, 0x8000, CRC(2ec1d17f) SHA1(74f0579a5ab3daf5d1290d3c15459f0f9b67bf79) )
	ROM_LOAD( "cu05.c9",        0x18000, 0x8000, CRC(dd038b85) SHA1(b1c3c1ab17c36a1c77726b5e485fc01581a4d97d) )
	ROM_LOAD( "cu08.c13",       0x20000, 0x8000, CRC(91290933) SHA1(60487f4eaf2e6c50b24c0f8fbd7abf92c04a342a) )
	ROM_LOAD( "cu07.c12",       0x28000, 0x8000, CRC(19b6b94f) SHA1(292264811206916af41d133f81dfd93c44f59a96) )

	ROM_REGION( 0x08000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "cu10.h17",       0x00000, 0x2000, CRC(3854c877) SHA1(2c8fe4591553ce798c907849e3dbd410e4fe424c) ) /* background tiles */
	ROM_CONTINUE(				0x04000, 0x2000 )
	ROM_CONTINUE(				0x02000, 0x2000 )
	ROM_CONTINUE(				0x06000, 0x2000 )

	ROM_REGION( 0x0200, REGION_PROMS, 0 )
	ROM_LOAD( "gb08.k10",       0x0000, 0x0100, CRC(509c65b6) SHA1(4cec37065a799ced4e7b6552f267aacc7f54ffe3) )
	ROM_LOAD( "gb09.k6",        0x0100, 0x0100, CRC(aa090565) SHA1(e289e77ec3402e86d93b873c0fa064f3e6277a62) )	/* priority encoder? (not used) */
ROM_END

ROM_START( shootouj )
	ROM_REGION( 0x20000, REGION_CPU1, 0 )	/* 128k for code  */
	ROM_LOAD( "cg02.bin",    0x08000, 0x8000, CRC(8fc5d632) SHA1(809ac4eba09972229fe741c96fa8036d7139b6a8) )
	ROM_LOAD( "cg00.bin",    0x10000, 0x8000, CRC(ef6ced1e) SHA1(feea508c7a60fc6cde1efee52cba628accd26028) )
	ROM_LOAD( "cg01.bin",    0x18000, 0x4000, CRC(74cf11ca) SHA1(59edbc4633cd560e7b928b33e4c42d0125332a1b) )

	ROM_REGION( 0x04000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "cu11.h19",       0x00000, 0x4000, CRC(eff00460) SHA1(15daaa3d3125a981a26f31d43283faa5be26e96b) ) /* foreground characters */

	ROM_REGION( 0x30000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "cg03.bin",    0x00000, 0x8000, CRC(5252ec19) SHA1(c6848a815badd8845f91e898b0a52b7f12ed8a39) )  /* sprites */
	ROM_LOAD( "cg04.bin",    0x10000, 0x8000, CRC(db06cfe9) SHA1(e13c16232f54fe8467c21e0218c87606a19dd25c) )
	ROM_LOAD( "cg05.bin",    0x20000, 0x8000, CRC(d634d6b8) SHA1(e2ddd12b1b3fb0063104d414f0574b94dbfa0403) )

	ROM_REGION( 0x08000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "cu10.h17",       0x00000, 0x2000, CRC(3854c877) SHA1(2c8fe4591553ce798c907849e3dbd410e4fe424c) ) /* background tiles */
	ROM_CONTINUE(				0x04000, 0x2000 )
	ROM_CONTINUE(				0x02000, 0x2000 )
	ROM_CONTINUE(				0x06000, 0x2000 )

	ROM_REGION( 0x0200, REGION_PROMS, 0 )
	ROM_LOAD( "gb08.k10",       0x0000, 0x0100, CRC(509c65b6) SHA1(4cec37065a799ced4e7b6552f267aacc7f54ffe3) )
	ROM_LOAD( "gb09.k6",        0x0100, 0x0100, CRC(aa090565) SHA1(e289e77ec3402e86d93b873c0fa064f3e6277a62) )	/* priority encoder? (not used) */
ROM_END

ROM_START( shootoub )
	ROM_REGION( 2*0x20000, REGION_CPU1, 0 )	/* 128k for code + 128k for decrypted opcodes */
	ROM_LOAD( "shootout.006", 0x08000, 0x8000, CRC(2c054888) SHA1(cb0de2f7d743506789626304e6bcbbc292fbe8bc) )
	ROM_LOAD( "shootout.008", 0x10000, 0x8000, CRC(9651b656) SHA1(e90eddf2833ef36fa73b7b8d81d28443d2f60220) )
	ROM_LOAD( "cg01.bin",     0x18000, 0x4000, CRC(74cf11ca) SHA1(59edbc4633cd560e7b928b33e4c42d0125332a1b) )

	ROM_REGION( 0x04000, REGION_GFX1, ROMREGION_DISPOSE )
	ROM_LOAD( "cu11.h19",       0x00000, 0x4000, CRC(eff00460) SHA1(15daaa3d3125a981a26f31d43283faa5be26e96b) ) /* foreground characters */

	ROM_REGION( 0x30000, REGION_GFX2, ROMREGION_DISPOSE )
	ROM_LOAD( "shootout.005",   0x00000, 0x8000, CRC(e6357ba3) SHA1(1ceb46450a0c4f6f7f7109601ad6617f08364df5) )   /* sprites */
	ROM_LOAD( "shootout.004",   0x10000, 0x8000, CRC(7f422c93) SHA1(97d9a17956e838801c416461b020876c780bf260) )
	ROM_LOAD( "shootout.003",   0x20000, 0x8000, CRC(eea94535) SHA1(65819b7925ecd9ae6e62decb3b0164f627b73fe5) )

	ROM_REGION( 0x08000, REGION_GFX3, ROMREGION_DISPOSE )
	ROM_LOAD( "cu10.h17",       0x00000, 0x2000, CRC(3854c877) SHA1(2c8fe4591553ce798c907849e3dbd410e4fe424c) ) /* background tiles */
	ROM_CONTINUE(				0x04000, 0x2000 )
	ROM_CONTINUE(				0x02000, 0x2000 )
	ROM_CONTINUE(				0x06000, 0x2000 )

	ROM_REGION( 0x0220, REGION_PROMS, 0 )
	ROM_LOAD( "gb08.k10",       0x0000, 0x0100, CRC(509c65b6) SHA1(4cec37065a799ced4e7b6552f267aacc7f54ffe3) )
	ROM_LOAD( "gb09.k6",        0x0100, 0x0100, CRC(aa090565) SHA1(e289e77ec3402e86d93b873c0fa064f3e6277a62) )	/* priority encoder? (not used) */
	ROM_LOAD( "shootclr.003",   0x0200, 0x0020, CRC(6b0c2942) SHA1(7d25acc753923b265792fc78f8fc70175c0e0ec2) )	/* opcode decrypt table (bootleg only) */
ROM_END


static DRIVER_INIT( shootout )
{
	unsigned char *rom = memory_region(REGION_CPU1);
	int diff = memory_region_length(REGION_CPU1) / 2;
	int A;

	memory_set_opcode_base(0,rom+diff);

	for (A = 0;A < diff;A++)
		rom[A+diff] = (rom[A] & 0x9f) | ((rom[A] & 0x40) >> 1) | ((rom[A] & 0x20) << 1);
}


GAME( 1985, shootout, 0,		 shootout, shootout, shootout, ROT0, "Data East USA", "Shoot Out (US)")
GAMEX( 1985, shootouj, shootout, shootouj, shootout, 0, 	   ROT0, "Data East USA", "Shoot Out (Japan)", GAME_NO_COCKTAIL )
GAMEX( 1985, shootoub, shootout, shootouj, shootout, shootout, ROT0, "bootleg", "Shoot Out (Korean Bootleg)", GAME_NO_COCKTAIL )

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
