/**
	* \file			MAMEoXUtil.cpp
	* \brief		MAMEoX utility functions
	*
	*/


//= I N C L U D E S ====================================================
#include "MAMEoX.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "DebugLogger.h"
#include "Sections.h"
#include "FontSet.h"

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
CInputManager			  g_inputManager;
CGraphicsManager	  g_graphicsManager;
CFontSet            g_fontSet;  // The global font manager

extern BOOL         g_soundEnabled;   // Sound processing override (defined in xbox_Main.cpp)
ROMListOptions_t    g_romListOptions;
MAMEoXLaunchData_t  g_persistentLaunchData;   //!<  Launch data that persists via the INI


extern "C" {

  // from MAME\cheat.c
extern char *cheatfile;
extern const char *history_filename;  // Defined in datafile.c

  //! Lightgun calibration data
lightgunCalibration_t    g_calibrationData[4] = { {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767} };

  //! The fonttype to use for *FontRender
static fonttype          g_fontType = FONTTYPE_DEFAULT;

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


// Note: The "STARTUP" segment is unloaded in xbox_JoystickMouse.c
#pragma code_seg("STARTUP")

//-------------------------------------------------------------
//	LoadOptions
//-------------------------------------------------------------
void LoadOptions( void )
{
  CStdString iniFileName = g_FileIOConfig.m_IniPath;
  iniFileName += "\\" INIFILENAME;
  CSystem_IniFile iniFile( iniFileName );

/*
	int		mame_debug;		          1 to enable debugging
	
	int 	gui_host;		            1 to tweak some UI-related things for better GUI integration
*/

    // 1 to enable cheating
  options.cheat = iniFile.GetProfileInt( "General", "CheatsEnabled", FALSE );
  cheatfile = strdup( iniFile.GetProfileString( "General", "CheatFilename", "cheat.dat" ).c_str() );
  if( !cheatfile )
    options.cheat = FALSE;
  history_filename = NULL; //strdup( iniFile.GetProfileString( "General", "HistoryFilename", "history.dat" ).c_str() );  
  options.skip_disclaimer = iniFile.GetProfileInt( "General", "SkipDisclaimer", FALSE );   // 1 to skip the disclaimer screen at startup
	options.skip_gameinfo = iniFile.GetProfileInt( "General", "SkipGameInfo", FALSE );    // 1 to skip the game info screen at startup

  g_soundEnabled = iniFile.GetProfileInt( "Sound", "SoundEnable", TRUE );
    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );

  g_rendererOptions.m_vsync =               iniFile.GetProfileInt( "Video", "VSYNC", FALSE );       // Enable VSYNC for game rendering
  g_rendererOptions.m_throttleFramerate =   iniFile.GetProfileInt( "Video", "ThrottleFramerate", TRUE ); // Sync only to vsync
  g_rendererOptions.m_preserveAspectRatio = iniFile.GetProfileInt( "Video", "AspectRatioCorrection", TRUE );  // aspect ratio correction code
  g_rendererOptions.m_screenRotation =      (screenrotation_t)iniFile.GetProfileInt( "Video", "ScreenRotation", SR_0 );
  g_rendererOptions.m_frameskip =           iniFile.GetProfileInt( "Video", "Frameskip", AUTO_FRAMESKIP );
  g_rendererOptions.m_minFilter =           (D3DTEXTUREFILTERTYPE)iniFile.GetProfileInt( "Video", "MinificationFilter", D3DTEXF_LINEAR );
  g_rendererOptions.m_magFilter =           (D3DTEXTUREFILTERTYPE)iniFile.GetProfileInt( "Video", "MagnificationFilter", D3DTEXF_LINEAR );
	options.brightness =    iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright =  iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma =         iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth =   0;//iniFile.GetProfileInt( "Video", "ColorDepth", 15 );
	// int		ui_orientation;	        // orientation of the UI relative to the video
  
    // Validate some of the video options to keep them from ripping shit up
  if( g_rendererOptions.m_minFilter > D3DTEXF_GAUSSIANCUBIC || g_rendererOptions.m_minFilter < D3DTEXF_POINT )
    g_rendererOptions.m_minFilter = D3DTEXF_LINEAR;
  if( g_rendererOptions.m_magFilter > D3DTEXF_GAUSSIANCUBIC || g_rendererOptions.m_magFilter < D3DTEXF_POINT )
    g_rendererOptions.m_magFilter = D3DTEXF_LINEAR;
  if( g_rendererOptions.m_screenRotation > SR_270 )
    g_rendererOptions.m_screenRotation = SR_0;

  FLOAT xPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_X", DEFAULT_SCREEN_X_PERCENTAGE );
  FLOAT yPercentage = iniFile.GetProfileFloat( "Video", "ScreenUsage_Y", DEFAULT_SCREEN_Y_PERCENTAGE );
  SetScreenUsage( xPercentage, yPercentage );

  FLOAT xPosition = iniFile.GetProfileFloat( "Video", "ScreenPos_X", 0.0f );
  FLOAT yPosition = iniFile.GetProfileFloat( "Video", "ScreenPos_Y", 0.0f );
  SetScreenPosition( xPosition, yPosition );

  options.use_artwork = iniFile.GetProfileInt( "Video", "Artwork", ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS );
  options.artwork_res = 1;
  options.artwork_crop = TRUE;

	options.vector_width =  iniFile.GetProfileInt( "VectorOptions", "VectorWidth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "VectorOptions", "VectorHeight", 480 );	    // requested height for vector games; 0 means default (480)

    //- Vector options ------------------------------------------------------------------------------------
	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "Translucency", TRUE );      // 1 to enable translucency on vectors
	 
    // Antialiasing holds forever in vector.c due to an apparent signed/unsigned problem
  options.antialias = FALSE; //iniFile.GetProfileInt( "VectorOptions", "Antialiasing", FALSE );		    // 1 to enable antialiasing on vectors

	//char	savegame;		            character representing a savegame to load


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

  g_FileIOConfig.m_RomBackupPath      = iniFile.GetProfileString( "Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH );
  g_FileIOConfig.m_RomPath0           = iniFile.GetProfileString( "Directories", "RomsPath0",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath1           = iniFile.GetProfileString( "Directories", "RomsPath1",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath2           = iniFile.GetProfileString( "Directories", "RomsPath2",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath3           = iniFile.GetProfileString( "Directories", "RomsPath3",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_ArtPath            = iniFile.GetProfileString( "Directories", "ArtPath",             DEFAULT_ARTPATH );
  g_FileIOConfig.m_AudioPath          = iniFile.GetProfileString( "Directories", "AudioPath",           DEFAULT_AUDIOPATH );
  g_FileIOConfig.m_ConfigPath         = iniFile.GetProfileString( "Directories", "ConfigPath",          DEFAULT_CONFIGPATH );
  g_FileIOConfig.m_GeneralPath        = iniFile.GetProfileString( "Directories", "GeneralPath",         DEFAULT_GENERALPATH );
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString( "Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH );
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString( "Directories", "HiScoresPath",        DEFAULT_HISCOREPATH );
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString( "Directories", "NVRamPath",           DEFAULT_NVRAMPATH );
  g_FileIOConfig.m_ScreenshotPath     = iniFile.GetProfileString( "Directories", "ScreenshotPath",      DEFAULT_SCREENSHOTPATH );
  // There's no reason to allow this to be changed, it's totally internal
  //  g_FileIOConfig.m_DefaultRomListPath = iniFile.GetProfileString("Directories", "DefaultRomsListPath", DEFAULT_DEFAULTROMLISTPATH);
  g_FileIOConfig.MakeLower();

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

    //-- ROM List Options --------------------------------------------------------------------
  g_romListOptions.m_verboseMode =               iniFile.GetProfileInt( "ROMListOptions", "VerboseMode", TRUE );
  g_romListOptions.m_sortMode = (ROMListSortMode)iniFile.GetProfileInt( "ROMListOptions", "SortMode", (UINT32)SM_BYNAME );
  g_romListOptions.m_showROMStatus =             iniFile.GetProfileInt( "ROMListOptions", "ShowROMStatus", TRUE );
  g_romListOptions.m_hideFiltered =              iniFile.GetProfileInt( "ROMListOptions", "HideFilteredROMs", FALSE );
  g_romListOptions.m_filterMode =                iniFile.GetProfileInt( "ROMListOptions", "FilterMode", (UINT32)FM_CLONE );
  g_romListOptions.m_numPlayersFilter =          iniFile.GetProfileInt( "ROMListOptions", "Filter_NumPlayers", 0 );
  g_persistentLaunchData.m_cursorPosition =      iniFile.GetProfileFloat( "ROMListOptions", "CursorPosition", 0.0f );
  g_persistentLaunchData.m_pageOffset =          iniFile.GetProfileFloat( "ROMListOptions", "PageOffset", 0.0f );
  g_persistentLaunchData.m_superscrollIndex =    iniFile.GetProfileInt(   "ROMListOptions", "SuperscrollIndex", 0 );
}


//-------------------------------------------------------------
//	SaveOptions
//-------------------------------------------------------------
void SaveOptions( void )
{
  CStdString iniFileName = g_FileIOConfig.m_IniPath;
  iniFileName += "\\" INIFILENAME;
  CSystem_IniFile iniFile( iniFileName );

  iniFile.WriteProfileInt( "General", "CheatsEnabled", options.cheat );
  if( cheatfile )
    iniFile.WriteProfileString( "General", "CheatFilename", cheatfile );
  if( history_filename )
    iniFile.WriteProfileString( "General", "HistoryFilename", history_filename );

  iniFile.WriteProfileInt( "General", "SkipDisclaimer", options.skip_disclaimer );
  iniFile.WriteProfileInt( "General", "SkipGameInfo", options.skip_gameinfo );
  iniFile.WriteProfileInt( "Sound", "SoundEnable", g_soundEnabled );
  iniFile.WriteProfileInt( "Sound", "SampleRate", options.samplerate );
  iniFile.WriteProfileInt( "Sound", "UseSamples", options.use_samples );
  iniFile.WriteProfileInt( "Sound", "UseFilter", options.use_filter );


  iniFile.WriteProfileInt( "Video", "VSYNC", g_rendererOptions.m_vsync );       // Enable VSYNC for game rendering
  iniFile.WriteProfileInt( "Video", "ThrottleFramerate", g_rendererOptions.m_throttleFramerate ); // Sync only to vsync
  iniFile.WriteProfileInt( "Video", "AspectRatioCorrection", g_rendererOptions.m_preserveAspectRatio );
  iniFile.WriteProfileInt( "Video", "MinificationFilter", g_rendererOptions.m_minFilter );
  iniFile.WriteProfileInt( "Video", "MagnificationFilter", g_rendererOptions.m_magFilter );
  iniFile.WriteProfileInt( "Video", "Frameskip", g_rendererOptions.m_frameskip );
  iniFile.WriteProfileInt( "Video", "ScreenRotation", g_rendererOptions.m_screenRotation );
  iniFile.WriteProfileFloat( "Video", "Brightness", options.brightness );		    // brightness of the display
  iniFile.WriteProfileFloat( "Video", "PauseBrightness", options.pause_bright );     // brightness when in pause
	iniFile.WriteProfileFloat( "Video", "Gamma", options.gamma );			        // gamma correction of the display
//	iniFile.WriteProfileInt( "Video", "ColorDepth", options.color_depth );
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

  FLOAT xPosition, yPosition;
  GetScreenPosition( &xPosition, &yPosition );
  iniFile.WriteProfileFloat( "Video", "ScreenPos_X", xPosition );
  iniFile.WriteProfileFloat( "Video", "ScreenPos_Y", yPosition );

  iniFile.WriteProfileInt( "Video", "Artwork", options.use_artwork );


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
  iniFile.WriteProfileString( "Directories", "RomsPath0",          g_FileIOConfig.m_RomPath0 );
  iniFile.WriteProfileString( "Directories", "RomsPath1",          g_FileIOConfig.m_RomPath1 );
  iniFile.WriteProfileString( "Directories", "RomsPath2",          g_FileIOConfig.m_RomPath2 );
  iniFile.WriteProfileString( "Directories", "RomsPath3",          g_FileIOConfig.m_RomPath3 );
  iniFile.WriteProfileString("Directories", "ArtPath",             g_FileIOConfig.m_ArtPath );
  iniFile.WriteProfileString("Directories", "AudioPath",           g_FileIOConfig.m_AudioPath );
  iniFile.WriteProfileString("Directories", "ConfigPath",          g_FileIOConfig.m_ConfigPath );
  iniFile.WriteProfileString("Directories", "GeneralPath",         g_FileIOConfig.m_GeneralPath );
  iniFile.WriteProfileString("Directories", "HDImagePath",         g_FileIOConfig.m_HDImagePath );
  iniFile.WriteProfileString("Directories", "HiScoresPath",        g_FileIOConfig.m_HiScorePath );
  iniFile.WriteProfileString("Directories", "NVRamPath",           g_FileIOConfig.m_NVramPath );
  iniFile.WriteProfileString("Directories", "BackupPath",          g_FileIOConfig.m_RomBackupPath );
  iniFile.WriteProfileString("Directories", "ScreenshotPath",      g_FileIOConfig.m_ScreenshotPath );
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

    //-- ROM List Options --------------------------------------------------------------------
  iniFile.WriteProfileInt(   "ROMListOptions", "VerboseMode", g_romListOptions.m_verboseMode );
  iniFile.WriteProfileInt(   "ROMListOptions", "SortMode", (UINT32)g_romListOptions.m_sortMode );
  iniFile.WriteProfileInt(   "ROMListOptions", "ShowROMStatus", g_romListOptions.m_showROMStatus );
  iniFile.WriteProfileInt(   "ROMListOptions", "HideFilteredROMs", g_romListOptions.m_hideFiltered );
  iniFile.WriteProfileInt(   "ROMListOptions", "FilterMode", g_romListOptions.m_filterMode );
  iniFile.WriteProfileInt(   "ROMListOptions", "Filter_NumPlayers", g_romListOptions.m_numPlayersFilter );

  iniFile.WriteProfileFloat( "ROMListOptions", "CursorPosition", g_persistentLaunchData.m_cursorPosition );
  iniFile.WriteProfileFloat( "ROMListOptions", "PageOffset", g_persistentLaunchData.m_pageOffset );
  iniFile.WriteProfileInt(   "ROMListOptions", "SuperscrollIndex", g_persistentLaunchData.m_superscrollIndex );
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

  #ifdef _DEBUG
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
  #endif
}

#pragma code_seg()

//-------------------------------------------------------------
//  RequireController
//-------------------------------------------------------------
void RequireController( DWORD number )
{
  g_inputManager.PollDevices();
  if( g_inputManager.IsGamepadConnected( number ) )
    return;

  WCHAR wBuf[256];
  swprintf( wBuf, L"Please insert a controller into slot %d!", number + 1 );

  BeginFontRender( TRUE, FONTTYPE_DEFAULT );
    FontRender( 320, 180, D3DCOLOR_XRGB(255,255,255), wBuf, XBFONT_CENTER_X );
  EndFontRender( TRUE );
  
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
	g_inputManager.WaitForAnyButton();
}

//-------------------------------------------------------------
// WaitForNoButton
//-------------------------------------------------------------
void WaitForNoButton( void )
{
	g_inputManager.WaitForNoButton();
}

//-------------------------------------------------------------
// WaitForAnyInput
//-------------------------------------------------------------
void WaitForAnyInput( void )
{
	g_inputManager.WaitForAnyInput();
}

//-------------------------------------------------------------
// WaitForNoInput
//-------------------------------------------------------------
void WaitForNoInput( void )
{
	g_inputManager.WaitForNoInput();
}


//-------------------------------------------------------------
//	GetLightgunCalibratedPosition
//-------------------------------------------------------------
void GetLightgunCalibratedPosition( UINT32 player, INT32 *deltax, INT32 *deltay )
{
  if( !deltax || !deltay || player > 3 )
    return;

  lightgunCalibration_t &calibData = g_calibrationData[player];
  CGamepad *gp = g_inputManager.GetGamepad(player);

    // Don't bother if we're not pointing at the screen
  if( !gp || !gp->IsLightgunPointedAtScreen() )
  {
    *deltax = *deltay = 0;
    return;
  }

  *deltax = -1 * (int)((FLOAT)gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ) * 128.0f / (FLOAT)XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_X );
  *deltay = -1 * (int)((FLOAT)gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y ) * 128.0f / (FLOAT)XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_Y );

    // Lock to the expected range
  if( *deltax > 128 )
    *deltax = 128;
  else if( *deltax < -128 )
    *deltax = -128;

  if( *deltay > 128 )
    *deltay = 128;
  else if( *deltay < -128 )
    *deltay = -128;



  PRINTMSG_TO_CONSOLE(  T_INFO, 
                        "x: %d:%d y: %d:%d", 
                        *deltax, 
                        gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ),
                        *deltay,
                        gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y ));
}

//-------------------------------------------------------------
//	BeginFontRender
//-------------------------------------------------------------
void BeginFontRender( BOOL ClearScreen, fonttype fontType )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();
  g_fontType = fontType;

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

  g_fontSet.GetFont(g_fontType).Begin();
}

//-------------------------------------------------------------
//	FontRender
//-------------------------------------------------------------
void FontRender( INT32 x, INT32 y, UINT32 color, const WCHAR *str, UINT32 flags )
{
	g_fontSet.GetFont(g_fontType).DrawText( (FLOAT)x, (FLOAT)y, color, str, flags );
}

//-------------------------------------------------------------
//	EndFontRender
//-------------------------------------------------------------
void EndFontRender( BOOL present )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();
  g_fontSet.GetFont(g_fontType).End();

  if( present )
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


	g_inputManager.WaitForNoButton();

  while( !g_inputManager.IsAnyButtonPressed() )
  {
    BeginFontRender( TRUE, FONTTYPE_DEFAULT );
      FontRender( 320, 200, D3DCOLOR_XRGB(255,200,200), L"This is a DEBUG version of MAMEoX!", 2 );
      FontRender( 320, 280, D3DCOLOR_XRGB(255,255,255), L"Mem: Avail/Total", 2 );
      FontRender( 320, 300, D3DCOLOR_XRGB(255,255,255), memStr, 2 );
    EndFontRender( TRUE );
  }
	g_inputManager.WaitForNoButton();
}
#endif


}	// End Extern "C"

// Note: The "STARTUP" segment is unloaded in xbox_JoystickMouse.c
#pragma code_seg("STARTUP")
//-------------------------------------------------------------
//  RenderToTextureStart
//-------------------------------------------------------------
BOOL RenderToTextureStart( RenderToTextureToken_t &token, LPDIRECT3DDEVICE8 pD3DDevice, LPDIRECT3DTEXTURE8 texture, D3DVIEWPORT8 &textureViewpoint )
{
  if( !texture )
    return FALSE;

  token.m_pD3DDevice = pD3DDevice;
  token.m_viewPoint = textureViewpoint;
  token.m_texture = texture;

      // Set up to render to texture
  pD3DDevice->GetRenderTarget( &token.m_backBuffer );
  pD3DDevice->GetDepthStencilSurface( &token.m_zBuffer );
  texture->GetSurfaceLevel( 0, &token.m_textureSurface );
  pD3DDevice->SetRenderTarget( token.m_textureSurface, NULL );
  pD3DDevice->SetViewport( &textureViewpoint );

  return TRUE;
}

//-------------------------------------------------------------
//  RenderToTextureStop
//-------------------------------------------------------------
void RenderToTextureStop( RenderToTextureToken_t &token )
{
  D3DVIEWPORT8 vpBackBuffer = { 0, 0, 640, 480, 0.0f, 1.0f };
  token.m_pD3DDevice->SetRenderTarget( token.m_backBuffer, token.m_zBuffer );
  token.m_pD3DDevice->SetViewport( &vpBackBuffer );
  SAFE_RELEASE( token.m_backBuffer );
  SAFE_RELEASE( token.m_zBuffer );
  SAFE_RELEASE( token.m_textureSurface );
}


//-------------------------------------------------------------
//  Enable128MegCaching
//-------------------------------------------------------------
void Enable128MegCaching( void )
{
  LARGE_INTEGER regVal;

    // Verify that we have 128 megs available
  MEMORYSTATUS memStatus;
  GlobalMemoryStatus( &memStatus );
  if( memStatus.dwTotalPhys < (100 * 1024 * 1024) )
    return;

    // Grab the existing default type
  READMSRREG( IA32_MTRR_DEF_TYPE, &regVal );
  
    // Set the default to WriteBack (0x06)
  regVal.LowPart = (regVal.LowPart & ~0xFF) | 0x06;
  WRITEMSRREG( IA32_MTRR_DEF_TYPE, regVal );
}

#pragma code_seg()

