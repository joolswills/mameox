/**
	* \file		Keyboard.cpp
  *         Simple wrapper around the XINPUT_STATE and XINPUT_CAPABILITIES
  *         structures, providing utility functions such as IsKeyPressed
	*/


//= I N C L U D E S ===========================================================
#include "Keyboard.h"
#include "InputManager.h"
#include "DebugLogger.h"


//= D E F I N E S =============================================================
#define BITARRAYINDEX( _code__ )      ((_code__)>>3)
#define BITPOSITION( _code__ )        ((_code__) - (BITARRAYINDEX(_code__) << 3))

#define SET_KEY( _code__ )            m_keystrokeVector[BITARRAYINDEX(_code__)] |= (1 << BITPOSITION(_code__))
#define CLEAR_KEY( _code__ )          m_keystrokeVector[BITARRAYINDEX(_code__)] &= ~(1<<BITPOSITION(_code__))
#define CHECK_KEY( _code__ )          (m_keystrokeVector[BITARRAYINDEX(_code__)] & (1<<BITPOSITION(_code__)))


//= G L O B A L = V A R S =====================================================


//= F U N C T I O N S =================================================

//------------------------------------------------------
//	PollDevice
//------------------------------------------------------
void CKeyboard::PollDevice( void )
{
  m_inputManager->AttachRemoveDevices();

  if( IsConnected() )
  {
    XINPUT_DEBUG_KEYSTROKE keyStroke;

    while( XInputDebugGetKeystroke( &keyStroke ) == ERROR_SUCCESS )
    {
      if( keyStroke.Flags & XINPUT_DEBUG_KEYSTROKE_FLAG_KEYUP )
      {
        if( CHECK_KEY( keyStroke.VirtualKey ) )
        {
          --m_numKeysPressed;
          CLEAR_KEY( keyStroke.VirtualKey );
        }
      }
      else if( !(keyStroke.Flags & XINPUT_DEBUG_KEYSTROKE_FLAG_REPEAT) )
      {
        if( !CHECK_KEY( keyStroke.VirtualKey ) )
        {
          ++m_numKeysPressed;
          SET_KEY( keyStroke.VirtualKey );
        }
      }
    }
  }
}

//------------------------------------------------------
//	WaitForAnyButton
//------------------------------------------------------
void CKeyboard::WaitForAnyButton( void ) 
{
  while( !m_numKeysPressed && IsConnected() )
    PollDevice();
}


//------------------------------------------------------
//	WaitForNoButton
//------------------------------------------------------
void CKeyboard::WaitForNoButton( void ) 
{
  while( m_numKeysPressed && IsConnected() )
    PollDevice();
}

//------------------------------------------------------
//	IsKeyPressed
//------------------------------------------------------
BOOL CKeyboard::IsKeyPressed( BYTE virtualKeyCode ) const
{
  return CHECK_KEY( virtualKeyCode );
}

//------------------------------------------------------
//	IsOnlyKeyPressed
//------------------------------------------------------
BOOL CKeyboard::IsOnlyKeyPressed( BYTE virtualKeyCode ) const
{
  return ( m_numKeysPressed == 1 && CHECK_KEY( virtualKeyCode ) );
}

//------------------------------------------------------
//	AreAllOfKeysPressed
//------------------------------------------------------
BOOL CKeyboard::AreAllOfKeysPressed( const BYTE *virtualKeyCodeArray, UINT32 numCodes ) const
{
  for( UINT32 i = 0; i < numCodes; ++i )
  {
    if( !CHECK_KEY( virtualKeyCodeArray[i] ) )
      return FALSE;
  }

  return TRUE;
}

//------------------------------------------------------
//	IsOneOfKeysPressed
//------------------------------------------------------
BOOL CKeyboard::IsOneOfKeysPressed( const BYTE *virtualKeyCodeArray, UINT32 numCodes ) const
{
  for( UINT32 i = 0; i < numCodes; ++i )
  {
    if( CHECK_KEY( virtualKeyCodeArray[i] ) )
      return TRUE;
  }
  return FALSE;
}

//------------------------------------------------------
//	AttachRemoveDevices
//------------------------------------------------------
void CKeyboard::AttachRemoveDevices( void ) 
{
  static UINT32 nameMask[4][2] = { {XDEVICE_PORT0, XDEVICE_PORT0_MASK},
                                   {XDEVICE_PORT1, XDEVICE_PORT1_MASK},
                                   {XDEVICE_PORT2, XDEVICE_PORT2_MASK},
                                   {XDEVICE_PORT3, XDEVICE_PORT3_MASK} };


  if( m_portIndex == KEYBOARD_AUTOSEARCH_PORT )
  {
    for( UINT32 i = 0; i < 4; ++i )
    {
	    if( !(m_inputManager->GetKeyboardDeviceBitmap() & nameMask[i][1] ) && m_deviceHandle )
	    {
			    // Detach
		    XInputClose( m_deviceHandle );
		    m_deviceHandle = NULL;
	    }

	    if(	(m_inputManager->GetKeyboardDeviceBitmap() & nameMask[i][1] ) && !m_deviceHandle )
	    {
			    // Attach
		    m_deviceHandle = XInputOpen(  XDEVICE_TYPE_DEBUG_KEYBOARD,
                                      nameMask[i][0],				
																	    XDEVICE_NO_SLOT,			// Keyboard, so no slot
																	    NULL );								// No special polling params
        assert( m_deviceHandle );
        m_portIndex = i;
        m_numKeysPressed = 0;
        memset( m_keystrokeVector, 0, sizeof(m_keystrokeVector) );
        XInputGetCapabilities( m_deviceHandle, &m_caps );
        return;
	    }
    }
  }
  else
  {
	  if( !(m_inputManager->GetKeyboardDeviceBitmap() & nameMask[m_portIndex][1] ) && m_deviceHandle )
	  {
			  // Detach
		  XInputClose( m_deviceHandle );
      m_portIndex = 0xFF;
		  m_deviceHandle = NULL;
	  }

	  if(	(m_inputManager->GetKeyboardDeviceBitmap() & nameMask[m_portIndex][1] ) && !m_deviceHandle )
	  {
			  // Attach
		  m_deviceHandle = XInputOpen(  XDEVICE_TYPE_DEBUG_KEYBOARD,
                                    nameMask[m_portIndex][0],				
																	  XDEVICE_NO_SLOT,			// Keyboard, so no slot
																	  NULL );								// No special polling params
      assert( m_deviceHandle );
      m_numKeysPressed = 0;
      memset( m_keystrokeVector, 0, sizeof(m_keystrokeVector) );
      XInputGetCapabilities( m_deviceHandle, &m_caps );
	  }
  }
}

