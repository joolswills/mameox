/**********************************************************
		SkinParser.l
	===========================
	Skin XML file lexer

	NOTES: - Meant to be compiled with the -i option
	       - Expects the parser to be compiled with
		        "-Psp" to set the correct prefix


**********************************************************/

 /*= D E F I N I T I O N S ======================================*/
%{
	//= I N C L U D E S ====================================================
	#include "MAMEoX.h"
	#include "xbox_FileIO.h"
	#include "StdString.h"
	#include "DebugLogger.h"

	#include "Skin.h"
	#include "SkinMetadataParser_yacc.cpp.h"

	#include <vector>
	#include <string>
	#include <malloc.h>

	//= D E F I N E S ======================================================

		/* Tell lex that we're never interactive */
	#define YY_NEVER_INTERACTIVE   1

		/* Redefine YY_INPUT to use osd_file */
	#undef YY_INPUT
	#define YY_INPUT( buf, result, max_size )			\
			if( ((result = osd_fread( g_parserSkinObject->m_parserFile, buf, max_size )) == 0 ) && !osd_feof( g_parserSkinObject->m_parserFile ) ) \
				YY_FATAL_ERROR( "input in flex scanner failed" );



	//= G L O B A L = V A R S ==============================================
	extern CSkin *g_parserSkinObject;
	static std::vector<int>					g_parentStateStack;


	//= P R O T O T Y P E S ================================================
	static void pushState( int state );
	static void popState( void );

		// Defined in SkinMetadataParser.y
	const char *SkinMetadataParserNewString( const char * );

%}


	/* YYTEXT is a pointer */
%pointer

  /***** USER DEFINITIONS ************************************/

	/* Whitespace, including \r\n's and tabs */
WHITESPACE      [\t\r ]+

	/* Alphanumeric digit */
DIGIT		[0-9]

	/* Base 16 digit */
HEXDIGIT	[0-9a-f]

	/* Single letter */
LETTER		[a-z]

	/* An identifier starts with a letter and contains any
		number of '_''s, LETTER's, or base 10 digits
	*/
IDENTIFIER		{LETTER}(-|_|{LETTER}|{DIGIT})*


EXTENTION												[a-z0-9]{3,4}
FILENAME												{IDENTIFIER}"."{EXTENTION}


OPENTAG_START										"<"{WHITESPACE}*
OPENTAG_END											{WHITESPACE}*">"

CLOSETAG_START									{OPENTAG_START}"/"
CLOSETAG_END										{OPENTAG_END}


	/* Keywords */
COMMENT_START_KW								"<!--"
COMMENT_END_KW									"-->"

TAG_SKIN_OPEN										{OPENTAG_START}"skin"{OPENTAG_END}
TAG_SKIN_CLOSE									{CLOSETAG_START}"skin"{CLOSETAG_END}

TAG_METADATA_OPEN								{OPENTAG_START}"metadata"{OPENTAG_END}
TAG_METADATA_CLOSE							{CLOSETAG_START}"metadata"{CLOSETAG_END}

TAG_AUTHOR_OPEN									{OPENTAG_START}"author"{OPENTAG_END}
TAG_AUTHOR_CLOSE								{CLOSETAG_START}"author"{CLOSETAG_END}

TAG_NAME_OPEN										{OPENTAG_START}"name"{OPENTAG_END}
TAG_NAME_CLOSE									{CLOSETAG_START}"name"{CLOSETAG_END}

TAG_DESCRIPTION_OPEN						{OPENTAG_START}"description"{OPENTAG_END}
TAG_DESCRIPTION_CLOSE						{CLOSETAG_START}"description"{CLOSETAG_END}

TAG_PREVIEWFILE_OPEN						{OPENTAG_START}"previewfile"{OPENTAG_END}
TAG_PREVIEWFILE_CLOSE						{CLOSETAG_START}"previewfile"{CLOSETAG_END}


	/* Exclusive StartConditions */
%x COMMENT_STATE STRING_STATE IGNORE_STATE




%%
 /*= R U L E S ==================================================*/

{COMMENT_START_KW}					{ /* Jump into comment_state */
															pushState( COMMENT_STATE );
														}

<*>[\n]											{ /* Increment line counter */
															++g_parserSkinObject->m_parserLine;
														}

<*>{WHITESPACE}							{ /* Whitespace is junk */
														}


{TAG_SKIN_OPEN}						{ PRINTMSG(( T_TRACE, "SKIN_OPEN" )); return SKIN_OPEN; }
{TAG_SKIN_CLOSE}					{ PRINTMSG(( T_TRACE, "SKIN_CLOSE" )); return SKIN_CLOSE; }

{TAG_METADATA_OPEN}				{ PRINTMSG(( T_TRACE, "METADATA_OPEN" )); return METADATA_OPEN; }
{TAG_METADATA_CLOSE}			{ PRINTMSG(( T_TRACE, "METADATA_CLOSE" )); pushState(IGNORE_STATE); return METADATA_CLOSE; }

{TAG_AUTHOR_OPEN}					{ PRINTMSG(( T_TRACE, "AUTHOR_OPEN" )); return AUTHOR_OPEN; }
{TAG_AUTHOR_CLOSE}				{ PRINTMSG(( T_TRACE, "AUTHOR_CLOSE" )); return AUTHOR_CLOSE; }

{TAG_NAME_OPEN}						{ PRINTMSG(( T_TRACE, "NAME_OPEN" )); return NAME_OPEN; }
{TAG_NAME_CLOSE}					{ PRINTMSG(( T_TRACE, "NAME_CLOSE" )); return NAME_CLOSE; }

{TAG_DESCRIPTION_OPEN}		{ PRINTMSG(( T_TRACE, "DESCRIPTION_OPEN" )); return DESCRIPTION_OPEN; }
{TAG_DESCRIPTION_CLOSE}		{ PRINTMSG(( T_TRACE, "DESCRIPTION_CLOSE" )); return DESCRIPTION_CLOSE; }

{TAG_PREVIEWFILE_OPEN}		{ PRINTMSG(( T_TRACE, "PREVIEWFILE_OPEN" )); return PREVIEWFILE_OPEN; }
{TAG_PREVIEWFILE_CLOSE}		{ PRINTMSG(( T_TRACE, "PREVIEWFILE_CLOSE" )); return PREVIEWFILE_CLOSE; }


{FILENAME}				{
										smplval.stringVal = SkinMetadataParserNewString( smptext );
										PRINTMSG(( T_TRACE, "_TOK_FILENAME_: %s", smptext ));
										return _TOK_FILENAME_;
									}

"\""							{
										pushState(STRING_STATE);
									}


.									{ /* Anything hitting this rule is junk, ignore it */
										#ifdef _DEBUG
										ECHO;
										#endif
									}


	/*--- I G N O R E - S T A T E --------------------------------------*/
<IGNORE_STATE>{TAG_SKIN_CLOSE}		{ PRINTMSG(( T_TRACE, "SKIN_CLOSE" )); return SKIN_CLOSE; }

<IGNORE_STATE>.										{}

	/*--- C O M M E N T - S T A T E -----------------------------------*/
<COMMENT_STATE>.*{COMMENT_END_KW}		{  /* Eat anything until COMMENT_END */
																				/* Maintain the line count */
																			const char *newline = strchr( smptext, '\n' );
																			while( newline )
																			{
																				++g_parserSkinObject->m_parserLine;
																				newline = strchr( newline + 1, '\n' );
																			}

																			popState();
																		}

	/*--- S T R I N G - S T A T E -------------------------------------*/
<STRING_STATE>.*"\""						{  /* Eat everything until the string is ended  */
																		/* Maintain the line count */
																	const char *newline = strchr( smptext, '\n' );
																	while( newline )
																	{
																		++g_parserSkinObject->m_parserLine;
																		newline = strchr( newline + 1, '\n' );
																	}

																	smplval.stringVal = SkinMetadataParserNewString( smptext );
																	PRINTMSG(( T_TRACE, "_TOK_STRING_: %s", smptext ));
																	popState();

																	return _TOK_STRING_;
																}

%%
 /*= U S E R = C O D E ==========================================*/
/*--------------------------------------------------------------------
	smpwrap
--------------------------------------------------------------------*/
int smpwrap( void )
{
		/* Return 1 to indicate that there are no more input files */
	return 1;
}

/*--------------------------------------------------------------------
	smperror
--------------------------------------------------------------------*/
void smperror( const char *error_text )
{
	PRINTMSG(( T_ERROR, 
						 "An error has occured while parsing skin.xml line %d: %s",
						 g_parserSkinObject->m_parserLine,
						 error_text ));

	g_parserSkinObject->m_parserErrorString = error_text;
}

/*--------------------------------------------------------------------
	smperror
--------------------------------------------------------------------*/
void smperror( const CStdString &error_text )
{
	PRINTMSG(( T_ERROR, 
						 "An error has occured while parsing skin.xml line %d: %s",
						 g_parserSkinObject->m_parserLine,
						 error_text.c_str() ));

	g_parserSkinObject->m_parserErrorString = error_text;
}


/*--------------------------------------------------------------------
	pushState
--------------------------------------------------------------------*/
static void pushState( int state )
{
	g_parentStateStack.push_back( YY_START );
	BEGIN( state );
}

/*--------------------------------------------------------------------
	popState
--------------------------------------------------------------------*/
static void popState( void )
{
	int parentState = g_parentStateStack.back();
	g_parentStateStack.pop_back();
	BEGIN(parentState);
}
