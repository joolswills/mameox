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
BOOL CGamepad::IsConnected( void ) 
{
  return m_gamepadDeviceHandle != NULL;
}

//------------------------------------------------------
//	IsMUConnected
//------------------------------------------------------
BOOL CGamepad::IsMUConnected( BOOL bottomMU ) 
{
  return m_memunitDeviceHandles[bottomMU] != NULL;
}

//------------------------------------------------------
//	GetAnalogButtonState
//------------------------------------------------------
UINT8 CGamepad::GetAnalogButtonState( gamepadButtonID_t buttonID )
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
SHORT CGamepad::GetAnalogAxisState( gamepadAnalogID_t analogID, gamepadAxisID_t axisID )
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
//	IsAnyButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsAnyButtonPressed( void ) 
{
  if( !IsConnected() )
    return FALSE;

  const XINPUT_GAMEPAD	&gp = m_state.Gamepad;
	return ( gp.wButtons ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_A] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_B] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_X] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_Y] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] >= BUTTON_PRESS_THRESHOLD ||
					  gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] >= BUTTON_PRESS_THRESHOLD ||
  					gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] >= BUTTON_PRESS_THRESHOLD );
}

//------------------------------------------------------
//	IsButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsButtonPressed( UINT32 buttonID )
{
  if( !IsConnected() )
    return FALSE;

  BOOL ret = TRUE;
  const XINPUT_GAMEPAD	&gp = m_state.Gamepad;

  if( buttonID & GP_DPAD_UP )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
  if( buttonID & GP_DPAD_RIGHT )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
  if( buttonID & GP_DPAD_DOWN )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
  if( buttonID & GP_DPAD_LEFT )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);

  if( buttonID & GP_BACK )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_BACK) != 0);
  if( buttonID & GP_START )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_START) != 0);

  if( buttonID & GP_LEFT_ANALOG )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
  if( buttonID & GP_RIGHT_ANALOG )
    ret &= ((gp.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);

  if( buttonID & GP_A )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_A] >= BUTTON_PRESS_THRESHOLD);
  if( buttonID & GP_B )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_B] >= BUTTON_PRESS_THRESHOLD);
  if( buttonID & GP_X )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_X] >= BUTTON_PRESS_THRESHOLD);
  if( buttonID & GP_Y )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_Y] >= BUTTON_PRESS_THRESHOLD);

  if( buttonID & GP_WHITE )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] >= BUTTON_PRESS_THRESHOLD);
  if( buttonID & GP_BLACK )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] >= BUTTON_PRESS_THRESHOLD);
 
  if( buttonID & GP_LEFT_TRIGGER )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] >= BUTTON_PRESS_THRESHOLD);
  if( buttonID & GP_RIGHT_TRIGGER )
    ret &= (gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] >= BUTTON_PRESS_THRESHOLD);

  #define ANALOG_AS_DIGITAL_VAL    (SHORT)(32767.0f * STICK_DEADZONE)

  if( buttonID & GP_LA_UP )
    ret &= (gp.sThumbLY >= ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_LEFT )
    ret &= (gp.sThumbLX <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_DOWN )
    ret &= (gp.sThumbLY <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_RIGHT )
    ret &= (gp.sThumbLX >= ANALOG_AS_DIGITAL_VAL);

  if( buttonID & GP_RA_UP )
    ret &= (gp.sThumbRY >= ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_LEFT )
    ret &= (gp.sThumbRX <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_DOWN )
    ret &= (gp.sThumbRY <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_RIGHT )
    ret &= (gp.sThumbRX >= ANALOG_AS_DIGITAL_VAL);


  return ret;
}

//------------------------------------------------------
//	IsOneOfButtonsPressed
//------------------------------------------------------
BOOL CGamepad::IsOneOfButtonsPressed( UINT32 buttonID )
{
  if( !IsConnected() )
    return FALSE;

  BOOL ret = FALSE;
  const XINPUT_GAMEPAD	&gp = m_state.Gamepad;

  if( buttonID & GP_DPAD_UP )
    ret |= gp.wButtons & XINPUT_GAMEPAD_DPAD_UP;
  if( buttonID & GP_DPAD_RIGHT )
    ret |= gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
  if( buttonID & GP_DPAD_DOWN )
    ret |= gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
  if( buttonID & GP_DPAD_LEFT )
    ret |= gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;

  if( buttonID & GP_BACK )
    ret |= gp.wButtons & XINPUT_GAMEPAD_BACK;
  if( buttonID & GP_START )
    ret |= gp.wButtons & XINPUT_GAMEPAD_START;

  if( buttonID & GP_LEFT_ANALOG )
    ret |= gp.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
  if( buttonID & GP_RIGHT_ANALOG )
    ret |= gp.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

  if( buttonID & GP_A )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_A] >= BUTTON_PRESS_THRESHOLD;
  if( buttonID & GP_B )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_B] >= BUTTON_PRESS_THRESHOLD;
  if( buttonID & GP_X )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_X] >= BUTTON_PRESS_THRESHOLD;
  if( buttonID & GP_Y )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_Y] >= BUTTON_PRESS_THRESHOLD;

  if( buttonID & GP_WHITE )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] >= BUTTON_PRESS_THRESHOLD;
  if( buttonID & GP_BLACK )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] >= BUTTON_PRESS_THRESHOLD;
 
  if( buttonID & GP_LEFT_TRIGGER )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] >= BUTTON_PRESS_THRESHOLD;
  if( buttonID & GP_RIGHT_TRIGGER )
    ret |= gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] >= BUTTON_PRESS_THRESHOLD;

  if( buttonID & GP_LA_UP )
    ret |= (gp.sThumbLY >= ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_LEFT )
    ret |= (gp.sThumbLX <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_DOWN )
    ret |= (gp.sThumbLY <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_LA_RIGHT )
    ret |= (gp.sThumbLX >= ANALOG_AS_DIGITAL_VAL);

  if( buttonID & GP_RA_UP )
    ret |= (gp.sThumbRY >= ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_LEFT )
    ret |= (gp.sThumbRX <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_DOWN )
    ret |= (gp.sThumbRY <= -ANALOG_AS_DIGITAL_VAL);
  if( buttonID & GP_RA_RIGHT )
    ret |= (gp.sThumbRX >= ANALOG_AS_DIGITAL_VAL);


  return ret;
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



