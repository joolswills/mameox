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
#include "palette.h"
#include "common.h"


#include <stdio.h>
#include <stdarg.h>

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	osd_display_loading_rom_message
//---------------------------------------------------------------------
int osd_display_loading_rom_message( const char *name, struct rom_load_data *romdata )
{
	/* called while loading ROMs. It is called a last time with name == 0 to signal */
	/* that the ROM loading process is finished. */
	/* return non-zero to abort loading */

	WCHAR title[128];
	WCHAR bar[128];
	PRINTMSG( T_TRACE, "osd_display_loading_rom_message" );

	swprintf( bar, L"[%80c]", L' ' );
#if 0
struct rom_load_data
{
	int warnings;				/* warning count during processing */
	int errors;				/* error count during processing */

	int romsloaded;				/* current ROMs loaded count */
	int romstotal;				/* total number of ROMs to read */

	void * file;				/* current file */

	UINT8 *	regionbase;			/* base of current region */
	UINT32 regionlength;			/* length of current region */

	char errorbuf[4096];			/* accumulated errors */
	UINT8 tempbuf[65536];			/* temporary buffer */
};
#endif



	if( name )
	{
		UINT32 i = 0;
		wcscpy( title, L"Loading \"" );
		mbstowcs( &title[wcslen(title)], name, 32 );
		swprintf( &title[wcslen(title)], L"\" (%d/ %d)", romdata->romsloaded, romdata->romstotal );

		for( ; i < (ULONG)(((FLOAT)romdata->romsloaded / (FLOAT)romdata->romstotal) * 80.0f); ++i )
			bar[i+1] = L'|';
	}
	else
	{
		UINT32 i = 0;
		wcscpy( title, L"Loading complete!" );
		for( ; i < 80; ++i )
			bar[i+1] = L'|';
	}


	BeginFontRender( TRUE );
	FontRender( 70, 220, D3DCOLOR_RGBA( 230, 230, 230, 255 ), title, 0 );
	FontRender( 320, 240, D3DCOLOR_RGBA( 120, 230, 120, 255 ), bar, 2 );
	EndFontRender();
	
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
	char buf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

	PRINTMSG( T_ERROR, buf );
	DebugLoggerFlush();
}

//---------------------------------------------------------------------
//	osd_print_error
//---------------------------------------------------------------------
void osd_print_error( const char *fmt, ... )
{
	char buf[1024];
  wchar_t wBuf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );


    // Display the error to the user
	BeginFontRender( TRUE );
  mbstowcs( wBuf, buf, 1024 );
  FontRender( 320, 60, D3DCOLOR_RGBA( 255, 200, 200, 255 ), L"Error:", 2 /*XBFONT_CENTER_X*/ );
	FontRender( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, 2 /*XBFONT_CENTER_X*/ );
	FontRender( 320, 320, D3DCOLOR_RGBA( 70, 235, 125, 255), L"Press any button to continue.", 2 /*XBFONT_CENTER_X*/ );
	EndFontRender();

  WaitForNoKey();     // Ensure that the user sees the message
  WaitForKey();
  WaitForNoKey();     // Ensure that we don't bounce right back into the ROM that brought up the error
}

