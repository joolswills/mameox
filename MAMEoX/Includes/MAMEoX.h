/**
	* \file			MAMEoX.h
	* \brief		Global utility functions
	*/

  // Must be defined to include the KB functions
#pragma once


//= I N C L U D E S ====================================================
#include <Xtl.h>
#include "DebugLogger.h"

#ifdef __cplusplus

#include "StdString.h"

extern "C" {
#endif

#include "osdepend.h"
#include "osd_cpu.h"


//= D E F I N E S ======================================================

  // MTRR register addresses
#define IA32_MTRRCAP            0x00FE
#define IA32_MISCENABLE         0x01A0
#define IA32_MTRR_PHYSBASE0     0x0200
#define IA32_MTRR_PHYSMASK0     0x0201
#define IA32_MTRR_PHYSBASE1     0x0202
#define IA32_MTRR_PHYSMASK1     0x0203
#define IA32_MTRR_PHYSBASE2     0x0204
#define IA32_MTRR_PHYSMASK2     0x0205
#define IA32_MTRR_PHYSBASE3     0x0206
#define IA32_MTRR_PHYSMASK3     0x0207
#define IA32_MTRR_PHYSBASE4     0x0208
#define IA32_MTRR_PHYSMASK4     0x0209
#define IA32_MTRR_PHYSBASE5     0x020A
#define IA32_MTRR_PHYSMASK5     0x020B
#define IA32_MTRR_PHYSBASE6     0x020C
#define IA32_MTRR_PHYSMASK6     0x020D
#define IA32_MTRR_PHYSBASE7     0x020E
#define IA32_MTRR_PHYSMASK7     0x020F
#define IA32_MTRR_DEF_TYPE      0x02FF

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

#define VERSION_STRING            "0.73" RELEASE_STRING
#define LVERSION_STRING           L"0.73" LRELEASE_STRING

  //! Used in RenderProgressBar to render a "bubble" instead of a progress bar
#define BUBBLEBAR                 0xFFFFFFFF

#define DRIVERLIST_FILESTAMP      "MAMEoX" VERSION_STRING

  // Maximum delay before displaying the screensaver in the ROM list (in minutes)
#define MAX_SCREENSAVER_TIMEOUT   15

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

typedef enum ROMListFilterMode
{
  FM_NONE                   = 0,          //!< Don't filter
  FM_CLONE                  = (1<<0),     //!< Filter out clones
  FM_NUMPLAYERS             = (1<<1),     //!< Filter based on the number of players
  FM_GENRE                  = (1<<2),     //!< Filter on the Genre
  FM_ROMSTATUS_SLOW         = (1<<3),     //!< Filter out "Slow" games
  FM_ROMSTATUS_OUTOFMEM     = (1<<4),     //!< Filter out "Out of Memory" games
  FM_ROMSTATUS_CRASH        = (1<<5),     //!< Filter out "Crash" games
  FM_ROMSTATUS_NONWORKING   = (1<<6),     //!< Filter out "Other nonworking" games
  FM_DISLIKED               = (1<<7),     //!< Filter out roms w/ a favorite status < "average"
  FM_ROMSTATUS_UNPLAYABLE   = FM_ROMSTATUS_OUTOFMEM | FM_ROMSTATUS_CRASH | FM_ROMSTATUS_NONWORKING,  //!< Filter out any games that cannot be played
  FM_ROMSTATUS_IMPERFECT    = FM_ROMSTATUS_SLOW | FM_ROMSTATUS_UNPLAYABLE  //!< Filter all imperfect games
} ROMListFilterMode;

typedef enum ROMListSortMode
{
  SM_BYNAME = 0x00,               //!< Sort by the ROM name
  SM_BYROMSTATUS,                 //!< Sort by the ROM working status
  SM_BYMANUFACTURER,              //!< Sort by the manufacturer
  SM_BYYEAR,                      //!< Sort by the year
  SM_BYPARENT,                    //!< Sort by the parent for clones, alphabetically otherwise
  SM_BYNUMPLAYERS,                //!< Sort by the number of players
  SM_BYFAVORITESTATUS,            //!< Sort by favorite status (best liked -> least liked)
  SM_BYNUMTIMESPLAYED,            //!< Sort by number of times played (most played -> least played)
  SM_BYGENRE,                     //!< Sort by genre
  SM_LAST                         //!< Invalid "last" tag
} ROMListSortMode;

typedef enum ROMListDisplayMode
{
  DM_VERBOSELIST,                 //!<  Verbose list (w/ data columns) 
  DM_SIMPLELIST,                  //!<  Simple list (just name)
  DM_DETAILED,                    //!<  Detailed single ROM viewer w/ screenshots
  DM_LAST                         //!<  Invalid "last" tag
} ROMListDisplayMode;

typedef enum MAMEoXFavoriteStatus
{
  FS_INDIFFERENT = 0x00,          //!< The game is neutral or unrated
  FS_STRONGDISLIKE,               //!< The game is strongly disliked
  FS_DISLIKE,                     //!< The game is disliked
  FS_LIKE,                        //!< The game is liked
  FS_STRONGLIKE                   //!< The game is strongly liked
} MAMEoXFavoriteStatus;

//= S T R U C T U R E S ================================================
typedef struct MAMEoXLaunchData_t
{
  mameoxLaunchCommand   m_command;                      //!<  The function which should be performed by the MAMEoX task
  DWORD                 m_totalMAMEGames;               //!<  The total number of games supported by the MAME core
  DWORD                 m_gameIndex;                    //!<  The index of the currently selected game
  DWORD                 m_driverListGenerationAttempts; //!<  The number of attempts made at generating the driver list

  FLOAT                 m_cursorPosition;     //!<  Cursor position within the ROM list
  FLOAT                 m_pageOffset;         //!<  Page offset within the ROM list
  UINT32                m_superscrollIndex;   //!<  Superscroll index for the ROM list
} MAMEoXLaunchData_t;

typedef struct MAMEDriverData_t
{
  UINT32                  m_index;              //!<  The index of this driver in the sorted list
  char                    *m_romFileName;       //!<  Main ROM zip file name
  char                    *m_description;       //!<  Description from the MAME core
  BOOL                    m_isClone;            //!<  Whether or not the game is a clone
	char                    *m_cloneFileName;	    //!<  Clone ROM zip file name
	char                    *m_manufacturer;	    //!<  Manufacturer name
	char                    *m_year;					    //!<  Year
  UINT32                  m_numPlayers;         //!<  Number of players
  UINT32                  m_screenOrientation;  //!<  Screen orientation (used to rotate screenshots correctly)
} MAMEDriverData_t;

#ifdef __cplusplus
  //! \struct MAMEoXDriverMetadata_t
  //! \brief  Contains MAMEoX specific data about ROMs
typedef struct MAMEoXDriverMetadata_t
{
    //-----------------------------------------------------------
    // Constructor
    //-----------------------------------------------------------
  MAMEoXDriverMetadata_t( void ) :
    m_romIndex( 0 ),
    m_romFileName( NULL ),
    m_favoriteStatus( FS_INDIFFERENT ),
    m_timesPlayed( 0 ),
    m_genre( NULL ),
    m_versionAdded( NULL ) {
  }

    //-----------------------------------------------------------
    // Constructor (const MAMEoXDriverMetadata_t &)
    //-----------------------------------------------------------
  MAMEoXDriverMetadata_t( const MAMEoXDriverMetadata_t &a ) :
    m_romIndex( 0 ),
    m_romFileName( NULL ),
    m_favoriteStatus( FS_INDIFFERENT ),
    m_timesPlayed( 0 ),
    m_genre( NULL ),
    m_versionAdded( NULL ) {

    *this = a;
  }

    //-----------------------------------------------------------
    // Destructor
    //-----------------------------------------------------------
  ~MAMEoXDriverMetadata_t( void ) {
    if( m_romFileName )
      free( m_romFileName );
    if( m_genre )
      free( m_genre );
    if( m_versionAdded )
      free( m_versionAdded );
    m_romFileName = m_genre = m_versionAdded = NULL;
  }

    //-----------------------------------------------------------
    // operator=
    //-----------------------------------------------------------
  MAMEoXDriverMetadata_t &operator=( const MAMEoXDriverMetadata_t &a ) {
    m_romIndex = a.m_romIndex;
    m_favoriteStatus = a.m_favoriteStatus;
    m_timesPlayed = a.m_timesPlayed;

    if( m_versionAdded )
      free( m_versionAdded );
    if( m_genre )
      free( m_genre );
    if( m_romFileName )
      free( m_romFileName );
    m_versionAdded = m_genre = m_romFileName = NULL;

    if( a.m_romFileName )
      m_romFileName = strdup( a.m_romFileName );

    if( a.m_genre )
      m_genre = strdup( a.m_genre );

    if( a.m_versionAdded )
      m_versionAdded = strdup( a.m_versionAdded );
    return *this;
  }

  UINT32                  m_romIndex;         //!<  The rom index, used as the ID unless there's a core upgrade  
  char                    *m_romFileName;     //!<  The ROM filename, used as the fallback "ID" for this ROM as it's more stable than an index
  MAMEoXFavoriteStatus    m_favoriteStatus;   //!<  Favorite status for this ROM
  UINT32                  m_timesPlayed;      //!<  The number of times this ROM has been played

  char                    *m_genre;           //!<  [Category] from catver.ini
  char                    *m_versionAdded;    //!<  [VerAdded] from catver.ini
} MAMEoXDriverMetadata_t;

#endif // __cplusplus

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


#ifdef __cplusplus
typedef struct ROMListOptions_t
{
  ROMListDisplayMode  m_displayMode;        //!<  Whether or not to show verbose info (manufacturer, year, etc...)
  ROMListSortMode     m_sortMode;           //!<  The current sort mode
  BOOL                m_showROMStatus;      //!<  Whether or not to colorize based on ROM Status
  BOOL                m_hideFiltered;       //!<  Whether or not to hide ROMs that have been filtered out

  UINT32              m_filterMode;         //!<  General filter mode bitvector (uses ROMListFilterMode flags)
//  UINT32              m_numPlayersFilter;
//  CStdString          m_genreFilter;
} ROMListOptions_t;
#endif

//= G L O B A L = V A R S ==============================================
extern lightgunCalibration_t    g_calibrationData[4];     //!< Lightgun calibration data
extern MAMEoXLaunchData_t       g_persistentLaunchData;   //!<  Launch data that persists via the INI

#ifdef __cplusplus
extern ROMListOptions_t         g_romListOptions;         //!<  ROM List display options that persist via the INI
#endif

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
	//	RenderProgressBar
	//! \brief		Draw a graphical progressbar or "bubblebar"
  //!
  //! \param    left - Position of the left side of the border
  //! \param    top - Position of the top of the border
  //! \param    right - Position of the right side of the border
  //! \param    bottom - Position of the bottom of the border
  //! \param    curValue - Current value used to determine the progress %
  //! \param    maxValue - The maximum possible value or BUBBLEBAR to
  //!                      draw a bubblebar
  //! \param    barColor - The color of the "bar" or "bubble" itself
  //! \param    borderColor - The color of the capsule for the bar
	//-------------------------------------------------------------------
void RenderProgressBar( INT32 left, 
                        INT32 top, 
                        INT32 right, 
                        INT32 bottom, 
                        UINT32 curValue, 
                        UINT32 maxValue, 
                        D3DCOLOR barColor, 
                        D3DCOLOR borderColor );

	//-------------------------------------------------------------------
	//	PresentFrame
	//! \brief		Present the backbuffer to the user
	//-------------------------------------------------------------------
void PresentFrame( void );

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

	//-------------------------------------------------------------------
  // Enable128MegCaching
  //! \brief    Enable writeback caching on the upper 64 megs
	//-------------------------------------------------------------------
void Enable128MegCaching( void );

	//-------------------------------------------------------------------
  // READMSRREG
  //! \brief    Write to a 64 bit MSR register
	//-------------------------------------------------------------------
inline void READMSRREG( UINT32 reg, LARGE_INTEGER *val ) 
{
  UINT32 lowPart, highPart;
  _asm {
    mov   ecx, reg
    rdmsr
    mov   lowPart, eax
    mov   highPart, edx
  };

  val->LowPart = lowPart;
  val->HighPart = highPart;
}

	//-------------------------------------------------------------------
  // WRITEMSRREG64
  //! \brief    Write to a 64 bit MSR register
	//-------------------------------------------------------------------
inline void WRITEMSRREG( UINT32 reg, LARGE_INTEGER val ) 
{
  _asm {
    mov   ecx, reg
    mov   eax, val.LowPart
    mov   edx, val.HighPart
    wrmsr
  };
}


#endif


