/**
	* \file			MAMEoX.h
	* \brief		Global utility functions
	*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"
#include <Xtl.h>

//= D E F I N E S ======================================================
#ifdef _DEBUG
#define CHECKRAM()    CheckRAM()
#else
#define CHECKRAM()
#endif

//= P R O T O T Y P E S ================================================
	//-------------------------------------------------------------------
	//	GetGamepadState
	//! \brief		Returns the XINPUT_GAMEPAD containing state
	//!            info for joynum
	//!
	//! \param		joynum - The gamepad index (0-3)
	//!
	//! \return		const XINPUT_GAMEPAD * - Current state
	//! \retval		NULL - Operation failed
	//! \retval		other - current state
	//-------------------------------------------------------------------
const XINPUT_GAMEPAD *GetGamepadState( UINT32 joynum );

	//-------------------------------------------------------------------
	//	PollGamepads
	//! \brief		Polls the gamepads
	//-------------------------------------------------------------------
void PollGamepads( void );

	//-------------------------------------------------------------------
	//	BeginFontRender
	//! \brief		Set up a font rendering batch
  //!
  //! \param    clearScreen - Whether or not to clear the backbuffer to black
	//-------------------------------------------------------------------
void BeginFontRender( BOOL clearScreen );

	//-------------------------------------------------------------------
	//	FontRender
	//! \brief		Render some text to the backbuffer
  //!
  //! \param    x - The x coordinate to begin rendering at
  //! \param    y - The y coordinate to begin rendering at
  //! \param    color - The color (RGBA) to render with
  //! \param    str - The string to be rendered
  //! \param    flags - Positional flags (see xbfont.h)
	//-------------------------------------------------------------------
void FontRender( INT32 x, INT32 y, UINT32 color, const WCHAR *str, UINT32 flags );

	//-------------------------------------------------------------------
	//	EndFontRender
	//! \brief		Flush the font rendering batch
	//-------------------------------------------------------------------
void EndFontRender( void );

	//-------------------------------------------------------------------
	//	WaitForKey
	//! \brief		Block until any key is pressed
	//-------------------------------------------------------------------
void WaitForKey( void );

	//-------------------------------------------------------------------
	//	WaitForNoKey
	//! \brief		Block until all keys are released
	//-------------------------------------------------------------------
void WaitForNoKey( void );


#ifdef _DEBUG
	//-------------------------------------------------------------------
	//	CheckRAM
	//! \brief		Checks the system RAM and prints it out
	//-------------------------------------------------------------------
void CheckRAM( void );
#endif

#ifdef __cplusplus
}
#endif


