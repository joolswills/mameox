#!/bin/perl
if( @ARGV ) {
	foreach( @ARGV ) {
		$FileName = $_;
		$DriverFileName = $_;
		
			# Add .c if it's not there
		if( !( $DriverFileName =~ /(.*)\.c$/ ) ) {
			$DriverFileName .= ".c";
		}

		print "Attempting to remove sections from a driver called $DriverFileName\n";

		if( RemoveDriverSection( "./MAME/src/drivers/".$DriverFileName ) eq false ) {
			print "Looks like it's not a driver, trying to find a CPU directory named $FileName\n";

				# Try again as a CPU
			local @FILES = `find ./MAME/src/cpu/$FileName/*.c`;
			if( scalar( @FILES ) != 0 ) {
				foreach( @FILES ) {
					chomp;
					if( RemoveCPUSection( $_ ) eq false ) {
						print "Failed removing sections from file $_!\n";
					}
					else {
						print "Removed section from file $_\n";
					}
				}
				print "Done checking the c files within directory ./MAME/src/cpu/$FileName!\n";
			} else {
				print "Can't do anything with $FileName. It's not a driver or a CPU directory.\n";
			}
		} else {
			print "Section removed!\n";
		}

	}
}
else {
	local @FILEs = `find ./MAME/src/drivers/*.c`;

	print "Removing sections from drivers, sound hardware, and video hardware...\n";

	foreach( @FILEs ) {
		chomp( $_ );
		RemoveDriverSection( $_ );
	}

	#------------------------------------------------------------------------------


	print "Removing sections from CPU's...\n";

	@FILEs    = `find ./MAME/src/cpu/ -name *.c`;

	foreach( @FILEs ) {
		chomp( $_ );

		$DriverFileName = $_;
		RemoveCPUSection( $DriverFileName );
	}

	print "Done!\n";
}


#------------------------------------------------------------------------------
# RemoveDriverSection
#
# \param   Name (and path) of the driver file to desectionize
#------------------------------------------------------------------------------
sub RemoveDriverSection( $ ) {
	$DriverFileName = shift;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || return false;
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

	return true;
}


#------------------------------------------------------------------------------
# RemoveCPUSection
#
# \param   Path to the CPU directory to desectionize
#------------------------------------------------------------------------------
sub RemoveCPUSection( $ ) {

	$DriverFileName = shift;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || return false;
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

	return true;
}
