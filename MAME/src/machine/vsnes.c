#pragma code_seg("C603")
#pragma data_seg("D603")
#pragma bss_seg("B603")
#pragma const_seg("K603")
#pragma comment(linker, "/merge:D603=603")
#pragma comment(linker, "/merge:C603=603")
#pragma comment(linker, "/merge:B603=603")
#pragma comment(linker, "/merge:K603=603")
/***************************************************************************

Nintendo VS UniSystem and DualSystem - (c) 1984 Nintendo of America

	Portions of this code are heavily based on
	Brad Oliver's MESS implementation of the NES.

***************************************************************************/

#include "driver.h"
#include "vidhrdw/ppu2c03b.h"

/* Globals */
int vsnes_gun_controller;

/* Locals */
static int input_latch[4];
static UINT8 *remapped_colortable;

static int sound_fix=0;
/*************************************
 *
 *	Color Mapping
 *
 *************************************/

/* RP2C04-001 */
static UINT8 rp2c04001_colortable[] =
{
	0x35, 0xff, 0x16, 0x22, 0x1c, 0xff, 0xff, 0x15, /* 0x00 - 0x07 */
	0xff, 0x00, 0x27, 0x05, 0x04, 0x27, 0x08, 0x30, /* 0x08 - 0x0f */
	0x21, 0xff, 0xff, 0x29, 0x3c, 0xff, 0x36, 0x12, /* 0x10 - 0x17 */
	0xff, 0x2b, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, /* 0x18 - 0x1f */
	0xff, 0x31, 0xff, 0x2a, 0x2c, 0x0c, 0xff, 0xff, /* 0x20 - 0x27 */
	0xff, 0x07, 0x34, 0x06, 0x13, 0xff, 0x26, 0x0f, /* 0x28 - 0x2f */
	0xff, 0x19, 0x10, 0x0a, 0xff, 0xff, 0xff, 0x17, /* 0x30 - 0x37 */
	0xff, 0x11, 0x09, 0xff, 0xff, 0x25, 0x18, 0xff  /* 0x38 - 0x3f */
};

/* RP2C04-002 */
static UINT8 rp2c04002_colortable[] =
{
	0xff, 0x27, 0x18, 0xff, 0x3a, 0x25, 0xff, 0x31, /* 0x00 - 0x07 */
	0x16, 0x13, 0x38, 0x34, 0x20, 0x23, 0xff, 0x0b, /* 0x08 - 0x0f */
	0xff, 0x21, 0x06, 0xff, 0x1b, 0x29, 0xff, 0x22, /* 0x10 - 0x17 */
	0xff, 0x24, 0xff, 0xff, 0xff, 0x08, 0xff, 0x03, /* 0x18 - 0x1f */
	0xff, 0x36, 0x26, 0x33, 0x11, 0xff, 0x10, 0x02, /* 0x20 - 0x27 */
	0x14, 0xff, 0x00, 0x09, 0x12, 0x0f, 0xff, 0x30, /* 0x28 - 0x2f */
	0xff, 0xff, 0x2a, 0x17, 0x0c, 0x01, 0x15, 0x19, /* 0x30 - 0x37 */
	0xff, 0x2c, 0x07, 0x37, 0xff, 0x05, 0xff, 0xff  /* 0x38 - 0x3f */
};

/* RP2C04-003 */
/* check 0x0f, 0x2e */
static UINT8 rp2c04003_colortable[] =
{
	0xff, 0xff, 0xff, 0x10, 0x1a, 0x30, 0x31, 0x09, /* 0x00 - 0x07 */
	0x01, 0x0f, 0x36, 0x08, 0x15, 0xff, 0xff, 0xf0, /* 0x08 - 0x0f */
	0x22, 0x1c, 0xff, 0x12, 0x19, 0x18, 0x17, 0xff, /* 0x10 - 0x17 */
	0x00, 0xff, 0xff, 0x02, 0x16, 0x06, 0xff, 0x35, /* 0x18 - 0x1f */
	0x23, 0xff, 0x8b, 0xf7, 0xff, 0x27, 0x26, 0x20, /* 0x20 - 0x27 */
	0x29, 0xff, 0x21, 0x24, 0x11, 0xff, 0xef, 0xff, /* 0x28 - 0x2f */
	0x2c, 0xff, 0xff, 0xff, 0x07, 0xf9, 0x28, 0xff, /* 0x30 - 0x37 */
	0x0a, 0xff, 0x32, 0x37, 0x13, 0xff, 0xff, 0x0c  /* 0x38 - 0x3f */
};

/* RP2C05-004 */
/* check 0x03 0x1d, 0x38, 0x3b*/
static UINT8 rp2c05004_colortable[] =
{
	0x18, 0xff, 0x1c, 0x89, 0xff, 0xff, 0x01, 0x17, /* 0x00 - 0x07 */
	0x10, 0x0f, 0x2a, 0xff, 0x36, 0x37, 0x1a, 0xff, /* 0x08 - 0x0f */
	0x25, 0xff, 0x12, 0xff, 0x0f, 0xff, 0xff, 0x26, /* 0x10 - 0x17 */
	0xff, 0xff, 0x22, 0xff, 0xff, 0x0f, 0x3a, 0x21, /* 0x18 - 0x1f */
	0x05, 0x0a, 0x07, 0xc2, 0x13, 0xff, 0x00, 0x15, /* 0x20 - 0x27 */
	0x0c, 0xff, 0x11, 0xff, 0xff, 0x38, 0xff, 0xff, /* 0x28 - 0x2f */
	0xff, 0xff, 0x08, 0x45, 0xff, 0xff, 0x30, 0x3c, /* 0x30 - 0x37 */
	0x0f, 0x27, 0xff, 0x60, 0x29, 0xff, 0x30, 0x09  /* 0x38 - 0x3f */
};



/* remap callback */
static int remap_colors( int num, int addr, int data )
{
	/* this is the protection. color codes are shuffled around */
	/* the ones with value 0xff are unknown */

	if ( addr >= 0x3f00 )
	{
		int newdata = remapped_colortable[ data & 0x3f ];

		if ( newdata != 0xff )
			data = newdata;

		#ifdef MAME_DEBUG
		else
			usrintf_showmessage( "Unmatched color %02x, at address %04x\n", data & 0x3f, addr );
		#endif
	}

	return data;
}

/*************************************
 *
 *	Input Ports
 *
 *************************************/
WRITE_HANDLER( vsnes_in0_w )
{
	/* Toggling bit 0 high then low resets both controllers */
	if ( data & 1 )
	{
		/* load up the latches */
		input_latch[0] = readinputport( 0 );
		input_latch[1] = readinputport( 1 );
	}
}

READ_HANDLER( gun_in0_r )
{
	int ret = ( input_latch[0] ) & 1;

	/* shift */
	input_latch[0] >>= 1;

	ret |= readinputport( 2 ); 				/* merge coins, etc */
	ret |= ( readinputport( 3 ) & 3 ) << 3; /* merge 2 dipswitches */

/* The gun games expect a 1 returned on every 5th read after sound_fix is reset*/
/* Info Supplied by Ben Parnell <xodnizel@home.com> of FCE Ultra fame */

	if (sound_fix == 4)
	{
		ret = 1;
	}

	sound_fix++;

	return ret;

}


READ_HANDLER( vsnes_in0_r )
{

	int ret = ( input_latch[0] ) & 1;

	/* shift */
	input_latch[0] >>= 1;

	ret |= readinputport( 2 ); 				/* merge coins, etc */
	ret |= ( readinputport( 3 ) & 3 ) << 3; /* merge 2 dipswitches */

	return ret;

}


READ_HANDLER( vsnes_in1_r )
{
	int ret = ( input_latch[1] ) & 1;

	ret |= readinputport( 3 ) & ~3;			/* merge the rest of the dipswitches */

	/* shift */
	input_latch[1] >>= 1;

	return ret;
}

WRITE_HANDLER( vsnes_in0_1_w )
{
	/* Toggling bit 0 high then low resets both controllers */
	if ( data & 1 )
	{
		/* load up the latches */
		input_latch[2] = readinputport( 4 );
		input_latch[3] = readinputport( 5 );
	}
}

READ_HANDLER( vsnes_in0_1_r )
{
	int ret = ( input_latch[2] ) & 1;

	/* shift */
	input_latch[2] >>= 1;

	ret |= readinputport( 6 ); 				/* merge coins, etc */
	ret |= ( readinputport( 7 ) & 3 ) << 3; /* merge 2 dipswitches */
	return ret;
}

READ_HANDLER( vsnes_in1_1_r )
{
	int ret = ( input_latch[3] ) & 1;

	ret |= readinputport( 7 ) & ~3;			/* merge the rest of the dipswitches */

	/* shift */
	input_latch[3] >>= 1;

	return ret;

}

/*************************************
 *
 *	Init machine
 *
 *************************************/

MACHINE_INIT( vsnes )
{
	input_latch[0] = input_latch[1] = 0;
	input_latch[2] = input_latch[3] = 0;

	/* reset the ppu */
	ppu2c03b_reset( 0, 1 );

	/* if we need to remap, install the callback */
	if ( remapped_colortable )
		ppu2c03b_set_vidaccess_callback( 0, remap_colors );
}

/*************************************
 *
 *	Init machine
 *
 *************************************/
MACHINE_INIT( vsdual )
{
	input_latch[0] = input_latch[1] = 0;
	input_latch[2] = input_latch[3] = 0;

	/* reset the ppu */
	ppu2c03b_reset( 0,1);
	ppu2c03b_reset( 1,1 );

	/* if we need to remap, install the callback */
	if ( remapped_colortable )
	{
		ppu2c03b_set_vidaccess_callback( 0, remap_colors );
		ppu2c03b_set_vidaccess_callback( 1, remap_colors );
	}
}

/*************************************
 *
 *	Common init for all games
 *
 *************************************/
DRIVER_INIT( vsnes )
{
	/* set the controller to default */
	vsnes_gun_controller = 0;

	/* no color remapping */
	remapped_colortable = 0;
}

/**********************************************************************************
 *
 *	Game and Board-specific initialization
 *
 **********************************************************************************/

static WRITE_HANDLER( vsnormal_vrom_banking )
{
	/* switch vrom */
	ppu2c03b_set_videorom_bank( 0, 0, 8, ( data & 4 ) ? 1 : 0, 512 );

	/* bit 1 ( data & 2 ) enables writes to extra ram, we ignore it */

	/* move along */
	vsnes_in0_w( offset, data );
}

/* Most games switch VROM Banks in controller 0 write */
/* they dont do any other trickery */
DRIVER_INIT( vsnormal )
{
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, vsnormal_vrom_banking );
}

static WRITE_HANDLER( ppuRC2C05_protection )
{
	/* This PPU has registers mapped at $2000 and $2001 inverted */
	/* and no remapped color */

	if ( offset == 0 )
	{
		ppu2c03b_0_w( 1, data );
		return;
	}

	ppu2c03b_0_w( 0, data );
}

/**********************************************************************************/

/* Super Mario Bros. Extra ram at $6000 (NV?) and remapped colors */

DRIVER_INIT( suprmrio )
{
	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	/* extra ram at $6000 is enabled with bit 1 of $4016 */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c05004_colortable;
}

/**********************************************************************************/

/* Gun Games - VROM Banking in controller 0 write */

static WRITE_HANDLER( duckhunt_vrom_banking )
{
	static int zapstore;

	/* switch vrom */
	ppu2c03b_set_videorom_bank( 0, 0, 8, ( data & 4 ) ? 1 : 0, 512 );

	/* here we do things a little different */
	if ( data & 1 )
	{

		/* load up the latches */
		input_latch[0] = readinputport( 0 );

		/* do the gun thing */
		if ( vsnes_gun_controller )
		{
			int x = readinputport( 4 );
			int y = readinputport( 5 );
			UINT32 pix, color_base;
			pen_t *pens = Machine->pens;

			/* get the pixel at the gun position */
			pix = ppu2c03b_get_pixel( 0, x, y );

			/* get the color base from the ppu */
			color_base = ppu2c03b_get_colorbase( 0 );

			/* look at the screen and see if the cursor is over a bright pixel */
			if ( ( pix == pens[color_base+0x20] ) || ( pix == pens[color_base+0x30] ) ||
				 ( pix == pens[color_base+0x33] ) || ( pix == pens[color_base+0x34] ) )
			{
				input_latch[0] |= 0x40;
			}
		}

		input_latch[1] = readinputport( 1 );
	}

    if ( ( zapstore & 1 ) && ( !( data & 1 ) ) )
	/* reset sound_fix to keep sound from hanging */
    {
		sound_fix = 0;
	}

    zapstore = data;

}

DRIVER_INIT( duckhunt )
{
	install_mem_read_handler ( 0, 0x4016, 0x4016, gun_in0_r);
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, duckhunt_vrom_banking );

	/* common init */
	init_vsnes();

	/* enable gun controller */
	vsnes_gun_controller = 1;
}

/**********************************************************************************/

/* The Goonies, VS Gradius: ROMs bankings at $8000-$ffff */

static WRITE_HANDLER( goonies_rom_banking )
{
	int reg = ( offset >> 12 ) & 0x07;
	int bankoffset = ( data & 7 ) * 0x2000 + 0x10000;

	switch( reg )
	{
		case 0: /* code bank 0 */
			memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[bankoffset], 0x2000 );
		break;

		case 2: /* code bank 1 */
			memcpy( &memory_region( REGION_CPU1 )[0x0a000], &memory_region( REGION_CPU1 )[bankoffset], 0x2000 );
		break;

		case 4: /* code bank 2 */
			memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[bankoffset], 0x2000 );
		break;

		case 6: /* vrom bank 0 */
			ppu2c03b_set_videorom_bank( 0, 0, 4, data, 256 );
		break;

		case 7: /* vrom bank 1 */
			ppu2c03b_set_videorom_bank( 0, 4, 4, data, 256 );
		break;
	}
}

DRIVER_INIT( goonies )
{
	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x18000], 0x8000 );

	/* banking is done with writes to the $8000-$ffff area */
	install_mem_write_handler( 0, 0x8000, 0xffff, goonies_rom_banking );

	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	remapped_colortable = rp2c04003_colortable;
}

DRIVER_INIT( vsgradus )
{
	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x18000], 0x8000 );

	/* banking is done with writes to the $8000-$ffff area */
	install_mem_write_handler( 0, 0x8000, 0xffff, goonies_rom_banking );

	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	remapped_colortable = rp2c04001_colortable;
}

DRIVER_INIT( vspinbal )
{
	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	/* now override the vidaccess callback */
	remapped_colortable = rp2c04001_colortable;

}

DRIVER_INIT( hogalley )
{

	install_mem_read_handler ( 0, 0x4016, 0x4016, gun_in0_r);
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, duckhunt_vrom_banking );

	/* common init */
	init_vsnes();

	/* enable gun controller */
	vsnes_gun_controller = 1;

	/* now override the vidaccess callback */
	remapped_colortable = rp2c04001_colortable;
}

/***********************************************************************/

/* Vs. Gumshoe */

static READ_HANDLER( vsgshoe_security_r )
{
	/* low part must be 0x1c */
	return ppu2c03b_0_r( 2 ) | 0x1c;
}

DRIVER_INIT( vsgshoe )
{

	//Game
	memcpy (&memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x10000], 0x2000);

	//Title Screen
	//memcpy (&memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x12000], 0x2000);

	memcpy (&memory_region( REGION_CPU1 )[0x0a000], &memory_region( REGION_CPU1 )[0x14000], 0x6000);

	/* Protection */
	install_mem_read_handler( 0,0x2002, 0x2002, vsgshoe_security_r );
	install_mem_write_handler( 0, 0x2000, 0x2001, ppuRC2C05_protection );

	install_mem_read_handler ( 0, 0x4016, 0x4016, gun_in0_r);
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, duckhunt_vrom_banking );

	/* common init */
	init_vsnes();

	vsnes_gun_controller = 1;

}

/**********************************************************************************/

/* Dr Mario: ROMs bankings at $8000-$ffff */

static int drmario_shiftreg;
static int drmario_shiftcount;

static WRITE_HANDLER( drmario_rom_banking )
{
	/* basically, a MMC1 mapper from the nes */
	static int size16k, switchlow, vrom4k;

	int reg = ( offset >> 13 );

	/* reset mapper */
	if ( data & 0x80 )
	{
		drmario_shiftreg = drmario_shiftcount = 0;

		size16k = 1;

		switchlow = 1;
		vrom4k = 0;

		return;
	}

	/* see if we need to clock in data */
	if ( drmario_shiftcount < 5 )
	{
		drmario_shiftreg >>= 1;
		drmario_shiftreg |= ( data & 1 ) << 4;
		drmario_shiftcount++;
	}

	/* are we done shifting? */
	if ( drmario_shiftcount == 5 )
	{
		/* reset count */
		drmario_shiftcount = 0;

		/* apply data to registers */
		switch( reg )
		{
			case 0:		/* mirroring and options */
				{
					int mirroring;

					vrom4k = drmario_shiftreg & 0x10;
					size16k = drmario_shiftreg & 0x08;
					switchlow = drmario_shiftreg & 0x04;

					switch( drmario_shiftreg & 3 )
					{
						case 0:
							mirroring = PPU_MIRROR_LOW;
						break;

						case 1:
							mirroring = PPU_MIRROR_HIGH;
						break;

						case 2:
							mirroring = PPU_MIRROR_VERT;
						break;

						default:
						case 3:
							mirroring = PPU_MIRROR_HORZ;
						break;
					}

					/* apply mirroring */
					ppu2c03b_set_mirroring( 0, mirroring );
				}
			break;

			case 1:	/* video rom banking - bank 0 - 4k or 8k */
				ppu2c03b_set_videorom_bank( 0, 0, ( vrom4k ) ? 4 : 8, drmario_shiftreg, ( vrom4k ) ? 256 : 512 );
			break;

			case 2: /* video rom banking - bank 1 - 4k only */
				if ( vrom4k )
					ppu2c03b_set_videorom_bank( 0, 4, 4, drmario_shiftreg, 256 );
			break;

			case 3:	/* program banking */
				{
					int bank = ( drmario_shiftreg & 0x03 ) * 0x4000;

					if ( !size16k )
					{
						/* switch 32k */
						memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x010000+bank], 0x8000 );
					}
					else
					{
						/* switch 16k */
						if ( switchlow )
						{
							/* low */
							memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x010000+bank], 0x4000 );
						}
						else
						{
							/* high */
							memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[0x010000+bank], 0x4000 );
						}
					}
				}
			break;
		}

		drmario_shiftreg = 0;
	}
}

DRIVER_INIT( drmario )
{
	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x10000], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[0x1c000], 0x4000 );

	/* MMC1 mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, drmario_rom_banking );

	drmario_shiftreg = 0;
	drmario_shiftcount = 0;

	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	remapped_colortable = rp2c04003_colortable;
}

/***********************************************************************/

/* Vs. Freedom Force */

DRIVER_INIT( vsfdf )
{
	//It doesn't work due to the jump at location 0xe63e

	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x10000], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[0x2c000], 0x4000 );

	install_mem_read_handler ( 0, 0x4016, 0x4016, gun_in0_r );
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, duckhunt_vrom_banking );

	/* MMC1 mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, drmario_rom_banking );

	drmario_shiftreg = 0;
	drmario_shiftcount = 0;

	init_vsnes();

	vsnes_gun_controller = 1;

	remapped_colortable = rp2c04001_colortable;

}

/**********************************************************************************/

/* Excite Bike */

DRIVER_INIT( excitebk )
{
	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c04003_colortable;
}

DRIVER_INIT( excitbkj )
{
	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c05004_colortable;
}

/**********************************************************************************/

/* Mach Rider */

DRIVER_INIT( machridr )
{

	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c04002_colortable;
}

/**********************************************************************************/

/* VS Slalom */

DRIVER_INIT( vsslalom )
{
	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c04002_colortable;
}

/**********************************************************************************/

/* Castelvania: ROMs bankings at $8000-$ffff */

static WRITE_HANDLER( castlevania_rom_banking )
{
	int rombank = 0x10000 + ( data & 7 ) * 0x4000;

	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[rombank], 0x4000 );
}

DRIVER_INIT( cstlevna )
{
	/* when starting the game, the 1st 16k and the last 16k are loaded into the 2 banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x28000], 0x8000 );

   	/* banking is done with writes to the $8000-$ffff area */
	install_mem_write_handler( 0, 0x8000, 0xffff, castlevania_rom_banking );

	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c04002_colortable;
}

/**********************************************************************************/

/* VS Top Gun: ROMs bankings at $8000-$ffff, plus some protection */

static READ_HANDLER( topgun_security_r )
{
	/* low part must be 0x1b */
	return ppu2c03b_0_r( 2 ) | 0x1b;
}

DRIVER_INIT( topgun )
{
	/* when starting the game, the 1st 16k and the last 16k are loaded into the 2 banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x28000], 0x8000 );

   	/* banking is done with writes to the $8000-$ffff area */
	install_mem_write_handler( 0, 0x8000, 0xffff, castlevania_rom_banking );

	/* tap on the PPU, due to some tricky protection */
	install_mem_read_handler( 0, 0x2002, 0x2002, topgun_security_r );
	install_mem_write_handler( 0, 0x2000, 0x2001, ppuRC2C05_protection );

	/* common init */
	init_vsnes();
}

/**********************************************************************************/

/* RBI Baseball: ROMs bankings at $8000-$ffff */

static int rbibb_scanline_counter;
static int rbibb_scanline_latch;
static int mmc3_bank;
static int supxev_protection;

static void rbibb_scanline_cb( int num, int scanline, int vblank, int blanked )
{
	if ( !vblank && !blanked )
	{
		if ( --rbibb_scanline_counter <= 0 )
		{
			rbibb_scanline_counter = rbibb_scanline_latch;
			cpu_set_irq_line( 0, 0, PULSE_LINE );
		}
	}
}

static READ_HANDLER( rbi_hack_r)
{
/* Supplied by Ben Parnell <xodnizel@home.com> of FCE Ultra fame */

	static int VSindex;

	if (offset == 0)
	{
		VSindex=0;
		return 0xFF;

	}
	else
	{
		switch(VSindex++)
		{
   			case 9:
    			return 0x6F;
			break;

			case 14:
				return 0x94;
			break;

   			default:
    			return 0xB4;
			break;
		}
	}
}

static WRITE_HANDLER( rbibb_rom_switch_w )
{
	/* basically, a MMC3 mapper from the nes */
	static int last_bank = 0xff;
	static int rbibb_command;

	switch( offset & 0x7001 )
	{
		case 0x0000:
			rbibb_command = data;
			if ( last_bank != ( data & 0xc0 ) )
			{
				/* reset the banks */
				memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x2000 );
				memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x2000 );

				last_bank = data & 0xc0;
			}
		break;

		case 0x0001:
			{
				UINT8 cmd = rbibb_command & 0x07;
				int page = ( rbibb_command & 0x80 ) >> 5;

				switch( cmd )
				{
					case 0:	/* char banking */
					case 1: /* char banking */
						data &= 0xfe;
						page ^= ( cmd << 1 );
						ppu2c03b_set_videorom_bank( 0, page, 2, data, 64 );

					break;

					case 2: /* char banking */
					case 3: /* char banking */
					case 4: /* char banking */
					case 5: /* char banking */
						page ^= cmd + 2;
						ppu2c03b_set_videorom_bank( 0, page, 1, data, 64 );

					break;

					case 6: /* program banking */
						if ( rbibb_command & 0x40 )
						{
							/* high bank */
							int bank = ( data & 0x07 ) * 0x2000 + 0x10000;

							memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x2000 );
							memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[bank], 0x2000 );

						}
						else
						{
							/* low bank */
							int bank = ( data & 0x07 ) * 0x2000 + 0x10000;

							memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[bank], 0x2000 );
							memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x2000 );

						}
					break;

					case 7: /* program banking */
						{
							int bank = ( data & 0x07 ) * 0x2000 + 0x10000;

							memcpy( &memory_region( REGION_CPU1 )[0x0a000], &memory_region( REGION_CPU1 )[bank], 0x2000 );

						}
					break;
				}
			}
		break;

		case 0x2000: /* mirroring */
			if ( data & 0x40 )
				ppu2c03b_set_mirroring( 0, PPU_MIRROR_HIGH );
			else
				ppu2c03b_set_mirroring( 0, ( data & 1 ) ? PPU_MIRROR_HORZ : PPU_MIRROR_VERT );

		break;

		case 0x2001: /* enable ram at $6000 */
			/* ignored - we always enable it */
		break;

		case 0x4000: /* scanline counter */
			rbibb_scanline_counter = data;
		break;

		case 0x4001: /* scanline latch */
			rbibb_scanline_latch = data;
		break;

		case 0x6000: /* disable irqs */
			ppu2c03b_set_scanline_callback( 0, 0 );
		break;

		case 0x6001: /* enable irqs */
			ppu2c03b_set_scanline_callback( 0, rbibb_scanline_cb );
		break;
	}
}

DRIVER_INIT( rbibb )
{
	mmc3_bank = 0x1c000;

	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );

	/* RBI Base ball hack */
	install_mem_read_handler(0,0x5e00, 0x5e01, rbi_hack_r) ;

	/* MMC3 mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, rbibb_rom_switch_w );

	/* extra ram at $6000-$7fff */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	/* common init */
	init_vsnes();

	remapped_colortable = rp2c04003_colortable;
}

/**********************************************************************************/
/* Vs. Super Xevious */

static READ_HANDLER( xevious_hack_r )
{
	return 0x05;
}

static READ_HANDLER( xevious_hack_r1 )
{
	if( supxev_protection )
		return 0;
	else
		return 0x01;
}

static READ_HANDLER( xevious_hack_r2 )
{
	if( supxev_protection )
		return 0xd1;
	else
		return 0x89;
}

static READ_HANDLER( xevious_hack_r3 )
{
	if( supxev_protection )
	{
		supxev_protection = 0;
		return 0x3e;
	}
	else
	{
		supxev_protection = 1;
		return 0x37;
	}
}

DRIVER_INIT( xevious )
{
	mmc3_bank = 0x2c000;

	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );

	/* MMC3 mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, rbibb_rom_switch_w );

	install_mem_read_handler( 0, 0x54ff, 0x54ff, xevious_hack_r );
	install_mem_read_handler( 0, 0x5678, 0x5678, xevious_hack_r1 );
	install_mem_read_handler( 0, 0x578f, 0x578f, xevious_hack_r2 );
	install_mem_read_handler( 0, 0x5567, 0x5567, xevious_hack_r3 );

	supxev_protection = 0;

	/* extra ram at $6000-$7fff */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	/* common init */
	init_vsnes();

	remapped_colortable = rp2c04001_colortable;
}

static READ_HANDLER( tko_security_r )
{
	static int security_counter;
	static UINT8 security_data[] = {
		0xff, 0xbf, 0xb7, 0x97, 0x97, 0x17, 0x57, 0x4f,
		0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90, 0x94, 0x14,
		0x56, 0x4e, 0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90,
		0xd4, 0x5c, 0x3e, 0x26, 0x87, 0x83, 0x13, 0x00
	};

	if ( offset == 0 )
	{
		security_counter = 0;
		return 0;
	}

	return security_data[(security_counter++)];

}

DRIVER_INIT( tkoboxng )
{
	mmc3_bank  = 0x1c000;

	/* We do manual banking, in case the code falls through */
	/* Copy the initial banks */
	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[mmc3_bank], 0x4000 );

	/* MMC3 mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, rbibb_rom_switch_w );

	/* extra ram at $6000-$7fff */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	/* security device at $5e00-$5e01 */
	install_mem_read_handler( 0, 0x5e00, 0x5e01, tko_security_r );

	/* common init */
	init_vsnes();

	/* now override the vidaccess callback */
	/* we need to remap color tables */
	/* this *is* the VS games protection, I guess */
	remapped_colortable = rp2c04003_colortable;
}

/**********************************************************************************/

/* VS SkyKid: ROMs bankings at $8000-$ffff */

static WRITE_HANDLER( vsskykid_rom_switch_w )
{

	/* basically, a MMC3 mapper from the nes with out the program rom banking */

	static int mapper_command;

	switch( offset & 0x7001 )
	{
		case 0x0000:

			mapper_command = data;

			break;

		case 0x0001:
			{

				UINT8 cmd = mapper_command & 0x07;
				int page = ( mapper_command & 0x80 ) >> 5;

				switch( cmd )
				{
					case 0:	/* char banking */
					case 1:	/* char banking */

						data &= 0xfe;
						page ^= ( cmd << 1 );
						ppu2c03b_set_videorom_bank( 0, page, 2, data, 64 );

					break;

					case 2: /* char banking */
					case 3: /* char banking */
					case 4: /* char banking */
					case 5: /* char banking */

						page ^= cmd + 2;
						ppu2c03b_set_videorom_bank( 0, page, 1, data, 64 );

					break;

				}
			}
		break;

	}

}

DRIVER_INIT( vsskykid )
{
	/* ??? mapper at writes to $8000-$ffff */
	install_mem_write_handler( 0, 0x8000, 0xffff, vsskykid_rom_switch_w );

	/* common init */
	init_vsnes();

}

/**********************************************************************************/
/* Platoon rom banking */

static WRITE_HANDLER( mapper68_rom_banking ){

	switch (offset & 0x7000)
	{
		case 0x0000:
		ppu2c03b_set_videorom_bank(0,0,2,data,128);

		break;
		case 0x1000:
		ppu2c03b_set_videorom_bank(0,2,2,data,128);

		break;
		case 0x2000:
		ppu2c03b_set_videorom_bank(0,4,2,data,128);

		break;
		case 0x3000: /* ok? */
		ppu2c03b_set_videorom_bank(0,6,2,data,128);

		break;

		case 0x7000:
		memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x10000 +data*0x4000], 0x4000 );
		break;

	}

}

DRIVER_INIT( platoon )
{

	/* when starting a mapper 68 game  the first 16K ROM bank in the cart is loaded into $8000
	the LAST 16K ROM bank is loaded into $C000. The last 16K of ROM cannot be swapped. */

	memcpy( &memory_region( REGION_CPU1 )[0x08000], &memory_region( REGION_CPU1 )[0x10000], 0x4000 );
	memcpy( &memory_region( REGION_CPU1 )[0x0c000], &memory_region( REGION_CPU1 )[0x2c000], 0x4000 );

	install_mem_write_handler( 0, 0x8000, 0xffff, mapper68_rom_banking );

	init_vsnes();

 	remapped_colortable = rp2c04001_colortable;

}

/**********************************************************************************/
/* Vs. Raid on Bungeling Bay (Japan) */

static int ret;
static WRITE_HANDLER ( set_bnglngby_irq_w )
{
	ret = data;
	cpu_set_irq_line( 0, 0, ( data & 2 ) ? ASSERT_LINE : CLEAR_LINE );
	/* other values ??? */
	/* 0, 4, 84 */
}

static READ_HANDLER ( set_bnglngby_irq_r )
{
	return ret;
}

DRIVER_INIT( bnglngby )
{
	install_mem_read_handler( 0, 0x0231, 0x0231, set_bnglngby_irq_r );
	install_mem_write_handler( 0, 0x0231, 0x0231, set_bnglngby_irq_w );

	/* extra ram */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	ret = 0;

	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();

	remapped_colortable = rp2c04002_colortable;
}

/**********************************************************************************/
/* Vs. Ninja Jajamaru Kun */

static READ_HANDLER( jajamaru_security_r )
{
	/* low part must be 0x40 */
	return ppu2c03b_0_r( 2 ) | 0x40;
}

DRIVER_INIT( jajamaru )
{
	//It executes an illegal opcode: 0x04 at 0x9e67 and 0x9e1c
	//At 0x9e5d and 0x9e12 there is a conditional jump to it
	//Maybe it should be a DOP (double NOP)

	/* Protection */
	install_mem_read_handler( 0, 0x2002, 0x2002, jajamaru_security_r );
	install_mem_write_handler( 0, 0x2000, 0x2001, ppuRC2C05_protection );

	/* common init */
	init_vsnes();

	/* normal banking */
	init_vsnormal();
}

/***********************************************************************/

/* Vs. Mighty Bomb Jack */

static READ_HANDLER( mightybj_security_r )
{
	/* low part must be 0x3d */
	return ppu2c03b_0_r( 2 ) | 0x3d;
}

DRIVER_INIT( mightybj )
{
	/* Protection */
	install_mem_read_handler( 0, 0x2002, 0x2002, mightybj_security_r );
	install_mem_write_handler( 0, 0x2000, 0x2001, ppuRC2C05_protection );

	/* common init */
	init_vsnes();
}

/**********************************************************************************/
/* VS Tennis */

static WRITE_HANDLER( vstennis_vrom_banking )
{
	int other_cpu = cpu_getactivecpu() ^ 1;

	/* switch vrom */
	ppu2c03b_set_videorom_bank( cpu_getactivecpu(), 0, 8, ( data & 4 ) ? 1 : 0, 512 );

	/* bit 1 ( data & 2 ) triggers irq on the other cpu */
	cpu_set_irq_line( other_cpu, 0, ( data & 2 ) ? CLEAR_LINE : ASSERT_LINE );

	/* move along */
	if ( cpu_getactivecpu() == 0 )
		vsnes_in0_w( offset, data );
	else
		vsnes_in0_1_w( offset, data );
}

DRIVER_INIT( vstennis )
{
	/* vrom switching is enabled with bit 2 of $4016 */
	install_mem_write_handler( 0, 0x4016, 0x4016, vstennis_vrom_banking );
	install_mem_write_handler( 1, 0x4016, 0x4016, vstennis_vrom_banking );

	/* shared ram at $6000 */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_BANK2 );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_BANK2 );
	install_mem_read_handler( 1, 0x6000, 0x7fff, MRA_BANK2 );
	install_mem_write_handler( 1, 0x6000, 0x7fff, MWA_BANK2 );

}

/**********************************************************************/
/* Wrecking Crew Init*/

DRIVER_INIT( wrecking )
{
	/* only differance between this and vstennis is the colors */

	init_vstennis();
	remapped_colortable = rp2c04002_colortable;
}

/**********************************************************************/
/* VS Balloon Fight */

DRIVER_INIT( balonfgt )
{
	/* only differance between this and vstennis is the colors */

	init_vstennis();

	remapped_colortable = rp2c04003_colortable;
}


/**********************************************************************/
/* VS Baseball */

DRIVER_INIT( vsbball )
{
	/* only differance between this and vstennis is the colors */

	init_vstennis();

	remapped_colortable = rp2c04001_colortable;

}


/**********************************************************************/
/* Dual Ice Climber Jpn */

DRIVER_INIT( iceclmrj )
{
	/* only differance between this and vstennis is the colors */

	init_vstennis();

	remapped_colortable = rp2c05004_colortable;

}

/**********************************************************************/
/* Battle City */
DRIVER_INIT( btlecity )
{
	init_vsnes();
	init_vsnormal();
	remapped_colortable = rp2c04003_colortable;
}

/***********************************************************************/
/* Tetris */
DRIVER_INIT( vstetris )
{
	/* extra ram at $6000 is enabled with bit 1 of $4016 */
	install_mem_read_handler( 0, 0x6000, 0x7fff, MRA_RAM );
	install_mem_write_handler( 0, 0x6000, 0x7fff, MWA_RAM );

	init_vsnes();
	init_vsnormal();
	remapped_colortable = rp2c04003_colortable;
}

//remapped_colortable = rp2c04001_colortable;
//remapped_colortable = rp2c04002_colortable;
//remapped_colortable = rp2c04003_colortable;
//remapped_colortable = rp2c05004_colortable;
#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
