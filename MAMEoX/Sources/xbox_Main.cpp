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

#include "System_IniFile.h"
#include "xbox_Timing.h"
#include "xbox_JoystickMouse.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_FileIO.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
}

//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
  DWORD         diffuse;  // The diffuse color of the vertex
//  FLOAT         tu, tv;   // The texture coordinates
};

//= G L O B A L = V A R S =============================================
CInputManager			g_inputManager;
CGraphicsManager	g_graphicsManager;
CXBFont						g_font;

static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;


#define SCREENRANGE_DEADZONE    15000


//= P R O T O T Y P E S ===============================================
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice );
static BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );
static void DestroyBackdrop( void );
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 );
static void LoadOptions( void );
static void SaveOptions( void );
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

	LoadOptions();
  SaveOptions();

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
  DWORD totalMAMEGames = 0;
	for( totalMAMEGames = 0; drivers[totalMAMEGames]; ++totalMAMEGames)
		;
	qsort( drivers, totalMAMEGames, sizeof(drivers[0]), compareDriverNames );



		// Load/Generate the ROM listing
	CROMList romList( pD3DDevice, g_font );
	if( !romList.LoadROMList( TRUE ) )
		Die( pD3DDevice, "Could not generate ROM list!" );

    // Grab the current screen usage so we can render a border
  FLOAT xPercentage, yPercentage;
  GetScreenUsage( &xPercentage, &yPercentage );
  CreateBackdrop( xPercentage, yPercentage );


    //-- Initialize the rendering engine -------------------------------
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );

		// Main loop
	while( 1 )
	{
		g_inputManager.PollDevices();

			// Reboot on LT+RT+Black
		if( gp0.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 200 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 200 &&
				gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 200 )
		{
        // Strangely enough, we need XGRPH to use ifstream
      XLoadSection( "XGRPH" );    
      SaveOptions();
      LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
      XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
		}



			// Handle user input
		if( gp0.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 )
		{
				// Run the selected ROM
      UINT32 romIndex = romList.GetCurrentGameIndex();
      if( romIndex != INVALID_ROM_INDEX  )
      {
        DestroyBackdrop();
        Helper_RunRom( romIndex );
        CreateBackdrop( xPercentage, yPercentage );
      }
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
    else if(  gp0.sThumbLX < -SCREENRANGE_DEADZONE || gp0.sThumbLX > SCREENRANGE_DEADZONE || 
              gp0.sThumbLY < -SCREENRANGE_DEADZONE || gp0.sThumbLY > SCREENRANGE_DEADZONE )
    {
      if( gp0.sThumbLX < -SCREENRANGE_DEADZONE )
        xPercentage -= 0.00025f;
      else if( gp0.sThumbLX > SCREENRANGE_DEADZONE )
        xPercentage += 0.00025f;

      if( gp0.sThumbLY < -SCREENRANGE_DEADZONE )
        yPercentage -= 0.00025f;
      else if( gp0.sThumbLY > SCREENRANGE_DEADZONE )
        yPercentage += 0.00025f;

      if( xPercentage < 0.25f )
        xPercentage = 0.25f;
      else if( xPercentage > 1.0f )
        xPercentage = 1.0f;

      if( yPercentage < 0.25f )
        yPercentage = 0.25f;
      else if( yPercentage > 1.0f )
        yPercentage = 1.0f;

      SetScreenUsage( xPercentage, yPercentage );
      DestroyBackdrop();
      CreateBackdrop( xPercentage, yPercentage );
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
    g_graphicsManager.GetD3DDevice()->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

		romList.MoveCursor( gp0 );
		romList.Draw( FALSE, FALSE );

    g_graphicsManager.GetD3DDevice()->Present( NULL, NULL, NULL, NULL );

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
	
  g_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255),   L"MAMEoX v0.61b", XBFONT_CENTER_X );
  g_font.DrawText( 320, 100, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Uses MAME version 0.67", XBFONT_CENTER_X );
  g_font.DrawText( 320, 260, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"Portions based on:", XBFONT_CENTER_X );
  g_font.DrawText( 320, 280, D3DCOLOR_RGBA( 240, 240, 240, 255 ), L"\"MAMEX(b5): updated by superfro, original port by opcode\"", XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255 ),  L"Press any button to continue.", XBFONT_CENTER_X );

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
//	LoadOptions
//-------------------------------------------------------------
static void LoadOptions( void )
{
  CSystem_IniFile iniFile( INIFILENAME );

/*
	int		mame_debug;		          1 to enable debugging
	int		cheat;			            1 to enable cheating
	int 	gui_host;		            1 to tweak some UI-related things for better GUI integration
	int 	skip_disclaimer;	      1 to skip the disclaimer screen at startup
	int 	skip_gameinfo;		      1 to skip the game info screen at startup
*/

    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );

	options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
	options.vector_width = iniFile.GetProfileInt( "Video", "ColorDepth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "Video", "ColorDepth", 480 );	    // requested height for vector games; 0 means default (480)
	// int		ui_orientation;	        // orientation of the UI relative to the video

	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", TRUE );      // 1 to enable translucency on vectors
	options.antialias = FALSE;		    // 1 to enable antialiasing on vectors

	//int		use_artwork;	          bitfield indicating which artwork pieces to use
	//int		artwork_res;	          1 for 1x game scaling, 2 for 2x
	//int		artwork_crop;	          1 to crop artwork to the game screen
	//char	savegame;		            character representing a savegame to load


  FLOAT xPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_X", DEFAULT_SCREEN_X_PERCENTAGE );
  FLOAT yPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_Y", DEFAULT_SCREEN_Y_PERCENTAGE );
  SetScreenUsage( xPercentage, yPercentage );
}


//-------------------------------------------------------------
//	SaveOptions
//-------------------------------------------------------------
static void SaveOptions( void )
{
  CSystem_IniFile iniFile( INIFILENAME );

  iniFile.WriteProfileInt( "Sound", "SampleRate", options.samplerate );
  iniFile.WriteProfileInt( "Sound", "UseSamples", options.use_samples );
  iniFile.WriteProfileInt( "Sound", "UseFilter", options.use_filter );
  iniFile.WriteProfileFloat( "Video", "Brightness", options.brightness );		    // brightness of the display
  iniFile.WriteProfileFloat( "Video", "PauseBrightness", options.pause_bright );     // brightness when in pause
	iniFile.WriteProfileFloat( "Video", "Gamma", options.gamma );			        // gamma correction of the display
	iniFile.WriteProfileInt( "Video", "ColorDepth", options.color_depth );
	iniFile.WriteProfileInt( "Video", "ColorDepth", options.vector_width );	      // requested width for vector games; 0 means default (640)
	iniFile.WriteProfileInt( "Video", "ColorDepth", options.vector_height );	    // requested height for vector games; 0 means default (480)
	iniFile.WriteProfileInt( "VectorOptions", "BeamWidth", options.beam );			            // vector beam width
	iniFile.WriteProfileFloat( "VectorOptions", "FlickerEffect", options.vector_flicker );	  // vector beam flicker effect control
	iniFile.WriteProfileFloat( "VectorOptions", "BeamIntensity", options.vector_intensity );  // vector beam intensity
	iniFile.WriteProfileInt( "VectorOptions", "BeamWidth", options.translucency );      // 1 to enable translucency on vectors

  FLOAT xPercentage, yPercentage;
  GetScreenUsage( &xPercentage, &yPercentage );
  iniFile.WriteProfileFloat( "Video", "ScreenUsage_X", xPercentage );
  iniFile.WriteProfileFloat( "Video", "ScreenUsage_Y", yPercentage );
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
  LoadDriverSectionByName( "src\\drivers\\bzone.c" );     // gravitar
  LoadDriverSectionByName( "src\\drivers\\nova2001.c" );  // Penguin-Kun War
  LoadDriverSectionByName( "src\\drivers\\gottlieb.c" );  // exterminator
  LoadDriverSectionByName( "src\\drivers\\pengo.c" );     // eyes
  LoadDriverSectionByName( "src\\drivers\\megasys1.c" );  // F1 Grand Prix Star
  LoadDriverSectionByName( "src\\drivers\\namcos1.c" );   // Face Off (Japan)
  LoadDriverSectionByName( "src\\drivers\\rallyx.c" );    // Loco-Motion
  LoadDriverSectionByName( "src\\drivers\\timeplt.c" );   // Loco-Motion
  LoadDriverSectionByName( "src\\drivers\\exidy.c" );     // Victory


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


//-------------------------------------------------------------
//  CreateBackdrop
//-------------------------------------------------------------
static BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }

    // Create the vertex buffer
  g_graphicsManager.GetD3DDevice()->CreateVertexBuffer( sizeof(CUSTOMVERTEX) << 2,
																		                    D3DUSAGE_WRITEONLY,
																		                    D3DFVF_XYZ | D3DFVF_DIFFUSE,
																		                    D3DPOOL_MANAGED,
																		                    &g_pD3DVertexBuffer );

	CUSTOMVERTEX *pVertices;
	g_pD3DVertexBuffer->Lock( 0,										// Offset to lock
														0,										// Size to lock
														(BYTE**)&pVertices,		// ppbData
														0 );									// Flags

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

	g_pD3DVertexBuffer->Unlock();


  return TRUE;
}

//-------------------------------------------------------------
//  DestroyBackdrop
//-------------------------------------------------------------
static void DestroyBackdrop( void )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }
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
/*
		// Reboot on LT+RT+Black
	if( gp0.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 200 &&
			gp0.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 200 &&
			gp0.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 200 )
	{
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
	}
*/
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

