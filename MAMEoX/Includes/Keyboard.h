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
    //! \brief    Sets up the keyboard instance, must be
    //!           called before any other functions will become
    //!           valid.
    //!
    //! \param    inputManager - Pointer to the CInputManager parent
		//------------------------------------------------------
  BOOL Create( CInputManager *inputManager ) {
    m_numKeysPressed = 0;
    memset( m_keystrokeVector, 0, sizeof(m_keystrokeVector) );

      // We pass KEYBOARD_AUTOSEARCH_PORT as the gamepad index, as this
      // allows us to just take the first port that contains a keyboard
    return CInputDevice::Create( KEYBOARD_AUTOSEARCH_PORT, inputManager );
  }

		//------------------------------------------------------
		//	PollDevice
    //! \brief    Poll the associated physical device for
    //!           its current state
		//------------------------------------------------------
  virtual void PollDevice( void );

		//------------------------------------------------------
		//	GetNumKeysPressed
    //! \brief    Check the number of keys currently held down
    //!
    //! \return   The number of valid keypresses in the buffer
		//------------------------------------------------------
  UINT32 GetNumKeysPressed( void ) const { return m_numKeysPressed; }

		//------------------------------------------------------
		//	WaitForAnyButton
		//! \brief		Wait for any key to be pressed on the
		//!            keyboard
		//------------------------------------------------------
  void WaitForAnyButton( void );

		//------------------------------------------------------
		//	WaitForNoButton
		//! \brief		Wait for all buttons to be released on the
		//!            keyboard
		//------------------------------------------------------
  void WaitForNoButton( void );

		//------------------------------------------------------
		//	WaitForAnyInput
		//! \brief		Wait for anything to be pressed on
		//!            the keyboard
		//------------------------------------------------------
  void WaitForAnyInput( void ) { WaitForAnyButton(); }

		//------------------------------------------------------
		//	WaitForNoInput
		//! \brief		Wait for everything to be released on the
		//!            keyboard
		//------------------------------------------------------
  void WaitForNoInput( void ) { WaitForNoButton(); }

    //------------------------------------------------------
    //	IsKeyPressed
    //! \brief    Returns TRUE if the given key is pressed 
    //!           on the gamepad
    //!
    //! \param    virtualKeyCode - The virtual keycode to check against
    //!
    //! \return   BOOL - TRUE if key is pressed, else FALSE
    //------------------------------------------------------
  BOOL IsKeyPressed( BYTE virtualKeyCode ) const;

    //------------------------------------------------------
    //	IsOnlyKeyPressed
    //! \brief    Returns TRUE if the given key is pressed 
    //!           on the keyboard, and no other keys are
    //!           pressed
    //!
    //! \param    virtualKeyCode - The virtual keycode to check against
    //!
    //! \return   BOOL - TRUE if key is exclusively pressed, 
    //!                  else FALSE
    //------------------------------------------------------
  BOOL IsOnlyKeyPressed( BYTE virtualKeyCode ) const;

    //------------------------------------------------------
    //	AreAllOfKeysPressed
    //! \brief    Returns TRUE if all of the given keys
    //!           are pressed
    //!
    //! \param    virtualKeyCodeArray - Array of virtual keycodes to check against
    //! \param    numCodes - Number of valid entries in the virtualKeyCodeArray
    //!
    //! \return   BOOL - TRUE if key is pressed, else FALSE
    //------------------------------------------------------
  BOOL AreAllOfKeysPressed( const BYTE *virtualKeyCodeArray, UINT32 numCodes ) const;

    //------------------------------------------------------
    //	IsOneOfKeysPressed
    //! \brief    Returns TRUE if any of the given keys
    //!           is pressed
    //!
    //! \param    virtualKeyCodeArray - Array of virtual keycodes to check against
    //! \param    numCodes - Number of valid entries in the virtualKeyCodeArray
    //!
    //! \return   BOOL - TRUE if key is pressed, else FALSE
    //------------------------------------------------------
  BOOL IsOneOfKeysPressed( const BYTE *virtualKeyCodeArray, UINT32 numCodes ) const;

		//------------------------------------------------------
		//	AttachRemoveDevices
    //! \brief    Attach or remove physical devices at
    //!           the associated gameport
		//------------------------------------------------------
  void AttachRemoveDevices( void );

protected:
  UINT8                     m_keystrokeVector[256 >> 3];  //!< Bit array for the keys
  UINT32                    m_numKeysPressed;           //!< The number of keys that are pressed
};

