/**
	* \file			xbox_JoystickMouse.h
	* \brief		Declarations for the Joystick/Mouse subsystem
	*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

//= D E F I N E S ======================================================

	// macros for building/mapping keycodes
#define JOYCODE(joy, type, index)	      ((index) | ((type) << 8) | ((joy) << 12))
#define JOYINDEX(joycode)			          ((joycode) & 0xff)
#define JT(joycode)			                (((joycode) >> 8) & 0xf)
#define JOYNUM(joycode)				          (((joycode) >> 12) & 0xf)

  //! Macros for converting non-standard internal axis/button codes to MAME ones 
  //!  (only use them for things that are not mapped to an entry in input.h)
#define AXISCODE( _stick__, _axis__ )   joyoscode_to_code( JOYCODE( _stick__, _axis__, 0 ) )
#define BUTTONCODE( _stick__, _ID__ )   joyoscode_to_code( JOYCODE( _stick__, JT_BUTTON, _ID__ ) )

#define STDCODE( cde )    (stickIndex == 0 ? JOYCODE_1_##cde : (stickIndex == 1 ? JOYCODE_2_##cde : (stickIndex == 2 ? JOYCODE_3_##cde : JOYCODE_4_##cde )))


  //! \enum       XBOXJoyType
	//! \brief      Defines a logical type for every available button/axis
typedef enum XBOXJoyType {
	JT_LSTICK_UP = 0,
	JT_LSTICK_DOWN,
	JT_LSTICK_LEFT,
	JT_LSTICK_RIGHT,
	JT_RSTICK_UP,
	JT_RSTICK_DOWN,        // 5
	JT_RSTICK_LEFT,
	JT_RSTICK_RIGHT,
	JT_DPAD_UP,
	JT_DPAD_DOWN,
	JT_DPAD_LEFT,          // 10
	JT_DPAD_RIGHT,
	JT_BUTTON
} XBOXJoyType;

typedef enum ButtonID {
  BUTTON_A = 0,
  BUTTON_X,
  BUTTON_B,
  BUTTON_Y,
  BUTTON_LEFT_TRIGGER,
  BUTTON_RIGHT_TRIGGER,
  BUTTON_START,
  BUTTON_BACK,
  BUTTON_WHITE,
  BUTTON_BLACK,           
  BUTTON_LA,              // Left analog pressed
  BUTTON_RA               // Right analog pressed
};


//= P R O T O T Y P E S ================================================
	//--------------------------------------------------------------------------
	//	InitializeJoystickMouse
	//! \brief		Initializes the Joystick/Mouse subsystem
  //!
  //! \note     Due to the way that the joystick device is passed to MAME,
  //!           this function should be called only in 
  //!           osd_customize_inputport_defaults()
	//--------------------------------------------------------------------------
void InitializeJoystickMouse( void );

	//--------------------------------------------------------------------------
	//	TerminateJoystickMouse
	//! \brief		Shuts down the Joystick/Mouse subsystem
	//--------------------------------------------------------------------------
void TerminateJoystickMouse( void );

#ifdef __cplusplus
}
#endif


