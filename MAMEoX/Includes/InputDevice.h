/**
	* \file		InputDevice.h
  *         Virtual base class providing input device functions
	*/

#pragma once

//= I N C L U D E S ===========================================================
#include "MAMEoX.h"
#include "osd_cpu.h"


//= D E F I N E S =============================================================
  // Dummy port index to allow the keyboard to be inserted into any port
#define KEYBOARD_AUTOSEARCH_PORT      0xFF

//= C L A S S E S =============================================================
class CInputManager;

  //! \class    CInputDevice
  //! \brief    Virtual base class providing input device functions
class CInputDevice
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
  CInputDevice( void ) :
    m_inputManager( NULL ),
    m_portIndex( 0 ),
    m_portMask( 0 ), 
    m_portName( 0 ),
    m_deviceHandle( NULL )
  {
    memset( &m_state, 0, sizeof(m_state) );
	  memset( &m_feedback, 0, sizeof(m_feedback) );
    memset( &m_caps, 0, sizeof(m_caps) );
  }

		//------------------------------------------------------
		//	Create
    //! \brief    Sets up the device instance, must be
    //!           called before any other functions will become
    //!           valid.
    //!
    //! \param    gpIndex - 0 based index of this device
    //! \param    inputManager - Pointer to the CInputManager parent
		//------------------------------------------------------
  virtual BOOL Create( UINT32 gpIndex, CInputManager *inputManager ) {
    if( inputManager )
    {
      m_inputManager = inputManager;
      m_portIndex = gpIndex;

	    switch( m_portIndex )
	    {
	    default:
    //			assert( 0 && "Invalid gamepad port number!" );
		    // fallthrough

          // Special case for keyboard "autosearch" (there
          // can be only one keyboard, but it can go into
          // any port)
      case KEYBOARD_AUTOSEARCH_PORT:

		    // *** 0 *** //
	    case 0:
		    m_portName          = XDEVICE_PORT0;
		    m_portMask          = XDEVICE_PORT0_MASK;
		    break;

		    // *** 1 *** //
	    case 1:
		    m_portName          = XDEVICE_PORT1;
		    m_portMask          = XDEVICE_PORT1_MASK;
		    break;

		    // *** 2 *** //
	    case 2:
		    m_portName          = XDEVICE_PORT2;
		    m_portMask          = XDEVICE_PORT2_MASK;
		    break;

		    // *** 3 *** //
	    case 3:
		    m_portName          = XDEVICE_PORT3;
		    m_portMask          = XDEVICE_PORT3_MASK;
		    break;
	    }

      return TRUE;
    }
    return FALSE;
  }

		//------------------------------------------------------
		//	PollDevice
    //! \brief    Poll the associated physical device for
    //!           its current state
		//------------------------------------------------------
	virtual void PollDevice( void ) = 0;

		//------------------------------------------------------
		//	AttachRemoveDevices
    //! \brief    Attach or remove physical devices at
    //!           the associated gameport
		//------------------------------------------------------
	virtual void AttachRemoveDevices( void ) = 0;


		//------------------------------------------------------
		//	IsConnected
    //! \brief    Checks to see if this device is connected
    //!
    //! \return   BOOL - TRUE if the device is connected
		//------------------------------------------------------
  virtual BOOL IsConnected( void ) const {
    return m_deviceHandle != NULL;
  }

		//------------------------------------------------------
		//	WaitForAnyInput
		//! \brief		Wait for anything to be pressed/moved on
		//!            the device
		//------------------------------------------------------
	virtual void WaitForAnyInput( void ) = 0;

		//------------------------------------------------------
		//	WaitForNoInput
		//! \brief		Wait for everything to be released on the
		//!            device
		//------------------------------------------------------
  virtual void WaitForNoInput( void ) = 0;

		//------------------------------------------------------
		//	SetDeviceFeedbackState
    //! Send a force feedback effect to a device
    //!
    //! \param  feedback - Struct describing the effect to send
		//------------------------------------------------------
	inline BOOL SetDeviceFeedbackState( const XINPUT_FEEDBACK &feedback ) {

			// Make sure an op isn't already in progress
		if( m_feedback.Header.dwStatus == ERROR_IO_PENDING )
			return FALSE;

			// Store the var to ensure persistency (XInputSetState is async)
		m_feedback = feedback;
		if( m_deviceHandle )
		{
			if( XInputSetState( m_deviceHandle, &m_feedback ) != ERROR_IO_PENDING )
				return FALSE;
		}

		return TRUE;
	}

		//------------------------------------------------------
		//	GetDeviceCaps
    //! Return the capabilities of this device
    //!
    //! \retval   const XINPUT_CAPABILITIES * - The requested
    //!                                         caps object
		//------------------------------------------------------
  const XINPUT_CAPABILITIES *GetDeviceCaps( void ) const {
	  if( IsConnected() )
      return &m_caps;

    return NULL;
  }

  protected:

  CInputManager       *m_inputManager;

  DWORD               m_portIndex;              //!<  Port this device is connected to
	DWORD               m_portMask, m_portName;
	HANDLE					    m_deviceHandle;	          //!<	Input handle for this device


	XINPUT_STATE		    m_state;	      //!<	device device state struct
	XINPUT_FEEDBACK     m_feedback;			//!<	Feedback struct
  XINPUT_CAPABILITIES m_caps;         //!<  device device capabilities
};

