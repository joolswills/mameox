/**
	* \file			xbox_Keyboard.h
	* \brief		Declarations for the "Keyboard" section of
	*           osdepend.h
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
	//--------------------------------------------------------------------------
	//	InitializeKeyboard
	//! \brief		Initializes the MAME keyboard subsystem
  //!
  //! \note     Due to the way that joystick devices are passed to MAME,
  //!           this function should be called only in 
  //!           osd_customize_inputport_defaults()
	//--------------------------------------------------------------------------
void InitializeKeyboard( void );

#ifdef __cplusplus
}
#endif


