/**
	* \file			Skin.cpp
	* \brief		CSkin class
	*
	*/


//= I N C L U D E S ====================================================
#include "Skin.h"

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
CSkin							*g_loadedSkin = NULL;


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

//= P R O T O T Y P E S ===============================================
static const char *ResourceIDToINISection( SkinResourceID_t idToMap );
static const char *ElementIDToINISection( SkinElementID_t idToMap );

//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	LoadSkin
//---------------------------------------------------------------------
BOOL CSkin::LoadSkin( CStdString *errorReport )
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


	for( int i = 0; i < SKINRESOURCE_COUNT; ++i )
	{
		CSkinSpriteResource tempInfo;
		if( tempInfo.ParseINI( iniFile, ResourceIDToINISection( (SkinResourceID_t)i ), g_resourceIDToINIEntry[i] ) )
			m_spriteResourceArray[i] = new CSkinSpriteResource( tempInfo );
	}



		// PARSEITEM
	#define PARSEITEM( id, __elementType )  { \
			##__elementType temp; \
			if( temp.ParseINI( iniFile, ElementIDToINISection( id ) ) ) { \
				m_spriteElementArray[id] = new __elementType##( temp );	\
				PRINTMSG(( T_INFO, "Created " #id " of type " #__elementType "!" )); \
			} \
	}


		// PARSESUBITEM
	#define PARSESUBITEM( id, __elementType, parentIniEntry )  { \
			##__elementType temp; \
			if( temp.ParseINI( iniFile, ElementIDToINISection( id ), parentIniEntry ) ) { \
				m_spriteElementArray[id] = new __elementType##( temp );	\
				PRINTMSG(( T_INFO, "Created " #id " of type " #__elementType "!" )); \
			} \
	}

		// PARSESUBITEMEX
	#define PARSESUBITEMEX( id, __elementType, constructorArg, parentIniEntry )  { \
			##__elementType temp( constructorArg ); \
			if( temp.ParseINI( iniFile, ElementIDToINISection( id ), parentIniEntry ) ) { \
				m_spriteElementArray[id] = new __elementType##( temp );	\
				PRINTMSG(( T_INFO, "Created " #id " of type " #__elementType "!" )); \
			} \
	}

		// PARSELINKEDITEM
	#define PARSELINKEDITEM( id, __elementType, parentIniEntry, parentID )  { \
			##__elementType temp; \
			if( temp.ParseINI( iniFile, ElementIDToINISection( id ), parentIniEntry, parentID ) ) { \
				m_spriteElementArray[id] = new __elementType##( temp );	\
				PRINTMSG(( T_INFO, "Created " #id " of type " #__elementType "!" )); \
			} \
	}

		// PARSEICON
	#define PARSEICON( id, __elementType, fullIniEntry, iconID )  { \
			##__elementType temp( iconID ); \
			if( temp.ParseINI( iniFile, ElementIDToINISection( id ), fullIniEntry ) ) { \
				m_spriteElementArray[id] = new __elementType##( temp );	\
				PRINTMSG(( T_INFO, "Created " #id " of type " #__elementType "!" )); \
			} \
	}



		// ---- General Popup Options -----------------------------------------
	PARSEITEM( SKINELEMENT_POPUPOPTIONS_DIFFUSEOVERLAY, CSkinDiffuseOverlay );


		// ROMList Screen --------------------------------------------------------
	PARSEITEM(				SKINELEMENT_ROMLISTSCREEN_HEADER,																	CSkinMenuHeader );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_SIMPLE_NAME_TEXT,								CSkinText, "TitleBar.Simple.Name" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_NAME_TEXT,						CSkinText, "TitleBar.Verbose.Name" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_MANUFACTERER_TEXT,		CSkinText, "TitleBar.Verbose.Manufacturer" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_YEAR_TEXT,						CSkinText, "TitleBar.Verbose.Year" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_NUMPLAYERS_TEXT,			CSkinText, "TitleBar.Verbose.NumPlayers" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_CLONE_TEXT,					CSkinText, "TitleBar.Verbose.Clone" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_HEADER_DETAIL_NAME_TEXT,								CSkinText, "TitleBar.Detail.Name" );

	PARSEITEM(				SKINELEMENT_ROMLISTSCREEN_BODY,																		CSkinMenuBody );
	PARSELINKEDITEM(	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA,												CSkinScrollArea, "Body", GetSkinElement(SKINELEMENT_ROMLISTSCREEN_BODY) );

	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWORKING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Verbose.ROMWorking" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWARNING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Verbose.ROMWarning" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMNONWORKING_NAME_TEXT,	CSkinText, "Body.ScrollArea.SingleRow.Verbose.ROMNonworking" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_MANUFACTURER_TEXT,				CSkinText, "Body.ScrollArea.SingleRow.Verbose.Manufacturer" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_YEAR_TEXT,								CSkinText, "Body.ScrollArea.SingleRow.Verbose.Year" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_NUMPLAYERS_TEXT,					CSkinText, "Body.ScrollArea.SingleRow.Verbose.NumPlayers" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_CLONE_TEXT,								CSkinText, "Body.ScrollArea.SingleRow.Verbose.Clone" );

	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWORKING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Simple.ROMWorking" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWARNING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Simple.ROMWarning" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMNONWORKING_NAME_TEXT,		CSkinText, "Body.ScrollArea.SingleRow.Simple.ROMNonworking" );

	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWORKING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Detail.ROMWorking" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWARNING_NAME_TEXT,			CSkinText, "Body.ScrollArea.SingleRow.Detail.ROMWarning" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT,		CSkinText, "Body.ScrollArea.SingleRow.Detail.ROMNonworking" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_NUMPLAYERS_TEXT,						CSkinText, "Body.ScrollArea.SingleRow.Detail.NumPlayers" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_MANUFACTURER_TEXT,					CSkinText, "Body.ScrollArea.SingleRow.Detail.Manufacturer" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_YEAR_TEXT,									CSkinText, "Body.ScrollArea.SingleRow.Detail.Year" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_PARENT_TEXT,								CSkinText, "Body.ScrollArea.SingleRow.Detail.Parent" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_GENRE_TEXT,								CSkinText, "Body.ScrollArea.SingleRow.Detail.Genre" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_VERSIONADDED_TEXT,					CSkinText, "Body.ScrollArea.SingleRow.Detail.VersionAdded" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_TIMESPLAYED_TEXT,					CSkinText, "Body.ScrollArea.SingleRow.Detail.TimesPlayed" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FAVORITESTATUS_TEXT,				CSkinText, "Body.ScrollArea.SingleRow.Detail.FavoriteStatus" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FILENAME_TEXT,							CSkinText, "Body.ScrollArea.SingleRow.Detail.Filename" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_SCREENSHOT,								CSkinScreenshot, "Body.ScrollArea.SingleRow.Detail" );

	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_UP,														CSkinIcon, "Body.ScrollIcon.Up", SPRITE_LIST_SCROLLICON_UP );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_DOWN,													CSkinIcon, "Body.ScrollIcon.Down", SPRITE_LIST_SCROLLICON_DOWN );

	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_MANUFACTURER_DIVIDER,						CSkinDivider, "Body.Verbose.Manufacturer" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_YEAR_DIVIDER,										CSkinDivider, "Body.Verbose.Year" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_NUMPLAYERS_DIVIDER,							CSkinDivider, "Body.Verbose.NumPlayers" );
	PARSESUBITEM(			SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_CLONE_DIVIDER,										CSkinDivider, "Body.Verbose.Clone" );

	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_A,																	CSkinButtonInfo, "ButtonInfo.A", SPRITE_BUTTON_A );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_B,																	CSkinButtonInfo, "ButtonInfo.B", SPRITE_BUTTON_B );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_X,																	CSkinButtonInfo, "ButtonInfo.X", SPRITE_BUTTON_X );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_Y,																	CSkinButtonInfo, "ButtonInfo.Y", SPRITE_BUTTON_Y );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_WHITE,															CSkinButtonInfo, "ButtonInfo.White", SPRITE_BUTTON_WHITE );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_BLACK,															CSkinButtonInfo, "ButtonInfo.Black", SPRITE_BUTTON_BLACK );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_TRIGGER_LEFT,											CSkinButtonInfo, "ButtonInfo.Trigger.Left", SPRITE_BUTTON_LEFTTRIGGER );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_TRIGGER_RIGHT,											CSkinButtonInfo, "ButtonInfo.Trigger.Right", SPRITE_BUTTON_RIGHTTRIGGER );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_ANALOG_LEFT,												CSkinButtonInfo, "ButtonInfo.Analog.Left", SPRITE_BUTTON_LEFTANALOG );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_ANALOG_RIGHT,											CSkinButtonInfo, "ButtonInfo.Analog.Right", SPRITE_BUTTON_RIGHTANALOG );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_BACK,															CSkinButtonInfo, "ButtonInfo.Back", SPRITE_BUTTON_BACK );
	PARSEICON(				SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_START,															CSkinButtonInfo, "ButtonInfo.Start", SPRITE_BUTTON_START );

	PARSEITEM(				SKINELEMENT_ROMLISTSCREEN_FOOTER,																				CSkinMenuFooter );

		// Help Screen --------------------------------------------------------
	PARSEITEM(				SKINELEMENT_HELPSCREEN_HEADER,													CSkinMenuHeader );
	PARSELINKEDITEM(	SKINELEMENT_HELPSCREEN_HEADER_TEXT,											CSkinText, "TitleBar", GetSkinElement(SKINELEMENT_HELPSCREEN_HEADER) );
	PARSEITEM(				SKINELEMENT_HELPSCREEN_BODY,														CSkinMenuBody );
	PARSELINKEDITEM(	SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA,									CSkinScrollArea, "Body", GetSkinElement(SKINELEMENT_HELPSCREEN_BODY) );
	PARSESUBITEM(			SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA_SINGLEROW_TEXT,	CSkinText, "Body.ScrollArea.SingleRow" );
	PARSEICON(				SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_UP,							CSkinIcon, "Body.ScrollIcon.Up", SPRITE_LIST_SCROLLICON_UP );
	PARSEICON(				SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_DOWN,						CSkinIcon, "Body.ScrollIcon.Down", SPRITE_LIST_SCROLLICON_DOWN );
	PARSEICON(				SKINELEMENT_HELPSCREEN_BUTTONINFO_START,								CSkinButtonInfo, "ButtonInfo.Start", SPRITE_BUTTON_START );
	PARSEITEM(				SKINELEMENT_HELPSCREEN_FOOTER,													CSkinMenuFooter );

		// Options Screen --------------------------------------------------------
	PARSEITEM(				SKINELEMENT_OPTIONSSCREEN_HEADER,																			CSkinMenuHeader );
	PARSELINKEDITEM(	SKINELEMENT_OPTIONSSCREEN_HEADER_TEXT,																CSkinText, "TitleBar", GetSkinElement(SKINELEMENT_HELPSCREEN_HEADER) );
	PARSEITEM(				SKINELEMENT_OPTIONSSCREEN_BODY,																				CSkinMenuBody );
	PARSELINKEDITEM(	SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA,														CSkinScrollArea, "Body", GetSkinElement(SKINELEMENT_HELPSCREEN_BODY) );
	PARSESUBITEM(			SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONNAME_TEXT,	CSkinText, "Body.ScrollArea.SingleRow.OptionName" );
	PARSESUBITEM(			SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONVALUE_TEXT,	CSkinText, "Body.ScrollArea.SingleRow.OptionValue" );
	PARSESUBITEM(			SKINELEMENT_OPTIONSSCREEN_BODY_DIVIDER,																CSkinDivider, "Body" );
	PARSESUBITEMEX(		SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_LEFT,											CSkinTriggerInfo, FALSE, "Body.LeftTrigger" );
	PARSESUBITEMEX(		SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_RIGHT,											CSkinTriggerInfo, TRUE, "Body.RightTrigger" );
	PARSEICON(				SKINELEMENT_OPTIONSSCREEN_BUTTONINFO_START,														CSkinButtonInfo, "ButtonInfo.Start", SPRITE_BUTTON_START );
	PARSEITEM(				SKINELEMENT_OPTIONSSCREEN_FOOTER,																			CSkinMenuFooter );


		// SkinChooser Screen -------------------------------------------------
	PARSEITEM(				SKINELEMENT_SKINCHOOSERSCREEN_HEADER,																									CSkinMenuHeader );
	PARSELINKEDITEM(	SKINELEMENT_SKINCHOOSERSCREEN_HEADER_TEXT,																						CSkinText, "TitleBar", GetSkinElement(SKINELEMENT_SKINCHOOSERSCREEN_HEADER) );
	PARSEITEM(				SKINELEMENT_SKINCHOOSERSCREEN_BODY,																										CSkinMenuBody );
	PARSEICON(				SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_UP,																			CSkinIcon, "Body.ScrollIcon.Up", SPRITE_LIST_SCROLLICON_UP );
	PARSEICON(				SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_DOWN,																		CSkinIcon, "Body.ScrollIcon.Down", SPRITE_LIST_SCROLLICON_DOWN );
	PARSELINKEDITEM(	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA,																				CSkinScrollArea, "Body", GetSkinElement(SKINELEMENT_SKINCHOOSERSCREEN_BODY) );
	PARSESUBITEM(			SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAME_TEXT,										CSkinText, "Body.ScrollArea.SingleRow.Name" );
	PARSESUBITEM(			SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_DESCRIPTION_TEXT,							CSkinText, "Body.ScrollArea.SingleRow.Description" );
	PARSESUBITEM(			SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAMEWITHOUTDESCRIPTION_TEXT,	CSkinText, "Body.ScrollArea.SingleRow.NameWithoutDescription" );
	PARSESUBITEM(			SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_SCREENSHOT,										CSkinScreenshot, "Body.ScrollArea.SingleRow" );
	PARSEICON(				SKINELEMENT_SKINCHOOSERSCREEN_BUTTONINFO_START,																				CSkinButtonInfo, "ButtonInfo.Start", SPRITE_BUTTON_START );
	PARSEITEM(				SKINELEMENT_SKINCHOOSERSCREEN_FOOTER, CSkinMenuFooter );


		// ScreenSaverOptions -------------------------------------------------
	PARSESUBITEM(	SKINELEMENT_SCREENSAVEROPTIONS_TEXT,				CSkinText, "" );
	
		// MessageScreenOptions -------------------------------------------------
	PARSESUBITEM(	SKINELEMENT_MESSAGESCREEN_TEXT,					CSkinText, "" );
	PARSESUBITEM( SKINELEMENT_MESSAGESCREEN_PROGRESSBAR,	CSkinProgressbar, "" );

		// SplashScreenOptions -------------------------------------------------
	PARSESUBITEM( SKINELEMENT_SPLASHSCREEN_TEXT,							CSkinText, "" );
	PARSESUBITEM( SKINELEMENT_SPLASHSCREEN_IMPORTANT_TEXT,		CSkinText, "Important" );

		// VirtualKeyboardOptions -------------------------------------------------
	PARSESUBITEM( SKINELEMENT_VIRTUALKEYBOARD_HEADER_TEXT,					CSkinText, "TitleBar" );
	PARSESUBITEM( SKINELEMENT_VIRTUALKEYBOARD_BODY_TEXT,						CSkinText, "Body" );
	PARSESUBITEM( SKINELEMENT_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR,		CSkinHighlightBar, "Body" );

		// StartMenuOptions -------------------------------------------------
	PARSESUBITEM( SKINELEMENT_STARTMENU_HEADER_TEXT,					CSkinText, "TitleBar" );
	PARSESUBITEM( SKINELEMENT_STARTMENU_BODY_TEXT,						CSkinText, "Body" );
	PARSESUBITEM( SKINELEMENT_STARTMENU_BODY_HIGHLIGHTBAR,		CSkinHighlightBar, "Body" );



	#undef PARSEITEM
	#undef PARSELINKEDITEM
	#undef PARSESUBITEM
	#undef PARSESUBITEMEX
	#undef PARSEICON

	return TRUE;
}

/*

*/


//---------------------------------------------------------------------
//	LoadSkinTexture
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE8 CSkin::LoadSkinTexture(	CSystem_IniFile &iniFile,
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
//	GetSkinTexture
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE8 CSkin::GetSkinTexture( SkinResourceID_t id ) const
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
//	ElementIDToSectionID
//---------------------------------------------------------------------
inline SkinINISectionID_t ElementIDToSectionID( SkinElementID_t idToMap )
{
	if( idToMap == SKINELEMENT_POPUPOPTIONS_DIFFUSEOVERLAY )
		return SECTION_POPUP_OPTIONS;

	if( idToMap >= SKINELEMENT_HELPSCREEN_HEADER && idToMap <= SKINELEMENT_HELPSCREEN_FOOTER )
		return SECTION_HELPSCREEN_OPTIONS;

	if( idToMap >= SKINELEMENT_OPTIONSSCREEN_HEADER && idToMap <= SKINELEMENT_OPTIONSSCREEN_FOOTER )
		return SECTION_OPTIONSSCREEN_OPTIONS;

	if( idToMap >= SKINELEMENT_SKINCHOOSERSCREEN_HEADER && idToMap <= SKINELEMENT_SKINCHOOSERSCREEN_FOOTER )
		return SECTION_SKINCHOOSERSCREEN_OPTIONS;

	if( idToMap == SKINELEMENT_SCREENSAVEROPTIONS_TEXT )
		return SECTION_SCREENSAVER_OPTIONS;

	if( idToMap >= SKINELEMENT_MESSAGESCREEN_TEXT && idToMap <= SKINELEMENT_MESSAGESCREEN_PROGRESSBAR )
		return SECTION_MESSAGESCREEN_OPTIONS;

	if( idToMap >= SKINELEMENT_SPLASHSCREEN_TEXT && idToMap <= SKINELEMENT_SPLASHSCREEN_IMPORTANT_TEXT )
		return SECTION_SPLASHSCREEN_OPTIONS;

	if( idToMap >= SKINELEMENT_VIRTUALKEYBOARD_HEADER_TEXT && idToMap <= SKINELEMENT_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR )
		return SECTION_VIRTUALKEYBOARD_OPTIONS;

	if( idToMap >= SKINELEMENT_STARTMENU_HEADER_TEXT && idToMap <= SKINELEMENT_STARTMENU_BODY_HIGHLIGHTBAR )
		return SECTION_STARTMENU_OPTIONS;

	if( idToMap >= SKINELEMENT_ROMLISTSCREEN_HEADER && idToMap <= SKINELEMENT_ROMLISTSCREEN_FOOTER )
		return SECTION_ROMLISTSCREEN_OPTIONS;

	return SECTION_INVALID;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static const char *ElementIDToINISection( SkinElementID_t idToMap )
{
	SkinINISectionID_t section = ElementIDToSectionID( idToMap );
	if( section != SECTION_INVALID )
		return g_skinINISections[section];

	PRINTMSG(( T_ERROR, "Unknown SkinElementID_t entry %d!", idToMap ));
	return NULL;
}

