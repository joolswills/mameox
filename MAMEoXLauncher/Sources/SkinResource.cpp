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
	SECTION_BUTTON_SPRITES
} SkinINISectionID_t;



//= G L O B A L = V A R S =============================================
extern CGraphicsManager	  g_graphicsManager;

	// Array of INI sections containing sprites
const char *g_skinINISections[] = { "SplashBackdrop",
																		"MessageBackdrop",
																		"ListBackdrop",
																		"TVCalibrationSprites", 
																		"LightgunCalibrationSprites", 
																		"ListSprites",
																		"MenuSprites",
																		"ButtonSprites" };


	// Map of SkinResourceID_t's to their name in the skin.ini file 
static const char *g_spriteIDToINIEntry[] = {
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

	"Button.A",									// SPRITE_BUTTON_A
	"Button.B",									// SPRITE_BUTTON_B
	"Button.X",									// SPRITE_BUTTON_X
	"Button.Y",									// SPRITE_BUTTON_Y
	"Button.Black",							// SPRITE_BUTTON_BLACK
	"Button.White",							// SPRITE_BUTTON_WHITE
	"Button.Start",							// SPRITE_BUTTON_START
	"Button.Back",							// SPRITE_BUTTON_BACK
	"Button.LeftTrigger",				// SPRITE_BUTTON_LEFTTRIGGER
	"Button.RightTrigger",			// SPRITE_BUTTON_RIGHTTRIGGER
	"Button.LeftAnalog",				// SPRITE_BUTTON_LEFTANALOG
	"Button.RightAnalog"				// SPRITE_BUTTON_RIGHTANALOG
};




//= P R O T O T Y P E S ===============================================
static const char *ResourceIDToINISection( SkinResourceID_t idToMap );
static LPDIRECT3DTEXTURE8 HelperCreateTextureFromPNGFile( PNGFile_t &colorFileData, BYTE *alphaChannel );


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
		
		CStdString entryName = g_spriteIDToINIEntry[i];
		entryName += ".FileOffset.";

		tempInfo.m_left		= iniFile.GetProfileInt( sectionName, entryName + "left", -1 );
		tempInfo.m_right	= iniFile.GetProfileInt( sectionName, entryName + "right", -1 );
		tempInfo.m_top		= iniFile.GetProfileInt( sectionName, entryName + "top", -1 );
		tempInfo.m_bottom = iniFile.GetProfileInt( sectionName, entryName + "bottom", -1 );

		if( tempInfo.m_left != -1.0f && tempInfo.m_right != -1.0f && tempInfo.m_top != -1.0f && tempInfo.m_bottom != -1.0f )
			m_spriteInfoArray[i] = new SkinResourceInfo_t( tempInfo );
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
				// Convert the PNG file to just alpha bytes
			if( !(alphaChannel = (BYTE*)malloc( alphaFileData.m_imageWidth * alphaFileData.m_imageHeight )) )
			{
				PRINTMSG(( T_INFO, "Out of memory allocating space for alphaChannel, alpha data will be ignored!" ));
			}
			else
			{
				UINT32 y;
				switch( alphaFileData.m_infoPtr->color_type )
				{
					// *** PNG_COLOR_TYPE_PALETTE *** //
				case PNG_COLOR_TYPE_PALETTE:
					{
						switch( alphaFileData.m_infoPtr->bit_depth )
						{
							// Monochrome
						case 1:
							{
								BYTE *dest = alphaChannel;
								for( y = 0; y < alphaFileData.m_imageHeight; ++y )
								{
									BYTE *src = (BYTE*)alphaFileData.m_data[y];
									for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 3); ++x )
									{      
										for( INT32 i = 7; i >= 0; --i )
										{
											BOOL palIndex = (*src & (1 << i)) ? 1 : 0;
											*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;
										}
										++src;
									}
									free( alphaFileData.m_data[y] );
									alphaFileData.m_data[y] = NULL;
								}
							}
							break;

						case 2:
							{
								BYTE *dest = alphaChannel;
								for( y = 0; y < alphaFileData.m_imageHeight; ++y )
								{
									BYTE *src = (BYTE*)alphaFileData.m_data[y];
									for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 2); ++x )
									{      
										BYTE palIndex = (*src >> 6) & 0x03;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										palIndex = (*src >> 4) & 0x03;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										palIndex = (*src >> 2) & 0x03;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										palIndex = (*src) & 0x03;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										++src;
									}
									free( alphaFileData.m_data[y] );
									alphaFileData.m_data[y] = NULL;
								}
							}
							break;

						case 4:
							{
								BYTE *dest = alphaChannel;
								for( y = 0; y < alphaFileData.m_imageHeight; ++y )
								{
									BYTE *src = (BYTE*)alphaFileData.m_data[y];
									for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 1); ++x )
									{
										BYTE palIndex = (*src >> 4) & 0x07;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										palIndex = *src & 0x07;
										*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

										++src;
									}
									free( alphaFileData.m_data[y] );
									alphaFileData.m_data[y] = NULL;
								}
							}
							break;

						default:
							{
								BYTE *dest = alphaChannel;
								for( y = 0; y < alphaFileData.m_imageHeight; ++y )
								{
									BYTE *src = (BYTE*)alphaFileData.m_data[y];
									for( UINT32 x = 0; x < alphaFileData.m_imageWidth; ++x )
										*(dest++) = alphaFileData.m_infoPtr->palette[*(src++)].green;

									free( alphaFileData.m_data[y] );
									alphaFileData.m_data[y] = NULL;
								}
							}
							break;
						}
					}
					break;

					// *** PNG_COLOR_TYPE_RGB *** //
				case PNG_COLOR_TYPE_RGB:
					{
						BYTE *dest = alphaChannel;
						for( y = 0; y < alphaFileData.m_imageHeight; ++y )
						{
							BYTE *src = (BYTE*)alphaFileData.m_data[y];
							for( UINT32 x = 0; x < alphaFileData.m_imageWidth; ++x )
							{      
									// Grab the green channel
								*(dest++) = *(src+1);
								src += 3;
							}
							free( alphaFileData.m_data[y] );
							alphaFileData.m_data[y] = NULL;
						}
					}
					break;
				}
			}

			ClosePNGFile( alphaFileData );
		}
	}

	LPDIRECT3DTEXTURE8 ret = HelperCreateTextureFromPNGFile( colorFileData, alphaChannel );
	ClosePNGFile( colorFileData );
	if( alphaChannel )
		free( alphaChannel );

	PRINTMSG(( T_INFO, "Texture created: 0x%X", ret ));

	return ret;		
}


//---------------------------------------------------------------------
//	ResourceIDToINISection
//---------------------------------------------------------------------
static const char *ResourceIDToINISection( SkinResourceID_t idToMap )
{
	if( idToMap == ASSET_SPLASH_BACKDROP )
		return g_skinINISections[SECTION_SPLASH_BACKDROP];

	if( idToMap == ASSET_MESSAGE_BACKDROP )
		return g_skinINISections[SECTION_MESSAGE_BACKDROP];

	if( idToMap == ASSET_LIST_BACKDROP )
		return g_skinINISections[SECTION_LIST_BACKDROP];

	if( idToMap == SPRITE_TVCALIBRATION_CURSOR )
		return g_skinINISections[SECTION_TVCALIBRATION_SPRITES];

	if( idToMap >= SPRITE_LIGHTGUNCALIBRATION_CURSOR && idToMap <= SPRITE_LIGHTGUNCALIBRATION_TARGET )
		return g_skinINISections[SECTION_LIGHTGUNCALIBRATION_SPRITES];

	if( idToMap >= SPRITE_LIST_SCROLLICON_DOWN && idToMap <= SPRITE_LIST_TRIGGERICON_TAIL )
		return g_skinINISections[SECTION_LIST_SPRITES];

	if( idToMap >= SPRITE_MENU_TITLEBAR_LEFTSIDE && idToMap <= SPRITE_MENU_FOOTER_RIGHTSIDE )
		return g_skinINISections[SECTION_MENU_SPRITES];

	if( idToMap >= SPRITE_BUTTON_A && idToMap <= SPRITE_BUTTON_RIGHTANALOG )
		return g_skinINISections[SECTION_BUTTON_SPRITES];

	PRINTMSG(( T_ERROR, "Unknown SkinResourceID_t entry %d!", idToMap ));
	return NULL;
}



//---------------------------------------------------------------------
//	HelperCreateXRGBTextureFromPNGFile
//---------------------------------------------------------------------
static LPDIRECT3DTEXTURE8 HelperCreateTextureFromPNGFile( PNGFile_t &colorFileData, BYTE *alphaChannel )
{
	LPDIRECT3DTEXTURE8 ret;

	PRINTMSG(( T_INFO, "Create Texture from file w/ size %lux%lu", colorFileData.m_imageWidth, colorFileData.m_imageHeight ));

    // Create a new texture and read the data into it
  if( (D3DXCreateTexture( g_graphicsManager.GetD3DDevice(),
                          colorFileData.m_imageWidth,
                          colorFileData.m_imageHeight,
													1,									// Mip levels
                          0,                  // Usage
													alphaChannel ? D3DFMT_LIN_A8R8G8B8 : D3DFMT_LIN_X8R8G8B8,	// Format
													0,		              // Pool (unused)
                          &ret )) != S_OK )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    PRINTMSG(( T_ERROR, "Failed to create texture for skin" ));
    PRINTMSG(( T_INFO, "Memory status" ));
    PRINTMSG(( T_INFO, "Physical:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailPhys ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalPhys ));
    PRINTMSG(( T_INFO, "Page File:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailPageFile ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalPageFile ));
    PRINTMSG(( T_INFO, "Virtual:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailVirtual ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalVirtual ));
		return FALSE;
	}


		// Grab the surface description
	D3DSURFACE_DESC desc;
  ret->GetLevelDesc( 0, &desc );

    // Lock the region so we can render to it later
  D3DLOCKED_RECT lockedRect;
	ret->LockRect(	0, &lockedRect, NULL, 0 );
  memset( lockedRect.pBits, 0, desc.Width * desc.Height * sizeof(DWORD) );

  UINT32 y;
	switch( colorFileData.m_infoPtr->color_type )
  {
    // *** PNG_COLOR_TYPE_PALETTE *** //
  case PNG_COLOR_TYPE_PALETTE:
    {
      switch( colorFileData.m_infoPtr->bit_depth )
      {
        // Monochrome
      case 1:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 3); ++x )
            {      
              for( INT32 i = 7; i >= 0; --i )
              {
                BOOL palIndex = (*src & (1 << i)) ? 1 : 0;
                png_color &color = colorFileData.m_infoPtr->palette[palIndex];

								if( alphaSrc )
	                *(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
								else
	                *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
              }

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 2:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 2); ++x )
            {      
              BYTE palIndex = (*src >> 6) & 0x03;
              png_color color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
	              *(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 4) & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 2) & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 4:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 1); ++x )
            {
              BYTE palIndex = (*src >> 4) & 0x07;
              png_color &color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x07;
              png_color &color2 = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color2.red, color2.green, color2.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color2.red, color2.green, color2.blue );

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      default:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < colorFileData.m_imageWidth; ++x )
            {      
              png_color &color = colorFileData.m_infoPtr->palette[*(src++)];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;
      }
    }
    break;

    // *** PNG_COLOR_TYPE_RGB *** //
  case PNG_COLOR_TYPE_RGB:
    {
			BYTE *alphaSrc = alphaChannel;
      D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
      for( y = 0; y < colorFileData.m_imageHeight; ++y )
      {
        D3DCOLOR *dest = destRow;
        BYTE *src = (BYTE*)colorFileData.m_data[y];
        for( UINT32 x = 0; x < colorFileData.m_imageWidth; ++x )
        {      
					if( alphaSrc )
						*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), *(src), *(src+1), *(src+2) );
					else
	          *(dest++) = D3DCOLOR_XRGB( *(src), *(src+1), *(src+2) );
          src += 3;
        }
        free( colorFileData.m_data[y] );
        colorFileData.m_data[y] = NULL;
        destRow += desc.Width;
      }
    }
    break;
  }

	return ret;
}



