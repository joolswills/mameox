/**
	* \file		Gamepad.cpp
  *         Simple wrapper around the XINPUT_STATE and XINPUT_CAPABILITIES
  *         structures, providing utility functions such as IsButtonPressed
	*/


//= I N C L U D E S ===========================================================
#include "Gamepad.h"
#include "DebugLogger.h"


//= D E F I N E S =============================================================
#define STICK_DEADZONE              0.35f

//= G L O B A L = V A R S =====================================================


//= F U N C T I O N S =================================================



//------------------------------------------------------
//	Constructor
//------------------------------------------------------
CGamepad::CGamepad( void ) 
{
	memset( this, NULL, sizeof(*this) );
}

//------------------------------------------------------
//	Create
//------------------------------------------------------
BOOL CGamepad::Create(  DWORD gpIndex, 
                        DWORD maxMemUnits, 
                        const DWORD *gpBitmap, 
                        const DWORD *muBitmap ) 
{
  if( gpIndex > 3 )
    return FALSE;

  m_gamepadIndex = gpIndex;

	m_gamepadDeviceBitmap = gpBitmap;
	m_memunitDeviceBitmap = muBitmap;


	switch( m_gamepadIndex )
	{
	default:
//			assert( 0 && "Invalid gamepad port number!" );
		// fallthrough

		// *** 0 *** //
	case 0:
		m_portName          = XDEVICE_PORT0;
		m_portMask          = XDEVICE_PORT0_MASK;
		m_topMemPortMask    = XDEVICE_PORT0_TOP_MASK;
		m_bottomMemPortMask = XDEVICE_PORT0_BOTTOM_MASK;
		break;

		// *** 1 *** //
	case 1:
		m_portName          = XDEVICE_PORT1;
		m_portMask          = XDEVICE_PORT1_MASK;
		m_topMemPortMask    = XDEVICE_PORT1_TOP_MASK;
		m_bottomMemPortMask = XDEVICE_PORT1_BOTTOM_MASK;
		break;

		// *** 2 *** //
	case 2:
		m_portName          = XDEVICE_PORT2;
		m_portMask          = XDEVICE_PORT2_MASK;
		m_topMemPortMask    = XDEVICE_PORT2_TOP_MASK;
		m_bottomMemPortMask = XDEVICE_PORT2_BOTTOM_MASK;
		break;

		// *** 3 *** //
	case 3:
		m_portName          = XDEVICE_PORT3;
		m_portMask          = XDEVICE_PORT3_MASK;
		m_topMemPortMask    = XDEVICE_PORT3_TOP_MASK;
		m_bottomMemPortMask = XDEVICE_PORT3_BOTTOM_MASK;
		break;
	}

  return TRUE;
}

//------------------------------------------------------
//	WaitForAnyButton
//------------------------------------------------------
void CGamepad::WaitForAnyButton( void ) 
{
  if( !m_gamepadDeviceHandle )
  {
    PRINTMSG( T_INFO, "WaitForAnyButton called on invalid gamepad!" );
    return;
  }

  do
  {
    PollDevice();
  } while( !IsAnyButtonPressed() );
}

//------------------------------------------------------
//	WaitForNoButton
//------------------------------------------------------
void CGamepad::WaitForNoButton( void ) 
{
  if( !m_gamepadDeviceHandle )
  {
    PRINTMSG( T_INFO, "WaitForNoButton called on invalid gamepad!" );
    return;
  }

	do
	{
		PollDevice();
  } while( IsAnyButtonPressed() );
}


//------------------------------------------------------
//	IsConnected
//------------------------------------------------------
BOOL CGamepad::IsConnected( void ) const
{
  return m_gamepadDeviceHandle != NULL;
}

//------------------------------------------------------
//	IsMUConnected
//------------------------------------------------------
BOOL CGamepad::IsMUConnected( BOOL bottomMU ) const
{
  return m_memunitDeviceHandles[bottomMU] != NULL;
}

//------------------------------------------------------
//	GetAnalogButtonState
//------------------------------------------------------
UINT8 CGamepad::GetAnalogButtonState( gamepadButtonID_t buttonID ) const
{
  if( !IsConnected() )
    return 0;

  const XINPUT_GAMEPAD	&gp = m_state.Gamepad;
  switch( buttonID )
  {
  case GP_A:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_A];

  case GP_B:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_B];

  case GP_X:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_X];

  case GP_Y:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_Y];

  case GP_WHITE:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE];

  case GP_BLACK:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK];

  case GP_LEFT_TRIGGER:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER];

  case GP_RIGHT_TRIGGER:
    return gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER];
  }
  return 0;
}

//------------------------------------------------------
//	GetAnalogAxisState
//------------------------------------------------------
SHORT CGamepad::GetAnalogAxisState( gamepadAnalogID_t analogID, gamepadAxisID_t axisID ) const
{
  if( !IsConnected() )
    return 0;

  const XINPUT_GAMEPAD &gp = m_state.Gamepad;
  if( analogID == GP_ANALOG_LEFT )
  {
    if( axisID == GP_AXIS_X )
      return gp.sThumbLX;
    else
      return gp.sThumbLY;
  }
  else
  {
    if( axisID == GP_AXIS_X )
      return gp.sThumbRX;
    else
      return gp.sThumbRY;
  }
}

//------------------------------------------------------
//	GetButtonState
//------------------------------------------------------
UINT32 CGamepad::GetButtonState( void ) const
{
  if( !IsConnected() )
    return 0;

  const XINPUT_GAMEPAD &gp = m_state.Gamepad;
	UINT32 curState;

	curState = 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_DPAD_UP ) != 0 ) ?    GP_DPAD_UP : 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) != 0 ) ? GP_DPAD_RIGHT : 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) != 0 ) ?  GP_DPAD_DOWN : 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) != 0 ) ?  GP_DPAD_LEFT : 0;

	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_BACK ) != 0 ) ?  GP_BACK : 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_START ) != 0 ) ? GP_START : 0;

	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) != 0 ) ?  GP_LEFT_ANALOG : 0;
	curState |= ( ( gp.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) != 0 ) ? GP_RIGHT_ANALOG : 0;

	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_A ] >= BUTTON_PRESS_THRESHOLD ) ? GP_A : 0;
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_B ] >= BUTTON_PRESS_THRESHOLD ) ? GP_B : 0;
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_X ] >= BUTTON_PRESS_THRESHOLD ) ? GP_X : 0;
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_Y ] >= BUTTON_PRESS_THRESHOLD ) ? GP_Y : 0;

	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_WHITE ] >= BUTTON_PRESS_THRESHOLD ) ? GP_WHITE : 0;
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_BLACK ] >= BUTTON_PRESS_THRESHOLD ) ? GP_BLACK : 0;
 
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_LEFT_TRIGGER ] >= BUTTON_PRESS_THRESHOLD ) ? GP_LEFT_TRIGGER : 0;
	curState |= ( gp.bAnalogButtons[ XINPUT_GAMEPAD_RIGHT_TRIGGER ] >= BUTTON_PRESS_THRESHOLD ) ? GP_RIGHT_TRIGGER : 0;

  #define ANALOG_AS_DIGITAL_VAL    ( SHORT )( 32767.0f * STICK_DEADZONE )

	curState |= ( gp.sThumbLY >=  ANALOG_AS_DIGITAL_VAL ) ? GP_LA_UP : 0;
	curState |= ( gp.sThumbLX <= -ANALOG_AS_DIGITAL_VAL ) ? GP_LA_LEFT : 0;
	curState |= ( gp.sThumbLY <= -ANALOG_AS_DIGITAL_VAL ) ? GP_LA_DOWN : 0;
	curState |= ( gp.sThumbLX >=  ANALOG_AS_DIGITAL_VAL ) ? GP_LA_RIGHT : 0;

	curState |= ( gp.sThumbRY >=  ANALOG_AS_DIGITAL_VAL ) ? GP_RA_UP : 0;
	curState |= ( gp.sThumbRX <= -ANALOG_AS_DIGITAL_VAL ) ? GP_RA_LEFT : 0;
	curState |= ( gp.sThumbRY <= -ANALOG_AS_DIGITAL_VAL ) ? GP_RA_DOWN : 0;
	curState |= ( gp.sThumbRX >=  ANALOG_AS_DIGITAL_VAL ) ? GP_RA_RIGHT : 0;

  return( curState );
}

//------------------------------------------------------
//	IsAnyButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsAnyButtonPressed( void ) const
{
	UINT32 state = GetButtonState();

	  // Only Buttons
	state &= ( GP_A | GP_B | GP_X | GP_Y | 
             GP_BLACK | GP_WHITE | 
             GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER | 
             GP_LEFT_ANALOG | GP_RIGHT_ANALOG |
             GP_START | GP_BACK );

	return( state != 0 );
}

//------------------------------------------------------
//	IsButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsButtonPressed( UINT32 buttonID ) const
{
  return ( (GetButtonState() & buttonID) == buttonID );
}

//------------------------------------------------------
//	IsOnlyButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsOnlyButtonPressed( UINT32 buttonID ) const
{
  return ( GetButtonState() == buttonID );
}

//------------------------------------------------------
//	IsOneOfButtonsPressed
//------------------------------------------------------
BOOL CGamepad::IsOneOfButtonsPressed( UINT32 buttonID ) const
{
  return ( GetButtonState() & buttonID );
}


//------------------------------------------------------
//	GetGamepadDeviceState
//------------------------------------------------------
const XINPUT_GAMEPAD *CGamepad::GetGamepadDeviceState( void ) const 
{
	return &m_state.Gamepad;
}

//------------------------------------------------------
//	GetGamepadDeviceCaps
//------------------------------------------------------
const XINPUT_CAPABILITIES *CGamepad::GetGamepadDeviceCaps( void ) const 
{
	return &m_caps;
}



