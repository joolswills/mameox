#!/bin/perl

local @FILEs = `find ./MAME/src/drivers/*.c`;


foreach( @FILEs ) {

	$DriverFile = $_;
	chomp( $DriverFile );
	open( FILE, "<$DriverFile" ) || die "Failed to open file $DriverFileName!\n";
	
	print "--- $DriverFile ---------------------------------------------\n";
	while( <FILE> ) {
		if( $_ =~ /GAME.*\(.*,(.*),.*,.*,.*,.*,.*,.*,.*\"(.*)\",?.*\)/ ) {
			print "$2  [$1]\n";
		}
	}
	close( FILE );
}

