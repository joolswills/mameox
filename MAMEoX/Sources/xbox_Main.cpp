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
#define CREATEFONT( fntObj, fntName )     fntObj.Create( pD3DDevice, fntName )
#else
#define CREATEFONT( fntObj, fntName )     fntObj.Create( fntName )
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
static BOOL Helper_SaveDriverInfoFile( void );
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

		// Initialize the graphics subsystem
	g_graphicsManager.Create();
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

		// Load the font, reboot if it's missing (without a font,
    //  no error message is possible (as of today)
  if( FAILED( CREATEFONT( g_font, "HawaiianPunk_16.xpr" ) ) )
  {
    PRINTMSG_TO_LOG( T_ERROR, "Failed loading font Media/HawaiianPunk_16.xpr!" );
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
  }

  LoadOptions();

	  // Initialize the input subsystem
	g_inputManager.Create( 4, 0 );  // 4 controllers, no memory cards

	  // Intialize the various MAMEoX subsystems
  InitializeTiming();
	InitializeFileIO();
	InitializeD3DRenderer( g_graphicsManager, &g_font );
  
  SaveOptions();

  CHECKRAM();

    // Register the loadable section names for lookup at runtime
  InitDriverSectionizer();
  InitCPUSectionizer();

  DEBUGGERCHECKRAM()

    // Check the launch data to ensure that we've been started properly
  if( getLaunchInfoRet != ERROR_SUCCESS || g_launchDataType != LDT_TITLE )
  {
      // This XBE shouldn't be launched directly. Throw up a splash screen
      // saying so, then try to launch MAMEoxLauncher
    memset( mameoxLaunchData, 0, sizeof(*mameoxLaunchData) );
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
    Helper_SaveDriverInfoFile();
  }
  else
  {
      // Bring up the network
    if( !g_NetworkConfig.m_networkDisabled )
      InitializeNetwork();

      // Unload the XGRAPHICS section, as we won't be using it at all
    XFreeSection( "XGRPH" );

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

    // VC6 seems to be calling this with the full path so strstr just trims down the path
    // appropriately. NOTE: we probably don't need this to conditionally compile and could
    // just leave the first call but would like someone with VS.net to test first just in case :)
    //
    // Should be perfectly fine w/ the strstr
  if( !LoadDriverSectionByName( strstr(DriverName.c_str(),"src\\drivers\\") ) )
    PRINTMSG( T_ERROR, "Failed to load section for file %s!", DriverName.c_str() );

    // Unload all the other XBE data sections, as we'll only be using the one
    // for the file we're loading
    // Note that we do this _after_ the LoadDriverSectionByName.
    // This increments the refcount on the driver causing the system 
    // not to release anything allocated by the segment back to the heap
    // (MAME shouldn't allocate anything at this point, but it's good to be safe)
  UnloadDriverSections();
  //UnloadCPUSections();  // Sections are unloaded in mame/src/cpuexec.c

    // Free up unneeded sectionizer memory
  TerminateDriverSectionizer();
//  TerminateCPUSectionizer();

    // Override sound processing
  DWORD samplerate = options.samplerate;
  if( !g_soundEnabled )
    options.samplerate = 0;

  #ifdef _DEBUG
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus(  &memStatus );
    PRINTMSG( T_INFO, 
              "Memory: %lu/%lu",
              memStatus.dwAvailPhys, 
              memStatus.dwTotalPhys );
  }
  #endif

	ret = run_game( romIndex );

    // Restore the old value, just in case somebody writes to the INI
  options.samplerate = samplerate;

  #ifdef _PROFILER
  DmStopProfiling();
  #endif

  return (!ret);  // run_game works on inverted logic
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

// Note: The "STARTUP" segment is unloaded in xbox_JoystickMouse.c
#pragma code_seg("STARTUP")
//-------------------------------------------------------------
//  Helper_SaveDriverInfoFile
//-------------------------------------------------------------
static BOOL Helper_SaveDriverInfoFile( void )
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
  if( !WriteFile( hFile, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1, &bytesWritten, NULL ) ||
      bytesWritten != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG( T_ERROR, "Write failed!" );
		CloseHandle( hFile );

			// Delete the file
		DeleteFile( driverListFile.c_str() );
		return FALSE;
	}


  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;


    // Calculate a signature for the list, so we can validate it when reading
    //  to eliminate corrupt data
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    Die( g_graphicsManager.GetD3DDevice(), "Could not calculate driver list signature!" );
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    Die( g_graphicsManager.GetD3DDevice(), "Could not allocate memory for driver list signature!" );
  }

    // Write in a blank signature
  if( !WriteFile( hFile, sigData, sigSize, &bytesWritten, NULL ) || bytesWritten != sigSize )
  {
    free( sigData );
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing blank signature!" );
    Die( g_graphicsManager.GetD3DDevice(), "Failed writing blank signature!" );
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
      DeleteFile( driverListFile.c_str() ); \
      return FALSE; \
    }

    // Store the number of drivers
  WRITEDATA( &mameoxLaunchData->m_totalMAMEGames, sizeof(mameoxLaunchData->m_totalMAMEGames) )

    // Write data for each driver
  for( DWORD i = 0; i < mameoxLaunchData->m_totalMAMEGames; ++i )
  {
    DrawDriverProgressData( drivers[i]->name, i, mameoxLaunchData->m_totalMAMEGames );

      // Write the index
    WRITEDATA( &i, sizeof(i) );

      // Write the length of the filename
    len = strlen(drivers[i]->name) + 1;
    WRITEDATA( &len, sizeof(len) );

      // Write the filename
    WRITEDATA( drivers[i]->name, len );
    
      // Write the length of the filename
    len = strlen(drivers[i]->description) + 1;
    WRITEDATA( &len, sizeof(len) );

      // Write the description
    WRITEDATA( drivers[i]->description, len );
    
      // Store whether or not this is a clone
      // All drivers are clones of _driver_0, whose clone_of is NULL,
      //  so check against that to decide whether this is a clone or not
    BOOL isClone = (drivers[i]->clone_of && drivers[i]->clone_of->clone_of);
    WRITEDATA( &isClone, sizeof( isClone ) );

      // Write the length of the clonename
    if( drivers[i]->clone_of )
    {
      len = strlen( drivers[i]->clone_of->name ) + 1;
      WRITEDATA( &len, sizeof(len) );
      WRITEDATA( drivers[i]->clone_of->name, len );
    }
    else
    {
      len = 1;
      WRITEDATA( &len, sizeof(len) );
      WRITEDATA( "", len );
    }

      // Write the length of the manufacturer
		len = strlen( drivers[i]->manufacturer ) + 1;
    WRITEDATA( &len, sizeof(len) );

			// Write the manufacturer
    WRITEDATA( drivers[i]->manufacturer, len );

      // Write the length of the year
		len = strlen( drivers[i]->year ) + 1;
    WRITEDATA( &len, sizeof(len) );

			// Write the year
    WRITEDATA( drivers[i]->year, len );
	}

    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( sigData );
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    Die( g_graphicsManager.GetD3DDevice(), "Failed getting signature!" );
  }

    // Write in the real signature
  SetFilePointer( hFile, sizeof(DRIVERLIST_FILESTAMP) - 1, NULL, FILE_BEGIN );
  if( !WriteFile( hFile, sigData, sigSize, &bytesWritten, NULL ) || bytesWritten != sigSize )
  {
    free( sigData );
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed writing signature!" );
    Die( g_graphicsManager.GetD3DDevice(), "Failed writing signature!" );
  }

  free( sigData );
	CloseHandle( hFile );
  return TRUE;
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
      // Temporary: The Hawaiian Punk font doesn't have a | character, so use 1 instead
    UINT32 percentage = (UINT32)( (FLOAT)index * (25.0f / (FLOAT)total) + 0.5f ); 
    UINT32 i = 0;
	  WCHAR wBuf[256] = L"[";
    for( ; i < percentage; ++i )
      wcscat( wBuf, L"1" );
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
//	compareDriverNames
//-------------------------------------------------------------
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 )
{
	struct GameDriver *drv1 = *(struct GameDriver **)elem1;
	struct GameDriver *drv2 = *(struct GameDriver **)elem2;

		// Test the description field (game name)
	return strcmp( drv1->description, drv2->description );
}
#pragma code_seg()


extern "C" {
//-------------------------------------------------------------
//	osd_init
//-------------------------------------------------------------
int osd_init( void )
{
  //InitializeJoystickMouse();    // Done in osd_customize_inputport_defaults(), as MAME does input system init _after_ osd_init

        /* 
          [EBA] Have to load all driver sections to allow parsing of
          the drivers array. The problem is that the ROM may already be
          loaded at this point, so it might fail to load all driver sections,
          causing a crash still... Will have to look into a better solution
          sometime in the future.

          Idea: Parse the driver array for clones in xbox_Main.cpp. Store this
          in a data section that is immediately unloaded, load it here, and parse
          as though it were the real drivers[] array.
        */ 
#/*
      InitDriverSectionizer();
      LoadDriverSections();

			foundworking = 0;
			i = 0;
			while (drivers[i])
			{
				if (drivers[i] == maindrv || drivers[i]->clone_of == maindrv)
				{
					if ((drivers[i]->flags & (GAME_NOT_WORKING | GAME_UNEMULATED_PROTECTION)) == 0)
					{
						if (foundworking == 0)
						{
							strcat(buf,"\n\n");
							strcat(buf, ui_getstring (UI_workingclones));
							strcat(buf,"\n\n");
						}
						foundworking = 1;

						sprintf(&buf[strlen(buf)],"%s\n",drivers[i]->name);
					}
				}
				i++;
			}

        // Unload the unnecessary sections to save memory if possible
      LoadDriverSectionByName( strstr( maindrv->source_file, "src\\drivers\\" ) );
      UnloadDriverSections();
      TerminateDriverSectionizer();
*/

	return 0;
}

//-------------------------------------------------------------
//	osd_exit
//-------------------------------------------------------------
void osd_exit( void )
{
//  TerminateJoystickMouse(); // Unnecessary as we'll just exit anyway
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

