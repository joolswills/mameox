/**
	* \file			SkinResource.cpp
	* \brief		CSkinResource class
	*
	*/


//= I N C L U D E S ====================================================
#include "SkinResource.h"

#include "BaseView.h"		// For the PNG file functions
#include "DebugLogger.h"

#include "GraphicsManager.h"
#include "FontSet.h"

#include "xbox_FileIO.h"
#include "xbox_Network.h"


//= D E F I N E S =====================================================
#define SKININIFILENAME       "skin.ini"

	// Enum to give the g_skinINISections some friendly indices
typedef enum SkinINISectionID_t {
	SECTION_SPLASH_BACKDROP,
	SECTION_MESSAGE_BACKDROP,
	SECTION_LIST_BACKDROP,
	SECTION_TVCALIBRATION_SPRITES,
	SECTION_LIGHTGUNCALIBRATION_SPRITES,
	SECTION_LIST_SPRITES,
	SECTION_MENU_SPRITES,
	SECTION_BUTTON_SPRITES,

	SECTION_POPUP_OPTIONS,
	SECTION_HELPSCREEN_OPTIONS,
	SECTION_OPTIONSSCREEN_OPTIONS,
	SECTION_ROMLISTSCREEN_OPTIONS,
	SECTION_SCREENSAVER_OPTIONS,
	SECTION_SKINCHOOSERSCREEN_OPTIONS,
	SECTION_STARTMENU_OPTIONS,
	SECTION_VIRTUALKEYBOARD_OPTIONS,
	SECTION_MESSAGESCREEN_OPTIONS,
	SECTION_SPLASHSCREEN_OPTIONS,

	SECTION_INVALID = 0x7FFFFFFF
} SkinINISectionID_t;



//= G L O B A L = V A R S =============================================
	// The currently loaded skin (managed by SkinChooserScreen)
CSkinResource							*g_loadedSkin = NULL;


	// Array of INI sections containing sprites
const char *g_skinINISections[] = { "SplashBackdrop",							// SECTION_SPLASH_BACKDROP
																		"MessageBackdrop",						// SECTION_MESSAGE_BACKDROP
																		"ListBackdrop",								// SECTION_LIST_BACKDROP
																		"TVCalibrationSprites", 			// SECTION_TVCALIBRATION_SPRITES
																		"LightgunCalibrationSprites", // SECTION_LIGHTGUNCALIBRATION_SPRITES
																		"ListSprites",								// SECTION_LIST_SPRITES
																		"MenuSprites",								// SECTION_MENU_SPRITES
																		"ButtonSprites",							// SECTION_BUTTON_SPRITES
																		"PopupOptions",								// SECTION_POPUP_OPTIONS
																		"HelpScreenOptions", 					// SECTION_HELPSCREEN_OPTIONS
																		"OptionsScreenOptions",				// SECTION_OPTIONSSCREEN_OPTIONS
																		"ROMListScreenOptions",				// SECTION_ROMLISTSCREEN_OPTIONS
																		"ScreensaverOptions",					// SECTION_SCREENSAVER_OPTIONS
																		"SkinChooserScreenOptions",		// SECTION_SKINCHOOSERSCREEN_OPTIONS
																		"StartMenuOptions",						// SECTION_STARTMENU_OPTIONS
																		"VirtualKeyboardOptions",			// SECTION_VIRTUALKEYBOARD_OPTIONS
																		"MessageScreenOptions",				// SECTION_MESSAGESCREEN_OPTIONS
																		"SplashScreenOptions"					// SECTION_SPLASHSCREEN_OPTIONS
};


	// Map of SkinResourceID_t's to their name in the skin.ini file 
static const char *g_resourceIDToINIEntry[] = {
	"Backdrop",									// ASSET_SPLASH_BACKDROP

	"Backdrop",									// ASSET_MESSAGE_BACKDROP

	"Backdrop",									// ASSET_LIST_BACKDROP

	"Cursor",										// SPRITE_TVCALIBRATION_CURSOR

	"Cursor",										// SPRITE_LIGHTGUNCALIBRATION_CURSOR
	"Target",										// SPRITE_LIGHTGUNCALIBRATION_TARGET

	"ScrollIcon.Down",					// SPRITE_LIST_SCROLLICON_DOWN
	"ScrollIcon.Up",						// SPRITE_LIST_SCROLLICON_UP
	"TriggerIcon.Head",					// SPRITE_LIST_TRIGGERICON_HEAD
	"TriggerIcon.Center",				// SPRITE_LIST_TRIGGERICON_CENTER
	"TriggerIcon.Tail",					// SPRITE_LIST_TRIGGERICON_TAIL

	"TitleBar.LeftSide",				// SPRITE_MENU_TITLEBAR_LEFTSIDE
	"TitleBar.Center",					// SPRITE_MENU_TITLEBAR_CENTER
	"TitleBar.RightSide",				// SPRITE_MENU_TITLEBAR_RIGHTSIDE
	"Body.LeftSide",						// SPRITE_MENU_BODY_LEFTSIDE
	"Body.Center",							// SPRITE_MENU_BODY_CENTER
	"Body.RightSide",						// SPRITE_MENU_BODY_RIGHTSIDE
	"Footer.LeftSide",					// SPRITE_MENU_FOOTER_LEFTSIDE
	"Footer.Center",						// SPRITE_MENU_FOOTER_CENTER
	"Footer.RightSide",					// SPRITE_MENU_FOOTER_RIGHTSIDE

	"A",												// SPRITE_BUTTON_A
	"B",												// SPRITE_BUTTON_B
	"X",												// SPRITE_BUTTON_X
	"Y",												// SPRITE_BUTTON_Y
	"Black",										// SPRITE_BUTTON_BLACK
	"White",										// SPRITE_BUTTON_WHITE
	"Start",										// SPRITE_BUTTON_START
	"Back",											// SPRITE_BUTTON_BACK
	"LeftTrigger",							// SPRITE_BUTTON_LEFTTRIGGER
	"RightTrigger",							// SPRITE_BUTTON_RIGHTTRIGGER
	"LeftAnalog",								// SPRITE_BUTTON_LEFTANALOG
	"RightAnalog"								// SPRITE_BUTTON_RIGHTANALOG
};


	// Map of SkinColorID_t's to their name in the skin.ini file 
static const char *g_colorIDToINIEntry[] = {
	"DarkenOverlay.Color",							// COLOR_POPUP_DARKENOVERLAY

	"TitleBar.TextColor",								// COLOR_HELPSCREEN_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_HELPSCREEN_BODY_TEXT
	"ButtonLabels.Color",								// COLOR_HELPSCREEN_BUTTONICON_TEXT
 
	"TitleBar.TextColor",								// COLOR_OPTIONSSCREEN_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_OPTIONSSCREEN_BODY_TEXT
	"Body.HighlightBarColor",						// COLOR_OPTIONSSCREEN_BODY_HIGHLIGHTBAR
	"Body.DividerColor",								// COLOR_OPTIONSSCREEN_BODY_DIVIDER
	"Body.TriggerIcon.TextColor",				// COLOR_OPTIONSSCREEN_BODY_TRIGGERICON_TEXT
	"ButtonLabels.Color",								// COLOR_OPTIONSSCREEN_BUTTONICON_TEXT

	"TitleBar.TextColor",								// COLOR_ROMLISTSCREEN_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_ROMLISTSCREEN_BODY_TEXT
	"Body.ROMWorkingTextColor",					// COLOR_ROMLISTSCREEN_ROMWORKING_TEXT
	"Body.ROMWarningTextColor",					// COLOR_ROMLISTSCREEN_ROMWARNING_TEXT
	"Body.ROMNonworkingTextColor",			// COLOR_ROMLISTSCREEN_ROMNONWORKING_TEXT
	"Body.HighlightBarColor",						// COLOR_ROMLISTSCREEN_BODY_HIGHLIGHTBAR
	"Body.DividerColor",								// COLOR_ROMLISTSCREEN_BODY_DIVIDER
	"Body.Screenshot.BackgroundColor",	// COLOR_ROMLISTSCREEN_SCREENSHOT_BACKGROUND
	"Body.Screenshot.TextColor",				// COLOR_ROMLISTSCREEN_SCREENSHOT_TEXT
	"ButtonLabels.Color",								// COLOR_ROMLISTSCREEN_BUTTONICON_TEXT

	"TextColor",												// COLOR_SCREENSAVER_TEXT

	"TitleBar.TextColor",								// COLOR_SKINCHOOSERSCREEN_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_SKINCHOOSERSCREEN_BODY_TEXT
	"Body.HighlightBarColor",						// COLOR_SKINCHOOSERSCREEN_BODY_HIGHLIGHTBAR
	"Body.DividerColor",								// COLOR_SKINCHOOSERSCREEN_BODY_DIVIDER
	"Body.Screenshot.BackgroundColor",	// COLOR_SKINCHOOSERSCREEN_SCREENSHOT_BACKGROUND
	"Body.Screenshot.TextColor",				// COLOR_SKINCHOOSERSCREEN_SCREENSHOT_TEXT
	"ButtonLabels.Color",								// COLOR_SKINCHOOSERSCREEN_BUTTONICON_TEXT

	"TitleBar.TextColor",								// COLOR_STARTMENU_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_STARTMENU_BODY_TEXT
	"Body.HighlightBarColor",						// COLOR_STARTMENU_BODY_HIGHLIGHTBAR

	"TitleBar.TextColor",								// COLOR_VIRTUALKEYBOARD_TITLEBAR_TEXT
	"Body.TextColor",										// COLOR_VIRTUALKEYBOARD_BODY_TEXT
	"Body.HighlightBarColor",						// COLOR_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR

	"TextColor",												// COLOR_MESSAGESCREEN_TEXT
	"ProgressBar.EndColor",							// COLOR_MESSAGESCREEN_PROGRESSBAR_END
	"ProgressBar.BarColor",							// COLOR_MESSAGESCREEN_PROGRESSBAR_BAR
	"ProgressBar.BackgroundColor",			// COLOR_MESSAGESCREEN_PROGRESSBAR_BACKGROUND

	"TextColor",												// COLOR_SPLASHSCREEN_TEXT
	"ImportantTextColor"								// COLOR_SPLASHSCREEN_IMPORTANT_TEXT

};


static const char *g_argbColorDelimiters = ";:., ";

//= P R O T O T Y P E S ===============================================
static const char *ResourceIDToINISection( SkinResourceID_t idToMap );
static const char *ColorIDToINISection( SkinColorID_t idToMap );
static D3DCOLOR StringToColor( const CStdString &argbString );

//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	LoadSkin
//---------------------------------------------------------------------
BOOL CSkinResource::LoadSkin( CStdString *errorReport )
{
  CStdString iniFileName = m_skinName;
  iniFileName += "\\" SKININIFILENAME;
  
  CStdString fullIniFilePath = g_FileIOConfig.m_skinPath;
  fullIniFilePath += "\\" + iniFileName;


	PRINTMSG(( T_INFO, "LoadSkin from INI %s", fullIniFilePath.c_str() ));

    // Make sure there's a skin.ini file
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, fullIniFilePath.c_str(), "r" );
  if( !file )
  {
    if( errorReport )
      *errorReport = "Missing \"" SKININIFILENAME "\"!";

		PRINTMSG(( T_ERROR, "Failed to load INI file %s!", fullIniFilePath.c_str() ));
    return FALSE;
  }
  osd_fclose( file );


    // Parse the INI file
  CSystem_IniFile iniFile( fullIniFilePath );

		// Grab the resource files
	m_splashBackdropTexture = LoadSkinTexture(	iniFile, 
																							g_skinINISections[SECTION_SPLASH_BACKDROP], 
																							"ResourcesColorChannelFilename", 
																							"" );

	m_messageBackdropTexture = LoadSkinTexture(	iniFile, 
																							g_skinINISections[SECTION_MESSAGE_BACKDROP], 
																							"ResourcesColorChannelFilename", 
																							"" );

	m_listBackdropTexture = LoadSkinTexture(	iniFile, 
																							g_skinINISections[SECTION_LIST_BACKDROP], 
																							"ResourcesColorChannelFilename", 
																							"" );


	m_TVCalibrationSpritesTexture = LoadSkinTexture(	iniFile, 
																										g_skinINISections[SECTION_TVCALIBRATION_SPRITES], 
																										"ResourcesColorChannelFilename", 
																										"ResourcesAlphaChannelFilename" );

	m_lightgunCalibrationSpritesTexture	= LoadSkinTexture(	iniFile, 
																													g_skinINISections[SECTION_LIGHTGUNCALIBRATION_SPRITES], 
																													"ResourcesColorChannelFilename", 
																													"ResourcesAlphaChannelFilename" );

	m_listSpritesTexture = LoadSkinTexture(	iniFile, 
																					g_skinINISections[SECTION_LIST_SPRITES], 
																					"ResourcesColorChannelFilename", 
																					"ResourcesAlphaChannelFilename" );

	m_menuSpritesTexture = LoadSkinTexture( iniFile, 
																					g_skinINISections[SECTION_MENU_SPRITES], 
																					"ResourcesColorChannelFilename", 
																					"ResourcesAlphaChannelFilename" );

	m_buttonSpritesTexture = LoadSkinTexture( iniFile, 
																						g_skinINISections[SECTION_BUTTON_SPRITES], 
																						"ResourcesColorChannelFilename", 
																						"ResourcesAlphaChannelFilename" );


	for( int i = 0; i < RESOURCE_COUNT; ++i )
	{
		SkinResourceInfo_t tempInfo;
		CStdString sectionName = ResourceIDToINISection( (SkinResourceID_t)i );
		
		CStdString entryName = g_resourceIDToINIEntry[i];
		entryName += ".FileOffset.";

		tempInfo.m_left		= iniFile.GetProfileInt( sectionName, entryName + "left", -1 );
		tempInfo.m_right	= iniFile.GetProfileInt( sectionName, entryName + "right", -1 );
		tempInfo.m_top		= iniFile.GetProfileInt( sectionName, entryName + "top", -1 );
		tempInfo.m_bottom = iniFile.GetProfileInt( sectionName, entryName + "bottom", -1 );

		PRINTMSG(( T_INFO, "Sprite %s:%s = (%f,%f)-(%f,%f)", sectionName.c_str(), entryName.c_str(), tempInfo.m_left, tempInfo.m_top, tempInfo.m_right, tempInfo.m_bottom ));

		if( tempInfo.m_left != -1.0f && tempInfo.m_right != -1.0f && tempInfo.m_top != -1.0f && tempInfo.m_bottom != -1.0f )
			m_spriteInfoArray[i] = new SkinResourceInfo_t( tempInfo );
	}

	for( int i = 0; i < COLOR_COUNT; ++i )
	{
		CStdString sectionName = ColorIDToINISection( (SkinColorID_t)i );
		CStdString entryName = g_colorIDToINIEntry[i];

		m_spriteColorArray[i] = StringToColor( iniFile.GetProfileString( sectionName, entryName, "255 0 0 0" ) );
		PRINTMSG(( T_INFO, "Color %s:%s = 0x%X", sectionName.c_str(), entryName.c_str(), m_spriteColorArray[i] ));
	}

	return TRUE;
}


//---------------------------------------------------------------------
//	LoadSkinTexture
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE8 CSkinResource::LoadSkinTexture(	CSystem_IniFile &iniFile,
																										const CStdString &sectionName, 
																										const CStdString &colorChannelEntry, 
																										const CStdString &alphaChannelEntry )
{
	CStdString textureColorFilename, textureAlphaFilename = "";
	textureColorFilename	= iniFile.GetProfileString( sectionName, colorChannelEntry, "" );

	if( alphaChannelEntry.size() )
		textureAlphaFilename	= iniFile.GetProfileString( sectionName, alphaChannelEntry, "" );

	PRINTMSG(( T_INFO, "Load texture at %s:%s[%s]", sectionName.c_str(), colorChannelEntry.c_str(), alphaChannelEntry.c_str() ));
	PRINTMSG(( T_INFO, "Files resolved to %s[%s]", textureColorFilename.c_str(), textureAlphaFilename.c_str() ));


		// Don't continue at all if there's no color channel
	if( textureColorFilename == "" )
		return NULL;

  CStdString fullResourceFilePath = g_FileIOConfig.m_skinPath;
	fullResourceFilePath += "\\" + m_skinName + "\\";

	PNGFile_t colorFileData;
	if( !LoadPNGFile( fullResourceFilePath + textureColorFilename, &colorFileData ) )
	{
		PRINTMSG(( T_ERROR, "Failed to load color channel for texture %s:%s[%s]!", sectionName.c_str(), colorChannelEntry.c_str(), alphaChannelEntry.c_str() ));
		return NULL;
	}

		// If we don't have an alpha file, just create an XRGB and return
	BYTE *alphaChannel = NULL;
	if( textureAlphaFilename != "" )
	{
		PNGFile_t alphaFileData;
		if( !LoadPNGFile( fullResourceFilePath + textureAlphaFilename, &alphaFileData ) )
		{
				// If the load fails, just print a warning and load as XRGB
			PRINTMSG(( T_ERROR, "Failed to load alpha channel for texture %s:%s[%s]! Creating as XRGB instead...", sectionName.c_str(), colorChannelEntry.c_str(), alphaChannelEntry.c_str() ));
		}
		else
		{
			CreateAlphaChannelFromPNGFile( alphaFileData, &alphaChannel );
			ClosePNGFile( alphaFileData );
		}
	}

	LPDIRECT3DTEXTURE8 ret = CreateTextureFromPNGFile( colorFileData, alphaChannel );
	ClosePNGFile( colorFileData );
	if( alphaChannel )
		free( alphaChannel );

	PRINTMSG(( T_INFO, "Texture created: 0x%X", ret ));

	return ret;		
}


//---------------------------------------------------------------------
//	ResourceIDToSectionID
//---------------------------------------------------------------------
inline SkinINISectionID_t ResourceIDToSectionID( SkinResourceID_t idToMap )
{
	if( idToMap == ASSET_SPLASH_BACKDROP )
		return SECTION_SPLASH_BACKDROP;

	if( idToMap == ASSET_MESSAGE_BACKDROP )
		return SECTION_MESSAGE_BACKDROP;

	if( idToMap == ASSET_LIST_BACKDROP )
		return SECTION_LIST_BACKDROP;

	if( idToMap == SPRITE_TVCALIBRATION_CURSOR )
		return SECTION_TVCALIBRATION_SPRITES;

	if( idToMap >= SPRITE_LIGHTGUNCALIBRATION_CURSOR && idToMap <= SPRITE_LIGHTGUNCALIBRATION_TARGET )
		return SECTION_LIGHTGUNCALIBRATION_SPRITES;

	if( idToMap >= SPRITE_LIST_SCROLLICON_DOWN && idToMap <= SPRITE_LIST_TRIGGERICON_TAIL )
		return SECTION_LIST_SPRITES;

	if( idToMap >= SPRITE_MENU_TITLEBAR_LEFTSIDE && idToMap <= SPRITE_MENU_FOOTER_RIGHTSIDE )
		return SECTION_MENU_SPRITES;

	if( idToMap >= SPRITE_BUTTON_A && idToMap <= SPRITE_BUTTON_RIGHTANALOG )
		return SECTION_BUTTON_SPRITES;

	return SECTION_INVALID;
}


//---------------------------------------------------------------------
//	GetSkinResourceTexture
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE8 CSkinResource::GetSkinResourceTexture( SkinResourceID_t id ) const
{
	switch( ResourceIDToSectionID( id ) )
	{
	case SECTION_SPLASH_BACKDROP:
		return m_splashBackdropTexture;

	case SECTION_MESSAGE_BACKDROP:
		return m_messageBackdropTexture;

	case SECTION_LIST_BACKDROP:
		return m_listBackdropTexture;

	case SECTION_TVCALIBRATION_SPRITES:
		return m_TVCalibrationSpritesTexture;

	case SECTION_LIGHTGUNCALIBRATION_SPRITES:
		return m_lightgunCalibrationSpritesTexture;

	case SECTION_LIST_SPRITES:
		return m_listSpritesTexture;

	case SECTION_MENU_SPRITES:
		return m_menuSpritesTexture;

	case SECTION_BUTTON_SPRITES:
		return m_buttonSpritesTexture;
	}

	return NULL;
}

//---------------------------------------------------------------------
//	ResourceIDToINISection
//---------------------------------------------------------------------
static const char *ResourceIDToINISection( SkinResourceID_t idToMap )
{
	SkinINISectionID_t section = ResourceIDToSectionID( idToMap );
	if( section != SECTION_INVALID )
		return g_skinINISections[section];

	PRINTMSG(( T_ERROR, "Unknown SkinResourceID_t entry %d!", idToMap ));
	return NULL;
}


//---------------------------------------------------------------------
//	ColorIDToSectionID
//---------------------------------------------------------------------
inline SkinINISectionID_t ColorIDToSectionID( SkinColorID_t idToMap )
{
	if( idToMap == COLOR_POPUP_DARKENOVERLAY )
		return SECTION_POPUP_OPTIONS;

	if( idToMap >= COLOR_HELPSCREEN_TITLEBAR_TEXT && idToMap <= COLOR_HELPSCREEN_BUTTONICON_TEXT )
		return SECTION_HELPSCREEN_OPTIONS;

	if( idToMap >= COLOR_OPTIONSSCREEN_TITLEBAR_TEXT && idToMap <= COLOR_OPTIONSSCREEN_BUTTONICON_TEXT )
		return SECTION_OPTIONSSCREEN_OPTIONS;

	if( idToMap >= COLOR_ROMLISTSCREEN_TITLEBAR_TEXT && idToMap <= COLOR_ROMLISTSCREEN_BUTTONICON_TEXT )
		return SECTION_ROMLISTSCREEN_OPTIONS;

	if( idToMap == COLOR_SCREENSAVER_TEXT )
		return SECTION_SCREENSAVER_OPTIONS;

	if( idToMap >= COLOR_SKINCHOOSERSCREEN_TITLEBAR_TEXT && idToMap <= COLOR_SKINCHOOSERSCREEN_BUTTONICON_TEXT )
		return SECTION_SKINCHOOSERSCREEN_OPTIONS;

	if( idToMap >= COLOR_STARTMENU_TITLEBAR_TEXT && idToMap <= COLOR_STARTMENU_BODY_HIGHLIGHTBAR )
		return SECTION_STARTMENU_OPTIONS;

	if( idToMap >= COLOR_VIRTUALKEYBOARD_TITLEBAR_TEXT && idToMap <= COLOR_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR )
		return SECTION_VIRTUALKEYBOARD_OPTIONS;

	if( idToMap >= COLOR_MESSAGESCREEN_TEXT && idToMap <= COLOR_MESSAGESCREEN_PROGRESSBAR_BACKGROUND )
		return SECTION_MESSAGESCREEN_OPTIONS;

	if( idToMap >= COLOR_SPLASHSCREEN_TEXT && idToMap <= COLOR_SPLASHSCREEN_IMPORTANT_TEXT )
		return SECTION_SPLASHSCREEN_OPTIONS;

	return SECTION_INVALID;
}

//---------------------------------------------------------------------
//	ColorIDToINISection
//---------------------------------------------------------------------
static const char *ColorIDToINISection( SkinColorID_t idToMap )
{
	SkinINISectionID_t section = ColorIDToSectionID( idToMap );
	if( section != SECTION_INVALID )
		return g_skinINISections[section];

	PRINTMSG(( T_ERROR, "Unknown SkinColorID_t entry %d!", idToMap ));
	return NULL;
}


//---------------------------------------------------------------------
//	StringToColor
//---------------------------------------------------------------------
static D3DCOLOR StringToColor( const CStdString &argbString )
{
	int color[4] = { 255, 0, 0, 0 };
	char *str = strdup( argbString.c_str() );

	char *value = str;
	char *delim = strpbrk( value, g_argbColorDelimiters );
	for( int i = 0; i < 4 && value; ++i )
	{
		char *next = NULL;
		if( delim )
		{
			*delim = 0;
					
				// Skip to the next number
			next = strpbrk( delim + 1, "0123456789abcdef" );
			delim = strpbrk( next, g_argbColorDelimiters );
		}

		sscanf( value, "%d", &color[i] );
		value = next;
	}

	free( str );

	return D3DCOLOR_ARGB( color[0], color[1], color[2], color[3] );
}

