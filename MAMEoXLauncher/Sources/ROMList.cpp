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


//= D E F I N E S ======================================================

#define NORMAL_ITEM_COLOR				D3DCOLOR_RGBA( 100, 255, 100, 255 )
#define SELECTED_ITEM_COLOR			D3DCOLOR_RGBA( 255, 255, 255, 255 )

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							31

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

	m_ROMListWithClones.clear();
  m_ROMListNoClones.clear();
  m_allowClones = allowClones;


	m_displayDevice->Clear(	0L,																// Count
													NULL,															// Rects to clear
													D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
													D3DCOLOR_XRGB(0,0,0),							// Color
													1.0f,															// Z
													0L );															// Stencil
	m_font.Begin();
	m_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Loading ROM list", XBFONT_CENTER_X );
	m_font.End();
	m_displayDevice->Present( NULL, NULL, NULL, NULL );	


  if( LoadROMListFile() )
  {
    GenerateSuperscrollJumpTable();
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
void CROMList::MoveCursor( CGamepad &gp )
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
      m_gameListPageOffset = 0.0f;
      m_gameListCursorPosition = (FLOAT)absCursorPos;
    }
    else
    {
      m_gameListPageOffset = (FLOAT)(absCursorPos - pageHalfwayPoint);
      m_gameListCursorPosition = (FLOAT)pageHalfwayPoint;
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
		m_gameListCursorSpeedBandTimeout = 0.0f;
	else
	{
		m_gameListCursorSpeedBandTimeout += elapsedTime;
		if( m_gameListCursorSpeedBandTimeout > SBTIMEOUT_FASTEST )
			m_gameListCursorSpeedBandTimeout = SBTIMEOUT_FASTEST;
	}

		// DPAD overrides the triggers
  if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_gameListPageOffset = (FLOAT)((LONG)m_gameListPageOffset);
		m_gameListCursorPosition = (FLOAT)((LONG)m_gameListCursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that subtracting 1 will move to the next item
    m_gameListPageOffset = (FLOAT)((LONG)m_gameListPageOffset);
		m_gameListCursorPosition = (FLOAT)((LONG)m_gameListCursorPosition);
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
	if( m_gameListCursorSpeedBandTimeout == SBTIMEOUT_FASTEST )
		cursorVelocity *= SBMULTIPLIER_FASTEST;
	else if( m_gameListCursorSpeedBandTimeout > SBTIMEOUT_FASTER )
		cursorVelocity *= SBMULTIPLIER_FASTER;
	else if( m_gameListCursorSpeedBandTimeout > SBTIMEOUT_FAST )
		cursorVelocity *= SBMULTIPLIER_FAST;


	DWORD pageSize = (CURRENTROMLIST().size() < MAXPAGESIZE ? CURRENTROMLIST().size() : MAXPAGESIZE);
	ULONG pageHalfwayPoint = (pageSize >> 1);
	ULONG maxPageOffset = CURRENTROMLIST().size() - pageSize;

	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_gameListCursorPosition < pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_gameListCursorPosition) < pageHalfwayPoint )
				m_gameListCursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_gameListCursorPosition);
				m_gameListCursorPosition = (FLOAT)pageHalfwayPoint;
				m_gameListPageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (ULONG)(cursorVelocity + m_gameListPageOffset) <= maxPageOffset )
			{
				m_gameListPageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_gameListPageOffset) <= (FLOAT)maxPageOffset )
					m_gameListCursorPosition += cursorVelocity;
				else
				{
					cursorVelocity -= ((FLOAT)maxPageOffset - m_gameListPageOffset);
					m_gameListPageOffset = (FLOAT)maxPageOffset;
					m_gameListCursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (ULONG)m_gameListPageOffset > maxPageOffset )
			m_gameListPageOffset = (FLOAT)maxPageOffset;
		if( (ULONG)m_gameListCursorPosition > (pageSize - 1) )
			m_gameListCursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_gameListCursorPosition > pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_gameListCursorPosition) > pageHalfwayPoint )
				m_gameListCursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_gameListCursorPosition);
				m_gameListCursorPosition = (FLOAT)pageHalfwayPoint;
				m_gameListPageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (LONG)(cursorVelocity + m_gameListPageOffset) >= 0 )
			{
				m_gameListPageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_gameListPageOffset) >= 0.0f )
					m_gameListCursorPosition += cursorVelocity;
				else
				{
					cursorVelocity += m_gameListPageOffset;
					m_gameListPageOffset = 0.0f;
					m_gameListCursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (LONG)m_gameListPageOffset < 0 )
			m_gameListPageOffset = 0.0f;
		if( (LONG)m_gameListCursorPosition < 0 )
			m_gameListCursorPosition = 0.0f;
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
//	GetHeaderLine
//---------------------------------------------------------------------
WCHAR *CROMList::GetHeaderLine( FLOAT width )
{
	static WCHAR hl[ 512 ];
	FLOAT w, h;
	std::string	headline;
	m_font.GetTextExtent( L"-", &w, &h, FALSE );

	for( int i = 0; i < width / w; i++ )
		headline += '-';

	mbstowcs( hl, headline.c_str(), 512 );

	return( hl );
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CROMList::Draw( BOOL opaque, BOOL flipOnCompletion )
{
	WCHAR name[512];

	#define X_POS		( 10 )
	#define Y_POS		( 25 )
	#define WIDTH		( 512 - (X_POS<<1) )

		// Display the error to the user
  if( opaque )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

	m_font.Begin();

	swprintf( name, L"Names %s", ( m_allowClones == FALSE ) ? L"(No Clones)" : L"(Clones)   " );
	m_font.DrawText(  X_POS + 0, Y_POS + 0, D3DCOLOR_RGBA( 255, 255, 255, 255 ), name );
	if( m_additionalinfo )
	{
		m_font.DrawText( X_POS + 240, Y_POS +  0, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Manufacturer" );
		m_font.DrawText( X_POS + 370, Y_POS +  0, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Year" );
		m_font.DrawText( X_POS + 428, Y_POS +  0, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Clone" );
		m_font.DrawText( X_POS +   0, Y_POS + 10, D3DCOLOR_RGBA( 255, 255, 255, 255 ), GetHeaderLine( 230 ) );
		m_font.DrawText( X_POS + 240, Y_POS + 10, D3DCOLOR_RGBA( 255, 255, 255, 255 ), GetHeaderLine( 120 ) );
		m_font.DrawText( X_POS + 370, Y_POS + 10, D3DCOLOR_RGBA( 255, 255, 255, 255 ), GetHeaderLine(  48 ) );
		m_font.DrawText( X_POS + 428, Y_POS + 10, D3DCOLOR_RGBA( 255, 255, 255, 255 ), GetHeaderLine(  60 ) );
	}
	else
		m_font.DrawText( X_POS +   0, Y_POS + 10, D3DCOLOR_RGBA( 255, 255, 255, 255 ), GetHeaderLine( WIDTH ) );

		// Render the titles
	DWORD color;
	FLOAT xPos, xDelta;
	FLOAT yPos = 24;
	DWORD pageSize = (CURRENTROMLIST().size() < MAXPAGESIZE ? CURRENTROMLIST().size() : MAXPAGESIZE);
	ULONG absListIDX = (ULONG)m_gameListPageOffset;
	if( absListIDX > (CURRENTROMLIST().size() - pageSize) )
	{
			// The current page offset is invalid (due to list shrinkage), reset it and
			//  set the cursor position to the last item in the list
    absListIDX = (CURRENTROMLIST().size() - pageSize);
    m_gameListPageOffset = (FLOAT)absListIDX;
		m_gameListCursorPosition = (FLOAT)(pageSize - 1);
	}

	for( DWORD i = 0; i < pageSize; ++i )
	{
			// Render the selected item as bright white
		xPos = 10;
		xDelta = 0;
		if( i == (ULONG)m_gameListCursorPosition )
		{
			color = SELECTED_ITEM_COLOR;
			xDelta = 10;
		}
		else
			color = NORMAL_ITEM_COLOR;

		mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX++ ] ].m_description, 255 );
		m_font.DrawText( X_POS + xPos + xDelta, Y_POS + yPos, color, name, XBFONT_TRUNCATED, ( m_additionalinfo ? 230 : WIDTH ) - xDelta );
		if( m_additionalinfo )
		{
			mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_manufacturer, 255 );
			m_font.DrawText( X_POS + 240, Y_POS + yPos, color, name, XBFONT_TRUNCATED, 120 );
			mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_year, 255 );
			m_font.DrawText( X_POS + 370, Y_POS + yPos, color, name, XBFONT_TRUNCATED,  48 );
			mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[ absListIDX - 1 ] ].m_cloneFileName, 255 );
			m_font.DrawText( X_POS + 430, Y_POS + yPos, color, name, XBFONT_TRUNCATED,  60 );
		}

			// Inc the Y position
		yPos += 14;
	}

  #if defined(_PROFILER) || defined(_DEBUG)
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    WCHAR memStr[256];
    swprintf( memStr, 
              L"Memory: %lu/%lu",
              memStatus.dwAvailPhys, 
              memStatus.dwTotalPhys );

    m_font.DrawText( 256, Y_POS + yPos, D3DCOLOR_XRGB(100,220,220), memStr, XBFONT_CENTER_X );
  #else
    m_font.DrawText( 256, Y_POS + yPos, D3DCOLOR_XRGB(100,220,220), L"Press X for help", XBFONT_CENTER_X );
  #endif

  if( m_superscrollMode )
  {
      // Display the superscroll character
    WCHAR displayString[2] = L"";
    mbtowc( displayString, &g_superscrollCharacterSet[m_superscrollCharacterIdx], 1 );
		swprintf( name, L"Names %s [%s]", ( m_allowClones == FALSE ) ? L"(No Clones)" : L"(Clones)   ", displayString );
		m_font.DrawText( X_POS +   0, Y_POS +  0, D3DCOLOR_RGBA( 255, 255, 255, 255 ), name );
  }

	m_font.End();

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

	m_font.Begin();
	
	  m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Searching for ROM files", XBFONT_CENTER_X );

		  // Draw some progress dots
	  WCHAR wBuf[256];
  	
	  wcscpy( wBuf, L"<                >" );
	  wBuf[8 + cursorPos] = L'*';
	  m_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 125, 125, 255), wBuf, XBFONT_CENTER_X );

		  // Draw the current filename
	  mbstowcs( wBuf, fileName, 256 );
	  m_font.DrawText( 320, 100, D3DCOLOR_RGBA( 60, 100, 255, 255 ), wBuf, XBFONT_CENTER_X );

	m_font.End();
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

	m_font.Begin();
	
	m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Checking against known ROM files", XBFONT_CENTER_X );

		  // Draw a progress bar
    UINT32 percentage = (UINT32)( (FLOAT)index * (25.0f / (FLOAT)m_numDrivers) + 0.5f ); 
    UINT32 i = 0;
	  WCHAR wBuf[256] = L"[";
    for( ; i < percentage; ++i )
      wcscat( wBuf, L"|" );
    for( ; i < 25; ++i )
      wcscat( wBuf, L" " );
    wcscat( wBuf, L"]" );

	  m_font.DrawText( 320, 80, D3DCOLOR_XRGB( 255, 125, 125 ), wBuf, XBFONT_CENTER_X );

		  // Draw the current filename
	  mbstowcs( wBuf, m_driverInfoList[index].m_description, 256 );
	  m_font.DrawText( 320, 100, D3DCOLOR_XRGB( 60, 100, 255 ), wBuf, XBFONT_CENTER_X );

	m_font.End();
	m_displayDevice->Present( NULL, NULL, NULL, NULL );

}

/*
//---------------------------------------------------------------------
//	SetCurrentGameIndex
//---------------------------------------------------------------------
void CROMList::SetCurrentGameIndex( UINT32 idx )
{
  if( idx < m_ROMList.size() )
  {
	  UINT32 pageSize = (m_ROMList.size() < MAXPAGESIZE ? m_ROMList.size() : MAXPAGESIZE);
	  UINT32 pageHalfwayPoint = (pageSize >> 1);

    if( idx > pageHalfwayPoint )
    {
      m_gameListPageOffset = (FLOAT)(idx - pageHalfwayPoint);
      m_gameListCursorPosition = (FLOAT)pageHalfwayPoint;
    }
    else
    {
      m_gameListPageOffset = 0;
      m_gameListCursorPosition = (FLOAT)idx;
    }
  }
  else
    m_gameListPageOffset = m_gameListCursorPosition = 0.0f;
}
*/

//---------------------------------------------------------------------
//	RemoveCurrentGameIndex
//---------------------------------------------------------------------
void CROMList::RemoveCurrentGameIndex( void )
{
	UINT32 curCursorPos = (ULONG)m_gameListPageOffset + (ULONG)m_gameListCursorPosition;
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
