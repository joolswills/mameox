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
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
};

//= D E F I N E S ======================================================

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							14

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
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f



//= G L O B A L = V A R S ==============================================
const char g_superscrollCharacterSet[NUM_SUPERSCROLL_CHARS+1] = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ";


//= P R O T O T Y P E S ================================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );              // Defined in main.cpp
void DestroyBackdrop( void );                                   // Defined in main.cpp
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	LoadROMList
//---------------------------------------------------------------------
BOOL CROMList::LoadROMList( BOOL bGenerate, BOOL allowClones )
{
	PRINTMSG( T_TRACE, "LoadROMList" );

  m_maxPageSize = MAXPAGESIZE;
	m_ROMListWithClones.clear();
  m_ROMListNoClones.clear();
  m_allowClones = allowClones;

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
    GenerateSuperscrollJumpTable();
    m_numLinesInList = CURRENTROMLIST().size();
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
	m_ROMListWithClones.clear();
  m_ROMListNoClones.clear();

	std::vector< CStdString > zipFileNames;
	WIN32_FIND_DATA findData;

  g_FileIOConfig.m_RomPath.MakeLower();
	PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath.c_str() );

  // Check if the rom path is on a smb share
  if (g_FileIOConfig.m_RomPath.Left(6) == "smb://")
  {
    CSMBHandler smbHandler;
    smbHandler.GetDirectory(g_FileIOConfig.m_RomPath, ".zip", zipFileNames, TRUE);
  }
  else
  {
    HANDLE findHandle = FindFirstFile( (g_FileIOConfig.m_RomPath + "\\*").c_str(),
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
      DrawZipData( findData.cFileName, i );

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
    DrawZipCheckProgress( i );
    CStdString driverFileName = m_driverInfoList[i].m_romFileName;
    driverFileName.ToLower();

    std::vector<CStdString>::iterator it = std::find( zipFileNames.begin(), 
                                                      zipFileNames.end(), 
                                                      driverFileName );
    if( it != zipFileNames.end() )
    {
      if( !m_driverInfoList[i].m_isClone )
        m_ROMListNoClones.push_back( i );
      m_ROMListWithClones.push_back( i );
    }
	}

	PRINTMSG( T_INFO, "Found %lu games, %lu unique!", m_ROMListWithClones.size(), m_ROMListNoClones.size() );

  if( !SaveROMListFile() )
  {
    PRINTMSG( T_INFO, "Failed to store the ROM list file!" );
    return FALSE;
  }

    // Create the superscroll jump table
  GenerateSuperscrollJumpTable();
  m_numLinesInList = CURRENTROMLIST().size();
	return TRUE;
}

//-------------------------------------------------------------
//  SaveROMListFile
//-------------------------------------------------------------
BOOL CROMList::SaveROMListFile( void )
{
		// Write the indices to the ROM list file
	std::string	romListFile = g_ROMListPath;
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
  DWORD numROMs = m_ROMListWithClones.size();
  WRITEDATA( &numROMs, sizeof(numROMs) );

    // Write the indices
	std::vector<UINT32>::iterator it = m_ROMListWithClones.begin();
	for( ; it != m_ROMListWithClones.end(); ++it )
	{
		DWORD idx = (*it);
    WRITEDATA( &idx, sizeof(idx) );
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
    DWORD idx;
    READDATA_NOMALLOC( &idx, sizeof(idx) );

		m_ROMListWithClones.push_back( idx );
    if( !m_driverInfoList[idx].m_isClone )
      m_ROMListNoClones.push_back( idx );
  }
  free( fileData );

  return TRUE;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CROMList::MoveCursor( CGamepad &gp, BOOL useSpeedBanding )
{
  	// Handle user input
  if(  gp.IsButtonPressed( GP_B | GP_A ) )
  {
    m_shouldGenerateROMList = TRUE;
  }
  else if( gp.IsOneOfButtonsPressed( GP_A | GP_START ) )
	{
			// Run the selected ROM
    if( GetCurrentGameIndex() != INVALID_ROM_INDEX  )
    {
      m_gameSelected = TRUE;
    }
	}
	else if( gp.IsButtonPressed( GP_X | GP_B ) )
	{				
			// Move the currently selected game to the backup dir
		UINT32 romIDX = GetCurrentGameIndex();

    // Don't allow removing from a samba share
    if (g_FileIOConfig.m_RomPath.Left(5) != "smb:\\")
    {
      std::string oldPath = g_FileIOConfig.m_RomPath + "\\";
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
      {
        PRINTMSG( T_ERROR, "Failed moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );
      }
    }

    gp.WaitForNoButton();
		RemoveCurrentGameIndex();
	}
	else if( gp.IsButtonPressed( GP_WHITE ) )
	{
		m_additionalinfo = !m_additionalinfo;
    gp.WaitForNoButton();
	}
  else if( gp.IsButtonPressed( GP_BLACK ) )
  {
      // No need to regenerate the list, just switch to
      // the noclones (or clones) list
		m_allowClones = !m_allowClones;
    m_numLinesInList = CURRENTROMLIST().size();
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
void CROMList::SuperScrollModeMoveCursor( CGamepad &gp, FLOAT elapsedTime )
{
  if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    UINT32 startPos = m_superscrollCharacterIdx;
    while( 1 )
    {
        // Wrap around
      if( ++m_superscrollCharacterIdx >= NUM_SUPERSCROLL_CHARS )
        m_superscrollCharacterIdx = 0;
      if( m_superscrollJumpTable[m_superscrollCharacterIdx] != INVALID_SUPERSCROLL_JUMP_IDX ||
          m_superscrollCharacterIdx == startPos )
        return;
    }
	}
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    UINT32 startPos = m_superscrollCharacterIdx;
    while( 1 )
    {
        // Wrap around
      if( !m_superscrollCharacterIdx )
        m_superscrollCharacterIdx = NUM_SUPERSCROLL_CHARS - 1;
      else
        --m_superscrollCharacterIdx;

      if( m_superscrollJumpTable[m_superscrollCharacterIdx] != INVALID_SUPERSCROLL_JUMP_IDX ||
          m_superscrollCharacterIdx == startPos )
        return;
    }
	}

    // This should always succeed, as SuperScrollModeMoveCursor only presents valid
    // character indices to the user
  UINT32 absCursorPos = m_superscrollJumpTable[m_superscrollCharacterIdx];
  if( absCursorPos != INVALID_SUPERSCROLL_JUMP_IDX )
  {
      // Jump the cursor to the selected letter
	  UINT32 pageSize = (CURRENTROMLIST().size() < MAXPAGESIZE ? CURRENTROMLIST().size() : MAXPAGESIZE);
	  UINT32 pageHalfwayPoint = (pageSize >> 1);
	  UINT32 maxPageOffset = CURRENTROMLIST().size() - pageSize;

      // Put the page offset at absoluteCursorPos - pageHalwayPoint, or 0
    if( absCursorPos <= pageHalfwayPoint || CURRENTROMLIST().size() < MAXPAGESIZE )
    {
      m_pageOffset = 0.0f;
      m_cursorPosition = (FLOAT)absCursorPos;
    }
    else
    {
      m_pageOffset = (FLOAT)(absCursorPos - pageHalfwayPoint);
      m_cursorPosition = (FLOAT)pageHalfwayPoint;
    }
  }
}

//---------------------------------------------------------------------
//  NormalModeMoveCursor
//---------------------------------------------------------------------
void CROMList::NormalModeMoveCursor( CGamepad &gp, FLOAT elapsedTime )
{
		// General idea taken from XMAME

		// The combined trigger offset, scaled to the range [-1.0f,1.0f]
  FLOAT cursorVelocity =  ((FLOAT)gp.GetAnalogButtonState( GP_RIGHT_TRIGGER ) - 
												  (FLOAT)gp.GetAnalogButtonState( GP_LEFT_TRIGGER )) / 256.0f;


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


	DWORD pageSize = (CURRENTROMLIST().size() < MAXPAGESIZE ? CURRENTROMLIST().size() : MAXPAGESIZE);
	ULONG pageHalfwayPoint = (pageSize >> 1);
	ULONG maxPageOffset = CURRENTROMLIST().size() - pageSize;

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


    // Make the superscroll char index == whatever we're on now 
  char CurrentGameSuperscrollChar = '#';
  if( GetCurrentGameIndex() != INVALID_ROM_INDEX )
    CurrentGameSuperscrollChar = toupper( m_driverInfoList[GetCurrentGameIndex()].m_description[0] );
  if( !(CurrentGameSuperscrollChar >= 'A' && CurrentGameSuperscrollChar <= 'Z') )
    CurrentGameSuperscrollChar = '#';

  if( g_superscrollCharacterSet[m_superscrollCharacterIdx] != CurrentGameSuperscrollChar )
  {
    for( m_superscrollCharacterIdx = 0; m_superscrollCharacterIdx < INVALID_SUPERSCROLL_JUMP_IDX; ++m_superscrollCharacterIdx )
    {
      if( g_superscrollCharacterSet[m_superscrollCharacterIdx] == CurrentGameSuperscrollChar )
        return;
    }

    PRINTMSG( T_ERROR, "Unhandled superscroll char %c (0x%X)", CurrentGameSuperscrollChar, CurrentGameSuperscrollChar );
    m_superscrollCharacterIdx = 0;
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

  #define HEADER_COLOR          D3DCOLOR_XRGB( 0, 0, 0 )
  #define ITEM_COLOR			      D3DCOLOR_XRGB( 0, 0, 0 )
  #define HIGHLIGHTBAR_COLOR    D3DCOLOR_ARGB( 180, 175, 179, 212 )
  #define SCROLLICON_COLOR      D3DCOLOR_XRGB( 255, 255, 255 )

  #define TITLEBAR_ROW          116
  #define FIRSTDATA_ROW         139

  #define HIGHLIGHTBAR_LEFT     34
  #define HIGHLIGHTBAR_RIGHT    606
  #define NAME_COLUMN           40
  #define MANUFACTURER_COLUMN   305
  #define YEAR_COLUMN           460
  #define CLONE_COLUMN          530 
  #define COLUMN_PADDING        5     // Number of pixels to subtract from the column width before truncating text

  #define SCROLLUP_TOP          137
  #define SCROLLUP_LEFT         32
  #define SCROLLUP_RIGHT        SCROLLUP_LEFT + 32
  #define SCROLLUP_BOTTOM       SCROLLUP_TOP + 32

  #define SCROLLDOWN_BOTTOM     448
  #define SCROLLDOWN_TOP        SCROLLDOWN_BOTTOM - 32
  #define SCROLLDOWN_LEFT       32
  #define SCROLLDOWN_RIGHT      SCROLLDOWN_LEFT + 32

  FLOAT textWidth, textHeight;
  m_fontSet.SmallThinFont().GetTextExtent( L"i^jg", &textWidth, &textHeight );

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

    if( m_superscrollMode )
    {
        // Display the superscroll character
      WCHAR displayString[2] = L"";
      mbtowc( displayString, &g_superscrollCharacterSet[m_superscrollCharacterIdx], 1 );
		  swprintf( name, L"Names %s [%s]", ( m_allowClones == FALSE ) ? L"(No Clones)" : L"(Clones)   ", displayString );
    }
    else
  	  swprintf( name, L"Names %s", ( m_allowClones == FALSE ) ? L"(No Clones)" : L"(Clones)   " );

	  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, TITLEBAR_ROW, HEADER_COLOR, name );
	  if( m_additionalinfo )
	  {
		  m_fontSet.SmallThinFont().DrawText( MANUFACTURER_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Manufacturer" );
		  m_fontSet.SmallThinFont().DrawText( YEAR_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Year" );
		  m_fontSet.SmallThinFont().DrawText( CLONE_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Clone" );
	  }


		  // Render the titles
	  FLOAT yPos = 0.0f;
    FLOAT pageSize = GetCurrentPageSize();
	  ULONG absListIDX = (ULONG)m_pageOffset;

	  for( DWORD i = 0; i < pageSize; ++i )
	  {
		  mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX++ ] ].m_description, 255 );
		  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN,
                                          FIRSTDATA_ROW + yPos,
                                          ITEM_COLOR,
                                          name,
                                          XBFONT_TRUNCATED,
                                          ( m_additionalinfo ? MANUFACTURER_COLUMN : 600 ) - (NAME_COLUMN + COLUMN_PADDING) );

		  if( m_additionalinfo )
		  {
			  mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_manufacturer, 255 );
			  m_fontSet.SmallThinFont().DrawText( MANUFACTURER_COLUMN,
                                            FIRSTDATA_ROW + yPos,
                                            ITEM_COLOR,
                                            name,
                                            XBFONT_TRUNCATED,
                                            YEAR_COLUMN - (MANUFACTURER_COLUMN + COLUMN_PADDING) );

			  mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_year, 255 );
			  m_fontSet.SmallThinFont().DrawText( YEAR_COLUMN, 
                                            FIRSTDATA_ROW + yPos, 
                                            ITEM_COLOR, 
                                            name, 
                                            XBFONT_TRUNCATED,
                                            CLONE_COLUMN - (YEAR_COLUMN + COLUMN_PADDING) );

			  mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_cloneFileName, 255 );
			  m_fontSet.SmallThinFont().DrawText( CLONE_COLUMN, 
                                            FIRSTDATA_ROW + yPos,
                                            ITEM_COLOR,
                                            name,
                                            XBFONT_TRUNCATED,
                                            600 - (CLONE_COLUMN + COLUMN_PADDING) );
		  }

			  // Inc the Y position
		  yPos += textHeight;
	  }

  /*
    #if defined(_PROFILER) || defined(_DEBUG)
      MEMORYSTATUS memStatus;
      GlobalMemoryStatus( &memStatus );

      WCHAR memStr[256];
      swprintf( memStr, 
                L"Memory: %lu/%lu",
                memStatus.dwAvailPhys, 
                memStatus.dwTotalPhys );

      m_fontSet.SmallThinFont().DrawText( 256, Y_POS + yPos, D3DCOLOR_XRGB(100,220,220), memStr, XBFONT_CENTER_X );
    #else
      m_fontSet.SmallThinFont().DrawText( 256, Y_POS + yPos, D3DCOLOR_XRGB(100,220,220), L"Press X for help", XBFONT_CENTER_X );
    #endif
  */

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
	  m_displayDevice->SetTexture( 0, m_scrollUpIconTexture );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  if( (DWORD)m_pageOffset < (m_numLinesInList - (DWORD)pageSize) )
  {
	  m_displayDevice->SetTexture( 0, m_scrollDownIconTexture );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
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
void CROMList::DrawZipData( const char *fileName, DWORD index )
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

	m_fontSet.DefaultFont().Begin();
	
	  m_fontSet.DefaultFont().DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Searching for ROM files", XBFONT_CENTER_X );

		  // Draw some progress dots
	  WCHAR wBuf[256];
  	
	  wcscpy( wBuf, L"<                >" );
	  wBuf[8 + cursorPos] = L'*';
	  m_fontSet.DefaultFont().DrawText( 320, 80, D3DCOLOR_RGBA( 255, 125, 125, 255), wBuf, XBFONT_CENTER_X );

		  // Draw the current filename
	  mbstowcs( wBuf, fileName, 256 );
	  m_fontSet.DefaultFont().DrawText( 320, 100, D3DCOLOR_RGBA( 60, 100, 255, 255 ), wBuf, XBFONT_CENTER_X );

	m_fontSet.DefaultFont().End();
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

	m_fontSet.DefaultFont().Begin();
	
	m_fontSet.DefaultFont().DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Checking against known ROM files", XBFONT_CENTER_X );

		  // Draw a progress bar
    UINT32 percentage = (UINT32)( (FLOAT)index * (25.0f / (FLOAT)m_numDrivers) + 0.5f ); 
    UINT32 i = 0;
	  WCHAR wBuf[256] = L"[";
    for( ; i < percentage; ++i )
      wcscat( wBuf, L"|" );
    for( ; i < 25; ++i )
      wcscat( wBuf, L" " );
    wcscat( wBuf, L"]" );

	  m_fontSet.DefaultFont().DrawText( 320, 80, D3DCOLOR_XRGB( 255, 125, 125 ), wBuf, XBFONT_CENTER_X );

		  // Draw the current filename
	  mbstowcs( wBuf, m_driverInfoList[index].m_description, 256 );
	  m_fontSet.DefaultFont().DrawText( 320, 100, D3DCOLOR_XRGB( 60, 100, 255 ), wBuf, XBFONT_CENTER_X );

	m_fontSet.DefaultFont().End();
	m_displayDevice->Present( NULL, NULL, NULL, NULL );

}

//---------------------------------------------------------------------
//	RemoveCurrentGameIndex
//---------------------------------------------------------------------
void CROMList::RemoveCurrentGameIndex( void )
{
	UINT32 curCursorPos = (ULONG)m_pageOffset + (ULONG)m_cursorPosition;
	std::vector<UINT32>::iterator it = CURRENTROMLIST().begin();
	for( UINT32 i = 0; i < curCursorPos; ++i )
		++it;

	CURRENTROMLIST().erase( it );

    // Note: This needs to be fixed to remove from both
    //       ROM lists, as well as write the list back
    //       to the file.
}

//---------------------------------------------------------------------
//	GenerateSuperscrollJumpTable
//---------------------------------------------------------------------
void CROMList::GenerateSuperscrollJumpTable( void )
{
    // Invalidate jump table
  for( UINT32 idx = 0; idx < NUM_SUPERSCROLL_CHARS; ++idx )
    m_superscrollJumpTable[idx] = INVALID_SUPERSCROLL_JUMP_IDX;
  
  char charToLookFor = g_superscrollCharacterSet[0];
  for( UINT32 j = 0, i = 0; j < CURRENTROMLIST().size() ; ++j )
  {
    char currentChar = toupper( m_driverInfoList[ CURRENTROMLIST()[j] ].m_description[0] );

      // Map any non-alphanumerics to '#'
    if( !(currentChar >= 'A' && currentChar <= 'Z') )
      currentChar = '#';
    {
      if( currentChar == charToLookFor )
      {
          // Fill out the jump table
        if( m_superscrollJumpTable[i] == INVALID_SUPERSCROLL_JUMP_IDX )
        {
          m_superscrollJumpTable[i] = j;

            // Dump out if we've found a 'Z', as there's nothing important left in the list
          if( charToLookFor == g_superscrollCharacterSet[NUM_SUPERSCROLL_CHARS-1] )
            return;

            // Look for the next character in the set
          charToLookFor = g_superscrollCharacterSet[++i];
        }
      }
      else if( currentChar > charToLookFor )
      {
          // We've passed the character we were looking for
        while( charToLookFor != currentChar )
          charToLookFor = g_superscrollCharacterSet[++i];
          
          // Fill out the jump table for the char we found
        if( m_superscrollJumpTable[i] == INVALID_SUPERSCROLL_JUMP_IDX )
        {
          m_superscrollJumpTable[i] = j;

            // Dump out if we've found a 'Z', as there's nothing important left in the list
          if( charToLookFor == g_superscrollCharacterSet[NUM_SUPERSCROLL_CHARS-1] )
            return;

            // Look for the next character in the set
          charToLookFor = g_superscrollCharacterSet[++i];
        }
      }
    }
  }

}
