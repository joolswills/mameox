/**
	* \file			MAMEoXUtil.cpp
	* \brief		MAMEoX utility functions
	*
	*/


//= I N C L U D E S ====================================================
#include <Xtl.h>

#include "MAMEoX.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "DebugLogger.h"
#include "Sections.h"

	// Font class from the XDK
#include "XBFont.h"

#include "System_IniFile.h"
#include "xbox_Timing.h"
#include "xbox_JoystickMouse.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_FileIO.h"
#include "xbox_Network.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
}

//= D E F I N E S =====================================================
#define MAPDRIVE( _drivePath__, _driveLetter__ )    { \
                                                      UNICODE_STRING devName = { strlen(_drivePath__), strlen(_drivePath__) + 1, _drivePath__ }; \
                                                      UNICODE_STRING devLetter = { strlen(_driveLetter__), strlen(_driveLetter__) + 1, _driveLetter__ }; \
                                                      DWORD status = IoCreateSymbolicLink( &devLetter, &devName ); \
                                                      PRINTMSG( T_INFO, "Map %s = %s, status %d", _driveLetter__, _drivePath__, status ); \
                                                    }

//= S T R U C T U R E S ===============================================
typedef struct _UNICODE_STRING
{
  USHORT      m_length;
  USHORT      m_maxLength;
  const char *m_str;
} UNICODE_STRING, *PUNICODE_STRING;

//= G L O B A L = V A R S =============================================
CInputManager			g_inputManager;
CGraphicsManager	g_graphicsManager;
CXBFont						g_font;

extern "C" {

  // from MAME\cheat.c
extern char *cheatfile;

  //! Lightgun calibration data
lightgunCalibration_t    g_calibrationData[4] = { {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767} };


//= P R O T O T Y P E S ===============================================
XBOXAPI DWORD WINAPI IoCreateSymbolicLink( PUNICODE_STRING symLinkName, PUNICODE_STRING devName );
XBOXAPI DWORD WINAPI IoDeleteSymbolicLink( PUNICODE_STRING symLinkName );


//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//  vsnprintf
//-------------------------------------------------------------
int vsnprintf( char *buf, size_t count, const char *fmt, va_list lst )
{
  return vsprintf( buf, fmt, lst );
}

//-------------------------------------------------------------
//	LoadOptions
//-------------------------------------------------------------
void LoadOptions( void )
{
  CSystem_IniFile iniFile( INIFILENAME );

/*
	int		mame_debug;		          1 to enable debugging
	
	int 	gui_host;		            1 to tweak some UI-related things for better GUI integration
*/

    // 1 to enable cheating
  options.cheat = iniFile.GetProfileInt( "General", "CheatsEnabled", FALSE );
  cheatfile = strdup( iniFile.GetProfileString( "General", "CheatFilename", "cheat.dat" ).c_str() );
  if( !cheatfile )
    options.cheat = FALSE;

    // 1 to skip the disclaimer screen at startup
  options.skip_disclaimer = iniFile.GetProfileInt( "General", "SkipDisclaimer", FALSE );

    // 1 to skip the game info screen at startup
	options.skip_gameinfo = iniFile.GetProfileInt( "General", "SkipGameInfo", FALSE );
  
    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );

	options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
	options.vector_width = iniFile.GetProfileInt( "VectorOptions", "VectorWidth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "VectorOptions", "VectorHeight", 480 );	    // requested height for vector games; 0 means default (480)
	// int		ui_orientation;	        // orientation of the UI relative to the video

	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "Translucency", TRUE );      // 1 to enable translucency on vectors
	 
    // Antialiasing holds forever in vector.c due to an apparent signed/unsigned problem
  options.antialias = FALSE; //iniFile.GetProfileInt( "VectorOptions", "Antialiasing", FALSE );		    // 1 to enable antialiasing on vectors

	//int		use_artwork;	          bitfield indicating which artwork pieces to use
	//int		artwork_res;	          1 for 1x game scaling, 2 for 2x
	//int		artwork_crop;	          1 to crop artwork to the game screen
	//char	savegame;		            character representing a savegame to load


  FLOAT xPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_X", DEFAULT_SCREEN_X_PERCENTAGE );
  FLOAT yPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_Y", DEFAULT_SCREEN_Y_PERCENTAGE );
  SetScreenUsage( xPercentage, yPercentage );

  // Grab the network settings
  g_NetworkConfig.m_networkDisabled = iniFile.GetProfileInt( "Network", "DisableNetworking",  FALSE );
  g_NetworkConfig.m_IPAddr          = iniFile.GetProfileString("Network", "IPAddress",  DEFAULT_IPADDR);
  g_NetworkConfig.m_Gateway         = iniFile.GetProfileString("Network", "Gateway",    DEFAULT_GATEWAY);
  g_NetworkConfig.m_SubnetMask      = iniFile.GetProfileString("Network", "Subnet",     DEFAULT_SUBNETMASK);
  g_NetworkConfig.m_NameServer      = iniFile.GetProfileString("Network", "NameServer", DEFAULT_NAMESERVER);

  // Grab the directory settings
  g_FileIOConfig.m_ALTDrive           = iniFile.GetProfileString( "Directories", "ALTDrive",            DEFAULT_ALTDRIVE );
  g_FileIOConfig.m_LetterCMapping     = iniFile.GetProfileString( "Directories", "C_Mapping",           DEFAULT_CMAPPING );
  g_FileIOConfig.m_LetterEMapping     = iniFile.GetProfileString( "Directories", "E_Mapping",           DEFAULT_EMAPPING );
  g_FileIOConfig.m_LetterFMapping     = iniFile.GetProfileString( "Directories", "F_Mapping",           DEFAULT_FMAPPING );
  g_FileIOConfig.m_LetterGMapping     = iniFile.GetProfileString( "Directories", "G_Mapping",           DEFAULT_GMAPPING );
  g_FileIOConfig.m_LetterHMapping     = iniFile.GetProfileString( "Directories", "H_Mapping",           DEFAULT_HMAPPING );

  g_FileIOConfig.m_ArtPath            = iniFile.GetProfileString( "Directories", "ArtPath",             DEFAULT_ARTPATH );
  g_FileIOConfig.m_AudioPath          = iniFile.GetProfileString( "Directories", "AudioPath",           DEFAULT_AUDIOPATH );
  g_FileIOConfig.m_ConfigPath         = iniFile.GetProfileString( "Directories", "ConfigPath",          DEFAULT_CONFIGPATH );
  g_FileIOConfig.m_GeneralPath        = iniFile.GetProfileString( "Directories", "GeneralPath",         DEFAULT_GENERALPATH );
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString( "Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH );
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString( "Directories", "HiScoresPath",        DEFAULT_HISCOREPATH );
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString( "Directories", "NVRamPath",           DEFAULT_NVRAMPATH );
  g_FileIOConfig.m_RomBackupPath      = iniFile.GetProfileString( "Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH );
  g_FileIOConfig.m_RomPath            = iniFile.GetProfileString( "Directories", "RomsPath",            DEFAULT_ROMPATH );
  // There's no reason to allow this to be changed, it's totally internal
  //  g_FileIOConfig.m_DefaultRomListPath = iniFile.GetProfileString("Directories", "DefaultRomsListPath", DEFAULT_DEFAULTROMLISTPATH);

    //-- Lightgun calibration -----------------------------------------------
  g_calibrationData[0].m_xData[0] = iniFile.GetProfileInt( "Input", "Lightgun1_Left", -32767 );
  g_calibrationData[0].m_xData[1] = iniFile.GetProfileInt( "Input", "Lightgun1_CenterX", 0 );
  g_calibrationData[0].m_xData[2] = iniFile.GetProfileInt( "Input", "Lightgun1_Right", 32767 );

  g_calibrationData[0].m_yData[0] = iniFile.GetProfileInt( "Input", "Lightgun1_Top", 32767 );
  g_calibrationData[0].m_yData[1] = iniFile.GetProfileInt( "Input", "Lightgun1_CenterY", 0 );
  g_calibrationData[0].m_yData[2] = iniFile.GetProfileInt( "Input", "Lightgun1_Bottom", -32767 );

  g_calibrationData[1].m_xData[0] = iniFile.GetProfileInt( "Input", "Lightgun2_Left", -32767 );
  g_calibrationData[1].m_xData[1] = iniFile.GetProfileInt( "Input", "Lightgun2_CenterX", 0 );
  g_calibrationData[1].m_xData[2] = iniFile.GetProfileInt( "Input", "Lightgun2_Right", 32767 );

  g_calibrationData[1].m_yData[0] = iniFile.GetProfileInt( "Input", "Lightgun2_Top", 32767 );
  g_calibrationData[1].m_yData[1] = iniFile.GetProfileInt( "Input", "Lightgun2_CenterY", 0 );
  g_calibrationData[1].m_yData[2] = iniFile.GetProfileInt( "Input", "Lightgun2_Bottom", -32767 );

  g_calibrationData[2].m_xData[0] = iniFile.GetProfileInt( "Input", "Lightgun3_Left", -32767 );
  g_calibrationData[2].m_xData[1] = iniFile.GetProfileInt( "Input", "Lightgun3_CenterX", 0 );
  g_calibrationData[2].m_xData[2] = iniFile.GetProfileInt( "Input", "Lightgun3_Right", 32767 );

  g_calibrationData[2].m_yData[0] = iniFile.GetProfileInt( "Input", "Lightgun3_Top", 32767 );
  g_calibrationData[2].m_yData[1] = iniFile.GetProfileInt( "Input", "Lightgun3_CenterY", 0 );
  g_calibrationData[2].m_yData[2] = iniFile.GetProfileInt( "Input", "Lightgun3_Bottom", -32767 );

  g_calibrationData[3].m_xData[0] = iniFile.GetProfileInt( "Input", "Lightgun4_Left", -32767 );
  g_calibrationData[3].m_xData[1] = iniFile.GetProfileInt( "Input", "Lightgun4_CenterX", 0 );
  g_calibrationData[3].m_xData[2] = iniFile.GetProfileInt( "Input", "Lightgun4_Right", 32767 );

  g_calibrationData[3].m_yData[0] = iniFile.GetProfileInt( "Input", "Lightgun4_Top", 32767 );
  g_calibrationData[3].m_yData[1] = iniFile.GetProfileInt( "Input", "Lightgun4_CenterY", 0 );
  g_calibrationData[3].m_yData[2] = iniFile.GetProfileInt( "Input", "Lightgun4_Bottom", -32767 );
}


//-------------------------------------------------------------
//	SaveOptions
//-------------------------------------------------------------
void SaveOptions( void )
{
  CSystem_IniFile iniFile( INIFILENAME );

  iniFile.WriteProfileInt( "General", "CheatsEnabled", options.cheat );
  if( cheatfile )
    iniFile.WriteProfileString( "General", "CheatFilename", cheatfile );

  iniFile.WriteProfileInt( "General", "SkipDisclaimer", options.skip_disclaimer );
  iniFile.WriteProfileInt( "General", "SkipGameInfo", options.skip_gameinfo );
  iniFile.WriteProfileInt( "Sound", "SampleRate", options.samplerate );
  iniFile.WriteProfileInt( "Sound", "UseSamples", options.use_samples );
  iniFile.WriteProfileInt( "Sound", "UseFilter", options.use_filter );
  iniFile.WriteProfileFloat( "Video", "Brightness", options.brightness );		    // brightness of the display
  iniFile.WriteProfileFloat( "Video", "PauseBrightness", options.pause_bright );     // brightness when in pause
	iniFile.WriteProfileFloat( "Video", "Gamma", options.gamma );			        // gamma correction of the display
	iniFile.WriteProfileInt( "Video", "ColorDepth", options.color_depth );
	iniFile.WriteProfileInt( "VectorOptions", "VectorWidth", options.vector_width );	      // requested width for vector games; 0 means default (640)
	iniFile.WriteProfileInt( "VectorOptions", "VectorHeight", options.vector_height );	    // requested height for vector games; 0 means default (480)
	iniFile.WriteProfileInt( "VectorOptions", "BeamWidth", options.beam );			            // vector beam width
	iniFile.WriteProfileFloat( "VectorOptions", "FlickerEffect", options.vector_flicker );	  // vector beam flicker effect control
	iniFile.WriteProfileFloat( "VectorOptions", "BeamIntensity", options.vector_intensity );  // vector beam intensity
	iniFile.WriteProfileInt( "VectorOptions", "Translucency", options.translucency );      // 1 to enable translucency on vectors
	//iniFile.WriteProfileInt( "VectorOptions", "Antialiasing", options.antialias );		    // 1 to enable antialiasing on vectors

  FLOAT xPercentage, yPercentage;
  GetScreenUsage( &xPercentage, &yPercentage );
  iniFile.WriteProfileFloat( "Video", "ScreenUsage_X", xPercentage );
  iniFile.WriteProfileFloat( "Video", "ScreenUsage_Y", yPercentage );


    //-- Write the network settings ---------------------------------------------
  iniFile.WriteProfileInt( "Network", "DisableNetworking",  g_NetworkConfig.m_networkDisabled );
  iniFile.WriteProfileString( "Network", "IPAddress",  g_NetworkConfig.m_IPAddr );
  iniFile.WriteProfileString( "Network", "Gateway",    g_NetworkConfig.m_Gateway );
  iniFile.WriteProfileString( "Network", "Subnet",     g_NetworkConfig.m_SubnetMask );
  iniFile.WriteProfileString( "Network", "NameServer", g_NetworkConfig.m_NameServer );

    //-- Write the directory settings -------------------------------------------
  iniFile.WriteProfileString( "Directories", "ALTDrive",           g_FileIOConfig.m_ALTDrive );
  iniFile.WriteProfileString( "Directories", "C_Mapping",          g_FileIOConfig.m_LetterCMapping );
  iniFile.WriteProfileString( "Directories", "E_Mapping",          g_FileIOConfig.m_LetterEMapping );
  iniFile.WriteProfileString( "Directories", "F_Mapping",          g_FileIOConfig.m_LetterFMapping );
  iniFile.WriteProfileString( "Directories", "G_Mapping",          g_FileIOConfig.m_LetterGMapping );
  iniFile.WriteProfileString( "Directories", "H_Mapping",          g_FileIOConfig.m_LetterHMapping );
  iniFile.WriteProfileString("Directories", "ArtPath",             g_FileIOConfig.m_ArtPath );
  iniFile.WriteProfileString("Directories", "AudioPath",           g_FileIOConfig.m_AudioPath );
  iniFile.WriteProfileString("Directories", "ConfigPath",          g_FileIOConfig.m_ConfigPath );
  iniFile.WriteProfileString("Directories", "GeneralPath",         g_FileIOConfig.m_GeneralPath );
  iniFile.WriteProfileString("Directories", "HDImagePath",         g_FileIOConfig.m_HDImagePath );
  iniFile.WriteProfileString("Directories", "HiScoresPath",        g_FileIOConfig.m_HiScorePath );
  iniFile.WriteProfileString("Directories", "NVRamPath",           g_FileIOConfig.m_NVramPath );
  iniFile.WriteProfileString("Directories", "BackupPath",          g_FileIOConfig.m_RomBackupPath );
  iniFile.WriteProfileString("Directories", "RomsPath",            g_FileIOConfig.m_RomPath );
  // There's no reason to allow this to be changed, it's totally internal
  //iniFile.WriteProfileString("Directories", "DefaultRomsListPath", g_FileIOConfig.m_DefaultRomListPath );


    //-- Lightgun calibration -----------------------------------------------
  iniFile.WriteProfileInt( "Input", "Lightgun1_Left", g_calibrationData[0].m_xData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun1_CenterX", g_calibrationData[0].m_xData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun1_Right", g_calibrationData[0].m_xData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun1_Top", g_calibrationData[0].m_yData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun1_CenterY", g_calibrationData[0].m_yData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun1_Bottom", g_calibrationData[0].m_yData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun2_Left", g_calibrationData[1].m_xData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun2_CenterX", g_calibrationData[1].m_xData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun2_Right", g_calibrationData[1].m_xData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun2_Top", g_calibrationData[1].m_yData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun2_CenterY", g_calibrationData[1].m_yData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun2_Bottom", g_calibrationData[1].m_yData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun3_Left", g_calibrationData[2].m_xData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun3_CenterX", g_calibrationData[2].m_xData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun3_Right", g_calibrationData[2].m_xData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun3_Top", g_calibrationData[2].m_yData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun3_CenterY", g_calibrationData[2].m_yData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun3_Bottom", g_calibrationData[2].m_yData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun4_Left", g_calibrationData[3].m_xData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun4_CenterX", g_calibrationData[3].m_xData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun4_Right", g_calibrationData[3].m_xData[2] );

  iniFile.WriteProfileInt( "Input", "Lightgun4_Top", g_calibrationData[3].m_yData[0] );
  iniFile.WriteProfileInt( "Input", "Lightgun4_CenterY", g_calibrationData[3].m_yData[1] );
  iniFile.WriteProfileInt( "Input", "Lightgun4_Bottom", g_calibrationData[3].m_yData[2] );

}

//-------------------------------------------------------------
//  RequireController
//-------------------------------------------------------------
void RequireController( DWORD number )
{
  if( g_inputManager.GetGamepadDeviceState( number ) )
    return;

  WCHAR wBuf[256];
  swprintf( wBuf, L"Please insert a controller into slot %d!", number + 1 );

  BeginFontRender( TRUE );
    FontRender( 320, 180, D3DCOLOR_XRGB(255,255,255), wBuf, XBFONT_CENTER_X );
  EndFontRender();
  
  g_inputManager.WaitForControllerInsertion( number );
}

//-------------------------------------------------------------
//	GetGamepadState
//-------------------------------------------------------------
const XINPUT_GAMEPAD *GetGamepadState( UINT32 joynum )
{
	return g_inputManager.GetGamepadDeviceState( joynum );
}

//-------------------------------------------------------------
//	GetGamepadCaps
//-------------------------------------------------------------
const XINPUT_CAPABILITIES *GetGamepadCaps( UINT32 joynum )
{
  return g_inputManager.GetGamepadDeviceCaps( joynum );
}


//-------------------------------------------------------------
//	PollGamepads
//-------------------------------------------------------------
void PollGamepads( void )
{
	g_inputManager.PollDevices();
}


//-------------------------------------------------------------
// WaitForAnyButton
//-------------------------------------------------------------
void WaitForAnyButton( void )
{
	g_inputManager.WaitForAnyButton( 0 );
}

//-------------------------------------------------------------
// WaitForNoButton
//-------------------------------------------------------------
void WaitForNoButton( void )
{
	g_inputManager.WaitForNoButton( 0 );
}

//-------------------------------------------------------------
//	BeginFontRender
//-------------------------------------------------------------
void BeginFontRender( BOOL ClearScreen )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

  if( ClearScreen )
  {
		  // Clear the backbuffer
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                        D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil
  }

  g_font.Begin();

}

//-------------------------------------------------------------
//	FontRender
//-------------------------------------------------------------
void FontRender( INT32 x, INT32 y, UINT32 color, const WCHAR *str, UINT32 flags )
{
	g_font.DrawText( (FLOAT)x, (FLOAT)y, color, str, flags );
}

//-------------------------------------------------------------
//	EndFontRender
//-------------------------------------------------------------
void EndFontRender( void )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();
  g_font.End();
  pD3DDevice->Present( NULL, NULL, NULL, NULL );
}


#ifdef _DEBUG
//-------------------------------------------------------------
//	CheckRAM
//-------------------------------------------------------------
void CheckRAM( void )
{
  WCHAR memStr[256];
  MEMORYSTATUS memStatus;
  GlobalMemoryStatus(  &memStatus );

  swprintf( memStr, 
            L"Memory: %lu/%lu",
            memStatus.dwAvailPhys, 
            memStatus.dwTotalPhys );

  BeginFontRender( TRUE );
  FontRender( 320, 200, D3DCOLOR_XRGB(255,200,200), L"This is a DEBUG version of MAMEoX!", 2 );
  FontRender( 320, 280, D3DCOLOR_XRGB(255,255,255), L"Mem: Avail/Total", 2 );
  FontRender( 320, 300, D3DCOLOR_XRGB(255,255,255), memStr, 2 );
  EndFontRender();

  RequireController( 0 );
	g_inputManager.WaitForNoButton( 0 );
	g_inputManager.WaitForAnyButton( 0 );
	g_inputManager.WaitForNoButton( 0 );
}
#endif

//-------------------------------------------------------------
//	ShowLoadingScreen
//-------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
		// Clear the backbuffer
  pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                      D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

  g_font.Begin();
  g_font.DrawText( 320, 240, D3DCOLOR_RGBA( 255, 255, 255, 255),   L"Loading. Please wait...", XBFONT_CENTER_X );
  g_font.End();

  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  pD3DDevice->PersistDisplay();
}


//-------------------------------------------------------------
// RemapDriveLetters
//-------------------------------------------------------------
void RemapDriveLetters( void )
{
  MAPDRIVE( g_FileIOConfig.m_LetterCMapping.c_str(), "\\??\\C:" );
  MAPDRIVE( g_FileIOConfig.m_LetterEMapping.c_str(), "\\??\\E:" );
  MAPDRIVE( g_FileIOConfig.m_LetterFMapping.c_str(), "\\??\\F:" );
  MAPDRIVE( g_FileIOConfig.m_LetterGMapping.c_str(), "\\??\\G:" );
  MAPDRIVE( g_FileIOConfig.m_LetterHMapping.c_str(), "\\??\\H:" );

  WIN32_FIND_DATA findData;

  HANDLE h = FindFirstFile( "D:\\*", &findData );
  if( h != INVALID_HANDLE_VALUE )
  {
    do {
      PRINTMSG( T_INFO, "D:\\%s", findData.cFileName );
    } while( FindNextFile( h, &findData ) );
    FindClose( h );
  }

  h = FindFirstFile( "E:\\*", &findData );
  if( h != INVALID_HANDLE_VALUE )
  {
    do {
      PRINTMSG( T_INFO, "E:\\%s", findData.cFileName );
    } while( FindNextFile( h, &findData ) );
    FindClose( h );
  }

  h = FindFirstFile( "G:\\*", &findData );
  if( h != INVALID_HANDLE_VALUE )
  {
    do {
      PRINTMSG( T_INFO, "G:\\%s", findData.cFileName );
    } while( FindNextFile( h, &findData ) );
    FindClose( h );
  }

}

}	// End Extern "C"

