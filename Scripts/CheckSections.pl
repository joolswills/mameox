#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;

$MAMEoXSrcDir = MAMEoXScriptConstants::MAMEoX_DIR."/sources";

@DriverSectionsFile = `cat $MAMEoXSrcDir/DriverSections.cpp`;


foreach $LineData ( @DriverSectionsFile ) {
	chomp( $LineData );

	if( $LineData =~ /.+RegisterSectionName\( \"(.+)\", \"(\d+)\" \);/ ) {
		$DriverFilename = MAMEoXScriptConstants::MAME_DIR."/".$1;
		$DriverSection = $2;

		$DriverFilename =~ s/\\\\/\//g;

		open( FILE, "<$DriverFilename" ) || die "Failed to open file $DriverFilename!\n";
		sysread( FILE, $File, 1024 );
		close( FILE );
		if( $File =~ /^\#pragma code_seg\(\"C(\d+)\"\)/g ) {
			$SectionID = $1;
			if( $SectionID != $DriverSection ) {
				print "Section ID mismatch in file $DriverFilename. Section is $SectionID, should be $DriverSection!\n";
			}
		}
	}
}

print "Finished checking.\n";