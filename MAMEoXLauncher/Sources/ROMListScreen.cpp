/**
	* \file			ROMListScreen.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "ROMListScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_png.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"
#include "System_IniFile.h"

#include <string>
#include <vector>
#include <algorithm>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}

//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================

  //--- Layout defines -----------------------------------------
#define HEADER_COLOR          D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_WARNING_COLOR    D3DCOLOR_XRGB( 100, 100, 0 )
#define ITEM_NONWORKING_COLOR D3DCOLOR_XRGB( 100, 0, 0 )

#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define SCROLLICON_COLOR        D3DCOLOR_XRGB( 255, 255, 255 )
#define SPACER_COLOR            D3DCOLOR_XRGB( 255, 255, 255 )
#define NOSCREENSHOT_COLOR      D3DCOLOR_XRGB( 0, 0, 0 )
#define NOSCREENSHOTTEXT_COLOR  D3DCOLOR_XRGB( 255, 255, 255 )

#define TITLEBAR_ROW          101
#define FIRSTDATA_ROW         125

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    607

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

  // Detailed view layout
#define DETAIL_ROMSTATUS_X        NAME_COLUMN
#define DETAIL_ROMSTATUS_Y        FIRSTDATA_ROW

#define DETAIL_NUMPLAYERS_X       NAME_COLUMN
#define DETAIL_NUMPLAYERS_Y       (FIRSTDATA_ROW + 20)

#define DETAIL_MANUFACTURER_X     NAME_COLUMN
#define DETAIL_MANUFACTURER_Y     (FIRSTDATA_ROW + 40)

#define DETAIL_YEAR_X             NAME_COLUMN
#define DETAIL_YEAR_Y             (FIRSTDATA_ROW + 60)

#define DETAIL_PARENT_X           NAME_COLUMN
#define DETAIL_PARENT_Y           (FIRSTDATA_ROW + 80)

#define DETAIL_GENRE_X            NAME_COLUMN
#define DETAIL_GENRE_Y            (FIRSTDATA_ROW + 100)

#define DETAIL_VERSIONADDED_X     NAME_COLUMN
#define DETAIL_VERSIONADDED_Y     (FIRSTDATA_ROW + 120)

#define DETAIL_TIMESPLAYED_X      NAME_COLUMN
#define DETAIL_TIMESPLAYED_Y      (FIRSTDATA_ROW + 140)

#define DETAIL_FAVORITESTATUS_X   NAME_COLUMN
#define DETAIL_FAVORITESTATUS_Y   (FIRSTDATA_ROW + 160)

#define DETAIL_FILENAME_X         NAME_COLUMN
#define DETAIL_FILENAME_Y         (FIRSTDATA_ROW + 180)

#define DETAIL_SCREENSHOT_RIGHT   SCROLLUP_LEFT
#define DETAIL_SCREENSHOT_TOP     127
#define DETAIL_SCREENSHOT_LEFT    (DETAIL_SCREENSHOT_RIGHT - 320)
#define DETAIL_SCREENSHOT_BOTTOM  (DETAIL_SCREENSHOT_TOP + ( (FLOAT)(DETAIL_SCREENSHOT_RIGHT - DETAIL_SCREENSHOT_LEFT) * 3.0f / 4.0f))

#define DETAIL_SCREENSHOTCAPTION_X  (DETAIL_SCREENSHOT_LEFT + ((DETAIL_SCREENSHOT_RIGHT - DETAIL_SCREENSHOT_LEFT) >> 1))
#define DETAIL_SCREENSHOTCAPTION_Y  (DETAIL_SCREENSHOT_BOTTOM + 5)

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							15

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
  // Static member initialization
MAMEDriverData_t                        *CROMListScreen::m_driverInfoList = NULL;
UINT32                                  CROMListScreen::m_numDrivers = 0;
std::vector<ROMStatus>                  CROMListScreen::m_ROMStatus;
std::vector<MAMEoXDriverMetadata_t>     CROMListScreen::m_driverMetadata;

//= P R O T O T Y P E S ================================================
extern "C" void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems ); // Defined in main.cpp


  // Compare functions for ROM List sorting
static BOOL Compare_Description( UINT32 a, UINT32 b );
static BOOL Compare_Manufacturer( UINT32 a, UINT32 b );
static BOOL Compare_Year( UINT32 a, UINT32 b );
static BOOL Compare_ParentROM( UINT32 a, UINT32 b );
static BOOL Compare_Genre( UINT32 a, UINT32 b );
static BOOL Compare_NumPlayers( UINT32 a, UINT32 b );
static BOOL Compare_ROMStatus( UINT32 a, UINT32 b );
static BOOL Compare_FavoriteStatus( UINT32 a, UINT32 b );
static BOOL Compare_NumTimesPlayed( UINT32 a, UINT32 b );

static BOOL Helper_ReadXMLTag( osd_file *file, CStdString *tagName );
static void Helper_PNGRead( png_structp pngStruct, png_bytep buf, png_size_t size );

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	LoadROMList
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadROMList( BOOL bGenerate, BOOL allowClones )
{
	PRINTMSG( T_TRACE, "LoadROMList" );

  m_maxPageSize = MAXPAGESIZE;
	m_ROMListFull.clear();
  m_ROMListFiltered.clear();

	m_displayDevice->Clear(	0L,																// Count
													NULL,															// Rects to clear
													D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
													D3DCOLOR_XRGB(0,0,0),							// Color
													1.0f,															// Z
													0L );															// Stencil
	m_fontSet.DefaultFont().Begin();
	  m_fontSet.DefaultFont().DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Loading ROM list", XBFONT_CENTER_X );
	  m_fontSet.DefaultFont().End();
  m_fontSet.DefaultFont().End();

	m_displayDevice->Present( NULL, NULL, NULL, NULL );	


  if( LoadROMListFile() )
  {
    UpdateSortedList();
    m_numLinesInList = m_currentSortedList.size();
    return TRUE;
  }
  else if( bGenerate )
    return GenerateROMList();

  return FALSE;
}

//---------------------------------------------------------------------
//	GenerateROMList
//---------------------------------------------------------------------
BOOL CROMListScreen::GenerateROMList( void )
{
	PRINTMSG( T_TRACE, "GenerateROMList" );

  m_shouldGenerateROMList = FALSE;
	m_ROMListFull.clear();
  m_ROMListFiltered.clear();

  g_FileIOConfig.m_RomPath0.MakeLower();
  g_FileIOConfig.m_RomPath1.MakeLower();
  g_FileIOConfig.m_RomPath2.MakeLower();
  g_FileIOConfig.m_RomPath3.MakeLower();

	PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath0.c_str() );
  Helper_GenerateROMList( g_FileIOConfig.m_RomPath0 );

  if( g_FileIOConfig.m_RomPath1 != g_FileIOConfig.m_RomPath0 )
  {
	  PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath1.c_str() );
    Helper_GenerateROMList( g_FileIOConfig.m_RomPath1 );
  }

  if( g_FileIOConfig.m_RomPath2 != g_FileIOConfig.m_RomPath1 &&
      g_FileIOConfig.m_RomPath2 != g_FileIOConfig.m_RomPath0 )
  {
	  PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath2.c_str() );
    Helper_GenerateROMList( g_FileIOConfig.m_RomPath2 );
  }

  if( g_FileIOConfig.m_RomPath2 != g_FileIOConfig.m_RomPath2 &&
      g_FileIOConfig.m_RomPath2 != g_FileIOConfig.m_RomPath1 &&
      g_FileIOConfig.m_RomPath2 != g_FileIOConfig.m_RomPath0 )
  {
	  PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath3.c_str() );
    Helper_GenerateROMList( g_FileIOConfig.m_RomPath3 );
  }

    // Load the XML status file
  LoadROMStatusFile();
  LoadROMMetadataFile();
  ImportCatverINI();

  if( !SaveROMListFile() )
  {
    PRINTMSG( T_INFO, "Failed to store the ROM list file!" );
    return FALSE;
  }

    // Create the superscroll jump table
  UpdateSortedList();
  m_numLinesInList = m_currentSortedList.size();

  return TRUE;
}

//---------------------------------------------------------------------
//	Helper_GenerateROMList
//---------------------------------------------------------------------
BOOL CROMListScreen::Helper_GenerateROMList( CStdString &path )
{
	std::vector< CStdString > zipFileNames;
	WIN32_FIND_DATA findData;

    // Check if the rom path is on a smb share
  if( path.Left(6) == "smb://")
  {
    CSMBHandler smbHandler;
    smbHandler.GetDirectory(path, ".zip", zipFileNames, TRUE);
  }
  else
  {
    HANDLE findHandle = FindFirstFile( (path + "\\*").c_str(),
                                        &findData );
    if( findHandle == INVALID_HANDLE_VALUE )
    {
      PRINTMSG( T_INFO, "Could not find files!" );
      return TRUE;  // This is not really an error
    }

    DWORD i = 0;
    for( ; ; ++i )
    {
        // Notify the user that a new zip has been found
      DrawProgressbarMessage( m_displayDevice, 
                              "Searching for ROM files", 
                              findData.cFileName, 
                              i, 
                              0 );

      // Remove the extension
      CStdString filename = findData.cFileName;
      filename.ToLower();

      INT32 extensionIDX = filename.Find( ".zip" );
      if( extensionIDX != -1 )
      {
        filename = filename.Left( extensionIDX );
        zipFileNames.push_back( filename );
      }
      if( !FindNextFile( findHandle, &findData ) )
        break;
    }

    FindClose( findHandle );
  }

	PRINTMSG( T_INFO, "Found %lu zip files!", zipFileNames.size() );

  // Check the zip files against the list of all known zip files
	for(DWORD i = 0; i < m_numDrivers && zipFileNames.size(); ++i )
	{
      // Only redraw every 8th ROM, as rendering takes up
      // the vast majority of the overall time
    if( !(i & 0x07) )
    {
      DrawProgressbarMessage( m_displayDevice, 
                              "Checking against known ROM files",
                              m_driverInfoList[i].m_romFileName, 
                              i, 
                              m_numDrivers );
    }

    CStdString driverFileName = m_driverInfoList[i].m_romFileName;
    driverFileName.ToLower();

    std::vector<CStdString>::iterator it = std::find( zipFileNames.begin(), 
                                                      zipFileNames.end(), 
                                                      driverFileName );

    if( it != zipFileNames.end() )
    {
      if( std::find( m_ROMListFull.begin(), m_ROMListFull.end(), i ) == m_ROMListFull.end() )
        m_ROMListFull.push_back( i );

      zipFileNames.erase( it );
    }
	}

	PRINTMSG( T_INFO, "Found %lu games!", m_ROMListFull.size() );

	return TRUE;
}

//-------------------------------------------------------------
//  SaveROMListFile
//-------------------------------------------------------------
BOOL CROMListScreen::SaveROMListFile( void )
{
		// Write the indices to the ROM list file
	CStdString romListFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMLISTFILENAME;
	PRINTMSG( T_INFO, "Store ROM list: %s", romListFile.c_str() );


  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMLISTFILENAME, "w" );
	if( !file )
	{
		PRINTMSG( T_ERROR, "Could not create file %s!", romListFile.c_str() );
		return FALSE;
	}

    // Sign the file
  if( osd_fwrite( file, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		osd_fclose( file );

			// Delete the file
		DeleteFile( romListFile.c_str() );
		return FALSE;
	}


    // Calculate a signature for the list, so we can validate it when reading
    //  to eliminate corrupt data
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

    // Write in a blank signature
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing blank signature!" );
    return FALSE;
  }



    // Define a macro to write a block of data and calculate the data signature
  #define WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        osd_fwrite( file, (_data__), (_dataSize__) ) != (_dataSize__) ) \
    { \
      PRINTMSG( T_ERROR, "Write failed!" ); \
      free( sigData ); \
      osd_fclose( file ); \
      DeleteFile( romListFile.c_str() ); \
      return FALSE; \
    }


    // Write the number of indices
  DWORD numROMs = m_ROMListFull.size();
  WRITEDATA( &numROMs, sizeof(numROMs) );

    // Write the indices
	std::vector<UINT32>::iterator it = m_ROMListFull.begin();
	for( ; it != m_ROMListFull.end(); ++it )
	{
		DWORD val = (*it);
    WRITEDATA( &val, sizeof(val) );

      // Write the ROM status
    val = (DWORD)m_ROMStatus[(*it)];
    WRITEDATA( &val, sizeof(val) );
	}

    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Write in the real signature
  osd_fseek( file, sizeof(DRIVERLIST_FILESTAMP) - 1, SEEK_SET );
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing signature!" );
    return FALSE;
  }

  free( sigData );
	osd_fclose( file );
  return TRUE;
}


//---------------------------------------------------------------------
//  LoadROMListFile
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadROMListFile( void )
{
  m_ROMListFull.clear();
  m_ROMStatus.clear();
  m_driverMetadata.clear();

  UINT32 i = 0;
  for( ; i < m_numDrivers; ++i )
    m_ROMStatus.push_back( STATUS_UNKNOWN );

  std::string romListFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMLISTFILENAME;
	PRINTMSG( T_INFO, "Load ROM list: %s", romListFile.c_str() );
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMLISTFILENAME, "r" );
	if( !file )
	{
    PRINTMSG( T_ERROR, "Failed to open ROM list file %s!", romListFile.c_str() );
    return FALSE;
  }

    // Grab the entire file at once so we can check its signature
  osd_fseek( file, 0, SEEK_END );
  INT32 fileSize = osd_ftell( file );
  osd_fseek( file, 0, SEEK_SET );
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize );
  if( !fileData )
  {
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romListFile.c_str(), 
              fileSize );
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to read file %s!", romListFile.c_str() );
    return FALSE;
  }
  osd_fclose( file );

    // Check the signature
  if( memcmp( fileData, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Bad file signature!" );
    return FALSE;
  }

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to calculate data signature!" );
    return FALSE;
  }
  
    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Data signature mismatch!" );
    return FALSE;
  }
  free( sigData );





    // Define a macro to "read" a block of data and ensure that we're not reading past the end of
    //  the file
  #define READDATA( _data__, _dataSize__, _dataType__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romListFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romListFile.c_str() ); \
        PRINTMSG( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

  #define READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romListFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romListFile.c_str() ); \
        PRINTMSG( T_ERROR, "Attempt to read into NULL destination buffer!" ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }


    // Read in the number of items
  DWORD numItems;
  READDATA_NOMALLOC( &numItems, sizeof(numItems) );

  for( DWORD i = 0; i < numItems; ++i )
  {
    DWORD idx, status;
      // Read the ROM index
    READDATA_NOMALLOC( &idx, sizeof(idx) );
		m_ROMListFull.push_back( idx );

      // Read the ROM status
    READDATA_NOMALLOC( &status, sizeof(status) );
    m_ROMStatus[idx] = (ROMStatus)status;
  }
  free( fileData );

  if( !LoadROMMetadataFile() )
    PRINTMSG( T_ERROR, "Failed loading ROM metadata file!" );


  return TRUE;
}


//---------------------------------------------------------------------
//	SaveROMMetadataFile
//---------------------------------------------------------------------
BOOL CROMListScreen::SaveROMMetadataFile( void )
{
  CStdString romMetadataFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMMETADATAFILENAME;

		// Write the indices to the ROM list file
	PRINTMSG( T_TRACE, "SaveROMMetadataFile" );
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "w" );
	if( !file )
	{
		PRINTMSG( T_ERROR, "Could not create file %s!", romMetadataFile.c_str() );
		return FALSE;
	}

    // Sign the file
  if( osd_fwrite( file, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		osd_fclose( file );

			// Delete the file
		DeleteFile( romMetadataFile.c_str() );
		return FALSE;
	}

    // Calculate a signature for the list, so we can validate it when reading
    //  to eliminate corrupt data
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

    // Write in a blank signature
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing blank signature!" );
    return FALSE;
  }



    // Define a macro to write a block of data and calculate the data signature
  #define METADATA_WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        osd_fwrite( file, (_data__), (_dataSize__) ) != (_dataSize__) ) \
    { \
      PRINTMSG( T_ERROR, "Write failed!" ); \
      free( sigData ); \
      osd_fclose( file ); \
      DeleteFile( romMetadataFile.c_str() ); \
      return FALSE; \
    }


    // Write the number of indices
  DWORD numROMs = m_driverMetadata.size();
  METADATA_WRITEDATA( &numROMs, sizeof(numROMs) );

    // Write the indices
	std::vector<MAMEoXDriverMetadata_t>::iterator it = m_driverMetadata.begin();
	for( ; it != m_driverMetadata.end(); ++it )
	{
    DWORD len;

    METADATA_WRITEDATA( &(*it).m_romIndex, sizeof((*it).m_romIndex) );

      // Write the filename
    if( (*it).m_romFileName )
    {
      len = strlen( (*it).m_romFileName ) + 1;
      METADATA_WRITEDATA( &len, sizeof(len) );
      METADATA_WRITEDATA( (*it).m_romFileName, len );
    }
    else
    {
      len = 1;
      METADATA_WRITEDATA( &len, sizeof(len) );
      METADATA_WRITEDATA( "", 1 );
    }

      // Write the favorite status
    METADATA_WRITEDATA( &(*it).m_favoriteStatus, sizeof((*it).m_favoriteStatus) );

      // Write the number of times played
    METADATA_WRITEDATA( &(*it).m_timesPlayed, sizeof((*it).m_timesPlayed) );

      // Write the catver.ini [Category] 
    if( (*it).m_genre )
    {
      len = strlen( (*it).m_genre ) + 1;
      METADATA_WRITEDATA( &len, sizeof(len) );
      METADATA_WRITEDATA( (*it).m_genre, len );
    }
    else
    {
      len = 1;
      METADATA_WRITEDATA( &len, sizeof(len) );
      METADATA_WRITEDATA( "", 1 );
    }

      // Write the catver.ini [VersionAdded] 
    if( (*it).m_versionAdded )
    {
      len = strlen( (*it).m_versionAdded ) + 1;
      METADATA_WRITEDATA( &len, sizeof(len) );
      METADATA_WRITEDATA( (*it).m_versionAdded, len );
    }
    else
    {
      len = 0;
      METADATA_WRITEDATA( &len, sizeof(len) );
    }
	}


    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Write in the real signature
  osd_fseek( file, sizeof(DRIVERLIST_FILESTAMP) - 1, SEEK_SET );
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing signature!" );
    return FALSE;
  }

  free( sigData );
	osd_fclose( file );
  return TRUE;
}

//---------------------------------------------------------------------
//	LoadROMMetadataFile
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadROMMetadataFile( void )
{
	CStdString romMetadataFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMMETADATAFILENAME;
  UINT32 i = 0;
  MAMEoXDriverMetadata_t emptyMetadata;
  memset( &emptyMetadata, 0, sizeof(emptyMetadata) );
   
    // Set the driver metadata to defaults
  m_driverMetadata.clear();
  for( ; i < m_numDrivers; ++i )
  {
    emptyMetadata.m_romFileName = strdup( m_driverInfoList[i].m_romFileName );
    emptyMetadata.m_romIndex = i;
    m_driverMetadata.push_back( emptyMetadata );
  }

	PRINTMSG( T_INFO, "Load ROM metadata: %s", romMetadataFile.c_str() );
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "r" );
	if( !file )
	{
    PRINTMSG( T_ERROR, "Failed to open ROM metadata file %s!", romMetadataFile.c_str() );
    return FALSE;
  }

    // Grab the entire file at once so we can check its signature
  osd_fseek( file, 0, SEEK_END );
  INT32 fileSize = osd_ftell( file );
  osd_fseek( file, 0, SEEK_SET );
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize );
  if( !fileData )
  {
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romMetadataFile.c_str(), 
              fileSize );
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to read file %s!", romMetadataFile.c_str() );
    return FALSE;
  }
  osd_fclose( file );

    // Check the signature
  if( memcmp( fileData, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) )
  {
    free( fileData );
      // If the signature is not correct, it's quite possible that the file is
      // intact but from an older version. Try updating it to the latest
      // version so we don't lose usage statistics
		return UpdateROMMetadataFile();
  }

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to calculate data signature!" );
    return FALSE;
  }
  

    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Data signature mismatch!" );
    return FALSE;
  }
  free( sigData );


    // Define a macro to "read" a block of data and ensure that we're not reading past the end of
    //  the file
  #define METADATA_READDATA( _data__, _dataSize__, _dataType__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romMetadataFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

  #define METADATA_READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romMetadataFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG( T_ERROR, "Attempt to read into NULL destination buffer!" ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

    // Read in the number of items
  DWORD numItems;
  METADATA_READDATA_NOMALLOC( &numItems, sizeof(numItems) );

  for( DWORD i = 0; i < numItems; ++i )
  {
    DWORD len;
    MAMEoXDriverMetadata_t metadata;
    memset( &metadata, 0, sizeof(metadata) );

      // Read the index
    METADATA_READDATA_NOMALLOC( &metadata.m_romIndex, sizeof(metadata.m_romIndex) );

      // Read the filename
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    METADATA_READDATA( metadata.m_romFileName, len, char );

      // Read the favorite status
    METADATA_READDATA_NOMALLOC( &metadata.m_favoriteStatus, sizeof(metadata.m_favoriteStatus) );

    METADATA_READDATA_NOMALLOC( &metadata.m_timesPlayed, sizeof(metadata.m_timesPlayed) );

      // Read the catver.ini [Category] 
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    if( len )
    {
      METADATA_READDATA( metadata.m_genre, len, char );
    }
    else
      metadata.m_genre = strdup( "Unknown" );

      // Read the catver.ini [VersionAdded] 
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    if( len )
    {
      METADATA_READDATA( metadata.m_versionAdded, len, char );
    }
    else
      metadata.m_versionAdded = strdup( "Unknown" );


      // Add the loaded data to the metadata list
    m_driverMetadata[metadata.m_romIndex] = metadata;
  }
  free( fileData );

  return TRUE;
}

//---------------------------------------------------------------------
//	UpdateROMMetadataFile
//---------------------------------------------------------------------
BOOL CROMListScreen::UpdateROMMetadataFile( void )
{
	CStdString romMetadataFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMMETADATAFILENAME;
  UINT32 i = 0;
  MAMEoXDriverMetadata_t emptyMetadata;
  memset( &emptyMetadata, 0, sizeof(emptyMetadata) );
   
    // Set the driver metadata to defaults
  m_driverMetadata.clear();
  for( ; i < m_numDrivers; ++i )
  {
    emptyMetadata.m_romIndex = i;
    m_driverMetadata.push_back( emptyMetadata );
  }

	PRINTMSG( T_INFO, "Load ROM metadata: %s", romMetadataFile.c_str() );
	osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "r" );
	if( !file )
	{
    PRINTMSG( T_ERROR, "Failed to open ROM metadata file %s!", romMetadataFile.c_str() );
    return FALSE;
  }

    // Grab the entire file at once so we can check its signature
  osd_fseek( file, 0, SEEK_END );
  INT32 fileSize = osd_ftell( file );
  osd_fseek( file, 0, SEEK_SET );
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize );
  if( !fileData )
  {
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romMetadataFile.c_str(), 
              fileSize );
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to read file %s!", romMetadataFile.c_str() );
    return FALSE;
  }
  osd_fclose( file );

    // Don't waste time on the signature, it's invalid regardless

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to calculate data signature!" );
    return FALSE;
  }
  


    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG( T_ERROR, "Data signature mismatch!" );
    return FALSE;
  }
  free( sigData );


    // Define a macro to "read" a block of data and ensure that we're not reading past the end of
    //  the file
  #define METADATA_READDATA( _data__, _dataSize__, _dataType__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romMetadataFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

  #define METADATA_READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      free( fileData ); \
      DeleteFile( romMetadataFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG( T_ERROR, "Attempt to read into NULL destination buffer!" ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

    // Read in the number of items
  DWORD numItems;
  METADATA_READDATA_NOMALLOC( &numItems, sizeof(numItems) );

  for( DWORD i = 0; i < numItems; ++i )
  {
    DWORD len;
    MAMEoXDriverMetadata_t metadata;

      // Read the index
    METADATA_READDATA_NOMALLOC( &metadata.m_romIndex, sizeof(metadata.m_romIndex) );

      // Read the filename
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    METADATA_READDATA( metadata.m_romFileName, len, char );

      // Read the favorite status
    METADATA_READDATA_NOMALLOC( &metadata.m_favoriteStatus, sizeof(metadata.m_favoriteStatus) );

    METADATA_READDATA_NOMALLOC( &metadata.m_timesPlayed, sizeof(metadata.m_timesPlayed) );

      // Read the catver.ini [Category] 
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    METADATA_READDATA( metadata.m_genre, len, char );

      // Read the catver.ini [VersionAdded] 
    METADATA_READDATA_NOMALLOC( &len, sizeof(len) );
    METADATA_READDATA( metadata.m_versionAdded, len, char );

    DrawProgressbarMessage( m_displayDevice, 
                            "Updating metadata file", 
                            metadata.m_romFileName, 
                            i, 
                            0 );

      // Find the index for this ROM
    UINT32 index;
      // Go through the m_driverInfoList and find the entry matching that read from the XML file
    for( index = 0; index < m_numDrivers && stricmp( m_driverInfoList[index].m_romFileName, metadata.m_romFileName ); ++i )
      ;

    m_driverMetadata[index] = metadata;
  }
  free( fileData );

  return TRUE;
}

//---------------------------------------------------------------------
//	ImportCatverINI
//---------------------------------------------------------------------
BOOL CROMListScreen::ImportCatverINI( void )
{
  CStdString iniFileName = g_FileIOConfig.m_GeneralPath;
  iniFileName += "\\catver.ini";

  DrawProgressbarMessage( m_displayDevice, 
                          "Parsing catver.ini file...", 
                          "", 
                          0xFFFFFFFF, 
                          0 );


    // Make sure there's a file to parse
  osd_file *file = osd_fopen( FILETYPE_HISTORY, 0, "catver.ini", "r" );
  if( !file )
    return FALSE;
  osd_fclose( file );

  CSystem_IniFile iniFile( iniFileName );

  std::vector<MAMEoXDriverMetadata_t>::iterator it = m_driverMetadata.begin();
  UINT32 i = 0;
  for( ; it != m_driverMetadata.end(); ++it, ++i )
  {
    if( (*it).m_romFileName )
    {
      CStdString tempStr;
      

        // Only draw every 8th item
      if( !(i & 0x07) ) 
        DrawProgressbarMessage( m_displayDevice, 
                                "Importing catver.ini file", 
                                (*it).m_romFileName, 
                                i, 
                                m_driverMetadata.size() );

        // Grab the category
      tempStr = iniFile.GetProfileString( "Category", (*it).m_romFileName, "[Unknown]" );
      if( (*it).m_genre )
        free( (*it).m_genre );
      (*it).m_genre = strdup( tempStr.c_str() );

        // Grab the version added
      tempStr = iniFile.GetProfileString( "VerAdded", (*it).m_romFileName, "[Unknown]" );
      if( (*it).m_versionAdded )
        free( (*it).m_versionAdded );
      (*it).m_versionAdded = strdup( tempStr.c_str() );
    }
  }

  return TRUE;
}

//---------------------------------------------------------------------
//	RefreshROMStatus
//---------------------------------------------------------------------
BOOL CROMListScreen::RefreshROMStatus( void )
{
  DrawProgressbarMessage( m_displayDevice, 
                          "Parsing romstatus.xml file...", 
                          "", 
                          0xFFFFFFFF, 
                          0 );
  if( !LoadROMStatusFile() )
  {
    PRINTMSG( T_ERROR, "Failed to load the ROM status file" );
    return FALSE;
  }

  DrawProgressbarMessage( m_displayDevice, 
                          "Saving cache file...", 
                          "", 
                          0xFFFFFFFF, 
                          0 );


  if( !SaveROMListFile() )
  {
    PRINTMSG( T_ERROR, "Failed to save the ROM list file" );
    return FALSE;
  }

  return TRUE;
}

//---------------------------------------------------------------------
//	LoadROMStatusFile
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadROMStatusFile( void )
{
  m_ROMStatus.clear();

  UINT32 i = 0;
  for( ; i < m_numDrivers; ++i )
    m_ROMStatus.push_back( STATUS_UNKNOWN );

    // Open the ROMStatus.XML file (in the General directory)
  osd_file *file = osd_fopen( FILETYPE_HISTORY, 0, ROMSTATUSFILENAME, "r" );
  if( !file )
  {
    PRINTMSG( T_ERROR, "Failed to open rom status file: %s", ROMSTATUSFILENAME );
    return FALSE;
  }

  char buffer[2] = {0};
  UINT32 entryNumber = 0; // Throw away index number for DrawZipData usage
  CStdString tagName = "";
  CStdString closingTagName = "";
  CStdString tagData = "";
  CStdString romName = "";
  ROMStatus  romStatus = STATUS_UNKNOWN;

  while( osd_fread( file, buffer, 1 ) )
  {
    /*
      <Rom name="005" version="?">
        <Status>Working</Status> 
        <Statusnumber>1</Statusnumber> 
      </Rom>
    */
    
      // Go through the XML file, reading each <ROM> entry into the temp vars
    if( buffer[0] == '<' )
    {
        // Read the entire tag
      if( !Helper_ReadXMLTag( file, &tagName ) )
        break;

      if( tagName == "</rom>" )
      {
          // Just finished reading a ROM entry
          // Go through the m_driverInfoList and find the entry matching that read from the XML file
        for( i = 0; i < m_numDrivers && stricmp( m_driverInfoList[i].m_romFileName, romName.c_str() ); ++i )
          ;

        DrawProgressbarMessage( m_displayDevice, 
                                "Parsing status file", 
                                romName.c_str(), 
                                entryNumber++, 
                                0 );

          // Set m_ROMStatus at the same index
        if( i != m_numDrivers )
          m_ROMStatus[i] = romStatus;
        else
          PRINTMSG( T_INFO, "Unknown driver %s in status XML file!", "" );

        romName = "";
        romStatus = STATUS_UNKNOWN;
      }
      else if( tagName.substr( 0, 2 ) == "<?" || 
          tagName.substr( 0, 2 ) == "<!" || 
          tagName.substr( 0, 2 ) == "</" ||
          tagName == "<roms>" )
      {
          // Just ignore it
      }
      else if( tagName.substr( 0, 5 ) == "<rom " )
      {
          // Don't parse the whole tag, as we want the subfields separate
        romName = tagName.substr( 11 );
        i = romName.find( '"' );
        if( i != romName.npos )
          romName = romName.substr( 0, i );
      }
      else
      {
        tagData = "";

findEndTag:
          // Read until we find the end of this tag
        while( osd_fread( file, buffer, 1 ) && buffer[0] != '<' )
          tagData += buffer;

          // Read the tag
        if( !Helper_ReadXMLTag( file, &closingTagName ) )
          break;

        if( closingTagName.substr( 0, 2 ) != "</" || closingTagName.substr( 2 ) != tagName.substr( 1 ) )
        {
            // We've found a tag, but it's not the one we're looking for. Add it to the data and continue
            // searching
          tagData += closingTagName;
          goto findEndTag;
        }


          // Handle the basic tags
        if( tagName == "<statusnumber>" )
        {
            // Parse out the status ID
          UINT32 statusNum;
          sscanf( tagData.c_str(), "%lu", &statusNum );
          if( statusNum < STATUS_MAX )
            romStatus = (ROMStatus)statusNum;
        }
      }
    }
  }

  osd_fclose( file );

  return TRUE;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CROMListScreen::MoveCursor( CInputManager &gp, BOOL useSpeedBanding )
{
   	// Handle user input
  if(  gp.IsButtonPressed( GP_B | GP_A ) )
  {
    m_shouldGenerateROMList = TRUE;
    gp.WaitForNoButton();
  }
  else if( gp.IsOneOfButtonsPressed( GP_A ) )
	{
			// Run the selected ROM
    if( GetCurrentGameIndex() != INVALID_ROM_INDEX  )
    {
      if( !m_gameSelected )
      {
        m_gameSelected = TRUE;
        ++m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed;
        PRINTMSG( T_INFO, "m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed %lu", m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed );
      }
    }
	}
	else if( gp.IsButtonPressed( GP_WHITE ) )
	{
    if( m_options.m_displayMode < (DM_LAST - 1) )
      m_options.m_displayMode = (ROMListDisplayMode)(m_options.m_displayMode + 1);
    else
      m_options.m_displayMode = DM_VERBOSELIST;

    UpdateSortedList();
    m_numLinesInList = m_currentSortedList.size();
    gp.WaitForNoButton();
	}
  else if( gp.IsButtonPressed( GP_BLACK ) )
  {
      // No need to regenerate the list, just switch to
      // the noclones (or clones) list
    m_options.m_hideFiltered = !m_options.m_hideFiltered;

    UpdateSortedList();
    m_numLinesInList = m_currentSortedList.size();
    gp.WaitForNoButton();
  }
  else if( gp.IsButtonPressed( GP_DPAD_LEFT ) && m_favoriteStatusChangeDelay == 0.0f )
  {
    DWORD currentGameIdx = GetCurrentGameIndex();
    if( currentGameIdx != INVALID_ROM_INDEX )
    {
      MAMEoXDriverMetadata_t  &metadata = m_driverMetadata[currentGameIdx];
      switch( metadata.m_favoriteStatus )
      {
        // *** FS_INDIFFERENT *** //
      case FS_INDIFFERENT:
        metadata.m_favoriteStatus = FS_DISLIKE;
        break;

        // *** FS_STRONGDISLIKE *** //
      case FS_STRONGDISLIKE:
        // *** FS_DISLIKE *** //
      case FS_DISLIKE:
        metadata.m_favoriteStatus = FS_STRONGDISLIKE;
        break;

        // *** FS_LIKE *** //
      case FS_LIKE:
        metadata.m_favoriteStatus = FS_INDIFFERENT;
        break;
        
        // *** FS_STRONGLIKE *** //
      case FS_STRONGLIKE:
        metadata.m_favoriteStatus = FS_LIKE;
        break;
      }

      m_favoriteStatusChangeDelay = DPADCURSORMOVE_TIMEOUT;
    }
  }
  else if( gp.IsButtonPressed( GP_DPAD_RIGHT ) && m_favoriteStatusChangeDelay == 0.0f )
  {
    DWORD currentGameIdx = GetCurrentGameIndex();
    if( currentGameIdx != INVALID_ROM_INDEX )
    {
      MAMEoXDriverMetadata_t  &metadata = m_driverMetadata[currentGameIdx];
      switch( metadata.m_favoriteStatus )
      {
        // *** FS_INDIFFERENT *** //
      case FS_INDIFFERENT:
        metadata.m_favoriteStatus = FS_LIKE;
        break;

        // *** FS_STRONGDISLIKE *** //
      case FS_STRONGDISLIKE:
        metadata.m_favoriteStatus = FS_DISLIKE;
        break;

        // *** FS_DISLIKE *** //
      case FS_DISLIKE:
        metadata.m_favoriteStatus = FS_INDIFFERENT;
        break;

        // *** FS_LIKE *** //
      case FS_LIKE:
        // *** FS_STRONGLIKE *** //
      case FS_STRONGLIKE:
        metadata.m_favoriteStatus = FS_STRONGLIKE;
        break;
      }

      m_favoriteStatusChangeDelay = DPADCURSORMOVE_TIMEOUT;
    }
  }

		// General idea taken from XMAME
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
	if( !lastTime )
	{
			// lastTime isn't valid yet, so wait for the next frame
		lastTime = curTime;
		return;
	}
	lastTime = curTime;

		// Decrement the dpad movement timer
	if( m_dpadCursorDelay > 0.0f )
	{
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        !gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) )
			m_dpadCursorDelay = 0.0f;
	}

  if( m_favoriteStatusChangeDelay > 0.0f )
  {
		m_favoriteStatusChangeDelay -= elapsedTime;
    if( m_favoriteStatusChangeDelay < 0.0f || 
        !gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_DPAD_RIGHT ) )
			m_favoriteStatusChangeDelay = 0.0f;
  }

  if( gp.IsButtonPressed( GP_Y ) && !m_superscrollMode )
  {
    m_dpadCursorDelay = 0.0f;
    m_superscrollMode = TRUE;
  }
  else if( m_superscrollMode && !gp.IsButtonPressed( GP_Y ) )
  {
      //Exit superscrollmode
    m_dpadCursorDelay = 0.0f;
    m_superscrollMode = FALSE;
  }

  if( m_superscrollMode )
    SuperScrollModeMoveCursor( gp, elapsedTime );
  else
    NormalModeMoveCursor( gp, elapsedTime );
}

//---------------------------------------------------------------------
//	SuperScrollModeMoveCursor
//---------------------------------------------------------------------
void CROMListScreen::SuperScrollModeMoveCursor( CInputManager &gp, FLOAT elapsedTime )
{
  if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    UINT32 startPos = m_currentSuperscrollIndex;

    if( ++m_currentSuperscrollIndex >= m_superscrollJumpTable.size() )
      m_currentSuperscrollIndex = 0;
	}
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    UINT32 startPos = m_currentSuperscrollIndex;

    if( !m_currentSuperscrollIndex )
      m_currentSuperscrollIndex = m_superscrollJumpTable.size() - 1;
    else
      --m_currentSuperscrollIndex;
	}

    // Jump the cursor if there's a place to jump to, and we're not 
    // already in the section starting with the selected superscroll value
  CStdString curVal, curROMVal;
  GetFriendlySuperscrollIndexStringForROM( &curROMVal, GetAbsoluteCursorPosition() );
  GetFriendlySuperscrollIndexStringForJumpTableIndex( &curVal, m_currentSuperscrollIndex );

  if( curVal != curROMVal )
    SetAbsoluteCursorPosition( m_superscrollJumpTable[m_currentSuperscrollIndex] );
}

//---------------------------------------------------------------------
//  NormalModeMoveCursor
//---------------------------------------------------------------------
void CROMListScreen::NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime )
{
		// General idea taken from XMAME

		// The combined trigger offset, scaled to the range [-1.0f,1.0f]
  FLOAT cursorVelocity = 0.0f;

    // Loop through all the gamepads, checking the right and left triggers
    // until one with an offset is found
  for( UINT32 i = 0; i < 4 && cursorVelocity == 0.0f; ++i )
  {
    CGamepad *gamepad;
    if( (gamepad = gp.GetGamepad( i )) )
      cursorVelocity = (FLOAT)(gamepad->GetAnalogButtonState( GP_RIGHT_TRIGGER ) - gamepad->GetAnalogButtonState( GP_LEFT_TRIGGER ));
  }

  cursorVelocity /= 256.0f;

		// Reset the speed band timeout
	if( cursorVelocity < 0.99f && cursorVelocity > -0.99f )
		m_cursorSpeedBandTimeout = 0.0f;
	else
	{
		m_cursorSpeedBandTimeout += elapsedTime;
		if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTEST )
			m_cursorSpeedBandTimeout = SBTIMEOUT_FASTEST;
	}

		// DPAD overrides the triggers
  if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that subtracting 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = -1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
	else
	{
			// Handle the dead zone
		if( cursorVelocity > -DEADZONE && cursorVelocity < DEADZONE )
				return;

			// Rescale the cursor velocity to be [-CURSOR_SPEED,CURSOR_SPEED] without the deadzone
    if( cursorVelocity > 0 )
      cursorVelocity -= DEADZONE;
    else
      cursorVelocity += DEADZONE;
		cursorVelocity *= DEADZONE_RECTIFIER;
    cursorVelocity *= CURSOR_SPEED;
	}

	  // Apply speed bands
	if( m_cursorSpeedBandTimeout == SBTIMEOUT_FASTEST )
		cursorVelocity *= SBMULTIPLIER_FASTEST;
	else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTER )
		cursorVelocity *= SBMULTIPLIER_FASTER;
	else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FAST )
		cursorVelocity *= SBMULTIPLIER_FAST;


	DWORD pageSize = (m_currentSortedList.size() < MAXPAGESIZE ? m_currentSortedList.size() : MAXPAGESIZE);
	ULONG pageHalfwayPoint = (pageSize >> 1);
	ULONG maxPageOffset = m_currentSortedList.size() - pageSize;

	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_cursorPosition < pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) < pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (ULONG)(cursorVelocity + m_pageOffset) <= maxPageOffset )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) <= (FLOAT)maxPageOffset )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity -= ((FLOAT)maxPageOffset - m_pageOffset);
					m_pageOffset = (FLOAT)maxPageOffset;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (ULONG)m_pageOffset > maxPageOffset )
			m_pageOffset = (FLOAT)maxPageOffset;
		if( (ULONG)m_cursorPosition > (pageSize - 1) )
			m_cursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_cursorPosition > pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) > pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (LONG)(cursorVelocity + m_pageOffset) >= 0 )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) >= 0.0f )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity += m_pageOffset;
					m_pageOffset = 0.0f;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (LONG)m_pageOffset < 0 )
			m_pageOffset = 0.0f;
		if( (LONG)m_cursorPosition < 0 )
			m_cursorPosition = 0.0f;
	}


    // If the superscroll index is not set to this ROM, make it so
  CStdString curVal, currentROMVal;
  GetFriendlySuperscrollIndexStringForJumpTableIndex( &curVal, m_currentSuperscrollIndex );

  if( GetCurrentGameIndex() != INVALID_ROM_INDEX )
  {
    GetFriendlySuperscrollIndexStringForROM( &currentROMVal, GetAbsoluteCursorPosition() );
    if( curVal != currentROMVal )
    {
      for( m_currentSuperscrollIndex = 0; m_currentSuperscrollIndex < m_superscrollJumpTable.size(); ++m_currentSuperscrollIndex )
      {
        GetFriendlySuperscrollIndexStringForJumpTableIndex( &curVal, m_currentSuperscrollIndex );

        if( curVal == currentROMVal )
          return;
      }

      PRINTMSG( T_ERROR, "Could not find valid jump table index for ROM %lu [%s]!", GetCurrentGameIndex(), currentROMVal.c_str() );
      m_currentSuperscrollIndex = 0;
    }
  }
}


//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CROMListScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil



    // Render the backdrop texture
  RenderBackdrop();
  m_menuRenderer->Draw( FALSE, FALSE );

  if( GetCurrentGameIndex() != INVALID_ROM_INDEX )
  {
    switch( m_options.m_displayMode )
    {
      // *** DM_VERBOSELIST *** //
    case DM_VERBOSELIST:
      DrawVerboseList();
      break;

      // *** DM_SIMPLELIST *** //
    case DM_SIMPLELIST:
      DrawSimpleList();
      break;

      // *** DM_DETAILED *** //
    case DM_DETAILED:
      DrawDetailedList();
      break;
    }
  }

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


//---------------------------------------------------------------------
//	DrawVerboseList
//---------------------------------------------------------------------
void CROMListScreen::DrawVerboseList( void )
{
	WCHAR name[512];

  FLOAT textHeight = m_fontSet.SmallThinFontHeight();
  FLOAT selectedItemYPos = (textHeight * (ULONG)m_cursorPosition);

    // Render the highlight bar for the selected item
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos + textHeight, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos + textHeight, 1.0f, 1.0f );
  m_displayDevice->End();


    // Draw the vertical spacers
  m_displayDevice->Begin( D3DPT_QUADLIST );

    #define DRAWSPACER( spacerLeft ) { \
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SPACER_COLOR ); \
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, (spacerLeft), LISTPOS_TOP, 1.0f, 1.0f ); \
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SPACER_COLOR ); \
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, (spacerLeft) + SPACER_WIDTH, LISTPOS_TOP, 1.0f, 1.0f ); \
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SPACER_COLOR ); \
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, (spacerLeft) + SPACER_WIDTH, LISTPOS_BOTTOM, 1.0f, 1.0f ); \
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SPACER_COLOR ); \
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, (spacerLeft), LISTPOS_BOTTOM, 1.0f, 1.0f ); \
    }

      // Manufacturer
    DRAWSPACER( MANUFACTURER_COLUMN - 6 )

      // Year column
    DRAWSPACER( YEAR_COLUMN - 6 )

      // Number of players column
    DRAWSPACER( NUMPLAYERS_COLUMN - 6 )

      // Clone column
    DRAWSPACER( CLONE_COLUMN - 6 )

  m_displayDevice->End();


	m_fontSet.SmallThinFont().Begin();

	  swprintf( name, L"Names (%s)  ", m_options.m_hideFiltered ? L"Filtered " : L"Full List" );
    if( m_superscrollMode )
    {
        // Display the superscroll character
      WCHAR displayString[64] = L"";
      CStdString tempBuf;
      GetFriendlySuperscrollIndexStringForJumpTableIndex( &tempBuf, m_currentSuperscrollIndex );

      mbstowcs( displayString, tempBuf.c_str(), 64 );
		  swprintf( &name[wcslen(name)], L"[%s]", displayString );
    }

	  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, 
                                        TITLEBAR_ROW, 
                                        HEADER_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED,
                                        MANUFACTURER_COLUMN - (NAME_COLUMN + COLUMN_PADDING) );
		m_fontSet.SmallThinFont().DrawText( MANUFACTURER_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Manufacturer" );
		m_fontSet.SmallThinFont().DrawText( YEAR_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Year" );
    m_fontSet.SmallThinFont().DrawText( NUMPLAYERS_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"#P" );
		m_fontSet.SmallThinFont().DrawText( CLONE_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Clone" );

		  // Render the ROM info
	  FLOAT yPos = 0.0f;
    FLOAT pageSize = GetCurrentPageSize();
	  ULONG absListIDX = (ULONG)m_pageOffset;

	  for( DWORD i = absListIDX; i < pageSize + absListIDX; ++i )
	  {
      DWORD color = ITEM_COLOR;

        // Set the ROM color based on its status
      if( m_options.m_showROMStatus )
      {
        ROMStatus &status = m_ROMStatus[m_currentSortedList[i]];
        switch( status )
        {
        case STATUS_UNKNOWN:
        case STATUS_WORKING:
        default:
            // Do nothing, ITEM_COLOR is already correct
          break;

        case STATUS_SLOW:
          color = ITEM_WARNING_COLOR;
          break;

        case STATUS_CRASH:
        case STATUS_OUT_OF_MEMORY:
        case STATUS_GENERAL_NONWORKING:
          color = ITEM_NONWORKING_COLOR;
          break;
        }
      }

		  mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_description, 255 );
		  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN,
                                          FIRSTDATA_ROW + yPos,
                                          color,
                                          name,
                                          XBFONT_TRUNCATED,
                                          MANUFACTURER_COLUMN - (NAME_COLUMN + COLUMN_PADDING) );

			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_manufacturer, 255 );
			m_fontSet.SmallThinFont().DrawText( MANUFACTURER_COLUMN,
                                          FIRSTDATA_ROW + yPos,
                                          color,
                                          name,
                                          XBFONT_TRUNCATED,
                                          YEAR_COLUMN - (MANUFACTURER_COLUMN + COLUMN_PADDING) );


			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_year, 255 );
			m_fontSet.SmallThinFont().DrawText( YEAR_COLUMN, 
                                          FIRSTDATA_ROW + yPos, 
                                          color, 
                                          name, 
                                          XBFONT_TRUNCATED,
                                          NUMPLAYERS_COLUMN - (YEAR_COLUMN + COLUMN_PADDING) );


			swprintf( name, L"%lu", m_driverInfoList[m_currentSortedList[i]].m_numPlayers );
			m_fontSet.SmallThinFont().DrawText( NUMPLAYERS_COLUMN,
                                          FIRSTDATA_ROW + yPos,
                                          color,
                                          name,
                                          XBFONT_TRUNCATED,
                                          CLONE_COLUMN - (NUMPLAYERS_COLUMN + COLUMN_PADDING) );

			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_cloneFileName, 255 );
			m_fontSet.SmallThinFont().DrawText( CLONE_COLUMN, 
                                          FIRSTDATA_ROW + yPos,
                                          color,
                                          name,
                                          XBFONT_TRUNCATED,
                                          TEXTBOX_RIGHT - (CLONE_COLUMN + COLUMN_PADDING) );

			  // Inc the Y position
		  yPos += textHeight;
	  }
	m_fontSet.SmallThinFont().End();


    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    // Draw scroll up icon
  if( (DWORD)m_pageOffset )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  if( (DWORD)m_pageOffset < (m_numLinesInList - (DWORD)pageSize) )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}

//---------------------------------------------------------------------
//	DrawSimpleList
//---------------------------------------------------------------------
void CROMListScreen::DrawSimpleList( void )
{
	WCHAR name[512];

  FLOAT textHeight = m_fontSet.SmallThinFontHeight();
  FLOAT selectedItemYPos = (textHeight * (ULONG)m_cursorPosition);

    // Render the highlight bar for the selected item
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  m_displayDevice->SetTexture( 0, NULL );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos + textHeight, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos + textHeight, 1.0f, 1.0f );
  m_displayDevice->End();

	m_fontSet.SmallThinFont().Begin();

	  swprintf( name, L"Names (%s)  ", m_options.m_hideFiltered ? L"Filtered " : L"Full List" );
    if( m_superscrollMode )
    {
        // Display the superscroll character
      WCHAR displayString[64] = L"";
      CStdString tempBuf;
      GetFriendlySuperscrollIndexStringForJumpTableIndex( &tempBuf, m_currentSuperscrollIndex );

      mbstowcs( displayString, tempBuf.c_str(), 64 );
		  swprintf( &name[wcslen(name)], L"[%s]", displayString );
    }

	  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, 
                                        TITLEBAR_ROW, 
                                        HEADER_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );

		  // Render the ROM info
	  FLOAT yPos = 0.0f;
    FLOAT pageSize = GetCurrentPageSize();
	  ULONG absListIDX = (ULONG)m_pageOffset;

	  for( DWORD i = absListIDX; i < pageSize + absListIDX; ++i )
	  {
      DWORD color = ITEM_COLOR;

        // Set the ROM color based on its status
      if( m_options.m_showROMStatus )
      {
        ROMStatus &status = m_ROMStatus[m_currentSortedList[i]];
        switch( status )
        {
        case STATUS_UNKNOWN:
        case STATUS_WORKING:
        default:
            // Do nothing, ITEM_COLOR is already correct
          break;

        case STATUS_SLOW:
          color = ITEM_WARNING_COLOR;
          break;

        case STATUS_CRASH:
        case STATUS_OUT_OF_MEMORY:
        case STATUS_GENERAL_NONWORKING:
          color = ITEM_NONWORKING_COLOR;
          break;
        }
      }

		  mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_description, 255 );
		  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN,
                                          FIRSTDATA_ROW + yPos,
                                          color,
                                          name,
                                          XBFONT_TRUNCATED,
                                          TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );

			  // Inc the Y position
		  yPos += textHeight;
	  }
	m_fontSet.SmallThinFont().End();


    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    // Draw scroll up icon
  if( (DWORD)m_pageOffset )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  if( (DWORD)m_pageOffset < (m_numLinesInList - (DWORD)pageSize) )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}

//---------------------------------------------------------------------
//	DrawDetailedList
//---------------------------------------------------------------------
void CROMListScreen::DrawDetailedList( void )
{
	WCHAR name[256];
  WCHAR temp[256];

  FLOAT textHeight = m_fontSet.SmallThinFontHeight();
  FLOAT selectedItemYPos = (textHeight * (ULONG)m_cursorPosition);

  MAMEDriverData_t        &driverData = m_driverInfoList[GetCurrentGameIndex()];
  ROMStatus               &status     = m_ROMStatus[GetCurrentGameIndex()];
  MAMEoXDriverMetadata_t  &metadata   = m_driverMetadata[GetCurrentGameIndex()];

    // Sort of a hack, keep track of the last selected ROM
    // so we can load a screenshot file whenever we change to
    // a new ROM
  static UINT32 lastGameIndex = INVALID_ROM_INDEX;
  static UINT32 lastScreenshotIndex = 0;
  if( GetCurrentGameIndex() != lastGameIndex )
  {
    lastGameIndex = GetCurrentGameIndex();

      // Display the screenshot if there is one
      // Todo: mode to display full screen screenshots
    LoadScreenshotFile( 0 );
  }

    // Display the screenshot
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     FALSE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    FALSE );

  FLOAT screenshotLeft = DETAIL_SCREENSHOT_LEFT;
  FLOAT screenshotRight = DETAIL_SCREENSHOT_RIGHT;
  FLOAT screenshotTop = DETAIL_SCREENSHOT_TOP;
  FLOAT screenshotBottom = DETAIL_SCREENSHOT_BOTTOM;

		// Flip the width and height
  if( driverData.m_screenOrientation & ORIENTATION_SWAP_XY )
	{
      // Set the height = to the standard width
    screenshotBottom = screenshotTop + (DETAIL_SCREENSHOT_RIGHT-DETAIL_SCREENSHOT_LEFT);

      // Calculate left based on the new aspect ratio
    screenshotLeft = screenshotRight - ( (FLOAT)(DETAIL_SCREENSHOT_RIGHT - DETAIL_SCREENSHOT_LEFT) * 3.0f / 4.0f);
	}




  if( m_screenshotTexture )
  {
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
	  m_displayDevice->SetTexture( 0, m_screenshotTexture );

    m_displayDevice->Begin( D3DPT_QUADLIST );      

        // Apply screenshot rotations
      POINT ul, ur, br, bl;
      if( driverData.m_screenOrientation & ORIENTATION_SWAP_XY )
      {
        ul.x = driverData.m_screenOrientation & ORIENTATION_FLIP_X ? m_screenshotRect.left : m_screenshotRect.right;
        ul.y = driverData.m_screenOrientation & ORIENTATION_FLIP_Y ? m_screenshotRect.top : m_screenshotRect.bottom;

        ur.x = driverData.m_screenOrientation & ORIENTATION_FLIP_X ? m_screenshotRect.left : m_screenshotRect.right;
        ur.y = driverData.m_screenOrientation & ORIENTATION_FLIP_Y ? m_screenshotRect.bottom : m_screenshotRect.top;

        br.x = driverData.m_screenOrientation & ORIENTATION_FLIP_X ? m_screenshotRect.right : m_screenshotRect.left;
        br.y = driverData.m_screenOrientation & ORIENTATION_FLIP_Y ? m_screenshotRect.bottom : m_screenshotRect.top;

        bl.x = driverData.m_screenOrientation & ORIENTATION_FLIP_X ? m_screenshotRect.right : m_screenshotRect.left;
        bl.y = driverData.m_screenOrientation & ORIENTATION_FLIP_Y ? m_screenshotRect.top : m_screenshotRect.bottom;
      }
      else
      {
        bl.x = ul.x = m_screenshotRect.left;
        ur.y = ul.y = m_screenshotRect.top;
        
        br.x = ur.x = m_screenshotRect.right; 
        br.y = bl.y = m_screenshotRect.bottom;

        if( driverData.m_screenOrientation & ORIENTATION_FLIP_X )
        {
          UINT32 temp = bl.x;
          bl.x = ul.x = ur.x;
          br.x = ur.x = temp;
        }

        if( driverData.m_screenOrientation & ORIENTATION_FLIP_Y )
        {
          UINT32 temp = bl.y;
          bl.y = br.y = ul.y;
          ul.y = ur.y = temp;
        }
      }

      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, ul.x, ul.y );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotTop, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, ur.x, ur.y );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotTop, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, br.x, br.y );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotBottom, 1.0f, 1.0f );

      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, bl.x, bl.y );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotBottom, 1.0f, 1.0f );
    m_displayDevice->End();

/*
      // Draw the screenshot index
	  m_fontSet.SmallThinFont().Begin();
      swprintf( name, L"[%lu]", 0 );
	    m_fontSet.SmallThinFont().DrawText( DETAIL_SCREENSHOTCAPTION_X, 
                                          DETAIL_SCREENSHOTCAPTION_Y, 
                                          ITEM_COLOR, 
                                          name,
                                          XBFONT_CENTER_X );
*/
    m_fontSet.SmallThinFont().End();
  }
  else
  {
      // Render a black rect where the screenshot would go and put "No Screenshot" text in it
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
    m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	  m_displayDevice->SetTexture( 0, m_screenshotTexture );

    m_displayDevice->Begin( D3DPT_QUADLIST );      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotTop, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotTop, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotBottom, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotBottom, 1.0f, 1.0f );
    m_displayDevice->End();

      // Draw the screenshot index
	  m_fontSet.SmallThinFont().Begin();
      swprintf( name, L"[%lu]", 0 );
	    m_fontSet.SmallThinFont().DrawText( screenshotLeft + ((screenshotRight - screenshotLeft) / 2.0f), 
                                          screenshotTop + ((screenshotBottom - screenshotTop) / 2.0f), 
                                          NOSCREENSHOTTEXT_COLOR, 
                                          L"No Screenshots",
                                          XBFONT_CENTER_X );
    m_fontSet.SmallThinFont().End();
  }


	m_fontSet.SmallThinFont().Begin();

	  mbstowcs( name, driverData.m_description, 255 );
    wcscat( name, m_options.m_hideFiltered ? L"    (Filtered)  " : L"    (Full List)  " );
    if( m_superscrollMode )
    {
        // Display the superscroll character
      WCHAR displayString[64] = L"";
      CStdString tempBuf;
      GetFriendlySuperscrollIndexStringForJumpTableIndex( &tempBuf, m_currentSuperscrollIndex );

      mbstowcs( displayString, tempBuf.c_str(), 64 );
		  swprintf( &name[wcslen(name)], L"[%s]", displayString );
    }

	  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, 
                                        TITLEBAR_ROW, 
                                        HEADER_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );

		  // Render the ROM info

      //--- Display the ROM status ---------------------------
    DWORD color = ITEM_COLOR;
    swprintf( name, L"ROM Status: " );
    switch( status )
    {
        // *** STATUS_UNKNOWN *** //
      case STATUS_UNKNOWN:
      default:
        wcscat( name, L"Unknown" );
        break;

        // *** STATUS_WORKING *** //
      case STATUS_WORKING:
        wcscat( name, L"Working" );
        break;

        // *** STATUS_SLOW *** //
      case STATUS_SLOW:
        wcscat( name, L"Runs slowly" );
        color = ITEM_WARNING_COLOR;
        break;

        // *** STATUS_CRASH *** //
      case STATUS_CRASH:
        wcscat( name, L"Crashes" );
        color = ITEM_NONWORKING_COLOR;
        break;

        // *** STATUS_OUT_OF_MEMORY *** //
      case STATUS_OUT_OF_MEMORY:
        wcscat( name, L"Out of memory" );
        color = ITEM_NONWORKING_COLOR;
        break;

        // *** STATUS_GENERAL_NONWORKING *** //
      case STATUS_GENERAL_NONWORKING:
        wcscat( name, L"General nonworking" );
        color = ITEM_NONWORKING_COLOR;
        break;
    }
    m_fontSet.SmallThinFont().DrawText( DETAIL_ROMSTATUS_X, 
                                        DETAIL_ROMSTATUS_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_ROMSTATUS_X + COLUMN_PADDING) );


      //--- Display the number of players ---------------------------
    swprintf( name, L"Number of players: %lu", driverData.m_numPlayers );
    m_fontSet.SmallThinFont().DrawText( DETAIL_NUMPLAYERS_X, 
                                        DETAIL_NUMPLAYERS_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_NUMPLAYERS_X + COLUMN_PADDING) );


      //--- Display the manufacturer ---------------------------
    if( driverData.m_manufacturer )
      mbstowcs( temp, driverData.m_manufacturer, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Manufacturer: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_MANUFACTURER_X, 
                                        DETAIL_MANUFACTURER_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_MANUFACTURER_X + COLUMN_PADDING) );

      //--- Display the year ---------------------------
    if( driverData.m_year )
      mbstowcs( temp, driverData.m_year, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Year: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_YEAR_X, 
                                        DETAIL_YEAR_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_YEAR_X + COLUMN_PADDING) );


      //--- Display the parent ROM ---------------------------
    if( driverData.m_cloneFileName )
      mbstowcs( temp, driverData.m_cloneFileName, 255 );
    else
      wcscpy( temp, L"None" );
    swprintf( name, L"Parent ROM filename: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_PARENT_X, 
                                        DETAIL_PARENT_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_PARENT_X + COLUMN_PADDING) );


      //--- Display the genre ---------------------------
    if( metadata.m_genre )
      mbstowcs( temp, metadata.m_genre, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Genre: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_GENRE_X, 
                                        DETAIL_GENRE_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_GENRE_X + COLUMN_PADDING) );

      //--- Display the version this rom was added to MAME ---------------------------
    if( metadata.m_versionAdded )
      mbstowcs( temp, metadata.m_versionAdded, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Version added: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_VERSIONADDED_X, 
                                        DETAIL_VERSIONADDED_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_VERSIONADDED_X + COLUMN_PADDING) );

      //--- Display the version this rom was added to MAME ---------------------------
    swprintf( name, L"Times played: %lu", metadata.m_timesPlayed );
    m_fontSet.SmallThinFont().DrawText( DETAIL_TIMESPLAYED_X, 
                                        DETAIL_TIMESPLAYED_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_TIMESPLAYED_X + COLUMN_PADDING) );

      //--- Display the version this rom was added to MAME ---------------------------
    switch( metadata.m_favoriteStatus )
    {
      // *** FS_INDIFFERENT *** //
    case FS_INDIFFERENT:
      wcscpy( temp, L"Average" );
      break;

      // *** FS_STRONGDISLIKE *** //
    case FS_STRONGDISLIKE:
      wcscpy( temp, L"Strongly disliked" );
      break;

      // *** FS_DISLIKE *** //
    case FS_DISLIKE:
      wcscpy( temp, L"Disliked" );
      break;

      // *** FS_LIKE *** //
    case FS_LIKE:
      wcscpy( temp, L"Liked" );
      break;

      // *** FS_STRONGLIKE *** //
    case FS_STRONGLIKE:
      wcscpy( temp, L"Strongly liked" );
      break;
    }
    swprintf( name, L"Favorite status: %s", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_FAVORITESTATUS_X, 
                                        DETAIL_FAVORITESTATUS_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_FAVORITESTATUS_X + COLUMN_PADDING) );

      //--- Display the filename ---------------------------
    if( driverData.m_romFileName )
      mbstowcs( temp, driverData.m_romFileName, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"ROM filename: %s.zip", temp );
    m_fontSet.SmallThinFont().DrawText( DETAIL_FILENAME_X, 
                                        DETAIL_FILENAME_Y, 
                                        ITEM_COLOR, 
                                        name, 
                                        XBFONT_TRUNCATED, 
                                        screenshotLeft - (DETAIL_FILENAME_X + COLUMN_PADDING) );


	m_fontSet.SmallThinFont().End();


    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    // Draw scroll up icon
  if( GetAbsoluteCursorPosition() )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

    // Draw scroll down icon
  if( GetAbsoluteCursorPosition() < m_numLinesInList - 1 )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}

//---------------------------------------------------------------------
//  LoadScreenshotFile
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadScreenshotFile( UINT32 index )
{
    // Release the old screenshot data
  SAFE_RELEASE( m_screenshotTexture );

  MAMEDriverData_t &driverData = m_driverInfoList[GetCurrentGameIndex()];
  CStdString filename = driverData.m_romFileName;
  if( index )
  {
    char num[8];
    sprintf( num, "%4.4lu", index - 1 );
    filename += num;
  }
  filename += ".png";

  osd_file *file = osd_fopen( FILETYPE_SCREENSHOT, 0, filename.c_str(), "r" );
  if( !file )
    return FALSE;

    // Read the png signature
  {
    #define PNG_BYTES_TO_CHECK 4 
    char buf[PNG_BYTES_TO_CHECK];

   if( osd_fread( file, buf, PNG_BYTES_TO_CHECK ) != PNG_BYTES_TO_CHECK )
   {
     osd_fclose( file );
     return FALSE;
   }

   if( png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK ) )
   {
     osd_fclose( file );
     return FALSE;
   }
  }

  png_structp  png_ptr;
  png_infop    info_ptr;

  /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
  png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if( !png_ptr )
  {
    osd_fclose( file );
    return FALSE;
  }

  /* Allocate/initialize the memory for image information.  REQUIRED. */
  info_ptr = png_create_info_struct( png_ptr );
  if( !info_ptr )
  {
    osd_fclose( file );
    png_destroy_read_struct( &png_ptr, png_infopp_NULL, png_infopp_NULL );
    return FALSE;
  }

  /* Set error handling if you are using the setjmp/longjmp method (this is
  * the normal method of doing things with libpng).  REQUIRED unless you
  * set up your own error handlers in the png_create_read_struct() earlier.
  */

  if( setjmp( png_jmpbuf(png_ptr) ) )
  {
    /* Free all of the memory associated with the png_ptr and info_ptr */
    png_destroy_read_struct( &png_ptr, &info_ptr, png_infopp_NULL );
    osd_fclose( file );

      /* If we get here, we had a problem reading the file */
    return FALSE;
  }

  /* If you are using replacement read functions, instead of calling
  * png_init_io() here you would call:
  */
  png_set_read_fn( png_ptr, (void *)file, Helper_PNGRead );
  /* where user_io_ptr is a structure you want available to the callbacks */


  /* If we have already read some of the signature */
  png_set_sig_bytes( png_ptr, PNG_BYTES_TO_CHECK );

  memset( &m_screenshotRect, 0, sizeof(m_screenshotRect) );
  int bit_depth, color_type, interlace_type;
  /* The call to png_read_info() gives us all of the information from the
  * PNG file before the first IDAT (image data chunk).  REQUIRED
  */
  png_read_info(png_ptr, info_ptr);

  png_get_IHDR( png_ptr, 
                info_ptr, 
                (png_uint_32*)&m_screenshotRect.right, 
                (png_uint_32*)&m_screenshotRect.bottom, 
                &bit_depth, 
                &color_type,
                &interlace_type, 
                int_p_NULL, 
                int_p_NULL );


  /* Some suggestions as to how to get a screen gamma value */
  DOUBLE screen_gamma = 1.0;  /* A good guess for Mac systems */

  /* Tell libpng to handle the gamma conversion for you.  The final call
  * is a good guess for PC generated images, but it should be configurable
  * by the user at run time by the user.  It is strongly suggested that
  * your application support gamma correction.
  */

  int intent;

  if (png_get_sRGB(png_ptr, info_ptr, &intent))
    png_set_gamma(png_ptr, screen_gamma, 0.45455);
  else
  {
    double image_gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
        png_set_gamma(png_ptr, screen_gamma, image_gamma);
    else
        png_set_gamma(png_ptr, screen_gamma, 0.45455);
  }

  /* Turn on interlace handling.  REQUIRED if you are not using
  * png_read_image().  To see how to handle interlacing passes,
  * see the png_read_row() method below:
  */
  INT32 number_passes = png_set_interlace_handling(png_ptr);

  /* Optional call to gamma correct and add the background to the palette
  * and update info structure.  REQUIRED if you are expecting libpng to
  * update the palette for you (ie you selected such a transform above).
  */
  png_read_update_info(png_ptr, info_ptr);

  png_bytep *row_pointers = (png_bytep*)calloc( 1, m_screenshotRect.bottom * sizeof(png_bytep) );
  if( !row_pointers )
  {
    PRINTMSG( T_ERROR, "Out of memory allocating row_pointers array!" );
    png_destroy_read_struct( &png_ptr, &info_ptr, png_infopp_NULL );
    osd_fclose( file );
    return FALSE;
  }

  UINT32 y;
  for( y = 0; y < m_screenshotRect.bottom; ++y )
  {
    if( !(row_pointers[y] = (png_bytep)malloc( png_get_rowbytes(png_ptr,info_ptr))) )
    {
      PRINTMSG( T_ERROR, "Out of memory allocating row_pointers[%lu]!", y );
      png_destroy_read_struct( &png_ptr, &info_ptr, png_infopp_NULL );
      osd_fclose( file );
      return FALSE;
    }
  }

  png_read_image(png_ptr, row_pointers );

    /* read rest of file, and get additional chunks in info_ptr - REQUIRED */
  png_read_end( png_ptr, info_ptr );

    // Create a new texture and read the data into it
  if( (D3DXCreateTexture( m_displayDevice,
                          m_screenshotRect.right,
                          m_screenshotRect.bottom,
													1,									// Mip levels
                          0,                  // Usage
													D3DFMT_LIN_X8R8G8B8,// Format
													0,		              // Pool (unused)
                          &m_screenshotTexture )) != S_OK )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    PRINTMSG( T_ERROR, "Failed to create texture for screenshot" );
    PRINTMSG( T_INFO, "Memory status" );
    PRINTMSG( T_INFO, "Physical:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPhys );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPhys );
    PRINTMSG( T_INFO, "Page File:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPageFile );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPageFile );
    PRINTMSG( T_INFO, "Virtual:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailVirtual );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalVirtual );

    png_destroy_read_struct( &png_ptr, &info_ptr, png_infopp_NULL );
    osd_fclose( file );
		return FALSE;
	}


		// Grab the surface description
	D3DSURFACE_DESC desc;
  m_screenshotTexture->GetLevelDesc( 0, &desc );

    // Lock the region so we can render to it later
  D3DLOCKED_RECT lockedRect;
	m_screenshotTexture->LockRect(	0, &lockedRect, NULL, 0 );
  memset( lockedRect.pBits, 0, desc.Width * desc.Height * sizeof(DWORD) );

  switch( info_ptr->color_type )
  {
    // *** PNG_COLOR_TYPE_PALETTE *** //
  case PNG_COLOR_TYPE_PALETTE:
    {
      D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
      for( y = 0; y < m_screenshotRect.bottom; ++y )
      {
        D3DCOLOR *dest = destRow;
        BYTE *src = (BYTE*)row_pointers[y];
        for( UINT32 x = 0; x < m_screenshotRect.right; ++x )
        {      
          png_color &color = info_ptr->palette[*(src++)];
          *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
        }
        free( row_pointers[y] );
        row_pointers[y] = NULL;
        destRow += desc.Width;
      }
    }
    break;

    // *** PNG_COLOR_TYPE_RGB *** //
  case PNG_COLOR_TYPE_RGB:
    {
      D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
      for( y = 0; y < m_screenshotRect.bottom; ++y )
      {
        D3DCOLOR *dest = destRow;
        BYTE *src = (BYTE*)row_pointers[y];
        for( UINT32 x = 0; x < m_screenshotRect.right; ++x )
        {      
          *(dest++) = D3DCOLOR_XRGB( *(src), *(src+1), *(src+2) );
          src += 3;
        }
        free( row_pointers[y] );
        row_pointers[y] = NULL;
        destRow += desc.Width;
      }
    }
    break;
  }
  free( row_pointers );
  row_pointers = NULL;

    /* clean up after the read, and free any memory allocated - REQUIRED */
  png_destroy_read_struct( &png_ptr, &info_ptr, png_infopp_NULL );

  osd_fclose( file );

  return TRUE;
}

//---------------------------------------------------------------------
//	RemoveCurrentGameIndex
//---------------------------------------------------------------------
BOOL CROMListScreen::RemoveCurrentGameIndex( void )
{
	UINT32 curCursorPos = (ULONG)m_pageOffset + (ULONG)m_cursorPosition;
	std::vector<UINT32>::iterator it = m_currentSortedList.begin();
	for( UINT32 i = 0; i < curCursorPos; ++i )
		++it;


  UINT32 index = (*it);
	m_currentSortedList.erase( it );

    // Remove the item from the full listing
  it = std::find( m_ROMListFull.begin(), m_ROMListFull.end(), index );
  if( it != m_ROMListFull.end() )
    m_ROMListFull.erase( it );

    // Remove the item from the filtered listing
  it = std::find( m_ROMListFiltered.begin(), m_ROMListFiltered.end(), index );
  if( it != m_ROMListFiltered.end() )
    m_ROMListFiltered.erase( it );

    // Write out the new list to file
  if( !SaveROMListFile() )
  {
    PRINTMSG( T_INFO, "Failed to store the ROM list file!" );
    return FALSE;
  }

  return TRUE;
}

//---------------------------------------------------------------------
//	GenerateSuperscrollJumpTable
//---------------------------------------------------------------------
BOOL CROMListScreen::MoveCurrentGameToBackupDir( void )
{
  if( Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_RomPath0 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_RomPath1 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_RomPath2 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_RomPath3 ) )
      return TRUE;
  return FALSE;
}

//---------------------------------------------------------------------
//	Helper_MoveCurrentGameToBackupDir
//---------------------------------------------------------------------
BOOL CROMListScreen::Helper_MoveCurrentGameToBackupDir( CStdString &path )
{
		// Move the currently selected game to the backup dir
	UINT32 romIDX = GetCurrentGameIndex();

    // Don't allow removing from a samba share
  if( path.Left(5) != "smb:\\" )
  {
    std::string oldPath = path + "\\";
    oldPath += m_driverInfoList[romIDX].m_romFileName;
    oldPath += ".zip";

    std::string newPath = g_ROMBackupPath;
    newPath += "\\";
    newPath += m_driverInfoList[romIDX].m_romFileName;
    newPath += ".zip";

    // Make sure the backup dir exists
    CreateDirectory( g_ROMBackupPath, NULL );

    PRINTMSG( T_INFO, "Moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );

      // For some reason MoveFile seems to fail if I move from one partition to
      // another. For this reason, do a copy & delete
//    if( !MoveFile( oldPath.c_str(), newPath.c_str() ) )
    if( !CopyFile( oldPath.c_str(), newPath.c_str(), FALSE ) )
      PRINTMSG( T_ERROR, "Failed moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );
    else
    {
      DeleteFile( oldPath.c_str() );
    	RemoveCurrentGameIndex();
      return TRUE;
    }
  }

  return FALSE;
}


//---------------------------------------------------------------------
//	GenerateSuperscrollJumpTable
//---------------------------------------------------------------------
void CROMListScreen::GenerateSuperscrollJumpTable( void )
{
    // Invalidate the jump table
  m_superscrollJumpTable.clear();
  m_currentSuperscrollIndex = 0;

  CStdString lastVal = "", curVal = "";
  for( UINT32 i = 0; i < m_currentSortedList.size(); ++i )
  {
      // Get the superscroll string for the current ROM
    GetFriendlySuperscrollIndexStringForROM( &curVal, i );

      // If the value is different from the last one, add the
      // index to the table
    if( lastVal != curVal )
    {
      lastVal = curVal;
      m_superscrollJumpTable.push_back( i );
    }
  }
}

//---------------------------------------------------------------------
//	UpdateSortedList
//---------------------------------------------------------------------
void CROMListScreen::UpdateSortedList( void )
{
  UINT32 oldDriverIndex = GetCurrentGameIndex();

  if( !m_options.m_hideFiltered )
    m_currentSortedList = m_ROMListFull;
  else
  {
      // Reapply the filter
    UpdateFilteredList();
    m_currentSortedList = m_ROMListFiltered;
  }


    // Sort the list
  switch( m_options.m_sortMode )
  {
    // *** SM_BYNAME *** //
  default:
  case SM_BYNAME:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_Description );
    break;

    // *** SM_BYMANUFACTURER *** //
  case SM_BYMANUFACTURER:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_Manufacturer );
    break;

    // *** SM_BYYEAR *** //
  case SM_BYYEAR:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_Year );
    break;

    // *** SM_BYPARENT *** //
  case SM_BYPARENT:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_ParentROM );
    break;

    // *** SM_BYGENRE *** //
  case SM_BYGENRE:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_Genre );
    break;

    // *** SM_BYNUMPLAYERS *** //
  case SM_BYNUMPLAYERS:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_NumPlayers );
    break;

    // *** SM_BYROMSTATUS *** //
  case SM_BYROMSTATUS:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_ROMStatus );
    break;

    // *** SM_BYFAVORITESTATUS *** //
  case SM_BYFAVORITESTATUS:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_FavoriteStatus );
    break;

    // *** SM_BYNUMTIMESPLAYED *** //
  case SM_BYNUMTIMESPLAYED:
    std::stable_sort( m_currentSortedList.begin(), m_currentSortedList.end(), Compare_NumTimesPlayed );
    break;
  }

    // Attempt to find the ROM that the user was on in the old sort mode
    // in the new sort mode. If it can't be found, leave the cursor alone
    // if it's less than the list size, or force it to the end of the
    // list otherwise
  if( oldDriverIndex != INVALID_ROM_INDEX )
  {
    UINT32 pos = 0;
    std::vector<UINT32>::iterator i = m_currentSortedList.begin();
    for( ; i != m_currentSortedList.end() && (*i) != oldDriverIndex; ++i, ++pos )
      ;
    if( i != m_currentSortedList.end() )
      SetAbsoluteCursorPosition( pos );
    else
      SetAbsoluteCursorPosition( m_currentSortedList.size() - 1 );
  }
  
    // Generate the superscroll table from the new list
  GenerateSuperscrollJumpTable();

    // Reposition the superscroll cursor in the new table
  if( GetCurrentGameIndex() != INVALID_ROM_INDEX )
  {
    CStdString currentString;
    GetFriendlySuperscrollIndexStringForROM( &currentString, GetAbsoluteCursorPosition() );

    UINT32 &curIDX = m_currentSuperscrollIndex;
    for( curIDX = 0; curIDX < m_superscrollJumpTable.size(); ++curIDX )
    {
      CStdString indexString;
      GetFriendlySuperscrollIndexStringForJumpTableIndex( &indexString, curIDX );

      if( indexString == currentString )
        break;
    }
  }
}


//---------------------------------------------------------------------
//  UpdateFilteredList
//---------------------------------------------------------------------
void CROMListScreen::UpdateFilteredList( void )
{
  m_ROMListFiltered.clear();

  std::vector<UINT32>::iterator i = m_ROMListFull.begin();
  for( ; i != m_ROMListFull.end(); ++i )
  {
    MAMEDriverData_t        &driverData = m_driverInfoList[*i];
    ROMStatus               &driverStatus = m_ROMStatus[*i];
    MAMEoXDriverMetadata_t  &metadata = m_driverMetadata[*i];

      // Filter on CLONE status
    if( m_options.m_filterMode & FM_CLONE &&driverData.m_isClone )
      continue;

      // Filter on the number of players
//    if( m_options.m_filterMode & FM_NUMPLAYERS && driverData.m_numPlayers < m_options.m_numPlayersFilter )
//      continue;

      // Filter on the genre
    if( m_options.m_filterMode & FM_GENRE )
    {
    }

      // Filter out ROMs marked as "SLOW"
    if( m_options.m_filterMode & FM_ROMSTATUS_SLOW && driverStatus == STATUS_SLOW )
      continue;

      // Filter out ROMs marked as "Out of memory"
    if( m_options.m_filterMode & FM_ROMSTATUS_OUTOFMEM && driverStatus == STATUS_OUT_OF_MEMORY )
      continue;

      // Filter out ROMs marked as "Crash"
    if( m_options.m_filterMode & FM_ROMSTATUS_CRASH && driverStatus == STATUS_CRASH )
      continue;

      // Filter out ROMs marked as "Other nonworking"
    if( m_options.m_filterMode & FM_ROMSTATUS_NONWORKING && driverStatus == STATUS_GENERAL_NONWORKING )
      continue;

      // Filter out ROMs marked as "disliked" or "strongly disliked"
    if( m_options.m_filterMode & FM_DISLIKED && (metadata.m_favoriteStatus == FS_STRONGDISLIKE || metadata.m_favoriteStatus == FS_DISLIKE) )
      continue;

    m_ROMListFiltered.push_back( *i );
  }
}

//---------------------------------------------------------------------
//  SetAbsoluteCursorPosition
//---------------------------------------------------------------------
void CROMListScreen::SetAbsoluteCursorPosition( UINT32 pos )
{
  if( pos == INVALID_ROM_INDEX )
    return;

    // Jump the cursor to the selected position
	UINT32 pageSize = (m_currentSortedList.size() < MAXPAGESIZE ? m_currentSortedList.size() : MAXPAGESIZE);
	UINT32 pageHalfwayPoint = (pageSize >> 1);
	UINT32 maxPageOffset = m_currentSortedList.size() - pageSize;

    // Put the page offset at absoluteCursorPos - pageHalwayPoint, or 0
  if( pos <= pageHalfwayPoint || m_currentSortedList.size() < MAXPAGESIZE )
  {
    m_pageOffset = 0.0f;
    m_cursorPosition = (FLOAT)pos;
  }
  else if( pos - pageHalfwayPoint > maxPageOffset )
  {
    m_pageOffset = maxPageOffset;
    m_cursorPosition = pos - maxPageOffset;
  }
  else
  {
    m_pageOffset = (FLOAT)(pos - pageHalfwayPoint);
    m_cursorPosition = (FLOAT)pageHalfwayPoint;
  }
}

//---------------------------------------------------------------------
//  GetFriendlySuperscrollIndexStringForJumpTableIndex
//---------------------------------------------------------------------
void CROMListScreen::GetFriendlySuperscrollIndexStringForJumpTableIndex( CStdString *ret, UINT32 superscrollTableIndex )
{
  UINT32 jumpIndex;

  assert( ret );
  if( superscrollTableIndex >= m_superscrollJumpTable.size() || 
      (jumpIndex = m_superscrollJumpTable[superscrollTableIndex]) == INVALID_ROM_INDEX )
  {
    *ret = "Invalid";
    return;
  }

  GetFriendlySuperscrollIndexStringForROM( ret, jumpIndex );
}

//---------------------------------------------------------------------
//  GetFriendlySuperscrollIndexValue
//---------------------------------------------------------------------
void CROMListScreen::GetFriendlySuperscrollIndexStringForROM( CStdString *ret, UINT32 sortedListIndex )
{
  UINT32 romIndex;
  if( sortedListIndex >= m_currentSortedList.size() || 
      (romIndex = m_currentSortedList[sortedListIndex]) == INVALID_ROM_INDEX )
  {
    *ret = "Invalid";
    return;
  }

  MAMEDriverData_t        &driver = m_driverInfoList[romIndex];
  ROMStatus               &status = m_ROMStatus[romIndex];
  MAMEoXDriverMetadata_t  &metadata = m_driverMetadata[romIndex];

  switch( m_options.m_sortMode )
  {
    // *** SM_BYNAME *** //
  case SM_BYNAME:
  default:
    {
      char buf[2] = {0};
      buf[0] = toupper( driver.m_description[0] );

      if( !(buf[0] >= 'A' && buf[0] <= 'Z') )
        buf[0] = '#';
      *ret = buf;
    }
    break;

    // *** SM_BYMANUFACTURER *** //
  case SM_BYMANUFACTURER:
    if( driver.m_manufacturer && driver.m_manufacturer[0] )
      *ret = driver.m_manufacturer;
    else
      *ret = "-Unknown-";
    break;

    // *** SM_BYYEAR *** //
  case SM_BYYEAR:
    if( driver.m_year && driver.m_year[0] )
      *ret = driver.m_year;
    else
      *ret = "-Unknown-";
    break;

    // *** SM_BYPARENT *** //
  case SM_BYPARENT:
    if( driver.m_cloneFileName && driver.m_cloneFileName[0] )
      *ret = driver.m_cloneFileName;
    else
      *ret = "-None-";
    break;

    // *** SM_BYGENRE *** //
  case SM_BYGENRE:
    if( metadata.m_genre )
      *ret = metadata.m_genre;    
    else
      *ret = "-Unknown-";
    break;

    // *** SM_BYNUMPLAYERS *** //
  case SM_BYNUMPLAYERS:
    {
      char tempBuf[4] = {0};
      sprintf( tempBuf, "%lu", driver.m_numPlayers );
      *ret = tempBuf;
    }
    break;

    // *** SM_BYROMSTATUS *** //
  case SM_BYROMSTATUS:
    switch( status )
    {
    case STATUS_UNKNOWN:
      *ret = "Unknown";
      break;

    case STATUS_WORKING:
      *ret = "Working";
      break;

    case STATUS_SLOW:
      *ret = "Slow";
      break;

    case STATUS_CRASH:
      *ret = "Crashes";
      break;

    case STATUS_OUT_OF_MEMORY:
      *ret = "Out of memory";
      break;

    case STATUS_GENERAL_NONWORKING:
      *ret = "Other non-working";
      break;
    }
    break;


    // *** SM_BYFAVORITESTATUS *** //
  case SM_BYFAVORITESTATUS:
    switch( metadata.m_favoriteStatus )
    {
    case FS_INDIFFERENT:
      *ret = "Average";
      break;

    case FS_STRONGDISLIKE:
      *ret = "Strongly disliked";
      break;

    case FS_DISLIKE:
      *ret = "Disliked";
      break;

    case FS_LIKE:
      *ret = "Liked";
      break;

    case FS_STRONGLIKE:
      *ret = "Strongly liked";
      break;
    }
    break;

    // *** SM_BYNUMTIMESPLAYED *** //
  case SM_BYNUMTIMESPLAYED:
    {
      char tempBuf[32] = {0};
      sprintf( tempBuf, "%lu", metadata.m_timesPlayed );
      *ret = tempBuf;
    }
    break;
  }
}

//---------------------------------------------------------------------
//  Compare_Description
//---------------------------------------------------------------------
static BOOL Compare_Description( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];

  int cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}


//---------------------------------------------------------------------
//  Compare_Manufacturer
//---------------------------------------------------------------------
static BOOL Compare_Manufacturer( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];

    // Compare the manufacturer string, sorting by name if they're equal
  int cmp = stricmp( aDriver.m_manufacturer, bDriver.m_manufacturer );
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_Year
//---------------------------------------------------------------------
static BOOL Compare_Year( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];

    // Compare the year string, sorting by name if they're equal
  int cmp = stricmp( aDriver.m_year, bDriver.m_year );
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_ParentROM
//---------------------------------------------------------------------
static BOOL Compare_ParentROM( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];

    // Compare the parent rom string, sorting by name if they're equal
  int cmp = stricmp( aDriver.m_cloneFileName, bDriver.m_cloneFileName );
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_Genre
//---------------------------------------------------------------------
static BOOL Compare_Genre( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];
  MAMEoXDriverMetadata_t  &aStatus = CROMListScreen::m_driverMetadata[a];
  MAMEoXDriverMetadata_t  &bStatus = CROMListScreen::m_driverMetadata[b];

  int cmp = 0;
  if( aStatus.m_genre && bStatus.m_genre )
    cmp = stricmp( aStatus.m_genre, bStatus.m_genre );
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_NumPlayers
//---------------------------------------------------------------------
static BOOL Compare_NumPlayers( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];

    // Sort by number of players or name string (more players go
    // towards the bottom)
  int cmp = aDriver.m_numPlayers - bDriver.m_numPlayers;
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_ROMStatus
//---------------------------------------------------------------------
static BOOL Compare_ROMStatus( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMListScreen::m_driverInfoList[b];
  ROMStatus        &aStatus = CROMListScreen::m_ROMStatus[a];
  ROMStatus        &bStatus = CROMListScreen::m_ROMStatus[b];

    // Sort by the rom status, putting lower (better working) numbers first
  int cmp = bStatus - aStatus;
  if( !cmp )
    return stricmp( aDriver.m_description, bDriver.m_description ) < 0;

  return cmp > 0;
}

//---------------------------------------------------------------------
//  Compare_NumTimesPlayed
//---------------------------------------------------------------------
static BOOL Compare_NumTimesPlayed( UINT32 a, UINT32 b )
{
  MAMEDriverData_t        &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t        &bDriver = CROMListScreen::m_driverInfoList[b];
  MAMEoXDriverMetadata_t  &aStatus = CROMListScreen::m_driverMetadata[a];
  MAMEoXDriverMetadata_t  &bStatus = CROMListScreen::m_driverMetadata[b];

    // Sort by the rom status, putting higher (more often played) numbers first
  int cmp = bStatus.m_timesPlayed - aStatus.m_timesPlayed;
  if( !cmp )
    return stricmp( aDriver.m_description, bDriver.m_description ) < 0;

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_FavoriteStatus
//---------------------------------------------------------------------
static BOOL Compare_FavoriteStatus( UINT32 a, UINT32 b )
{
  MAMEDriverData_t        &aDriver = CROMListScreen::m_driverInfoList[a];
  MAMEDriverData_t        &bDriver = CROMListScreen::m_driverInfoList[b];
  MAMEoXDriverMetadata_t  &aStatus = CROMListScreen::m_driverMetadata[a];
  MAMEoXDriverMetadata_t  &bStatus = CROMListScreen::m_driverMetadata[b];

    // Sort by the rom favorite status, putting better rated first
  if( aStatus.m_favoriteStatus != bStatus.m_favoriteStatus )
  {
      // Hack because 0 is actually neutral
    UINT32 aNumericStatus, bNumericStatus;
    switch( aStatus.m_favoriteStatus )
    {
    case FS_STRONGDISLIKE:
      aNumericStatus = 0;
      break;
    case FS_DISLIKE:
      aNumericStatus = 1;
      break;
    default:
    case FS_INDIFFERENT:
      aNumericStatus = 2;
      break;
    case FS_LIKE:
      aNumericStatus = 3;
      break;
    case FS_STRONGLIKE:
      aNumericStatus = 4;
      break;
    }

    switch( bStatus.m_favoriteStatus )
    {
    case FS_STRONGDISLIKE:
      bNumericStatus = 0;
      break;
    case FS_DISLIKE:
      bNumericStatus = 1;
      break;
    default:
    case FS_INDIFFERENT:
      bNumericStatus = 2;
      break;
    case FS_LIKE:
      bNumericStatus = 3;
      break;
    case FS_STRONGLIKE:
      bNumericStatus = 4;
      break;
    }

    return bNumericStatus < aNumericStatus;
  }
  
  return stricmp( aDriver.m_description, bDriver.m_description ) < 0;
}

//---------------------------------------------------------------------
//  Helper_ReadXMLTag
//---------------------------------------------------------------------
static BOOL Helper_ReadXMLTag( osd_file *file, CStdString *tagName )
{
  char buffer[2] = {0};
  if( !tagName )
    return FALSE;

  *tagName = "<";
  while( osd_fread( file, buffer, 1 ) && buffer[0] != '>' )
  {
    buffer[0] = tolower( buffer[0] );
    if( buffer[0] != '\n' && buffer[0] != '\r' )
      *tagName += buffer;
  }

    // Did we find the end of the tag?
  if( buffer[0] != '>' )
    return FALSE;

    // Cat on the tag end
  *tagName += ">";
  return TRUE;
}

//---------------------------------------------------------------------
//  Helper_PNGRead
//---------------------------------------------------------------------
static void Helper_PNGRead( png_structp pngStruct, png_bytep buf, png_size_t size )
{
  osd_file *file = (osd_file*)png_get_io_ptr( pngStruct );
  osd_fread( file, buf, size );
}



