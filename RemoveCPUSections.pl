#!/bin/perl

#------------------------------------------------------------------------------


print "Removing sections from CPU's...\n";

local @FILEs    = `find ./MAME/src/cpu/ -name *.c`;

foreach( @FILEs ) {
	chomp( $_ );

	$DriverFileName = $_;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
	sysread( FILE, $File, $size );
	close( FILE );

	if( ($File =~ /\#pragma code_seg/) ) {


			# Wipe out the header
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\"C[C|B|D|K]\d+\"\)[\r|\n]+//g;
		$File =~ s/\#pragma comment\(linker, \"\/merge:C[C|B|D|K]\d+=CPU\d+\"\)[\r|\n]+//g;

			# Wipe out the footer
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

		open( FILE, ">$DriverFileName" ) || die "Failed to open file $DriverFileName for writing!\n";
		syswrite( FILE, $File, length($File) );
		close( FILE );
	}
}



print "Finished.\n";