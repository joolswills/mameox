#!/bin/perl

# Usage:
#   FindDependencies.pl BaseDirectoryContainingObjectFiles

require 5.0.0.0;
use MAMEoXScriptConstants;

package FindDependencies;



$ObjectDirectory = MAMEoXScriptConstants::MAME_DIR."/debug_obj";

	# Only add items that get sectionized, otherwise we'll have a massive
	# dependency graph for each object
@DirectoriesToGraph = ( 
                        $ObjectDirectory."/cpu",
												$ObjectDirectory."/drivers",
												$ObjectDirectory."/machine",
												$ObjectDirectory."/sndhrdw",
#												$ObjectDirectory."/sound",
												$ObjectDirectory."/vidhrdw" );


@SystemSymbols = ( "_strcpy", "_sprintf", "_strlen", "_memset",
									 "__RTC_CheckEsp", "__RTC_InitBase", "__RTC_Shutdown",
									 "_free", '@_RTC_CheckStackVars@8', '@__security_check_cookie@4',
									 "_memcmp", "_fclose", "_fopen", "_fseek", "_memcpy", "_memmove",
									 "_qsort", "_sscanf", "_strchr", "_strcmp", "_strstr", "_tolower",
									 "_fabs", "___security_cookie", "__chkstk", "__chvalidator", "__RTC_UninitUse",
									 "_fprintf", "_strncmp", "_exit", "_strcat", "___mb_cur_max", "__fltused", 
									 "__ftol2", "__isctype", "__allmul", "__allshl", "__aulldiv", "__aullshr", 
									 "_clock", "__iob"
								 );


# Main ==============================================================
local $ObjectFileToDependenciesRef = GetDependencyHash();
if( defined( $ObjectFileToDependenciesRef ) ) {
	open( OUTFILE, ">Dependencies.txt" ) || die "Failed to create Dependencies.txt!\n";

	foreach $ObjectFile ( sort( keys( %$ObjectFileToDependenciesRef ) ) ) {
		print OUTFILE "* $ObjectFile\n\n";

			# Print the list of dependencies for this object
		print OUTFILE "File Dependencies (CPU/driver/hardware only)\n";

		$DependencyNameArrayRef = %$ObjectFileToDependenciesRef->{$ObjectFile};
		foreach $Dependency ( @$DependencyNameArrayRef ) {
			print OUTFILE "> $Dependency\n";
		}
		print OUTFILE "\n\n=================================================\n\n";
	} 
	close( OUTFILE );
	print "Done!\n";
} else {
	die "Failed to create dependency hash!\n";
}




#= F U N C T I O N S ================================================

#------------------------------------------------------------------------------------
#	GetDependencyHash
#------------------------------------------------------------------------------------
sub GetDependencyHash() {
	local %DefinedSymbolToObjectFileIndex;
	local @UndefinedSymbolsPerObjectFile;
	local @ObjectFiles = `find $ObjectDirectory -name *.obj`;
	local $counter = 0;

	local %ObjectFileToDependencies;

	if( scalar(@ObjectFiles) == 0 ) {
		print "No .obj files found in directory $ObjectDirectory! Make sure you do a DEBUG build!\n";
		return undef;
	}

		# Pass 1: Figure out what each object file defines and imports
	print "Find defines/imports for each object file (".scalar(@ObjectFiles)." objects to parse)\n";
	foreach $ObjectFile ( @ObjectFiles ) {
		local( @UndefinedSymbols );
		local( @nmResults );
		chomp( $ObjectFile );

			# run nm on the object file
		@nmResults = `nm $ObjectFile`;
		foreach $i ( @nmResults ) {
			chomp( $i );

				# Defined symbol
				# nm makes the type field for static variables lowercase
				# so it's important to be case sensitive
			if( $i =~ /[\dabcdefABCDEF]+ [RDBCT] (.+)$/ ) {
				$DefinedSymbolToObjectFileIndex{ $1 } = $counter;
			}

				# Undefined symbol
			if( $i =~ /\s+ U (.+)$/ ) {
				push( @UndefinedSymbols, $1 );
			}
		}

		push( @UndefinedSymbolsPerObjectFile, \@UndefinedSymbols );
		$counter++;
		print ".";
	}

		# Pass 2: Figure out what files are required to populate the
		#  dependency graph for each object file as much as possible.
		#  Some symbols (in particular any stdlibc stuff) will be
		#  totally undefined and ignored.

	# Array of arrays's associating an object with
	#  the index of the files on which it directly depends
	@DirectDependencyArrayList;

	print "\n\nResolve undefined symbols for each object file\n";
	$counter = 0;
	foreach $ObjectFile ( @ObjectFiles ) {
			# UndefinedSymbolsRef is a reference to an array of undefined
			# symbol names

		$UndefinedSymbolsRef = @UndefinedSymbolsPerObjectFile[$counter];
		local( @DirectDependencies );

		FOREACHUNDEFINEDSYMBOL:
		foreach $UndefinedSymbol ( @$UndefinedSymbolsRef ) {

				# Cull out system symbols
			foreach $SystemSymbol ( @SystemSymbols ) {
				if( $SystemSymbol eq $UndefinedSymbol ) {
					next FOREACHUNDEFINEDSYMBOL;
				}
			}

				# Lookup the symbol in the defined symbol hash
				#   and add the index of the object in which the symbol is 
				#   defined to the location hash
			if( defined( $DefinedSymbolToObjectFileIndex{ $UndefinedSymbol } ) ) {
				$DefinedFileIndex = $DefinedSymbolToObjectFileIndex{ $UndefinedSymbol };
				if( DoesItemExist( $DefinedFileIndex, \@DirectDependencies ) ne true ) {
					push( @DirectDependencies, $DefinedFileIndex );
				}
			}
		}

		push( @DirectDependencyArrayList, \@DirectDependencies );

		$counter++;
		print ".";
	}


	print "\n\nBuild full dependency graph for sectionized files\n";

		# Print stuff to the file
	$counter = 0;
	foreach $ObjectFile (@ObjectFiles) {
		if( ShouldCreateGraphForFile( $ObjectFile ) eq true ) {
			local @FullDependencyList;

				# Populate the Dependencies array w/ all the immediate dependencies
				# for this object, + all their dependencies
			AppendDependencies( $counter, $counter, \@FullDependencyList );

			if( scalar(@FullDependencyList) > 0 ) {
				@FullDependencyList = sort( @FullDependencyList );
				print OUTFILE "* $ObjectFile\n\n";

					# Print the list of dependencies for this object
				print OUTFILE "File Dependencies (CPU/driver/hardware only)\n";

				foreach $Dependency ( @FullDependencyList ) {
					$Dependency = $ObjectFiles[$Dependency];
					print OUTFILE "> $Dependency\n";
				}

				$ObjectFileToDependencies{$ObjectFile} = \@FullDependencyList;

				print OUTFILE "\n\n=================================================\n\n";
			}
		}
		$counter++;
	}

	return \%ObjectFileToDependencies;
}


#--------------------------------------------------------------------
#
#--------------------------------------------------------------------
sub AppendDependencies( $, $, $ ) {
	local( $originator = shift );
	local( $objectIndex = shift );
	local( $FullDependencyArrayRef = shift );

		# Go through the undefined symbols for the current object index, recursively
		#  adding the dependencies if the file is not already in the dependency array
	$DirectDependencyArrayRef = $DirectDependencyArrayList[$objectIndex];
	foreach $Dependency ( @$DirectDependencyArrayRef ) {

			# Catch circular dependency against the original caller or any dependencies that
			# have already been handled
		if( $Dependency != $originator &&
			  DoesItemExist( $Dependency, $FullDependencyArrayRef ) ne true ) {

				# Only add items that get sectionized, otherwise we'll have a massive
				# dependency graph for each object
			if( ShouldCreateGraphForFile( $ObjectFiles[$Dependency] ) eq true ) {
				push( @$FullDependencyArrayRef, $Dependency );
				AppendDependencies( $originator, $Dependency, $FullDependencyArrayRef );
			}
		}
	}
}


#--------------------------------------------------------------------
#
#--------------------------------------------------------------------
sub DoesItemExist( $, $ ) {
	local( $Item = shift );
	local( $ArrayRef = shift );

	foreach $i ( @$ArrayRef ) {
		if( $i == $Item ) {
			return true;
		}
	}

	return false;
}


#--------------------------------------------------------------------
#
#--------------------------------------------------------------------
sub ShouldCreateGraphForFile( $ ) {
	local( $Filename = shift );

	foreach $dirToTest ( @DirectoriesToGraph ) {
		if( $Filename =~ /$dirToTest[\\\/].*/ ) {
			return true;
		}
	}

	return false;
}



# Required for use directive
1;