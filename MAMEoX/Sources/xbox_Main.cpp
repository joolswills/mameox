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
#include "xbox_Network.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
}

//= D E F I N E S =====================================================

// VC6 requires the 2 paramater call to create. _VC6 is defined in the VC6 dsp files
#ifdef _VC6
#define CREATEFONT( fntObj, fntName )     fntObj.Create( pD3DDevice, fntName );
#else
#define CREATEFONT( fntObj, fntName )     fntObj.Create( fntName );
#endif


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

  // Sound processing override
BOOL g_soundEnabled = TRUE;

//= P R O T O T Y P E S ===============================================
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL Helper_RunRom( UINT32 romIndex );
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 );
static BOOL Helper_SaveDriverList( void );
static void DrawDriverProgressData( const char *fileName, DWORD index, DWORD total );

//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();

    // Get the launch data immediately, just in case Die() is called
  DWORD getLaunchInfoRet = XGetLaunchInfo( &g_launchDataType, &g_launchData );
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;

		// Mount the utility drive for storage of the ROM list cache file
	//XMountUtilityDrive( FALSE );

    // Register the loadable section names for lookup at runtime
  RegisterDriverSectionNames();
  RegisterCPUSectionNames();

		// Initialize the graphics subsystem
	g_graphicsManager.Create();
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

		// Create a general purpose font
  CREATEFONT( g_font, "Arial_16.xpr" );
//  CREATEFONT( g_fixedWidthFont, "CourierNew_12.xpr" );
//  CREATEFONT( g_smallFont, "ArialNarrow_12.xpr" );

  LoadOptions();

	  // Initialize the input subsystem
	g_inputManager.Create( 4, 0 );  // 4 controllers, no memory cards

	  // Intialize the various MAMEoX subsystems
  InitializeTiming();
	InitializeFileIO();
	InitializeD3DRenderer( g_graphicsManager, &g_font );
  
  SaveOptions();

CHECKRAM();   

    // Check the launch data to ensure that we've been started properly
  if( getLaunchInfoRet != ERROR_SUCCESS || g_launchDataType != LDT_TITLE )
  {
      // This XBE shouldn't be launched directly. Throw up a splash screen
      // saying so, then try to launch MAMEoxLauncher
    Die( pD3DDevice, "MAMEoX wasn't called from the launcher!\nPlease run default.xbe instead.\n" );
      // Die never returns
  }


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
      // Bring up the network
    if( !g_NetworkConfig.m_networkDisabled )
      InitializeNetwork();

      // Unload the XGRAPHICS section, as we won't be using it at all
    XFreeSection( "XGRPH" );

      // This shouldn't be necessary, IMAGEBLD should not load the non-data sections,
      //  as they're defined /NOPRELOAD
      // Update: I hacked it so that imagebld would not preload the sections
      //         however, the hack was less than ideal, so I quickly profiled
      //         this call, and it takes basically no time whatsoever, so I changed
      //         everything back to the way it was before. [EBA]
    UnloadDriverNonDataSections();    
    //UnloadCPUNonDataSections();  // Sections are unloaded in mame/src/cpuexec.c

      // This should only be necessary if IMAGEBLD doesn't load any sections
      // Update: Taking out this line is what caused the 0.66b release to
      //         crash in Release mode. Leave it be, or figure out why :) [EBA]
    LoadDriverDataSections();

      // Sort the game drivers and run the ROM
    qsort( drivers, mameoxLaunchData->m_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );

    Helper_RunRom( mameoxLaunchData->m_gameIndex );

      // NOTE: The driver list is invalid after Helper_RunRom, so don't do anything with it
      // until the MAMEoX.xbe utility has been rebooted.
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
	DWORD len;
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
  WriteFile( hFile, 
             "MAMEoX" VERSION_STRING, 
             6 + strlen(VERSION_STRING), 
             &bytesWritten, 
             NULL );
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
    DrawDriverProgressData( drivers[i]->name, i, mameoxLaunchData->m_totalMAMEGames );

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
    len = strlen(drivers[i]->name) + 1;
    WriteFile( hFile, &len, sizeof( len ), &bytesWritten, NULL );
    if( bytesWritten != sizeof( len ) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the filename
    WriteFile( hFile, drivers[i]->name, len, &bytesWritten, NULL );
    if( bytesWritten != len )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}
    
      // Write the length of the filename
    len = strlen(drivers[i]->description) + 1;
    WriteFile( hFile, &len, sizeof( len ), &bytesWritten, NULL );
    if( bytesWritten != sizeof( len ) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the description
    WriteFile( hFile, drivers[i]->description, len, &bytesWritten, NULL );
    if( bytesWritten != len )
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
    WriteFile( hFile, &isClone, sizeof( isClone ), &bytesWritten, NULL );
    if( bytesWritten != sizeof(isClone) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the length of the clonename
    len = strlen( drivers[i]->clone_of->name ) + 1;
    WriteFile( hFile, &len, sizeof( len ), &bytesWritten, NULL );
    if( bytesWritten != sizeof( len ) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

			// Write the clonename
    WriteFile( hFile, drivers[i]->clone_of->name, len, &bytesWritten, NULL );
    if( bytesWritten != len )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the length of the manufacturer
		len = strlen( drivers[i]->manufacturer ) + 1;
    WriteFile( hFile, &len, sizeof( len ), &bytesWritten, NULL );
    if( bytesWritten != sizeof( len ) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

			// Write the manufacturer
    WriteFile( hFile, drivers[i]->manufacturer, len, &bytesWritten, NULL );
    if( bytesWritten != len )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

      // Write the length of the year
		len = strlen( drivers[i]->year ) + 1;
    WriteFile( hFile, &len, sizeof( len ), &bytesWritten, NULL );
    if( bytesWritten != sizeof( len ) )
		{
			PRINTMSG( T_ERROR, "Write failed!" );
			CloseHandle( hFile );

				// Delete the file
			DeleteFile( driverListFile.c_str() );
			return FALSE;
		}

			// Write the year
    WriteFile( hFile, drivers[i]->year, len, &bytesWritten, NULL );
    if( bytesWritten != len )
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
  RequireController( 0 );
  CGamepad *gp = g_inputManager.GetGamepad( 0 );
	g_inputManager.WaitForNoButton( 0 );

  while( !gp->IsAnyButtonPressed() )
  {
    RequireController( 0 );
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
  }

  g_inputManager.WaitForNoButton( 0 );

    // Make sure MAMEoXLauncher acts as though it was launched from the dashboard
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  mameoxLaunchData->m_command = LAUNCH_RUN_AS_IF_REBOOTED;

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  XLaunchNewImage( "D:\\default.xbe", &g_launchData );
}


//-------------------------------------------------------------
//  DrawDriverProgressData
//-------------------------------------------------------------
static void DrawDriverProgressData( const char *fileName, DWORD index, DWORD total )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

		// Display the error to the user
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	g_font.Begin();
	
    g_font.DrawText( 320, 60, D3DCOLOR_XRGB( 255, 255, 255 ), L"A new version of MAME has been", XBFONT_CENTER_X );
    g_font.DrawText( 320, 80, D3DCOLOR_XRGB( 255, 255, 255 ), L"detected.", XBFONT_CENTER_X );
  	g_font.DrawText( 320, 120, D3DCOLOR_XRGB( 255, 255, 255 ), L"Dumping driver data", XBFONT_CENTER_X );

		  // Draw a progress bar
    UINT32 percentage = (UINT32)( (FLOAT)index * (25.0f / (FLOAT)total) + 0.5f ); 
    UINT32 i = 0;
	  WCHAR wBuf[256] = L"[";
    for( ; i < percentage; ++i )
      wcscat( wBuf, L"|" );
    for( ; i < 25; ++i )
      wcscat( wBuf, L" " );
    wcscat( wBuf, L"]" );

	  g_font.DrawText( 320, 140, D3DCOLOR_XRGB( 255, 125, 125 ), wBuf, XBFONT_CENTER_X );

		  // Draw the current filename
	  mbstowcs( wBuf, fileName, 256 );
	  g_font.DrawText( 320, 170, D3DCOLOR_XRGB( 60, 100, 255 ), wBuf, XBFONT_CENTER_X );

	g_font.End();

	pD3DDevice->Present( NULL, NULL, NULL, NULL );
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
  
  PRINTMSG( T_INFO, "*** Driver name: %s\n", drivers[romIndex]->source_file );

    // Unload all loaded XBE data sections, as we'll only be using the one
    // for the file we're loading
  UnloadDriverDataSections();
  //UnloadCPUDataSections();  // Sections are unloaded in mame/src/cpuexec.c

    // VC6 seems to be calling this with the full path so strstr just trims down the path
    // appropriately. NOTE: we probably don't need this to conditionally compile and could
    // just leave the first call but would like someone with VS.net to test first just in case :)
    //
    // Should be perfectly fine w/ the strstr
  if( !LoadDriverSectionByName( strstr(DriverName.c_str(),"src\\drivers\\") ) )
    PRINTMSG( T_ERROR, "Failed to load section for file %s!", DriverName.c_str() );

    // Override sound processing
  DWORD samplerate = options.samplerate;
  if( !g_soundEnabled )
    options.samplerate = 0;

	ret = run_game( romIndex );

    // Restore the old value, just in case somebody writes to the INI
  options.samplerate = samplerate;

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

//---------------------------------------------------------------------
//	osd_print_error
//---------------------------------------------------------------------
int fatalerror( const char *fmt, ... )
{
  wchar_t wBuf[1024];
  char buf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

  mbstowcs( wBuf, buf, 1023 );

  RequireController( 0 );
  CGamepad *gp = g_inputManager.GetGamepad( 0 );
	g_inputManager.WaitForNoButton( 0 );

	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

  while( !gp->IsAnyButtonPressed() )
  {
    RequireController( 0 );
		  // Display the error to the user
	  pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											  D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil

	  g_font.Begin();
  	
      g_font.DrawText( 320, 60, D3DCOLOR_RGBA( 255, 200, 200, 255 ), L"Fatal Error:", XBFONT_CENTER_X );
	    g_font.DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, XBFONT_CENTER_X );
	    g_font.DrawText( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255), L"Press any button to continue.", XBFONT_CENTER_X );
	  g_font.End();

	  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

  g_inputManager.WaitForNoButton( 0 );

    // Make sure MAMEoXLauncher acts as though it was launched from the dashboard
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  mameoxLaunchData->m_command = LAUNCH_RUN_AS_IF_REBOOTED;

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  XLaunchNewImage( "D:\\default.xbe", &g_launchData );

    // Execution should never get here
  return 0;
}

}	// End Extern "C"

