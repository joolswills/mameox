#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;

open( OUTFILE, ">".MAMEoXScriptConstants::MAME_DIR."/MAME.vcproj" ) || die "Failed to create MAME.vcproj!\n";

$MAME_DIR = MAMEoXScriptConstants::MAME_DIR;

  # BasePath is prepended automatically during utilization
$BasePath = $MAME_DIR."/src/";

@SkippedDirectories = (  "xml2info",
												 "dbgfonts" );


@ExcludedDirectories = ( "windows",
						             "cpu/gensync" );

@ExcludedFiles = ( 
	"cpu/adsp2100/2100dasm.c",
	"cpu/adsp2100/2100ops.c",
	"cpu/arm/armdasm.c",
	"cpu/asap/asapdasm.c",
	"cpu/ccpu/ccpudasm.c",
	"cpu/ccpu/ccputabl.c",
	"cpu/dsp32/dsp32dis.c",
	"cpu/dsp32/dsp32ops.c",
	"cpu/e132xs/32xsdasm.c",
	"cpu/h6280/6280dasm.c",
	"cpu/h6280/tblh6280.c",
	"cpu/hd6309/6309dasm.c",
	"cpu/hd6309/6309ops.c",
	"cpu/hd6309/6309tbl.c",
	"cpu/i8039/8039dasm.c",
	"cpu/i8085/8085dasm.c",
	"cpu/i86/i286.c",
	"cpu/i86/i86dasm.c",
	"cpu/i86/i86time.c",
	"cpu/i86/instr186.c",
	"cpu/i86/instr286.c",
	"cpu/i86/instr86.c",
	"cpu/i86/instrv30.c",
	"cpu/i8x41/8x41dasm.c",
	"cpu/jaguar/jagdasm.c",
	"cpu/konami/knmidasm.c",
	"cpu/konami/konamops.c",
	"cpu/konami/konamtbl.c",
	"cpu/m6502/6502dasm.c",
	"cpu/m6502/m4510.c",
	"cpu/m6502/m6509.c",
	"cpu/m6502/m65ce02.c",
	"cpu/m6502/t6502.c",
	"cpu/m6502/t6509.c",
	"cpu/m6502/t6510.c",
	"cpu/m6502/t65c02.c",
	"cpu/m6502/t65ce02.c",
	"cpu/m6502/t65sc02.c",
	"cpu/m6502/tdeco16.c",
	"cpu/m6502/tn2a03.c",
	"cpu/m6800/6800dasm.c",
	"cpu/m6800/6800ops.c",
	"cpu/m6800/6800tbl.c",
	"cpu/m68000/asmintf.c",
	"cpu/m68000/m68k_in.c",
	"cpu/m68000/m68kdasm.c",
	"cpu/m68000/m68kmake.c",
	"cpu/m68000/make68k.c",
	"cpu/m6805/6805dasm.c",
	"cpu/m6805/6805ops.c",
	"cpu/m6809/6809dasm.c",
	"cpu/m6809/6809ops.c",
	"cpu/m6809/6809tbl.c",
	"cpu/mips/dismips.c",
	"cpu/mips/mips3dsm.c",
	"cpu/mips/mips3.c",		# Use mips3drc instead of mips3
	"cpu/mips/mipsdasm.c",
	"cpu/mips/r3kdasm.c",
	"cpu/nec/necdasm.c",
	"cpu/s2650/2650dasm.c",
	"cpu/sh2/sh2dasm.c",
	"cpu/t11/t11dasm.c",
	"cpu/t11/t11ops.c",
	"cpu/t11/t11table.c",
	"cpu/tms32010/32010dsm.c",
	"cpu/tms32010/dis32010.c",
	"cpu/tms32025/32025dsm.c",
	"cpu/tms32025/dis32025.c",
	"cpu/tms32031/32031ops.c",
	"cpu/tms32031/dis32031.c",
	"cpu/tms34010/34010dsm.c",
	"cpu/tms34010/34010gfx.c",
	"cpu/tms34010/34010ops.c",
	"cpu/tms34010/34010tbl.c",
	"cpu/tms34010/dis34010.c",
	"cpu/tms9900/9900dasm.c",
	"cpu/upd7810/7810dasm.c",
	"cpu/upd7810/7810ops.c",
	"cpu/upd7810/7810tbl.c",
	"cpu/v60/am.c",
	"cpu/v60/am1.c",
	"cpu/v60/am2.c",
	"cpu/v60/am3.c",
	"cpu/v60/op12.c",
	"cpu/v60/op2.c",
	"cpu/v60/op3.c",
	"cpu/v60/op4.c",
	"cpu/v60/op5.c",
	"cpu/v60/op6.c",
	"cpu/v60/op7a.c",
	"cpu/v60/optable.c",
	"cpu/v60/v60d.c",
	"cpu/v60/v60mem.c",
	"cpu/z180/z180cb.c",
	"cpu/z180/z180dasm.c",
	"cpu/z180/z180dd.c",
	"cpu/z180/z180ed.c",
	"cpu/z180/z180fd.c",
	"cpu/z180/z180op.c",
	"cpu/z180/z180xy.c",
	"cpu/z80/z80dasm.c",
	"cpu/z8000/8000dasm.c",
	"cpu/z8000/makedab.c",
	"cpu/z8000/z8000ops.c",
	"cpu/z8000/z8000tbl.c",
	"cpu/pic16c5x/dis16c5x.c",
	"drivers/jrcrypt.c",
	"drivers/fuuki16.c",
	"drivers/monaco.c",
	"sound/disc_dev.c",
	"sound/disc_flt.c",
	"sound/disc_inp.c",
	"sound/disc_mth.c",
	"sound/disc_out.c",
	"sound/disc_wav.c",
	"sound/scsplfo.c",
	"vidhrdw/jagblit.c",
	"vidhrdw/jagobj.c",
	"vidhrdw/voodblit.c",
	"xml2info/ascii.h",
	"xml2info/asciitab.h",
	"xml2info/expat.h",
	"xml2info/iasciitab.h",
	"xml2info/intconfig.h",
	"xml2info/internal.h",
	"xml2info/latin1tab.h",
	"xml2info/nametab.h",
	"xml2info/utf8tab.h",
	"xml2info/xml2info.c",
	"xml2info/xmlparse.c",
	"xml2info/xmlrole.c",
	"xml2info/xmlrole.h",
	"xml2info/xmltok.c",
	"xml2info/xmltok.h",
	"xml2info/xmltok_impl.c",
	"xml2info/xmltok_impl.h",
	"xml2info/xmltok_ns.c",
	"hdcomp.c",
	"romcmp.c",
	"mameblit.c",
	"chdman.c" );



print OUTFILE <<"EOF";
<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="7.10"
	Name="MAME"
	SccProjectName=""
	SccLocalPath="">
	<Platforms>
		<Platform
			Name="Xbox"/>
	</Platforms>
	<Configurations>
		<Configuration
			Name="Debug|Xbox"
			OutputDirectory="./Debug"
			IntermediateDirectory="./debug_obj"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE">
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				OptimizeForProcessor="2"
				AdditionalIncludeDirectories="src,src/includes,../MAMEoX/Includes,src/cpu/adsp2100,src/cpu/m68000,../,../c_m68000/output,../zlib"
				PreprocessorDefinitions="_XBOX,_DEBUG,INLINE=(static __inline)"
				ExceptionHandling="FALSE"
				BasicRuntimeChecks="3"
				SmallerTypeCheck="FALSE"
				RuntimeLibrary="5"
				BufferSecurityCheck="TRUE"
				EnableFunctionLevelLinking="TRUE"
				ForceConformanceInForLoopScope="TRUE"
				UsePrecompiledHeader="2"
				PrecompiledHeaderFile="./debug_obj/MAME.pch"
				AssemblerListingLocation="./debug_obj/"
				ObjectFile="./debug_obj/"
				ProgramDataBaseFileName="./debug_obj/"
				WarningLevel="3"
				SuppressStartupBanner="TRUE"
				DebugInformationFormat="3"
				CompileAs="0"
				ForcedIncludeFiles="VCPPMame.h"/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="./Debug/MAME.lib"
				SuppressStartupBanner="TRUE"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
		</Configuration>
		<Configuration
			Name="Release|Xbox"
			OutputDirectory="./Release"
			IntermediateDirectory="./release_obj"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			WholeProgramOptimization="TRUE">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="/GL"
				Optimization="4"
				GlobalOptimizations="TRUE"
				InlineFunctionExpansion="1"
				EnableIntrinsicFunctions="TRUE"
				FavorSizeOrSpeed="1"
				OmitFramePointers="TRUE"
				EnableFiberSafeOptimizations="TRUE"
				OptimizeForProcessor="2"
				AdditionalIncludeDirectories="src,src/includes,../MAMEoX/Includes,src/cpu/adsp2100,src/cpu/m68000,../,../c_m68000/output,../zlib"
				PreprocessorDefinitions="WIN32,_XBOX,NDEBUG"
				StringPooling="FALSE"
				ExceptionHandling="FALSE"
				RuntimeLibrary="4"
				BufferSecurityCheck="FALSE"
				EnableFunctionLevelLinking="FALSE"
				EnableEnhancedInstructionSet="1"
				ForceConformanceInForLoopScope="TRUE"
				PrecompiledHeaderFile="./release_obj/MAME.pch"
				AssemblerListingLocation="./release_obj/"
				ObjectFile="./release_obj/"
				ProgramDataBaseFileName="./release_obj/"
				WarningLevel="3"
				SuppressStartupBanner="TRUE"
				CallingConvention="0"
				CompileAs="0"
				ForcedIncludeFiles="VCPPMame.h"/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCLibrarianTool"
				AdditionalOptions="/LTCG"
				OutputFile="./Release/MAME.lib"
				SuppressStartupBanner="TRUE"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
		</Configuration>
		<Configuration
			Name="Profile|Xbox"
			OutputDirectory="Profile"
			IntermediateDirectory="profile_obj"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE">
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				OptimizeForProcessor="2"
				AdditionalIncludeDirectories="src,src/includes,../MAMEoX/Includes,src/cpu/adsp2100,src/cpu/m68000,../,../c_m68000/output,../zlib"
				PreprocessorDefinitions="_XBOX,_DEBUG,INLINE=(static __inline)"
				MinimalRebuild="FALSE"
				RuntimeLibrary="1"
				EnableFunctionLevelLinking="TRUE"
				UsePrecompiledHeader="0"
				PrecompiledHeaderFile="./profile_obj/MAME.pch"
				AssemblerListingLocation="./profile_obj/"
				ObjectFile="./profile_obj/"
				ProgramDataBaseFileName="./profile_obj/"
				WarningLevel="3"
				SuppressStartupBanner="TRUE"
				DebugInformationFormat="3"
				CompileAs="0"
				ForcedIncludeFiles="VCPPMame.h"
				FastCAP="TRUE"/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="./Profile/MAME.lib"
				SuppressStartupBanner="TRUE"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
		</Configuration>
	</Configurations>
	<References>
	</References>
	<Files>
EOF


ParseDir( MAMEoXScriptConstants::MAME_DIR."/src", "\t\t" );

print OUTFILE <<"EOF";
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
EOF

close( OUTFILE );






#------------------------------------------------------------
#
#------------------------------------------------------------
sub ParseDir( $$ ) {
	local $curDir = shift;
	local $tabStr = shift;
	local $DIRHANDLE;
	print "ParseDir: $curDir\n";
	opendir( $DIRHANDLE, "$curDir" ) || die "Invalid directory $curDir!\n";
	local @dirent = readdir( $DIRHANDLE );

	$curDir =~ /(.*[\\\/])*(.+)$/;
	local $curDirNoPath = $2;
	$curDirNoPath =~ tr/\//\\/;
	print "$curDirNoPath \n";
	print OUTFILE "$tabStr<Filter\n$tabStr\tName=\"$curDirNoPath\"\n$tabStr\tFilter=\"\">\n";

	local @Dirs = ();
	local @Files = ();


	foreach( @dirent ) {
		$curItem = $_;
		next if( substr( $curItem, 0, 1 ) eq "." );
		next if( lc($curItem) eq "cvs" );
		next if( $curItem =~ /.*\.o$/ );

		$fullPath = $curDir."/".$curItem;
		if( -d $fullPath ) {
			push( @Dirs, $fullPath );
		} else {
			push( @Files, $fullPath );
		}
	}
	closedir( $DIRHANDLE );

	@Dirs = sort( @Dirs );
	@Files = sort( @Files );

	foreach( @Dirs ) {
		print "Dir: $_\n";
		ParseDir( $_, $tabStr."\t" );
	}

	FILEPARSER:
	foreach $fileName ( @Files ) {
		print " -\t$fileName\n";

			# Set the "ExcludedFromBuild" property
		$excludeFile = "FALSE";
		foreach $ExcludedFile ( @ExcludedFiles ) {			 
			if( $BasePath.$ExcludedFile eq $fileName ) {
				$excludeFile = "TRUE";
				last;
			}
		}

			# Special: Skip anything in the skip directory array completely
		foreach $Directory ( @SkippedDirectories ) {		
			if( $fileName =~ /$BasePath$Directory[\/]?.*/ ) {
				next FILEPARSER;
			}
		}

			# Special: Don't build anything in the listed directories
		foreach $Directory ( @ExcludedDirectories ) {		
			if( $fileName =~ /$BasePath$Directory[\/]?.*/ ) {
				$excludeFile = "TRUE";
				last;
			}
		}


			# Skip anything that can't be compiled
		if( !($fileName =~ /.*\.[h|c|cpp|asm]$/) ) {
			next FILEPARSER;
		}		

		$isHeader = false;
		if( $excludeFile eq "FALSE" ) {
			if( $fileName =~ /.*\.h$/ ) {
				$isHeader = true;
			} 
		}

			# Cut off the path from the script directory to the MAME dir
		$fileName =~ s/^$BasePath/src\//;
		
			# Match 
			#   src/(whatever)/throwaway
		if( $fileName =~ /src\/(.*)\/[^\/]*$/ ) {
			$objectSubPath = "$1";
		} else {
			$objectSubPath = "";
		}

			# Change /'s to \'s
		$fileName =~ tr/\//\\/;

		print OUTFILE "$tabStr\t<File\n";
		print OUTFILE "$tabStr\t\tRelativePath=\"$fileName\">\n";

		if( $isHeader eq false ) {
				# Debug
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Debug|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./debug_obj/$objectSubPath/\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";

				# Release
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Release|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./release_obj/$objectSubPath/\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";

				# Profile
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Profile|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./profile_obj/$objectSubPath/\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";
		}

		print OUTFILE "$tabStr\t</File>\n";
	}

	print OUTFILE "$tabStr</Filter>\n";
}


