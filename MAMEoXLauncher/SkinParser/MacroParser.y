/**********************************************************
		MacroParser.y
	===========================
	Skin macro lexer

	-Meant to be compiled with "-p mp" to set proper prefix

**********************************************************/


/*= D E F I N I T I O N S =====================================*/
%{
	//= I N C L U D E S ====================================================
	#include "MAMEoX.h"
	#include "xbox_FileIO.h"
	#include "StdString.h"

	#include "Skin.h"

	#include <Xtl.h>
	#include <malloc.h>
	#include <vector>
	#include <map>


	//= D E F I N E S ======================================================
	#define SAFE_FREE( p )			if( p ) free( p );  p = NULL;
	#define CLEAN_AND_DIE()			YYABORT;


	//= S T R U C T U R E S ================================================

	//= P R O T O T Y P E S ================================================
	void	mperror( const char * );
	void	mperror( const CStdString & );
	int		mplex( void );

	//= G L O B A L = V A R S ==============================================

	//= F L E X = G E N E R A T E D ========================================
%}

	/* Type UNION */
%union {
    char					*stringVal;
    int						intVal;
		void					*voidPtrVal;
}

%start start_symbol

	/* Token Declarations */

%token <stringVal>			_TOK_IDENTIFIER_
%token <stringVal>			_TOK_HOOK_IDENTIFIER_
%token <intVal>					_TOK_INT_

%token <intVal>					_TOK_LEFT_
%token <intVal>					_TOK_RIGHT_
%token <intVal>					_TOK_TOP_
%token <intVal>					_TOK_BOTTOM_
%token <intVal>					_TOK_WIDTH_
%token <intVal>					_TOK_HEIGHT_

%token <intVal>					_TOK_LE_
%token <intVal>					_TOK_GE_
%token <intVal>					_TOK_NE_
%token <intVal>					_TOK_EQ_


	/* Type Declarations */
%type <intVal>					macro
%type <intVal>					arg

/*= P R O D U C T I O N S =====================================*/
%%

	
start_symbol:				macro
										{}


macro:							arg | 
										arg '+' arg { $$ = $1 + $3; } |
										arg '-' arg { $$ = $1 - $3; } |
										arg '*' arg { $$ = $1 * $3; } |
										arg '/' arg { $$ = $1 / $3; } |
										/* empty */
										{}

arg:								_TOK_INT_
										{
											$$ = $1;
										}

arg:								_TOK_LEFT_
										{}

arg:								_TOK_RIGHT_
										{}

arg:								_TOK_TOP_
										{}

arg:								_TOK_BOTTOM_
										{}

arg:								_TOK_WIDTH_
										{}

arg:								_TOK_HEIGHT_
										{}


arg:								_TOK_IDENTIFIER_ '.' _TOK_LEFT_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

arg:								_TOK_IDENTIFIER_ '.' _TOK_RIGHT_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

arg:								_TOK_IDENTIFIER_ '.' _TOK_TOP_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

arg:								_TOK_IDENTIFIER_ '.' _TOK_BOTTOM_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

arg:								_TOK_IDENTIFIER_ '.' _TOK_WIDTH_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

arg:								_TOK_IDENTIFIER_ '.' _TOK_HEIGHT_
										{
												/* Look up the identifier in the element vector */
											$$ = 0;
											SAFE_FREE( $1 );
										}

%%
/*= U S E R = C O D E =========================================*/
