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
	#include "SkinParser_yacc.cpp.h"

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

		// Defined in SkinParser.y
	const char *SkinParserNewString( const char * );

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


XML_TAG				"<"("/"|{LETTER}|{WHITESPACE})+">"

	/* An identifier starts with a letter and contains any
		number of '_''s, LETTER's, or base 10 digits
	*/
IDENTIFIER		{LETTER}(-|_|{LETTER}|{DIGIT})*
IMMEDIATE			{DIGIT}+|{HEXDIGIT}+

EXTENTION			[a-z0-9]{3,4}
FILENAME			{IDENTIFIER}"."{EXTENTION}

NUMBER				"-"?{DIGIT}+
HEXNUMBER			("0x"{HEXDIGIT}+)|({HEXDIGIT}+"h")

OPERATOR			"+"|"-"|"/"|"*"|"<"|"<="|">"|">="|"!="|"=="
PROPERTY			{IDENTIFIER}"->"{IDENTIFIER}
SYSVAR				"$"{IDENTIFIER}"$"

MACROVALUE		({IMMEDIATE}|{PROPERTY}|{SYSVAR})
MACRO					({MACROVALUE}{WHITESPACE}*{OPERATOR}{WHITESPACE}*{MACROVALUE})|{PROPERTY}|{SYSVAR}

OPENTAG_START										"<"{WHITESPACE}*
OPENTAG_END											{WHITESPACE}*">"

CLOSETAG_START									{OPENTAG_START}"/"
CLOSETAG_END										{OPENTAG_END}

EMPTYTAG_START									{OPENTAG_START}
EMPTYTAG_END										{WHITESPACE}*"/"{OPENTAG_END}


	/* Keywords */
COMMENT_START_KW								"<!--"
COMMENT_END_KW									"-->"

TAG_SKIN_OPEN										{OPENTAG_START}"skin"{OPENTAG_END}
TAG_SKIN_CLOSE									{CLOSETAG_START}"skin"{CLOSETAG_END}

TAG_METADATA_OPEN								{OPENTAG_START}"metadata"{OPENTAG_END}
TAG_METADATA_CLOSE							{CLOSETAG_START}"metadata"{CLOSETAG_END}


TAG_RESOURCEDEFINITIONS_OPEN		{OPENTAG_START}"resourcedefinitions"{OPENTAG_END}
TAG_RESOURCEDEFINITIONS_CLOSE		{CLOSETAG_START}"resourcedefinitions"{CLOSETAG_END}

TAG_RESOURCEFILE_OPEN						{OPENTAG_START}"resourcefile"{OPENTAG_END}
TAG_RESOURCEFILE_CLOSE					{CLOSETAG_START}"resourcefile"{CLOSETAG_END}

TAG_COLORCHANNELFILENAME_OPEN		{OPENTAG_START}"colorchannelfilename"{OPENTAG_END}
TAG_COLORCHANNELFILENAME_CLOSE	{CLOSETAG_START}"colorchannelfilename"{CLOSETAG_END}

TAG_ALPHACHANNELFILENAME_OPEN		{OPENTAG_START}"alphachannelfilename"{OPENTAG_END}
TAG_ALPHACHANNELFILENAME_CLOSE	{CLOSETAG_START}"alphachannelfilename"{CLOSETAG_END}

TAG_RESOURCE_OPEN								{OPENTAG_START}"resource"{OPENTAG_END}
TAG_RESOURCE_CLOSE							{CLOSETAG_START}"resource"{CLOSETAG_END}

TAG_ID_OPEN											{OPENTAG_START}"id"{OPENTAG_END}
TAG_ID_CLOSE										{CLOSETAG_START}"id"{CLOSETAG_END}

TAG_AREA_OPEN										{OPENTAG_START}"area"{OPENTAG_END}
TAG_AREA_CLOSE									{CLOSETAG_START}"area"{CLOSETAG_END}

TAG_LEFT_OPEN										{OPENTAG_START}"left"{OPENTAG_END}
TAG_LEFT_CLOSE									{CLOSETAG_START}"left"{CLOSETAG_END}

TAG_TOP_OPEN										{OPENTAG_START}"top"{OPENTAG_END}
TAG_TOP_CLOSE										{CLOSETAG_START}"top"{CLOSETAG_END}

TAG_RIGHT_OPEN									{OPENTAG_START}"right"{OPENTAG_END}
TAG_RIGHT_CLOSE									{CLOSETAG_START}"right"{CLOSETAG_END}

TAG_BOTTOM_OPEN									{OPENTAG_START}"bottom"{OPENTAG_END}
TAG_BOTTOM_CLOSE								{CLOSETAG_START}"bottom"{CLOSETAG_END}

TAG_BACKDROP_OPEN								{OPENTAG_START}"backdrop"{OPENTAG_END}
TAG_BACKDROP_CLOSE							{CLOSETAG_START}"backdrop"{CLOSETAG_END}

TAG_DEFAULTS_OPEN								{OPENTAG_START}"defaults"{OPENTAG_END}
TAG_DEFAULTS_CLOSE							{CLOSETAG_START}"defaults"{CLOSETAG_END}

TAG_TEXT_OPEN										{OPENTAG_START}"text"{OPENTAG_END}
TAG_TEXT_CLOSE									{CLOSETAG_START}"text"{CLOSETAG_END}

TAG_FONT_OPEN										{OPENTAG_START}"font"{OPENTAG_END}
TAG_FONT_CLOSE									{CLOSETAG_START}"font"{CLOSETAG_END}

TAG_COLOR_OPEN									{OPENTAG_START}"color"{OPENTAG_END}
TAG_COLOR_CLOSE									{CLOSETAG_START}"color"{CLOSETAG_END}

TAG_STRING_OPEN									{OPENTAG_START}"string"{OPENTAG_END}
TAG_STRING_CLOSE								{CLOSETAG_START}"string"{CLOSETAG_END}

TAG_CENTERVERTICALLY						{EMPTYTAG_START}"centervertically"{EMPTYTAG_END}
TAG_CENTERHORIZONTALLY					{EMPTYTAG_START}"centerhorizontally"{EMPTYTAG_END}
TAG_UNBOUNDED										{EMPTYTAG_START}"unbounded"{EMPTYTAG_END}

TAG_ALPHA_OPEN									{OPENTAG_START}"alpha"{OPENTAG_END}
TAG_ALPHA_CLOSE									{CLOSETAG_START}"alpha"{CLOSETAG_END}

TAG_RED_OPEN										{OPENTAG_START}"red"{OPENTAG_END}
TAG_RED_CLOSE										{CLOSETAG_START}"red"{CLOSETAG_END}

TAG_GREEN_OPEN									{OPENTAG_START}"green"{OPENTAG_END}
TAG_GREEN_CLOSE									{CLOSETAG_START}"green"{CLOSETAG_END}

TAG_BLUE_OPEN										{OPENTAG_START}"blue"{OPENTAG_END}
TAG_BLUE_CLOSE									{CLOSETAG_START}"blue"{CLOSETAG_END}

TAG_ARGB_OPEN										{OPENTAG_START}"argb"{OPENTAG_END}
TAG_ARGB_CLOSE									{CLOSETAG_START}"argb"{CLOSETAG_END}

TAG_ICON_OPEN										{OPENTAG_START}"icon"{OPENTAG_END}
TAG_ICON_CLOSE									{CLOSETAG_START}"icon"{CLOSETAG_END}

TAG_TEXTUREID_OPEN							{OPENTAG_START}"textureid"{OPENTAG_END}
TAG_TEXTUREID_CLOSE							{CLOSETAG_START}"textureid"{CLOSETAG_END}

TAG_POSITION_OPEN								{OPENTAG_START}"position"{OPENTAG_END}
TAG_POSITION_CLOSE							{CLOSETAG_START}"position"{CLOSETAG_END}

TAG_X_OPEN											{OPENTAG_START}"x"{OPENTAG_END}
TAG_X_CLOSE											{CLOSETAG_START}"x"{CLOSETAG_END}

TAG_Y_OPEN											{OPENTAG_START}"y"{OPENTAG_END}
TAG_Y_CLOSE											{CLOSETAG_START}"y"{CLOSETAG_END}

TAG_SIZE_OPEN										{OPENTAG_START}"size"{OPENTAG_END}
TAG_SIZE_CLOSE									{CLOSETAG_START}"size"{CLOSETAG_END}

TAG_WIDTH_OPEN									{OPENTAG_START}"width"{OPENTAG_END}
TAG_WIDTH_CLOSE									{CLOSETAG_START}"width"{CLOSETAG_END}

TAG_HEIGHT_OPEN									{OPENTAG_START}"height"{OPENTAG_END}
TAG_HEIGHT_CLOSE								{CLOSETAG_START}"height"{CLOSETAG_END}

TAG_CONDITION_OPEN							{OPENTAG_START}"condition"{OPENTAG_END}
TAG_CONDITION_CLOSE							{CLOSETAG_START}"condition"{CLOSETAG_END}




	/* Screen widgets */
TAG_HELPSCREEN_OPEN							{OPENTAG_START}"helpscreen"{OPENTAG_END}
TAG_HELPSCREEN_CLOSE						{CLOSETAG_START}"helpscreen"{CLOSETAG_END}

TAG_OPTIONSSCREEN_OPEN					{OPENTAG_START}"optionsscreen"{OPENTAG_END}
TAG_OPTIONSSCREEN_CLOSE					{CLOSETAG_START}"optionsscreen"{CLOSETAG_END}

TAG_ROMLISTSCREEN_OPEN					{OPENTAG_START}"romlistscreen"{OPENTAG_END}
TAG_ROMLISTSCREEN_CLOSE					{CLOSETAG_START}"romlistscreen"{CLOSETAG_END}

TAG_SPLASHSCREEN_OPEN						{OPENTAG_START}"splashscreen"{OPENTAG_END}
TAG_SPLASHSCREEN_CLOSE					{CLOSETAG_START}"splashscreen"{CLOSETAG_END}

TAG_MESSAGESCREEN_OPEN					{OPENTAG_START}"messagescreen"{OPENTAG_END}
TAG_MESSAGESCREEN_CLOSE					{CLOSETAG_START}"messagescreen"{CLOSETAG_END}

	/* Popup widgets */
TAG_STARTMENU_OPEN							{OPENTAG_START}"startmenu"{OPENTAG_END}
TAG_STARTMENU_CLOSE							{CLOSETAG_START}"startmenu"{CLOSETAG_END}

TAG_VIRTUALKEYBOARD_OPEN				{OPENTAG_START}"virtualkeyboard"{OPENTAG_END}
TAG_VIRTUALKEYBOARD_CLOSE				{CLOSETAG_START}"virtualkeyboard"{CLOSETAG_END}

	/* Screensaver */
TAG_SCREENSAVER_OPEN						{OPENTAG_START}"screensaver"{OPENTAG_END}
TAG_SCREENSAVER_CLOSE						{CLOSETAG_START}"screensaver"{CLOSETAG_END}



	/* Exclusive StartConditions */
%x COMMENT_STATE STRING_STATE METADATA_GARBAGE_STATE


%%
 /*= R U L E S ==================================================*/

{COMMENT_START_KW}					{ /* Jump into comment_state */
															pushState( COMMENT_STATE );
														}


{TAG_METADATA_OPEN}					{ /* Throw away anything until the closing tag  */
															pushState( METADATA_GARBAGE_STATE );
														}

{WHITESPACE}								{ /* Whitespace is junk */
														}



{TAG_SKIN_OPEN}															{ PRINTMSG(( T_TRACE, "SKIN_OPEN" )); return SKIN_OPEN; }
{TAG_SKIN_CLOSE}														{ PRINTMSG(( T_TRACE, "SKIN_CLOSE" )); return SKIN_CLOSE; }

{TAG_RESOURCEDEFINITIONS_OPEN}							{ PRINTMSG(( T_TRACE, "RESOURCEDEFINITIONS_OPEN" )); return RESOURCEDEFINITIONS_OPEN; }
{TAG_RESOURCEDEFINITIONS_CLOSE}							{ PRINTMSG(( T_TRACE, "RESOURCEDEFINITIONS_CLOSE" )); return RESOURCEDEFINITIONS_CLOSE; }

{TAG_RESOURCEFILE_OPEN}											{ PRINTMSG(( T_TRACE, "RESOURCEFILE_OPEN" )); return RESOURCEFILE_OPEN; }
{TAG_RESOURCEFILE_CLOSE}										{ PRINTMSG(( T_TRACE, "RESOURCEFILE_CLOSE" )); return RESOURCEFILE_CLOSE; }

{TAG_COLORCHANNELFILENAME_OPEN}							{ PRINTMSG(( T_TRACE, "COLORCHANNELFILENAME_OPEN" )); return COLORCHANNELFILENAME_OPEN; }
{TAG_COLORCHANNELFILENAME_CLOSE}  					{ PRINTMSG(( T_TRACE, "COLORCHANNELFILENAME_CLOSE" )); return COLORCHANNELFILENAME_CLOSE; }

{TAG_ALPHACHANNELFILENAME_OPEN}							{ PRINTMSG(( T_TRACE, "ALPHACHANNELFILENAME_OPEN" )); return ALPHACHANNELFILENAME_OPEN; }
{TAG_ALPHACHANNELFILENAME_CLOSE}  					{ PRINTMSG(( T_TRACE, "ALPHACHANNELFILENAME_CLOSE" )); return ALPHACHANNELFILENAME_CLOSE; }

{TAG_RESOURCE_OPEN}													{ PRINTMSG(( T_TRACE, "RESOURCE_OPEN" )); return RESOURCE_OPEN; }
{TAG_RESOURCE_CLOSE}  											{ PRINTMSG(( T_TRACE, "RESOURCE_CLOSE" )); return RESOURCE_CLOSE; }

{TAG_ID_OPEN}																{ PRINTMSG(( T_TRACE, "ID_OPEN" )); return ID_OPEN; }
{TAG_ID_CLOSE}															{ PRINTMSG(( T_TRACE, "ID_CLOSE" )); return ID_CLOSE; }

{TAG_AREA_OPEN}															{ PRINTMSG(( T_TRACE, "AREA_OPEN" )); return AREA_OPEN; }
{TAG_AREA_CLOSE}														{ PRINTMSG(( T_TRACE, "AREA_CLOSE" )); return AREA_CLOSE; }

{TAG_LEFT_OPEN}  														{ PRINTMSG(( T_TRACE, "LEFT_OPEN" )); return LEFT_OPEN; }
{TAG_LEFT_CLOSE}  													{ PRINTMSG(( T_TRACE, "LEFT_CLOSE" )); return LEFT_CLOSE; }

{TAG_TOP_OPEN}  														{ PRINTMSG(( T_TRACE, "TOP_OPEN" )); return TOP_OPEN; }
{TAG_TOP_CLOSE}  														{ PRINTMSG(( T_TRACE, "TOP_CLOSE" )); return TOP_CLOSE; }

{TAG_RIGHT_OPEN}  													{ PRINTMSG(( T_TRACE, "RIGHT_OPEN" )); return RIGHT_OPEN; }
{TAG_RIGHT_CLOSE}  													{ PRINTMSG(( T_TRACE, "RIGHT_CLOSE" )); return RIGHT_CLOSE; }

{TAG_BOTTOM_OPEN}  													{ PRINTMSG(( T_TRACE, "BOTTOM_OPEN" )); return BOTTOM_OPEN; }
{TAG_BOTTOM_CLOSE}  												{ PRINTMSG(( T_TRACE, "BOTTOM_CLOSE" )); return BOTTOM_CLOSE; }

{TAG_BACKDROP_OPEN}													{ PRINTMSG(( T_TRACE, "BACKDROP_OPEN" )); return BACKDROP_OPEN; }
{TAG_BACKDROP_CLOSE}  											{ PRINTMSG(( T_TRACE, "BACKDROP_CLOSE" )); return BACKDROP_CLOSE; }

{TAG_DEFAULTS_OPEN}  												{ PRINTMSG(( T_TRACE, "DEFAULTS_OPEN" )); return DEFAULTS_OPEN; }
{TAG_DEFAULTS_CLOSE}  											{ PRINTMSG(( T_TRACE, "DEFAULTS_CLOSE" )); return DEFAULTS_CLOSE; }

{TAG_TEXT_OPEN}  														{ PRINTMSG(( T_TRACE, "TEXT_OPEN" )); return TEXT_OPEN; }
{TAG_TEXT_CLOSE}  													{ PRINTMSG(( T_TRACE, "TEXT_CLOSE" )); return TEXT_CLOSE; }

{TAG_FONT_OPEN}  														{ PRINTMSG(( T_TRACE, "FONT_OPEN" )); return FONT_OPEN; }
{TAG_FONT_CLOSE}  													{ PRINTMSG(( T_TRACE, "FONT_CLOSE" )); return FONT_CLOSE; }

{TAG_COLOR_OPEN}  													{ PRINTMSG(( T_TRACE, "COLOR_OPEN" )); return COLOR_OPEN; }
{TAG_COLOR_CLOSE}  													{ PRINTMSG(( T_TRACE, "COLOR_CLOSE" )); return COLOR_CLOSE; }

{TAG_STRING_OPEN}  													{ PRINTMSG(( T_TRACE, "STRING_OPEN" )); return STRING_OPEN; }
{TAG_STRING_CLOSE}  												{ PRINTMSG(( T_TRACE, "STRING_CLOSE" )); return STRING_CLOSE; }

{TAG_CENTERVERTICALLY}											{ PRINTMSG(( T_TRACE, "CENTERVERTICALLY" )); return CENTERVERTICALLY; }
{TAG_CENTERHORIZONTALLY}  									{ PRINTMSG(( T_TRACE, "CENTERHORIZONTALLY" )); return CENTERHORIZONTALLY; }
{TAG_UNBOUNDED}  														{ PRINTMSG(( T_TRACE, "UNBOUNDED" )); return UNBOUNDED; }

{TAG_ALPHA_OPEN}  													{ PRINTMSG(( T_TRACE, "ALPHA_OPEN" )); return ALPHA_OPEN; }
{TAG_ALPHA_CLOSE}  													{ PRINTMSG(( T_TRACE, "ALPHA_CLOSE" )); return ALPHA_CLOSE; }

{TAG_RED_OPEN}  														{ PRINTMSG(( T_TRACE, "RED_OPEN" )); return RED_OPEN; }
{TAG_RED_CLOSE}  														{ PRINTMSG(( T_TRACE, "RED_CLOSE" )); return RED_CLOSE; }

{TAG_GREEN_OPEN}  													{ PRINTMSG(( T_TRACE, "GREEN_OPEN" )); return GREEN_OPEN; }
{TAG_GREEN_CLOSE}  													{ PRINTMSG(( T_TRACE, "GREEN_CLOSE" )); return GREEN_CLOSE; }

{TAG_BLUE_OPEN}  														{ PRINTMSG(( T_TRACE, "BLUE_OPEN" )); return BLUE_OPEN; }
{TAG_BLUE_CLOSE}  													{ PRINTMSG(( T_TRACE, "BLUE_CLOSE" )); return BLUE_CLOSE; }

{TAG_ARGB_OPEN}															{ PRINTMSG(( T_TRACE, "ARGB_OPEN" )); return ARGB_OPEN; }
{TAG_ARGB_CLOSE}  													{ PRINTMSG(( T_TRACE, "ARGB_CLOSE" )); return ARGB_CLOSE; }

{TAG_ICON_OPEN}  														{ PRINTMSG(( T_TRACE, "ICON_OPEN" )); return ICON_OPEN; }
{TAG_ICON_CLOSE}  													{ PRINTMSG(( T_TRACE, "ICON_CLOSE" )); return ICON_CLOSE; }

{TAG_TEXTUREID_OPEN}  											{ PRINTMSG(( T_TRACE, "TEXTUREID_OPEN" )); return TEXTUREID_OPEN; }
{TAG_TEXTUREID_CLOSE}  											{ PRINTMSG(( T_TRACE, "TEXTUREID_CLOSE" )); return TEXTUREID_CLOSE; }

{TAG_POSITION_OPEN}													{ PRINTMSG(( T_TRACE, "POSITION_OPEN" )); return POSITION_OPEN; }
{TAG_POSITION_CLOSE}  											{ PRINTMSG(( T_TRACE, "POSITION_CLOSE" )); return POSITION_CLOSE; }

{TAG_X_OPEN}																{ PRINTMSG(( T_TRACE, "X_OPEN" )); return X_OPEN; }
{TAG_X_CLOSE}																{ PRINTMSG(( T_TRACE, "X_CLOSE" )); return X_CLOSE; }

{TAG_Y_OPEN}																{ PRINTMSG(( T_TRACE, "Y_OPEN" )); return Y_OPEN; }
{TAG_Y_CLOSE}																{ PRINTMSG(( T_TRACE, "Y_CLOSE" )); return Y_CLOSE; }

{TAG_SIZE_OPEN}															{ PRINTMSG(( T_TRACE, "SIZE_OPEN" )); return SIZE_OPEN; }
{TAG_SIZE_CLOSE}  													{ PRINTMSG(( T_TRACE, "SIZE_CLOSE" )); return SIZE_CLOSE; }

{TAG_WIDTH_OPEN}  													{ PRINTMSG(( T_TRACE, "WIDTH_OPEN" )); return WIDTH_OPEN; }
{TAG_WIDTH_CLOSE}  													{ PRINTMSG(( T_TRACE, "WIDTH_CLOSE" )); return WIDTH_CLOSE; }

{TAG_HEIGHT_OPEN}  													{ PRINTMSG(( T_TRACE, "HEIGHT_OPEN" )); return HEIGHT_OPEN; }
{TAG_HEIGHT_CLOSE}													{ PRINTMSG(( T_TRACE, "HEIGHT_CLOSE" )); return HEIGHT_CLOSE; }

{TAG_CONDITION_OPEN}												{ PRINTMSG(( T_TRACE, "CONDITION_OPEN" )); return CONDITION_OPEN; }
{TAG_CONDITION_CLOSE}												{ PRINTMSG(( T_TRACE, "CONDITION_CLOSE" )); return CONDITION_CLOSE; }


	/* Screen widgets */
{TAG_HELPSCREEN_OPEN}  											{ PRINTMSG(( T_TRACE, "HELPSCREEN_OPEN" )); return HELPSCREEN_OPEN; }
{TAG_HELPSCREEN_CLOSE}  										{ PRINTMSG(( T_TRACE, "HELPSCREEN_CLOSE" )); return HELPSCREEN_CLOSE; }

{TAG_OPTIONSSCREEN_OPEN}  									{ PRINTMSG(( T_TRACE, "OPTIONSSCREEN_OPEN" )); return OPTIONSSCREEN_OPEN; }
{TAG_OPTIONSSCREEN_CLOSE}  									{ PRINTMSG(( T_TRACE, "OPTIONSSCREEN_CLOSE" )); return OPTIONSSCREEN_CLOSE; }

{TAG_ROMLISTSCREEN_OPEN}  									{ PRINTMSG(( T_TRACE, "ROMLISTSCREEN_OPEN" )); return ROMLISTSCREEN_OPEN; }
{TAG_ROMLISTSCREEN_CLOSE}  									{ PRINTMSG(( T_TRACE, "ROMLISTSCREEN_CLOSE" )); return ROMLISTSCREEN_CLOSE; }

{TAG_SPLASHSCREEN_OPEN}  										{ PRINTMSG(( T_TRACE, "SPLASHSCREEN_OPEN" )); return SPLASHSCREEN_OPEN; }
{TAG_SPLASHSCREEN_CLOSE}  									{ PRINTMSG(( T_TRACE, "SPLASHSCREEN_CLOSE" )); return SPLASHSCREEN_CLOSE; }

{TAG_MESSAGESCREEN_OPEN}  									{ PRINTMSG(( T_TRACE, "MESSAGESCREEN_OPEN" )); return MESSAGESCREEN_OPEN; }
{TAG_MESSAGESCREEN_CLOSE}  									{ PRINTMSG(( T_TRACE, "MESSAGESCREEN_CLOSE" )); return MESSAGESCREEN_CLOSE; }

	/* Popup widgets */
{TAG_STARTMENU_OPEN}  											{ PRINTMSG(( T_TRACE, "STARTMENU_OPEN" )); return STARTMENU_OPEN; }
{TAG_STARTMENU_CLOSE}  											{ PRINTMSG(( T_TRACE, "STARTMENU_CLOSE" )); return STARTMENU_CLOSE; }

{TAG_VIRTUALKEYBOARD_OPEN}  								{ PRINTMSG(( T_TRACE, "VIRTUALKEYBOARD_OPEN" )); return VIRTUALKEYBOARD_OPEN; }
{TAG_VIRTUALKEYBOARD_CLOSE}									{ PRINTMSG(( T_TRACE, "VIRTUALKEYBOARD_CLOSE" )); return VIRTUALKEYBOARD_CLOSE; }

	/* Screensaver */
{TAG_SCREENSAVER_OPEN}  										{ PRINTMSG(( T_TRACE, "SCREENSAVER_OPEN" )); return SCREENSAVER_OPEN; }
{TAG_SCREENSAVER_CLOSE}											{ PRINTMSG(( T_TRACE, "SCREENSAVER_CLOSE" )); return SCREENSAVER_CLOSE; }

 /* Note that all macros/filenames/numbers are returned as strings. The SkinParser
	 does not evaluate macros/immediate values. This is done in the MacroParser */
{MACRO}						{
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_MACRO_: %s", sptext ));
										return _TOK_MACRO_;
									}

{OPERATOR}				{
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_MACRO_OPERATOR_: %s", sptext ));
										return _TOK_MACRO_OPERATOR_;
									}


{IDENTIFIER}			{ 
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_IDENTIFIER_: %s", sptext ));
										return _TOK_IDENTIFIER_;
									}

{FILENAME}				{
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_FILENAME_: %s", sptext ));
										return _TOK_FILENAME_;
									}

{NUMBER}					{
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_INT_: %s", sptext ));
										return _TOK_INT_;
									}

{HEXNUMBER}				{
										splval.stringVal = SkinParserNewString( sptext );
										PRINTMSG(( T_INFO, "_TOK_INT_: %s", sptext ));
										return _TOK_INT_;
									}

"\""							{
										pushState(STRING_STATE);
									}


.									{ /* Anything hitting this rule is probably an error, but ignore it */
										#ifdef _DEBUG
											ECHO;
										#endif
									}





	/*--- C O M M E N T - S T A T E -----------------------------------*/
<COMMENT_STATE>.*{COMMENT_END_KW}		{  /* Eat anything until COMMENT_END */
																				/* Maintain the line count */
																			const char *newline = strchr( sptext, '\n' );
																			while( newline )
																			{
																				++g_parserSkinObject->m_parserLine;
																				newline = strchr( newline + 1, '\n' );
																			}

																			popState();
																		}

	/*--- M E T A D A T A - G A R B A G E - S T A T E -----------------*/
<METADATA_GARBAGE_STATE>.*{TAG_METADATA_CLOSE}		{  /* Eat anything until TAG_METADATA_CLOSE */
																											/* Maintain the line count */
																										const char *newline = strchr( sptext, '\n' );
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
																	const char *newline = strchr( sptext, '\n' );
																	while( newline )
																	{
																		++g_parserSkinObject->m_parserLine;
																		newline = strchr( newline + 1, '\n' );
																	}

																	splval.stringVal = SkinParserNewString( sptext );
																	PRINTMSG(( T_TRACE, "_TOK_STRING_: %s", sptext ));
																	popState();

																	return _TOK_STRING_;
																}

%%
 /*= U S E R = C O D E ==========================================*/
/*--------------------------------------------------------------------
	spwrap
--------------------------------------------------------------------*/
int spwrap( void )
{
		/* Return 1 to indicate that there are no more input files */
	return 1;
}

/*--------------------------------------------------------------------
	sperror
--------------------------------------------------------------------*/
void sperror( const char *error_text )
{
	PRINTMSG(( T_ERROR, 
						 "An error has occured while parsing skin.xml line %d: %s",
						 g_parserSkinObject->m_parserLine,
						 error_text ));

	g_parserSkinObject->m_parserErrorString = error_text;
}

/*--------------------------------------------------------------------
	sperror
--------------------------------------------------------------------*/
void sperror( const CStdString &error_text )
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
