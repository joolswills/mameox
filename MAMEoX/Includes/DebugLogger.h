

#ifndef _DEBUGLOGGER_H__
#define _DEBUGLOGGER_H__

//= I N C L U D E S ==================================================
#include "MAMEoX.h"
#include <crtdbg.h>



//= D E F I N E S ===============================================

  // Uncomment this line to send PRINTMSG data to Debug.Log in
  // the MAMEoX root directory. Note that it only takes effect
  // in release mode
//#define _LOGDEBUGMESSAGES

// Module types
#define MT_OFF         0x00
#define MT_TRACE       0x01
#define MT_ERROR       0x02
#define MT_INFO        0x04

  //! The number of seconds that the BACK button has to be
  //! held down before the debug console is toggled (note that the
  //! actual handling is done in xbox_Display.c and main.cpp, not here)
#define DEBUGCONSOLE_TOGGLE_DELAY     2

#ifdef _DEBUG
  #define RenderDebugConsole(dev)                         Helper_RenderDebugConsole( (void*)dev )
  #define PRINTMSG(c)												              Helper_OutputDebugStringPrintMsg c
  #define PRINTMSG_TO_CONSOLE(c)                          Helper_ConsolePrintMsg c
#else
  #define RenderDebugConsole(dev)

  #ifdef _LOGDEBUGMESSAGES
    #define PRINTMSG(c)							  			              Helper_WriteToFilePrintMsg c
    #define PRINTMSG_TO_CONSOLE(c)                        Helper_WriteToFilePrintMsg c
  #else
    #define PRINTMSG(c)						  				              Helper_InlineNOPDebugLoggerPrintMsg c
    #define PRINTMSG_TO_CONSOLE(c)                        Helper_InlineNOPDebugLoggerPrintMsg c
  #endif

#endif

#define PRINTMSG_TO_LOG(c)                                Helper_WriteToFilePrintMsg c

  // To kill an output type, just define it to T_NOLOG
#define T_NOLOG        NEVERMODULE, LASTLOG, NULL, 0

#define T_TRACE        MT_TRACE, __FILE__, __LINE__, __FUNCTION__
#define T_ERROR        MT_ERROR, __FILE__, __LINE__, __FUNCTION__
#define T_INFO         MT_INFO, __FILE__, __LINE__, __FUNCTION__
#define T_NOPOSITION   MT_INFO, NULL, 0, NULL

#define EZ_TRACE()		PRINTMSG(( T_TRACE, "" ));

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
  //  ToggleDebugConsole
  //! \brief    Toggle between showing and hiding the debug console
  //----------------------------------------------------------------------------
void ToggleDebugConsole( void );

#ifdef _DEBUG
  //----------------------------------------------------------------------------
  //  Helper_DebugLoggerPrintMsg
  //! \brief    Send a message to the debug logger
  //----------------------------------------------------------------------------
void Helper_DebugLoggerPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... );

  //----------------------------------------------------------------------------
  //  Helper_OutputDebugStringPrintMsg
  //! \brief    Send a message to the debugger/xboxwatson tool
  //----------------------------------------------------------------------------
void Helper_OutputDebugStringPrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... );

  //----------------------------------------------------------------------------
  //  Helper_ConsolePrintMsg
  //! \brief    Put a message on the debug console
  //----------------------------------------------------------------------------
void Helper_ConsolePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... );

  //----------------------------------------------------------------------------
  //  Helper_RenderDebugConsole
  //! \brief    Render the debug console to the device retrieved from g_graphicsManager
  //----------------------------------------------------------------------------
void Helper_RenderDebugConsole( void *device );
#endif

  //----------------------------------------------------------------------------
  //  Helper_WriteToFilePrintMsg
  //! \brief    Write a message to the debug.log file in the MAMEoX directory
  //----------------------------------------------------------------------------
void Helper_WriteToFilePrintMsg( ULONG msgLevel, const char *fileName, ULONG lineNumber, const char *function, const char *fmt, ... );

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

