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

//= P R O T O T Y P E S ================================================
	//-------------------------------------
	//	InitializeJoystickMouse
	//! \brief		Initializes the Joystick/Mouse 
	//!            subsystem
	//-------------------------------------
void InitializeJoystickMouse( void );

	//-------------------------------------
	//	TerminateJoystickMouse
	//! \brief		Shuts down the Joystick/Mouse 
	//!            subsystem
	//-------------------------------------
void TerminateJoystickMouse( void );

#ifdef __cplusplus
}
#endif


