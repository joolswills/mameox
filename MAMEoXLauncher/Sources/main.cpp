/**
	* \file			xbox_Main.cpp
	* \brief		MAMEoX entry point
	*
	*/


//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <stdarg.h>
#include <stdio.h>
#include <crtdbg.h>

#ifdef _PROFILER
  #include <xbdm.h>
#endif

#include "MAMEoX.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "ROMList.h"
#include "OptionsPage.h"
#include "System_IniFile.h"
#include "xbox_FileIO.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_Timing.h"
#include "DebugLogger.h"

	// Font class from the XDK
#include "XBFont.h"

//= D E F I N E S =====================================================

	// Number of seconds between valid X button readings
#define XBUTTON_TIMEOUT	0.50f


//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
  DWORD         diffuse;  // The diffuse color of the vertex
//  FLOAT         tu, tv;   // The texture coordinates
};

//= G L O B A L = V A R S =============================================
extern CInputManager			g_inputManager;
extern CGraphicsManager	  g_graphicsManager;
extern CXBFont						g_font;

  // XBE Launch data
static DWORD              g_launchDataType;
static LAUNCH_DATA        g_launchData;

  // Defines the percentage of the total screen area that should actually be used
  // This is required because TV's have some overscan area that is not actually
  // visible
static FLOAT                      g_screenXPercentage = DEFAULT_SCREEN_X_PERCENTAGE;
static FLOAT                      g_screenYPercentage = DEFAULT_SCREEN_Y_PERCENTAGE;

static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

  //! The data for each driver
static MAMEDriverData_t   *g_driverData = NULL;


extern "C" {
  // Fake options struct for load/store options
GameOptions options;

  // Fake cheatfile name for load/store options
const char *cheatfile = NULL;
}

//= P R O T O T Y P E S ===============================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );
void DestroyBackdrop( void );
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL Helper_LoadDriverInfoFile( void );

//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	ShowSplashScreen
//-------------------------------------------------------------
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
		// Clear the backbuffer
  pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                      D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

  g_font.Begin();
	
  g_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255),   L"MAMEoX v" LVERSION_STRING, XBFONT_CENTER_X );
  g_font.DrawText( 320, 100, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Uses MAME version 0.67", XBFONT_CENTER_X );
  g_font.DrawText( 320, 260, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"Portions based on:", XBFONT_CENTER_X );
  g_font.DrawText( 320, 280, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"\"MAMEX(b5): updated by superfro, original port by opcode\"", XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255 ),  L"Press any button to continue.", XBFONT_CENTER_X );

  g_font.End();
  pD3DDevice->Present( NULL, NULL, NULL, NULL );

	g_inputManager.WaitForAnyKey();
	g_inputManager.WaitForNoKey( 0 );
}


//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();

		// Initialize the graphics subsystem
	g_graphicsManager.Create();
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

		// Create a general purpose font
// VC6 requires the 2 paramater call to create. _VC6 is defined in the VC6 dsp files
#ifdef _VC6
	g_font.Create( pD3DDevice, "Font.xpr" );
#else
	g_font.Create( "Font.xpr", 0 );
#endif
		// Intialize the various MAME OSD-specific subsystems
	InitializeFileIO();
	InitializeTiming();

		// Initialize the input subsystem
	g_inputManager.Create( 4, 8 );
	const XINPUT_GAMEPAD	&gp0 = g_inputManager.GetGamepadDeviceState( 0 );
	const XINPUT_GAMEPAD	&gp1 = g_inputManager.GetGamepadDeviceState( 1 );
	const XINPUT_GAMEPAD	&gp2 = g_inputManager.GetGamepadDeviceState( 2 );
	const XINPUT_GAMEPAD	&gp3 = g_inputManager.GetGamepadDeviceState( 3 );

	LoadOptions();
  SaveOptions();

    // Get the launch data
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  DWORD ret = XGetLaunchInfo( &g_launchDataType, &g_launchData );

    // See who launched us
  if( ret != ERROR_SUCCESS || g_launchDataType != LDT_TITLE )
  {
		  // Show a splash screen if we were started from the dashboard and not from MAMEoX
	  ShowSplashScreen( pD3DDevice );

      // Create the MAME driver list
    mameoxLaunchData->m_gameIndex = 0;
    mameoxLaunchData->m_cursorPosition = 0.0f; 
    mameoxLaunchData->m_pageOffset = 0.0f;
    mameoxLaunchData->m_totalMAMEGames = 0;
    mameoxLaunchData->m_command = LAUNCH_CREATE_MAME_GAME_LIST;

      // Try to load the MAME driver info, asking MAMEoX to produce
      // it if we can't
    if( !Helper_LoadDriverInfoFile() )
    {
      ShowLoadingScreen( pD3DDevice );
      XLaunchNewImage( "D:\\MAMEoX.xbe", &g_launchData );
		  Die( pD3DDevice, "Could not execute MAMEoX.xbe!" );
    }
  }
  else
  {
      // Load the driver info file
    if( !Helper_LoadDriverInfoFile() )
      Die( pD3DDevice, "Could not load driver info file!" );
  }

  // At this point the MAMEoX process is guaranteed to have run, setting up
  // our totalMAMEGames member, as well as producing the driver info file

		// Load/Generate the ROM listing
  CROMList romList( pD3DDevice, 
                    g_font, 
                    g_driverData, 
                    mameoxLaunchData->m_totalMAMEGames,
                    &g_launchData );
	if( !romList.LoadROMList( TRUE ) )
		Die( pD3DDevice, "Could not generate ROM list!" );

    // Reposition the romList cursor to its previous value
  romList.SetCursorPosition(  mameoxLaunchData->m_cursorPosition, 
                              mameoxLaunchData->m_pageOffset );

    // Grab the current screen usage so we can render a border
  FLOAT xPercentage, yPercentage;
  GetScreenUsage( &xPercentage, &yPercentage );
  CreateBackdrop( xPercentage, yPercentage );

  COptionsPage optionsPage( pD3DDevice,
                            g_font,
                            options );

    //-- Initialize the rendering engine -------------------------------
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );

    // Toggle for whether or not we're in options mode
  BOOL optionsMode = FALSE;
	FLOAT xButtonTimeout = 0.0f;
	UINT64 lastTime = osd_cycles();

		// Main loop
	while( 1 )
	{
		g_inputManager.PollDevices();

	  UINT64 curTime = osd_cycles();
	  FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
	  lastTime = curTime;

		  // Decrement the dpad movement timer
	  if( xButtonTimeout > 0.0f )
	  {
		  xButtonTimeout -= elapsedTime;
		  if( xButtonTimeout < 0.0f )
			  xButtonTimeout = 0.0f;
	  }


			// Reboot on LT+RT+Black
		if( gp0.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 150 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 150 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 150 )
		{
      SaveOptions();
      LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
      XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
		}

    if( gp0.bAnalogButtons[XINPUT_GAMEPAD_X] > 150 && xButtonTimeout == 0.0f )
    {
        // Toggle options mode
      optionsMode = !optionsMode;
      xButtonTimeout = XBUTTON_TIMEOUT;
    }


		
			// Move the cursor position and render
    g_graphicsManager.GetD3DDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    g_graphicsManager.GetD3DDevice()->SetTransform( D3DTS_VIEW, &matWorld );
    g_graphicsManager.GetD3DDevice()->SetTransform( D3DTS_PROJECTION, &matWorld );

	  g_graphicsManager.GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	  g_graphicsManager.GetD3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	  g_graphicsManager.GetD3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    g_graphicsManager.GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
    g_graphicsManager.GetD3DDevice()->Clear(	0L,																// Count
											                        NULL,															// Rects to clear
											                        D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                                              D3DCOLOR_XRGB(0,0,0),							// Color
											                        1.0f,															// Z
											                        0L );															// Stencil
    g_graphicsManager.GetD3DDevice()->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
    g_graphicsManager.GetD3DDevice()->SetStreamSource(	0,												  // Stream number
																	                      g_pD3DVertexBuffer,					// Stream data
																	                      sizeof(CUSTOMVERTEX) );		  // Vertex stride

    g_graphicsManager.GetD3DDevice()->DrawPrimitive( D3DPT_QUADLIST, 0, 5 );

    if( optionsMode )
    {
      optionsPage.MoveCursor( gp0 );
      optionsPage.Draw( FALSE, FALSE );
    }
    else
    {
		  romList.MoveCursor( gp0 );
		  romList.Draw( FALSE, FALSE );
    }

    g_graphicsManager.GetD3DDevice()->Present( NULL, NULL, NULL, NULL );

	}
}

//-------------------------------------------------------------
// Helper_LoadDriverInfoFile
//-------------------------------------------------------------
static BOOL Helper_LoadDriverInfoFile( void )
{
	std::string		driverListFile = ROMLISTPATH;
	driverListFile += "\\";
	driverListFile += DRIVERLISTFILENAME;

	PRINTMSG( T_INFO, "Store driver list: %s", driverListFile.c_str() );

	HANDLE hFile = CreateFile(	driverListFile.c_str(),
															GENERIC_READ,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );
	if( !hFile )
	{
		PRINTMSG( T_ERROR, "Could not open file %s!", driverListFile.c_str() );
		return FALSE;
	}

    // Read in the signature
	DWORD BytesRead = 0;
  char signature[32] = {0};
  ReadFile( hFile, signature, 6 + strlen(VERSION_STRING), &BytesRead, NULL );
  if( BytesRead != 6 + strlen(VERSION_STRING) || strcmp( signature, "MAMEoX" VERSION_STRING ) )
  {
    CloseHandle( hFile );
    return FALSE;
  }


  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  ReadFile( hFile, 
            &mameoxLaunchData->m_totalMAMEGames, 
            sizeof(mameoxLaunchData->m_totalMAMEGames), 
            &BytesRead, 
            NULL );
  if( BytesRead != sizeof(mameoxLaunchData->m_totalMAMEGames) )
  {
    CloseHandle( hFile );
    return FALSE;
  }

    // Read in the driver info
  g_driverData = new MAMEDriverData_t[ mameoxLaunchData->m_totalMAMEGames ];
  for( DWORD i = 0; i < mameoxLaunchData->m_totalMAMEGames; ++i )
  {
    ReadFile( hFile, 
              &g_driverData[i].m_index, 
              sizeof(g_driverData[i].m_index), 
              &BytesRead, 
              NULL );
    if( BytesRead != sizeof(g_driverData[i].m_index) )
    {
      CloseHandle( hFile );
      return FALSE;
    }

      // Read the filename length
    DWORD fileNameLen = 0;
    ReadFile( hFile, 
              &fileNameLen, 
              sizeof(fileNameLen), 
              &BytesRead, 
              NULL );
    if( BytesRead != sizeof(fileNameLen) )
    {
      CloseHandle( hFile );
      return FALSE;
    }

      // Read the filename data
    g_driverData[i].m_romFileName = new char[fileNameLen];
    ReadFile( hFile, 
              g_driverData[i].m_romFileName, 
              fileNameLen, 
              &BytesRead, 
              NULL );
    if( BytesRead != fileNameLen )
    {
      CloseHandle( hFile );
      return FALSE;
    }

      // Read the description length
    ReadFile( hFile, 
              &fileNameLen, 
              sizeof(fileNameLen), 
              &BytesRead, 
              NULL );
    if( BytesRead != sizeof(fileNameLen) )
    {
      CloseHandle( hFile );
      return FALSE;
    }

      // Read the description data
    g_driverData[i].m_description = new char[fileNameLen];
    ReadFile( hFile, 
              g_driverData[i].m_description, 
              fileNameLen, 
              &BytesRead, 
              NULL );
    if( BytesRead != fileNameLen )
    {
      CloseHandle( hFile );
      return FALSE;
    }
    
    ReadFile( hFile, 
              &g_driverData[i].m_isClone, 
              sizeof(g_driverData[i].m_isClone), 
              &BytesRead, 
              NULL );
    if( BytesRead != sizeof(g_driverData[i].m_isClone) )
    {
      CloseHandle( hFile );
      return FALSE;
    }
  }

  CloseHandle( hFile );

  return TRUE;
}

//-------------------------------------------------------------
//	Die
//-------------------------------------------------------------
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... )
{
	char buf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

	PRINTMSG( T_ERROR, "Die: %s", buf );

		// Display the error to the user
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	g_font.Begin();
	
	WCHAR wBuf[1024];
	mbstowcs( wBuf, buf, strlen(buf) + 1 );

	g_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	g_font.End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );

	g_inputManager.WaitForNoKey( 0 );
	g_inputManager.WaitForAnyKey( 0 );
	g_inputManager.WaitForNoKey( 0 );

    // Reboot
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
}



//-------------------------------------------------------------
//  CreateBackdrop
//-------------------------------------------------------------
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }

    // Create the vertex buffer
  g_graphicsManager.GetD3DDevice()->CreateVertexBuffer( (sizeof(CUSTOMVERTEX) << 2) * 5,
																		                    D3DUSAGE_WRITEONLY,
																		                    D3DFVF_XYZ | D3DFVF_DIFFUSE,
																		                    D3DPOOL_MANAGED,
																		                    &g_pD3DVertexBuffer );

	CUSTOMVERTEX *pVertices;
	g_pD3DVertexBuffer->Lock( 0,										// Offset to lock
														0,										// Size to lock
														(BYTE**)&pVertices,		// ppbData
														0 );									// Flags

      //-- Draw the backdrop -------------------------------------------------
		pVertices[0].pos.x = -xUsage;
		pVertices[0].pos.y = yUsage;
		pVertices[0].pos.z = 1.0f;
    pVertices[0].diffuse = D3DCOLOR_RGBA( 30, 50, 30, 255 );
    //pVertices[0].tu = 0.0f;
    //pVertices[0].tv = 0.0f;

		pVertices[1].pos.x = xUsage;
		pVertices[1].pos.y = yUsage;
		pVertices[1].pos.z = 1.0f;
    pVertices[1].diffuse = D3DCOLOR_RGBA( 30, 50, 30, 255 );
    //pVertices[1].tu = 1.0f;
    //pVertices[1].tv = 0.0f;
		
		pVertices[2].pos.x = xUsage;
		pVertices[2].pos.y = -yUsage;
		pVertices[2].pos.z = 1.0f;
    pVertices[2].diffuse = D3DCOLOR_RGBA( 30, 50, 30, 255 );
    //pVertices[2].tu = 1.0f;
    //pVertices[2].tv = 1.0f;
		
		pVertices[3].pos.x = -xUsage;
		pVertices[3].pos.y = -yUsage;
		pVertices[3].pos.z = 1.0f;
    pVertices[3].diffuse = D3DCOLOR_RGBA( 30, 50, 30, 255 );
    //pVertices[3].tu = 0.0f;
    //pVertices[3].tv = 1.0f;



      //-- Draw the corner pieces -----------------------------
    #define LINE_WIDTH    0.02f
    #define LINE_COLOR    D3DCOLOR_RGBA( 150, 150, 100, 255 );

		pVertices[4].pos.x = -xUsage;
		pVertices[4].pos.y = yUsage;
		pVertices[4].pos.z = 1.0f;
    pVertices[4].diffuse = LINE_COLOR

		pVertices[5].pos.x = xUsage;
		pVertices[5].pos.y = yUsage;
		pVertices[5].pos.z = 1.0f;
    pVertices[5].diffuse = LINE_COLOR;
		
		pVertices[6].pos.x = xUsage;
		pVertices[6].pos.y = yUsage - LINE_WIDTH;
		pVertices[6].pos.z = 1.0f;
    pVertices[6].diffuse = LINE_COLOR;
		
		pVertices[7].pos.x = -xUsage;
		pVertices[7].pos.y = yUsage - LINE_WIDTH;
		pVertices[7].pos.z = 1.0f;
    pVertices[7].diffuse = LINE_COLOR;


		pVertices[8].pos.x = -xUsage;
		pVertices[8].pos.y = -yUsage;
		pVertices[8].pos.z = 1.0f;
    pVertices[8].diffuse = LINE_COLOR

		pVertices[9].pos.x = xUsage;
		pVertices[9].pos.y = -yUsage;
		pVertices[9].pos.z = 1.0f;
    pVertices[9].diffuse = LINE_COLOR;
		
		pVertices[10].pos.x = xUsage;
		pVertices[10].pos.y = -yUsage + LINE_WIDTH;
		pVertices[10].pos.z = 1.0f;
    pVertices[10].diffuse = LINE_COLOR;
		
		pVertices[11].pos.x = -xUsage;
		pVertices[11].pos.y = -yUsage + LINE_WIDTH;
		pVertices[11].pos.z = 1.0f;
    pVertices[11].diffuse = LINE_COLOR;


		pVertices[12].pos.x = -xUsage;
		pVertices[12].pos.y = -yUsage;
		pVertices[12].pos.z = 1.0f;
    pVertices[12].diffuse = LINE_COLOR

		pVertices[13].pos.x = -xUsage + LINE_WIDTH;
		pVertices[13].pos.y = -yUsage;
		pVertices[13].pos.z = 1.0f;
    pVertices[13].diffuse = LINE_COLOR;
		
		pVertices[14].pos.x = -xUsage + LINE_WIDTH;
		pVertices[14].pos.y = yUsage;
		pVertices[14].pos.z = 1.0f;
    pVertices[14].diffuse = LINE_COLOR;
		
		pVertices[15].pos.x = -xUsage;
		pVertices[15].pos.y = yUsage;
		pVertices[15].pos.z = 1.0f;
    pVertices[15].diffuse = LINE_COLOR;


		pVertices[16].pos.x = xUsage;
		pVertices[16].pos.y = -yUsage;
		pVertices[16].pos.z = 1.0f;
    pVertices[16].diffuse = LINE_COLOR

		pVertices[17].pos.x = xUsage - LINE_WIDTH;
		pVertices[17].pos.y = -yUsage;
		pVertices[17].pos.z = 1.0f;
    pVertices[17].diffuse = LINE_COLOR;
		
		pVertices[18].pos.x = xUsage - LINE_WIDTH;
		pVertices[18].pos.y = yUsage;
		pVertices[18].pos.z = 1.0f;
    pVertices[18].diffuse = LINE_COLOR;
		
		pVertices[19].pos.x = xUsage;
		pVertices[19].pos.y = yUsage;
		pVertices[19].pos.z = 1.0f;
    pVertices[19].diffuse = LINE_COLOR;




	g_pD3DVertexBuffer->Unlock();


  return TRUE;
}

//-------------------------------------------------------------
//  DestroyBackdrop
//-------------------------------------------------------------
void DestroyBackdrop( void )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }
}

//-------------------------------------------------------------
//	SetScreenUsage
//-------------------------------------------------------------
void SetScreenUsage( FLOAT xPercentage, FLOAT yPercentage )
{
  g_screenXPercentage = xPercentage;
  g_screenYPercentage = yPercentage;
}

//-------------------------------------------------------------
//	GetScreenUsage
//-------------------------------------------------------------
void GetScreenUsage( FLOAT *xPercentage, FLOAT *yPercentage )
{
  if( xPercentage )
    *xPercentage = g_screenXPercentage;
  if( yPercentage )
    *yPercentage = g_screenYPercentage;
}



extern "C" {
//-------------------------------------------------------------
//	osd_init
//-------------------------------------------------------------
int osd_init( void )
{
	return 0;
}

//-------------------------------------------------------------
//	osd_exit
//-------------------------------------------------------------
void osd_exit( void )
{
}

}	// End Extern "C"

