#!/bin/perl

# Auto generate an 8 character unique name for each file
local $autoNameNumber = 0;
local @newFILEs;
local @FILEs = `find ./MAME/src/*.c`;


print "Converting all stock malloc() calls to osd_malloc...\n";

foreach( @FILEs ) {
	chomp( $_ );

	$FileName = $_;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $FileName );

	open( FILE, "<$FileName" ) || die "Failed to open file $FileName!\n";
	sysread( FILE, $File, $size );
	close( FILE );

	if( $File =~ /(\s+|\(.+\*\))(m|c|re)alloc\s*\(/ ) {
		$File =~ s/(\s+|\(.+\*\))((m|c|re)alloc\s*)\(/$1osd_$2\(/g;

		open( FILE, ">$FileName" ) || die "Failed to open $FileName for writing!\n";
		syswrite( FILE, $File, length($File) );
		close( FILE );
	}
}

print "Done!\n";
