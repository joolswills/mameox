#!/bin/perl

# Usage:
#   FindDependencies.pl BaseDirectoryContainingObjectFiles


@SystemSymbols = ( "_strcpy", "_sprintf", "_strlen", "_memset",
									 "__RTC_CheckEsp", "__RTC_InitBase", "__RTC_Shutdown",
									 "_free", '@_RTC_CheckStackVars@8', '@__security_check_cookie@4',
									 "_memcmp", "_fclose", "_fopen", "_fseek", "_memcpy", "_memmove",
									 "_qsort", "_sscanf", "_strchr", "_strcmp", "_strstr", "_tolower",
									 "_fabs", "___security_cookie", "__chkstk", "__chvalidator", "__RTC_UninitUse",
									 "_fprintf", "_strncmp", "_exit", "_strcat"
#									 "___mb_cur_max", "__fltused", "__ftol2", "__isctype",
#									 "__allmul", "__allshl", "__aulldiv", "__aullshr", "_clock", 
								 );

%DefinedSymbolToObjectFileIndex;
@UndefinedSymbolsPerObjectFile;


$ObjectDirectoryName = shift;
@ObjectFiles = `find $ObjectDirectoryName -name *.obj`;

open( OUTFILE, ">Dependencies.txt" ) || die "Failed to create Dependencies.txt!\n";

	# Pass 1: Figure out what each object file defines and imports
print "Pass 1: Find defines/imports for each object file ($#ObjectFiles objects to parse)\n";
$counter = 0;
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

	$ref = \@UndefinedSymbols;
	push( @UndefinedSymbolsPerObjectFile, $ref );

	$counter++;
	print ".";
#	if( $counter > 200 ) {
#		last;
#	}
}


	# Pass 2: Figure out what files are required to populate the
	#  dependency graph for each object file as much as possible.
	#  Some symbols (in particular any stdlibc stuff) will be
	#  totally undefined and ignored.
print "\n\nPass 2: Resolve imports for each object file\n";
$counter = 0;
foreach $ObjectFile ( @ObjectFiles ) {
		# UndefinedSymbolsRef is a reference to an array of undefined
		# symbol names
	print OUTFILE "$ObjectFile depends on:\n";

	$UndefinedSymbolsRef = @UndefinedSymbolsPerObjectFile[$counter];

	FOREACHUNDEFINEDSYMBOL:
	foreach $UndefinedSymbol ( @$UndefinedSymbolsRef ) {

			# Cull out system symbols
		foreach $SystemSymbol ( @SystemSymbols ) {
			if( $SystemSymbol eq $UndefinedSymbol ) {
				next FOREACHUNDEFINEDSYMBOL;
			}
		}

		print OUTFILE "$UndefinedSymbol  - ";

			# Lookup the symbol in the defined symbol hash
		if( defined( $DefinedSymbolToObjectFileIndex{ $UndefinedSymbol } ) ) {
			print OUTFILE $ObjectFiles[$DefinedSymbolToObjectFileIndex{ $UndefinedSymbol }];
		} else {
			printf OUTFILE "UNDEFINED";
		}

		print OUTFILE "\n";
	}


	print OUTFILE "\n\n=================================================\n\n";

	$counter++;
	print ".";
#	if( $counter > 200 ) {
#		last;
#	}
}


print "Done!\n";
