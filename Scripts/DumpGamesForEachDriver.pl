#!/bin/perl


require 5.0.0.0;
use MAMEoXScriptConstants;

$DRIVER_DIR = MAMEoXScriptConstants::MAME_DIR."/src/drivers";

local @FILEs = `find $DRIVER_DIR/*.c`;
foreach $DriverFile ( @FILEs ) {
	chomp( $DriverFile );
	open( FILE, "<$DriverFile" ) || die "Failed to open file $DriverFileName!\n";
	
	$DriverFile =~ s/$DRIVER_DIR\///;
	print "--- $DriverFile ---------------------------------------------\n";
	while( <FILE> ) {
		if( $_ =~ /GAME.*\(.*,(.*),.*,.*,.*,.*,.*,.*,.*\"(.*)\",?.*\)/ ) {
			print "$2  [$1]\n";
		}
	}
	close( FILE );
	print "\n\n";
}
