/**
	* \file		Gamepad.h
  *         Simple wrapper around the XINPUT_STATE and XINPUT_CAPABILITIES
  *         structures, providing utility functions such as IsButtonPressed
	*/

#pragma once

//= I N C L U D E S ===========================================================
#include <Xtl.h>
#include "osd_cpu.h"


//= D E F I N E S =============================================================
#define BUTTON_PRESS_THRESHOLD      50    //!< Press threshold for analog buttons

  //! \enum   gamepadButtonID_t 
  //! \brief  Gamepad Button ID's
typedef enum gamepadButtonID_t {
  GP_DPAD_UP          = 1,
  GP_DPAD_RIGHT       = (1<<1),
  GP_DPAD_DOWN        = (1<<2),
  GP_DPAD_LEFT        = (1<<3),
  GP_BACK             = (1<<4),
  GP_START            = (1<<5),
  GP_LEFT_ANALOG      = (1<<6),   //!<  Left analog button
  GP_RIGHT_ANALOG     = (1<<7),   //!<  Right analog button
  GP_A                = (1<<8),
  GP_B                = (1<<9),
  GP_X                = (1<<10),
  GP_Y                = (1<<11),
  GP_WHITE            = (1<<12),
  GP_BLACK            = (1<<13),
  GP_LEFT_TRIGGER     = (1<<14),
  GP_RIGHT_TRIGGER    = (1<<15),
  GP_LA_UP            = (1<<16),
  GP_LA_RIGHT         = (1<<17),
  GP_LA_DOWN          = (1<<18),
  GP_LA_LEFT          = (1<<19),
  GP_RA_UP            = (1<<20),
  GP_RA_RIGHT         = (1<<21),
  GP_RA_DOWN          = (1<<22),
  GP_RA_LEFT          = (1<<23),

  GP_DPAD_MASK        = GP_DPAD_UP | GP_DPAD_DOWN | GP_DPAD_LEFT | GP_DPAD_RIGHT,
  GP_LA_MASK          = GP_LA_UP | GP_LA_DOWN | GP_LA_LEFT | GP_LA_RIGHT,
  GP_RA_MASK          = GP_RA_UP | GP_RA_DOWN | GP_RA_LEFT | GP_RA_RIGHT
} gamepadButtonID_t;


typedef enum gamepadAnalogID_t {
  GP_ANALOG_LEFT = 0x00,
  GP_ANALOG_RIGHT
} gamepadAnalogID_t;

typedef enum gamepadAxisID_t {
  GP_AXIS_X = 0x00,
  GP_AXIS_Y
} gamepadAxisID_t;


//= C L A S S E S =============================================================
class CInputManager;

  //! \class    CGamepad
  //! \brief    Simple wrapper around the XINPUT_STATE and XINPUT_CAPABILITIES
  //!           structures, providing utility functions such as IsButtonPressed
class CGamepad
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CGamepad( void );

		//------------------------------------------------------
		//	Create
    //! \brief    Sets up the gamepad instance, must be
    //!           called before any other functions will become
    //!           valid.
    //!
    //! \param    gpIndex - 0 based index of this gamepad
    //! \param    maxMemUnits - Max number of mem units for this gp 
    //! \param    inputManager - Pointer to the CInputManager parent
		//------------------------------------------------------
	BOOL Create(  DWORD gpIndex, 
                DWORD maxMemUnits,
                CInputManager *inputManager );

		//------------------------------------------------------
		//	IsConnected
    //! \brief    Checks to see if this gamepad is connected
    //!
    //! \return   BOOL - TRUE if the gamepad is connected
		//------------------------------------------------------
  BOOL IsConnected( void ) const;

		//------------------------------------------------------
		//	IsMUConnected
    //! \brief    Returns TRUE if a memory unit is inserted
    //!           in the requested slot
    //!
    //! \param    bottomMU - Selects the slot to check (top or bottom)
    //!
    //! \return   BOOL - TRUE if an MU is inserted
		//------------------------------------------------------
  BOOL IsMUConnected( BOOL bottomMU = FALSE ) const;

		//------------------------------------------------------
		//	GetAnalogButtonState
    //! \brief    Returns the selected analog button value
    //!
    //! \param    buttonID - ID of the button to query
    //!
    //! \return   UINT8 - Selected axis position value
		//------------------------------------------------------
  UINT8 GetAnalogButtonState( gamepadButtonID_t buttonID ) const;

		//------------------------------------------------------
		//	GetAnalogAxisState
    //! \brief    Returns the selected analog axis position
    //!
    //! \param    analogID - ID of the analog joystick to query
    //! \param    axisID - ID of the axis value to be returned
    //!
    //! \return   SHORT - Selected axis position value
		//------------------------------------------------------
  SHORT GetAnalogAxisState( gamepadAnalogID_t analogID, gamepadAxisID_t axisID ) const;

		//------------------------------------------------------
		//	GetButtonState
    //! \brief    Returns button state of the gamepad
    //!
    //! \return   UINT32 - button state of the gamepad
		//------------------------------------------------------
  UINT32 GetButtonState( void ) const;

		//------------------------------------------------------
		//	IsAnyButtonPressed
    //! \brief    Returns TRUE if any button is pressed on the gamepad
    //!
    //! \return   BOOL - TRUE if any button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsAnyButtonPressed( void ) const;

		//------------------------------------------------------
		//	IsButtonPressed
    //! \brief    Returns TRUE if the given button is pressed 
    //!           on the gamepad
    //!
    //! \return   BOOL - TRUE if button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsButtonPressed( UINT32 buttonID ) const;

		//------------------------------------------------------
		//	IsOnlyButtonPressed
    //! \brief    Returns TRUE if the given button is pressed 
    //!           on the gamepad, and no other buttons are
    //!           pressed
    //!
    //! \return   BOOL - TRUE if button is exclusively pressed, 
    //!                  else FALSE
		//------------------------------------------------------
  BOOL IsOnlyButtonPressed( UINT32 buttonID ) const;

		//------------------------------------------------------
		//	IsOneOfButtonsPressed
    //! \brief    Returns TRUE if any of the given buttons   
    //!           is pressed on the gamepad
    //!
    //! \return   BOOL - TRUE if button is pressed, else FALSE
		//------------------------------------------------------
  BOOL IsOneOfButtonsPressed( UINT32 buttonID ) const;

		//------------------------------------------------------
		//	WaitForAnyButton
		//! \brief		Wait for any button to be pressed on the
		//!            gamepad
		//------------------------------------------------------
	void WaitForAnyButton( void );

		//------------------------------------------------------
		//	WaitForNoButton
		//! \brief		Wait for all buttons to be released on the
		//!            gamepad
		//------------------------------------------------------
	void WaitForNoButton( void );

		//------------------------------------------------------
		//	SetGamepadFeedbackState
    //! Send a force feedback effect to a gamepad
    //!
    //! \param  feedback - Struct describing the effect to send
		//------------------------------------------------------
	inline BOOL SetGamepadFeedbackState( const XINPUT_FEEDBACK &feedback ) {

			// Make sure an op isn't already in progress
		if( m_feedback.Header.dwStatus == ERROR_IO_PENDING )
			return FALSE;

			// Store the var to ensure persistency (XInputSetState is async)
		m_feedback = feedback;
		if( m_gamepadDeviceHandle )
		{
			if( XInputSetState( m_gamepadDeviceHandle, &m_feedback ) != ERROR_IO_PENDING )
				return FALSE;
		}

		return TRUE;
	}

		//------------------------------------------------------
		//	PollDevice
    //! \brief    Poll the associated physical device for
    //!           its current state
		//------------------------------------------------------
	void PollDevice( void );

		//------------------------------------------------------
		//	AttachRemoveDevices
    //! \brief    Attach or remove physical devices at
    //!           the associated gameport
		//------------------------------------------------------
	void AttachRemoveDevices( void );

		//------------------------------------------------------
		//	GetGamepadDeviceState
    //! Return the current state of a given gamepad
    //!
    //! \retval   const XINPUT_GAMEPAD * - The requested
    //!                                    gamepad state object
		//------------------------------------------------------
	const XINPUT_GAMEPAD *GetGamepadDeviceState( void ) const;

		//------------------------------------------------------
		//	GetGamepadDeviceCaps
    //! Return the capabilities of a given gamepad
    //!
    //! \retval   const XINPUT_CAPABILITIES * - The requested
    //!                                    gamepad caps object
		//------------------------------------------------------
	const XINPUT_CAPABILITIES *GetGamepadDeviceCaps( void ) const;



  protected:
		//------------------------------------------------------
		//	AttachRemoveGamepadDevice
		//------------------------------------------------------
	void AttachRemoveGamepadDevice( void );

		//------------------------------------------------------
		//	AttachRemoveMemUnitDevicePair
		//------------------------------------------------------
	inline void AttachRemoveMemUnitDevicePair( void );

  CInputManager       *m_inputManager;

	DWORD               m_portMask, m_portName;
	DWORD               m_topMemPortMask, m_bottomMemPortMask;

  DWORD               m_gamepadIndex;             //!<  Index of this gamepad
	HANDLE					    m_gamepadDeviceHandle;	    //!<	Input handles for gamepads
	HANDLE					    m_memunitDeviceHandles[2];	//!<	Input handles for mem units


	XINPUT_STATE		    m_state;	      //!<	Gamepad device state struct
	XINPUT_FEEDBACK     m_feedback;			//!<	Feedback struct
  XINPUT_CAPABILITIES m_caps;         //!<  Gamepad device capabilities

};

