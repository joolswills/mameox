

//= I N C L U D E S ==================================================
#include "DebugLogger.h"

#include <xtl.h>
#include <xbdm.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>



//= D E F I N E S ====================================================
#define DBG_FIFO_SZ       32    // Max of 32 items pending
#define MAX_DBG_STRINGSZ  1024  // Max of 1023 printable chars


#define XBX_LOG                   0x2B                                 //!< "Log this message" command


//= G L O B A L = V A R S ============================================
static char         g_debugLoggerStringFIFO[DBG_FIFO_SZ][MAX_DBG_STRINGSZ];
static ULONG        g_fifoWriteHead = 0;
static ULONG        g_fifoReadHead = 0;

static HANDLE       g_workerThread = INVALID_HANDLE_VALUE;
static DWORD        g_workerThreadID = 0;

static const char   g_LevelName[][4] = {	"TRC",
                                          "INF", 
																	        "ERR" };

const char          g_logClientConnected[] = "LogClntConnected";
const char          g_logClientFlushed[] = "LogClntFlushed";
static HANDLE       g_logClientConnectedEvent = NULL;
static HANDLE       g_logClientFlushedEvent = NULL;

static ULONG        g_messageID = 0;

//= P R O T O T Y P E S ===============================================
DWORD WINAPI debugloggermain( void *data );

//= F U N C T I O N S =================================================

//-------------------------------------------------------
//  DebugLoggerInit
//-------------------------------------------------------
extern "C" HRESULT DebugLoggerInit( void )
{
	#ifdef _DEBUGLOGGER
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

  #else
    // Debug logger isn't used, so unload the network XBE section to
    //  save memory 
  XFreeSection( "XTL" );
	#endif

  return S_OK;
}

//-------------------------------------------------------
//  DebugLoggerTerm
//-------------------------------------------------------
void DebugLoggerTerm( void )
{
    // Can't stop the debug logger! (ala Can't stop the music, god bless philips marketing)
//  WSACleanup();
//  XNetCleanup();
}



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
	#ifdef _DEBUGLOGGER
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
  #endif

  return 0;
}

extern "C" {


#ifdef _DEBUGLOGGER
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

#endif // _DEBUGLOGGER


}



