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
extern CInputManager			g_inputManager;
extern CGraphicsManager	  g_graphicsManager;
extern CXBFont						g_font;

  // XBE Launch data
DWORD             g_launchDataType;
LAUNCH_DATA       g_launchData;

static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

//= P R O T O T Y P E S ===============================================
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL Helper_RunRom( UINT32 romIndex );
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 );
static BOOL Helper_SaveDriverList( void );

//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();

    // Get the launch data immediately, just in case Die() is called
  DWORD ret = XGetLaunchInfo( &g_launchDataType, &g_launchData );

		// Mount the utility drive for storage of the ROM list cache file
	//XMountUtilityDrive( FALSE );

    // Register the loadable section names for lookup at runtime
  RegisterDriverSectionNames();

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
	InitializeD3DRenderer( g_graphicsManager, &g_font );

		// Initialize the input subsystem
	g_inputManager.Create( 4, 8 );

	LoadOptions();
  SaveOptions();


    // Check the launch data to ensure that we've been started properly
  if( ret != ERROR_SUCCESS || g_launchDataType != LDT_TITLE )
  {
      // This XBE shouldn't be launched directly. Throw up a splash screen
      // saying so, then try to launch MAMEoxLauncher
    Die( pD3DDevice, "MAMEoX wasn't called from the launcher!\nPlease run default.xbe instead.\n" );
      // Die never returns
  }
 
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;

    // Create the sorted game listing and exit
  if( mameoxLaunchData->m_command == LAUNCH_CREATE_MAME_GAME_LIST )
  {
	    // Count and sort the game drivers (Taken from XMame)
    DWORD totalMAMEGames = 0;
	  for( totalMAMEGames = 0; drivers[totalMAMEGames]; ++totalMAMEGames)
		  ;
    mameoxLaunchData->m_totalMAMEGames = totalMAMEGames;
    mameoxLaunchData->m_gameIndex = 0;

    qsort( drivers, mameoxLaunchData->m_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );

      // Dump the drivers to a file
    Helper_SaveDriverList();
  }
  else
  {
      // Unload the XGRAPHICS section, as we won't be using it at all
    XFreeSection( "XGRPH" );

      // This shouldn't be necessary, IMAGEBLD should not load the non-data sections,
      //  as they're defined /NOPRELOAD
    UnloadDriverNonDataSections();    

      // This should only be necessary if IMAGEBLD doesn't load any sections
    LoadDriverDataSections();

      // Sort the game drivers and run the ROM
    qsort( drivers, mameoxLaunchData->m_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );

    Helper_RunRom( mameoxLaunchData->m_gameIndex );
  }

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  XLaunchNewImage( "D:\\default.xbe", &g_launchData );
}

//-------------------------------------------------------------
//  Helper_SaveDriverList
//-------------------------------------------------------------
static BOOL Helper_SaveDriverList( void )
{
	std::string		driverListFile = g_ROMListPath;
	driverListFile += "\\";
	driverListFile += DRIVERLISTFILENAME;

	PRINTMSG( T_INFO, "Store driver list: %s", driverListFile.c_str() );

	HANDLE hFile = CreateFile(	driverListFile.c_str(),
															GENERIC_READ | GENERIC_WRITE,
															0,
															NULL,
															CREATE_ALWAYS,
															FILE_ATTRIBUTE_NORMAL,
															NULL );
	if( !hFile )
	{
		PRINTMSG( T_ERROR, "Could not create file %s!", driverListFile.c_str() );
		return FALSE;
	}

  DWORD bytesWritten;
  WriteFile( hFile, "MAMEoX" VERSION_STRING, 6 + strlen(VERSION_STRING), &bytesWritten, NULL );
  if( bytesWritten != 6 + strlen(VERSION_STRING) )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		CloseHandle( hFile );

			// Delete the file
		DeleteFile( driverListFile.c_str() );
		return FALSE;
	}


  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;

    // Store the number of drivers
  WriteFile(  hFile, 
              &mameoxLaunchData->m_totalMAMEGames, 
              sizeof(mameoxLaunchData->m_totalMAMEGames),
              &bytesWritten, 
              NULL );
  if( bytesWritten != sizeof(mameoxLaunchData->m_totalMAMEGames) )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		CloseHandle( hFile );

			// Delete the file
		DeleteFile( driverListFile.c_str() );
		return FALSE;
	}


    // Write data for each driver
  for( DWORD i = 0; i < mameoxLaunchData->m_totalMAMEGames; ++i )
  {
      // Write the index
    WriteFile( hFile, &i, sizeof(i), &bytesWritten, NULL );
    if( bytesWritten != sizeof(i) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the length of the filename
    DWORD filenameLen = strlen(drivers[i]->name) + 1;
    WriteFile( hFile, &filenameLen, sizeof(filenameLen), &bytesWritten, NULL );
    if( bytesWritten != sizeof(filenameLen) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the filename
    WriteFile( hFile, drivers[i]->name, filenameLen, &bytesWritten, NULL );
    if( bytesWritten != filenameLen )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}
    
      // Write the length of the filename
    filenameLen = strlen(drivers[i]->description) + 1;
    WriteFile( hFile, &filenameLen, sizeof(filenameLen), &bytesWritten, NULL );
    if( bytesWritten != sizeof(filenameLen) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the description
    WriteFile( hFile, drivers[i]->description, filenameLen, &bytesWritten, NULL );
    if( bytesWritten != filenameLen )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}
    
      // Store whether or not this is a clone
      // All drivers are clones of _driver_0, whose clone_of is NULL,
      //  so check against that to decide whether this is a clone or not
    BOOL isClone = (drivers[i]->clone_of && drivers[i]->clone_of->clone_of);
    WriteFile( hFile, &isClone, sizeof(isClone), &bytesWritten, NULL );
    if( bytesWritten != sizeof(isClone) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}
  }

	CloseHandle( hFile );
  return TRUE;
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
	mbstowcs( wBuf, buf, strlen(buf) + 1 );

	g_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	g_font.End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );


	g_inputManager.WaitForNoKey( 0 );
	g_inputManager.WaitForAnyKey( 0 );
	g_inputManager.WaitForNoKey( 0 );

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  XLaunchNewImage( "D:\\default.xbe", &g_launchData );
}

//-------------------------------------------------------------
//  Helper_RunRom
//-------------------------------------------------------------
static BOOL Helper_RunRom( UINT32 romIndex )
{
  BOOL ret;
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

    // VC6 seems to be calling this with the full path so strstr just trims down the path
    // appropriately. NOTE: we probably don't need this to conditionally compile and could
    // just leave the first call but would like someone with VS.net to test first just in case :)
    //
    // Should be perfectly fine w/ the strstr
  if( !LoadDriverSectionByName( strstr(DriverName.c_str(),"src\\drivers\\") ) )
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
//	compareDriverNames
//-------------------------------------------------------------
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 )
{
	struct GameDriver *drv1 = *(struct GameDriver **)elem1;
	struct GameDriver *drv2 = *(struct GameDriver **)elem2;

		// Test the description field (game name)
	return strcmp( drv1->description, drv2->description );
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

}	// End Extern "C"

