/**
	* \file		Gamepad.cpp
  *         Simple wrapper around the XINPUT_STATE and XINPUT_CAPABILITIES
  *         structures, providing utility functions such as IsButtonPressed
	*/


//= I N C L U D E S ===========================================================
#include "Gamepad.h"
#include "InputManager.h"
#include "DebugLogger.h"


//= D E F I N E S =============================================================
#define STICK_DEADZONE              0.38f

//= G L O B A L = V A R S =====================================================


//= F U N C T I O N S =================================================



//------------------------------------------------------
//	Constructor
//------------------------------------------------------
CGamepad::CGamepad( void ) : CInputDevice()
{
	m_topMemPortMask = m_bottomMemPortMask = 0;
	m_memunitDeviceHandles[0] = m_memunitDeviceHandles[1] = NULL;
}

//------------------------------------------------------
//	Create
//------------------------------------------------------
BOOL CGamepad::Create(  DWORD gpIndex, 
                        DWORD maxMemUnits, 
                        CInputManager *inputManager )
{
  if( gpIndex > 3 || !inputManager )
    return FALSE;

  if( !CInputDevice::Create( gpIndex, inputManager ) )
    return FALSE;

	switch( m_portIndex )
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
//	PollDevice
//------------------------------------------------------
void CGamepad::PollDevice( void )
{
  m_inputManager->AttachRemoveDevices();

  if( m_deviceHandle )
    XInputGetState( m_deviceHandle, &m_state );
}

//------------------------------------------------------
//	WaitForAnyButton
//------------------------------------------------------
void CGamepad::WaitForAnyButton( void ) 
{
  if( !m_deviceHandle )
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
  if( !m_deviceHandle )
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
//	WaitForAnyInput
//------------------------------------------------------
void CGamepad::WaitForAnyInput( void ) 
{
  if( !m_deviceHandle )
  {
    PRINTMSG( T_INFO, "WaitForAnyInput called on invalid gamepad!" );
    return;
  }

	do
	{
		PollDevice();
  } while( !GetInputState() );
}

//------------------------------------------------------
//	WaitForNoInput
//------------------------------------------------------
void CGamepad::WaitForNoInput( void ) 
{
  if( !m_deviceHandle )
  {
    PRINTMSG( T_INFO, "WaitForNoInput called on invalid gamepad!" );
    return;
  }

	do
	{
		PollDevice();
  } while( GetInputState() );
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
//	GetInputState
//------------------------------------------------------
UINT32 CGamepad::GetInputState( void ) const
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
	UINT32 state = GetInputState();

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
  return ( (GetInputState() & buttonID) == buttonID );
}

//------------------------------------------------------
//	IsOnlyButtonPressed
//------------------------------------------------------
BOOL CGamepad::IsOnlyButtonPressed( UINT32 buttonID ) const
{
  return ( GetInputState() == buttonID );
}

//------------------------------------------------------
//	IsOneOfButtonsPressed
//------------------------------------------------------
BOOL CGamepad::IsOneOfButtonsPressed( UINT32 buttonID ) const
{
  return ( GetInputState() & buttonID );
}


//------------------------------------------------------
//	GetGamepadDeviceState
//------------------------------------------------------
const XINPUT_GAMEPAD *CGamepad::GetGamepadDeviceState( void ) const 
{
  if( IsConnected() )
	  return &m_state.Gamepad;

  return NULL;
}


//------------------------------------------------------
//	SetLightgunCalibration
//------------------------------------------------------
void CGamepad::SetLightgunCalibration( INT32 cx, INT32 cy, INT32 ulx, INT32 uly )
{
  if( m_caps.SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
    return;

  if( IsConnected() )
  {
    XINPUT_LIGHTGUN_CALIBRATION_OFFSETS offsets;
    offsets.wCenterX = cx;
    offsets.wCenterY = cy;
    offsets.wUpperLeftX = ulx;
    offsets.wUpperLeftY = uly;

    XInputSetLightgunCalibration( m_deviceHandle, &offsets );
  }
}

//------------------------------------------------------
//	GetLightgunFlags
//------------------------------------------------------
DWORD CGamepad::GetLightgunFlags( void ) const
{
  if( m_caps.SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
    return 0;
  if( IsConnected() )
    return m_state.Gamepad.wButtons & (XINPUT_LIGHTGUN_ONSCREEN | XINPUT_LIGHTGUN_FRAME_DOUBLER | XINPUT_LIGHTGUN_LINE_DOUBLER );

  return 0;
}

//------------------------------------------------------
//	IsLightgunPointedAtScreen
//------------------------------------------------------
BOOL CGamepad::IsLightgunPointedAtScreen( void ) const
{
  return GetLightgunFlags() & XINPUT_LIGHTGUN_ONSCREEN;
}

//------------------------------------------------------
//	AttachRemoveDevices
//------------------------------------------------------
void CGamepad::AttachRemoveDevices( void ) 
{
		// Attach/Remove gamepads
	AttachRemoveGamepadDevice();

		// Attach/Remove MemUnit sets
	AttachRemoveMemUnitDevicePair();
}

//------------------------------------------------------
//	AttachRemoveGamepadDevice
//------------------------------------------------------
void CGamepad::AttachRemoveGamepadDevice( void )
{
	if( !(m_inputManager->GetGamepadDeviceBitmap() & m_portMask ) && m_deviceHandle )
	{
			// Detach
		XInputClose( m_deviceHandle );
		m_deviceHandle = NULL;
		m_state.dwPacketNumber = 0;
	}

	if(	(m_inputManager->GetGamepadDeviceBitmap() & m_portMask ) && !m_deviceHandle )
	{
			// Attach
		m_deviceHandle = XInputOpen( XDEVICE_TYPE_GAMEPAD,
																							            m_portName,				
																							            XDEVICE_NO_SLOT,			// Gamepad, so no slot
																							            NULL );								// No special polling params
    assert( m_deviceHandle );
    XInputGetCapabilities( m_deviceHandle, &m_caps );
	}
}

//------------------------------------------------------
//	AttachRemoveMemUnitDevicePair
//------------------------------------------------------
void CGamepad::AttachRemoveMemUnitDevicePair( void )
{

		// -- Top --------------------------------
	if( !(m_inputManager->GetMUDeviceBitmap() & m_topMemPortMask ) && m_memunitDeviceHandles[0] )
	{
			// Detach
		XInputClose( m_memunitDeviceHandles[0] );
		m_memunitDeviceHandles[0] = NULL;
	}
	if( (m_inputManager->GetMUDeviceBitmap() & m_topMemPortMask) && !m_memunitDeviceHandles[0] )
	{
			// Attach
		m_memunitDeviceHandles[0] = XInputOpen( XDEVICE_TYPE_MEMORY_UNIT,
																						m_portName,				
																						XDEVICE_TOP_SLOT,			// Gamepad, so no slot
																						NULL );								// No special polling params
	}

		// -- Bottom --------------------------------
	if( !(m_inputManager->GetMUDeviceBitmap() & m_bottomMemPortMask ) && m_memunitDeviceHandles[1] )
	{
			// Detach
		XInputClose( m_memunitDeviceHandles[1] );
		m_memunitDeviceHandles[1] = NULL;
	}
	if( (m_inputManager->GetMUDeviceBitmap() & m_bottomMemPortMask) && !m_memunitDeviceHandles[1] )
	{
			// Attach
		m_memunitDeviceHandles[1] = XInputOpen( XDEVICE_TYPE_MEMORY_UNIT,
																						m_portName,				
																						XDEVICE_BOTTOM_SLOT,			// Gamepad, so no slot
																						NULL );								// No special polling params
	}
}



