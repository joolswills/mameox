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

#include "Sections.h"

#include "MAMEoX.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "ROMList.h"
#include "DebugLogger.h"

	// Font class from the XDK
#include "XBFont.h"

#include "xbox_Timing.h"
#include "xbox_JoystickMouse.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_FileIO.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
}


//= G L O B A L = V A R S =============================================
CInputManager			g_inputManager;
CGraphicsManager	g_graphicsManager;
CXBFont						g_font;


DWORD							g_totalMAMEGames = 0;


//= P R O T O T Y P E S ===============================================
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice );
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 );
static void SetOptions( void );
static BOOL Helper_RunRom( UINT32 romIndex );




//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();

		// Mount the utility drive for storage of the ROM list cache file
	//XMountUtilityDrive( FALSE );

    // Register the loadable section names for lookup at runtime
  RegisterDriverSectionNames();

	SetOptions();

		// Initialize the graphics subsystem
	g_graphicsManager.Create();
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

		// Create a general purpose font
//	g_font.Create( pD3DDevice, "Font.xpr" );
	g_font.Create( "Font.xpr", 0 );

		// Intialize the various MAME OSD-specific subsystems
	InitializeFileIO();
	InitializeTiming();
	InitializeD3DRenderer( g_graphicsManager, &g_font );

		// Initialize the input subsystem
	g_inputManager.Create( 4, 8 );
	const XINPUT_GAMEPAD	&gp0 = g_inputManager.GetGamepadDeviceState( 0 );
	const XINPUT_GAMEPAD	&gp1 = g_inputManager.GetGamepadDeviceState( 1 );
	const XINPUT_GAMEPAD	&gp2 = g_inputManager.GetGamepadDeviceState( 2 );
	const XINPUT_GAMEPAD	&gp3 = g_inputManager.GetGamepadDeviceState( 3 );

		// Show a splash screen
	ShowSplashScreen( pD3DDevice );

    // Unload the XGRAPHICS section, as we won't be using it at all
  XFreeSection( "XGRPH" );

    // This shouldn't be necessary, IMAGEBLD should not load the non-data sections,
    //  as they're defined /NOPRELOAD
  UnloadDriverNonDataSections();    

    // This should only be necessary if IMAGEBLD doesn't load any sections
  LoadDriverDataSections();

	  // Count and sort the game drivers (Taken from XMame)
	for( g_totalMAMEGames = 0; drivers[g_totalMAMEGames]; ++g_totalMAMEGames)
		;
	qsort( drivers, g_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );



		// Load/Generate the ROM listing
	CROMList romList( pD3DDevice, g_font );
	if( !romList.LoadROMList( TRUE ) )
		Die( pD3DDevice, "Could not generate ROM list!" );


		// Main loop
	while( 1 )
	{
		g_inputManager.PollDevices();

			// Reboot on LT+RT+Black
		if( gp0.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 200 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 200 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 200 )
		{
      LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
      XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
		}



			// Handle user input
		if( gp0.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 )
		{
				// Run the selected ROM
      UINT32 romIndex = romList.GetCurrentGameIndex();
      if( romIndex != INVALID_ROM_INDEX  )
        Helper_RunRom( romIndex );
		}
		else if( gp0.bAnalogButtons[XINPUT_GAMEPAD_X] > 200 &&
						 gp0.bAnalogButtons[XINPUT_GAMEPAD_B] > 200 )
		{				
				// Move the currently selected game to the backup dir
			UINT32 romIDX = romList.GetCurrentGameIndex();

			std::string oldPath = ROMPATH "\\";
			oldPath += drivers[romIDX]->name;
			oldPath += ".zip";

			std::string newPath = ROMPATH "\\backup\\";
			newPath += drivers[romIDX]->name;
			newPath += ".zip";

        // Make sure the backup dir exists
      CreateDirectory( ROMPATH "\\backup", NULL );

			PRINTMSG( T_INFO, "Moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );
			if( !MoveFile( oldPath.c_str(), newPath.c_str() ) )
			{
				PRINTMSG( T_ERROR, "Failed moving ROM %s to %s!", oldPath.c_str(), newPath.c_str() );
			}

			g_inputManager.WaitForNoKeys( 0 );
			romList.RemoveCurrentGameIndex();
		}
		else if( gp0.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 200 )
		{
				// Regenerate the rom listing, allowing clones
			romList.GenerateROMList();
		}
    else if( gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 200 )
    {
        // Regenerate the rom listing, hiding clones
      romList.GenerateROMList( FALSE );
    }

		
			// Move the cursor position and render
		romList.MoveCursor( gp0 );
		romList.Draw();
	}
}



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
	
//	g_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255), L"MAMEoX v0.4-67", XBFONT_CENTER_X );
	g_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"B's Momma v0.4", XBFONT_CENTER_X );
  g_font.DrawText( 320, 100, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Uses MAME version 0.67", XBFONT_CENTER_X );
  g_font.DrawText( 320, 260, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"Portions based on:", XBFONT_CENTER_X );
  g_font.DrawText( 320, 280, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"\"MAMEX(b5): updated by superfro, original port by opcode\"", XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255 ), L"Press any button to continue.", XBFONT_CENTER_X );

  g_font.End();
  pD3DDevice->Present( NULL, NULL, NULL, NULL );

	g_inputManager.WaitForAnyKey();
	g_inputManager.WaitForNoKeys( 0 );
}



//-------------------------------------------------------------
//	Die
//-------------------------------------------------------------
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... )
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
	mbstowcs( wBuf, buf, strlen(buf) );

	g_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	g_font.End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );


	g_inputManager.WaitForNoKeys( 0 );
	g_inputManager.WaitForAnyKey( 0 );
	g_inputManager.WaitForNoKeys( 0 );

	
		// Reboot
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
}



//-------------------------------------------------------------
//	compareDriverNames
//-------------------------------------------------------------
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 )
{
	struct GameDriver *drv1 = *(struct GameDriver **)elem1;
	struct GameDriver *drv2 = *(struct GameDriver **)elem2;

		// Test the description field (game name)
	return strcmp( drv1->description, drv2->description );
}




//-------------------------------------------------------------
//	SetOptions
//-------------------------------------------------------------
static void SetOptions( void )
{
/*
	int		mame_debug;		          1 to enable debugging
	int		cheat;			            1 to enable cheating
	int 	gui_host;		            1 to tweak some UI-related things for better GUI integration
	int 	skip_disclaimer;	      1 to skip the disclaimer screen at startup
	int 	skip_gameinfo;		      1 to skip the game info screen at startup
*/

	options.samplerate = 44100;   // sound sample playback rate, in Hz
  options.use_samples = TRUE;   // 1 to enable external .wav samples
  options.use_filter = TRUE;    // 1 to enable FIR filter on final mixer output

	options.brightness = 1.0f;		  // brightness of the display
  options.pause_bright = 0.65f;   // brightness when in pause
	options.gamma = 1.0f;			      // gamma correction of the display
	options.color_depth = 32;
	// int		vector_width;	          // requested width for vector games; 0 means default (640)
	// int		vector_height;	        // requested height for vector games; 0 means default (480)
	// int		ui_orientation;	        // orientation of the UI relative to the video

	//int		beam;			              // vector beam width
	//float	vector_flicker;	        // vector beam flicker effect control
	//float	vector_intensity;       // vector beam intensity
	options.translucency = TRUE;    // 1 to enable translucency on vectors
	options.antialias = TRUE;		    // 1 to enable antialiasing on vectors

	//int		use_artwork;	          bitfield indicating which artwork pieces to use
	//int		artwork_res;	          1 for 1x game scaling, 2 for 2x
	//int		artwork_crop;	          1 to crop artwork to the game screen
	//char	savegame;		            character representing a savegame to load
}

//-------------------------------------------------------------
//  Helper_RunRom
//-------------------------------------------------------------
static BOOL Helper_RunRom( UINT32 romIndex )
{
  BOOL ret;
  if( romIndex == INVALID_ROM_INDEX )
    return FALSE;

	options.ui_orientation = drivers[romIndex]->flags & ORIENTATION_MASK;

    // Because the D3D renderer will be rotating the output into place, 
    //  we have to set up the UI w/ options such that it will produce
    //  graphics inline w/ those produced by the ROM itself (otherwise
    //  the renderer will rotate "correct" UI into an incorrect orientation)
	if( options.ui_orientation & ORIENTATION_SWAP_XY )
	{
		  // if only one of the components is inverted, swap which is inverted
		if( (options.ui_orientation & ROT180) == ORIENTATION_FLIP_X ||
				(options.ui_orientation & ROT180) == ORIENTATION_FLIP_Y)
			options.ui_orientation ^= ROT180;
	}

  #ifdef _PROFILER
  DmStartProfiling( "xd:\\perf.log", 0 );
  #endif


  std::string DriverName = drivers[romIndex]->source_file;
  
  _RPT1( _CRT_WARN, "*** Driver name: %s\n", drivers[romIndex]->source_file );

    // Unload all loaded XBE data sections, as we'll only be using the one
    // for the file we're loading
  UnloadDriverDataSections();

    // Hacks
  LoadDriverSectionByName( "src\\drivers\\cps1.c" );      // cps2.c is dependent on cps1.c
  LoadDriverSectionByName( "src\\drivers\\mpatrol.c" );   // 10 yard fight is dependent on the mpatrol vidhrdw
  LoadDriverSectionByName( "src\\drivers\\snk.c" );       // hal121.c is dependent on snk vidhrdw (ASO - Armored Scrum Object)
  LoadDriverSectionByName( "src\\drivers\\system16.c" );  // afterburner dependent
  LoadDriverSectionByName( "src\\drivers\\galaxian.c" );  // Amidar
  LoadDriverSectionByName( "src\\drivers\\scramble.c" );  // Amidar
  LoadDriverSectionByName( "src\\drivers\\scobra.c" );    // Amidar
  LoadDriverSectionByName( "src\\drivers\\rampart.c" );   // Arcade Classic Arcadecl.c
  LoadDriverSectionByName( "src\\drivers\\williams.c" );  // Archrivals
  LoadDriverSectionByName( "src\\drivers\\rastan.c" );    // Asuka & Asuka (sound)
  LoadDriverSectionByName( "src\\drivers\\hal21.c" );     // Athena
  LoadDriverSectionByName( "src\\drivers\\espial.c" );    // battle cruiser

  if( !LoadDriverSectionByName( DriverName.c_str() ) )
  {
    _RPT1( _CRT_WARN, "Failed to load section for file %s!", DriverName.c_str() );
    PRINTMSG( T_ERROR, "Failed to load section for file %s!", DriverName.c_str() );
  }
  else
  {
		ret = run_game( romIndex );
    UnloadDriverSectionByName( DriverName.c_str() );
  }

    // Reload data XBE sections, as they are needed for ROMList parsing
  LoadDriverDataSections();

  #ifdef _PROFILER
  DmStopProfiling();
  #endif

  return (!ret);  // run_game works on inverted logic
}








extern "C" {
//-------------------------------------------------------------
//	osd_init
//-------------------------------------------------------------
int osd_init( void )
{
  //InitializeJoystickMouse();    // Done in osd_customize_inputport_defaults(), as MAME does input system init _after_ osd_init

	return 0;
}

//-------------------------------------------------------------
//	osd_exit
//-------------------------------------------------------------
void osd_exit( void )
{
  TerminateJoystickMouse();
}


//-------------------------------------------------------------
//	GetGamepadState
//-------------------------------------------------------------
const XINPUT_GAMEPAD *GetGamepadState( UINT32 joynum )
{
	const XINPUT_GAMEPAD	&gp = g_inputManager.GetGamepadDeviceState( joynum );
	return &gp;
}

//-------------------------------------------------------------
//	PollGamepads
//-------------------------------------------------------------
void PollGamepads( void )
{
	g_inputManager.PollDevices();
	const XINPUT_GAMEPAD	&gp0 = g_inputManager.GetGamepadDeviceState( 0 );

		// Reboot on LT+RT+Black
	if( gp0.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 200 &&
			gp0.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 200 &&
			gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 200 )
	{
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
	}
}


//-------------------------------------------------------------
// WaitForKey
//-------------------------------------------------------------
void WaitForKey( void )
{
	g_inputManager.WaitForAnyKey( 0 );
}

//-------------------------------------------------------------
// WaitForNoKey
//-------------------------------------------------------------
void WaitForNoKey( void )
{
	g_inputManager.WaitForNoKeys( 0 );
}

//-------------------------------------------------------------
//	BeginFontRender
//-------------------------------------------------------------
void BeginFontRender( BOOL ClearScreen )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

  if( ClearScreen )
  {
		  // Clear the backbuffer
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                        D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil
  }

  g_font.Begin();

}

//-------------------------------------------------------------
//	FontRender
//-------------------------------------------------------------
void FontRender( INT32 x, INT32 y, UINT32 color, const WCHAR *str, UINT32 flags )
{
	g_font.DrawText( x, y, color, str, flags );
}

//-------------------------------------------------------------
//	EndFontRender
//-------------------------------------------------------------
void EndFontRender( void )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();
  g_font.End();
  pD3DDevice->Present( NULL, NULL, NULL, NULL );
}


#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckRAM
//-------------------------------------------------------------
void CheckRAM( void )
{
  WCHAR memStr[256];
  MEMORYSTATUS memStatus;
  GlobalMemoryStatus(  &memStatus );

  swprintf( memStr, 
            L"Memory: %lu/%lu",
            memStatus.dwAvailPhys, 
            memStatus.dwTotalPhys );

  BeginFontRender( TRUE );
  FontRender( 320, 300, D3DCOLOR_RGBA(255,255,255,255), memStr, 2 );
  EndFontRender();

	g_inputManager.WaitForNoKeys( 0 );
	g_inputManager.WaitForAnyKey( 0 );
	g_inputManager.WaitForNoKeys( 0 );
}
#endif



}	// End Extern "C"

