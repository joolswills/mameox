/**
	* \file		InputManager.h
	*
	*/

#ifndef _INPUTMANAGER_H__
#define _INPUTMANAGER_H__


//= I N C L U D E S ===========================================================
#include <Xtl.h>


//= C L A S S E S =============================================================
class CInputManager
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CInputManager( void ) {
		memset( this, NULL, sizeof(*this) );
	}


		//------------------------------------------------------
		//	Create
    //! Set up the InputManager instance
    //!
    //! \param    maxGamepads - The maximum number of gamepads
    //!                         supported by the app.
    //!
    //! \param    maxMemUnits - The maximum number of memory
    //!                         units supported by the app.
		//------------------------------------------------------
	BOOL Create( DWORD maxGamepads, DWORD maxMemUnits ) {
		if( m_created )
			return FALSE;

		if( !maxGamepads )
			return FALSE;

		m_maxGamepadDevices = maxGamepads;
		m_maxMemUnitDevices = maxMemUnits;

		XDEVICE_PREALLOC_TYPE preallocArray[2] = { { XDEVICE_TYPE_GAMEPAD, 1 },
																							{ XDEVICE_TYPE_MEMORY_UNIT, 1 } };
		preallocArray[0].dwPreallocCount = m_maxGamepadDevices;
		preallocArray[1].dwPreallocCount = m_maxMemUnitDevices;

			// Initialize the input subsystem
		XInitDevices( 2,								// DWORD dwPreallocTypeCount
									preallocArray );	// PXDEVICE_PREALLOC_TYPE PreallocTypes

			// Get the list of devices currently attached to the system
		m_gamepadDeviceBitmap = XGetDevices( XDEVICE_TYPE_GAMEPAD );
		m_memunitDeviceBitmap = XGetDevices( XDEVICE_TYPE_MEMORY_UNIT );

		AttachRemoveDevices();

		return TRUE;
	}

		//------------------------------------------------------
		//	AttachRemoveDevices
		//------------------------------------------------------
	inline void AttachRemoveDevices( void ) {

			// Attach/Remove gamepads
		AttachRemoveGamepadDevice( 0 );
		AttachRemoveGamepadDevice( 1 );
		AttachRemoveGamepadDevice( 2 );
		AttachRemoveGamepadDevice( 3 );

			// Attach/Remove MemUnit sets
		AttachRemoveMemUnitDevicePair( 0 );
		AttachRemoveMemUnitDevicePair( 1 );
		AttachRemoveMemUnitDevicePair( 2 );
		AttachRemoveMemUnitDevicePair( 3 );
	}


		//------------------------------------------------------
		//	PollDevices
		//------------------------------------------------------
	void PollDevices( void ) {
			// 1) Check for insertions/removals
		DWORD insertions = 0;
		DWORD removals = 0;
		XGetDeviceChanges( XDEVICE_TYPE_GAMEPAD,
											 &insertions,                  
											 &removals );

		m_gamepadDeviceBitmap |= insertions;
		m_gamepadDeviceBitmap &= ~removals;

		XGetDeviceChanges( XDEVICE_TYPE_MEMORY_UNIT,
											 &insertions,                  
											 &removals );
		m_memunitDeviceBitmap |= insertions;
		m_memunitDeviceBitmap &= ~removals;

		AttachRemoveDevices();

		if( m_gamepadDeviceHandles[0] )
			XInputGetState( m_gamepadDeviceHandles[0], &m_gamepadDeviceState[0] );

		if( m_gamepadDeviceHandles[1] )
			XInputGetState( m_gamepadDeviceHandles[1], &m_gamepadDeviceState[1] );

		if( m_gamepadDeviceHandles[2] )
			XInputGetState( m_gamepadDeviceHandles[2], &m_gamepadDeviceState[2] );

		if( m_gamepadDeviceHandles[3] )
			XInputGetState( m_gamepadDeviceHandles[3], &m_gamepadDeviceState[3] );
	}


		//------------------------------------------------------
		//	GetGamepadDeviceState
    //! Return the current state of a given gamepad
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   const XINPUT_GAMEPAD & - The requested
    //!                                    gamepad state object
		//------------------------------------------------------
	const XINPUT_GAMEPAD &GetGamepadDeviceState( DWORD device ) const {
		if( device > 3 )
		{
			//assert( 0 && "Invalid gamepad device number!" );
			return m_gamepadDeviceState[0].Gamepad;
		}

		return m_gamepadDeviceState[device].Gamepad;
	}

		//------------------------------------------------------
		//	GetGamepadDeviceCaps
    //! Return the capabilities of a given gamepad
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   const XINPUT_CAPABILITIES & - The requested
    //!                                    gamepad caps object
		//------------------------------------------------------
	const XINPUT_CAPABILITIES &GetGamepadDeviceCaps( DWORD device ) const {
		if( device > 3 )
		{
			//assert( 0 && "Invalid gamepad device number!" );
			return m_gamepadCaps[0];
		}

		return m_gamepadCaps[device];
	}

		//------------------------------------------------------
		//	SetGamepadFeedbackState
    //! Send a force feedback effect to a gamepad
    //!
    //! \param  deviceNumber - The gamepad to send to (0-3)
    //! \param  feedback - Struct describing the effect to send
		//------------------------------------------------------
	inline BOOL SetGamepadFeedbackState( DWORD deviceNumber, const XINPUT_FEEDBACK &feedback ) {
			// Make sure an op isn't already in progress
		if( m_gamepadFeedback[deviceNumber].Header.dwStatus == ERROR_IO_PENDING )
			return FALSE;

			// Store the var to ensure persistency (XInputSetState is async)
		m_gamepadFeedback[deviceNumber] = feedback;
		if( m_gamepadDeviceHandles[deviceNumber] )
		{
			if( XInputSetState( m_gamepadDeviceHandles[deviceNumber], &m_gamepadFeedback[deviceNumber] ) != ERROR_IO_PENDING )
				return FALSE;
		}

		return TRUE;
	}


		//------------------------------------------------------
		//	WaitForAnyKey
		//! \brief		Wait for any button to be pressed on the
		//!            selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForAnyKey( DWORD gamepadNum = 0xFF ) {
		while( 1 )
		{
			PollDevices();

			if( gamepadNum == 0 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[0].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					return;
				}
			}


			if( gamepadNum == 1 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[1].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					return;
				}
			}

			if( gamepadNum == 2 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[2].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					return;
				}
			}

			if( gamepadNum == 3 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[3].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					return;
				}
			}
		}
	}

		//------------------------------------------------------
		//	WaitForNoKey
		//! \brief		Wait for all button to be released on the
		//!            selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForNoKey( DWORD gamepadNum = 0xFF ) {
		BOOL keyPressed = FALSE;
		do
		{
			keyPressed = FALSE;
			PollDevices();

			if( gamepadNum == 0 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[0].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					keyPressed = TRUE;
				}
			}


			if( gamepadNum == 1 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[1].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					keyPressed = TRUE;
				}
			}

			if( gamepadNum == 2 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[2].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					keyPressed = TRUE;
				}
			}

			if( gamepadNum == 3 || gamepadNum == 0xFF )
			{
				const XINPUT_GAMEPAD	&gp = m_gamepadDeviceState[3].Gamepad;
				if( gp.wButtons ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_A] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_B] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_X] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_Y] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 100 ||
						gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 100 )
				{
					keyPressed = TRUE;
				}
			}
		} while( keyPressed );
	}



protected:

		//------------------------------------------------------
		//	AttachRemoveGamepadDevice
		//------------------------------------------------------
	inline void AttachRemoveGamepadDevice( DWORD portNumber ) {
		DWORD portMask, portName;

		switch( portNumber )
		{
		default:
//			assert( 0 && "Invalid gamepad port number!" );
			// fallthrough

			// *** 0 *** //
		case 0:
			portMask = XDEVICE_PORT0_MASK;
			portName = XDEVICE_PORT0;
			break;

			// *** 1 *** //
		case 1:
			portMask = XDEVICE_PORT1_MASK;
			portName = XDEVICE_PORT1;
			break;

			// *** 2 *** //
		case 2:
			portMask = XDEVICE_PORT2_MASK;
			portName = XDEVICE_PORT2;
			break;

			// *** 3 *** //
		case 3:
			portMask = XDEVICE_PORT3_MASK;
			portName = XDEVICE_PORT3;
			break;
		}

		if(	(m_gamepadDeviceBitmap & portMask ) && !m_gamepadDeviceHandles[portNumber] )
		{
				// Attach
			m_gamepadDeviceHandles[portNumber] = XInputOpen( XDEVICE_TYPE_GAMEPAD,
																							         portName,				
																							         XDEVICE_NO_SLOT,			// Gamepad, so no slot
																							         NULL );								// No special polling params
      XInputGetCapabilities( m_gamepadDeviceHandles[portNumber], &m_gamepadCaps[portNumber] );
		}
		else if( !(m_gamepadDeviceBitmap & portMask ) && m_gamepadDeviceHandles[portNumber] )
		{
				// Detach
			XInputClose( m_gamepadDeviceHandles[portNumber] );
			m_gamepadDeviceHandles[portNumber] = NULL;
			m_gamepadDeviceState[portNumber].dwPacketNumber = 0;
		}
	}

		//------------------------------------------------------
		//	AttachRemoveMemUnitDevicePair
		//------------------------------------------------------
	inline void AttachRemoveMemUnitDevicePair( DWORD portNumber ) {
		DWORD topPortMask, bottomPortMask, portName;

		switch( portNumber )
		{
		default:
//			assert( 0 && "Invalid gamepad port number!" );
			// fallthrough

			// *** 0 *** //
		case 0:
			topPortMask = XDEVICE_PORT0_TOP_MASK;
			bottomPortMask = XDEVICE_PORT0_BOTTOM_MASK;
			portName = XDEVICE_PORT0;
			break;

			// *** 1 *** //
		case 1:
			topPortMask = XDEVICE_PORT1_TOP_MASK;
			bottomPortMask = XDEVICE_PORT1_BOTTOM_MASK;
			portName = XDEVICE_PORT1;
			break;

			// *** 2 *** //
		case 2:
			topPortMask = XDEVICE_PORT2_TOP_MASK;
			bottomPortMask = XDEVICE_PORT2_BOTTOM_MASK;
			topPortMask = XDEVICE_PORT0_MASK;
			portName = XDEVICE_PORT2;
			break;

			// *** 3 *** //
		case 3:
			topPortMask = XDEVICE_PORT3_TOP_MASK;
			bottomPortMask = XDEVICE_PORT3_BOTTOM_MASK;
			topPortMask = XDEVICE_PORT0_MASK;
			portName = XDEVICE_PORT3;
			break;
		}

		DWORD top = (portNumber << 1);
		DWORD bottom = top + 1;

			// -- Top --------------------------------
		if( (m_memunitDeviceBitmap & topPortMask) && !m_memunitDeviceHandles[top] )
		{
				// Attach
			m_memunitDeviceHandles[top] = XInputOpen( XDEVICE_TYPE_MEMORY_UNIT,
																								portName,				
																								XDEVICE_TOP_SLOT,			// Gamepad, so no slot
																								NULL );								// No special polling params
		}
		else if( !(m_memunitDeviceBitmap & topPortMask ) && m_memunitDeviceHandles[top] )
		{
				// Detach
			XInputClose( m_memunitDeviceHandles[top] );
			m_memunitDeviceHandles[top] = NULL;
		}

			// -- Bottom --------------------------------
		if( (m_memunitDeviceBitmap & bottomPortMask) && !m_memunitDeviceHandles[bottom] )
		{
				// Attach
			m_memunitDeviceHandles[bottom] = XInputOpen( XDEVICE_TYPE_MEMORY_UNIT,
																								portName,				
																								XDEVICE_BOTTOM_SLOT,			// Gamepad, so no slot
																								NULL );								// No special polling params
		}
		else if( !(m_memunitDeviceBitmap & bottomPortMask ) && m_memunitDeviceHandles[bottom] )
		{
				// Detach
			XInputClose( m_memunitDeviceHandles[bottom] );
			m_memunitDeviceHandles[bottom] = NULL;
		}

	}


	static BOOL			m_created;									//!<	Whether or not this singleton has been created

	DWORD						m_maxGamepadDevices;				//!<	The max number of gamepad devices to poll
	DWORD						m_maxMemUnitDevices;				//!<	The max number of mem units to poll

	DWORD						m_gamepadDeviceBitmap;			//!<	Bitmap storing which gamepad devices are currently attached
	DWORD						m_memunitDeviceBitmap;			//!<	Bitmap storing which mem unit devices are currently attached
	HANDLE					m_gamepadDeviceHandles[4];	//!<	Input handles for gamepads
	HANDLE					m_memunitDeviceHandles[8];	//!<	Input handles for mem units

	XINPUT_STATE		    m_gamepadDeviceState[4];	//!<	Gamepad device state structs
	XINPUT_FEEDBACK     m_gamepadFeedback[4];			//!<	Feedback struct for each gamepad
  XINPUT_CAPABILITIES m_gamepadCaps[4];         //!<  Gamepad device capabilities
};

#endif

