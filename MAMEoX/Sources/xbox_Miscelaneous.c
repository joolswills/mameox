/**
	* \file			xbox_Miscelaneous.c
	* \brief		Implementation of the "miscelaneous" section of
	*           osdepend.h
	*/

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

#include "MAMEoX.h"
#include "DebugLogger.h"

#include "mame.h"
#include "cpuexec.h"
#include "palette.h"
#include "common.h"


#include <stdio.h>
#include <stdarg.h>


//= D E F I N E S  =====================================================
//#define ENABLE_LOGERROR     // Enable the logerror function (this can spit out a _lot_ of data)

//= P R O T O T Y P E S ===============================================
BOOL osd_vmm_unloadLRUpage( void );       // Defined in MAMEoXUtil.cpp
int fatalerror( const char *fmt, ... );   // Defined in xbox_Main.cpp


//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//  osd_malloc_debug
//---------------------------------------------------------------------
void *osd_malloc_debug( size_t size, const char **filename, unsigned int line, const char **function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = malloc( size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = malloc( size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Malloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_debug
//---------------------------------------------------------------------
void *osd_calloc_debug( size_t num, size_t size, const char **filename, unsigned int line, const char **function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = calloc( num, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = calloc( num, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Calloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_debug
//---------------------------------------------------------------------
void *osd_realloc_debug( void *memblock, size_t size, const char **filename, unsigned int line, const char **function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = realloc( memblock, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Realloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_malloc_retail
//---------------------------------------------------------------------
void *osd_malloc_retail( size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = malloc( size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = malloc( size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Malloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_retail
//---------------------------------------------------------------------
void *osd_calloc_retail( size_t num, size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = calloc( num, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = calloc( num, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Calloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_retail
//---------------------------------------------------------------------
void *osd_realloc_retail( void *memblock, size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = realloc( memblock, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Realloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}


//---------------------------------------------------------------------
//	osd_display_loading_rom_message
//---------------------------------------------------------------------
int osd_display_loading_rom_message( const char *name, struct rom_load_data *romdata )
{
	/* called while loading ROMs. It is called a last time with name == 0 to signal */
	/* that the ROM loading process is finished. */
	/* return non-zero to abort loading */

	WCHAR title[128];
  WCHAR memory[64];
  MEMORYSTATUS memStatus;

	PRINTMSG( T_TRACE, "osd_display_loading_rom_message" );

	if( name )
	{
		wcscpy( title, L"Loading \"" );
		mbstowcs( &title[wcslen(title)], name, 32 );
		swprintf( &title[wcslen(title)], L"\" (%d/ %d)", romdata->romsloaded, romdata->romstotal );
	}
	else
		wcscpy( title, L"Loading complete!" );


  GlobalMemoryStatus( &memStatus );
  swprintf( memory, L"Available: %lu / Total: %lu", memStatus.dwAvailPhys, memStatus.dwTotalPhys );

	BeginFontRender( TRUE, FONTTYPE_DEFAULT );
	  FontRender( 70, 195, D3DCOLOR_XRGB( 230, 230, 230 ), title, 0 );
    FontRender( 320, 140, D3DCOLOR_XRGB( 60, 105, 225 ), L"MAMEoX version " LVERSION_STRING, 2 );
    FontRender( 320, 340, D3DCOLOR_XRGB( 60, 105, 225 ), L"Memory Status", 2 );
    FontRender( 320, 360, D3DCOLOR_XRGB( 60, 105, 225 ), memory, 2 );
  EndFontRender( FALSE );


  #define PROGRESSBAR_CAP_COLOR     D3DCOLOR_XRGB( 101, 197, 247 )
  #define PROGRESSBAR_BAR_COLOR     D3DCOLOR_XRGB( 16, 80, 124 )

  #define PROGRESSBAR_WIDTH         500
  #define PROGRESSBAR_HEIGHT        20

  if( name )
    RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                        240 - (PROGRESSBAR_HEIGHT >> 1), 
                        320 + (PROGRESSBAR_WIDTH>>1), 
                        240 + (PROGRESSBAR_HEIGHT >> 1), 
                        romdata->romsloaded, 
                        romdata->romstotal, 
                        PROGRESSBAR_BAR_COLOR, 
                        PROGRESSBAR_CAP_COLOR );
  else
    RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                        240 - (PROGRESSBAR_HEIGHT >> 1), 
                        320 + (PROGRESSBAR_WIDTH>>1), 
                        240 + (PROGRESSBAR_HEIGHT >> 1), 
                        100, 
                        100, 
                        PROGRESSBAR_BAR_COLOR, 
                        PROGRESSBAR_CAP_COLOR );

  PresentFrame();

	return 0;
}

//---------------------------------------------------------------------
//	osd_pause
//---------------------------------------------------------------------
void osd_pause( int paused )
{
}

//---------------------------------------------------------------------
//	logerror
//---------------------------------------------------------------------
void __cdecl logerror( const char *fmt, ... )
{
#ifdef ENABLE_LOGERROR
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

	PRINTMSG( T_ERROR, buf );
#endif
}

//---------------------------------------------------------------------
//	osd_print_error
//---------------------------------------------------------------------
void osd_print_error( const char *fmt, ... )
{
  wchar_t wBuf[1024];
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

  mbstowcs( wBuf, buf, 1023 );

    // Display the error to the user
	BeginFontRender( TRUE, FONTTYPE_DEFAULT );
    FontRender( 320, 60, D3DCOLOR_RGBA( 255, 200, 200, 255 ), L"Error:", 2 /*XBFONT_CENTER_X*/ );
	  FontRender( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, 2 /*XBFONT_CENTER_X*/ );
	  FontRender( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255), L"Press any button to continue.", 2 /*XBFONT_CENTER_X*/ );
	EndFontRender( TRUE );

  WaitForNoButton();     // Ensure that the user sees the message
  WaitForAnyButton();
  WaitForNoButton();     // Ensure that we don't bounce right back into the ROM that brought up the error
}

//---------------------------------------------------------------------
//	osd_autobootsavestate
//---------------------------------------------------------------------
void osd_autobootsavestate( const char *gameName )
{
    // This function is called by the MAME core as soon
    // as a ROM is up and running. It check to see if
    // an autoboot state exists for the passed gameName,
    // and schedule a load if one does exist.
  static char filename[64];
  UINT32 i = 0;
  sprintf( filename, "%s-autoboot.sta", gameName );

    // Check all of the registered STATE directories for
    // the autoboot file.
  for( ; i < osd_get_path_count( FILETYPE_STATE ); ++i )
  {
    if( osd_get_path_info( FILETYPE_STATE, i, filename ) == PATH_IS_FILE )
    {
        // Schedule a load
	    cpu_loadsave_schedule_file( LOADSAVE_LOAD, filename );
      return;
    }
  }
}

//---------------------------------------------------------------------
//	osd_die
//---------------------------------------------------------------------
void osd_die( const char *fmt, ... )
{
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

  fatalerror( buf );
}