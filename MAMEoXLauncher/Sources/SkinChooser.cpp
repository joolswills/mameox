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

//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================


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

