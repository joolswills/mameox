#!/bin/perl

require 5.0.0.0;
use MAMEoXScriptConstants;


$ROOT_DIR		= MAMEoXScriptConstants::ROOT_DIR;
$MAMEoX_DIR = MAMEoXScriptConstants::MAMEoX_DIR;
$MAME_DIR   = MAMEoXScriptConstants::MAME_DIR;
$MAME_SRC_DIR			= $MAME_DIR."/src";
$MAME_CPU_DIR			= $MAME_SRC_DIR."/cpu";
$MAME_DRIVER_DIR	= $MAME_SRC_DIR."/drivers";
$MAME_MACHINE_DIR = $MAME_SRC_DIR."/machine";
$MAME_SOUND_DIR		= $MAME_SRC_DIR."/sound";
$MAME_SNDHRDW_DIR = $MAME_SRC_DIR."/sndhrdw";
$MAME_VIDHRDW_DIR = $MAME_SRC_DIR."/vidhrdw";


%DependencyHash;
%FilesDependentHash;
RebuildDependencyHash( "Dependencies.txt" ) || die( "Failed to rebuild dependency hash. Run FindDependencies.pl!\n" );


 # Remove anything that won't be sectionized by OldSectionize.pl
foreach $Key ( keys( %DependencyHash ) ) {

		# Make sure the Key is either a CPU, Driver, or there's a driver of
		# the same name
	if( $Key =~ /[machine|sndhrdw|vidhrdw]\/(.*)/ ) {
		if( !defined( $DependencyHash{"drivers/$1"} ) ) {
			#print "Removing $Key\n";
			undef( $DependencyHash{$Key} );
			undef( $FilesDependentHash{$Key} );
		}		
	}
}


local @Families;


#print "\r\n\r\nFiles Dependent\r\n";
foreach $Key ( sort( keys( %DependencyHash ) ) ) {
	if( $Key =~ /drivers\/(.*)/ ) {
		local $BaseFile = $1;
		local $ArrayRef = $DependencyHash{ $Key };
		local @InterestingDeps;

		foreach $Dependent ( @$ArrayRef ) {
			  # Don't bother printing the obvious matches
			if( $Dependent =~ /[machine|sndhrdw|vidhrdw]\/$BaseFile/ ) {
				next;
			}

			if( $Dependent =~ /cpu\/.*/ ) {
					# Add all CPU dependencies
				push( @InterestingDeps, $Dependent );
			} else {
				  # Only add the driver name as a dependency once
				$Dependent =~ /[machine|sndhrdw|vidhrdw]\/(.*)/;
				if( DoesItemExist( $1, \@InterestingDeps ) eq true || !defined($DependencyHash{"drivers/$1"})) {
					next;
				}
				push( @InterestingDeps, $1 );
			}
		}

		if( scalar( @InterestingDeps ) > 0 ) {
			#print "\r\n* $Key\r\n";
			#foreach $Dependent ( sort( @InterestingDeps ) ) {
			#	print ">  $Dependent\r\n";
			#}

			$Key =~ /drivers\/(.*)/;
			push( @InterestingDeps, $1 );
			push( @Families, CopyArray( \@InterestingDeps ) );
		}
	}
}


#PrintFamilies( \@Families );

	# Concatenate the families
  # This shit is crazy inefficient, but I am feeling very
	# lazy and extremely tired today, so forgive me
@NewFamilies;
for ( $i = 0; $i < scalar( @Families ) ; $i++ ) {
	if( !defined( $Families[$i] ) ) {
		next;
	}

	local $DepArrayRef = $Families[$i];
	local @FullDependencyArray = @$DepArrayRef;



	for ( $j = $i + 1; $j < scalar( @Families ) ; $j++ ) {
		if( !defined( $Families[$j] ) ) {
			next;
		}

		$DepArrayRefJ = $Families[$j];
		foreach $Dependency ( @$DepArrayRefJ ) {
			if( DoesItemExist( $Dependency, \@FullDependencyArray ) eq true ) {

					# Add all the items in this dependency array to the fulldependencyarray
					# Remove the items in this dependency array from the Families array
					# Restart scanning at $i + 1
				foreach $DependencyToAdd ( @$DepArrayRefJ ) {
					if( DoesItemExist( $DependencyToAdd, \@FullDependencyArray ) ne true ) {
						push( @FullDependencyArray, $DependencyToAdd );
					}
				}
				$Families[$j] = undef;
				$j = $i;
				last;
			}
		}
	}

	push( @NewFamilies, CopyArray( \@FullDependencyArray ) );
}


PrintFamilies( \@NewFamilies );






#== F U N C T I O N S ===================================================

sub PrintFamilies( $ ) {
	local $FamilyArrayRef = shift;
	local $IsFirst;
	local $i;

	print "\r\n\r\nFamilies\r\n";
	for ( $i = 0; $i < scalar( @$FamilyArrayRef ); $i++ ) {
		local @CPUDependencyHack = ();

		print "local \@Family_$i = ( ";
		$IsFirst = true;

		$DepArrayRef = @{$FamilyArrayRef}[$i];
		foreach $Dependency ( @$DepArrayRef ) {
			if( $Dependency =~ /cpu\/.*/ ) {
				push( @CPUDependencyHack, $Dependency );
			} else {
				if( $IsFirst ne true ) {
					print ", ";
				}
				print "\"$Dependency\"";
				$IsFirst = false;
			}
		}

		print " );";

		if( scalar( @CPUDependencyHack ) > 0 ) {
			print "  # CPU deps: ";
			foreach $CPUDependency ( @CPUDependencyHack ) {
				print "\"$CPUDependency\" ";
			}
		}

		print "\r\n";
	}


	print "\r\n\r\n";
	print "local \@DriverFamilies = ( ";
	$IsFirst = true;
	for ( $i = 0; $i < scalar( @$FamilyArrayRef ); $i++ ) {
		if( $IsFirst ne true ) {
			print ", ";
		}
		print "\\\@Family_$i";
		$IsFirst = false;
	}
	print " );\r\n";

	print "\r\nDone\r\n";
}


#--------------------------------------------------------------------
#
#--------------------------------------------------------------------
sub DoesItemExist( $, $ ) {
	local( $Item = shift );
	local( $ArrayRef = shift );

	foreach $i ( @$ArrayRef ) {
		if( $i eq $Item ) {
			return true;
		}
	}

	return false;
}



#------------------------------------------------------------------------
# CopyArray
#------------------------------------------------------------------------
sub CopyArray( $ ) {
	local $ArrayToCopyRef = shift;
	local @Array = @$ArrayToCopyRef;
	return \@Array;
}


#------------------------------------------------------------------------
#	RebuildDependencyHash
#------------------------------------------------------------------------
sub RebuildDependencyHash( $ ) {
	local $DependencyHashFile = shift;
	open( FILE, "<$DependencyHashFile" ) || return false;
	local $KeyValue = undef;
	local @DependencyArray;

	while( <FILE> ) {
		$Line = $_;
		if( $Line =~ /\*\s+$MAME_DIR\/debug_obj\/(.+\/.+)\.obj/ ) {
				# New key value
				#* ../MAME/debug_obj/drivers/namcos22.obj
				# Store the dependency array if we have one
			if( defined( $KeyValue ) && scalar(@DependencyArray) > 0 ) {
				$DependencyHash{"$KeyValue.c"} = CopyArray( \@DependencyArray );
				@DependencyArray = ();
			}

			$KeyValue = $1;

		} elsif( $_ =~ />\s+$MAME_DIR\/debug_obj\/(.+\/.+)\.obj/ ) {
				#> ../MAME/debug_obj/vidhrdw/namcos22.obj
			push( @DependencyArray, "$1.c" );

			if( defined( $FilesDependentHash{ "$1.c" } ) ) {
				local $ArrayRef = $FilesDependentHash{ "$1.c" };
				push( @$ArrayRef, "$KeyValue.c" );
			} else {
				local @FilesDependent = ( "$KeyValue.c" );
				$FilesDependentHash{"$1.c"} = CopyArray( \@FilesDependent );
			}
		}
	}
	close( FILE );

		# Append the last item
	if( scalar( @DependencyArray ) > 0 ) {
		$DependencyHash{"$KeyValue.c"} = CopyArray( \@DependencyArray );
	}
}
