#!/bin/perl


@DriverSectionsFile = `cat ./MAMEoX/sources/DriverSections.cpp`;

foreach( @DriverSectionsFile ) {
	chomp( $_ );
	$LineData = $_;

	if( $LineData =~ /.+RegisterSectionName\( \"(.+)\", \"(\d+)\" \);/ ) {
		$DriverFilename = "./MAME/".$1;
		$DriverSection = $2;

		$DriverFilename =~ s/\\\\/\//g;

#		print "$DriverFilename =? $DriverSection\n";

		open( FILE, "<$DriverFilename" ) || die "Failed to open file $DriverFilename!\n";
		sysread( FILE, $File, 1024 );
		close( FILE );
		if( $File =~ /^\#pragma code_seg\(\"C(\d+)\"\)/g ) {
			$SectionID = $1;
			if( $SectionID != $DriverSection ) {
				print "Section ID mismatch in file $DriverFilename. Section is $SectionID, should be $DriverSection!\n";
			}
#			print "$SectionID =? $DriverSection\n";
		}
	}
}

print "Finished checking.\n";