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
	#define CLEAN_AND_DIE()			SkinParserCleanup(); YYABORT;


	//= S T R U C T U R E S ================================================
	/**
	 * \class		CSkinParserResourceFile
	 * \brief   
	 */
	class CSkinParserResourceFile
	{
	public:
			//---------------------------------------------------
			//---------------------------------------------------
		CSkinParserResourceFile( void ) {
			m_colorChannelFilename = m_alphaChannelFilename = "";
		}

		std::vector<CSkinResource>		m_resources;
		CStdString										m_colorChannelFilename;
		CStdString										m_alphaChannelFilename;
	};


	//= P R O T O T Y P E S ================================================
	void	sperror( const char * );
	void	sperror( const CStdString & );
	int		splex( void );


	static void SkinParserCleanup( void );
	static BOOL	FontTypeFromName( const char *name, fonttype *ret );
	static BOOL SkinParserAddResourceFile( const CSkinParserResourceFile &element, CStdString *errorReport );
	static BOOL SkinParserAddElement( CSkinElement *element, CStdString *errorReport );


	const char *SkinParserNewString( const char * );
	void SkinParserDeleteString( const char *str );


	//= G L O B A L = V A R S ==============================================

	static CSkinParserResourceFile			*g_tempResourceFile = NULL;
	static CSkinResource								*g_tempResource = NULL;
	static CSkinRect										*g_tempRect = NULL;
	static CSkinPoint										*g_tempPoint = NULL;
	static CSkinDimension								*g_tempDimension = NULL;
	static CSkinColor										*g_tempColor = NULL;
	static CSkinText										*g_tempText = NULL;
	static CSkinScreen									*g_tempScreen = NULL;
	static CSkinIcon										*g_tempIcon = NULL;

  static std::map< CStdString, CSkinElement *>			g_namedElements;
  static std::vector<CSkinElement*>									g_unnamedElements;
	static std::vector<LPDIRECT3DTEXTURE8>						g_spriteTextureList;
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
%token <stringVal>			_TOK_MACRO_
%token <stringVal>			_TOK_MACRO_OPERATOR_
%token <stringVal>			_TOK_FILENAME_
%token <stringVal>			_TOK_INT_


%token <intVal>        SKIN_OPEN
%token <intVal>        SKIN_CLOSE

	/* Metadata */
%token <intVal>        METADATA_OPEN
%token <intVal>        METADATA_CLOSE

	/* Resource Definitions */
%token <intVal>        RESOURCEDEFINITIONS_OPEN
%token <intVal>        RESOURCEDEFINITIONS_CLOSE

	/* Screen widgets */
%token <intVal>        HELPSCREEN_OPEN
%token <intVal>        HELPSCREEN_CLOSE

%token <intVal>        OPTIONSSCREEN_OPEN
%token <intVal>        OPTIONSSCREEN_CLOSE

%token <intVal>        ROMLISTSCREEN_OPEN
%token <intVal>        ROMLISTSCREEN_CLOSE

%token <intVal>        SPLASHSCREEN_OPEN
%token <intVal>        SPLASHSCREEN_CLOSE

%token <intVal>        MESSAGESCREEN_OPEN
%token <intVal>        MESSAGESCREEN_CLOSE

	/* Popup widgets */
%token <intVal>        STARTMENU_OPEN
%token <intVal>        STARTMENU_CLOSE

%token <intVal>        VIRTUALKEYBOARD_OPEN
%token <intVal>        VIRTUALKEYBOARD_CLOSE

	/* Screensaver */
%token <intVal>        SCREENSAVER_OPEN
%token <intVal>        SCREENSAVER_CLOSE




%token <intVal>        AUTHOR_OPEN
%token <intVal>        AUTHOR_CLOSE

%token <intVal>        NAME_OPEN
%token <intVal>        NAME_CLOSE

%token <intVal>        DESCRIPTION_OPEN
%token <intVal>        DESCRIPTION_CLOSE

%token <intVal>        PREVIEWFILE_OPEN
%token <intVal>        PREVIEWFILE_CLOSE


%token <intVal>        RESOURCEFILE_OPEN
%token <intVal>        RESOURCEFILE_CLOSE

%token <intVal>        COLORCHANNELFILENAME_OPEN
%token <intVal>        COLORCHANNELFILENAME_CLOSE

%token <intVal>        ALPHACHANNELFILENAME_OPEN
%token <intVal>        ALPHACHANNELFILENAME_CLOSE

%token <intVal>        RESOURCE_OPEN
%token <intVal>        RESOURCE_CLOSE

%token <intVal>        ID_OPEN
%token <intVal>        ID_CLOSE

%token <intVal>        AREA_OPEN
%token <intVal>        AREA_CLOSE

%token <intVal>        LEFT_OPEN
%token <intVal>        LEFT_CLOSE

%token <intVal>        TOP_OPEN
%token <intVal>        TOP_CLOSE

%token <intVal>        RIGHT_OPEN
%token <intVal>        RIGHT_CLOSE

%token <intVal>        BOTTOM_OPEN
%token <intVal>        BOTTOM_CLOSE

%token <intVal>        BACKDROP_OPEN
%token <intVal>        BACKDROP_CLOSE

%token <intVal>        DEFAULTS_OPEN
%token <intVal>        DEFAULTS_CLOSE

%token <intVal>        TEXT_OPEN
%token <intVal>        TEXT_CLOSE

%token <intVal>        FONT_OPEN
%token <intVal>        FONT_CLOSE

%token <intVal>        COLOR_OPEN
%token <intVal>        COLOR_CLOSE

%token <intVal>        STRING_OPEN
%token <intVal>        STRING_CLOSE

%token <intVal>        CENTERVERTICALLY
%token <intVal>        CENTERHORIZONTALLY
%token <intVal>        UNBOUNDED

%token <intVal>        ALPHA_OPEN
%token <intVal>        ALPHA_CLOSE

%token <intVal>        RED_OPEN
%token <intVal>        RED_CLOSE

%token <intVal>        GREEN_OPEN
%token <intVal>        GREEN_CLOSE

%token <intVal>        BLUE_OPEN
%token <intVal>        BLUE_CLOSE

%token <intVal>        ARGB_OPEN
%token <intVal>        ARGB_CLOSE

%token <intVal>        ICON_OPEN
%token <intVal>        ICON_CLOSE

%token <intVal>        TEXTUREID_OPEN
%token <intVal>        TEXTUREID_CLOSE

%token <intVal>        POSITION_OPEN
%token <intVal>        POSITION_CLOSE

%token <intVal>        X_OPEN
%token <intVal>        X_CLOSE

%token <intVal>        Y_OPEN
%token <intVal>        Y_CLOSE

%token <intVal>        SIZE_OPEN
%token <intVal>        SIZE_CLOSE

%token <intVal>        WIDTH_OPEN
%token <intVal>        WIDTH_CLOSE

%token <intVal>        HEIGHT_OPEN
%token <intVal>        HEIGHT_CLOSE

%token <intVal>        CONDITION_OPEN
%token <intVal>        CONDITION_CLOSE


	/* Type Declarations */
%type <stringVal>			TEXTSTRING
%type <stringVal>			helper_condition

%type <voidPtrVal>		helper_area
%type <voidPtrVal>		helper_position
%type <voidPtrVal>		helper_size
%type <voidPtrVal>		helper_color
%type <voidPtrVal>		helper_text
%type <voidPtrVal>		helper_icon


/*= P R O D U C T I O N S =====================================*/
%%

	
start_symbol:				SKIN_OPEN skin SKIN_CLOSE
										{}


  /* Special text string rule, accepts any identifier and converts it to a text string */
TEXTSTRING:							_TOK_IDENTIFIER_ | _TOK_STRING_ | _TOK_FILENAME_ | _TOK_MACRO_ | _TOK_MACRO_OPERATOR_ | _TOK_INT_
												{
													$$ = $1;
												}


skin:								skin METADATA_OPEN metadata METADATA_CLOSE |
										skin RESOURCEDEFINITIONS_OPEN resourcefiles RESOURCEDEFINITIONS_CLOSE |
										skin helpscreen |
										skin optionsscreen |
										skin romlistscreen |
										skin splashscreen |
										skin messagescreen |
										skin startmenu |
										skin virtualkeyboard |
										skin screensaver |
										/* empty */
										{}




	/*---------------- METADATA --------------------------------------------*/
metadata:						metadata AUTHOR_OPEN TEXTSTRING AUTHOR_CLOSE { g_parserSkinObject->m_author = $3; SkinParserDeleteString( $3 ); } |
										metadata NAME_OPEN	TEXTSTRING NAME_CLOSE { g_parserSkinObject->m_name = $3; SkinParserDeleteString( $3 ); } |
										metadata DESCRIPTION_OPEN TEXTSTRING DESCRIPTION_CLOSE { g_parserSkinObject->m_description = $3; SkinParserDeleteString( $3 ); } |
										metadata metadata_previewfile |
										/* empty */
										{}

metadata_previewfile:   PREVIEWFILE_OPEN TEXTSTRING PREVIEWFILE_CLOSE
												{
													if( !g_parserSkinObject->LoadPreviewTexture( $2 ) )
													{														 
														sperror( CStdString("Failed to load preview file ") + $2 );
														SkinParserDeleteString( $2 );
														CLEAN_AND_DIE();
													}
												}												




	/*--------------- Resources --------------------------------------------*/
resourcefiles:			resourcefiles RESOURCEFILE_OPEN { g_tempResourceFile = new CSkinParserResourceFile(); } resourcefile RESOURCEFILE_CLOSE |
										/* empty */
										{
											if( g_tempResourceFile )
											{
												CStdString errorReport;
												if( !SkinParserAddResourceFile( *g_tempResourceFile, &errorReport ) )
												{
													sperror( errorReport );
													SAFE_DELETE( g_tempResourceFile );
													CLEAN_AND_DIE();
												}
												SAFE_DELETE( g_tempResourceFile );
											}
										}
									
resourcefile:				resourcefile COLORCHANNELFILENAME_OPEN _TOK_FILENAME_ COLORCHANNELFILENAME_CLOSE { g_tempResourceFile->m_colorChannelFilename = $3; SkinParserDeleteString( $3 ); } |
										resourcefile ALPHACHANNELFILENAME_OPEN _TOK_FILENAME_ ALPHACHANNELFILENAME_CLOSE { g_tempResourceFile->m_alphaChannelFilename = $3; SkinParserDeleteString( $3 ); } |
										resourcefile resource |
										/* empty */
										{
										}

resource:						RESOURCE_OPEN { g_tempResource = new CSkinResource(); } resourcecontents RESOURCE_CLOSE
										{
											if( g_tempResourceFile )
												g_tempResourceFile->m_resources.push_back( *g_tempResource );
											SAFE_DELETE( g_tempResource );
										}

resourcecontents:		resourceID resourceArea |
										resourceArea resourceID
										{
											/* Both tags are required, but order independent */
										}

resourceID:					ID_OPEN _TOK_IDENTIFIER_ ID_CLOSE
										{
											g_tempResource->m_ID = $2;
											SkinParserDeleteString( $2 );
										}
	
resourceArea:				helper_area
										{
											g_tempResource->m_area = *((CSkinRect*)($1));
											SAFE_DELETE( $1 );
										}

	/*--------------- Help Screen ------------------------------------------*/
helpscreen:					HELPSCREEN_OPEN { g_tempScreen = new CSkinScreen( HELPSCREEN_ID ); } screen HELPSCREEN_CLOSE
										{
											CStdString errorReport = "";
											if( !SkinParserAddElement( g_tempScreen, &errorReport ) )
											{
												sperror( errorReport );
												SAFE_DELETE( g_tempScreen );
												CLEAN_AND_DIE();
											}
										}

optionsscreen:			OPTIONSSCREEN_OPEN garbage OPTIONSSCREEN_CLOSE
										{}

romlistscreen:			ROMLISTSCREEN_OPEN garbage ROMLISTSCREEN_CLOSE
										{}

splashscreen:				SPLASHSCREEN_OPEN garbage SPLASHSCREEN_CLOSE
										{}

messagescreen:			MESSAGESCREEN_OPEN garbage MESSAGESCREEN_CLOSE
										{}

startmenu:					STARTMENU_OPEN garbage STARTMENU_CLOSE
										{}

virtualkeyboard:		VIRTUALKEYBOARD_OPEN garbage VIRTUALKEYBOARD_CLOSE
										{}

screensaver:				SCREENSAVER_OPEN garbage SCREENSAVER_CLOSE
										{}



garbage:						/* empty */ 
										{}

screen:							screen screendefaults |
										screen helper_icon		{ CStdString errorReport = ""; if( !g_tempScreen->AddElement( *((CSkinIcon*)$2), &errorReport ) ) { sperror( errorReport ); SAFE_DELETE( $2 ); CLEAN_AND_DIE(); } SAFE_DELETE( $2 ); } |
										screen helper_text		{ CStdString errorReport = ""; if( !g_tempScreen->AddElement( *((CSkinText*)$2), &errorReport ) ) { sperror( errorReport ); SAFE_DELETE( $2 ); CLEAN_AND_DIE(); } SAFE_DELETE( $2 ); } |
										/* empty */
										{}


screendefaults:			DEFAULTS_OPEN helper_text DEFAULTS_CLOSE	{ g_tempScreen->m_defaultTextAttributes = *((CSkinText*)$2); SAFE_DELETE( $2 ); }


	/*--------------- General purpose helpers ------------------------------------------*/

				/* <Area> parser */
helper_area:				AREA_OPEN { g_tempRect = new CSkinRect(); } areacontents AREA_CLOSE
										{
											$$ = g_tempRect;
											g_tempRect = NULL;
										}

areacontents:				areacontents LEFT_OPEN _TOK_INT_ LEFT_CLOSE { g_tempRect->m_left = $3; SkinParserDeleteString( $3 ); } |
										areacontents TOP_OPEN _TOK_INT_ TOP_CLOSE { g_tempRect->m_top = $3; SkinParserDeleteString( $3 ); } |
										areacontents RIGHT_OPEN _TOK_INT_ RIGHT_CLOSE { g_tempRect->m_right = $3; SkinParserDeleteString( $3 ); } |
										areacontents BOTTOM_OPEN _TOK_INT_ BOTTOM_CLOSE { g_tempRect->m_bottom = $3; SkinParserDeleteString( $3 ); } |
										/* empty */
										{}


				/* <Position> parser */
helper_position:		POSITION_OPEN { g_tempPoint = new CSkinPoint(); } positioncontents POSITION_CLOSE
										{
											$$ = g_tempPoint;
											g_tempPoint = NULL;
										}

positioncontents:		positioncontents X_OPEN _TOK_INT_ X_CLOSE { g_tempPoint->m_x = $3; SkinParserDeleteString( $3 ); } |
										positioncontents Y_OPEN _TOK_INT_ Y_CLOSE { g_tempPoint->m_y = $3; SkinParserDeleteString( $3 ); } |
										/* empty */
										{}


				/* <Size> parser */
helper_size:				SIZE_OPEN { g_tempDimension = new CSkinDimension(); } sizecontents SIZE_CLOSE
										{
											$$ = g_tempDimension;
											g_tempDimension = NULL;
										}

sizecontents:				sizecontents WIDTH_OPEN _TOK_INT_ WIDTH_CLOSE		{ g_tempDimension->m_width = $3; SkinParserDeleteString( $3 ); } |
										sizecontents HEIGHT_OPEN _TOK_INT_ HEIGHT_CLOSE { g_tempDimension->m_height = $3; SkinParserDeleteString( $3 ); } |
										/* empty */
										{}



				/* <Color> parser */
helper_color:				COLOR_OPEN { g_tempColor = new CSkinColor(); } colorcontents COLOR_CLOSE
										{
											$$ = g_tempColor;
											g_tempColor = NULL;
										}

colorcontents:			colorcontents ALPHA_OPEN _TOK_INT_ ALPHA_CLOSE	{ g_tempColor->m_alpha = $3; SkinParserDeleteString( $3 ); } |
										colorcontents RED_OPEN _TOK_INT_ RED_CLOSE			{ g_tempColor->m_red = $3; SkinParserDeleteString( $3 ); } |
										colorcontents GREEN_OPEN _TOK_INT_ GREEN_CLOSE	{ g_tempColor->m_green = $3; SkinParserDeleteString( $3 ); } |
										colorcontents BLUE_OPEN _TOK_INT_ BLUE_CLOSE		{ g_tempColor->m_blue = $3; SkinParserDeleteString( $3 ); } |
										/* empty */
										{}

				/* <Condition> parser */
helper_condition:		CONDITION_OPEN TEXTSTRING CONDITION_CLOSE
										{ 
											$$ = $2;
										}

				/* <Text> parser */
helper_text:				TEXT_OPEN { g_tempText = new CSkinText(); } textcontents TEXT_CLOSE
										{
											$$ = g_tempText;
											g_tempText = NULL;
										}

textcontents:				textcontents FONT_OPEN _TOK_IDENTIFIER_ FONT_CLOSE	{ if( !FontTypeFromName( $3, &g_tempText->m_fontType ) ) { sperror( CStdString("Invalid font type ") + $3 ); SkinParserDeleteString( $3 ); CLEAN_AND_DIE(); } SkinParserDeleteString( $3 ); } |
										textcontents STRING_OPEN _TOK_STRING_ STRING_CLOSE	{ g_tempText->m_string = $3; SkinParserDeleteString( $3 ); } |
										textcontents helper_color														{ g_tempText->m_color = *((CSkinColor*)$2); SAFE_DELETE( $2 ); } |
										textcontents helper_condition												{ g_tempText->m_condition = $2; SkinParserDeleteString( $2 ); } |
										textcontents CENTERVERTICALLY												{ g_tempText->m_fontFlags |= XBFONT_CENTER_Y; } |
										textcontents CENTERHORIZONTALLY											{ g_tempText->m_fontFlags |= XBFONT_CENTER_X; } |
										textcontents UNBOUNDED															{ g_tempText->m_fontFlags &= ~XBFONT_TRUNCATED; } |
										/* empty */
										{}


				/* <Icon> parser */
helper_icon:				ICON_OPEN { g_tempIcon = new CSkinIcon(); } iconcontents ICON_CLOSE
										{
											$$ = g_tempIcon;
											g_tempIcon = NULL;
										}

iconcontents:				iconcontents TEXTUREID_OPEN _TOK_IDENTIFIER_ TEXTUREID_CLOSE	{	g_tempIcon->m_textureID = $3; SkinParserDeleteString( $3 ); } |
										iconcontents helper_position																	{ g_tempIcon->m_area.m_left = ((CSkinPoint*)$2)->m_x; g_tempIcon->m_area.m_top = ((CSkinPoint*)$2)->m_y; SAFE_DELETE( $2 ); } |
										iconcontents helper_size																			{ g_tempIcon->m_area.m_right = CStdString("x+") + (CStdString)((CSkinDimension*)$2)->m_width; g_tempIcon->m_area.m_bottom = CStdString("y+") + (CStdString)((CSkinDimension*)$2)->m_height; SAFE_DELETE( $2 ); } |
										iconcontents helper_area																			{ g_tempIcon->m_area = *((CSkinRect*)$2); SAFE_DELETE( $2 ); } |
										iconcontents helper_condition																	{ g_tempIcon->m_condition = $2; SkinParserDeleteString( $2 ); } |
										/* empty */
										{}

%%
/*= U S E R = C O D E =========================================*/

/*--------------------------------------------------------------------
	SkinParserCleanup
--------------------------------------------------------------------*/
static void SkinParserCleanup( void )
{
	for( std::vector<LPDIRECT3DTEXTURE8>::iterator i = g_spriteTextureList.begin(); i != g_spriteTextureList.end(); ++i )
		SAFE_RELEASE( (*i) );

	for( std::map< CStdString, CSkinElement *>::iterator i = g_namedElements.begin(); i != g_namedElements.end(); ++i )
		SAFE_DELETE( (*i).second );

	for( std::vector<CSkinElement*>::iterator i = g_unnamedElements.begin(); i != g_unnamedElements.end(); ++i )
		SAFE_DELETE( (*i) );

	for( std::vector<const char *>::iterator i = g_stringVars.begin(); i != g_stringVars.end(); ++i )
		free( (void*)(*i) );
	
	g_spriteTextureList.clear();
	g_namedElements.clear();
	g_unnamedElements.clear();
	g_stringVars.clear();

	SAFE_DELETE( g_tempResourceFile );
	SAFE_DELETE( g_tempResource );
	SAFE_DELETE( g_tempRect );
	SAFE_DELETE( g_tempPoint );
	SAFE_DELETE( g_tempDimension );
	SAFE_DELETE( g_tempColor );
	SAFE_DELETE( g_tempText );
	SAFE_DELETE( g_tempScreen );
	SAFE_DELETE( g_tempIcon );
}

/*--------------------------------------------------------------------
	SkinParserNewString
--------------------------------------------------------------------*/
const char *SkinParserNewString( const char *str )
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
	SkinParserDeleteString
--------------------------------------------------------------------*/
void SkinParserDeleteString( const char *str )
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

/*--------------------------------------------------------------------
	SkinParserAddResourceFile
--------------------------------------------------------------------*/
static BOOL SkinParserAddResourceFile( const CSkinParserResourceFile &element, CStdString *errorReport )
{
	if( element.m_colorChannelFilename == "" )
	{
		*errorReport = "Missing color channel filename";
		return FALSE;
	}

	UINT32 textureID = g_spriteTextureList.size();
	LPDIRECT3DTEXTURE8 t = g_parserSkinObject->LoadSkinTexture( element.m_colorChannelFilename, element.m_alphaChannelFilename );
	if( !t )
	{
		*errorReport = "Failed to load texture. Filename: ";
		*errorReport += element.m_colorChannelFilename;
		return FALSE;
	}
	g_spriteTextureList.push_back( t );


	std::vector<CSkinResource>::const_iterator i = element.m_resources.begin();
	for( ; i != element.m_resources.end(); ++i )
	{
		if( (*i).m_ID == "" )
		{
			*errorReport = "Missing ID for resource!";
			return FALSE;
		}

	 if( g_namedElements.find( (*i).m_ID ) != g_namedElements.end() )
		{
			*errorReport = "Name conflict with ID ";
			*errorReport += (*i).m_ID;
			return FALSE;
		}

		CSkinResource *res = new CSkinResource( *i );
		if( !res )
		{
			*errorReport = "Out of memory copying Resource with ID ";
			*errorReport += (*i).m_ID;
			return FALSE;
		}

		res->m_textureID = textureID;
		g_namedElements[(*i).m_ID] = res;
	}

	return TRUE;
}


/*--------------------------------------------------------------------
	SkinParserAddElement
--------------------------------------------------------------------*/
static BOOL SkinParserAddElement( CSkinElement *element, CStdString *errorReport )
{
	if( element->m_ID == "" )
		g_unnamedElements.push_back( element );
	else
	{
		if( g_namedElements.find( element->m_ID ) != g_namedElements.end() )
		{
			*errorReport = "Name conflict with ID ";
			*errorReport += element->m_ID;
			return FALSE;
		}

		g_namedElements[element->m_ID] = element;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
	FontTypeFromName
--------------------------------------------------------------------*/
static BOOL	FontTypeFromName( const char *name, fonttype *ret )
{
	if( !stricmp( name, "default" ) || !stricmp( name, "standard" ) )
		*ret = FONTTYPE_DEFAULT;
	else if( !stricmp( name, "fixedwidth" ) )
		*ret = FONTTYPE_FIXEDWIDTH;
	else if( !stricmp( name, "largethin" ) )
		*ret = FONTTYPE_LARGETHIN;
	else if( !stricmp( name, "smallthin" ) )
		*ret = FONTTYPE_SMALLTHIN;

	return FALSE;
}
