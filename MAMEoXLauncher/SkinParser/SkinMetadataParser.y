/**********************************************************
		SkinParser.y
	===========================
	Skin XML file parser

	-Meant to be compiled with "-p sp" to set proper prefix

**********************************************************/


/*= D E F I N I T I O N S =====================================*/
%{
	//= I N C L U D E S ====================================================
	#include "MAMEoX.h"
	#include "xbox_FileIO.h"
	#include "StdString.h"
	#include "FontSet.h"

	#include "Skin.h"
	#include "SkinPoint.h"
	#include "SkinDimension.h"
	#include "SkinColor.h"


	#include <Xtl.h>
	#include <malloc.h>
	#include <vector>
	#include <map>
	#include <list>
	#include <algorithm>


	//= D E F I N E S ======================================================
	#define CLEAN_AND_DIE()			SkinMetadataParserCleanup(); YYABORT;


	//= S T R U C T U R E S ================================================

	//= P R O T O T Y P E S ================================================
	void	smperror( const char * );
	void	smperror( const CStdString & );
	int		smplex( void );


	static void SkinMetadataParserCleanup( void );
	const char *SkinMetadataParserNewString( const char * );
	void SkinMetadataParserDeleteString( const char *str );

	//= G L O B A L = V A R S ==============================================
	static std::vector<const char *>									g_stringVars;

	//= F L E X = G E N E R A T E D ========================================
%}

	/* Type UNION */
%union {
    const char		*stringVal;
    int						intVal;
		void					*voidPtrVal;
}

%start start_symbol

	/* Token Declarations */

%token <stringVal>			_TOK_IDENTIFIER_
%token <stringVal>			_TOK_STRING_
%token <stringVal>			_TOK_MACROSET_
%token <stringVal>			_TOK_FILENAME_
%token <stringVal>			_TOK_INT_


%token <intVal>        SKIN_OPEN
%token <intVal>        SKIN_CLOSE

	/* Metadata */
%token <intVal>        METADATA_OPEN
%token <intVal>        METADATA_CLOSE


%token <intVal>        AUTHOR_OPEN
%token <intVal>        AUTHOR_CLOSE

%token <intVal>        NAME_OPEN
%token <intVal>        NAME_CLOSE

%token <intVal>        DESCRIPTION_OPEN
%token <intVal>        DESCRIPTION_CLOSE

%token <intVal>        PREVIEWFILE_OPEN
%token <intVal>        PREVIEWFILE_CLOSE


	/* Type Declarations */

/*= P R O D U C T I O N S =====================================*/
%%

	
start_symbol:				SKIN_OPEN skin SKIN_CLOSE
										{}


skin:								skin METADATA_OPEN metadata METADATA_CLOSE |
										/* empty */
										{}




	/*---------------- METADATA --------------------------------------------*/
metadata:						metadata AUTHOR_OPEN _TOK_STRING_ AUTHOR_CLOSE { g_parserSkinObject->m_author = $3; SkinMetadataParserDeleteString( $3 ); } |
										metadata NAME_OPEN	_TOK_STRING_ NAME_CLOSE { g_parserSkinObject->m_name = $3; SkinMetadataParserDeleteString( $3 ); } |
										metadata DESCRIPTION_OPEN _TOK_STRING_ DESCRIPTION_CLOSE { g_parserSkinObject->m_description = $3; SkinMetadataParserDeleteString( $3 ); } |
										metadata metadata_previewfile |
										/* empty */
										{}

metadata_previewfile:   PREVIEWFILE_OPEN _TOK_FILENAME_ PREVIEWFILE_CLOSE
												{
													if( !g_parserSkinObject->LoadPreviewTexture( $2 ) )
													{														 
														smperror( CStdString("Failed to load preview file ") + $2 );
														SkinMetadataParserDeleteString( $2 );
														CLEAN_AND_DIE();
													}

													SkinMetadataParserDeleteString( $2 );
												}

%%
/*= U S E R = C O D E =========================================*/

/*--------------------------------------------------------------------
	SkinMetadataParserCleanup
--------------------------------------------------------------------*/
static void SkinMetadataParserCleanup( void )
{
	for( std::vector<const char *>::iterator i = g_stringVars.begin(); i != g_stringVars.end(); ++i )
		free( (void*)(*i) );
	
	g_stringVars.clear();
}

/*--------------------------------------------------------------------
	SkinMetadataParserNewString
--------------------------------------------------------------------*/
const char *SkinMetadataParserNewString( const char *str )
{
	if( !str )
		return NULL;

	const char *ret = strdup( str );
	if( !ret )
	{
			// TODO: Handle OOM
		return NULL;
	}

	g_stringVars.push_back( ret );
	return ret;
}

/*--------------------------------------------------------------------
	SkinMetadataParserDeleteString
--------------------------------------------------------------------*/
void SkinMetadataParserDeleteString( const char *str )
{
	if( !str )
		return;

	std::vector<const char *>::iterator i = std::find( g_stringVars.begin(), g_stringVars.end(), str );
	if( i != g_stringVars.end() )
	{
		free( (void*)(*i) );
		g_stringVars.erase( i );
	}
}
