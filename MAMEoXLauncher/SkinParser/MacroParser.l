/**********************************************************
		SkinParser.l
	===========================
	Skin XML file lexer

	NOTES: - Meant to be compiled with the -i option
	       - Expects the parser to be compiled with
		        "-Pmp" to set the correct prefix


**********************************************************/

 /*= D E F I N I T I O N S ======================================*/
%{
	//= I N C L U D E S ====================================================
	#include "MAMEoX.h"
	#include "xbox_FileIO.h"
	#include "StdString.h"
	#include "DebugLogger.h"

	#include "Skin.h"
	#include "MacroParser_yacc.cpp.h"

	#include <vector>
	#include <string>
	#include <malloc.h>

	//= D E F I N E S ======================================================

		/* Tell lex that we're never interactive */
	#define YY_NEVER_INTERACTIVE   1

		/* Redefine YY_INPUT to use osd_file */
	#undef YY_INPUT
	#define YY_INPUT( buf, result, max_size )			\
			if( g_parserSkinObject->m_parserCurrentMacro.size() <= max_size ) \
			{ \
				if( g_parserSkinObject->m_parserCurrentMacro.size() ) \
					strcpy( buf, g_parserSkinObject->m_parserCurrentMacro.c_str() ); \
				g_parserSkinObject->m_parserCurrentMacro = ""; \
				return g_parserSkinObject->m_parserCurrentMacro.size(); \
			} \
			else \
			{ \
				CStdString left = g_parserSkinObject->m_parserCurrentMacro.substr( 0, max_size ); \
				g_parserSkinObject->m_parserCurrentMacro = g_parserSkinObject->m_parserCurrentMacro.substr( max_size ); \
				buf[0] = 0; \
				strncat( buf, left.c_str(), left.size() ); \
				return max_size; \
			}


	//= G L O B A L = V A R S ==============================================
	extern CSkin							*g_parserSkinObject;


	//= P R O T O T Y P E S ================================================

%}


	/* YYTEXT is a pointer */
%pointer

  /***** USER DEFINITIONS ************************************/

	/* Whitempace, including \r\n's and tabs */
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
IDENTIFIER						{LETTER}(_|{LETTER}|{DIGIT})*

HOOK_IDENTIFIER				"$"{IDENTIFIER}"$"

NUMBER								"-"?{DIGIT}+
HEXNUMBER							("0x"{HEXDIGIT}+)|({HEXDIGIT}+"h")

PROPERTY_SEPARATOR		"."

%%
 /*= R U L E S ==================================================*/

{WHITESPACE}					{ /* Whitempace is junk */
											}


{PROPERTY_SEPARATOR}	{ return '.'; }
"left"								{	return _TOK_LEFT_; }
"right"								{	return _TOK_RIGHT_; }
"top"									{	return _TOK_TOP_; }
"bottom"							{	return _TOK_BOTTOM_; }
"width"								{	return _TOK_WIDTH_; }
"height"							{	return _TOK_HEIGHT_; }
"+"										{ return '+'; }
"-"										{ return '-'; }
"/"										{ return '/'; }
"*"										{ return '*'; }
"<"										{ return '<'; }
"<="									{ return _TOK_LE_; }
">"										{ return '>'; }
">="									{ return _TOK_GE_; }
"!="									{ return _TOK_NE_; }
"=="									{ return _TOK_EQ_; }

{HOOK_IDENTIFIER}			{ 
												mplval.stringVal = strdup(mptext);
												return _TOK_HOOK_IDENTIFIER_;
											}


{IDENTIFIER}					{ 
												mplval.stringVal = strdup(mptext);
												return _TOK_IDENTIFIER_;
											}


{NUMBER}							{
												mplval.intVal = atoi( mptext );
												return _TOK_INT_;
											}

{HEXNUMBER}						{
												const char *str = mptext;
												if( !strnicmp( str, "0x", 2 ) )
													str += 2;
												sscanf( str, "%x", &mplval.intVal );
												return _TOK_INT_;
											}

.											{ /* Anything hitting this rule is probably an error, but ignore it */
											}


%%
 /*= U S E R = C O D E ==========================================*/
/*--------------------------------------------------------------------
	mpwrap
--------------------------------------------------------------------*/
int mpwrap( void )
{
		/* Return 1 to indicate that there are no more input files */
	return 1;
}

/*--------------------------------------------------------------------
	mperror
--------------------------------------------------------------------*/
void mperror( const char *error_text )
{
	PRINTMSG(( T_ERROR, 
						 "An error has occured while parsing macro \"%s\": %s",
						 g_parserSkinObject->m_parserCurrentMacro,
						 error_text ));

	g_parserSkinObject->m_parserErrorString = error_text;
}

/*--------------------------------------------------------------------
	mperror
--------------------------------------------------------------------*/
void mperror( const CStdString &error_text )
{
	PRINTMSG(( T_ERROR, 
						 "An error has occured while parsing macro \"%s\": %s",
						 g_parserSkinObject->m_parserCurrentMacro,
						 error_text.c_str() ));

	g_parserSkinObject->m_parserErrorString = error_text;
}
