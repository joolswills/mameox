#!/bin/perl

# Usage:
#   FindDependencies.pl BaseDirectoryContainingObjectFiles



$ObjectDirectoryName = shift;
@ObjectFiles = `find $ObjectDirectoryName -name *.obj`;

@SystemSymbols = ( "_strcpy", "_sprintf", "_strlen", "_memset",
									 "__RTC_CheckEsp", "__RTC_InitBase", "__RTC_Shutdown"
								 );

@DefinedSymbolsPerObjectFile;
@UndefinedSymbolsPerObjectFile;



$counter = 0;


	# Pass 1: Figure out what each object file defines and imports
foreach $ObjectFile ( @ObjectFiles ) {
	local( @DefinedSymbols );
	local( @UndefinedSymbols );
	local( @nmResults );


		# run nm on the object file
	@nmResults = `nm $ObjectFile`;
	foreach $i ( @nmResults ) {
		chomp( $i );

		  # Defined symbol
		if( $i =~ /\d+ [BCT] (.+)$/ ) {
			push( @DefinedSymbols, $1 );
		}

		  # Undefined symbol
		if( $i =~ /\s+ U (.+)$/ ) {
			push( @UndefinedSymbols, $1 );
		}
	}

	$ref = \@DefinedSymbols;
	push( @DefinedSymbolsPerObjectFile, $ref );

	$ref = \@UndefinedSymbols;
	push( @UndefinedSymbolsPerObjectFile, $ref );

	$counter++;
	if( $counter > 50 ) {
		last;
	}
}


	# Pass 2: Figure out what files are required to populate the
	#  dependency graph for each object file as much as possible.
	#  Some symbols (in particular any stdlibc stuff) will be
	#  totally undefined and ignored.
$counter = 0;
foreach $ObjectFile ( @ObjectFiles ) {
		# UndefinedSymbolsRef is a reference to an array of undefined
		# symbol names
	chomp( $ObjectFile );
	print "$ObjectFile depends on:\n";

	$UndefinedSymbolsRef = @UndefinedSymbolsPerObjectFile[$counter];

	FOREACHUNDEFINEDSYMBOL:
	foreach $UndefinedSymbol ( @$UndefinedSymbolsRef ) {

			# Cull out system symbols
		foreach $SystemSymbol ( @SystemSymbols ) {
			if( $SystemSymbol eq $UndefinedSymbol ) {
				next FOREACHUNDEFINEDSYMBOL;
			}
		}

		print "$UndefinedSymbol  - ";

			# Search through each define list until we
			# find the symbol
		$symbolFoundInFile = 0;
		FOREACHSYMBOLTABLE:
		foreach $DefinedSymbolTableRef (@DefinedSymbolsPerObjectFile) {
			foreach $DefinedSymbol ( @$DefinedSymbolTableRef ) {
				if( $DefinedSymbol eq $Symbol ) {
					print "$ObjectFiles[$symbolFoundInFile]";
					last FOREACHSYMBOLTABLE;
				}
			}
			$symbolFoundInFile++;
		}

		print "\n";
	}

	print "\n\n=================================================\n\n";

	$counter++;
	if( $counter > 50 ) {
		last;
	}
}
