#!/bin/perl

open( OUTFILE, ">MAME.vcproj" ) || die "Failed to create MAME.vcproj!\n";


@ExcludedDirectories = ( "./src/windows",
						 "./src/dbgfonts" );

@ExcludedFiles = ( 
	"./src/cpu/adsp2100/2100dasm.c",
	"./src/cpu/adsp2100/2100ops.c",
	"./src/cpu/arm/armdasm.c",
	"./src/cpu/asap/asapdasm.c",
	"./src/cpu/ccpu/ccpudasm.c",
	"./src/cpu/ccpu/ccputabl.c",
	"./src/cpu/dsp32/dsp32dis.c",
	"./src/cpu/dsp32/dsp32ops.c",
	"./src/cpu/h6280/6280dasm.c",
	"./src/cpu/h6280/tblh6280.c",
	"./src/cpu/hd6309/6309dasm.c",
	"./src/cpu/hd6309/6309ops.c",
	"./src/cpu/hd6309/6309tbl.c",
	"./src/cpu/i8039/8039dasm.c",
	"./src/cpu/i8085/8085dasm.c",
	"./src/cpu/i86/i286.c",
	"./src/cpu/i86/i86dasm.c",
	"./src/cpu/i86/i86time.c",
	"./src/cpu/i86/instr186.c",
	"./src/cpu/i86/instr286.c",
	"./src/cpu/i86/instr86.c",
	"./src/cpu/i86/instrv30.c",
	"./src/cpu/i8x41/8x41dasm.c",
	"./src/cpu/jaguar/jagdasm.c",
	"./src/cpu/konami/knmidasm.c",
	"./src/cpu/konami/konamops.c",
	"./src/cpu/konami/konamtbl.c",
	"./src/cpu/m6502/6502dasm.c",
	"./src/cpu/m6502/m4510.c",
	"./src/cpu/m6502/m6509.c",
	"./src/cpu/m6502/m65ce02.c",
	"./src/cpu/m6502/t6502.c",
	"./src/cpu/m6502/t6509.c",
	"./src/cpu/m6502/t6510.c",
	"./src/cpu/m6502/t65c02.c",
	"./src/cpu/m6502/t65ce02.c",
	"./src/cpu/m6502/t65sc02.c",
	"./src/cpu/m6502/tdeco16.c",
	"./src/cpu/m6502/tn2a03.c",
	"./src/cpu/m6800/6800dasm.c",
	"./src/cpu/m6800/6800ops.c",
	"./src/cpu/m6800/6800tbl.c",
	"./src/cpu/m68000/asmintf.c",
	"./src/cpu/m68000/m68k_in.c",
	"./src/cpu/m68000/m68kdasm.c",
	"./src/cpu/m68000/m68kmake.c",
	"./src/cpu/m68000/make68k.c",
	"./src/cpu/m6805/6805dasm.c",
	"./src/cpu/m6805/6805ops.c",
	"./src/cpu/m6809/6809dasm.c",
	"./src/cpu/m6809/6809ops.c",
	"./src/cpu/m6809/6809tbl.c",
	"./src/cpu/mips/dismips.c",
	"./src/cpu/mips/mips3dsm.c",
	"./src/cpu/mips/mips3.c",		# Use mips3drc instead of mips3
	"./src/cpu/mips/mipsdasm.c",
	"./src/cpu/mips/r3kdasm.c",
	"./src/cpu/nec/necdasm.c",
	"./src/cpu/s2650/2650dasm.c",
	"./src/cpu/sh2/sh2dasm.c",
	"./src/cpu/t11/t11dasm.c",
	"./src/cpu/t11/t11ops.c",
	"./src/cpu/t11/t11table.c",
	"./src/cpu/tms32010/32010dsm.c",
	"./src/cpu/tms32010/dis32010.c",
	"./src/cpu/tms32025/32025dsm.c",
	"./src/cpu/tms32025/dis32025.c",
	"./src/cpu/tms32031/32031ops.c",
	"./src/cpu/tms32031/dis32031.c",
	"./src/cpu/tms34010/34010dsm.c",
	"./src/cpu/tms34010/34010gfx.c",
	"./src/cpu/tms34010/34010ops.c",
	"./src/cpu/tms34010/34010tbl.c",
	"./src/cpu/tms34010/dis34010.c",
	"./src/cpu/tms9900/9900dasm.c",
	"./src/cpu/upd7810/7810dasm.c",
	"./src/cpu/upd7810/7810ops.c",
	"./src/cpu/upd7810/7810tbl.c",
	"./src/cpu/v60/am.c",
	"./src/cpu/v60/am1.c",
	"./src/cpu/v60/am2.c",
	"./src/cpu/v60/am3.c",
	"./src/cpu/v60/op12.c",
	"./src/cpu/v60/op2.c",
	"./src/cpu/v60/op3.c",
	"./src/cpu/v60/op4.c",
	"./src/cpu/v60/op5.c",
	"./src/cpu/v60/op6.c",
	"./src/cpu/v60/op7a.c",
	"./src/cpu/v60/optable.c",
	"./src/cpu/v60/v60d.c",
	"./src/cpu/v60/v60mem.c",
	"./src/cpu/z180/z180cb.c",
	"./src/cpu/z180/z180dasm.c",
	"./src/cpu/z180/z180dd.c",
	"./src/cpu/z180/z180ed.c",
	"./src/cpu/z180/z180fd.c",
	"./src/cpu/z180/z180op.c",
	"./src/cpu/z180/z180xy.c",
	"./src/cpu/z80/z80dasm.c",
	"./src/cpu/z8000/8000dasm.c",
	"./src/cpu/z8000/makedab.c",
	"./src/cpu/z8000/z8000ops.c",
	"./src/cpu/z8000/z8000tbl.c",
	"./src/cpu/pic16c5x/dis16c5x.c",
	"./src/drivers/jrcrypt.c",
	"./src/sound/disc_dev.c",
	"./src/sound/disc_flt.c",
	"./src/sound/disc_inp.c",
	"./src/sound/disc_mth.c",
	"./src/sound/disc_out.c",
	"./src/sound/disc_wav.c",
	"./src/vidhrdw/jagblit.c",
	"./src/vidhrdw/jagobj.c",
	"./src/xml2info/ascii.h",
	"./src/xml2info/asciitab.h",
	"./src/xml2info/expat.h",
	"./src/xml2info/iasciitab.h",
	"./src/xml2info/intconfig.h",
	"./src/xml2info/internal.h",
	"./src/xml2info/latin1tab.h",
	"./src/xml2info/nametab.h",
	"./src/xml2info/utf8tab.h",
	"./src/xml2info/xml2info.c",
	"./src/xml2info/xmlparse.c",
	"./src/xml2info/xmlrole.c",
	"./src/xml2info/xmlrole.h",
	"./src/xml2info/xmltok.c",
	"./src/xml2info/xmltok.h",
	"./src/xml2info/xmltok_impl.c",
	"./src/xml2info/xmltok_impl.h",
	"./src/xml2info/xmltok_ns.c",
	"./src/hdcomp.c",
	"./src/romcmp.c",
	"./src/mameblit.c" );

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


ParseDir( "./src", "\t\t" );

print OUTFILE <<"EOF";
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
EOF

close( OUTFILE );







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

	foreach( @Files ) {
		print "File: $_\n";
		$fileName = $_;
		if( $fileName =~ /\.[\\\/]src[\\\/](.*)[\/\\][^\/\\]*$/ ) {
			$fileWithoutExtension = "$1/";
		} else {
			$fileWithoutExtension = "/";
		}

			# Set the "ExcludedFromBuild" property
		$excludeFile = "FALSE";
		foreach( @ExcludedFiles ) {
			if( $_ eq $fileName ) {
				$excludeFile = "TRUE";
				last;
			}
		}

			# Special: Don't build anything in the listed directories
		foreach( @ExcludedDirectories ) {
			$dirToTest = $_;
			if( $fileName =~ /$dirToTest[\/]?.*/ ) {
				$excludeFile = "TRUE";
			}
		}


			# Skip anything that can't be compiled
		$isHeader = false;
		if( $excludeFile eq "FALSE" ) {
			if( $fileName =~ /.*\.h$/ ) {
				$isHeader = true;
			} elsif( !($fileName =~ /.*\.[c|cpp]$/) ) {
				next;
			}
		}

		$fileName =~ tr/\//\\/;
		$fileWithoutExtension =~ tr/\//\\/;

		print OUTFILE "$tabStr\t<File\n";
		print OUTFILE "$tabStr\t\tRelativePath=\"$fileName\">\n";

		if( $isHeader eq false ) {
				# Debug
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Debug|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./debug_obj/$fileWithoutExtension\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";

				# Release
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Release|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./release_obj/$fileWithoutExtension\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";

				# Profile
			print OUTFILE "$tabStr\t\t<FileConfiguration\n";
			print OUTFILE "$tabStr\t\t\tName=\"Profile|Xbox\"\n";
			print OUTFILE "$tabStr\t\t\tExcludedFromBuild=\"$excludeFile\">\n";
			print OUTFILE "$tabStr\t\t\t<Tool\n";
			print OUTFILE "$tabStr\t\t\t\tName=\"VCCLCompilerTool\"\n";
			print OUTFILE "$tabStr\t\t\t\tObjectFile=\"./profile_obj/$fileWithoutExtension\"/>\n";
			print OUTFILE "$tabStr\t\t</FileConfiguration>\n";
		}

		print OUTFILE "$tabStr\t</File>\n";
	}

	print OUTFILE "$tabStr</Filter>\n";
}


