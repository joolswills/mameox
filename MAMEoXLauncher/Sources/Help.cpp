/**
	* \file			Help.cpp
	* \brief		Helper class which display the help.txt
	*
	*/

//= I N C L U D E S ====================================================
#include "Help.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage

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

  // Analog stick deadzone
#define STICK_DEADZONE          0.45f

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f


//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );              // Defined in main.cpp
void DestroyBackdrop( void );                                   // Defined in main.cpp
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	osd_fgets
//---------------------------------------------------------------------
BOOL CHelp::FileGets( HANDLE file, char *buffer, UINT32 length )
{
	char c;
	int i = 0;
	DWORD BytesRead = 0;

	while( ReadFile( file, &c, sizeof( c ), &BytesRead , NULL ) && BytesRead > 0 )
	{
		if( i == length - 1 )
		{
			buffer[ i++ ] = '\0';
			return( TRUE );
		}
		if( c == '\n' )
		{
			buffer[ i++ ] = c;
			buffer[ i++ ] = '\0';
			return( TRUE );
		}

		buffer[ i++ ] = c;
	}

	return( FALSE );
}

//---------------------------------------------------------------------
//	LoadHelpFile
//---------------------------------------------------------------------
BOOL CHelp::LoadHelpFile()
{
	PRINTMSG( T_TRACE, "LoadHelpFile" );

	m_Help.clear();

	std::string		HelpFile = HELPFILENAME;

	PRINTMSG( T_INFO, "Load help file: %s", HelpFile.c_str() );
	HANDLE hFile = CreateFile(	HelpFile.c_str(),
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
		m_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Loading Help file", XBFONT_CENTER_X );
		m_font.End();
		m_displayDevice->Present( NULL, NULL, NULL, NULL );	

			// Read in the Help file
		char buffer[ 4096 ];
		std::string line;
		int tst = 0;
		while( FileGets( hFile, buffer, 4096 ) ) //&& tst++ < 286 )
		{
			line = buffer;

			m_Help.push_back( line );
		}

		CloseHandle( hFile );

		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CHelp::MoveCursor( const XINPUT_GAMEPAD	&gp )
{
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
		if( m_dpadCursorDelay < 0.0f )
			m_dpadCursorDelay = 0.0f;
	}

		// General idea taken from XMAME
		// The combined trigger offset, scaled to the range [-1.0f,1.0f]
	FLOAT cursorVelocity =  ((FLOAT)gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] - 
												  (FLOAT)gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER]) / 256.0f;


		// Reset the speed band timeout
	if( cursorVelocity < 0.99f && cursorVelocity > -0.99f )
		m_helpCursorSpeedBandTimeout = 0.0f;
	else
	{
		m_helpCursorSpeedBandTimeout += elapsedTime;
		if( m_helpCursorSpeedBandTimeout > SBTIMEOUT_FASTEST )
			m_helpCursorSpeedBandTimeout = SBTIMEOUT_FASTEST;
	}

		// DPAD overrides the triggers
  if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) || gp.sThumbLY < (-32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_helpPageOffset = (FLOAT)((LONG)m_helpPageOffset);
		m_helpCursorPosition = (FLOAT)((LONG)m_helpCursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_UP) || gp.sThumbLY > (32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that subtracting 1 will move to the next item
    m_helpPageOffset = (FLOAT)((LONG)m_helpPageOffset);
		m_helpCursorPosition = (FLOAT)((LONG)m_helpCursorPosition);
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
	if( m_helpCursorSpeedBandTimeout == SBTIMEOUT_FASTEST )
		cursorVelocity *= SBMULTIPLIER_FASTEST;
	else if( m_helpCursorSpeedBandTimeout > SBTIMEOUT_FASTER )
		cursorVelocity *= SBMULTIPLIER_FASTER;
	else if( m_helpCursorSpeedBandTimeout > SBTIMEOUT_FAST )
		cursorVelocity *= SBMULTIPLIER_FAST;


	DWORD pageSize = (m_Help.size() < MAXPAGESIZE ? m_Help.size() : MAXPAGESIZE);
	ULONG pageHalfwayPoint = (pageSize >> 1);
	ULONG maxPageOffset = m_Help.size() - pageSize;

	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_helpCursorPosition < pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_helpCursorPosition) < pageHalfwayPoint )
				m_helpCursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_helpCursorPosition);
				m_helpCursorPosition = (FLOAT)pageHalfwayPoint;
				m_helpPageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (ULONG)(cursorVelocity + m_helpPageOffset) <= maxPageOffset )
			{
				m_helpPageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_helpPageOffset) <= (FLOAT)maxPageOffset )
					m_helpCursorPosition += cursorVelocity;
				else
				{
					cursorVelocity -= ((FLOAT)maxPageOffset - m_helpPageOffset);
					m_helpPageOffset = (FLOAT)maxPageOffset;
					m_helpCursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (ULONG)m_helpPageOffset > maxPageOffset )
			m_helpPageOffset = (FLOAT)maxPageOffset;
		if( (ULONG)m_helpCursorPosition > (pageSize - 1) )
			m_helpCursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_helpCursorPosition > pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_helpCursorPosition) > pageHalfwayPoint )
				m_helpCursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_helpCursorPosition);
				m_helpCursorPosition = (FLOAT)pageHalfwayPoint;
				m_helpPageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (LONG)(cursorVelocity + m_helpPageOffset) >= 0 )
			{
				m_helpPageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_helpPageOffset) >= 0.0f )
					m_helpCursorPosition += cursorVelocity;
				else
				{
					cursorVelocity += m_helpPageOffset;
					m_helpPageOffset = 0.0f;
					m_helpCursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (LONG)m_helpPageOffset < 0 )
			m_helpPageOffset = 0.0f;
		if( (LONG)m_helpCursorPosition < 0 )
			m_helpCursorPosition = 0.0f;
	}
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CHelp::Draw( BOOL opaque, BOOL flipOnCompletion )
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

		// Render the titles
	DWORD color;
	FLOAT xPos;
	FLOAT yPos = 64;
	DWORD pageSize = (m_Help.size() < MAXPAGESIZE ? m_Help.size() : MAXPAGESIZE);
	ULONG absListIDX = (ULONG)m_helpPageOffset;
	if( absListIDX > (m_Help.size() - pageSize) )
	{
			// The current page offset is invalid (due to list shrinkage), reset it and
			//  set the cursor position to the last item in the list
    absListIDX = (m_Help.size() - pageSize);
    m_helpPageOffset = (FLOAT)absListIDX;
		m_helpCursorPosition = (FLOAT)(pageSize - 1);
	}

	for( DWORD i = 0; i < pageSize; ++i )
	{
		mbstowcs( name, m_Help[ absListIDX++ ].c_str(), 255 );

			// Render the selected item as bright white
		xPos = 70;
		if( i == (ULONG)m_helpCursorPosition )
			color = SELECTED_ITEM_COLOR;
		else
			color = NORMAL_ITEM_COLOR;
		m_font.DrawText( xPos, yPos, color, name, XBFONT_TRUNCATED, 500 );

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

    m_font.DrawText( 320, yPos, D3DCOLOR_RGBA(100,220,220,255), memStr, XBFONT_CENTER_X );
  #endif

	m_font.End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}
