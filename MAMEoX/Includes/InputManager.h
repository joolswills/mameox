/**
	* \file		InputManager.h
	*
	*/
#pragma once


//= I N C L U D E S ===========================================================
#include "MAMEoX.h"
#include <xkbd.h>

#include "Gamepad.h"
#include "Keyboard.h"

//= D E F I N E S =============================================================
  // Lighgun calibration step giving the UL corner numbers
#define CALIB_UL    1

  // Lightgun calibration step giving the center numbers
#define CALIB_C     0


//= C L A S S E S =============================================================
class CInputManager
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
  CInputManager( void ) :
    m_maxGamepadDevices( 0 ),
	  m_maxMemUnitDevices( 0 ),
    m_maxKeyboardDevices( 0 ),
	  m_gamepadDeviceBitmap( 0 ),
	  m_memunitDeviceBitmap( 0 ),
    m_keyboardDeviceBitmap( 0 ) {
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
	BOOL Create( DWORD maxGamepads = 4, DWORD maxMemUnits = 0, BOOL useKeyboard = FALSE ) {
		if( m_created )
			return FALSE;

		if( !maxGamepads )
			return FALSE;

		m_maxGamepadDevices = maxGamepads;
		m_maxMemUnitDevices = maxMemUnits;
    m_maxKeyboardDevices = 0;
    m_maxKeyboardDevices = (useKeyboard ? 1 : 0);

		XDEVICE_PREALLOC_TYPE preallocArray[] = { { XDEVICE_TYPE_GAMEPAD, 1 },
																							{ XDEVICE_TYPE_MEMORY_UNIT, 1 },
                                              { XDEVICE_TYPE_DEBUG_KEYBOARD, 0 } };
		preallocArray[0].dwPreallocCount = m_maxGamepadDevices;
		preallocArray[1].dwPreallocCount = m_maxMemUnitDevices;
    preallocArray[2].dwPreallocCount = m_maxKeyboardDevices;

			// Initialize the input subsystem
		XInitDevices( 3,								// DWORD dwPreallocTypeCount
									preallocArray );	// PXDEVICE_PREALLOC_TYPE PreallocTypes

    if( m_maxKeyboardDevices )
    {
        // Set up the keyboard queue
      XINPUT_DEBUG_KEYQUEUE_PARAMETERS kbParams;
      kbParams.dwFlags = XINPUT_DEBUG_KEYQUEUE_FLAG_KEYDOWN | XINPUT_DEBUG_KEYQUEUE_FLAG_KEYREPEAT;
      kbParams.dwQueueSize = 13;
      kbParams.dwRepeatDelay = 100;
      kbParams.dwRepeatInterval = 100;
      XInputDebugInitKeyboardQueue(&kbParams );
    }

			// Get the list of devices currently attached to the system
		m_gamepadDeviceBitmap = XGetDevices( XDEVICE_TYPE_GAMEPAD );
		m_memunitDeviceBitmap = XGetDevices( XDEVICE_TYPE_MEMORY_UNIT );
    m_keyboardDeviceBitmap = XGetDevices( XDEVICE_TYPE_DEBUG_KEYBOARD ); 
    
      // Create the gamepads
    for( UINT32 i = 0; i < 4; ++i )
    {
      m_gamepads[i].Create( i, 
                            maxMemUnits > 2 ? 2 : maxMemUnits, 
                            this );

      if( maxMemUnits > 2 )
        maxMemUnits -= 2;
      else
        maxMemUnits = 0;
    }

      // Create the keyboard
    if( m_maxKeyboardDevices )
      m_keyboard.Create( this );

    PollDevices();

		return TRUE;
	}

		//------------------------------------------------------
		//	AttachRemoveDevices
		//------------------------------------------------------
	inline void AttachRemoveDevices( void ) {
			// Attach/Remove gamepads
    BOOL gpAttached;

    #define ATTACH_AND_RESTORE( gpNum ) \
      gpAttached = m_gamepads[gpNum].IsConnected(); \
      m_gamepads[gpNum].AttachRemoveDevices(); \
      if( !gpAttached && m_gamepads[gpNum].IsConnected() ) {\
        lightgunCalibration_t &calibData = g_calibrationData[gpNum]; \
        m_gamepads[gpNum].SetLightgunCalibration( XINPUT_LIGHTGUN_CALIBRATION_CENTER_X - calibData.m_xData[CALIB_C], \
                                  XINPUT_LIGHTGUN_CALIBRATION_CENTER_Y - calibData.m_yData[CALIB_C], \
                                  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_X - calibData.m_xData[CALIB_UL],  \
                                  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_Y - calibData.m_yData[CALIB_UL] ); \
      }

    ATTACH_AND_RESTORE( 0 );
    ATTACH_AND_RESTORE( 1 );
    ATTACH_AND_RESTORE( 2 );
    ATTACH_AND_RESTORE( 3 );

    if( m_maxKeyboardDevices )
      m_keyboard.AttachRemoveDevices();
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
		m_gamepadDeviceBitmap &= ~removals;
		m_gamepadDeviceBitmap |= insertions;


		XGetDeviceChanges( XDEVICE_TYPE_MEMORY_UNIT,
											 &insertions,                  
											 &removals );
		m_memunitDeviceBitmap &= ~removals;
		m_memunitDeviceBitmap |= insertions;

    if( m_maxKeyboardDevices )
    {
		  XGetDeviceChanges( XDEVICE_TYPE_DEBUG_KEYBOARD,
											  &insertions,                  
											  &removals );
		  m_keyboardDeviceBitmap &= ~removals;
		  m_keyboardDeviceBitmap |= insertions;    
    }

		AttachRemoveDevices();

    m_gamepads[0].PollDevice();
    m_gamepads[1].PollDevice();
    m_gamepads[2].PollDevice();
    m_gamepads[3].PollDevice();

      // Keyboards do not poll
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
      while( !m_gamepads[device].IsConnected() && (m_maxKeyboardDevices ? !m_keyboard.IsConnected() : TRUE) )
        PollDevices();
    }
  }

		//------------------------------------------------------
		//	IsGamepadConnected
    //! Check to see if a given controller is inserted
    //!
    //! \param    device - The device number to query (0-3)
    //!
    //! \retval   BOOL - TRUE if gamepad is inserted, FALSE otherwise
		//------------------------------------------------------
  BOOL IsGamepadConnected( DWORD device ) const {
    if( device > 3 )
      return FALSE;

    return m_gamepads[device].IsConnected();
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

    if( m_gamepads[device].IsConnected() )
      return &m_gamepads[device];

    return NULL;
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
		//	SetFeedbackState
    //! Send a force feedback effect to a gamepad
    //!
    //! \param  deviceNumber - The gamepad to send to (0-3)
    //! \param  feedback - Struct describing the effect to send
		//------------------------------------------------------
	inline BOOL SetDeviceFeedbackState( DWORD deviceNumber, const XINPUT_FEEDBACK &feedback ) {
    if( deviceNumber > 3 )
      return FALSE;

    return m_gamepads[deviceNumber].SetDeviceFeedbackState( feedback );
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
		//! \brief		Wait for all buttons to be released on the
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
        keyPressed |= m_gamepads[0].IsAnyButtonPressed();

			if( gamepadNum == 1 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[1].IsAnyButtonPressed();

			if( gamepadNum == 2 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[2].IsAnyButtonPressed();

			if( gamepadNum == 3 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[3].IsAnyButtonPressed();

		} while( keyPressed );
	}

		//------------------------------------------------------
		//	WaitForAnyInput
		//! \brief		Wait for anything to be pressed/moved 
		//!            on the selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForAnyInput( DWORD gamepadNum = 0xFF ) {
		BOOL keyPressed = FALSE;
		do
		{
			keyPressed = FALSE;
			PollDevices();

			if( gamepadNum == 0 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[0].GetInputState();

			if( gamepadNum == 1 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[1].GetInputState();

			if( gamepadNum == 2 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[2].GetInputState();

			if( gamepadNum == 3 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[3].GetInputState();

		} while( !keyPressed );
	}

		//------------------------------------------------------
		//	WaitForNoInput
		//! \brief		Wait for everything to be released on the
		//!            selected joypad
		//!
		//! \param		gamepadNum - Joypad to test (0xFF = all)
		//------------------------------------------------------
	void WaitForNoInput( DWORD gamepadNum = 0xFF ) {
		BOOL keyPressed = FALSE;
		do
		{
			keyPressed = FALSE;
			PollDevices();

			if( gamepadNum == 0 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[0].GetInputState();

			if( gamepadNum == 1 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[1].GetInputState();

			if( gamepadNum == 2 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[2].GetInputState();

			if( gamepadNum == 3 || gamepadNum == 0xFF )
        keyPressed |= m_gamepads[3].GetInputState();

		} while( keyPressed );
	}

		//------------------------------------------------------
		//	IsAnyButtonPressed
    //! \brief    Returns TRUE if any button is pressed on any gamepad
    //!
    //! \return   BOOL - TRUE if any button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsAnyButtonPressed( void ) {
    PollDevices();
    return  m_gamepads[0].IsAnyButtonPressed() ||
            m_gamepads[1].IsAnyButtonPressed() ||
            m_gamepads[2].IsAnyButtonPressed() ||
            m_gamepads[3].IsAnyButtonPressed();
  }

		//------------------------------------------------------
		//	IsButtonPressed
    //! \brief    Returns TRUE if the given button is pressed 
    //!           on any gamepad
    //!
    //! \return   BOOL - TRUE if button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsButtonPressed( UINT32 buttonID ) {
    PollDevices();
    return  m_gamepads[0].IsButtonPressed(buttonID) ||
            m_gamepads[1].IsButtonPressed(buttonID) ||
            m_gamepads[2].IsButtonPressed(buttonID) ||
            m_gamepads[3].IsButtonPressed(buttonID);
  }

		//------------------------------------------------------
		//	IsOnlyButtonPressed
    //! \brief    Returns TRUE if the given button is pressed 
    //!           on any gamepad, and no other buttons are
    //!           pressed on that gamepad
    //!
    //! \return   BOOL - TRUE if button is exclusively pressed, 
    //!                  else FALSE
		//------------------------------------------------------
  BOOL IsOnlyButtonPressed( UINT32 buttonID ) {
    PollDevices();
    return  m_gamepads[0].IsOnlyButtonPressed(buttonID) ||
            m_gamepads[1].IsOnlyButtonPressed(buttonID) ||
            m_gamepads[2].IsOnlyButtonPressed(buttonID) ||
            m_gamepads[3].IsOnlyButtonPressed(buttonID);
  }

		//------------------------------------------------------
		//	IsOneOfButtonsPressed
    //! \brief    Returns TRUE if any of the given buttons   
    //!           is pressed on any gamepad
    //!
    //! \return   BOOL - TRUE if button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsOneOfButtonsPressed( UINT32 buttonID ) {
    PollDevices();
    return  m_gamepads[0].IsOneOfButtonsPressed(buttonID) ||
            m_gamepads[1].IsOneOfButtonsPressed(buttonID) ||
            m_gamepads[2].IsOneOfButtonsPressed(buttonID) ||
            m_gamepads[3].IsOneOfButtonsPressed(buttonID);
  }

  DWORD GetGamepadDeviceBitmap( void ) const { return m_gamepadDeviceBitmap; }
  DWORD GetMUDeviceBitmap( void ) const { return m_memunitDeviceBitmap; }
  DWORD GetKeyboardDeviceBitmap( void ) const { return m_keyboardDeviceBitmap; }

protected:

	static BOOL			m_created;									//!<	Whether or not this singleton has been created

	DWORD						m_maxGamepadDevices;				//!<	The max number of gamepad devices to poll
	DWORD						m_maxMemUnitDevices;				//!<	The max number of mem units to poll
  DWORD           m_maxKeyboardDevices;       //!<  The max number of keyboards to poll (can only be 0 or 1)

	DWORD						m_gamepadDeviceBitmap;			//!<	Bitmap storing which gamepad devices are currently attached
	DWORD						m_memunitDeviceBitmap;			//!<	Bitmap storing which mem unit devices are currently attached
  DWORD           m_keyboardDeviceBitmap;     //!<	Bitmap storing which keyboard devices are currently attached

  CGamepad        m_gamepads[4];              //!<  The gamepad objects
  CKeyboard       m_keyboard;                 //!<  The keyboard object
};
