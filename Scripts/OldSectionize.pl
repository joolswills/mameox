#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;


$ROOT_DIR		= MAMEoXScriptConstants::ROOT_DIR;
$MAMEoX_DIR = MAMEoXScriptConstants::MAMEoX_DIR;
$MAME_DIR   = MAMEoXScriptConstants::MAME_DIR;
$MAME_SRC_DIR			= $MAME_DIR."/src";
$MAME_CPU_DIR			= $MAME_SRC_DIR."/cpu";
$MAME_DRIVER_DIR	= $MAME_SRC_DIR."/drivers";
$MAME_MACHINE_DIR = $MAME_SRC_DIR."/machine";
$MAME_SOUND_DIR		= $MAME_SRC_DIR."/sound";
$MAME_SNDHRDW_DIR = $MAME_SRC_DIR."/sndhrdw";
$MAME_VIDHRDW_DIR = $MAME_SRC_DIR."/vidhrdw";

use constant DATA_PREFIX  => 'D';
use constant CODE_PREFIX  => 'C';
use constant BSS_PREFIX   => 'B';
use constant CONST_PREFIX => 'K';



%DriverNameToSectionMap;
%CPUIDToSectionMap;

  # Base name for the generated .h and .cpp files
$GeneratedFileBaseName = "XBESectionUtil";


print "Generating stub files...\n";
if( lc(@ARGV[0]) eq "-createheader" ) {
	CreateHeaderFile();
} else {
	print "Note: Skipping creation of $GeneratedFileBaseName.h, pass -createheader to generate this file.\n";
}
CreateSwitchStubs();

CreateSourceFileStub();


# Auto generate an 8 character unique name for each file
local $autoNameNumber = 0;


  # Driver families --------------------------------------------
@SkipDrivers = ( "jrcrypt.c", "monaco.c", "pong.c" );

#@TwinCobraFamily = ( "wardner.c", "twincobr.c" );
#@CapcomFamily = ( "cps1.c", "cps2.c" );
#@NamcoFamily = ( "namcoic.c", "namcona1.c", "namconb1.c", "namcond1.c", "namcos1.c",
#                 "namcos2.c", "namcos21.c", "namcos22.c", "namcos86.c", "namcos11.c" );
#@SegaFamily = ( "multi32.c", "system1.c", "system16.c", "system18.c", "system24.c", "system32.c",
#				"aburner.c", "sharrier.c", "outrun.c", "segasyse.c", "segac2.c" );
#@MoonPatrolFamily = ( "mpatrol.c", "yard.c" );								# 10 Yard Fight07/07/2003
#@SNKFamily = ( "snk.c", "hal21.c", "marvins.c", "sgladiat.c" );				# ASO - Armored Scrum Object, Athena, Marvin's Maze, Gladiator 1984
#@AmidarFamily = ( "galaxian.c", "scramble.c", "scobra.c", 
#                  "amidar.c", "frogger.c", "pacman.c",
#				  "pengo.c", "cclimber.c", "dkong.c", "yamato.c",				  
#				  "phoenix.c", "epos.c" );									# Amidar, Frog (Galaxian Hardware), Pac-Man (Galaxian Hardware), eyes, Moon Shuttle (US?), Special Forces, Special Forces II, Yamato, Batman Part 2, Beastie Feastie
#@RampartFamily = ( "rampart.c", "arcadecl.c" );								# Arcade Classics
#@RastanFamily = ( "rastan.c", "asuka.c", "opwolf.c", "rainbow.c", 
#                  "topspeed.c" );			                                # Asuka & Asuka, Operation Wolf, Rainbow Islands, Top Speed, Full Throttle
#@EspialFamily = ( "espial.c", "marineb.c", "zodiack.c" );					# Battle Cruiser M-12, Zodiack
#@BZoneFamily = ( "bzone.c", "bwidow.c" );									# Gravitar
#@Nova2001Family = ( "nova2001.c", "pkunwar.c" );							# Penguin-Kun War
#@GottLiebFamily = ( "gottlieb.c", "exterm.c" );							    # exterminator
#@Megasys1Family = ( "megasys1.c", "cischeat.c" );							# F-1 Grand Prix Star II
#@LocoMotionFamily = ( "rallyx.c", "timeplt.c", "locomotn.c", 
#					  "tutankhm.c", "pooyan.c", "rocnrope.c", 
#					  "junofrst.c" );				                        # Loco-Motion, Tutankham, Pooyan, Roc'n Rope, Juno First
#@ExidyFamily = ( "exidy.c", "victory.c" );									# Victory
#@M72Family = ( "m72.c", "m90.c", "vigilant.c", "shisen.c" );			    # Bomber Man World (World), Vigilante, Sichuan II
#@LelandFamily = ( "leland.c", "ataxx.c" );									# Asylum (prototype)
#@TrackFldFamily = ( "trackfld.c", "hyperspt.c", "yiear.c", "sbasketb.c" );	# Hyper Sports, Hyper Olympics '84, Yie ar Kung Fu, Super Basketball
#@Taito_F3Family = ( "taito_f3.c", "superchs.c", "groundfx.c",
#					"gunbustr.c", "undrfire.c" );							# Super Chase, Ground Effects, Gunbuster, Under Fire
#@FromanceFamily = ( "fromance.c", "pipedrm.c" );							# Pipe Dream
#@MidwayMCRFamily = ( "mcr1.c", "mcr2.c", "mcr3.c", "mcr68.c", "williams.c",
#					 "midyunit.c", "midtunit.c", "midwunit.c", 
#					 "midvunit.c", "midxunit.c" );							# Arch rivals, Mortal Kombat II, WWF: Wrestlemania (rev 1.30 08/10/95), Mortal Kombat, Revolution X
#@KonamiGXFamily = ( "konamigx.c", "mystwarr.c" );							# Metamorphic Force
#@SegarFamily = ( "segar.c", "sega.c" );										# Star Trek
#@ZaxxonFamily = ( "zaxxon.c", "congo.c" );									# Congo Bongo
#@WizFamily = ( "wiz.c", "rollrace.c" );										# Fighting Roller
#@TetrisPlus2Family = ( "ms32.c", "tetrisp2.c" );							# Tetris Plus 2 (MegaSystem 32 Version)
#@WipingFamily = ( "wiping.c", "clshroad.c" );								# Fire Battle, Clash-Road
#@D8080bwFamily = ( "8080bw.c", "rotaryf.c", "circus.c" );					# Rotary Fighter, Clowns, Dog Patch
#@BuggyChallengeFamily = ( "buggychl.c", "40love.c" );						# Forty-Love
#@BurgerTimeFamily = ( "btime.c", "scregg.c" );								# Dommy, Scrambled Egg
#@PlaymarkFamily = ( "playmark.c", "sslam.c" );								# Super Slam
#@NYNYFamily = ( "nyny.c", "spiders.c" );									# Spiders
#@DoubleDragonFamily = ( "ddragon.c", "chinagat.c" );						# China Gate
#@ThunderJawsFamily = ( "thunderj.c", "eprom.c" );							# Escape from the Planet of the Robot Monsters

#local @DriverFamilies = ( \@CapcomFamily, \@NamcoFamily, \@SegaFamily, \@MidwayMCRFamily, \@TwinCobraFamily,
#													\@MoonPatrolFamily, \@SNKFamily, \@AmidarFamily, \@RampartFamily, \@RastanFamily,
#													\@EspialFamily, \@BZoneFamily, \@Nova2001Family, \@GottLiebFamily, 
#													\@Megasys1Family, \@LocoMotionFamily, \@ExidyFamily, \@M72Family, \@LelandFamily,
#													\@TrackFldFamily, \@Taito_F3Family, \@FromanceFamily, \@MidYUnitFamily, \@KonamiGXFamily,
#													\@SegarFamily, \@ZaxxonFamily, \@WizFamily, \@TetrisPlus2Family, \@WipingFamily,
#													\@D8080bwFamily, \@BuggyChallengeFamily, \@BurgerTimeFamily, \@PlaymarkFamily,
#													\@NYNYFamily, \@DoubleDragonFamily, \@ThunderJawsFamily );


  # Generated w/ DetermineFamilies.pl
local @Family_0 = ( "buggychl.c", "40love.c" );
local @Family_1 = ( "circus.c", "8080bw.c", "rotaryf.c" );
local @Family_2 = ( "system16.c", "aburner.c", "taito_b.c", "taito_f3.c", "groundfx.c", "gunbustr.c", "outrun.c", "segasyse.c", "system18.c", "segac2.c", "sharrier.c", "superchs.c", "undrfire.c" );
local @Family_3 = ( "actfancr.c", "battlera.c", "cbuster.c", "cninja.c", "darkseal.c", "dassault.c", "dec0.c", "deco32.c", "funkyjet.c", "madmotor.c", "rohga.c", "supbtime.c", "tumblep.c", "vaportra.c" );  # CPU deps: "cpu/h6280/h6280.c" 
local @Family_4 = ( "bagman.c", "cclimber.c", "cvs.c", "dkong.c", "epos.c", "galaxian.c", "ladybug.c", "mario.c", "pengo.c", "phoenix.c", "frogger.c", "pacman.c", "scobra.c", "scramble.c", "amidar.c", "malzak.c", "yamato.c" );
local @Family_5 = ( "rampart.c", "arcadecl.c" );
local @Family_6 = ( "artmagic.c", "btoads.c", "coolpool.c", "gottlieb.c", "exterm.c", "harddriv.c", "kinst.c", "lethalj.c", "williams.c", "midyunit.c", "midtunit.c", "mcr3.c", "mcr68.c", "mcr1.c", "mcr2.c", "midvunit.c", "midwunit.c", "midxunit.c", "seattle.c", "tickee.c" );  # CPU deps: "cpu/tms34010/34010fld.c" "cpu/tms34010/tms34010.c" "cpu/dsp32/dsp32.c" "cpu/adsp2100/adsp2100.c" 
local @Family_7 = ( "rastan.c", "asuka.c", "opwolf.c", "rainbow.c", "topspeed.c" );
local @Family_8 = ( "leland.c", "ataxx.c" );
local @Family_9 = ( "baraduke.c", "kncljoe.c", "m62.c", "mpatrol.c", "namcos1.c", "namcos86.c", "pacland.c", "skykid.c", "namcoic.c", "namcos2.c", "tceptor.c", "namconb1.c", "namcos21.c", "travrusa.c", "troangel.c", "yard.c" );  # CPU deps: "cpu/m6800/m6800.c" 
local @Family_10 = ( "thunderj.c", "batman.c", "klax.c", "eprom.c", "vindictr.c" );
local @Family_11 = ( "lkage.c", "battlane.c" );
local @Family_12 = ( "megasys1.c", "bigstrkb.c", "cischeat.c" );
local @Family_13 = ( "bzone.c", "bwidow.c" );
local @Family_14 = ( "ddragon.c", "chinagat.c" );
local @Family_15 = ( "cinemat.c" );  # CPU deps: "cpu/ccpu/ccpu.c" 
local @Family_16 = ( "wiping.c", "clshroad.c" );
local @Family_17 = ( "cojag.c" );  # CPU deps: "cpu/jaguar/jaguar.c" 
local @Family_18 = ( "cps1.c", "cps2.c" );  # CPU deps: "cpu/m68000/m68kcpu.c" "cpu/m68000/m68kmame.c" "cpu/m68000/m68kopac.c" "cpu/m68000/m68kopdm.c" "cpu/m68000/m68kopnz.c" "cpu/m68000/m68kops.c" 
local @Family_19 = ( "drgnmst.c", "playmark.c", "sslam.c" );  # CPU deps: "cpu/pic16c5x/pic16c5x.c" 
local @Family_20 = ( "victory.c", "exidy.c" );
local @Family_21 = ( "exerion.c", "fcombat.c" );
local @Family_22 = ( "gaelco2.c", "gaelco.c" );
local @Family_23 = ( "kaneko16.c", "galpani2.c" );
local @Family_24 = ( "legionna.c", "goal92.c" );
local @Family_25 = ( "snk.c", "hal21.c", "jcross.c", "marvins.c", "sgladiat.c" );
local @Family_26 = ( "trackfld.c", "hyperspt.c", "sbasketb.c", "yiear.c" );
local @Family_27 = ( "tutankhm.c", "junofrst.c", "timeplt.c", "rallyx.c", "locomotn.c", "pooyan.c", "rocnrope.c" );
local @Family_28 = ( "konamigx.c", "konamigq.c", "mystwarr.c" );
local @Family_29 = ( "m72.c", "m90.c", "shisen.c", "vigilant.c" );
local @Family_30 = ( "m107.c", "m92.c" );
local @Family_31 = ( "espial.c", "marineb.c", "zodiack.c" );
local @Family_32 = ( "bublbobl.c", "missb2.c" );
local @Family_33 = ( "tetrisp2.c", "ms32.c" );
local @Family_34 = ( "system32.c", "multi32.c" );
local @Family_35 = ( "nbmj8688.c", "nbmj8891.c" );
local @Family_36 = ( "fromance.c", "pipedrm.c" );
local @Family_37 = ( "nova2001.c", "pkunwar.c" );
local @Family_38 = ( "wiz.c", "rollrace.c" );
local @Family_39 = ( "btime.c", "scregg.c" );
local @Family_40 = ( "segar.c", "sega.c" );
local @Family_41 = ( "nyny.c", "spiders.c" );
local @Family_42 = ( "stvhacks.c", "stv.c" );
local @Family_43 = ( "seta.c", "taito_x.c" );
local @Family_44 = ( "twin16.c", "tmnt.c" );
local @Family_45 = ( "mappy.c", "toypop.c" );
local @Family_46 = ( "twincobr.c", "wardner.c" );
local @Family_47 = ( "ninjaw.c", "warriorb.c" );
local @Family_48 = ( "wc90.c", "wc90b.c" );


local @DriverFamilies = ( \@Family_0, \@Family_1, \@Family_2, \@Family_3, \@Family_4, \@Family_5, \@Family_6, \@Family_7, \@Family_8, \@Family_9, \@Family_10, \@Family_11, \@Family_12, \@Family_13, \@Family_14, \@Family_15, \@Family_16, \@Family_17, \@Family_18, \@Family_19, \@Family_20, \@Family_21, \@Family_22, \@Family_23, \@Family_24, \@Family_25, \@Family_26, \@Family_27, \@Family_28, \@Family_29, \@Family_30, \@Family_31, \@Family_32, \@Family_33, \@Family_34, \@Family_35, \@Family_36, \@Family_37, \@Family_38, \@Family_39, \@Family_40, \@Family_41, \@Family_42, \@Family_43, \@Family_44, \@Family_45, \@Family_46, \@Family_47, \@Family_48 );





	# CPU Families -------------------------------------------------------
#@SkipCPUs = ( "DSP32", "I8085", "JAGUAR", "MIPS", "NEC", "PIC16C5X", "TMS9900" );
@SkipCPUs = ();

	# Notes on CPU families:
	# - Items on the first line have actual source files, 
	#	the rest share the original sources
	# - The families have been derived by reading cpuintrf.c, checking which
	#	#if (HAS_)'s lead to which #includes. Sometimes this fails, in which case
	#   the CPUx macros themselves need to be consulted (example HAS_R5000, provides CPUs
	#   CPU_R5000BE and CPU_R5000LE, no CPU_R5000)

	# The xxxFamily string defines the source directories of the CPU family
	# The xxxClones array defines the list of all CPU_ #defines that use the source files in xxxFamily
	# NOTE: it is very important that the family of the CPU be the first item in the Clones list,
	#       if the family name is the name of a real CPU! (there are cases where it's not, like "MIPS" )

$adsp2100Family = "ADSP2100";
@adsp2100Clones = ( "ADSP2100", "ADSP2101", "ADSP2105", "ADSP2115" );

$armFamily = "ARM";
@armClones = ( "ARM" );

$asapFamily = "ASAP";
@asapClones = ( "ASAP" );

$ccpuFamily = "CCPU";
@ccpuClones = ( "CCPU" );

$dsp32Family = "DSP32";
@dsp32Clones = ( "DSP32C" );

$h6280Family = "H6280";
@h6280Clones = ( "H6280" );

$hd6309Family = "HD6309";
@hd6309Clones = ( "HD6309" );

$i8039Family = "I8039";
@i8039Clones = ( "I8035", "I8039", "I8048", "N7751" );

$i8085Family = "I8085";
@Z80Clones = ( "8080", "8085A" );

$i86Family = "I86";
@i86Clones = ( "I86", 
#			   "I88",		# Disabled in VCPPMame.h
			   "I186" );
#			   "I188",		# Disabled in VCPPMame.h
#			   "I286" );	# Disabled in VCPPMame.h

$i8x41Family = "I8X41";
@i8x41Clones = ( "I8X41" );

$JaguarFamily = "JAGUAR";
@JaguarClones = ( "JAGUARGPU", "JAGUARDSP" );

$KonamiFamily = "KONAMI";
@KonamiClones = ( "KONAMI" );

$m6502Family = "M6502";
@m6502Clones = ( "M6502", 
				 "M65C02", 
#				 "M65SC02", # Disabled in VCPPMame.h
				 "M6510", 
#				 "M6510T",	# Disabled in VCPPMame.h
#				 "M7501",	# Disabled in VCPPMame.h
#				 "M8502",	# Disabled in VCPPMame.h
				 "N2A03", 
				 "DECO16" );
#				 "M4510" );	# Disabled in VCPPMame.h
#				 "M65CE02",	# Disabled in VCPPMame.h
#				 "M6509" ); # Disabled in VCPPMame.h

$m6800Family = "M6800";
@m6800Clones = ( "M6800", "M6801", "M6802", "M6803", "M6808", "HD63701", "NSC8105" );

$m68000Family = "M68000";
@m68000Clones = ( "M68000", "M68010", "M68020", "M68EC020" );

$m6805Family = "M6805";
@m6805Clones = ( "M6805", "M68705", "HD63705" );

$m6809Family = "M6809";
@m6809Clones = ( "M6809" );

$mipsFamily = "MIPS";
@mipsClones = (	"PSXCPU",
				"R3000BE", 
				"R3000LE", 
				"R4600BE", 
				"R4600LE", 
				"R5000BE",
				"R5000LE" );

$NECFamily = "NEC";
@NECClones = ( "V20", "V30", "V33" );

$pic16c5xFamily = "PIC16C5X";
@pic16c5xClones = ( "PIC16C54", "PIC16C55", "PIC16C56", "PIC16C57", "PIC16C58" );

$s2650Family = "S2650";
@s2650Clones = ( "S2650" );

$sh2Family = "SH2";
@sh2Clones = ( "SH2" );

$t11Family = "T11";
@t11Clones = ( "T11" );

$tms32010Family = "TMS32010";
@tms32010Clones = ( "TMS32010" );

$tms32025Family = "TMS32025";
@tms32025Clones = ( "TMS32025" );

$tms32031Family = "TMS32031";
@tms32031Clones = ( "TMS32031" );

$tms34010Family = "TMS34010";
@tms34010Clones = ( "TMS34010", "TMS34020" );

$tms9900Family = "TMS9900";
@tms9900Clones = (	#"TMS9900", # Disabled in VCPPMame.h
					#"TMS9940", # Disabled in VCPPMame.h
					"TMS9980", 
					#"TMS9985", # Disabled in VCPPMame.h
					#"TMS9989", # Disabled in VCPPMame.h
					"TMS9995" );
					#"TMS99105A",	# Disabled in VCPPMame.h
					#"TMS99110A" );	# Disabled in VCPPMame.h

$udp7810Family = "UPD7810";
@udp7810Clones = ( "UPD7810", "UPD7807" );

$v60Family = "V60";
@v60Clones = ( "V60", "V70" );

$Z180Family = "Z180";
@Z180Clones = ( "Z180" );

$Z80Family = "Z80";
@Z80Clones = ( "Z80" );

$z8000Family = "Z8000";
@z8000Clones = ( "Z8000" );

$g65816Family = "g65816";
@g65816Clones = ( "g65816" );

$spc700Family = "spc700";
@spc700Clones = ( "spc700" );

@CPUFamilies = (	\$Z80Family, 
									\$Z180Family, 
									\$i8085Family,
									\$m6502Family, 
									\$h6280Family,
									\$i86Family,
									\$NECFamily,
									\$v60Family,
									\$i8039Family,
									\$i8x41Family,
									\$m6800Family,
									\$m6805Family,
									\$m6809Family,
									\$hd6309Family,
									\$KonamiFamily,
									\$m68000Family,
									\$t11Family,
									\$s2650Family,
									\$tms34010Family,
									\$tms9900Family,
									\$z8000Family,
									\$tms32010Family,
									\$tms32025Family,
									\$tms32031Family,
									\$ccpuFamily,
									\$adsp2100Family,
									\$mipsFamily,
									\$asapFamily,
									\$udp7810Family,
									\$JaguarFamily,
									\$armFamily,
									\$sh2Family,
									\$dsp32Family,
									\$pic16c5xFamily,
									\$g65816Family,
									\$spc700Family );



@CPUClones = (	\@Z80Clones,
								\@Z180Clones,
								\@Z80Clones,
								\@m6502Clones,
								\@h6280Clones,
								\@i86Clones,
								\@NECClones,
								\@v60Clones,
								\@i8039Clones,
								\@i8x41Clones,
								\@m6800Clones,
								\@m6805Clones,
								\@m6809Clones,
								\@hd6309Clones,
								\@KonamiClones,
								\@m68000Clones,
								\@t11Clones,
								\@s2650Clones,
								\@tms34010Clones,
								\@tms9900Clones,
								\@z8000Clones,
								\@tms32010Clones,
								\@tms32025Clones,
								\@tms32031Clones,
								\@ccpuClones,
								\@adsp2100Clones,
								\@mipsClones,
								\@asapClones,
								\@udp7810Clones,
								\@JaguarClones,
								\@armClones,
								\@sh2Clones,
								\@dsp32Clones,
								\@pic16c5xClones,
								\@g65816Clones,
								\@spc700Clones );









print "Sectionizing drivers, sound hardware, and video hardware...\n";

BuildDriverMap();
WriteDriverFunctions();



#------------------------------------------------------------------------------


print "\n\nSectionizing CPU's...\n";

$autoNameNumber = scalar( @CPUFamilies ) + 10;

WriteCPUFunctions();


print GENERATEDFILE "\n//-------------------------------------------------------------\n";
print GENERATEDFILE "//	UnloadCPUSections\n";
print GENERATEDFILE "//-------------------------------------------------------------\n";
print GENERATEDFILE "BOOL UnloadCPUSections( void )\n";
print GENERATEDFILE "{\n";
print GENERATEDFILE "  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.begin();\n";
print GENERATEDFILE "  for( ; i != g_CPUIDToSectionMap.end(); ++i )\n";
print GENERATEDFILE "  {\n";
print GENERATEDFILE "      // Only unload families once (all but the first member are skipped)\n";
print GENERATEDFILE "    if( (*i).first == ";
local $IsFirst = true;
foreach $CloneArrayRef ( @CPUClones ) {
	foreach( @{$CloneArrayRef} ) {
			# Skip the first (key) member of the family
		if( $_ eq $CloneArrayRef->[0] ) {
			next;
		}

			# Print OR
		if( $IsFirst ne true ) {
			print GENERATEDFILE " ||\n        (*i).first == ";
		}
		$IsFirst = false;

			# Print code to ignore this member
		print GENERATEDFILE "CPU_$_";
	}
}
print GENERATEDFILE " )\n";
print GENERATEDFILE "        continue;\n";
print GENERATEDFILE "    XFreeSection( (*i).second.c_str() );\n";
print GENERATEDFILE "  }\n";
print GENERATEDFILE "  return TRUE;\n";
print GENERATEDFILE "}\n\n";
print GENERATEDFILE "\n//-------------------------------------------------------------\n";
print GENERATEDFILE "//	RegisterCPUSectionNames\n";
print GENERATEDFILE "//-------------------------------------------------------------\n";
print GENERATEDFILE "static void RegisterCPUSectionNames( void )\n";
print GENERATEDFILE "{\n";




# Do two passes, one to find the last autoNameNumber, another to actually
# modify the files
print "Pass 1...\n";

#local @CPUDirs = `find ./MAME/src/cpu -path \'*/CVS\' -prune -o -type d -a -print`;
@FILEs    = sort( `find $MAME_CPU_DIR -name *.c` );
@newFILEs = ();
local $OldCPUName = "";



CPU_PASS1_NEXTFILE:
foreach $DriverFileName ( @FILEs ) {
	chomp( $DriverFileName );

	$CPUName = $DriverFileName;
		# Change the CPUName to what will be present in the actual code
		# Drop the ./MAME/src/cpu portion
	$CPUName =~ /^$MAME_CPU_DIR\/(.+)\/.+\.c$/;
	$CPUName = $1;

		# Skip the fake jrcrypt.c file and all the hack files
	foreach $CPUToSkip ( @SkipCPUs ) {
		if( uc($CPUName) eq $CPUToSkip ) {
			print "Skipping $CPUName.\n";
			next CPU_PASS1_NEXTFILE;
		}
	}

	$Family = false;
	$FamilyID = 0;
	foreach( @CPUFamilies ) {
		$FamilyID++;
		if( uc($CPUName) eq $$_ ) {
			print "$CPUName is in family $FamilyID.\n";
			$Family = $FamilyID;
			last;
		}
	}


	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
	sysread( FILE, $File, $size );
	close( FILE );

	if( ($File =~ /\#pragma code_seg/) ) {
			# this should only happen on the first pass
		$File =~ /\#pragma code_seg\(\"CC(\d+)\"\)/;
		$myAutoNameNumber = $1;

			# Unlike the drivers, we want one name for an entire directory
			# so only register on a new directory (CPUName)
		if( $CPUName ne $OldCPUName ) {
			$OldCPUName = $CPUName;
			$ucaseCPUName = uc( $CPUName );
			if( $Family ne false ) {
				$myAutoNameNumber = 0;	# Don't increment (families should always be less anyway)
				print PRELOADFILE "/NOPRELOAD:\"C$Family\"\n";
				print DBGPRELOADFILE "/NOPRELOAD:\"C$Family\"\n";
					# Each real CPU is also listed as a clone, so this would be a duplicate
				#print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$Family\" );\n";

					# Register all the clones for this CPU
				$CloneArray = @CPUClones[$Family-1];
				foreach( @{$CloneArray} ) {
					print GENERATEDFILE "  RegisterSectionID( CPU_$_, \"CPU$Family\" );\n";
				}
			}
			else {
				print PRELOADFILE "/NOPRELOAD:\"C$myAutoNameNumber\"\n";
				print DBGPRELOADFILE "/NOPRELOAD:\"C$myAutoNameNumber\"\n";
				print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$myAutoNameNumber\" );\n";
			}
		}

		if( $myAutoNameNumber >= $autoNameNumber ) {
			$autoNameNumber = $myAutoNameNumber + 1;
		}
	} else {
		push @newFILEs, $DriverFileName;
	}
}




# Second pass, write out the section headers --------------------------
print "Pass 2...\n";
if( scalar( @newFILEs ) == 0 ) {
	print "Nothing to do.\n";
}

$OldCPUName = "";
CPU_PASS2_NEXTFILE:
foreach $DriverFileName ( @newFILEs ) {
	chomp( $DriverFileName );

	$CPUName = $DriverFileName;
		# Change the CPUName to what will be present in the actual code
		# Drop the ./MAME/src/cpu portion
	$CPUName =~ /^$MAME_CPU_DIR\/(.+)\/.+\.c$/;
	$CPUName = $1;

		# Skip the fake jrcrypt.c file and all the hack files
	foreach $CPUToSkip ( @SkipCPUs ) {
		$CPUToSkip =~ s/\.c/\\.c/;
		if( ($CPUName =~ /.*$CPUToSkip/ ) ) {
			print "Skipping $CPUName.\n";
			next CPU_PASS2_NEXTFILE;
		}
	}

	$Family = false;
	$FamilyID = 0;
	foreach( @CPUFamilies ) {
		$FamilyArray = $_;
		$FamilyID++;
		if( uc($CPUName) eq $$_ ) {
			print "$CPUName is in family $FamilyID.\n";
			$Family = $FamilyID;
			last;
		}
	}

		# Unlike the drivers, we want one name for an entire directory
		# so only register on a new directory (CPUName)
	if( $CPUName ne $OldCPUName ) {
		$OldCPUName = $CPUName;
		$ucaseCPUName = uc( $CPUName );
		if( $Family ne false ) {
			print PRELOADFILE "/NOPRELOAD:\"C$Family\"\n";
			print DBGPRELOADFILE "/NOPRELOAD:\"C$Family\"\n";
				# Each real CPU is also listed as a clone, so this would be a duplicate
			#print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$Family\" );\n";

				# Register all the clones for this CPU
			$CloneArray = @CPUClones[$Family-1];
			foreach( @{$CloneArray} ) {
				print GENERATEDFILE "  RegisterSectionID( CPU_$_, \"CPU$Family\" );\n";
			}
		} else {
			$autoNameNumber++;
			print PRELOADFILE "/NOPRELOAD:\"C$autoNameNumber\"\n";
			print DBGPRELOADFILE "/NOPRELOAD:\"C$autoNameNumber\"\n";
			print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$autoNameNumber\" );\n";
		}
	}


	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
	sysread( FILE, $File, $size );
	close( FILE );

		# Write out the section header/footer
	if( $Family ne false ) {
		WriteCPUSectionData( $DriverFileName, $File, $Family );
	} else {
		WriteCPUSectionData( $DriverFileName, $File, $autoNameNumber );
	}
}

print GENERATEDFILE "}\n";
print GENERATEDFILE "#pragma code_seg()\n";
print GENERATEDFILE "#pragma data_seg()\n";
print GENERATEDFILE "#pragma const_seg()\n";
print GENERATEDFILE "#pragma bss_seg()\n\n\n";
print GENERATEDFILE "} // End extern \"C\"\n\n\n";
close( GENERATEDFILE );


close( PRELOADFILE );
close( DBGPRELOADFILE );













#== F U N C T I O N S ===================================================



#------------------------------------------------------------------------
#	WriteSectionData
#------------------------------------------------------------------------
sub WriteSectionData( $$$ ) {
	my $FileName				= shift;
	my $File						= shift;
	my $autoNameNumber	= shift;
	my $DataSectionName = "\"D$autoNameNumber\"";
	my $CodeSectionName = "\"C$autoNameNumber\"";
	my $BSSSectionName	= "\"B$autoNameNumber\"";
	my $ConstSectionName= "\"K$autoNameNumber\"";

	open( FILE, ">$FileName" ) || die "Could not open $FileName for output!\n";

		#open the segment
	my $SegLine = "#pragma code_seg($CodeSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma data_seg($DataSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma bss_seg($BSSSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma const_seg($ConstSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );


		# Merge all of the sections into one segment (identified by $autoNameNumber)
		# Note: It is _very_ important to put the data section first, otherwise the
		#       merged section will be read-only!
	$SegLine = "#pragma comment(linker, \"/merge:D$autoNameNumber=$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:C$autoNameNumber=$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:B$autoNameNumber=$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:K$autoNameNumber=$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );


		#write the old file data
	syswrite( FILE, $File, $size );

		#Close the segment
	$SegLine = "#pragma code_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma data_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma bss_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma const_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );

	close( FILE );
}

#------------------------------------------------------------------------
#	WriteCPUSectionData
#------------------------------------------------------------------------
sub WriteCPUSectionData( $$$ ) {
	my $FileName				= shift;
	my $File						= shift;
	my $autoNameNumber	= shift;
	my $CodeSectionName = "\"CC$autoNameNumber\"";
	my $DataSectionName = "\"CD$autoNameNumber\"";
	my $BSSSectionName	= "\"CB$autoNameNumber\"";
	my $ConstSectionName= "\"CK$autoNameNumber\"";

	open( FILE, ">$FileName" ) || die "Could not open $FileName for output!\n";

		#open the segment
	my $SegLine = "#pragma code_seg($CodeSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma data_seg($DataSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma bss_seg($BSSSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma const_seg($ConstSectionName)\n";
	syswrite( FILE, $SegLine, length($SegLine) );

		# Merge all of the sections into one segment (identified by C$autoNameNumber)
		# Note: It is _very_ important to put the data section first, otherwise the
		#       merged section will be read-only!
	$SegLine = "#pragma comment(linker, \"/merge:CD$autoNameNumber=CPU$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:CC$autoNameNumber=CPU$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:CB$autoNameNumber=CPU$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma comment(linker, \"/merge:CK$autoNameNumber=CPU$autoNameNumber\")\n";
	syswrite( FILE, $SegLine, length($SegLine) );


		#write the old file data
	syswrite( FILE, $File, $size );

		#Close the segment
	$SegLine = "#pragma code_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma data_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma bss_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );
	$SegLine = "#pragma const_seg()\n";
	syswrite( FILE, $SegLine, length($SegLine) );

	close( FILE );
}


#------------------------------------------------------------------------
#	CreateSwitchStubs
#------------------------------------------------------------------------
sub CreateSwitchStubs() {
		# Create the imageblddbg.switch file
	open( DBGPRELOADFILE, ">$ROOT_DIR/imageblddbg.switch" );
	print DBGPRELOADFILE << "EOF";
/IN:"Debug\\MAMEoX.exe"
/OUT:"Debug\\MAMEoX.xbe"
/STACK:"0x20000"
/DEBUG
/NOLOGO
/nolibwarn
/formatud
/testname:"MAMEoX Util (Don't Run)"
/testid:0x4D414D45
/titleimage:"..\\Media\\mox-icon.xpr"
/defaultsaveimage:"..\\Media\\mox-icon.xpr"
EOF

	open( PRELOADFILE, ">$ROOT_DIR/imagebld.switch" );
	print PRELOADFILE << "EOF";
/IN:"Release\\MAMEoX.exe"
/OUT:"Release\\MAMEoX.xbe"
/STACK:"0x20000"
/NOLOGO
/nolibwarn
/formatud
/testname:"MAMEoX Util (Don't Run)"
/testid:0x4D414D45
/titleimage:"..\\Media\\mox-icon.xpr"
/defaultsaveimage:"..\\Media\\mox-icon.xpr"
EOF
}



#------------------------------------------------------------------------
#	CreateHeaderFile
#------------------------------------------------------------------------
sub CreateHeaderFile() {

	open( FILE, ">$MAMEoX_DIR/includes/$GeneratedFileBaseName.h" );
	print FILE << "EOF";
/**
  * \\file      $GeneratedFileBaseName.h
  * \\brief     Registration of MAME files for creation and usage of XBOX
  *             loadable sections.
  *
  * \\note      This file is autogenerated via Sectionize.pl DO NOT EDIT!
  */
#pragma once
//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "osd_cpu.h"
//= P R O T O T Y P E S ================================================

#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckDriverSectionRAM
//! \\brief    Prints the size of each driver/snd/vdeo segment
//-------------------------------------------------------------
void CheckDriverSectionRAM( void );

//-------------------------------------------------------------
//	CheckCPUSectionRAM
//! \\brief    Prints the size of each CPU segment
//-------------------------------------------------------------
void CheckCPUSectionRAM( void );
#endif

//-------------------------------------------------------------
//	InitDriverSectionizer
//! \\brief    Initializes the DriverSectionizer subsystem
//-------------------------------------------------------------
void InitDriverSectionizer( void );

//-------------------------------------------------------------
//	TerminateDriverSectionizer
//! \\brief    Terminates the DriverSectionizer subsystem
//-------------------------------------------------------------
void TerminateDriverSectionizer( void );

//-------------------------------------------------------------
//	LoadDriverSectionByName
//! \\brief    Loads the section associated with the passed name
//!
//! \\param    DriverFileName - The name of the file whose section
//!                             should be loaded
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL LoadDriverSectionByName( const char *DriverFileName );

//-------------------------------------------------------------
//	UnloadDriverSectionByName
//! \\brief    Unloads the section associated with the passed name
//!
//! \\param    DriverFileName - The name of the file whose section
//!                             should be unloaded
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL UnloadDriverSectionByName( const char *DriverFileName );

//-------------------------------------------------------------
//	LoadDriverSections
//! \\brief    Loads all of the driver sections
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL LoadDriverSections( void );

//-------------------------------------------------------------
//	UnloadDriverSections
//! \\brief    Unloads all of the driver sections
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL UnloadDriverSections( void );


//-------------------------------------------------------------
//	InitCPUSectionizer
//! \\brief    Initializes the DriverSectionizer subsystem
//-------------------------------------------------------------
void InitCPUSectionizer( void );

//-------------------------------------------------------------
//	TerminateCPUSectionizer
//! \\brief    Terminates the DriverSectionizer subsystem
//-------------------------------------------------------------
void TerminateCPUSectionizer( void );

//-------------------------------------------------------------
//	LoadCPUSectionByID
//! \\brief    Loads the section associated with the passed name
//!
//! \\param    CPUID - The ID of the CPU whose section
//!                    should be loaded
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL LoadCPUSectionByID( UINT32 CPUID );

//-------------------------------------------------------------
//	UnloadCPUSectionByID
//! \\brief    Unloads the section associated with the passed name
//!
//! \\param    CPUID - The ID of the CPU whose section
//!                    should be loaded
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL UnloadCPUSectionByID( UINT32 CPUID );

//-------------------------------------------------------------
//	LoadCPUSections
//! \\brief    Loads all of the CPU sections
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL LoadCPUSections( void );

//-------------------------------------------------------------
//	UnloadCPUSections
//! \\brief    Unloads all of the CPU sections
//!
//! \\return   BOOL - Operation status
//! \\retval   TRUE - success
//! \\return   FALSE - Failure
//-------------------------------------------------------------
BOOL UnloadCPUSections( void );
#ifdef __cplusplus
} // End extern "C"
#endif
EOF

	close( FILE );
}


#------------------------------------------------------------------------
#	CreateSourceFileStub
#------------------------------------------------------------------------
sub CreateSourceFileStub() {

	open( GENERATEDFILE, ">$MAMEoX_DIR/sources/$GeneratedFileBaseName.cpp" );
	print GENERATEDFILE "/**\r\n";
	print GENERATEDFILE "  * \\file      $GeneratedFileBaseName.cpp\r\n";
	print GENERATEDFILE "  * \\brief     Registration of Driver/CPU files for creation and usage of XBOX\r\n";
	print GENERATEDFILE "  *             loadable sections\r\n";
	print GENERATEDFILE "  *\r\n";
	print GENERATEDFILE "  * \\note      This file is autogenerated via Sectionize.pl, so any\r\n";
	print GENERATEDFILE "  *               changes will probably be lost.\r\n";
	print GENERATEDFILE "  */\r\n\r\n";



	print GENERATEDFILE << "EOF";
//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include <stdio.h>
#include <map>
#include <string>
#include "DebugLogger.h"


extern "C" {
#include "osd_cpu.h"
#include "cpuintrf.h"
}


//= D E F I N E S ======================================================

struct driverSectionRegistration_t 
{
		// For some reason making these a const char, then unloading
		// the DRVSNIZE will cause a crash in retail mode. I'm not at
		// all sure exactly why this is yet, my only guess is that there
		// is something odd about how quoted constants in initializers are
		// placed into the XBE section.
  //const char *m_driverName;
  //const char *m_sectionName;
	char	m_driverName[16];
	char	m_sectionName[8];
};


#define REGISTER_DRIVERSECTION( driverName, sectionName )     { driverName, sectionName }



//= G L O B A L = V A R S ==============================================
static std::map< UINT, std::string >         g_CPUIDToSectionMap;

static BOOL																	 g_driverSectionizerLoaded = TRUE;
static BOOL																	 g_cpuSectionizerLoaded = TRUE;

//= P R O T O T Y P E S ================================================
extern "C" static void RegisterCPUSectionNames( void );


//= F U N C T I O N S ==================================================
extern "C" {

//-------------------------------------------------------------
//	InitDriverSectionizer
//-------------------------------------------------------------
void InitDriverSectionizer( void )
{
	if( !g_driverSectionizerLoaded )
	{
		if( !XLoadSection( "DRVSNIZE" ) )
		{
			PRINTMSG(( T_ERROR, "XLoadSection failed! 0x%X\\r\\n", GetLastError() ));
		}
		else
			g_driverSectionizerLoaded = TRUE;
	}
}


//-------------------------------------------------------------
//	TerminateDriverSectionizer
//-------------------------------------------------------------
void TerminateDriverSectionizer( void )
{
  if( !XFreeSection( "DRVSNIZE" ) )
	{
		PRINTMSG(( T_ERROR, "XFreeSection failed! 0x%X\\r\\n", GetLastError() ));
	}
	else
		g_driverSectionizerLoaded = FALSE;
}

//-------------------------------------------------------------
//	InitCPUSectionizer
//-------------------------------------------------------------
void InitCPUSectionizer( void )
{
  g_CPUIDToSectionMap.clear();

	if( !g_cpuSectionizerLoaded )
	{
		if( !XLoadSection( "CPUSNIZE" ) )
		{
			PRINTMSG(( T_ERROR, "XLoadSection failed! 0x%X\\r\\n", GetLastError() ));
		}
		else
			g_cpuSectionizerLoaded = TRUE;
	}
  RegisterCPUSectionNames();
}

//-------------------------------------------------------------
//	TerminateCPUSectionizer
//-------------------------------------------------------------
void TerminateCPUSectionizer( void )
{
  g_CPUIDToSectionMap.clear();
  if( !XFreeSection( "CPUSNIZE" ) )
	{
		PRINTMSG(( T_ERROR, "XFreeSection failed! 0x%X\\r\\n", GetLastError() ));
	}
	else
		g_cpuSectionizerLoaded = FALSE;
}


EOF
}

#------------------------------------------------------------------------
#	WriteDriverFunctions
#------------------------------------------------------------------------
sub WriteDriverFunctions() {

	print GENERATEDFILE << "EOF";
#pragma code_seg("DRVCSNZE")
#pragma data_seg("DRVDSNZE")
#pragma bss_seg("DRVBSNZE")
#pragma const_seg("DRVKSNZE")
#pragma comment(linker, "/merge:DRVDSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVCSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVBSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVKSNZE=DRVSNIZE")

EOF
	print GENERATEDFILE "#define   NUM_DRIVERSECTIONS				".scalar( keys( %DriverNameToSectionMap ) )."\r\n";
	print GENERATEDFILE "static const driverSectionRegistration_t			g_driverSectionRegistry[NUM_DRIVERSECTIONS] = { \r\n";
		# Register the driver sections
	local $IsFirst = true;
	foreach $File ( sort( keys( %DriverNameToSectionMap ) ) ) {
		local $SectionID = $DriverNameToSectionMap{$File};
		print PRELOADFILE "/NOPRELOAD:\"$SectionID\"\r\n";
		print DBGPRELOADFILE "/NOPRELOAD:\"$SectionID\"\r\n";

		if( $IsFirst eq true ) {
			print GENERATEDFILE "           REGISTER_DRIVERSECTION( \"$File\", \"$SectionID\" )";
			$IsFirst = false;
		} else {
			print GENERATEDFILE ",\r\n           REGISTER_DRIVERSECTION( \"$File\", \"$SectionID\" )";
		}
	}

	print GENERATEDFILE " };\r\n";


	print GENERATEDFILE << "EOF";


//-------------------------------------------------------------
//	IsDriverClone
//-------------------------------------------------------------
inline BOOL IsDriverClone( const char *driverName )
{
EOF
	local $IsFirst = true;
	foreach $FamilyArrayRef ( @DriverFamilies ) {
		foreach( @$FamilyArrayRef ) {
				# Skip the first (key) member of the family
			if( $_ eq @{$FamilyArrayRef}[0] ) {
				next;
			}

				# Print OR
			if( $IsFirst ne true ) {
				print GENERATEDFILE " ||\n        !strcmp( driverName, ";
			} else {
				print GENERATEDFILE "  return( !strcmp( driverName, ";
				$IsFirst = false;
			}

				# Print code to ignore this member
			print GENERATEDFILE "\"$_\" )";
		}
	}


	print GENERATEDFILE << "EOF";
 );
}

#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckDriverSectionRAM
//-------------------------------------------------------------
void CheckDriverSectionRAM( void )
{
  DWORD total = 0;

	const driverSectionRegistration_t *entry = (const driverSectionRegistration_t *)g_driverSectionRegistry;
	for( int i = 0; i < NUM_DRIVERSECTIONS; ++entry, ++i )
	{
      // Only test families once (all but the first member are skipped)
		if( !IsDriverClone( entry->m_driverName ) )
		{
			HANDLE h = XGetSectionHandle( entry->m_sectionName );
			if( h != INVALID_HANDLE_VALUE )
			{
				UINT32 sz = XGetSectionSize( h );
				PRINTMSG(( T_INFO, "Driver %s %lu", entry->m_driverName, sz ));
				total += sz;
			}
			else
				PRINTMSG(( T_ERROR, "Invalid section %s for file %s!", entry->m_sectionName, entry->m_driverName ));
		}
	}
  PRINTMSG(( T_INFO, "Total %lu bytes\\n", total ));
}
#endif


EOF

	print GENERATEDFILE << "EOF";


//-------------------------------------------------------------
//	LoadDriverSections
//-------------------------------------------------------------
BOOL LoadDriverSections( void )
{
	const driverSectionRegistration_t *entry = (const driverSectionRegistration_t *)g_driverSectionRegistry;
	for( int i = 0; i < NUM_DRIVERSECTIONS; ++entry, ++i )
	{
      // Only load families once (all but the first member are skipped)
		if( !IsDriverClone( entry->m_driverName ) )
		{
			if( !XLoadSection( entry->m_sectionName ) )
			{
				PRINTMSG(( T_ERROR, "XLoadSection %s for driver %s failed! 0x%X", entry->m_sectionName, entry->m_driverName, GetLastError() ));
				return FALSE;
			}
		}
  }
  return TRUE;
}

//-------------------------------------------------------------
//	UnloadDriverSections
//-------------------------------------------------------------
BOOL UnloadDriverSections( void )
{
	const driverSectionRegistration_t *entry = (const driverSectionRegistration_t *)g_driverSectionRegistry;
	for( int i = 0; i < NUM_DRIVERSECTIONS; ++entry, ++i )
	{
      // Only unload families once (all but the first member are skipped)
		if( !IsDriverClone( entry->m_driverName ) )
		{
		  if( !XFreeSection( entry->m_sectionName ) )
			{
				PRINTMSG(( T_ERROR, "XFreeSection %s for driver %s failed! 0x%X", entry->m_sectionName, entry->m_driverName, GetLastError() ));
			}
		}
	}

	return TRUE;
}

//-------------------------------------------------------------
//	LoadDriverSectionByName
//-------------------------------------------------------------
BOOL LoadDriverSectionByName( const char *driverFileName )
{
	const driverSectionRegistration_t *entry = (const driverSectionRegistration_t *)g_driverSectionRegistry;
	for( int i = 0; i < NUM_DRIVERSECTIONS; ++entry, ++i )
	{
		if( !strcmp( entry->m_driverName, driverFileName ) )
		{
			PRINTMSG(( T_INFO, "Load section %s for driver %s\\n", entry->m_sectionName, entry->m_driverName ));
			if( !XLoadSection( entry->m_sectionName ) )
			{
				PRINTMSG(( T_ERROR, "XLoadSection %s for driver %s failed! 0x%X", entry->m_sectionName, entry->m_driverName, GetLastError() ));
				return FALSE;
			}

			return TRUE;
		}
	}

	PRINTMSG(( T_INFO, "No section registered for driver %s!\\n", driverFileName ));
  return FALSE;
}


//-------------------------------------------------------------
//	UnloadDriverSectionByName
//-------------------------------------------------------------
BOOL UnloadDriverSectionByName( const char *driverFileName )
{
	const driverSectionRegistration_t *entry = (const driverSectionRegistration_t *)g_driverSectionRegistry;
	for( int i = 0; i < NUM_DRIVERSECTIONS; ++entry, ++i )
	{
		if( !strcmp( entry->m_driverName, driverFileName ) )
		{
			PRINTMSG(( T_INFO, "Free section %s for driver %s\\n", entry->m_sectionName, entry->m_driverName ));
			if( !XFreeSection( entry->m_sectionName ) )
			{
				PRINTMSG(( T_ERROR, "XFreeSection %s for driver %s failed! 0x%X", entry->m_sectionName, entry->m_driverName, GetLastError() ));
				return FALSE;
			}

			return TRUE;
		}
	}

	PRINTMSG(( T_INFO, "No section registered for driver %s!\\n", driverFileName ));
  return FALSE;
}

#pragma code_seg()			// End of DRVSNIZE
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()


EOF
}



#------------------------------------------------------------------------
#	WriteCPUFunctions
#------------------------------------------------------------------------
sub WriteCPUFunctions() {
	# Create the CPUSections.cpp file
print GENERATEDFILE << "EOF";

  //--- CPU Functions -----------------------------------------------

#pragma code_seg( "CPUCSNZE" )
#pragma data_seg( "CPUDSNZE" )
#pragma const_seg( "CPUKSNZE" )
#pragma bss_seg( "CPUBSNZE" )
#pragma comment(linker, "/merge:CPUCSNZE=CPUSNIZE")
#pragma comment(linker, "/merge:CPUDSNZE=CPUSNIZE")
#pragma comment(linker, "/merge:CPUKSNZE=CPUSNIZE")
#pragma comment(linker, "/merge:CPUBSNZE=CPUSNIZE")

#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckCPUSectionRAM
//-------------------------------------------------------------
void CheckCPUSectionRAM( void )
{
  DWORD total = 0;
  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.begin();
  for( ; i != g_CPUIDToSectionMap.end(); ++i )
  {
    HANDLE h = XGetSectionHandle( (*i).second.c_str() );
    if( h != INVALID_HANDLE_VALUE )
    {
      UINT32 sz = XGetSectionSize( h );
      PRINTMSG(( T_INFO, "CPU%lu %lu", (*i).first, sz ));
      total += sz;
    }
    else
      PRINTMSG(( T_ERROR, "Invalid section %s for CPU%lu!", (*i).second.c_str(), (*i).first ));
  }
  PRINTMSG(( T_INFO, "Total %lu bytes\\n", total ));
}
#endif


//-------------------------------------------------------------
//	RegisterSectionID
//-------------------------------------------------------------
static void RegisterSectionID( UINT32 CPUID, const char *DataSectionName )
{
    // Add the section name to the map
  g_CPUIDToSectionMap[ CPUID ] = DataSectionName;
}


//-------------------------------------------------------------
//	LoadCPUSectionByID
//-------------------------------------------------------------
BOOL LoadCPUSectionByID( UINT32 CPUID )
{
  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.find( CPUID );
  if( i == g_CPUIDToSectionMap.end() )
    return FALSE;
  void *addr;
  PRINTMSG(( T_INFO, "Load section CPU%lu, ID %s\\n", CPUID, (*i).second.c_str() ));
  addr = XLoadSection( (*i).second.c_str() );
  if( !addr )
  {
    UINT32 lastErr = GetLastError();
    PRINTMSG(( T_ERROR, "XLoadSection failed for section %s! 0x%X\\r\\n", (*i).second.c_str(), lastErr ));
  }
  return TRUE;
}


//-------------------------------------------------------------
//	UnloadCPUSectionByID
//-------------------------------------------------------------
BOOL UnloadCPUSectionByID( UINT32 CPUID )
{
  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.find( CPUID );
  if( i == g_CPUIDToSectionMap.end() )
    return FALSE;
  return XFreeSection( (*i).second.c_str() );
}


//-------------------------------------------------------------
//	LoadCPUSections
//-------------------------------------------------------------
BOOL LoadCPUSections( void )
{
  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.begin();
  for( ; i != g_CPUIDToSectionMap.end(); ++i )
  {
    if( !XLoadSection( (*i).second.c_str() ) )
    {
      PRINTMSG(( T_ERROR, "Failed to load section %s!", (*i).second.c_str() ));
      //return FALSE;
    }
  }
  return TRUE;
}

EOF
}





#------------------------------------------------------------------------
#	BuildDriverMap
#------------------------------------------------------------------------
sub BuildDriverMap() {

	local @newFILEs;
	local @FILEs = sort( `find $MAME_DRIVER_DIR -name *.c` );
	$autoNameNumber = scalar( @DriverFamilies ) + 50;

	# Do two passes, one to find the last autoNameNumber, another to actually
	# modify the files
	print "Pass 1...\n";

	DRIVER_PASS1_NEXTFILE:
	foreach $DriverFileName ( @FILEs ) {
		chomp( $DriverFileName );

			# Change the DriverName to what will be present in the actual MAME code
			# Drop the ./MAME/ portion
		$DriverFileName =~ /^$MAME_DRIVER_DIR\/(.*\.c)$/;
		local $DriverNoPath = $1;

			# Skip the fake jrcrypt.c file and all the hack files
		foreach( @SkipDrivers ) {
			if( ($DriverNoPath eq $_ ) ) {
				print "Skipping $DriverNoPath.\n";
				next DRIVER_PASS1_NEXTFILE;
			}
		}


		$Family = false;
		$FamilyID = 0;
		foreach $FamilyArrayRef ( @DriverFamilies ) {
			$FamilyID++;
			foreach( @{$FamilyArrayRef} ) {
				if( ($DriverNoPath eq $_ ) ) {
					print "$DriverNoPath is in family $FamilyID.\n";
					$Family = $FamilyID;
					last;
				}
			}
		}



		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

		open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
		sysread( FILE, $File, $size );
		close( FILE );

		if( ($File =~ /\#pragma code_seg/) ) {
				# this should only happen on the first pass
			if( $Family ne false ) {
				$DriverNameToSectionMap{$DriverNoPath} = $Family;
				$myAutoNameNumber = 0;	# Don't increment (families should always be less anyway)
			} else {
				$File =~ /\#pragma code_seg\(\"C(\d+)\"\)/;
				$myAutoNameNumber = $1;
				$DriverNameToSectionMap{$DriverNoPath} = $myAutoNameNumber;
			}


			if( $myAutoNameNumber >= $autoNameNumber ) {
				$autoNameNumber = $myAutoNameNumber + 1;
			}		

		} else {
			 push @newFILEs, $DriverFileName;
		}
	}


	# Second pass, write out the section headers
	print "Pass 2...\n";
	if( scalar( @newFILEs ) == 0 ) {
		print "Nothing to do.\n";
	}

	DRIVER_PASS2_NEXTFILE:
	foreach $DriverFileName ( @newFILEs ) {
		chomp( $DriverFileName );

			# Change the DriverName to what will be present in the actual MAME code
			# Drop the ./MAME/ portion
		$DriverFileName =~ /^$MAME_DRIVER_DIR\/(.*\.c)$/;
		local $DriverNoPath = $1;

			# Skip the fake jrcrypt.c file and all the hack files
		foreach( @SkipDrivers ) {
			if( ($DriverNoPath eq $_ ) ) {
				print "Skipping $DriverNoPath.\n";
				next DRIVER_PASS2_NEXTFILE;
			}
		}

		$Family = false;
		$FamilyID = 0;
		foreach $FamilyArrayRef ( @DriverFamilies ) {
			$FamilyID++;
			foreach( @{$FamilyArrayRef} ) {
				if( ($DriverNoPath eq $_ ) ) {
					print "$DriverNoPath is in family $FamilyID.\n";
					$Family = $FamilyID;
					last;
				}
			}
		}

		if( $Family ne false ) {
				$DriverNameToSectionMap{$DriverNoPath} = $Family;
		} else {
			$DriverNameToSectionMap{$DriverNoPath} = $autoNameNumber;
		}

		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

		open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
		sysread( FILE, $File, $size );
		close( FILE );

			# Write out the section header/footer
		if( $Family ne false ) {
			WriteSectionData( $DriverFileName, $File, $Family );
		} else {
			WriteSectionData( $DriverFileName, $File, $autoNameNumber );
		}

			# Also do the vidhdrw file, if one exists
		$VidHardwareName = $DriverFileName;
		$VidHardwareName =~ s/\/drivers\//\/vidhrdw\//;

		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $VidHardwareName );

		if( open( FILE, "<$VidHardwareName" ) ) {
			$File = "";
			sysread( FILE, $File, $size );
			close( FILE );

			if( !($File =~ /\#pragma code_seg/) ) {
				if( $Family ne false ) {
					WriteSectionData( $VidHardwareName, $File, $Family );
				} else {
					WriteSectionData( $VidHardwareName, $File, $autoNameNumber );
				}
			}
		}

			# Also do the sndhrdw file, if one exists
		$SoundHardwareName = $DriverFileName;
		$SoundHardwareName =~ s/\/drivers\//\/sndhrdw\//;

		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $SoundHardwareName );

		if( open( FILE, "<$SoundHardwareName" ) ) {
			$File = "";
			sysread( FILE, $File, $size );
			close( FILE );

			if( !($File =~ /\#pragma code_seg/) ) {
				if( $Family ne false ) {
					WriteSectionData( $SoundHardwareName, $File, $Family );
				} else {
					WriteSectionData( $SoundHardwareName, $File, $autoNameNumber );
				}
			}
		}

			# Also do the machine file, if one exists
		$MachineHardwareName = $DriverFileName;
		$MachineHardwareName =~ s/\/drivers\//\/machine\//;

		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $MachineHardwareName );

		if( open( FILE, "<$MachineHardwareName" ) ) {
			$File = "";
			sysread( FILE, $File, $size );
			close( FILE );

			if( !($File =~ /\#pragma code_seg/) ) {
				if( $Family ne false ) {
					WriteSectionData( $MachineHardwareName, $File, $Family );
				} else {
					WriteSectionData( $MachineHardwareName, $File, $autoNameNumber );
				}
			}
		}

		$autoNameNumber++;
	}
}
