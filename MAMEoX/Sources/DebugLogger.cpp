

//= I N C L U D E S ==================================================
#include "MAMEoX.h"
#include <xbdm.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <list>

#include "DebugLogger.h"

#include "GraphicsManager.h"
#include "FontSet.h"
#include "StdString.h"




//= D E F I N E S ====================================================
#define DBG_FIFO_SZ       32    // Max of 32 items pending
#define MAX_DBG_STRINGSZ  1024  // Max of 1023 printable chars

#define DEBUG_LOG_FILE            "D:\\debug.log"

//= G L O B A L = V A R S ============================================
  // Defined in MAMEoXUtil.cpp
extern CGraphicsManager	  g_graphicsManager;
extern CFontSet           g_fontSet;

  // Whether or not to enable the debug console
static BOOL               g_enableDebugConsole = TRUE;

//#if defined(_DEBUG) || defined(_LOGDEBUGMESSAGES)
static const char   g_LevelName[][4] = {	"TRC",
                                          "INF", 
																	        "ERR" };

static char         g_debugLoggerString[MAX_DBG_STRINGSZ];
//#endif

#ifdef _DEBUG
#define DEBUGCONSOLE_MAXLINES   16
std::list<CStdString> g_debugConsoleData;
#endif

//= P R O T O T Y P E S ===============================================
DWORD WINAPI debugloggermain( void *data );

//= F U N C T I O N S =================================================

//-------------------------------------------------------
//  DebugLoggerInit
//-------------------------------------------------------
extern "C" HRESULT DebugLoggerInit( void )
{
  return S_OK;
}


//-------------------------------------------------------
//  ToggleDebugConsole
//-------------------------------------------------------
void ToggleDebugConsole( void )
{
  g_enableDebugConsole = !g_enableDebugConsole;
}

extern "C" {


#ifdef _DEBUG
//-------------------------------------------------------
//  Helper_OutputDebugStringPrintMsg
//-------------------------------------------------------
void Helper_OutputDebugStringPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  sprintf(  g_debugLoggerString, 
            "<%s %-24.24s [%6.6lu] %-24.24s> ", 
            (msgLevel & MT_TRACE) ? g_LevelName[0] : ((msgLevel & MT_INFO) ? g_LevelName[1] : g_LevelName[2]),
            strrchr(fileName, '\\') + 1, 
            lineNumber,
						function );
	int len = strlen( g_debugLoggerString );


  va_list arg;
  va_start( arg, fmt );
  vsnprintf( &g_debugLoggerString[len], MAX_DBG_STRINGSZ - (len + 2), fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\r\n" );

  OutputDebugString( g_debugLoggerString );
}

//----------------------------------------------------------------------------
//  Helper_ConsolePrintMsg
//----------------------------------------------------------------------------
void Helper_ConsolePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  if( function )
  {
    sprintf(  g_debugLoggerString, 
              "%-16.16s> ", 
						  function );
  }
  else
    g_debugLoggerString[0] = 0;
	int len = strlen( g_debugLoggerString );

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( &g_debugLoggerString[len], MAX_DBG_STRINGSZ - (len + 2), fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\r\n" );

  if( g_debugConsoleData.size() == DEBUGCONSOLE_MAXLINES )
    g_debugConsoleData.pop_front();

  g_debugConsoleData.push_back( g_debugLoggerString );
}

//----------------------------------------------------------------------------
//  Helper_RenderDebugConsole
//----------------------------------------------------------------------------
void Helper_RenderDebugConsole( void *device )
{
  #define CONSOLECOLOR      D3DCOLOR_RGBA( 40, 40, 40, 200 )
  #define CONSOLETEXTCOLOR  D3DCOLOR_XRGB( 255, 255, 255 )
  #define X_POS             80
  #define Y_POS             100
  #define WIDTH             440

  if( !g_debugConsoleData.size() || !device || !g_enableDebugConsole )
    return;

  LPDIRECT3DDEVICE8 pD3DDevice = (LPDIRECT3DDEVICE8)device;



    // Store the values of options that we're going to change
  DWORD vertexShader;
  pD3DDevice->GetVertexShader( &vertexShader );

  IDirect3DVertexBuffer8 *pStreamData;
  UINT stride;
  pD3DDevice->GetStreamSource( 0, &pStreamData, &stride );

  DWORD alphaState, srcBlend, destBlend;
  pD3DDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &alphaState );
  pD3DDevice->GetRenderState( D3DRS_SRCBLEND, &srcBlend );
  pD3DDevice->GetRenderState( D3DRS_DESTBLEND, &destBlend );

  IDirect3DBaseTexture8 *pTexture;
  pD3DDevice->GetTexture( 0, &pTexture );



    // Render a translucent quad for a backdrop
  pD3DDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  pD3DDevice->SetTexture( 0, NULL );

  pD3DDevice->Begin( D3DPT_QUADLIST );
    pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CONSOLECOLOR );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, X_POS - 10, Y_POS - 10, 1.0f, 1.0f );
    
    pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CONSOLECOLOR );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, X_POS + WIDTH + 10, Y_POS - 10, 1.0f, 1.0f );
    
    pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CONSOLECOLOR );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, X_POS + WIDTH + 10, Y_POS + (14 * (g_debugConsoleData.size()+1) ), 1.0f, 1.0f );

    pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CONSOLECOLOR );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, X_POS - 10, Y_POS + (14 * (g_debugConsoleData.size()+1) ), 1.0f, 1.0f );
  pD3DDevice->End();

    // Render the text
  g_fontSet.SmallThinFont().Begin();

    std::list<CStdString>::iterator i = g_debugConsoleData.begin();
    for( UINT32 y = 0; i != g_debugConsoleData.end(); ++i, y += 14 )
    {
      WCHAR wBuf[256];
      mbstowcs( wBuf, (*i).c_str(), 256 );
	    g_fontSet.SmallThinFont().DrawText( X_POS, Y_POS + y, CONSOLETEXTCOLOR, wBuf, XBFONT_TRUNCATED, WIDTH );
    }

  g_fontSet.SmallThinFont().End();


    // Restore settings
  pD3DDevice->SetTexture( 0, pTexture );
  pD3DDevice->SetVertexShader( vertexShader );
  pD3DDevice->SetStreamSource( 0, pStreamData, stride );
  pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, alphaState );
  pD3DDevice->SetRenderState( D3DRS_SRCBLEND, srcBlend );
  pD3DDevice->SetRenderState( D3DRS_DESTBLEND, destBlend );

    // GetStreamSource increments the ref count on the vertex buffer,
    // so we have to release it
  if( pStreamData )
    pStreamData->Release();
}

#endif

//-------------------------------------------------------
//  Helper_WriteToFilePrintMsg
//-------------------------------------------------------
void Helper_WriteToFilePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  sprintf(  g_debugLoggerString, 
            "<%s %-24.24s [%6.6lu] %-32.32s> ", 
            (msgLevel & MT_TRACE) ? g_LevelName[0] : ((msgLevel & MT_INFO) ? g_LevelName[1] : g_LevelName[2]),
            strrchr(fileName, '\\') + 1, 
            lineNumber,
						function );
	int len = strlen( g_debugLoggerString );

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( &g_debugLoggerString[len], MAX_DBG_STRINGSZ - (len + 2), fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\r\n" );


  HANDLE h = CreateFile(  DEBUG_LOG_FILE,
                          GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );
  if( h != INVALID_HANDLE_VALUE )
  {
      // Seek to the end of the file
    LONG upperPos = 0;
    SetFilePointer( h, 0, &upperPos, FILE_END );

      // Write out the text
    DWORD bytesWritten;
    WriteFile( h, g_debugLoggerString, strlen(g_debugLoggerString), &bytesWritten, NULL );

    CloseHandle( h );
  }
}

} // extern "C"



