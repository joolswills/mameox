

#ifndef _DEBUGLOGGER_H__
#define _DEBUGLOGGER_H__

//= I N C L U D E S ==================================================
#include <xtl.h>
#include <crtdbg.h>



//= D E F I N E S ===============================================

// Module types
#define MT_OFF         0x00
#define MT_TRACE       0x01
#define MT_ERROR       0x02
#define MT_INFO        0x04
#define MT_ALL         0xFF

// This is the default flag for each type
#define MT_DEFAULT     MT_ALL


#ifdef _DEBUGLOGGER
#define DebugLoggerFlush()							Helper_DebugLoggerFlush()
#define DebugLoggerWaitForLogClient()		Helper_DebugLoggerWaitForLogClient()
  // Don't send more than 850 chars or so
#define PRINTMSG												Helper_DebugLoggerPrintMsg
#else
#define DebugLoggerFlush()							
#define DebugLoggerWaitForLogClient()		
#define PRINTMSG												Helper_InlineNOPDebugLoggerPrintMsg
#endif


  // To kill an output type, just define it to T_NOLOG
#define T_NOLOG        NEVERMODULE, LASTLOG, NULL, 0

#define T_TRACE        MT_TRACE, __FILE__, __LINE__, __FUNCTION__
#define T_ERROR        MT_ERROR, __FILE__, __LINE__, __FUNCTION__
#define T_INFO         MT_INFO, __FILE__, __LINE__, __FUNCTION__


#define EZ_TRACE()		PRINTMSG( T_TRACE, "" ); DebugLoggerFlush();

//= P R O T O T Y P E S ========================================

#ifdef __cplusplus
extern "C" {
#endif

  //----------------------------------------------------------------------------
  //  DebugLoggerInit
  //! \brief    Initialize the debug system
  //----------------------------------------------------------------------------
HRESULT DebugLoggerInit( void );


  //----------------------------------------------------------------------------
  //  Helper_DebugLoggerPrintMsg
  //! \brief    Send a message to the debug logger
  //----------------------------------------------------------------------------
void Helper_DebugLoggerPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... );

  //----------------------------------------------------------------------------
  //  Helper_InlineNOPDebugLoggerPrintMsg
  //! \brief    NOP function for non-_DEBUGLOGGER builds
  //----------------------------------------------------------------------------
__inline void Helper_InlineNOPDebugLoggerPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... ) {
}

  //----------------------------------------------------------------------------
  //  DebugLoggerWaitForLogClient
  //! \brief    Wait forever for a log client to connect
  //----------------------------------------------------------------------------
void Helper_DebugLoggerWaitForLogClient( void );

  //----------------------------------------------------------------------------
  //  DebugLoggerFlush
  //! \brief    Block until the message buffer is flushed.
  //! \note     If no log client is connected, this function returns immediately.
  //!           Use DebugLoggerWaitForLogClient to block until a client connects.
  //----------------------------------------------------------------------------
void Helper_DebugLoggerFlush( void );

#ifdef __cplusplus
}
#endif

#endif

