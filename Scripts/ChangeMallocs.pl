#!/bin/perl


require 5.0.0.0;
use MAMEoXScriptConstants;


print "Searching for source files...\n";

$SrcDirectory = MAMEoXScriptConstants::MAME_DIR."/src";
local @SourceFiles = `find $SrcDirectory -name *.c`;


print "Converting all stock *alloc() calls to osd_*alloc (".scalar(@SourceFiles)." files)\n";

foreach $FileName ( @SourceFiles ) {
	chomp( $FileName );

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
