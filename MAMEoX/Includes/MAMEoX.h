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
#include "DebugLogger.h"
#include <Xtl.h>

//= D E F I N E S ======================================================
#ifdef _DEBUG
#define CHECKRAM()    CheckRAM()
#define DEBUGGERCHECKRAM() {\
                              MEMORYSTATUS memStatus;\
                              GlobalMemoryStatus(  &memStatus );\
                              PRINTMSG( T_INFO, "Memory: %lu/%lu",memStatus.dwAvailPhys, memStatus.dwTotalPhys );\
                            }
#else
#define CHECKRAM()
#define DEBUGGERCHECKRAM()
#endif

#ifdef _DEBUG
  #define LBUILDCONFIG_STRING   L"[DEBUG]"
#else
  #define LBUILDCONFIG_STRING   L""
#endif

  // Version number
#define RELEASE_STRING            ".1b"
#define LRELEASE_STRING           L".1b"

#define VERSION_STRING            "0.71" RELEASE_STRING
#define LVERSION_STRING           L"0.71" LRELEASE_STRING


#define DRIVERLIST_FILESTAMP      "MAMEoX" VERSION_STRING

  //! \enum   mameoxLaunchCommand
  //! \brief  Determines what the MAMEoX task should do
typedef enum mameoxLaunchCommand
{
  LAUNCH_CREATE_MAME_GAME_LIST = 0x01,    //!<  Write the MAME supported game file and return
  LAUNCH_RUN_GAME,                        //!<  Run the gameIndex and return
  LAUNCH_RUN_AS_IF_REBOOTED               //!<  MAMEoXLauncher should act as though the system was totally rebooted
} mameoxLaunchCommand;

  //! \enum   fonttype
  //! \brief  Font type enumeration
typedef enum fonttype
{
  FONTTYPE_DEFAULT = 0x00,
  FONTTYPE_FIXEDWIDTH,
  FONTTYPE_SMALLTHIN,
  FONTTYPE_LARGETHIN
} fonttype;

typedef enum ROMListSortMode
{
  SM_BYNAME = 0x00,
  SM_BYMANUFACTURER,
  SM_BYYEAR,
  SM_BYPARENT,
  SM_BYGENRE,
  SM_BYNUMPLAYERS
} ROMListSortMode;

//= S T R U C T U R E S ================================================
typedef struct MAMEoXLaunchData_t
{
  mameoxLaunchCommand   m_command;                      //!<  The function which should be performed by the MAMEoX task
  DWORD                 m_totalMAMEGames;               //!<  The total number of games supported by the MAME core
  DWORD                 m_gameIndex;                    //!<  The index of the currently selected game
  DWORD                 m_driverListGenerationAttempts; //!<  The number of attempts made at generating the driver list

  float                 m_cursorPosition;     //!<  Cursor position within the ROM list
  float                 m_pageOffset;         //!<  Page offset within the ROM list
  UINT32                m_superscrollIndex;   //!<  Superscroll index for the ROM list
} MAMEoXLaunchData_t;

typedef struct MAMEDriverData_t
{
  DWORD m_index;          //!<  The index of this driver in the sorted list
  char  *m_romFileName;   //!<  Main ROM zip file name
  char  *m_description;   //!<  Description from the MAME core
  BOOL  m_isClone;        //!<  Whether or not the game is a clone
	char  *m_cloneFileName;	//!<  Clone ROM zip file name
	char  *m_manufacturer;	//!<  Manufacturer name
	char  *m_year;					//!<  Year
} MAMEDriverData_t;

typedef struct lightgunCalibration_t
{
  SHORT   m_xData[3];   //!< X-Axis calibration data (left,center,right)
  SHORT   m_yData[3];   //!< Y-Axis calibration data (top,center,bottom)
} lightgunCalibration_t;

  //! \struct  Token class for RenderToTexture functions
typedef struct RenderToTextureToken_t
{
  LPDIRECT3DSURFACE8  m_backBuffer;
  LPDIRECT3DSURFACE8  m_zBuffer;
  LPDIRECT3DSURFACE8  m_textureSurface;
  LPDIRECT3DDEVICE8   m_pD3DDevice;
  D3DVIEWPORT8        m_viewPoint;
  LPDIRECT3DTEXTURE8  m_texture;
} RenderToTextureToken_t;

typedef struct ROMListOptions_t
{
  BOOL              m_verboseMode;      //!<  Whether or not to show verbose info (manufacturer, year, etc...)
  BOOL              m_displayClones;    //!<  Whether or not to show clones
  ROMListSortMode   m_sortMode;         //!<  The current sort mode
} ROMListOptions_t;


//= G L O B A L = V A R S ==============================================
extern lightgunCalibration_t    g_calibrationData[4];     //!< Lightgun calibration data
extern ROMListOptions_t         g_romListOptions;         //!<  ROM List display options that persist via the INI
extern MAMEoXLaunchData_t       g_persistentLaunchData;   //!<  Launch data that persists via the INI

//= P R O T O T Y P E S ================================================
  //-------------------------------------------------------------------
	//	RequireController
	//! \brief		Notifies the user that the given controller
  //!           must be inserted, and waits until it is
	//!
	//! \param		joynum - The gamepad index (0-3)
  //-------------------------------------------------------------------
void RequireController( DWORD joynum );

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
	//	GetGamepadCaps
	//! \brief		Returns the XINPUT_CAPABILITIES containing
	//!            info about joynum
	//!
	//! \param		joynum - The gamepad index (0-3)
	//!
	//! \return		const XINPUT_CAPABILITIES * - Capabilities
	//! \retval		NULL - Operation failed
	//! \retval		other - current state
	//-------------------------------------------------------------------
const XINPUT_CAPABILITIES *GetGamepadCaps( UINT32 joynum );

	//-------------------------------------------------------------------
	//	PollGamepads
	//! \brief		Polls the gamepads
	//-------------------------------------------------------------------
void PollGamepads( void );

	//-------------------------------------------------------------------
	//	GetLightgunCalibratedPosition
	//! \brief		Returns the calibrated position of the selected lightgun
  //!            mapped to [-128,128]
  //!
  //! \param    joynum - The 0 based index of the gameport containing a lightgun
  //! \param    x - [OUT] variable to receive the X position
  //! \param    y - [OUT] variable to receive the Y position
	//-------------------------------------------------------------------
void GetLightgunCalibratedPosition( UINT32 joynum, INT32 *x, INT32 *y );

	//-------------------------------------------------------------------
	//	BeginFontRender
	//! \brief		Set up a font rendering batch
  //!
  //! \param    clearScreen - Whether or not to clear the backbuffer to black
  //! \param    fontType - The font to use
	//-------------------------------------------------------------------
void BeginFontRender( BOOL clearScreen, fonttype fontType );

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
  //! \param    present - Whether or not to present the backbuffer
	//-------------------------------------------------------------------
void EndFontRender( BOOL present );

	//-------------------------------------------------------------------
	//	WaitForAnyButton
	//! \brief		Block until any button is pressed on any gamepad
	//-------------------------------------------------------------------
void WaitForAnyButton( void );

	//-------------------------------------------------------------------
	//	WaitForNoButton
	//! \brief		Block until all buttons are released on all gamepads
	//-------------------------------------------------------------------
void WaitForNoButton( void );

	//-------------------------------------------------------------------
	//	WaitForAnyInput
	//! \brief		Block until anything is pressed on any gamepad
	//-------------------------------------------------------------------
void WaitForAnyInput( void );

	//-------------------------------------------------------------------
	//	WaitForNoInput
	//! \brief		Block until everything is released on every gamepad
	//-------------------------------------------------------------------
void WaitForNoInput( void );


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
  //!
  //! \param    pD3DDevice - D3D device to render the screen to
	//-------------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice );


	//-------------------------------------------------------------------
  // vsnprintf
  //! \brief    Fake implementation of vsnprintf to get MAME core
  //!           version 0.69 to compile
  //!
  //! \param    buf - The buffer to print to
  //! \param    count - The maximum number of characters to print
  //! \param    fmt - The printf format string
  //! \param    lst - The var arg variable to retrieve data from
	//-------------------------------------------------------------------
int vsnprintf( char *buf, size_t count, const char *fmt, va_list lst );


	//-------------------------------------------------------------------
  // RemapDriveLetters
  //! \brief    Remap the drive letters to the paths that the
  //!           user has defined in the INI file.
	//-------------------------------------------------------------------
void RemapDriveLetters( void );

#ifdef __cplusplus
} // End of extern "C"

	//-------------------------------------------------------------------
  // RenderToTextureStart
  //! \brief    Set up the renderer to output to a texture
	//-------------------------------------------------------------------
BOOL RenderToTextureStart( RenderToTextureToken_t &token, LPDIRECT3DDEVICE8 pD3DDevice, LPDIRECT3DTEXTURE8 texture, D3DVIEWPORT8 &textureViewpoint );

	//-------------------------------------------------------------------
  // RenderToTextureStop
  //! \brief    Reset the renderer to output to the screen
	//-------------------------------------------------------------------
void RenderToTextureStop( RenderToTextureToken_t &token );

#endif


