

//= I N C L U D E S ==================================================
#include <xtl.h>
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
/*
  #error "The debug logger requires a thread, the current C lib is single threaded!"
    // Set up an unsecure connection to allow the PC logger to connect
  XNetStartupParams xnsp;
  memset( &xnsp, 0, sizeof( xnsp ) );
  xnsp.cfgSizeOfStruct = sizeof( XNetStartupParams );
  xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
  INT err = XNetStartup( &xnsp );

  WSADATA WSAData;
  WSAStartup( MAKEWORD( 2, 0 ), &WSAData );


  g_logClientConnectedEvent = CreateEvent(  NULL,
                                            TRUE,   // Manual Reset
                                            FALSE,  // Initial state
                                            g_logClientConnected );

  g_logClientFlushedEvent = CreateEvent(  NULL,
                                          TRUE,   // Manual Reset
                                          TRUE,   // Initial state
                                          g_logClientFlushed );

			// Start up the debug logger thread
		g_workerThread = CreateThread(  NULL,
																		8 * 1024,
																		debugloggermain,
																		NULL,
																		0,
																		&g_workerThreadID );
		if( g_workerThread == INVALID_HANDLE_VALUE )
		{
			return ERROR_INVALID_HANDLE;
		}

    // Debug logger isn't used, so unload the network XBE section to
    //  save memory 
  XFreeSection( "XTL" );
*/
  return S_OK;
}


//-------------------------------------------------------
//  ToggleDebugConsole
//-------------------------------------------------------
void ToggleDebugConsole( void )
{
  g_enableDebugConsole = !g_enableDebugConsole;
}

/*
//-------------------------------------------------------
//  Helper_TransmitDebugLog
//-------------------------------------------------------
static BOOL Helper_TransmitDebugLog( SOCKET outputSock, const char *logString )
{
	#ifdef _DEBUGLOGGER
  if( !logString )
    return TRUE;

  if( outputSock < 0 )
    return FALSE;

  ULONG toSend = strlen( logString );

  char msgHeader[5] = { XBX_LOG, 0 };
  *((ULONG*)&msgHeader[1]) = toSend;
  if( send( outputSock, msgHeader, 5, 0 ) != 5 )
    return FALSE;

  for( ULONG sent = 0; sent < toSend; )
  {
    ULONG snt = send( outputSock, &logString[sent], toSend - sent, 0 );

      // If some error occurred return
    if( snt < 0 )
      return FALSE;

    sent += snt;
  }
  #endif
  return TRUE;
}

//-------------------------------------------------------
//  debugloggermain
//-------------------------------------------------------
DWORD WINAPI debugloggermain( void *data )
{
  ULONG numSocks = 1;
  SOCKET listenSock = socket( AF_INET, SOCK_STREAM, 0 );
  SOCKET outputSock = -1;


  HANDLE LogClientConnectedEvent = OpenEvent( 0, FALSE, g_logClientConnected );
  ResetEvent( LogClientConnectedEvent );

  HANDLE LogClientFlushedEvent = OpenEvent( 0, FALSE, g_logClientFlushed );
  SetEvent( LogClientFlushedEvent );

  struct sockaddr_in saddrIN;
  saddrIN.sin_family = AF_INET;
  saddrIN.sin_port = htons( 20 );
  saddrIN.sin_addr.s_addr = INADDR_ANY;
  bind( listenSock, (const struct sockaddr *)&saddrIN, sizeof(saddrIN) );
  listen( listenSock, 1 );

  while( 1 )
  {
    fd_set fds;
    FD_ZERO( &fds );
    FD_SET( listenSock, &fds );

    struct timeval timeOut = { 0, 500 };

    int numEvents = select( numSocks, &fds, NULL, NULL, &timeOut );
    if( numEvents > 0 && FD_ISSET( listenSock, &fds ) )
    {
      if( outputSock >= 0 )
      {
            shutdown( outputSock, 2 );
            close( outputSock );
            ResetEvent( LogClientConnectedEvent );
      }
      if( (outputSock = accept( listenSock, NULL, NULL )) >= 0 )
        SetEvent( LogClientConnectedEvent );
    }


    if( outputSock >= 0 && g_fifoReadHead != g_fifoWriteHead )
    {
        // Flush any pending logs
      if( g_fifoReadHead > g_fifoWriteHead )
      {
        for( ; g_fifoReadHead < DBG_FIFO_SZ; ++g_fifoReadHead )
        {
          if( !Helper_TransmitDebugLog( outputSock, g_debugLoggerStringFIFO[g_fifoReadHead] ) )
          {
            shutdown( outputSock, 2 );
            close( outputSock );
            ResetEvent( LogClientConnectedEvent );
            goto sendingComplete;          
          }
        }
        g_fifoReadHead = 0;
      }
      for( ; g_fifoReadHead < g_fifoWriteHead; ++g_fifoReadHead )
      {
          if( !Helper_TransmitDebugLog( outputSock, g_debugLoggerStringFIFO[g_fifoReadHead] ) )
          {
            shutdown( outputSock, 2 );
            close( outputSock );
            ResetEvent( LogClientConnectedEvent );
          }
      }

        // Store the fact that the log buffer is now flushed
      SetEvent( LogClientFlushedEvent );


sendingComplete:
      ;
    }
  }
  return 0;
}
*/




extern "C" {

/*
//-------------------------------------------------------
//  Helper_DebugLoggerPrintMsg
//-------------------------------------------------------
void Helper_DebugLoggerPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  g_messageID = (g_messageID + 1) & 0x0FFFFF;

    // Drop messages if we have no more room in the FIFO
  if( g_fifoWriteHead == g_fifoReadHead - 1 || (!g_fifoReadHead && g_fifoWriteHead == DBG_FIFO_SZ - 1) )
    return;

  char *stringBuffer = g_debugLoggerStringFIFO[g_fifoWriteHead];

  sprintf(  stringBuffer, 
            "<%s %-24.24s [%6.6lu] %-24.24s %6.6lu> ", 
            (msgLevel & MT_TRACE) ? g_LevelName[0] : ((msgLevel & MT_INFO) ? g_LevelName[1] : g_LevelName[2]),
            strrchr(fileName, '\\') + 1, 
            lineNumber,
						function,
            g_messageID );

  va_list arg;
  va_start( arg, fmt );
  vsprintf( &stringBuffer[strlen(stringBuffer)], fmt, arg );
  va_end( arg );


  g_fifoWriteHead = g_fifoWriteHead + 1;
  if( g_fifoWriteHead == DBG_FIFO_SZ )
    g_fifoWriteHead = 0;

    // Store the fact that the log buffer is not flushed
  ResetEvent( g_logClientFlushedEvent );
}


//-------------------------------------------------------
//  Helper_DebugLoggerWaitForLogClient
//-------------------------------------------------------
void Helper_DebugLoggerWaitForLogClient( void )
{
    // Log client hasn't been started yet
  if( !g_logClientConnectedEvent )
    return;

  WaitForSingleObject( g_logClientConnectedEvent, INFINITE );
}


//-------------------------------------------------------
//  Helper_DebugLoggerFlush
//-------------------------------------------------------
void Helper_DebugLoggerFlush( void )
{
    // Log client hasn't been started yet
  if( !g_logClientFlushedEvent || !g_logClientConnectedEvent )
    return;

    // Make sure somebody is connected
  if( WaitForSingleObject( g_logClientFlushedEvent, 1 ) == WAIT_TIMEOUT )
    return;

  WaitForSingleObject( g_logClientFlushedEvent, INFINITE );
}

*/

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

  va_list arg;
  va_start( arg, fmt );
  vsprintf( &g_debugLoggerString[strlen(g_debugLoggerString)], fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\n" );

  OutputDebugString( g_debugLoggerString );
}

//----------------------------------------------------------------------------
//  Helper_ConsolePrintMsg
//----------------------------------------------------------------------------
void Helper_ConsolePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  sprintf(  g_debugLoggerString, 
            "%-16.16s> ", 
						function );

  va_list arg;
  va_start( arg, fmt );
  vsprintf( &g_debugLoggerString[strlen(g_debugLoggerString)], fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\n" );

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
}

#endif

//-------------------------------------------------------
//  Helper_WriteToFilePrintMsg
//-------------------------------------------------------
void Helper_WriteToFilePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... )
{
  sprintf(  g_debugLoggerString, 
            "<%s %-24.24s [%6.6lu] %-24.24s> ", 
            (msgLevel & MT_TRACE) ? g_LevelName[0] : ((msgLevel & MT_INFO) ? g_LevelName[1] : g_LevelName[2]),
            strrchr(fileName, '\\') + 1, 
            lineNumber,
						function );

  va_list arg;
  va_start( arg, fmt );
  vsprintf( &g_debugLoggerString[strlen(g_debugLoggerString)], fmt, arg );
  va_end( arg );

  strcat( g_debugLoggerString, "\n" );


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



