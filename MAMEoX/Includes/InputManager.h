/**
	* \file		InputManager.h
	*
	*/

#ifndef _INPUTMANAGER_H__
#define _INPUTMANAGER_H__


//= I N C L U D E S ===========================================================
#include <Xtl.h>
#include "Gamepad.h"


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
    
      // Create the gamepads
    for( UINT32 i = 0; i < 4; ++i )
    {
      m_gamepads[i].Create( i, 
                            maxMemUnits > 2 ? 2 : maxMemUnits, 
                            &m_gamepadDeviceBitmap,
                            &m_memunitDeviceBitmap );
      if( maxMemUnits > 2 )
        maxMemUnits -= 2;
      else
        maxMemUnits = 0;
    }

    PollDevices();

		return TRUE;
	}

		//------------------------------------------------------
		//	AttachRemoveDevices
		//------------------------------------------------------
	inline void AttachRemoveDevices( void ) {
			// Attach/Remove gamepads
    m_gamepads[0].AttachRemoveDevices();
    m_gamepads[1].AttachRemoveDevices();
    m_gamepads[2].AttachRemoveDevices();
    m_gamepads[3].AttachRemoveDevices();
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

    m_gamepads[0].PollDevice();
    m_gamepads[1].PollDevice();
    m_gamepads[2].PollDevice();
    m_gamepads[3].PollDevice();
	}


		//------------------------------------------------------
		//	WaitForControllerInsertion
    //! Wait until a given controller is inserted
    //!
    //! \param    device - The device number to query (0-3)
		//------------------------------------------------------
  void WaitForControllerInsertion( DWORD device ) {
    if( device < 4 )
    {
      while( !m_gamepads[0].IsConnected() )
        PollDevices();
    }
  }

		//------------------------------------------------------
		//	GetGamepad
    //! Return a given gamepad instance
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   CGamepad * - The requested gamepad object
		//------------------------------------------------------
  CGamepad *GetGamepad( DWORD device ) {
		if( device > 3 )
			return NULL;

    return &m_gamepads[device];
  }

		//------------------------------------------------------
		//	GetGamepad (const version)
    //! Return a given gamepad instance
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   const CGamepad * - The requested gamepad object
		//------------------------------------------------------
  const CGamepad *GetGamepad( DWORD device ) const {
		if( device > 3 )
			return NULL;

    return &m_gamepads[device];
  }

		//------------------------------------------------------
		//	GetGamepadDeviceState
    //! Return the current state of a given gamepad
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   const XINPUT_GAMEPAD * - The requested
    //!                                    gamepad state object
		//------------------------------------------------------
	const XINPUT_GAMEPAD *GetGamepadDeviceState( DWORD device ) const {
		if( device > 3 )
			return NULL;

    return m_gamepads[device].GetGamepadDeviceState();
	}

		//------------------------------------------------------
		//	GetGamepadDeviceCaps
    //! Return the capabilities of a given gamepad
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   const XINPUT_CAPABILITIES * - The requested
    //!                                    gamepad caps object
		//------------------------------------------------------
	const XINPUT_CAPABILITIES *GetGamepadDeviceCaps( DWORD device ) const {
		if( device > 3 )
			return NULL;

    return m_gamepads[device].GetGamepadDeviceCaps();
	}

		//------------------------------------------------------
		//	SetGamepadFeedbackState
    //! Send a force feedback effect to a gamepad
    //!
    //! \param  deviceNumber - The gamepad to send to (0-3)
    //! \param  feedback - Struct describing the effect to send
		//------------------------------------------------------
	inline BOOL SetGamepadFeedbackState( DWORD deviceNumber, const XINPUT_FEEDBACK &feedback ) {
    if( deviceNumber > 3 )
      return FALSE;

    return m_gamepads[deviceNumber].SetGamepadFeedbackState( feedback );
	}


		//------------------------------------------------------
		//	WaitForAnyButton
		//! \brief		Wait for any button to be pressed on the
		//!            selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForAnyButton( DWORD gamepadNum = 0xFF ) {
		while( 1 )
		{
			PollDevices();

			if( ((gamepadNum == 0 || gamepadNum == 0xFF) && m_gamepads[0].IsAnyButtonPressed()) ||
          ((gamepadNum == 1 || gamepadNum == 0xFF) && m_gamepads[1].IsAnyButtonPressed()) ||
          ((gamepadNum == 2 || gamepadNum == 0xFF) && m_gamepads[2].IsAnyButtonPressed()) ||
          ((gamepadNum == 3 || gamepadNum == 0xFF) && m_gamepads[3].IsAnyButtonPressed()) )
        return;
		}
	}

		//------------------------------------------------------
		//	WaitForNoButton
		//! \brief		Wait for all button to be released on the
		//!            selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForNoButton( DWORD gamepadNum = 0xFF ) {
		BOOL keyPressed = FALSE;
		do
		{
			keyPressed = FALSE;
			PollDevices();

			if( gamepadNum == 0 || gamepadNum == 0xFF )
        keyPressed = m_gamepads[0].IsAnyButtonPressed();

			if( gamepadNum == 1 || gamepadNum == 0xFF )
        keyPressed = m_gamepads[1].IsAnyButtonPressed();

			if( gamepadNum == 2 || gamepadNum == 0xFF )
        keyPressed = m_gamepads[2].IsAnyButtonPressed();

			if( gamepadNum == 3 || gamepadNum == 0xFF )
        keyPressed = m_gamepads[3].IsAnyButtonPressed();

		} while( keyPressed );
	}



protected:

	static BOOL			m_created;									//!<	Whether or not this singleton has been created

	DWORD						m_maxGamepadDevices;				//!<	The max number of gamepad devices to poll
	DWORD						m_maxMemUnitDevices;				//!<	The max number of mem units to poll

	DWORD						m_gamepadDeviceBitmap;			//!<	Bitmap storing which gamepad devices are currently attached
	DWORD						m_memunitDeviceBitmap;			//!<	Bitmap storing which mem unit devices are currently attached

  CGamepad        m_gamepads[4];
};

#endif

