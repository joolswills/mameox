/**
	* \file		Keyboard.h
  *         Simple wrapper around the debug keyboard functionality
	*/

#pragma once

//= I N C L U D E S ===========================================================
#include "InputDevice.h"

//= D E F I N E S =============================================================

//= C L A S S E S =============================================================
class CInputManager;

  //! \class    CKeyboard
  //! \brief    Simple wrapper around the debug keyboard functionality
class CKeyboard : public CInputDevice
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
  CKeyboard( void ) : CInputDevice() {}

		//------------------------------------------------------
		//	Create
    //! \brief    Sets up the gamepad instance, must be
    //!           called before any other functions will become
    //!           valid.
    //!
    //! \param    inputManager - Pointer to the CInputManager parent
		//------------------------------------------------------
  BOOL Create( CInputManager *inputManager ) {
    return CInputDevice::Create( 0, inputManager );
  }

		//------------------------------------------------------
		//	PollDevice
    //! \brief    Poll the associated physical device for
    //!           its current state
		//------------------------------------------------------
  virtual void PollDevice( void ) {
    // KB devices are not polled
  }


		//------------------------------------------------------
		//	GetInputState
    //! \brief    Returns the input state of the gamepad
    //!
    //! \return   UINT32 - input state of the gamepad, made up
    //!                    of or'd gamepadButtonID_t values
		//------------------------------------------------------
  UINT32 GetInputState( void ) const {
    return 0;
  }

		//------------------------------------------------------
		//	WaitForAnyButton
		//! \brief		Wait for any button to be pressed on the
		//!            gamepad
		//------------------------------------------------------
  void WaitForAnyButton( void ) {}

		//------------------------------------------------------
		//	WaitForNoButton
		//! \brief		Wait for all buttons to be released on the
		//!            gamepad
		//------------------------------------------------------
  void WaitForNoButton( void ) {}

		//------------------------------------------------------
		//	WaitForAnyInput
		//! \brief		Wait for anything to be pressed/moved on
		//!            the gamepad
		//------------------------------------------------------
  void WaitForAnyInput( void ) {}

		//------------------------------------------------------
		//	WaitForNoInput
		//! \brief		Wait for everything to be released on the
		//!            gamepad
		//------------------------------------------------------
  void WaitForNoInput( void ) {}

		//------------------------------------------------------
		//	AttachRemoveDevices
    //! \brief    Attach or remove physical devices at
    //!           the associated gameport
		//------------------------------------------------------
  void AttachRemoveDevices( void ) {}

  protected:
};

