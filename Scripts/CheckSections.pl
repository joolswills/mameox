#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;

$MAMEoXSrcDir = MAMEoXScriptConstants::MAMEoX_DIR."/sources";
@DriverSectionsFile = `cat $MAMEoXSrcDir/XBESectionUtil.cpp`;


foreach $LineData ( @DriverSectionsFile ) {
	chomp( $LineData );

	if( $LineData =~ /.+REGISTER_DRIVERSECTION\( \"(.+)\", \"(\d+)\" \)/ ) {
		$DriverFilename = MAMEoXScriptConstants::MAME_DIR."/".$1;
		$DriverSection = $2;

		$DriverFilename =~ tr/\\\\/\//;

		$MachineFilename = $DriverFilename;
		$MachineFilename =~ s/drivers/machine/;

		$SndhrdwFilename = $DriverFilename;
		$SndhrdwFilename =~ s/drivers/sndhrdw/;

		$VidhrdwFilename = $DriverFilename;
		$VidhrdwFilename =~ s/drivers/vidhrdw/;



		open( FILE, "<$DriverFilename" ) || die "Failed to open file $DriverFilename!\r\n";
		sysread( FILE, $File, -s $DriverFilename );
		close( FILE );
		TestFile( \$File, \$DriverFilename, \$DriverSection );


		if( open( FILE, "<$MachineFilename" ) ) {
		sysread( FILE, $File, -s $DriverFilename );
			close( FILE );
			TestFile( \$File, \$MachineFilename, \$DriverSection );
		}

		if( open( FILE, "<$SndhrdwFilename" ) ) {
		sysread( FILE, $File, -s $DriverFilename );
			close( FILE );
			TestFile( \$File, \$SndhrdwFilename, \$DriverSection );
		}

		if( open( FILE, "<$VidhrdwFilename" ) ) {
		sysread( FILE, $File, -s $DriverFilename );
			close( FILE );
			TestFile( \$File, \$VidhrdwFilename, \$DriverSection );
		}
	}
}


$CPUDir = MAMEoXScriptConstants::MAME_DIR."/src/cpu";
@FilesToCheck = `find $CPUDir -name *.c`;
foreach $FileName ( @FilesToCheck ) {
	open( FILE, "<$FileName" ) || die "Failed to open file $FileName!\r\n";
	sysread( FILE, $File, -s $FileName );
	close( FILE );
	TestCPUFile( \$File, \$FileName );
}

print "Finished checking.\n";






sub TestFile( $, $, $ ) {
	local $File = shift;
	local $DriverFilename = shift;
	local $DriverSection = shift;
	local $SectionID;

	if( $$File =~ /^\#pragma code_seg\(\".?C(\d+)\"\)/g ) {
		$SectionID = $1;
		if( $SectionID != $$DriverSection ) {
			print "Section ID mismatch in file $$DriverFilename. Section is $SectionID, should be $$DriverSection!\r\n";
		}
	}

	if( !defined($$File =~ /^\#pragma code_seg\(\)/g ) ) {
		print "Section $SectionID not terminated in file $$DriverFilename!\r\n";
	}

	if( !defined($$File =~ /^\#pragma comment\(linker, \"\/merge:.?C(\d+)=.*\"\)/g ) ) {
		print "Section not merged in file $$DriverFilename!\r\n";
	} elsif( $1 ne $SectionID ) {
		print "Section $SectionID merged into section $1 in file $$DriverFilename! Is this really desired?\r\n";
	}
}

sub TestCPUFile( $, $ ) {
	local $File = shift;
	local $DriverFilename = shift;

	if( !defined($$File =~ /^\#pragma code_seg\(\".?C(\d+)\"\)/g) ) {
		print "File $$DriverFilename not sectionized!\r\n";
	} else {
		if( !defined($$File =~ /^\#pragma code_seg\(\)/g ) ) {
			print "Section $SectionID not terminated in file $$DriverFilename!\r\n";
		} 

		if( !defined($$File =~ /^\#pragma comment\(linker, \"\/merge:.?C(\d+)=.*\"\)/g ) ) {
			print "Section not merged in file $$DriverFilename!\r\n";
		} elsif( $1 ne $SectionID ) {
			print "Section $SectionID merged into section $1 in file $$DriverFilename! Is this really desired?\r\n";
		}
	}
}
