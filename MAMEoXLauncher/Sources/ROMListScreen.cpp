/**
	* \file			ROMListScreen.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "ROMListScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

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

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	LoadROMList
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadROMList( BOOL bGenerate, BOOL allowClones )
{
	PRINTMSG(( T_TRACE, "LoadROMList" ));

  m_maxPageSize = 15;
	m_ROMListFull.clear();
  m_ROMListFiltered.clear();

	m_displayDevice->Clear(	0L,																// Count
													NULL,															// Rects to clear
													D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
													D3DCOLOR_XRGB(0,0,0),							// Color
													1.0f,															// Z
													0L );															// Stencil
	m_fontSet.DefaultFont().Begin();
	  m_fontSet.DefaultFont().DrawText( 320, 230, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Loading ROM list", XBFONT_CENTER_X );
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
	PRINTMSG(( T_TRACE, "GenerateROMList" ));

  m_shouldGenerateROMList = FALSE;
	m_ROMListFull.clear();
  m_ROMListFiltered.clear();

  g_FileIOConfig.m_romPath0.MakeLower();
  g_FileIOConfig.m_romPath1.MakeLower();
  g_FileIOConfig.m_romPath2.MakeLower();
  g_FileIOConfig.m_romPath3.MakeLower();

	PRINTMSG(( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_romPath0.c_str() ));
  Helper_GenerateROMList( g_FileIOConfig.m_romPath0 );

  if( g_FileIOConfig.m_romPath1 != g_FileIOConfig.m_romPath0 )
  {
	  PRINTMSG(( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_romPath1.c_str() ));
    Helper_GenerateROMList( g_FileIOConfig.m_romPath1 );
  }

  if( g_FileIOConfig.m_romPath2 != g_FileIOConfig.m_romPath1 &&
      g_FileIOConfig.m_romPath2 != g_FileIOConfig.m_romPath0 )
  {
	  PRINTMSG(( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_romPath2.c_str() ));
    Helper_GenerateROMList( g_FileIOConfig.m_romPath2 );
  }

  if( g_FileIOConfig.m_romPath2 != g_FileIOConfig.m_romPath2 &&
      g_FileIOConfig.m_romPath2 != g_FileIOConfig.m_romPath1 &&
      g_FileIOConfig.m_romPath2 != g_FileIOConfig.m_romPath0 )
  {
	  PRINTMSG(( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_romPath3.c_str() ));
    Helper_GenerateROMList( g_FileIOConfig.m_romPath3 );
  }

    // Load the XML status file
  LoadROMStatusFile();
  LoadROMMetadataFile();
  ImportCatverINI();

  if( !SaveROMListFile() )
  {
    PRINTMSG(( T_INFO, "Failed to store the ROM list file!" ));
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

    // Hacks for the ROM-less PatchMAME games
  static BOOL pongAdded = FALSE;

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
      PRINTMSG(( T_INFO, "Could not find files!" ));
      return TRUE;  // This is not really an error
    }

    DWORD i = 0;
    for( ; ; ++i )
    {
        // Notify the user that a new zip has been found
        // Only redraw every 4th ROM, as rendering takes up
        // the vast majority of the overall time
      if( !(i & 0x03) )
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

	PRINTMSG(( T_INFO, "Found %lu zip files!", zipFileNames.size() ));

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


      // Special handling for PatchMAME's ROM-less games
    if( !pongAdded && driverFileName == "pong" )    
    {
      pongAdded = TRUE;
      m_ROMListFull.push_back( i );
    }
    else
    {
        // Standard handling
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
	}

	PRINTMSG(( T_INFO, "Found %lu games!", m_ROMListFull.size() ));

	return TRUE;
}

//-------------------------------------------------------------
//  SaveROMListFile
//-------------------------------------------------------------
BOOL CROMListScreen::SaveROMListFile( void )
{
		// Write the indices to the ROM list file
	CStdString romListFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMLISTFILENAME;
	PRINTMSG(( T_INFO, "Store ROM list: %s", romListFile.c_str() ));


  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMLISTFILENAME, "w" );
	if( !file )
	{
		PRINTMSG(( T_ERROR, "Could not create file %s!", romListFile.c_str() ));
		return FALSE;
	}

    // Sign the file
  if( osd_fwrite( file, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG(( T_ERROR, "Write failed!" ));
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
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

    // Write in a blank signature
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing blank signature!" ));
    return FALSE;
  }



    // Define a macro to write a block of data and calculate the data signature
  #define WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        osd_fwrite( file, (_data__), (_dataSize__) ) != (_dataSize__) ) \
    { \
      PRINTMSG(( T_ERROR, "Write failed!" )); \
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
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Write in the real signature
  osd_fseek( file, sizeof(DRIVERLIST_FILESTAMP) - 1, SEEK_SET );
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing signature!" ));
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
	PRINTMSG(( T_INFO, "Load ROM list: %s", romListFile.c_str() ));
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMLISTFILENAME, "r" );
	if( !file )
	{
    PRINTMSG(( T_ERROR, "Failed to open ROM list file %s!", romListFile.c_str() ));
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
    PRINTMSG(( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romListFile.c_str(), 
              fileSize ));
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to read file %s!", romListFile.c_str() ));
    return FALSE;
  }
  osd_fclose( file );

    // Check the signature
  if( memcmp( fileData, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Bad file signature!" ));
    return FALSE;
  }

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to calculate data signature!" ));
    return FALSE;
  }
  
    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romListFile.c_str() );
    PRINTMSG(( T_ERROR, "Data signature mismatch!" ));
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romListFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) )); \
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romListFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Attempt to read into NULL destination buffer!" )); \
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
    PRINTMSG(( T_ERROR, "Failed loading ROM metadata file!" ));


  return TRUE;
}


//---------------------------------------------------------------------
//	SaveROMMetadataFile
//---------------------------------------------------------------------
BOOL CROMListScreen::SaveROMMetadataFile( void )
{
  CStdString romMetadataFile = DEFAULT_MAMEOXSYSTEMPATH "\\" ROMMETADATAFILENAME;

		// Write the indices to the ROM list file
	PRINTMSG(( T_TRACE, "SaveROMMetadataFile" ));
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "w" );
	if( !file )
	{
		PRINTMSG(( T_ERROR, "Could not create file %s!", romMetadataFile.c_str() ));
		return FALSE;
	}

    // Sign the file
  if( osd_fwrite( file, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG(( T_ERROR, "Write failed!" ));
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
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

    // Write in a blank signature
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing blank signature!" ));
    return FALSE;
  }



    // Define a macro to write a block of data and calculate the data signature
  #define METADATA_WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        osd_fwrite( file, (_data__), (_dataSize__) ) != (_dataSize__) ) \
    { \
      PRINTMSG(( T_ERROR, "Write failed!" )); \
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
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Write in the real signature
  osd_fseek( file, sizeof(DRIVERLIST_FILESTAMP) - 1, SEEK_SET );
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing signature!" ));
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

	PRINTMSG(( T_INFO, "Load ROM metadata: %s", romMetadataFile.c_str() ));
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "r" );
	if( !file )
	{
    PRINTMSG(( T_ERROR, "Failed to open ROM metadata file %s!", romMetadataFile.c_str() ));
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
    PRINTMSG(( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romMetadataFile.c_str(), 
              fileSize ));
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to read file %s!", romMetadataFile.c_str() ));
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
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to calculate data signature!" ));
    return FALSE;
  }
  

    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Data signature mismatch!" ));
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) )); \
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Attempt to read into NULL destination buffer!" )); \
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

	PRINTMSG(( T_INFO, "Load ROM metadata: %s", romMetadataFile.c_str() ));
	osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, ROMMETADATAFILENAME, "r" );
	if( !file )
	{
    PRINTMSG(( T_ERROR, "Failed to open ROM metadata file %s!", romMetadataFile.c_str() ));
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
    PRINTMSG(( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romMetadataFile.c_str(), 
              fileSize ));
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to read file %s!", romMetadataFile.c_str() ));
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
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to calculate data signature!" ));
    return FALSE;
  }
  


    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( romMetadataFile.c_str() );
    PRINTMSG(( T_ERROR, "Data signature mismatch!" ));
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) )); \
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
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        free( fileData ); \
        DeleteFile( romMetadataFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Attempt to read into NULL destination buffer!" )); \
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
    for( index = 0; index < m_numDrivers && stricmp( m_driverInfoList[index].m_romFileName, metadata.m_romFileName ); ++index )
      ;

    m_driverMetadata[i] = metadata;
  }
  free( fileData );

  return TRUE;
}

//---------------------------------------------------------------------
//	ImportCatverINI
//---------------------------------------------------------------------
BOOL CROMListScreen::ImportCatverINI( void )
{
  CStdString iniFileName = g_FileIOConfig.m_generalPath;
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
    PRINTMSG(( T_ERROR, "Failed to load the ROM status file" ));
    return FALSE;
  }

  DrawProgressbarMessage( m_displayDevice, 
                          "Saving cache file...", 
                          "", 
                          0xFFFFFFFF, 
                          0 );


  if( !SaveROMListFile() )
  {
    PRINTMSG(( T_ERROR, "Failed to save the ROM list file" ));
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
    PRINTMSG(( T_ERROR, "Failed to open rom status file: %s", ROMSTATUSFILENAME ));
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

          // Don't redraw for every entry, as that will slow the parsing down drastically
        if( !(entryNumber & 0x03) )
        {
          DrawProgressbarMessage( m_displayDevice, 
                                  "Parsing status file", 
                                  romName.c_str(), 
                                  entryNumber++, 
                                  0 );
        }

          // Set m_ROMStatus at the same index
        if( i != m_numDrivers )
          m_ROMStatus[i] = romStatus;
        else
          PRINTMSG(( T_INFO, "Unknown driver %s in status XML file!", "" ));

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
  if( gp.IsButtonPressed( GP_A ) || gp.IsKeyPressed( VK_RETURN )  )
	{
			// Run the selected ROM
    if( GetCurrentGameIndex() != INVALID_ROM_INDEX  )
    {
      if( !m_gameSelected )
      {
        m_gameSelected = TRUE;
        ++m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed;
        PRINTMSG(( T_INFO, "m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed %lu", m_driverMetadata[GetCurrentGameIndex()].m_timesPlayed ));
      }
    }
	}
  else if( gp.IsButtonPressed( GP_WHITE ) || gp.IsKeyPressed( VK_TAB ) )
	{
    if( m_options.m_displayMode < (DM_LAST - 1) )
      m_options.m_displayMode = (ROMListDisplayMode)(m_options.m_displayMode + 1);
    else
      m_options.m_displayMode = DM_VERBOSELIST;

    UpdateSortedList();
    m_numLinesInList = m_currentSortedList.size();
    gp.WaitForNoButton();
	}
  else if( gp.IsButtonPressed( GP_BLACK ) || gp.IsKeyPressed( VK_SPACE ) )
  {
      // No need to regenerate the list, just switch to
      // the noclones (or clones) list
    m_options.m_hideFiltered = !m_options.m_hideFiltered;

    UpdateSortedList();
    m_numLinesInList = m_currentSortedList.size();
    gp.WaitForNoButton();
  }
  else if( (gp.IsButtonPressed( GP_DPAD_LEFT ) || gp.IsKeyPressed( VK_LEFT )) && m_favoriteStatusChangeDelay == 0.0f )
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
  else if( (gp.IsButtonPressed( GP_DPAD_RIGHT ) || gp.IsKeyPressed( VK_RIGHT )) && m_favoriteStatusChangeDelay == 0.0f )
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
    BYTE vkArrowKeys[2] = { VK_UP, VK_DOWN };
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        (!gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) &&
         !gp.IsOneOfKeysPressed( vkArrowKeys, 2 )) )
			m_dpadCursorDelay = 0.0f;
	}

  if( m_favoriteStatusChangeDelay > 0.0f )
  {
    BYTE vkArrowKeys[2] = { VK_LEFT, VK_RIGHT };
		m_favoriteStatusChangeDelay -= elapsedTime;
    if( m_favoriteStatusChangeDelay < 0.0f || 
        (!gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_DPAD_RIGHT ) &&
         !gp.IsOneOfKeysPressed( vkArrowKeys, 2 )) )
			m_favoriteStatusChangeDelay = 0.0f;
  }

  if( (gp.IsButtonPressed( GP_Y ) || gp.IsKeyPressed( VK_LSHIFT )) && !m_superscrollMode )
  {
    m_dpadCursorDelay = 0.0f;
    m_superscrollMode = TRUE;
  }
  else if( m_superscrollMode && !(gp.IsButtonPressed( GP_Y ) || gp.IsKeyPressed( VK_LSHIFT )) )
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
  if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) 
       && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    UINT32 startPos = m_currentSuperscrollIndex;

    if( ++m_currentSuperscrollIndex >= m_superscrollJumpTable.size() )
      m_currentSuperscrollIndex = 0;
	}
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) 
            && m_dpadCursorDelay == 0.0f )
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

  if( curVal != curROMVal && curVal != "Invalid" )
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
  if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) && m_dpadCursorDelay == 0.0f )
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


	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA );
	if( area )
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;
	else
		m_maxPageSize = 15;

	DWORD pageSize = (m_currentSortedList.size() < m_maxPageSize ? m_currentSortedList.size() : m_maxPageSize);
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

      PRINTMSG(( T_ERROR, "Could not find valid jump table index for ROM %lu [%s]!", GetCurrentGameIndex(), currentROMVal.c_str() ));
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

	if( !g_loadedSkin )
		return;

    // Render the backdrop texture
  RenderBackdrop();

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_FOOTER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_FOOTER )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_BODY )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_HEADER )->Render( m_displayDevice );





		// ** A ** //
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_A) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_A)->Render( m_displayDevice, L"Play ROM" );

		// ** X ** //
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_X) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_X)->Render( m_displayDevice, L"Help" );


		// ** Y ** //
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_Y) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_Y)->Render( m_displayDevice, L"Fast Jump" );


		// ** Start ** //
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_START) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_START)->Render( m_displayDevice, L"Menu" );


  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    FALSE );
  m_displayDevice->SetTexture( 0, NULL );


  if( GetCurrentGameIndex() != INVALID_ROM_INDEX )
  {
    switch( m_options.m_displayMode )
    {
      // *** DM_VERBOSELIST *** //
    case DM_VERBOSELIST:
			{
					// Draw the dividers
				FLOAT headerLeft = 0.0f;
				FLOAT headerTop = 0.0f;
				if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER ) )
				{
					const CSkinElement *bodyArea = g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_HEADER );
					headerLeft = bodyArea->m_left;
					headerTop = bodyArea->m_top;
				}

				if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_MANUFACTURER_DIVIDER ) )
				{
					g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_MANUFACTURER_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																														headerLeft,
																																																														headerTop );
				}

				if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_YEAR_DIVIDER ) )
				{
					g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_YEAR_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																														headerLeft,
																																																														headerTop );
				}

				if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NUMPLAYERS_DIVIDER ) )
				{
					g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NUMPLAYERS_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																														headerLeft,
																																																														headerTop );
				}

				if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_CLONE_DIVIDER ) )
				{
					g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_CLONE_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																														headerLeft,
																																																														headerTop );
				}

				DrawVerboseList();
			}
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


    //-- Render the scroll up and/or scroll down icons --------------------------------------------
	FLOAT pageSize = GetCurrentPageSize();
	if( (UINT32)m_pageOffset && CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_UP ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_UP )->Render( m_displayDevice );

	if( (UINT32)m_pageOffset < (m_numLinesInList - (UINT32)pageSize) && CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_DOWN ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_DOWN )->Render( m_displayDevice );

	if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


//---------------------------------------------------------------------
//	DrawVerboseList
//---------------------------------------------------------------------
void CROMListScreen::DrawVerboseList( void )
{
	if( !CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA ) )
	{
		PRINTMSG(( T_ERROR, "SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA invalid!" ));
		return;
	}

	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA );

	WCHAR name[512];
	FLOAT selectedItemYPos = area->m_top + (area->m_singleRowHeight * (ULONG)m_cursorPosition);

	if( area->m_highlightBar )
		area->m_highlightBar->RenderAsOffset( m_displayDevice, area->m_left, selectedItemYPos );

    // Draw the dividers
	FLOAT bodyLeft = 0.0f;
	FLOAT bodyTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY ) )
	{
		const CSkinElement *bodyArea = g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_BODY );
		bodyLeft = bodyArea->m_left;
		bodyTop = bodyArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_MANUFACTURER_DIVIDER ) )
	{
		g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_MANUFACTURER_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																											bodyLeft,
																																																											bodyTop );
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_YEAR_DIVIDER ) )
	{
		g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_YEAR_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																											bodyLeft,
																																																											bodyTop );
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_NUMPLAYERS_DIVIDER ) )
	{
		g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_NUMPLAYERS_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																											bodyLeft,
																																																											bodyTop );
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_CLONE_DIVIDER ) )
	{
		g_loadedSkin->GetSkinElementDivider(SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_CLONE_DIVIDER)->RenderAsOffset( m_displayDevice,
																																																											bodyLeft,
																																																											bodyTop );
	}


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

	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NAME_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NAME_TEXT)->RenderAsOffset( m_displayDevice, name, titleBarLeft, titleBarTop );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_MANUFACTERER_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_MANUFACTERER_TEXT)->RenderAsOffset( m_displayDevice, L"Manufacturer", titleBarLeft, titleBarTop );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_YEAR_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_YEAR_TEXT)->RenderAsOffset( m_displayDevice, L"Year", titleBarLeft, titleBarTop );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NUMPLAYERS_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_NUMPLAYERS_TEXT)->RenderAsOffset( m_displayDevice, L"#P", titleBarLeft, titleBarTop );

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_CLONE_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSE_CLONE_TEXT)->RenderAsOffset( m_displayDevice, L"Clone", titleBarLeft, titleBarTop );


		// Render the ROM info
	FLOAT yPos = area->m_top;
  FLOAT pageSize = GetCurrentPageSize();
	ULONG absListIDX = (ULONG)m_pageOffset;

	for( DWORD i = absListIDX; i < pageSize + absListIDX; ++i )
	{
		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWORKING_NAME_TEXT ) )
		{
			const CSkinText *nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWORKING_NAME_TEXT);

				// Set the ROM color based on its status
			if( m_options.m_showROMStatus )
			{
				ROMStatus &status = m_ROMStatus[m_currentSortedList[i]];
				switch( status )
				{
				case STATUS_UNKNOWN:
				case STATUS_WORKING:
				default:
						// Do nothing
					break;

				case STATUS_SLOW:
					if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWARNING_NAME_TEXT) )
						nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWARNING_NAME_TEXT);
					break;

				case STATUS_CRASH:
				case STATUS_OUT_OF_MEMORY:
				case STATUS_GENERAL_NONWORKING:
					if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMNONWORKING_NAME_TEXT) )
						nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMNONWORKING_NAME_TEXT);
					break;
				}
			}

			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_description, 255 );
			nameText->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_MANUFACTURER_TEXT ) )
		{
			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_manufacturer, 255 );
			g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_MANUFACTURER_TEXT)->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_YEAR_TEXT ) )
		{
			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_year, 255 );
			g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_YEAR_TEXT)->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_NUMPLAYERS_TEXT ) )
		{
			swprintf( name, L"%lu", m_driverInfoList[m_currentSortedList[i]].m_numPlayers );
			g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_NUMPLAYERS_TEXT)->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_CLONE_TEXT ) )
		{
			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_cloneFileName, 255 );
			g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_CLONE_TEXT)->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

			// Inc the Y position
		yPos += area->m_singleRowHeight;
	}
}

//---------------------------------------------------------------------
//	DrawSimpleList
//---------------------------------------------------------------------
void CROMListScreen::DrawSimpleList( void )
{
	if( !CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA ) )
	{
		PRINTMSG(( T_ERROR, "SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA invalid!" ));
		return;
	}

	WCHAR name[512];

		// Draw the title bar
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


	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER_SIMPLE_NAME_TEXT ) )
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_SIMPLE_NAME_TEXT)->RenderAsOffset( m_displayDevice, name, titleBarLeft, titleBarTop );






	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA );
	FLOAT selectedItemYPos = area->m_top + (area->m_singleRowHeight * (ULONG)m_cursorPosition);

	if( area->m_highlightBar )
		area->m_highlightBar->RenderAsOffset( m_displayDevice, area->m_left, selectedItemYPos );


		// Render the ROM info
	FLOAT yPos = area->m_top;
  FLOAT pageSize = GetCurrentPageSize();
	ULONG absListIDX = (ULONG)m_pageOffset;

	for( DWORD i = absListIDX; i < pageSize + absListIDX; ++i )
	{
		if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWORKING_NAME_TEXT ) )
		{
			const CSkinText *nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWORKING_NAME_TEXT);

				// Set the ROM color based on its status
			if( m_options.m_showROMStatus )
			{
				ROMStatus &status = m_ROMStatus[m_currentSortedList[i]];
				switch( status )
				{
				case STATUS_UNKNOWN:
				case STATUS_WORKING:
				default:
						// Do nothing
					break;

				case STATUS_SLOW:
					if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWARNING_NAME_TEXT) )
						nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWARNING_NAME_TEXT);
					break;

				case STATUS_CRASH:
				case STATUS_OUT_OF_MEMORY:
				case STATUS_GENERAL_NONWORKING:
					if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMNONWORKING_NAME_TEXT) )
						nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMNONWORKING_NAME_TEXT);
					break;
				}
			}

			mbstowcs( name, m_driverInfoList[ m_currentSortedList[i] ].m_description, 255 );
			nameText->RenderAsOffset( m_displayDevice, name, area->m_left, yPos );
		}

			// Inc the Y position
		yPos += area->m_singleRowHeight;
	}
}

//---------------------------------------------------------------------
//	DrawDetailedList
//---------------------------------------------------------------------
void CROMListScreen::DrawDetailedList( void )
{
	if( !CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA ) )
	{
		PRINTMSG(( T_ERROR, "SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA invalid!" ));
		return;
	}

	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA );

	WCHAR name[256];
  WCHAR temp[256];

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


	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_SCREENSHOT) )
	{
		const CSkinScreenshot *screenshot = g_loadedSkin->GetSkinElementScreenshot(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_SCREENSHOT);

    //swprintf( name, L"[%lu]", 0 );
		screenshot->RenderAsOffset( m_displayDevice,
																m_screenshotTexture,
																m_screenshotRect,
																area->m_left,
																area->m_top,
																driverData.m_screenOrientation & ORIENTATION_SWAP_XY,
																driverData.m_screenOrientation & ORIENTATION_FLIP_X,
																driverData.m_screenOrientation & ORIENTATION_FLIP_Y );
	}

	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_ROMLISTSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_HEADER_DETAIL_NAME_TEXT) )
	{
	  mbstowcs( name, driverData.m_description, 255 );
    wcscat( name, m_options.m_hideFiltered ? L" (Filtered)" : L" (Full List)" );
    if( m_superscrollMode )
    {
        // Display the superscroll character
      WCHAR displayString[64] = L"";
      CStdString tempBuf;
      GetFriendlySuperscrollIndexStringForJumpTableIndex( &tempBuf, m_currentSuperscrollIndex );

      mbstowcs( displayString, tempBuf.c_str(), 64 );
		  swprintf( &name[wcslen(name)], L"[%s]", displayString );
    }

		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_HEADER_DETAIL_NAME_TEXT)->RenderAsOffset( m_displayDevice, name, titleBarLeft, titleBarTop );
	}



		// Render the ROM info
	if( CheckResourceValidity( SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWORKING_NAME_TEXT ) )
	{
		const CSkinText *nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWORKING_NAME_TEXT);
		swprintf( name, L"ROM Status: " );

			// Set the ROM color based on its status
		if( m_options.m_showROMStatus )
		{
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
				if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWARNING_NAME_TEXT) )
					nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWARNING_NAME_TEXT);
				break;

				// *** STATUS_CRASH *** //
			case STATUS_CRASH:
				wcscat( name, L"Crashes" );
				if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT) )
					nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT);
				break;

				// *** STATUS_OUT_OF_MEMORY *** //
			case STATUS_OUT_OF_MEMORY:
				wcscat( name, L"Out of memory" );
				if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT) )
					nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT);
				break;

				// *** STATUS_GENERAL_NONWORKING *** //
			case STATUS_GENERAL_NONWORKING:
				wcscat( name, L"General nonworking" );
				if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT) )
					nameText = g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT);
				break;
			}
		}

		nameText->RenderAsOffset( m_displayDevice, name, area->m_left, area->m_top );
	}

      //--- Display the number of players ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_NUMPLAYERS_TEXT) )
	{
    swprintf( name, L"Number of players: %lu", driverData.m_numPlayers );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_NUMPLAYERS_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Display the manufacturer ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_MANUFACTURER_TEXT) )
	{
    if( driverData.m_manufacturer )
      mbstowcs( temp, driverData.m_manufacturer, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Manufacturer: %s", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_MANUFACTURER_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}


      //--- Display the year ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_YEAR_TEXT) )
	{
    if( driverData.m_year )
      mbstowcs( temp, driverData.m_year, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Year: %s", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_YEAR_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Display the parent ROM ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_PARENT_TEXT) )
	{
    if( driverData.m_cloneFileName )
      mbstowcs( temp, driverData.m_cloneFileName, 255 );
    else
      wcscpy( temp, L"None" );
    swprintf( name, L"Parent ROM filename: %s", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_PARENT_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Display the genre ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_GENRE_TEXT) )
	{
    if( metadata.m_genre )
      mbstowcs( temp, metadata.m_genre, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Genre: %s", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_GENRE_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Display the version this rom was added to MAME ---------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_VERSIONADDED_TEXT) )
	{
    if( metadata.m_versionAdded )
      mbstowcs( temp, metadata.m_versionAdded, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"Version added: %s", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_VERSIONADDED_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Times played ---------------------------------------------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_TIMESPLAYED_TEXT) )
	{
    swprintf( name, L"Times played: %lu", metadata.m_timesPlayed );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_TIMESPLAYED_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Favorite status ---------------------------------------------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FAVORITESTATUS_TEXT) )
	{
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
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FAVORITESTATUS_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}

      //--- Filename ---------------------------------------------------------------
	if( CheckResourceValidity(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FILENAME_TEXT) )
	{
    if( driverData.m_romFileName )
      mbstowcs( temp, driverData.m_romFileName, 255 );
    else
      wcscpy( temp, L"Unknown" );
    swprintf( name, L"ROM filename: %s.zip", temp );
		g_loadedSkin->GetSkinElementText(SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FILENAME_TEXT)->RenderAsOffset( m_displayDevice,
																																																																	name,
																																																																	area->m_left,
																																																																	area->m_top );
	}
}

//---------------------------------------------------------------------
//  LoadScreenshotFile
//---------------------------------------------------------------------
BOOL CROMListScreen::LoadScreenshotFile( UINT32 index )
{
    // Release the old screenshot data
  SAFE_RELEASE( m_screenshotTexture );

  MAMEDriverData_t &driverData = m_driverInfoList[GetCurrentGameIndex()];
  CStdString filename = g_FileIOConfig.m_screenshotPath;
  filename += "\\";
  if( index )
  {
    char num[8];
    sprintf( num, "$.4s%4.4lu", driverData.m_romFileName, index - 1 );
    filename += num;
  }
  else
    filename += driverData.m_romFileName;

  filename += ".png";

  return( LoadPNGToTexture( filename, &m_screenshotTexture, &m_screenshotRect ) );
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
    PRINTMSG(( T_INFO, "Failed to store the ROM list file!" ));
    return FALSE;
  }

  return TRUE;
}

//---------------------------------------------------------------------
//	GenerateSuperscrollJumpTable
//---------------------------------------------------------------------
BOOL CROMListScreen::MoveCurrentGameToBackupDir( void )
{
  if( Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_romPath0 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_romPath1 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_romPath2 ) ||
      Helper_MoveCurrentGameToBackupDir( g_FileIOConfig.m_romPath3 ) )
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

    PRINTMSG(( T_INFO, "Moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() ));

      // For some reason MoveFile seems to fail if I move from one partition to
      // another. For this reason, do a copy & delete
//    if( !MoveFile( oldPath.c_str(), newPath.c_str() ) )
    if( !CopyFile( oldPath.c_str(), newPath.c_str(), FALSE ) )
      PRINTMSG(( T_ERROR, "Failed moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() ));
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

      // Filter out working ROMs (for debugging/testing or screwy people)
    if( m_options.m_filterMode & FM_WORKING && driverStatus == STATUS_WORKING )
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

	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA );
	if( area )
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;
	else
		m_maxPageSize = 15;


    // Jump the cursor to the selected position
	UINT32 pageSize = m_currentSortedList.size() < m_maxPageSize ? m_currentSortedList.size() : m_maxPageSize;
	UINT32 pageHalfwayPoint = (pageSize >> 1);
	UINT32 maxPageOffset = m_currentSortedList.size() - pageSize;

    // Put the page offset at absoluteCursorPos - pageHalwayPoint, or 0
  if( pos <= pageHalfwayPoint || m_currentSortedList.size() < m_maxPageSize )
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




