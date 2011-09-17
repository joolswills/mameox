/**
	* \file			xbox_Main.cpp
	* \brief		MAMEoX entry point
	*
	*/


//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include <stdarg.h>
#include <stdio.h>
#include <crtdbg.h>

#ifdef _PROFILER
  #include <xbdm.h>
#endif

#include "XBESectionUtil.h"

#include "MAMEoX.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "DebugLogger.h"
#include "FontSet.h"

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
#include "virtualmemory.h"
}


//= D E F I N E S =====================================================


//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
  DWORD         diffuse;  // The diffuse color of the vertex
//  FLOAT         tu, tv;   // The texture coordinates
};

//= G L O B A L = V A R S =============================================
  // Defined in MAMEoXUtil.cpp
extern CInputManager			g_inputManager;
extern CGraphicsManager	  g_graphicsManager;
extern CFontSet           g_fontSet;

  // XBE Launch data
DWORD                     g_launchDataType;
LAUNCH_DATA               g_launchData;

static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

  // Sound processing override
BOOL g_soundEnabled = TRUE;


  // Dummy screensaverTimeout value (only used in the Launcher)
UINT32 g_screensaverTimeout;

//= P R O T O T Y P E S ===============================================
static void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL Helper_RunRom( UINT32 romIndex );
static BOOL __cdecl compareDriverNames( const void *elem1, const void *elem2 );
static BOOL Helper_SaveDriverInfoFile( void );
static void DrawDriverProgressData( const char *fileName, DWORD index, DWORD total );
static BOOL Helper_IsBIOS( const GameDriver *drv );



//= D E F I N E S  =====================================================
//#define ENABLE_LOGERROR     // Enable the logerror function (this can spit out a _lot_ of data)

extern "C" {
void InitVirtualMem( DWORD romIndex );
void auto_free(void);

}

//= F U N C T I O N S =================================================


int ExceptionFilter(LPEXCEPTION_POINTERS e)
{
	int slotnumber ;
	DWORD pagenumber ;
	VIRTMEM_LOC *slot ;
	int numfree ;
	DWORD offset ;
	DWORD newAddress, oldAddress ;
	DWORD numwritten, numread ;
	DWORD addr ;


    // we are only interested in access violations (memory faults)
    if (e->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
        return EXCEPTION_CONTINUE_SEARCH;

    // the exception information includes the type of access (read
    // or write), and the address of the fault
    //bool writing = (e->ExceptionRecord->ExceptionInformation[0] != 0);
    addr = (DWORD)e->ExceptionRecord->ExceptionInformation[1];


	if ( ( addr < g_virtualmem_locs[0].baseAddress  ) || ( addr >= ( g_virtualmem_locs[0].baseAddress  + ( VIRTUAL_MEMORY_TOTAL_SIZE) ) ) )
	{
        return EXCEPTION_CONTINUE_SEARCH;
	}

	offset = addr - g_virtualmem_locs[0].baseAddress   ;

	slotnumber = offset >> ( VIRTUAL_MEMORY_PAGE_PER_CHUNK_BITS + VIRTUAL_MEMORY_PAGE_BITS ) ;

	slot = &(g_virtualmem_locs[slotnumber]) ;

#ifdef _DEBUG
	slot->hitnum++ ;
#endif

	pagenumber = ( offset >> VIRTUAL_MEMORY_PAGE_BITS ) & VIRTUAL_MEMORY_PAGE_MASK ;

	offset = pagenumber << VIRTUAL_MEMORY_PAGE_BITS ;

	newAddress = slot->baseAddress + offset ;

	oldAddress = slot->allocatedPages[*(slot->head)] ;

	SetFilePointer( g_vmemfile, slot->fileoffset + ( oldAddress - slot->baseAddress ), 0, FILE_BEGIN ) ;
	WriteFile( g_vmemfile, (unsigned char*)(oldAddress), VIRTUAL_MEMORY_PAGESIZE, &numwritten, NULL ) ;

	VirtualFree( (LPVOID)oldAddress, VIRTUAL_MEMORY_PAGESIZE, MEM_DECOMMIT ) ;

	VirtualAlloc( (LPVOID)newAddress, VIRTUAL_MEMORY_PAGESIZE, MEM_COMMIT, PAGE_READWRITE ) ;

	SetFilePointer( g_vmemfile, slot->fileoffset + offset, 0, FILE_BEGIN ) ;
	ReadFile( g_vmemfile, (void*)newAddress, VIRTUAL_MEMORY_PAGESIZE, &numread, NULL ) ;

	slot->allocatedPages[*(slot->head)] = newAddress ;
	*(slot->head) = ((*(slot->head))+1)%( slot->numpages ) ;

    return EXCEPTION_CONTINUE_EXECUTION;
}



//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread

	//Sleep(15000) ;


	DebugLoggerInit();

  Enable128MegCaching();

    // Get the launch data immediately, just in case Die() is called
  DWORD getLaunchInfoRet = XGetLaunchInfo( &g_launchDataType, &g_launchData );
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;

  LoadOptions();      // Must be done before graphics mgr (for VSYNC) and before input mgr (lightgun calib data)
	InitializeFileIO(); // Initialize fileIO (must be done after LoadOptions!)


	//-----------------------------
  InitDriverSectionizer();
  InitCPUSectionizer();




  if( mameoxLaunchData->m_command == LAUNCH_CREATE_MAME_GAME_LIST )
  {
		mameoxLaunchData->m_gameIndex = 0;

  }
  else
  {

    qsort( drivers, mameoxLaunchData->m_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );

	const char *DriverName = drivers[mameoxLaunchData->m_gameIndex]->source_file;
	  
	PRINTMSG(( T_INFO, "*** Driver name: %s\n", drivers[mameoxLaunchData->m_gameIndex]->source_file ));

		// VC6 seems to be calling this with the full path so strstr just trims down the path
		// appropriately. NOTE: we probably don't need this to conditionally compile and could
		// just leave the first call but would like someone with VS.net to test first just in case :)
		//
		// [EBA] Should be perfectly fine w/ the strstr
		const char *driverName = strstr(DriverName,"drivers\\");
		if( !driverName || !driverName[8] )
		{
			PRINTMSG(( T_ERROR, "Invalid driver name at index %lu!", mameoxLaunchData->m_gameIndex ));
			return ;
			//fatalerror( "An unrecoverable error has occurred!\r\nInvalid driver name at index %lu!", romIndex );
		}

	if( !LoadDriverSectionByName( &driverName[8] ) )
		{
		PRINTMSG(( T_ERROR, "Failed to load section for file %s!", DriverName ));
		return ;
			//fatalerror( "An unrecoverable error has occurred!\r\nFailed to load XBE section for file %s!", &driverName[8] );
		}

		// Unload all the other XBE data sections, as we'll only be using the one
		// for the file we're loading
		// Note that we do this _after_ the LoadDriverSectionByName.
		// This increments the refcount on the driver causing the system 
		// not to release anything allocated by the segment back to the heap
		// (MAME shouldn't allocate anything at this point, but it's good to be safe)
	UnloadDriverSections();
	//UnloadCPUSections();          // CPU sections are unloaded in mame/src/cpuexec.c

		// Free up unneeded sectionizer memory
	TerminateDriverSectionizer();

  }










//------------------------------------ 


		// Initialize the graphics subsystem
  g_graphicsManager.Create( g_rendererOptions.m_vsync, ( mameoxLaunchData->m_command == LAUNCH_CREATE_MAME_GAME_LIST ) );
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

    // Create the fonts or die
  if( !g_fontSet.Create() )
  {
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    DWORD retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
    Die( pD3DDevice, "Failed to launch the dashboard! 0x%X", retVal );
  }


	  // Initialize the input subsystem
	g_inputManager.Create( 4, 0, TRUE );  // 4 controllers, no memory cards, allow keyboard

	  // Intialize the various MAMEoX subsystems
  InitializeTiming();
	InitializeD3DRenderer( g_graphicsManager, &g_fontSet.DefaultFont() );
  
  CHECKRAM();

    // Register the loadable section names for lookup at runtime
  //InitDriverSectionizer();
  //InitCPUSectionizer();

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
    else
    {
        // Unload all the samba and networking stuff
//      XFreeSection( "NETWORK" );
      XFreeSection( "MOXNET" );
      XFreeSection( "SAMBA" );
    }

      // Unload the XGRAPHICS section, as we won't be using it at all
    XFreeSection( "XGRPH" );

      // Sort the game drivers and run the ROM
    //qsort( drivers, mameoxLaunchData->m_totalMAMEGames, sizeof(drivers[0]), compareDriverNames );

	__try {
		InitVirtualMem( mameoxLaunchData->m_gameIndex  );
		Helper_RunRom( mameoxLaunchData->m_gameIndex );
    } __except(ExceptionFilter(GetExceptionInformation())) {
        //handle all other bad exceptions here
    };

      // NOTE: The driver list is invalid after Helper_RunRom, so don't do anything with it
      // until the MAMEoX.xbe utility has been rebooted.
  }

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  DWORD retVal = XLaunchNewImage( "D:\\default.xbe", &g_launchData );
  Die( pD3DDevice, "Failed to launch D:\\default.xbe! 0x%X", retVal );
}


//-------------------------------------------------------------
//  Helper_IsBIOS
//-------------------------------------------------------------
static BOOL Helper_IsBIOS( const GameDriver *drv )
{
  if( !drv )
    return FALSE;

    // The list of bios drivers from www.mame.dk
  if( !stricmp( drv->name, "decocass" ) ||
      !stricmp( drv->name, "cvs" ) ||
      !stricmp( drv->name, "neogeo" ) ||
      !stricmp( drv->name, "pgm" ) ||
      !stricmp( drv->name, "playch10" ) ||
      !stricmp( drv->name, "stvbios" ) ||
      !stricmp( drv->name, "skns" ) ||
      !stricmp( drv->name, "konamigx" ) )
    return TRUE;

  return FALSE;
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

#if 0

  std::string DriverName = drivers[romIndex]->source_file;
  
  PRINTMSG(( T_INFO, "*** Driver name: %s\n", drivers[romIndex]->source_file ));

    // VC6 seems to be calling this with the full path so strstr just trims down the path
    // appropriately. NOTE: we probably don't need this to conditionally compile and could
    // just leave the first call but would like someone with VS.net to test first just in case :)
    //
    // [EBA] Should be perfectly fine w/ the strstr
	const char *driverName = strstr(DriverName.c_str(),"drivers\\");
	if( !driverName || !driverName[8] )
	{
		PRINTMSG(( T_ERROR, "Invalid driver name at index %lu!", romIndex ));
		fatalerror( "An unrecoverable error has occurred!\r\nInvalid driver name at index %lu!", romIndex );
	}

  if( !LoadDriverSectionByName( &driverName[8] ) )
	{
    PRINTMSG(( T_ERROR, "Failed to load section for file %s!", DriverName.c_str() ));
		fatalerror( "An unrecoverable error has occurred!\r\nFailed to load XBE section for file %s!", &driverName[8] );
	}

    // Unload all the other XBE data sections, as we'll only be using the one
    // for the file we're loading
    // Note that we do this _after_ the LoadDriverSectionByName.
    // This increments the refcount on the driver causing the system 
    // not to release anything allocated by the segment back to the heap
    // (MAME shouldn't allocate anything at this point, but it's good to be safe)
  UnloadDriverSections();
  //UnloadCPUSections();          // CPU sections are unloaded in mame/src/cpuexec.c

    // Free up unneeded sectionizer memory
  TerminateDriverSectionizer();
//  TerminateCPUSectionizer();    // CPU sectionizer is unloaded in mame/src/cpuexec.c

#endif

    // Override sound processing
  DWORD samplerate = options.samplerate;
  if( !g_soundEnabled )
    options.samplerate = 0;


    // Unload the "STARTUP" section
  XFreeSection( "STARTUP" );

//  #ifdef _DEBUG
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus(  &memStatus );
    PRINTMSG(( T_INFO, 
              "Memory: %lu/%lu",
              memStatus.dwAvailPhys, 
              memStatus.dwTotalPhys ));
  }
//  #endif

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
  WCHAR wBuf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

	PRINTMSG(( T_ERROR, "Die: %s", buf ));
	g_inputManager.WaitForNoButton();

  while( !(g_inputManager.IsAnyButtonPressed() || g_inputManager.IsAnyKeyPressed()) )
  {
    g_inputManager.PollDevices();

		  // Display the error to the user
	  pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											  D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil

	  g_fontSet.DefaultFont().Begin();    	
	    mbstowcs( wBuf, buf, strlen(buf) + 1 );
	    g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	    g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	  g_fontSet.DefaultFont().End();
	  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

  g_inputManager.WaitForNoButton();

    // Make sure MAMEoXLauncher acts as though it was launched from the dashboard
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  mameoxLaunchData->m_command = LAUNCH_RUN_AS_IF_REBOOTED;

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  DWORD retVal = XLaunchNewImage( "D:\\default.xbe", &g_launchData );

  while( !(g_inputManager.IsAnyButtonPressed() || g_inputManager.IsAnyKeyPressed()) )
  {
    g_inputManager.PollDevices();

		  // Display the error to the user
	  pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											  D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil
    g_fontSet.DefaultFont().Begin();
      swprintf( wBuf, L"Failed to launch d:\\default.xbe! 0x%X", retVal );
	    g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	    g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );
	  g_fontSet.DefaultFont().End();
	  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }
  g_inputManager.WaitForNoButton();

    // Attempt to get back to the dashboard
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );

  g_fontSet.DefaultFont().Begin();
    swprintf( wBuf, L"Failed to launch the dashboard! 0x%X", retVal );
  g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
  g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_RGBA( 255, 255, 255, 255), L"You need to power off manually!", XBFONT_CENTER_X );
	g_fontSet.DefaultFont().End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

// Note: The "STARTUP" segment is unloaded in xbox_JoystickMouse.c
#pragma code_seg("STARTUP")
//-------------------------------------------------------------
//  Helper_SaveDriverInfoFile
//-------------------------------------------------------------
static BOOL Helper_SaveDriverInfoFile( void )
{
	DWORD len;
	CStdString driverListFile = DEFAULT_MAMEOXSYSTEMPATH "\\" DRIVERLISTFILENAME;
	PRINTMSG(( T_INFO, "Store driver list: %s", driverListFile.c_str() ));

  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, DRIVERLISTFILENAME, "w" );
	if( !file )
	{
		PRINTMSG(( T_ERROR, "Could not create file %s!", driverListFile.c_str() ));
		return FALSE;
	}

  if( osd_fwrite( file, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) != sizeof(DRIVERLIST_FILESTAMP) - 1 )
	{
		PRINTMSG(( T_ERROR, "Write failed!" ));
		osd_fclose( file );

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
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    Die( g_graphicsManager.GetD3DDevice(), "Could not calculate driver list signature!" );
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    Die( g_graphicsManager.GetD3DDevice(), "Could not allocate memory for driver list signature!" );
  }

    // Write in a blank signature
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing blank signature!" ));
    Die( g_graphicsManager.GetD3DDevice(), "Failed writing blank signature!" );
  }



    // Define a macro to write a block of data and calculate the data signature
  #define WRITEDATA( _data__, _dataSize__ ) \
    if( XCalculateSignatureUpdate( sigHandle, (const BYTE *)(_data__), (_dataSize__) ) != ERROR_SUCCESS || \
        osd_fwrite( file, (_data__), (_dataSize__) ) != (_dataSize__) ) \
    { \
      PRINTMSG(( T_ERROR, "Write failed!" )); \
      free( sigData ); \
      osd_fclose( file ); \
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
    BOOL isClone = (drivers[i]->clone_of && drivers[i]->clone_of->clone_of &&
                    !Helper_IsBIOS( drivers[i]->clone_of ) );
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

      // Write the number of players
    UINT32 numPlayers = 0;

	if ( drivers[i]->construct_ipt )
	{
		const InputPort *inport = input_port_allocate(drivers[i]->construct_ipt);

		while (inport->type != IPT_END)
		{

			switch (inport->player )
			{
				case 0:
					if( numPlayers < 1 )
					numPlayers = 1;
					break;

				case 1:
					if( numPlayers < 2 )
					numPlayers = 2;
					break;

				case 2:
					if( numPlayers < 3 )
					numPlayers = 3;
					break;

				case 3:
					if( numPlayers < 4 )
					numPlayers = 4;
					break;
			}
			inport++ ;
		}
	}
  
	auto_free() ;

    WRITEDATA( &numPlayers, sizeof(numPlayers) );

    UINT32 orientation = (drivers[i]->flags & ORIENTATION_MASK);
    WRITEDATA( &orientation, sizeof(orientation) );
	}

    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    Die( g_graphicsManager.GetD3DDevice(), "Failed getting signature!" );
  }

    // Write in the real signature
  osd_fseek( file, sizeof(DRIVERLIST_FILESTAMP) - 1, SEEK_SET );
  if( osd_fwrite( file, sigData, sigSize ) != sigSize )
  {
    free( sigData );
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed writing signature!" ));
    Die( g_graphicsManager.GetD3DDevice(), "Failed writing signature!" );
  }

  free( sigData );
	osd_fclose( file );
  return TRUE;
}

//-------------------------------------------------------------
//  DrawDriverProgressData
//-------------------------------------------------------------
static void DrawDriverProgressData( const char *fileName, DWORD index, DWORD total )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

	if ( pD3DDevice == NULL )
		return ;

		// Display the error to the user
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	g_fontSet.DefaultFont().Begin();
	
    g_fontSet.DefaultFont().DrawText( 320, 125, D3DCOLOR_XRGB( 255, 255, 255 ), L"A new version of MAME has been", XBFONT_CENTER_X );
    g_fontSet.DefaultFont().DrawText( 320, 150, D3DCOLOR_XRGB( 255, 255, 255 ), L"detected.", XBFONT_CENTER_X );
  	g_fontSet.DefaultFont().DrawText( 320, 200, D3DCOLOR_XRGB( 255, 255, 255 ), L"Dumping driver data", XBFONT_CENTER_X );

		  // Draw the current filename
	  WCHAR wBuf[256];
	  mbstowcs( wBuf, fileName, 256 );
	g_fontSet.DefaultFont().DrawText( 320, 258, D3DCOLOR_XRGB( 0, 0, 0 ), wBuf, XBFONT_CENTER_X );

	g_fontSet.DefaultFont().End();

  #define PROGRESSBAR_CAP_COLOR     D3DCOLOR_XRGB( 255, 255, 255 )
  #define PROGRESSBAR_BAR_COLOR     D3DCOLOR_XRGB( 110, 120, 200 )
  #define PROGRESSBAR_BCK_COLOR     D3DCOLOR_RGBA( 190, 190, 190, 100 )

  #define PROGRESSBAR_WIDTH         410
  #define PROGRESSBAR_HEIGHT        20

  RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                      240 - (PROGRESSBAR_HEIGHT >> 1), 
                      320 + (PROGRESSBAR_WIDTH>>1), 
                      240 + (PROGRESSBAR_HEIGHT >> 1), 
                      index, 
                      total, 
                      PROGRESSBAR_BAR_COLOR, 
											PROGRESSBAR_CAP_COLOR,
											PROGRESSBAR_BCK_COLOR );


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
	return strcasecmp( drv1->description, drv2->description );
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
//	fatalerror
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

	g_inputManager.WaitForNoButton();

	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

  while( !(g_inputManager.IsAnyButtonPressed() || g_inputManager.IsAnyKeyPressed()) )
  {
    g_inputManager.PollDevices();

		  // Display the error to the user
	  pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											  D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil

	  g_fontSet.DefaultFont().Begin();
  	
      g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Fatal Error:", XBFONT_CENTER_X );
	  g_fontSet.DefaultFont().DrawText( 320, 75, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, XBFONT_CENTER_X );
	  g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_RGBA( 255, 255, 255, 255), L"Press any button to continue.", XBFONT_CENTER_X );
	  g_fontSet.DefaultFont().End();

	  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

  g_inputManager.WaitForNoButton();

    // Make sure MAMEoXLauncher acts as though it was launched from the dashboard
//  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
//  mameoxLaunchData->m_command = LAUNCH_RUN_AS_IF_REBOOTED;

    // Relaunch MAMEoXLauncher
  ShowLoadingScreen( pD3DDevice );
  DWORD retVal = XLaunchNewImage( "D:\\default.xbe", &g_launchData );
  Die( pD3DDevice, "Failed to launch D:\\default.xbe! 0x%X", retVal );

    // Execution should never get here
  return 0;
}


//-------------------------------------------------------------
//	ShowLoadingScreen
//-------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{

		// Clear the backbuffer
  pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                      D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

  g_fontSet.DefaultFont().Begin();
  g_fontSet.DefaultFont().DrawText( 320, 230, D3DCOLOR_RGBA( 255, 255, 255, 255),   L"Loading. Please wait...", XBFONT_CENTER_X );
  g_fontSet.DefaultFont().End();

  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  pD3DDevice->PersistDisplay();
}

}	// End Extern "C"

