/**
	* \file			SkinChooser.h
	* \brief		Helper class which takes care of loading and maintaining
	*           the list of available skins
	*
	*/

//= I N C L U D E S ====================================================
#include "SkinChooser.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"
#include "System_IniFile.h"

#include <string>
#include <vector>
#include <algorithm>


//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================

  //--- Layout defines -----------------------------------------
#define HEADER_COLOR          D3DCOLOR_XRGB( 1, 1, 1 )
#define HELPITEM_COLOR        D3DCOLOR_XRGB( 20, 20, 20 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 1, 1, 1 )
#define ITEM_WARNING_COLOR    D3DCOLOR_XRGB( 100, 100, 0 )
#define ITEM_NONWORKING_COLOR D3DCOLOR_XRGB( 240, 20, 20 )

#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define SCROLLICON_COLOR        D3DCOLOR_XRGB( 255, 255, 255 )
#define SPACER_COLOR            D3DCOLOR_ARGB( 100, 255, 255, 255 )
#define NOSCREENSHOT_COLOR      D3DCOLOR_XRGB( 1, 1, 1 )
#define NOSCREENSHOTTEXT_COLOR  D3DCOLOR_XRGB( 255, 255, 255 )

#define TITLEBAR_ROW          99
#define FIRSTDATA_ROW         124

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    605

#define SPACER_WIDTH          2

#define NAME_COLUMN           42
#define MANUFACTURER_COLUMN   305
#define YEAR_COLUMN           437
#define NUMPLAYERS_COLUMN     506
#define CLONE_COLUMN          535 
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text

#define SCROLLUP_TOP          122
#define SCROLLUP_RIGHT        608
#define SCROLLUP_LEFT         (SCROLLUP_RIGHT - 32)
#define SCROLLUP_BOTTOM       (SCROLLUP_TOP + 32)

#define SCROLLDOWN_BOTTOM     451
#define SCROLLDOWN_TOP        (SCROLLDOWN_BOTTOM - 32)
#define SCROLLDOWN_RIGHT      608
#define SCROLLDOWN_LEFT       (SCROLLDOWN_RIGHT - 32)

  //-- Button help messages ------
#define HELP_START_ICON_X   200
#define HELP_START_ICON_Y   40
#define HELP_START_TEXT_X   (HELP_START_ICON_X + desc->GetWidth() + 4)
#define HELP_START_TEXT_Y   (HELP_START_ICON_Y + 5)


#define DETAIL_SCREENSHOT_RIGHT   SCROLLUP_LEFT
#define DETAIL_SCREENSHOT_TOP     127
#define DETAIL_SCREENSHOT_LEFT    (DETAIL_SCREENSHOT_RIGHT - 128)
#define DETAIL_SCREENSHOT_BOTTOM  (DETAIL_SCREENSHOT_TOP + ( (FLOAT)(DETAIL_SCREENSHOT_RIGHT - DETAIL_SCREENSHOT_LEFT) * 3.0f / 4.0f))

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							3

	// Timeout values for the cursor movement acceleration bands
	// Values are measured in seconds
#define SBTIMEOUT_FASTEST				3.0f
#define SBTIMEOUT_FASTER				2.0f
#define SBTIMEOUT_FAST					1.0f

	// Multiplier values for the cursor movement acceleration bands
	// Values are multiplied against the cursorVelocity
#define SBMULTIPLIER_FASTEST		5.0f
#define SBMULTIPLIER_FASTER			2.5f
#define SBMULTIPLIER_FAST				1.5f

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.8f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f



//= G L O B A L = V A R S ==============================================



//= P R O T O T Y P E S ================================================
extern "C" void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems ); // Defined in main.cpp

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	FindSkins
//---------------------------------------------------------------------
BOOL CSkinChooser::FindSkins( void )
{
	CStdString path = g_FileIOConfig.m_skinPath;

	std::vector< CStdString > zipFileNames;
	WIN32_FIND_DATA findData;

  HANDLE findHandle = FindFirstFile( (path + "\\*").c_str(), &findData );
  if( findHandle == INVALID_HANDLE_VALUE )
  {
    PRINTMSG(( T_INFO, "Could not find files!" ));
    return FALSE;
  }

		// Search the subdirectories for skin.ini files and preview.png's
	do {
    CStdString basepath = findData.cFileName;
		if( basepath[0] == '.' )
			continue;

		CStdString fullBasePath = path + "\\" + basepath;
		CStdString iniFile = fullBasePath + "\\skin.ini";
		
			// See if there's a skin.ini file
		osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, iniFile.c_str(), "r" );
		if( file )
		{
			osd_fclose( file );

			PRINTMSG(( T_INFO, "Skin %s found!", basepath.c_str() ));

				// Try to load a preview for this skin
			RECT rct = { 0, 0, 0, 0 };
			CStdString previewFile = fullBasePath + "\\preview.png";

			LPDIRECT3DTEXTURE8 texture = NULL;
			PNGFile_t colorFileData;
			if( LoadPNGFile( previewFile, &colorFileData ) )
			{				
				rct.right = colorFileData.m_imageWidth;
				rct.bottom = colorFileData.m_imageHeight;
				texture = CreateTextureFromPNGFile( colorFileData, NULL );
				ClosePNGFile( colorFileData );
			}

				// We have a skin on our hands
			m_skinResourceVector.push_back( new CSkinResource( basepath.c_str(), texture, rct ) );
		}

	} while( FindNextFile( findHandle, &findData ) );
  FindClose( findHandle );


	PRINTMSG(( T_INFO, "Found %lu skins.", m_skinResourceVector.size() ));

		// Try to find the skin stored in our options struct
		// If the skin doesn't exist, just load the first available one
	int skinToLoad = 0;
	if( m_options.m_currentSkin != "" )
	{
		for( int i = 0; i < m_skinResourceVector.size(); ++i )
		{
			if( m_options.m_currentSkin == m_skinResourceVector[i]->GetSkinName() )
			{
				skinToLoad = i;
				break;
			}
		}
	}

	g_loadedSkin = m_skinResourceVector[skinToLoad];
	if( g_loadedSkin )
	{
		BOOL skinLoaded = g_loadedSkin->LoadSkin( NULL );	
		if( !skinLoaded )
		{
			PRINTMSG(( T_ERROR, "Failed to load skin!" ));

			// Todo: Do something about the skin failing to load
			return FALSE;
		}
	}

	return TRUE;
}

