#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;

$DRIVER_DIR		= MAMEoXScriptConstants::MAME_DIR."/src/drivers";
$MACHINE_DIR	= MAMEoXScriptConstants::MAME_DIR."/src/machine";
$SOUND_DIR		= MAMEoXScriptConstants::MAME_DIR."/src/sound";
$SNDHRDW_DIR	= MAMEoXScriptConstants::MAME_DIR."/src/sndhrdw";
$VIDHRDW_DIR	= MAMEoXScriptConstants::MAME_DIR."/src/vidhrdw";
$CPU_DIR			= MAMEoXScriptConstants::MAME_DIR."/src/cpu";




if( @ARGV ) {
	foreach $FileName ( @ARGV ) {
		local $DriverFileName = $FileName;
		
			# Add .c if it's not there
		if( !( $DriverFileName =~ /(.*)\.c$/ ) ) {
			$DriverFileName .= ".c";
		}

		$ReturnMessage = "Can't do anything with $FileName. It's not a driver or a CPU directory.\n";


		print "Attempting to remove sections from a driver file called $DriverFileName\n";
		if( RemoveDriverSection( $DRIVER_DIR."/".$DriverFileName ) eq true ) {
			$ReturnMessage = "Section(s) removed!\n";
		}

		print "Attempting to remove sections from a machine file called $DriverFileName\n";
		if( RemoveDriverSection( $MACHINE_DIR."/".$DriverFileName ) eq true ) {
			$ReturnMessage = "Section(s) removed!\n";
		}

		print "Attempting to remove sections from a sound file called $DriverFileName\n";
		if( RemoveDriverSection( $SOUND_DIR."/".$DriverFileName ) eq true ) {
			$ReturnMessage = "Section(s) removed!\n";
		}

		print "Attempting to remove sections from a sndhrdw file called $DriverFileName\n";
		if( RemoveDriverSection( $SNDHRDW_DIR."/".$DriverFileName ) eq true ) {
			$ReturnMessage = "Section(s) removed!\n";
		}

		print "Attempting to remove sections from a vidhrdw file called $DriverFileName\n";
		if( RemoveDriverSection( $VIDHRDW_DIR."/".$DriverFileName ) eq true ) {
			$ReturnMessage = "Section(s) removed!\n";
		}

		if( $ReturnMessage eq "Can't do anything with $FileName. It's not a driver or a CPU directory.\n" ) {
			print "Looks like it's not a driver, trying to find a CPU directory named $FileName\n";

				# Try again as a CPU
			local @FILES = `find $CPU_DIR/$FileName -name *.c`;
			if( scalar(@FILES) != 0 ) {
				foreach( @FILES ) {
					chomp;
					if( RemoveCPUSection( $_ ) eq false ) {
						print "Failed to remove section from file $_!\n";
					}
					else {
						print "Removed section from file $_\n";
						$ReturnMessage = "CPU section(s) removed!\n";
					}
				}
				print "Done checking the c files within directory $CPU_DIR/$FileName!\n";
			}
		}

		print $ReturnMessage;
	}
}
else {
	print "Removing sections from driver dir...\n";
	local @FILEs = `find $DRIVER_DIR -name *.c`;
	foreach $FileName ( @FILEs ) {
		chomp( $FileName );
		RemoveDriverSection( $FileName );
	}

	#------------------------------------------------------------------------------

	print "Removing sections from machine dir...\n";
	local @FILEs = `find $MACHINE_DIR -name *.c`;
	foreach $FileName ( @FILEs ) {
		chomp( $FileName );
		RemoveDriverSection( $FileName );
	}

	#------------------------------------------------------------------------------

	print "Removing sections from sound dir...\n";
	local @FILEs = `find $SOUND_DIR -name *.c`;
	foreach $FileName ( @FILEs ) {
		chomp( $FileName );
		RemoveDriverSection( $FileName );
	}

	#------------------------------------------------------------------------------

	print "Removing sections from sndhrdw dir...\n";
	local @FILEs = `find $SNDHRDW_DIR -name *.c`;
	foreach $FileName ( @FILEs ) {
		chomp( $FileName );
		RemoveDriverSection( $FileName );
	}

	#------------------------------------------------------------------------------

	print "Removing sections from vidhrdw dir...\n";
	local @FILEs = `find $VIDHRDW_DIR -name *.c`;
	foreach $FileName ( @FILEs ) {
		chomp( $FileName );
		RemoveDriverSection( $FileName );
	}

	#------------------------------------------------------------------------------


	print "Removing sections from CPU's...\n";

	@FILEs    = `find $CPU_DIR -name *.c`;

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
	local $DriverFileName = shift;

	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	 $atime,$mtime,$ctime,$blksize,$blocks) = stat( $DriverFileName );

	open( FILE, "<$DriverFileName" ) || return false;
	sysread( FILE, $File, $size );
	close( FILE );

	if( ($File =~ /\#pragma code_seg/) ) {

			# Wipe out the headers

			# Match   #pragma code_seg("C714D")
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\"[C|B|D|K].?\d+[D|V|S|M]?\"\)[\r|\n]+//g;

			# Match   #pragma comment(linker, "/merge:D714D=714D")
		$File =~ s/\#pragma comment\(linker, \"\/merge:.?.?\d+[D|V|S|M]?=.?.?\d+[D|V|S|M]?\"\)[\r|\n]+//g;

			# Wipe out the footer
		$File =~ s/\#pragma (code|data|bss|const)_seg\(\)[\r|\n]+//g;

		open( FILE, ">$DriverFileName" ) || die "Failed to open file $DriverFileName for writing!\n";
		syswrite( FILE, $File, length($File) );
		close( FILE );
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
