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



  // Build string
#ifdef _VC6
#define BUILD_STRING         "::VC6"
#define LBUILD_STRING        L"::VC6"
#else
#define BUILD_STRING         "::VC7"
#define LBUILD_STRING        L"::VC7"
#endif

  // Version number
#define VERSION_STRING        "0.64b" BUILD_STRING
#define LVERSION_STRING       L"0.64b" LBUILD_STRING

#define LMAMEVERSION_STRING   L"0.68"

  //! \enum   mameoxLaunchCommand
  //! \brief  Determines what the MAMEoX task should do
typedef enum mameoxLaunchCommand
{
  LAUNCH_CREATE_MAME_GAME_LIST = 0x01,    //!<  Write the MAME supported game file and return
  LAUNCH_RUN_GAME                         //!<  Run the gameIndex and return
} mameoxLaunchCommand;

//= S T R U C T U R E S ================================================
typedef struct MAMEoXLaunchData_t
{
  mameoxLaunchCommand   m_command;        //!<  The function which should be performed by the MAMEoX task
  DWORD                 m_totalMAMEGames; //!<  The total number of games supported by the MAME core
  DWORD                 m_gameIndex;      //!<  The index of the currently selected game
  FLOAT                 m_cursorPosition;
  FLOAT                 m_pageOffset;
} MAMEoXLaunchData_t;

typedef struct MAMEDriverData_t
{
  DWORD m_index;          //!<  The index of this driver in the sorted list
  char  *m_romFileName;   //!<  Main ROM zip file name
  char  *m_description;   //!<  Description from the MAME core
  BOOL  m_isClone;        //!<  Whether or not the game is a clone
} MAMEDriverData_t;

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

	//-------------------------------------------------------------------
	//	LoadOptions
	//! \brief		Load options from the INI file
	//-------------------------------------------------------------------
void LoadOptions( void );

	//-------------------------------------------------------------------
	//	SaveOptions
	//! \brief		Store options to the INI file
	//-------------------------------------------------------------------
void SaveOptions( void );


	//-------------------------------------------------------------------
	//	ShowLoadingScreen
	//! \brief		Display a persistent loading screen for use when
  //!           switching between XBEs
	//-------------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice );

#ifdef __cplusplus
}
#endif


