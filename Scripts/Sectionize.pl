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

use constant PREFIX_DRIVER		=> 'D';
use constant PREFIX_MACHINE		=> 'M';
use constant PREFIX_SOUND			=> 'N';
use constant PREFIX_SNDHRDW   => 'S';
use constant PREFIX_VIDHRDW		=> 'V';









	#local @SkipCPUs = ( "DSP32", "I8085", "JAGUAR", "MIPS", "NEC", "PIC16C5X", "TMS9900" );
	local @SkipCPUs = ();

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

	local $adsp2100Family = "ADSP2100";
	local @adsp2100Clones = ( "ADSP2100", "ADSP2101", "ADSP2105", "ADSP2115" );

	local $armFamily = "ARM";
	local @armClones = ( "ARM" );

	local $asapFamily = "ASAP";
	local @asapClones = ( "ASAP" );

	local $ccpuFamily = "CCPU";
	local @ccpuClones = ( "CCPU" );

	local $dsp32Family = "DSP32";
	local @dsp32Clones = ( "DSP32C" );

	local $h6280Family = "H6280";
	local @h6280Clones = ( "H6280" );

	local $hd6309Family = "HD6309";
	local @hd6309Clones = ( "HD6309" );

	local $i8039Family = "I8039";
	local @i8039Clones = ( "I8035", "I8039", "I8048", "N7751" );

	local $i8085Family = "I8085";
	local @Z80Clones = ( "8080", "8085A" );

	local $i86Family = "I86";
	local @i86Clones = ( "I86", 
	#			   "I88",		# Disabled in VCPPMame.h
					 "I186" );
	#			   "I188",		# Disabled in VCPPMame.h
	#			   "I286" );	# Disabled in VCPPMame.h

	local $i8x41Family = "I8X41";
	local @i8x41Clones = ( "I8X41" );

	local $JaguarFamily = "JAGUAR";
	local @JaguarClones = ( "JAGUARGPU", "JAGUARDSP" );

	local $KonamiFamily = "KONAMI";
	local @KonamiClones = ( "KONAMI" );

	local $m6502Family = "M6502";
	local @m6502Clones = ( "M6502", 
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

	local $m6800Family = "M6800";
	local @m6800Clones = ( "M6800", "M6801", "M6802", "M6803", "M6808", "HD63701", "NSC8105" );

	local $m68000Family = "M68000";
	local @m68000Clones = ( "M68000", "M68010", "M68020", "M68EC020" );

	local $m6805Family = "M6805";
	local @m6805Clones = ( "M6805", "M68705", "HD63705" );

	local $m6809Family = "M6809";
	local @m6809Clones = ( "M6809" );

	local $mipsFamily = "MIPS";
	local @mipsClones = (	"PSXCPU",
					"R3000BE", 
					"R3000LE", 
					"R4600BE", 
					"R4600LE", 
					"R5000BE",
					"R5000LE" );

	local $NECFamily = "NEC";
	local @NECClones = ( "V20", "V30", "V33" );

	local $pic16c5xFamily = "PIC16C5X";
	local @pic16c5xClones = ( "PIC16C54", "PIC16C55", "PIC16C56", "PIC16C57", "PIC16C58" );

	local $s2650Family = "S2650";
	local @s2650Clones = ( "S2650" );

	local $sh2Family = "SH2";
	local @sh2Clones = ( "SH2" );

	local $t11Family = "T11";
	local @t11Clones = ( "T11" );

	local $tms32010Family = "TMS32010";
	local @tms32010Clones = ( "TMS32010" );

	local $tms32025Family = "TMS32025";
	local @tms32025Clones = ( "TMS32025" );

	local $tms32031Family = "TMS32031";
	local @tms32031Clones = ( "TMS32031" );

	local $tms34010Family = "TMS34010";
	local @tms34010Clones = ( "TMS34010", "TMS34020" );

	local $tms9900Family = "TMS9900";
	local @tms9900Clones = (	#"TMS9900", # Disabled in VCPPMame.h
						#"TMS9940", # Disabled in VCPPMame.h
						"TMS9980", 
						#"TMS9985", # Disabled in VCPPMame.h
						#"TMS9989", # Disabled in VCPPMame.h
						"TMS9995" );
						#"TMS99105A",	# Disabled in VCPPMame.h
						#"TMS99110A" );	# Disabled in VCPPMame.h

	local $udp7810Family = "UPD7810";
	local @udp7810Clones = ( "UPD7810", "UPD7807" );

	local $v60Family = "V60";
	local @v60Clones = ( "V60", "V70" );

	local $Z180Family = "Z180";
	local @Z180Clones = ( "Z180" );

	local $Z80Family = "Z80";
	local @Z80Clones = ( "Z80" );

	local $z8000Family = "Z8000";
	local @z8000Clones = ( "Z8000" );

	local $g65816Family = "g65816";
	local @g65816Clones = ( "g65816" );

	local $spc700Family = "spc700";
	local @spc700Clones = ( "spc700" );

	local @Families = (	\$Z80Family, 
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



	local @Clones = ( \@Z80Clones,
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






%DependencyHash;
RebuildDependencyHash( "Dependencies.txt" ) || die( "Failed to rebuild dependency hash. Run FindDependencies.pl!\n" );

	# Maps from a certain file name to its section
%FilenameToSectionMap;

	# Array of all the sectionized driver-style filenames
@DriverTypeFilenames;
%CPUIDToSectionNameHash;

  # Base name for the generated .h and .cpp files
$GeneratedFileBaseName = "XBESectionUtil";

print "Generating stub files...\n";
CreateHeaderFile();
CreateSwitchStubs();
CreateSourceFileStub();


#=== Drivers/Hardware ======================================================================
print "\n\nSectionizing Driver dir...\n";
SectionizeDriverTypeDir( $MAME_DRIVER_DIR, PREFIX_DRIVER );

print "\n\nSectionizing Machine dir...\n";
SectionizeDriverTypeDir( $MAME_MACHINE_DIR, PREFIX_MACHINE );

print "\n\nSectionizing Sound dir...\n";
SectionizeDriverTypeDir( $MAME_SOUND_DIR, PREFIX_SOUND );

print "\n\nSectionizing Sndhrdw dir...\n";
SectionizeDriverTypeDir( $MAME_SNDHRDW_DIR, PREFIX_SNDHRDW );

print "\n\nSectionizing Vidhrdw dir...\n";
SectionizeDriverTypeDir( $MAME_VIDHRDW_DIR, PREFIX_VIDHRDW );

print "\n\nSectionizing CPUs...\n";
SectionizeCPUDir();





	# Register all of the driver-style files
print GENERATEDFILE "static const char													*g_driverTypeSections[] = {\r\n";
foreach $Key ( sort( @DriverTypeFilenames ) ) {
	print GENERATEDFILE "                 \"$FilenameToSectionMap{$Key}\"";
	if( $Key ne $DriverTypeFilenames[-1] ) {
		print GENERATEDFILE ",\r\n";
	}
}
print GENERATEDFILE " };\r\n";
print GENERATEDFILE "static const DWORD			                         g_numDriverTypeSections = sizeof(g_driverTypeSections) / sizeof(g_driverTypeSections[0]);\r\n\r\n";


	# Build up a big static table of driver files and their dependencies
print GENERATEDFILE "static const driverDependencyEntry_t				g_driverDependencyGraph[] = {\r\n";

$NumRealDrivers = 0;
foreach $Key ( sort( @DriverTypeFilenames ) ) {
		# See if we're dealing with an actual driver
	if( $Key =~ /drivers\/(.*)/ ) {
		$NumRealDrivers++;
		local $DriverName = "src\\\\drivers\\\\$1";
		print GENERATEDFILE "REGISTER_DRIVERSECTION_START( \"$DriverName\" )\r\n";

			# Add the actual driver section
		print GENERATEDFILE "  DRIVER_DEPENDENCY(\"$FilenameToSectionMap{$Key}\")\r\n";

			# Add all the driver dependencies
		$DependencyArrayRef = $DependencyHash{$Key};

		foreach $Dependency ( @$DependencyArrayRef ) {
			$SectionID = $FilenameToSectionMap{$Dependency};
			if( !defined( $SectionID ) ) {
				print "Warning: Dependency $Dependency does not map to a section!\n";
			} else {
				print GENERATEDFILE "  DRIVER_DEPENDENCY(\"$SectionID\")\r\n";
			}
		}

		print GENERATEDFILE "REGISTER_DRIVERSECTION_END()\r\n\r\n";
	} else {
			# Don't preload non-driver files
		print PRELOADFILE "/NOPRELOAD:\"$FilenameToSectionMap{$Key}\"\n";
		print DBGPRELOADFILE "/NOPRELOAD:\"$FilenameToSectionMap{$Key}\"\n";
	}
}

print GENERATEDFILE "};\r\n\r\n";

print GENERATEDFILE "static const DWORD			                         g_numDriverDependencyGraphEntries = $NumRealDrivers;\r\n";



WriteDriverFileStub();

print GENERATEDFILE << "EOF";

#pragma code_seg()  // End of DRVSNIZE
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()


EOF






















#------------------------------------------------------------------------------

WriteCPUFileStub();

	# Section ID's to be written to the switch file(s)
@CPUSwitchFileEntries;

print GENERATEDFILE << "EOF";
\n//-------------------------------------------------------------
//	UnloadCPUSections
//-------------------------------------------------------------
BOOL UnloadCPUSections( void )
{
  std::map< UINT32, std::string >::iterator i = g_CPUIDToSectionMap.begin();
  for( ; i != g_CPUIDToSectionMap.end(); ++i )
  {
      // Only unload clones once (all but the first member must be skipped)
EOF

print GENERATEDFILE "    if( (*i).first == ";
local $IsFirst = true;
foreach $CloneArray ( @Clones ) {
	foreach( @$CloneArray ) {
			# Skip the first (key) member of the family
		if( $_ eq $CloneArray->[0] ) {
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

print GENERATEDFILE << "EOF";
 )
        continue;
    XFreeSection( (*i).second.c_str() );
  }
  return TRUE;
}


//-------------------------------------------------------------
//	RegisterCPUSectionNames
//-------------------------------------------------------------
static void RegisterCPUSectionNames( void )
{
EOF

  # Print out the registration calls
foreach $Key ( sort( keys( %CPUIDToSectionNameHash ) ) ) {
	print GENERATEDFILE "  RegisterSectionID( $Key, $CPUIDToSectionNameHash{$Key} );\n";
}


print GENERATEDFILE << "EOF";
}

#pragma code_seg()   // CPUCSNZE
#pragma data_seg()

} // End extern "C"


EOF
close( GENERATEDFILE );

	# Write out the switch file entries for CPUs
foreach $Entry ( @CPUSwitchFileEntries ) {
	print PRELOADFILE "/NOPRELOAD:\"$Entry\"\n";
	print DBGPRELOADFILE "/NOPRELOAD:\"$Entry\"\n";
}
close( PRELOADFILE );
close( DBGPRELOADFILE );


print "\n\n\nOperation complete!\n";











#== F U N C T I O N S ===================================================

#------------------------------------------------------------------------
# CopyArray
#------------------------------------------------------------------------
sub CopyArray( $ ) {
	local $ArrayToCopyRef = shift;
	local @Array = @$ArrayToCopyRef;
	return \@Array;
}


#------------------------------------------------------------------------
#	RebuildDependencyHash
#------------------------------------------------------------------------
sub RebuildDependencyHash( $ ) {
	local $DependencyHashFile = shift;
	open( FILE, "<$DependencyHashFile" ) || return false;
	local $KeyValue = undef;
	local @DependencyArray;

	while( <FILE> ) {
		$Line = $_;
		if( $Line =~ /\*\s+$MAME_DIR\/debug_obj\/(.+\/.+)\.obj/ ) {
				# New key value
				#* ../MAME/debug_obj/drivers/namcos22.obj
				# Store the dependency array if we have one
			if( defined( $KeyValue ) && scalar(@DependencyArray) > 0 ) {
				$DependencyHash{"$KeyValue.c"} = CopyArray( \@DependencyArray );
				@DependencyArray = ();
			}

			$KeyValue = $1;

		} elsif( $_ =~ />\s+$MAME_DIR\/debug_obj\/(.+\/.+)\.obj/ ) {
				#> ../MAME/debug_obj/vidhrdw/namcos22.obj
			push( @DependencyArray, "$1.c" );
		}
	}
	close( FILE );

		# Append the last item
	if( scalar( @DependencyArray ) > 0 ) {
		$DependencyHash{"$KeyValue.c"} = CopyArray( \@DependencyArray );
	}
}

#------------------------------------------------------------------------
#	WriteDriverTypeSectionData
#------------------------------------------------------------------------
sub WriteDriverTypeSectionData( $$$ ) {
	local $FileName					= shift;
	local $File							= shift;
	local $SectionID				= shift;
	local $DataSectionName	= "D$SectionID";
	local $CodeSectionName	= "C$SectionID";
	local $BSSSectionName		= "B$SectionID";
	local $ConstSectionName	= "K$SectionID";

	open( FILE, ">$FileName" ) || die "Could not open $FileName for output!\n";

		#open the segment
	print FILE "#pragma code_seg(\"$CodeSectionName\")\r\n";
	print FILE "#pragma data_seg(\"$DataSectionName\")\r\n";
	print FILE "#pragma bss_seg(\"$BSSSectionName\")\r\n";
	print FILE "#pragma const_seg(\"$ConstSectionName\")\r\n";

		# Merge all of the sections into one segment (identified by $autoNameNumber)
		# Note: It is _very_ important to put the data section first, otherwise the
		#       merged section will be read-only!
	print FILE "#pragma comment(linker, \"/merge:$DataSectionName=$SectionID\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:$CodeSectionName=$SectionID\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:$BSSSectionName=$SectionID\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:$ConstSectionName=$SectionID\")\r\n";


		#write the original file data
	print FILE $File;


		#Close the segment
	print FILE "#pragma code_seg()\r\n";
	print FILE "#pragma data_seg()\r\n";
	print FILE "#pragma bss_seg()\r\n";
	print FILE "#pragma const_seg()\r\n";

	close( FILE );
}



#------------------------------------------------------------------------
#	WriteCPUSectionData
#------------------------------------------------------------------------
sub WriteCPUSectionData( $$$ ) {
	local $FileName					= shift;
	local $File							= shift;
	local $autoNameNumber		= shift;
	local $CodeSectionName	= "\"CC$autoNameNumber\"";
	local $DataSectionName	= "\"CD$autoNameNumber\"";
	local $BSSSectionName		= "\"CB$autoNameNumber\"";
	local $ConstSectionName	= "\"CK$autoNameNumber\"";

	open( FILE, ">$FileName" ) || die "Could not open $FileName for output!\n";

		#open the segment
	print FILE "#pragma code_seg($CodeSectionName)\r\n";
	print FILE "#pragma data_seg($DataSectionName)\r\n";
	print FILE "#pragma bss_seg($BSSSectionName)\r\n";
	print FILE "#pragma const_seg($ConstSectionName)\r\n";

		# Merge all of the sections into one segment (identified by C$autoNameNumber)
		# Note: It is _very_ important to put the data section first, otherwise the
		#       merged section will be read-only!
	print FILE "#pragma comment(linker, \"/merge:CD$autoNameNumber=CPU$autoNameNumber\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:CC$autoNameNumber=CPU$autoNameNumber\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:CB$autoNameNumber=CPU$autoNameNumber\")\r\n";
	print FILE "#pragma comment(linker, \"/merge:CK$autoNameNumber=CPU$autoNameNumber\")\r\n";

		#write the old file data
	print FILE $File;


		#Close the segment
	print FILE "#pragma code_seg()\r\n";
	print FILE "#pragma data_seg()\r\n";
	print FILE "#pragma bss_seg()\r\n";
	print FILE "#pragma const_seg()\r\n";

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
#include <vector>
#include "DebugLogger.h"
extern "C" {
#include "osd_cpu.h"
#include "cpuintrf.h"
}


//= D E F I N E S ======================================================

typedef std::vector<std::string>       dependencyGraph_t;

struct driverDependencyEntry_t {
  const char *m_driverName;
  const char *m_driverDependencySections[64];
};


#define REGISTER_DRIVERSECTION_START( driverName )  { driverName, {
#define DRIVER_DEPENDENCY( sectionName )							sectionName,
#define REGISTER_DRIVERSECTION_END()									NULL } },

//= G L O B A L = V A R S ==============================================
	// Map from MAME CPUID's to section names
static std::map< UINT32, std::string >        g_CPUIDToSectionMap;


//= P R O T O T Y P E S ================================================
extern "C" {
	static void RegisterCPUSectionNames( void );
}


//= F U N C T I O N S ==================================================
extern "C" {

//-------------------------------------------------------------
//	InitDriverSectionizer
//-------------------------------------------------------------
void InitDriverSectionizer( void )
{
  void *addr;
  addr = XLoadSection( "DRVSNIZE" );
  if( !addr )
  {
    UINT32 lastErr = GetLastError();
    PRINTMSG(( T_ERROR, "XLoadSection failed! 0x%X\\n", lastErr ));
  }
}

//-------------------------------------------------------------
//	InitCPUSectionizer
//-------------------------------------------------------------
void InitCPUSectionizer( void )
{
  g_CPUIDToSectionMap.clear();
  void *addr;
  addr = XLoadSection( "CPUSNIZE" );
  if( !addr )
  {
    UINT32 lastErr = GetLastError();
    PRINTMSG(( T_ERROR, "XLoadSection failed! 0x%X\\n", lastErr ));
  }
  RegisterCPUSectionNames();
}


//-------------------------------------------------------------
//	TerminateDriverSectionizer
//-------------------------------------------------------------
void TerminateDriverSectionizer( void )
{
  XFreeSection( "DRVSNIZE" );
}

//-------------------------------------------------------------
//	TerminateCPUSectionizer
//-------------------------------------------------------------
void TerminateCPUSectionizer( void )
{
  g_CPUIDToSectionMap.clear();
  XFreeSection( "CPUSNIZE" );
}


	// Driver section manipulation code
#pragma code_seg("DRVCSNZE")
#pragma data_seg("DRVDSNZE")
#pragma bss_seg("DRVBSNZE")
#pragma const_seg("DRVKSNZE")
#pragma comment(linker, "/merge:DRVDSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVCSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVBSNZE=DRVSNIZE")
#pragma comment(linker, "/merge:DRVKSNZE=DRVSNIZE")
EOF
}




#--------------------------------------------------------------------
#  WriteDriverFileStub
#--------------------------------------------------------------------
sub WriteDriverFileStub() {

	print GENERATEDFILE << "EOF";


#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckDriverSectionRAM
//-------------------------------------------------------------
void CheckDriverSectionRAM( void )
{
  DWORD total = 0;
	for( int i = 0; i < g_numDriverTypeSections; ++i )
	{
    HANDLE h = XGetSectionHandle( g_driverTypeSections[i] );
    if( h != INVALID_HANDLE_VALUE )
    {
      UINT32 sz = XGetSectionSize( h );
      PRINTMSG(( T_INFO, "Driver section %s %lu", g_driverTypeSections[i], sz ));
      total += sz;
    }
    else
      PRINTMSG(( T_ERROR, "Invalid section %s!", g_driverTypeSections[i] ));
	}
  PRINTMSG(( T_INFO, "Total %lu bytes\\n", total ));
}
#endif


//-------------------------------------------------------------
//	LoadDriverSectionByName
//-------------------------------------------------------------
BOOL LoadDriverSectionByName( const char *DriverFileName )
{
	int i = 0;
	for( ; i < g_numDriverDependencyGraphEntries; ++i )
	{
		if( !strcmp( DriverFileName, g_driverDependencyGraph[i].m_driverName ) )
			break;
	}

	if( i == g_numDriverDependencyGraphEntries )
		return FALSE;

  void *addr;
  PRINTMSG(( T_INFO, "Load sections for %s\n", DriverFileName ));


	char const **sectionID = (char const **)g_driverDependencyGraph[i].m_driverDependencySections;
	while( *sectionID )
	{
		PRINTMSG(( T_INFO, "\tID: %s\n", *sectionID ));
		addr = XLoadSection( *sectionID );
		if( !addr )
		{
			UINT32 lastErr = GetLastError();
			PRINTMSG(( T_ERROR, "XLoadSection failed! 0x%X\n", lastErr ));
			//osd_print_error( "Failed to load section %s, dependency of %s!", sectionID, DriverFileName );
			//return FALSE;
		}

		++sectionID;
	}

  return TRUE;
}


//-------------------------------------------------------------
//	UnloadDriverSectionByName
//-------------------------------------------------------------
BOOL UnloadDriverSectionByName( const char *DriverFileName )
{
	int i = 0;
	for( ; i < g_numDriverDependencyGraphEntries; ++i )
	{
		if( !strcmp( DriverFileName, g_driverDependencyGraph[i].m_driverName ) )
			break;
	}

	if( i == g_numDriverDependencyGraphEntries )
		return FALSE;



	BOOL retVal = TRUE;
	const char **sectionID = (const char **)g_driverDependencyGraph[i].m_driverDependencySections;
	while( *sectionID )
	{
		if( !XFreeSection( *sectionID ) )
		{
			retVal = FALSE;
			PRINTMSG(( T_ERROR, "XFreeSection failed for section %s!", *sectionID ));
		}

		++sectionID;
	}

  return retVal;
}


//-------------------------------------------------------------
//	LoadDriverSections
//-------------------------------------------------------------
BOOL LoadDriverSections( void )
{
	for( int i = 0; i < g_numDriverTypeSections; ++i )
	{
		if( !XLoadSection( g_driverTypeSections[i] ) )
		{
			PRINTMSG(( T_ERROR, "Failed to load section %s!", g_driverTypeSections[i] ));
			//return FALSE;
		}
	}

  return TRUE;
}


//-------------------------------------------------------------
//	UnloadDriverSections
//-------------------------------------------------------------
BOOL UnloadDriverSections( void )
{
	for( int i = 0; i < g_numDriverTypeSections; ++i )
		!XFreeSection( g_driverTypeSections[i] );

  return TRUE;
}


EOF
}






#------------------------------------------------------------------------
#	WriteCPUFileStub
#------------------------------------------------------------------------
sub WriteCPUFileStub() {


	print GENERATEDFILE << "EOF";
#pragma code_seg( "CPUCSNZE" )
#pragma data_seg( "CPUDSNZE" )
#pragma comment(linker, "/merge:CPUCSNZE=CPUSNIZE")
#pragma comment(linker, "/merge:CPUDSNZE=CPUSNIZE")

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
    PRINTMSG(( T_ERROR, "XLoadSection failed for section %s! 0x%X\\n", (*i).second.c_str(), lastErr ));
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
#	SectionizeDriverDir
#------------------------------------------------------------------------
sub SectionizeDriverTypeDir( $$ ) {
	local $DirectoryPath = shift;
	local $SectionFiletypeSuffix = shift;
		
	local $DirectorySubPath = $DirectoryPath;
	$DirectorySubPath =~ s/$MAME_DIR\/src\///;

		# Auto generate an 8 character unique name for each file
	local $autoNameNumber = 0;

	# Do two passes, one to find the last autoNameNumber, another to actually
	# modify the files
	print "Pass 1...\n";

	local @NewFiles;
	local @DriverFiles = `find $DirectoryPath -name *.c`;

	PASSONEDRIVERFILENAME:
	foreach $DriverFileName ( @DriverFiles ) {
		chomp( $DriverFileName );

			# Grab the driver name sans path
		$DriverFileName =~ /^$DirectoryPath\/(.*\.c)$/;
		$DriverNoPath = $1;

			# Skip the fake jrcrypt.c file and all the hack files
		foreach( @SkipDrivers ) {
			if( ($DriverNoPath eq $_ ) ) {
				print "Skipping $DriverNoPath.\n";
				next PASSONEDRIVERFILENAME;
			}
		}

			# See if the file has already been sectionized
		local $File;
		($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
		 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

		open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
		sysread( FILE, $File, $size );
		close( FILE );


		if( ($File =~ /\#pragma code_seg/) ) {
			$File =~ /\#pragma code_seg\(\"C(\d+)([D|V|S|M|N|])\"\)/;
			$myAutoNameNumber = $1;
			$FilenameToSectionMap{ "$DirectorySubPath/$DriverNoPath" } = "$1$2";
			push( @DriverTypeFilenames, "$DirectorySubPath/$DriverNoPath" );

			if( $myAutoNameNumber >= $autoNameNumber ) {
				$autoNameNumber = $myAutoNameNumber + 1;
			}		
		} else {
			 push( @NewFiles, $DriverFileName );
		}
	}



		#--- Second pass, write out the section headers --------------------------
	print "Pass 2...\n";
	if( scalar(@NewFiles) == 0 ) {
		print "Nothing to do.\n";
	} else {
		print "Adding XBE headers to ".scalar(@NewFiles)." files...\n";
		PASSTWODRIVERFILENAME:
		foreach $DriverFileName ( @NewFiles ) {
			chomp( $DriverFileName );

				# Grab the driver name sans path
			$DriverFileName =~ /^$DirectoryPath\/(.*\.c)$/;
			$DriverNoPath = $1;

				# Skip the fake jrcrypt.c file and all the hack files
			foreach( @SkipDrivers ) {
				if( ($DriverNoPath eq $_ ) ) {
					print "Skipping $DriverNoPath.\n";
					next PASSTWODRIVERFILENAME;
				}
			}

			($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
			 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

			open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
			sysread( FILE, $File, $size );
			close( FILE );

				# Write out the section header/footer
			WriteDriverTypeSectionData( $DriverFileName, $File, "$autoNameNumber$SectionFiletypeSuffix" );
print "New driver: $DirectorySubPath/$DriverNoPath  => $1$2\n";
			$FilenameToSectionMap{ "$DirectorySubPath/$DriverNoPath" } = "$1$2";
			push( @DriverTypeFilenames, "$DirectorySubPath/$DriverNoPath" );

			$autoNameNumber++;
		}
	}
}








#------------------------------------------------------------------------
#	SectionizeCPUDir
#------------------------------------------------------------------------
sub SectionizeCPUDir() {
	local $autoNameNumber = scalar(@Families) + 50;


	local @CPUFiles    = sort( `find $MAME_CPU_DIR -name *.c` );
	local @NewFiles;
	local $OldCPUName = "";

		# Do two passes, one to find the last autoNameNumber, another to actually
		# modify the files
	print "Pass 1...\n";

	PASSONECPUDRIVERFILENAME:
	foreach $DriverFileName  ( @CPUFiles ) {
		chomp( $DriverFileName );

			# Grab the driver name sans path
		$DriverFileName =~ /^$MAME_SRC_DIR\/(.*\.c)$/;
		$DriverNoPath = $1;

		$CPUName = $DriverFileName;

			# Change the CPUName to what will be present in the actual code
			# Drop the ./MAME/src/cpu portion
		$CPUName =~ /^$MAME_CPU_DIR\/(.+)\/.+\.c$/;
		$CPUName = $1;

			# Skip any junk CPU's
		foreach $CPUToSkip ( @SkipCPUs ) {
			if( uc($CPUName) eq $CPUToSkip ) {
				print "Skipping $CPUName.\n";
				next PASSONECPUDRIVERFILENAME;
			}
		}

		$Family = false;
		$FamilyID = 0;
		foreach( @Families ) {
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

				# Add an entry to the section hash
			$FilenameToSectionMap{ $DriverNoPath } = "C$myAutoNameNumber";

				# Unlike the drivers, we want one name for an entire directory
				# so only register on a new directory (CPUName)
			if( $CPUName ne $OldCPUName ) {
				$OldCPUName = $CPUName;
				$ucaseCPUName = uc( $CPUName );
				if( $Family ne false ) {
					$myAutoNameNumber = 0;	# Don't increment (families should always be less anyway)
					push( @CPUSwitchFileEntries, "C$Family" );
						# Each real CPU is also listed as a clone, so this would be a duplicate
					#print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$Family\" );\n";

						# Register all the clones for this CPU
					$CloneArray = @Clones[$Family-1];
					foreach( @{$CloneArray} ) {
						$CPUIDToSectionNameHash{ "CPU_$_" } = "\"CPU$Family\"";
					}

				}
				else {
					push( @CPUSwitchFileEntries, "C$myAutoNameNumber" );
					$CPUIDToSectionNameHash{ "CPU_$ucaseCPUName" } = "\"CPU$myAutoNameNumber\"";
				}
			}

			if( $myAutoNameNumber >= $autoNameNumber ) {
				$autoNameNumber = $myAutoNameNumber + 1;
			}
		} else {
			push @NewFiles, $DriverFileName;
		}
	}

	#--- Second pass, write out the section headers ------------------
	print "Pass 2...\n";
	if( scalar(@NewFiles) == 0 ) {
		print "Nothing to do.\n";
	}

	$OldCPUName = "";
	PASSTWOCPUDRIVERFILENAME:
	foreach $DriverFileName ( @NewFiles ) {
		chomp( $DriverFileName );
		$CPUName = $DriverFileName;

			# Grab the driver name sans path
		$DriverFileName =~ /^$MAME_SRC_DIR\/(.*\.c)$/;
		$DriverNoPath = $1;

			# Change the CPUName to what will be present in the actual code
			# Drop the ./MAME/src/cpu portion
		$CPUName =~ /^$MAME_CPU_DIR\/(.+)\/.+\.c$/;
		$CPUName = $1;

			# Skip any junk CPU's
		foreach $CPUToSkip ( @SkipCPUs ) {
			$CPUToSkip =~ s/\.c/\\.c/;
			if( ($CPUName =~ /.*$CPUToSkip/ ) ) {
				print "Skipping $CPUName.\n";
				next PASSTWOCPUDRIVERFILENAME;
			}
		}

		$Family = false;
		$FamilyID = 0;
		foreach( @Families ) {
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
				push( @CPUSwitchFileEntries, "C$Family" );
					# Each real CPU is also listed as a clone, so this would be a duplicate
				#print GENERATEDFILE "  RegisterSectionID( CPU_$ucaseCPUName, \"CPU$Family\" );\n";



					# Register all the clones for this CPU
				$CloneArray = @Clones[$Family-1];
				foreach( @{$CloneArray} ) {
					$CPUIDToSectionNameHash{ "CPU_$_" } = "\"CPU$Family\"";
				}
			} else {
				$autoNameNumber++;
				push( @CPUSwitchFileEntries, "C$autoNameNumber" );
				$CPUIDToSectionNameHash{ "CPU_$ucaseCPUName" } = "\"CPU$autoNameNumber\"";
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
				# Add an entry to the section hash
			$FilenameToSectionMap{ $DriverNoPath } = "C$Family";
		} else {
			WriteCPUSectionData( $DriverFileName, $File, $autoNameNumber );
				# Add an entry to the section hash
			$FilenameToSectionMap{ $DriverNoPath } = "C$autoNameNumber";
		}
	}
}

