/**
	* \file			ROMList.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "ROMList.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"

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

#define HIGHLIGHTBAR_COLOR    D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define SCROLLICON_COLOR      D3DCOLOR_XRGB( 255, 255, 255 )

#define TITLEBAR_ROW          101
#define FIRSTDATA_ROW         126

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    607
#define NAME_COLUMN           40
#define MANUFACTURER_COLUMN   305
#define YEAR_COLUMN           437
#define NUMPLAYERS_COLUMN     506
#define CLONE_COLUMN          535 
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text

#define SCROLLUP_TOP          122
#define SCROLLUP_RIGHT        608
#define SCROLLUP_LEFT         SCROLLUP_RIGHT - 32
#define SCROLLUP_BOTTOM       SCROLLUP_TOP + 32

#define SCROLLDOWN_BOTTOM     451
#define SCROLLDOWN_TOP        SCROLLDOWN_BOTTOM - 32
#define SCROLLDOWN_RIGHT      608
#define SCROLLDOWN_LEFT       SCROLLDOWN_RIGHT - 32


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
MAMEDriverData_t       *CROMList::m_driverInfoList = NULL;
UINT32                 CROMList::m_numDrivers = 0;
std::vector<ROMStatus> CROMList::m_ROMStatus;


//= P R O T O T Y P E S ================================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );              // Defined in main.cpp
void DestroyBackdrop( void );                                   // Defined in main.cpp
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp


  // Compare functions for ROM List sorting
static BOOL Compare_Description( UINT32 a, UINT32 b );
static BOOL Compare_Manufacturer( UINT32 a, UINT32 b );
static BOOL Compare_Year( UINT32 a, UINT32 b );
static BOOL Compare_ParentROM( UINT32 a, UINT32 b );
static BOOL Compare_Genre( UINT32 a, UINT32 b );
static BOOL Compare_NumPlayers( UINT32 a, UINT32 b );
static BOOL Compare_ROMStatus( UINT32 a, UINT32 b );

static BOOL Helper_ReadXMLTag( osd_file *file, CStdString *tagName );

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	LoadROMList
//---------------------------------------------------------------------
BOOL CROMList::LoadROMList( BOOL bGenerate, BOOL allowClones )
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
BOOL CROMList::GenerateROMList( void )
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
BOOL CROMList::Helper_GenerateROMList( CStdString &path )
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

//g_FileIOConfig.m_recursiveSearching
      // Notify the user that a new zip has been found
      DrawZipData( "Searching for ROM files", findData.cFileName, i );

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
	for(DWORD i = 0; i < m_numDrivers; ++i )
	{
      // Only redraw every 8th ROM, as rendering takes up
      // the vast majority of the overall time
    if( !(i & 0x07) )
      DrawZipCheckProgress( i );
    CStdString driverFileName = m_driverInfoList[i].m_romFileName;
    driverFileName.ToLower();

    std::vector<CStdString>::iterator it = std::find( zipFileNames.begin(), 
                                                      zipFileNames.end(), 
                                                      driverFileName );

    if( it != zipFileNames.end() )
      if( std::find( m_ROMListFull.begin(), m_ROMListFull.end(), i ) == m_ROMListFull.end() )
        m_ROMListFull.push_back( i );
	}

	PRINTMSG( T_INFO, "Found %lu games!", m_ROMListFull.size() );

	return TRUE;
}

//-------------------------------------------------------------
//  SaveROMListFile
//-------------------------------------------------------------
BOOL CROMList::SaveROMListFile( void )
{
		// Write the indices to the ROM list file
	CStdString romListFile = g_ROMListPath;
	romListFile += "\\";
	romListFile += ROMLISTFILENAME;

	PRINTMSG( T_INFO, "Store ROM list: %s", romListFile.c_str() );

	HANDLE hFile = CreateFile(	romListFile.c_str(),
															GENERIC_READ | GENERIC_WRITE,
															0,
															NULL,
															CREATE_ALWAYS,
															FILE_ATTRIBUTE_NORMAL,
															NULL );
	if( !hFile )
	{
		PRINTMSG( T_ERROR, "Could not create file %s!", romListFile.c_str() );
		return FALSE;
	}

    // Sign the file
  DWORD bytesWritten;
  if( !WriteFile( hFile, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1, &bytesWritten, NULL ) ||
      bytesWritten != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		CloseHandle( hFile );

			// Delete the file
		DeleteFile( romListFile.c_str() );
		return FALSE;
	}


    // Calculate a signature for the list, so we can validate it when reading
    //  to eliminate corrupt data
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

    // Write in a blank signature
  if( !WriteFile( hFile, sigData, sigSize, &bytesWritten, NULL ) || bytesWritten != sigSize )
  {
    free( sigData );
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing blank signature!" );
    return FALSE;
  }


    // Define a macro to write a block of data and calculate the data signature
  #define WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        !WriteFile( hFile, (_data__), (_dataSize__), &bytesWritten, NULL ) || \
        bytesWritten != (_dataSize__) ) \
    { \
      PRINTMSG( T_ERROR, "Write failed!" ); \
      free( sigData ); \
      CloseHandle( hFile ); \
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
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Write in the real signature
  SetFilePointer( hFile, sizeof(DRIVERLIST_FILESTAMP) - 1, NULL, FILE_BEGIN );
  if( !WriteFile( hFile, sigData, sigSize, &bytesWritten, NULL ) || bytesWritten != sigSize )
  {
    free( sigData );
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing signature!" );
    return FALSE;
  }

  free( sigData );
	CloseHandle( hFile );
  return TRUE;
}


//---------------------------------------------------------------------
//  LoadROMListFile
//---------------------------------------------------------------------
BOOL CROMList::LoadROMListFile( void )
{
  m_ROMListFull.clear();
  m_ROMStatus.clear();

  UINT32 i = 0;
  for( ; i < m_numDrivers; ++i )
    m_ROMStatus.push_back( STATUS_UNKNOWN );

	std::string		romListFile = g_ROMListPath;
	romListFile += "\\";
	romListFile += ROMLISTFILENAME;

	PRINTMSG( T_INFO, "Load ROM list: %s", romListFile.c_str() );
	HANDLE hFile = CreateFile(	romListFile.c_str(),
															GENERIC_READ,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
    PRINTMSG( T_ERROR, "Failed to open ROM list file %s!", romListFile.c_str() );
    return FALSE;
  }

    // Grab the entire file at once so we can check its signature
  LARGE_INTEGER fileSize;
  if( !GetFileSizeEx( hFile, &fileSize ) || fileSize.HighPart )
  {
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not get filesize for %s!", romListFile.c_str() );
    return FALSE;
  }
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize.LowPart );
  if( !fileData )
  {
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              romListFile.c_str(), 
              fileSize.LowPart );
    return FALSE;
  }

    // Read the entire file into memory
  DWORD BytesRead;
  if( !ReadFile( hFile, fileData, fileSize.LowPart, &BytesRead, NULL ) || BytesRead != fileSize.LowPart )
  {
    free( fileData );
    CloseHandle( hFile );
		DeleteFile( romListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to read file %s!", romListFile.c_str() );
    return FALSE;
  }
  CloseHandle( hFile );

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
  DWORD listDataSize = fileSize.LowPart - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
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

  #define READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize.LowPart ) \
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

  return TRUE;
}

//---------------------------------------------------------------------
//	RefreshROMStatus
//---------------------------------------------------------------------
BOOL CROMList::RefreshROMStatus( void )
{
  if( !LoadROMStatusFile() )
  {
    PRINTMSG( T_ERROR, "Failed to load the ROM status file" );
    return FALSE;
  }

  DrawZipData( "Saving cache file...", "", 0 );
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
BOOL CROMList::LoadROMStatusFile( void )
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


	      DrawZipData( "Parsing status file", romName.c_str(), entryNumber++ );

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
void CROMList::MoveCursor( CInputManager &gp, BOOL useSpeedBanding )
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
      m_gameSelected = TRUE;
    }
	}
	else if( gp.IsButtonPressed( GP_WHITE ) )
	{
		m_options.m_verboseMode = !m_options.m_verboseMode;
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
void CROMList::SuperScrollModeMoveCursor( CInputManager &gp, FLOAT elapsedTime )
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
void CROMList::NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime )
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
void CROMList::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
	WCHAR name[512];

  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil


  FLOAT textHeight = m_fontSet.SmallThinFontHeight();

  if( m_options.m_verboseMode )
    m_backdropTexture = m_textureSet.GetROMListScreenBackdrop();
  else
    m_backdropTexture = m_textureSet.GetBasicBackdrop();    

    // Render the backdrop texture
  RenderBackdrop();

  FLOAT selectedItemYPos = (textHeight * (ULONG)m_cursorPosition);

    // Render the highlight bar for the selected item
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
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
                                        ( m_options.m_verboseMode ? MANUFACTURER_COLUMN : TEXTBOX_RIGHT ) - (NAME_COLUMN + COLUMN_PADDING) );
	  if( m_options.m_verboseMode )
	  {
		  m_fontSet.SmallThinFont().DrawText( MANUFACTURER_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Manufacturer" );
		  m_fontSet.SmallThinFont().DrawText( YEAR_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Year" );
      m_fontSet.SmallThinFont().DrawText( NUMPLAYERS_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"#P" );
		  m_fontSet.SmallThinFont().DrawText( CLONE_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Clone" );
	  }

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
        ROMStatus &status = m_ROMStatus[m_currentSortedList[ i ]];
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
                                          ( m_options.m_verboseMode ? MANUFACTURER_COLUMN : TEXTBOX_RIGHT ) - (NAME_COLUMN + COLUMN_PADDING) );

		  if( m_options.m_verboseMode )
		  {
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
		  }

			  // Inc the Y position
		  yPos += textHeight;
	  }
	m_fontSet.SmallThinFont().End();


    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

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



  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


//---------------------------------------------------------------------
//	DrawZipData
//---------------------------------------------------------------------
void CROMList::DrawZipData( const char *message, const char *fileName, DWORD index )
{
		// Each index value is 1/4th of a *
		// The *'s scroll between the < >'s
	INT cursorPos = ( (INT)((FLOAT)index / 4.0f) & 15) - 7;

		// Display the error to the user
	m_displayDevice->Clear(	0L,																// Count
													NULL,															// Rects to clear
													D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
													D3DCOLOR_XRGB(0,0,0),							// Color
													1.0f,															// Z
													0L );															// Stencil

  m_backdropTexture = m_textureSet.GetMessageScreenBackdrop();

    // Render the backdrop texture
  RenderBackdrop();

	m_fontSet.DefaultFont().Begin();
	
	  WCHAR wBuf[256];
    mbstowcs( wBuf, message, 256 );
	  m_fontSet.DefaultFont().DrawText( 320, 200, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, XBFONT_CENTER_X );

		  // Draw some progress dots
  	
		  // Draw the current filename
	  mbstowcs( wBuf, fileName, 256 );
	  m_fontSet.DefaultFont().DrawText( 320, 270, D3DCOLOR_RGBA( 60, 100, 255, 255 ), wBuf, XBFONT_CENTER_X );
	m_fontSet.DefaultFont().End();

  m_fontSet.LargeThinFont().Begin();
	  wcscpy( wBuf, L"<                >" );
	  wBuf[8 + cursorPos] = L'*';
	  m_fontSet.LargeThinFont().DrawText( 320, 240, D3DCOLOR_RGBA( 255, 125, 125, 255), wBuf, XBFONT_CENTER_X );
  m_fontSet.LargeThinFont().End();



	m_displayDevice->Present( NULL, NULL, NULL, NULL );

}

//---------------------------------------------------------------------
//	DrawZipCheckProgress
//---------------------------------------------------------------------
void CROMList::DrawZipCheckProgress( DWORD index )
{
		// Each index value is 1/4th of a *
		// The *'s scroll between the < >'s
	INT cursorPos = ( (INT)((FLOAT)index / 4.0f) & 15) - 7;

		// Display the error to the user
	m_displayDevice->Clear(	0L,																// Count
													NULL,															// Rects to clear
													D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
													D3DCOLOR_XRGB(0,0,0),							// Color
													1.0f,															// Z
													0L );															// Stencil

  

  m_backdropTexture = m_textureSet.GetMessageScreenBackdrop();

    // Render the backdrop texture
  RenderBackdrop();



	m_fontSet.DefaultFont().Begin();
	
	  m_fontSet.DefaultFont().DrawText( 320, 200, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Checking against known ROM files", XBFONT_CENTER_X );

		    // Draw the current filename
	    WCHAR wBuf[256];
	    mbstowcs( wBuf, m_driverInfoList[index].m_description, 256 );
	    m_fontSet.DefaultFont().DrawText( 320, 270, D3DCOLOR_XRGB( 60, 100, 255 ), wBuf, XBFONT_CENTER_X );

	m_fontSet.DefaultFont().End();

  m_fontSet.LargeThinFont().Begin();

		  // Draw a progress bar
    UINT32 percentage = (UINT32)( (FLOAT)index * (25.0f / (FLOAT)m_numDrivers) + 0.5f ); 
    UINT32 i = 0;
    wcscpy( wBuf, L"[" );
    for( ; i < percentage; ++i )
      wcscat( wBuf, L"|" );
    for( ; i < 25; ++i )
      wcscat( wBuf, L" " );
    wcscat( wBuf, L"]" );

	  m_fontSet.LargeThinFont().DrawText( 320, 240, D3DCOLOR_XRGB( 255, 125, 125 ), wBuf, XBFONT_CENTER_X );
  m_fontSet.LargeThinFont().End();


	m_displayDevice->Present( NULL, NULL, NULL, NULL );
}

//---------------------------------------------------------------------
//	RemoveCurrentGameIndex
//---------------------------------------------------------------------
BOOL CROMList::RemoveCurrentGameIndex( void )
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
BOOL CROMList::MoveCurrentGameToBackupDir( void )
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
BOOL CROMList::Helper_MoveCurrentGameToBackupDir( CStdString &path )
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
    if( !MoveFile( oldPath.c_str(), newPath.c_str() ) )
      PRINTMSG( T_ERROR, "Failed moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );
    else
    {
    	RemoveCurrentGameIndex();
      return TRUE;
    }
  }

  return FALSE;
}


//---------------------------------------------------------------------
//	GenerateSuperscrollJumpTable
//---------------------------------------------------------------------
void CROMList::GenerateSuperscrollJumpTable( void )
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
void CROMList::UpdateSortedList( void )
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
void CROMList::UpdateFilteredList( void )
{
  m_ROMListFiltered.clear();

  std::vector<UINT32>::iterator i = m_ROMListFull.begin();
  for( ; i != m_ROMListFull.end(); ++i )
  {
    MAMEDriverData_t &driverData = m_driverInfoList[*i];
    ROMStatus        &driverStatus = m_ROMStatus[*i];

      // Filter on CLONE status
    if( m_options.m_filterMode & FM_CLONE &&driverData.m_isClone )
      continue;

      // Filter on the number of players
    if( m_options.m_filterMode & FM_NUMPLAYERS && driverData.m_numPlayers < m_options.m_numPlayersFilter )
      continue;

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

    m_ROMListFiltered.push_back( *i );
  }
}

//---------------------------------------------------------------------
//  SetAbsoluteCursorPosition
//---------------------------------------------------------------------
void CROMList::SetAbsoluteCursorPosition( UINT32 pos )
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
void CROMList::GetFriendlySuperscrollIndexStringForJumpTableIndex( CStdString *ret, UINT32 superscrollTableIndex )
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
void CROMList::GetFriendlySuperscrollIndexStringForROM( CStdString *ret, UINT32 sortedListIndex )
{
  UINT32 romIndex;
  if( sortedListIndex >= m_currentSortedList.size() || 
      (romIndex = m_currentSortedList[sortedListIndex]) == INVALID_ROM_INDEX )
  {
    *ret = "Invalid";
    return;
  }

  MAMEDriverData_t &driver = m_driverInfoList[romIndex];
  ROMStatus        &status = m_ROMStatus[romIndex];

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
    *ret = "Unsupported";
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
  }
}

//---------------------------------------------------------------------
//  Compare_Description
//---------------------------------------------------------------------
static BOOL Compare_Description( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

  int cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}


//---------------------------------------------------------------------
//  Compare_Manufacturer
//---------------------------------------------------------------------
static BOOL Compare_Manufacturer( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

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
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

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
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

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
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

    // There is no genre available yet, just sort by name
  int cmp = 0; //stricmp( aDriver.m_cloneFileName, bDriver.m_cloneFileName );
  if( !cmp )
    cmp = stricmp( aDriver.m_description, bDriver.m_description );

  return cmp < 0;
}

//---------------------------------------------------------------------
//  Compare_NumPlayers
//---------------------------------------------------------------------
static BOOL Compare_NumPlayers( UINT32 a, UINT32 b )
{
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];

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
  MAMEDriverData_t &aDriver = CROMList::m_driverInfoList[a];
  MAMEDriverData_t &bDriver = CROMList::m_driverInfoList[b];
  ROMStatus        &aStatus = CROMList::m_ROMStatus[a];
  ROMStatus        &bStatus = CROMList::m_ROMStatus[b];

    // Sort by the rom status, putting lower (better working) numbers first
  int cmp = bStatus - aStatus;
  if( !cmp )
    return stricmp( aDriver.m_description, bDriver.m_description ) < 0;

  return cmp > 0;
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




