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

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================

#define NORMAL_ITEM_COLOR				D3DCOLOR_RGBA( 100, 255, 100, 255 )
#define SELECTED_ITEM_COLOR			D3DCOLOR_RGBA( 255, 255, 255, 255 )

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							18

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

	std::string		romListFile = g_ROMListPath;
	romListFile += "\\";
	romListFile += ROMLISTFILENAME;

	PRINTMSG( T_INFO, "Load rom list: %s", romListFile.c_str() );
	HANDLE hFile = CreateFile(	romListFile.c_str(),
															GENERIC_READ,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );

	if( hFile != INVALID_HANDLE_VALUE )
	{
			// Display the error to the user
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

  
    char signature[8] = {0};
		DWORD BytesRead = 0;
    ReadFile( hFile, signature, 6, &BytesRead, NULL );
    if( BytesRead != 6 || strcmp( signature, "MAMEoX" ) )
    {
      CloseHandle( hFile );
	    if( bGenerate )
        return GenerateROMList();		
      return FALSE;
    }

			// Read in the ROM list
		while( 1 )
		{
			DWORD idx;
			ReadFile( hFile, &idx, sizeof(idx), &BytesRead, NULL );
			if( BytesRead != sizeof(idx) )
				break;

			m_ROMListWithClones.push_back( idx );
      if( !m_driverInfoList[idx].m_isClone )
        m_ROMListNoClones.push_back( idx );
		}

		CloseHandle( hFile );
    GenerateSuperscrollJumpTable();
		return TRUE;
	}
	else if( bGenerate )
  {
    return GenerateROMList();		
  }

	return FALSE;
}

//---------------------------------------------------------------------
//	GenerateROMList
//---------------------------------------------------------------------
BOOL CROMList::GenerateROMList( void )
{
	PRINTMSG( T_TRACE, "GenerateROMList" );

	m_ROMListWithClones.clear();
  m_ROMListNoClones.clear();

	std::vector< CStdString > zipFileNames;
	WIN32_FIND_DATA findData;

	PRINTMSG( T_INFO, "Finding files %s\\*.zip", g_FileIOConfig.m_RomPath );

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
      PRINTMSG( T_ERROR, "Could not find files!" );
      return FALSE;
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
    DrawZipData( m_driverInfoList[i].m_description, i );
		std::vector<CStdString>::iterator it = zipFileNames.begin();
		for( ; it != zipFileNames.end(); ++it )
		{
      if( it->CompareNoCase(m_driverInfoList[i].m_romFileName) == 0 )
			{
        if( !m_driverInfoList[i].m_isClone )
          m_ROMListNoClones.push_back( i );
        m_ROMListWithClones.push_back( i );
			}
		}
	}

	PRINTMSG( T_INFO, "Found %lu games, %lu unique!", m_ROMListWithClones.size(), m_ROMListNoClones.size() );


		// Write the indices to the ROM list file
	std::string	romListFile = g_ROMListPath;
	romListFile += "\\";
	romListFile += ROMLISTFILENAME;

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

  DWORD bytesWritten;
  WriteFile( hFile, "MAMEoX", 6, &bytesWritten, NULL );
  if( bytesWritten != 6 )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		CloseHandle( hFile );

			// Delete the file
		DeleteFile( romListFile.c_str() );
		return FALSE;
	}

	std::vector<UINT32>::iterator it = m_ROMListWithClones.begin();
	for( ; it != m_ROMListWithClones.end(); ++it )
	{
		DWORD idx = (*it);
		WriteFile( hFile, &idx, sizeof(idx), &bytesWritten, NULL );
		if( bytesWritten != sizeof(idx) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( romListFile.c_str() );
			return FALSE;
		}
	}

	CloseHandle( hFile );

    // Create the superscroll jump table
  GenerateSuperscrollJumpTable();
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
    GenerateROMList();
		WaitForNoButton();
  }
  else if( gp.IsOneOfButtonsPressed( GP_A | GP_START ) )
	{
			// Run the selected ROM
    if( GetCurrentGameIndex() != INVALID_ROM_INDEX  )
    {
        // Pack info to be passed to MAMEoX
      MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)m_launchData->Data;
      mameoxLaunchData->m_gameIndex = GetCurrentGameIndex();
      GetCursorPosition(  &mameoxLaunchData->m_cursorPosition, 
                          &mameoxLaunchData->m_pageOffset,
                          &mameoxLaunchData->m_superscrollIndex );
      mameoxLaunchData->m_command = LAUNCH_RUN_GAME;

      SaveOptions();
      ShowLoadingScreen( m_displayDevice );
      XLaunchNewImage( "D:\\MAMEoX.xbe", m_launchData );
		  Die( m_displayDevice, "Could not execute MAMEoX.xbe!" );
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
  char CurrentGameSuperscrollChar = toupper( m_driverInfoList[GetCurrentGameIndex()].m_description[0] );
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
void CROMList::Draw( BOOL opaque, BOOL flipOnCompletion )
{
	WCHAR name[256];

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
	m_font.DrawText(  70, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), name );
	if( m_additionalinfo )
	{
		m_font.DrawText( 440, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Rate" );
		m_font.DrawText( 498, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Favorite" );
		m_font.DrawText(  65, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"----------------------------------------------------" );
		m_font.DrawText( 435, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"--------" );
		m_font.DrawText( 495, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"------------" );
	}
	else
		m_font.DrawText(  65, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"-------------------------------------------------------------------------" );

		// Render the titles
	DWORD color;
	FLOAT xPos, xDelta;
	FLOAT yPos = 64;
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
		mbstowcs( name, m_driverInfoList[ CURRENTROMLIST()[absListIDX++] ].m_description, 255 );

			// Render the selected item as bright white
		xPos = 70;
		xDelta = 0;
		if( i == (ULONG)m_gameListCursorPosition )
		{
			color = SELECTED_ITEM_COLOR;
			xDelta = 16;
		}
		else
			color = NORMAL_ITEM_COLOR;
		m_font.DrawText( xPos + xDelta, yPos, color, name, XBFONT_TRUNCATED, ( m_additionalinfo ? 365 : 500 ) - xDelta );
		if( m_additionalinfo )
		{
			m_font.DrawText( 445, yPos, color, L"100" );
			m_font.DrawText( 518, yPos, color, L"Yes" );
		}

			// Inc the Y position
		yPos += 20;
	}

  #if defined(_PROFILER) || defined(_DEBUG)
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    WCHAR memStr[256];
    swprintf( memStr, 
              L"Memory: %lu/%lu",
              memStatus.dwAvailPhys, 
              memStatus.dwTotalPhys );

    m_font.DrawText( 320, yPos, D3DCOLOR_XRGB(100,220,220), memStr, XBFONT_CENTER_X );
  #endif


  if( m_superscrollMode )
  {
      // Display the superscroll character
    WCHAR displayString[2] = L"";
    mbtowc( displayString, &g_superscrollCharacterSet[m_superscrollCharacterIdx], 1 );
		swprintf( name, L"Names %s [%s]", ( m_allowClones == FALSE ) ? L"(No Clones)" : L"(Clones)   ", displayString );
		m_font.DrawText(  70, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), name );
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
	PRINTMSG( T_TRACE, "DrawZipData" );

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
