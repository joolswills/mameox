#!/bin/perl

local @FILEs = `find ./MAME/src/drivers/*.c`;


print "Removing sections from drivers, sound hardware, and video hardware...\n";

foreach( @FILEs ) {
	chomp( $_ );

	$DriverFileName = $_;
	$DriverName = $_;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || die "Failed to open file $DriverFileName!\n";
	sysread( FILE, $File, $size );
	close( FILE );

	if( ($File =~ /\#pragma code_seg/) ) {

			# Wipe out the header
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\"[C|B|D|K]\d+\"\)[\r|\n]+//g;
		$File =~ s/\#pragma comment\(linker, \"\/merge:.?\d+=.?\d+\"\)[\r|\n]+//g;

			# Wipe out the footer
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

		open( FILE, ">$DriverFileName" ) || die "Failed to open file $DriverFileName for writing!\n";
		syswrite( FILE, $File, length($File) );
		close( FILE );
	}


		# Also do the vidhdrw file, if one exists
	$VidHardwareName = $DriverFileName;
	$VidHardwareName =~ s/\/drivers\//\/vidhrdw\//;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $VidHardwareName );

	if( open( FILE, "<$VidHardwareName" ) ) {
		$File = "";
		sysread( FILE, $File, $size );
		close( FILE );

		if( ($File =~ /\#pragma code_seg/) ) {

				# Wipe out the header
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\"[C|B|D|K]\d+\"\)[\r|\n]+//g;
			$File =~ s/\#pragma comment\(linker, \"\/merge:.?\d+=.?\d+\"\)[\r|\n]+//g;

				# Wipe out the footer
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

			open( FILE, ">$VidHardwareName" ) || die "Failed to open file $VidHardwareName for writing!\n";
			syswrite( FILE, $File, length($File) );
			close( FILE );
		}
	}

		# Also do the sndhrdw file, if one exists
	$SoundHardwareName = $DriverFileName;
	$SoundHardwareName =~ s/\/drivers\//\/sndhrdw\//;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $SoundHardwareName );

	if( open( FILE, "<$SoundHardwareName" ) ) {
		$File = "";
		sysread( FILE, $File, $size );
		close( FILE );

		if( ($File =~ /\#pragma code_seg/) ) {

				# Wipe out the header
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\"[C|B|D|K]\d+\"\)[\r|\n]+//g;
			$File =~ s/\#pragma comment\(linker, \"\/merge:.?\d+=.?\d+\"\)[\r|\n]+//g;

				# Wipe out the footer
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

			open( FILE, ">$SoundHardwareName" ) || die "Failed to open file $SoundHardwareName for writing!\n";
			syswrite( FILE, $File, length($File) );
			close( FILE );
		}
	}


		# Also do the machine file, if one exists
	$MachineHardwareName = $DriverFileName;
	$MachineHardwareName =~ s/\/drivers\//\/machine\//;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $MachineHardwareName );

	if( open( FILE, "<$MachineHardwareName" ) ) {
		$File = "";
		sysread( FILE, $File, $size );
		close( FILE );

		if( ($File =~ /\#pragma code_seg/) ) {

				# Wipe out the header
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\"[C|B|D|K]\d+\"\)[\r|\n]+//g;
			$File =~ s/\#pragma comment\(linker, \"\/merge:.?\d+=.?\d+\"\)[\r|\n]+//g;

				# Wipe out the footer
			$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

			open( FILE, ">$MachineHardwareName" ) || die "Failed to open file $MachineHardwareName for writing!\n";
			syswrite( FILE, $File, length($File) );
			close( FILE );
		}
	}
}

print "Finished.\n";