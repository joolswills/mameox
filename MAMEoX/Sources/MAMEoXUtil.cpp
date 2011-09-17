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
#include "XBESectionUtil.h"

#include "FontSet.h"
#include "VirtualMemoryManager.h"

#include "System_IniFile.h"
#include "xbox_Timing.h"
#include "xbox_JoystickMouse.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_FileIO.h"
#include "xbox_Network.h"
#include "lightgun.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
#include "virtualmemory.h"
}

//= D E F I N E S =====================================================
#define MAPDRIVE( _drivePath__, _driveLetter__ )    { \
                                                      UNICODE_STRING devName = { strlen(_drivePath__), strlen(_drivePath__) + 1, _drivePath__ }; \
                                                      UNICODE_STRING devLetter = { strlen(_driveLetter__), strlen(_driveLetter__) + 1, _driveLetter__ }; \
                                                      DWORD status = IoCreateSymbolicLink( &devLetter, &devName ); \
                                                      PRINTMSG(( T_INFO, "Map %s = %s, status %d", _driveLetter__, _drivePath__, status )); \
                                                    }

//= S T R U C T U R E S ===============================================
typedef struct _UNICODE_STRING
{
  USHORT      m_length;
  USHORT      m_maxLength;
  const char *m_str;
} UNICODE_STRING, *PUNICODE_STRING;

//= G L O B A L = V A R S =============================================
CInputManager			    g_inputManager;
CGraphicsManager	    g_graphicsManager;
CFontSet              g_fontSet;          // The global font manager
CVirtualMemoryManager g_vmManager;        // The global memory manager
CLightgun g_lightgunGame ;

extern BOOL           g_soundEnabled;   // Sound processing override (defined in xbox_Main.cpp)

/*
  TODO: Try putting these into the startup section to save some bytes
#pragma data_seg("STRT.D")
#pragma bss_seg("STRT.D")
#pragma comment(linker, "/merge:STRT.D=STARTUP")
#pragma comment(linker, "/merge:STRT.D=STARTUP")
#pragma comment(linker, "/section:STARTUP,E,W,R")
*/

ROMListOptions_t      g_romListOptions;
SkinOptions_t					g_skinOptions;
MAMEoXLaunchData_t    g_persistentLaunchData;   //!<  Launch data that persists via the INI
/*
#pragma data_seg()
#pragma bss_seg()
*/

extern UINT32         g_screensaverTimeout;     //!<  Time before the Launcher screensaver kicks in

extern "C" int fatalerror( const char *fmt, ... );

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
//  osd_vmm_malloc
//-------------------------------------------------------------
void *osd_vmm_malloc( size_t size )
{
  void *ret = g_vmManager.Malloc( size );
  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Virtual Malloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}

//-------------------------------------------------------------
//  osd_vmm_free
//-------------------------------------------------------------
void osd_vmm_free( void *ptr )
{
  g_vmManager.Free( ptr );
}

//-------------------------------------------------------------
//  osd_vmm_accessaddress
//-------------------------------------------------------------
BOOL osd_vmm_accessaddress( void *ptr )
{
  return g_vmManager.AccessAddressRange( ptr, 0 );
}

//-------------------------------------------------------------
//  osd_vmm_unloadLRUpage
//-------------------------------------------------------------
BOOL osd_vmm_unloadLRUpage( void )
{
  return g_vmManager.UnloadLRUPage();
}

//-------------------------------------------------------------
//  osd_vmm_printinfo
//-------------------------------------------------------------
void osd_vmm_printinfo( void )
{
#ifdef _DEBUG
  g_vmManager.PrintInfo();
#endif
}

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
  CStdString iniFileName = DEFAULT_MAMEOXSYSTEMPATH "\\" INIFILENAME;
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
  g_screensaverTimeout =  iniFile.GetProfileInt( "General", "ScreenSaverTimeout", 10 );    // Minutes before screensaver in Launcher kicks in

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
	g_rendererOptions.m_FilterType =          (EFilterType)iniFile.GetProfileInt("Video", "GraphicsFilter", eftNone);
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
  options.artwork_res = 0;
  options.artwork_crop = FALSE;

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
  g_FileIOConfig.m_letterCMapping     = iniFile.GetProfileString( "Directories", "C_Mapping",           DEFAULT_CMAPPING );
  g_FileIOConfig.m_letterEMapping     = iniFile.GetProfileString( "Directories", "E_Mapping",           DEFAULT_EMAPPING );
  g_FileIOConfig.m_letterFMapping     = iniFile.GetProfileString( "Directories", "F_Mapping",           DEFAULT_FMAPPING );
  g_FileIOConfig.m_letterGMapping     = iniFile.GetProfileString( "Directories", "G_Mapping",           DEFAULT_GMAPPING );
  g_FileIOConfig.m_letterHMapping     = iniFile.GetProfileString( "Directories", "H_Mapping",           DEFAULT_HMAPPING );

  g_FileIOConfig.m_romBackupPath      = iniFile.GetProfileString( "Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH );
  g_FileIOConfig.m_romPath0           = iniFile.GetProfileString( "Directories", "RomsPath0",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath1           = iniFile.GetProfileString( "Directories", "RomsPath1",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath2           = iniFile.GetProfileString( "Directories", "RomsPath2",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath3           = iniFile.GetProfileString( "Directories", "RomsPath3",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_artPath            = iniFile.GetProfileString( "Directories", "ArtPath",             DEFAULT_ARTPATH );
  g_FileIOConfig.m_audioPath          = iniFile.GetProfileString( "Directories", "AudioPath",           DEFAULT_AUDIOPATH );
  g_FileIOConfig.m_configPath         = iniFile.GetProfileString( "Directories", "ConfigPath",          DEFAULT_CONFIGPATH );
  g_FileIOConfig.m_generalPath        = iniFile.GetProfileString( "Directories", "GeneralPath",         DEFAULT_GENERALPATH );
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString( "Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH );
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString( "Directories", "HiScoresPath",        DEFAULT_HISCOREPATH );
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString( "Directories", "NVRamPath",           DEFAULT_NVRAMPATH );
  g_FileIOConfig.m_screenshotPath     = iniFile.GetProfileString( "Directories", "ScreenshotPath",      DEFAULT_SCREENSHOTPATH );
  g_FileIOConfig.m_autoBootSavePath   = iniFile.GetProfileString( "Directories", "AutoBootSavePath",    DEFAULT_BOOTSAVESTATE );


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
  g_romListOptions.m_displayMode = (ROMListDisplayMode)iniFile.GetProfileInt( "ROMListOptions", "DisplayMode", DM_VERBOSELIST );
  g_romListOptions.m_sortMode =       (ROMListSortMode)iniFile.GetProfileInt( "ROMListOptions", "SortMode", (UINT32)SM_BYNAME );
  g_romListOptions.m_showROMStatus =                   iniFile.GetProfileInt( "ROMListOptions", "ShowROMStatus", TRUE );
  g_romListOptions.m_hideFiltered =                    iniFile.GetProfileInt( "ROMListOptions", "HideFilteredROMs", FALSE );
  g_romListOptions.m_filterMode =                      iniFile.GetProfileInt( "ROMListOptions", "FilterMode", (UINT32)FM_CLONE );
//  g_romListOptions.m_numPlayersFilter =                iniFile.GetProfileInt( "ROMListOptions", "Filter_NumPlayers", 0 );
  g_persistentLaunchData.m_cursorPosition =            iniFile.GetProfileFloat( "ROMListOptions", "CursorPosition", 0.0f );
  g_persistentLaunchData.m_pageOffset =                iniFile.GetProfileFloat( "ROMListOptions", "PageOffset", 0.0f );
  g_persistentLaunchData.m_superscrollIndex =          iniFile.GetProfileInt(   "ROMListOptions", "SuperscrollIndex", 0 );


		//-- Skin Options ------------------------------------------------------------------------
	g_skinOptions.m_currentSkin = iniFile.GetProfileString( "SkinOptions", "SelectedSkin", "Original" );


	//-- VMM Options ------------------------------------------------------------------------

	g_forcevmem =                    iniFile.GetProfileInt( "VMMOptions", "ForceVMM", FALSE );

	if ( g_forcevmem )
	{
		g_vmemThreshold =                    iniFile.GetProfileInt( "VMMOptions", "Threshold", 0x400000 );
		g_vmemCommitSize =                    iniFile.GetProfileInt( "VMMOptions", "CommitSize", 0x100000 );
		g_vmemDistribute =                    iniFile.GetProfileInt( "VMMOptions", "Distribute", 0xFFFF );
	}

}


//-------------------------------------------------------------
//	SaveOptions
//-------------------------------------------------------------
void SaveOptions( void )
{
  CStdString iniFileName = DEFAULT_MAMEOXSYSTEMPATH "\\" INIFILENAME;
  CSystem_IniFile iniFile( iniFileName );

  iniFile.WriteProfileInt( "General", "CheatsEnabled", options.cheat );
  if( cheatfile )
    iniFile.WriteProfileString( "General", "CheatFilename", cheatfile );
  if( history_filename )
    iniFile.WriteProfileString( "General", "HistoryFilename", history_filename );

  iniFile.WriteProfileInt( "General", "SkipDisclaimer", options.skip_disclaimer );
  iniFile.WriteProfileInt( "General", "SkipGameInfo", options.skip_gameinfo );
  iniFile.WriteProfileInt( "General", "ScreenSaverTimeout", g_screensaverTimeout );


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
	iniFile.WriteProfileInt( "Video", "GraphicsFilter", g_rendererOptions.m_FilterType);
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
  iniFile.WriteProfileString( "Directories", "C_Mapping",          g_FileIOConfig.m_letterCMapping );
  iniFile.WriteProfileString( "Directories", "E_Mapping",          g_FileIOConfig.m_letterEMapping );
  iniFile.WriteProfileString( "Directories", "F_Mapping",          g_FileIOConfig.m_letterFMapping );
  iniFile.WriteProfileString( "Directories", "G_Mapping",          g_FileIOConfig.m_letterGMapping );
  iniFile.WriteProfileString( "Directories", "H_Mapping",          g_FileIOConfig.m_letterHMapping );
  iniFile.WriteProfileString( "Directories", "RomsPath0",          g_FileIOConfig.m_romPath0 );
  iniFile.WriteProfileString( "Directories", "RomsPath1",          g_FileIOConfig.m_romPath1 );
  iniFile.WriteProfileString( "Directories", "RomsPath2",          g_FileIOConfig.m_romPath2 );
  iniFile.WriteProfileString( "Directories", "RomsPath3",          g_FileIOConfig.m_romPath3 );
  iniFile.WriteProfileString("Directories", "ArtPath",             g_FileIOConfig.m_artPath );
  iniFile.WriteProfileString("Directories", "AudioPath",           g_FileIOConfig.m_audioPath );
  iniFile.WriteProfileString("Directories", "ConfigPath",          g_FileIOConfig.m_configPath );
  iniFile.WriteProfileString("Directories", "GeneralPath",         g_FileIOConfig.m_generalPath );
  iniFile.WriteProfileString("Directories", "HDImagePath",         g_FileIOConfig.m_HDImagePath );
  iniFile.WriteProfileString("Directories", "HiScoresPath",        g_FileIOConfig.m_HiScorePath );
  iniFile.WriteProfileString("Directories", "NVRamPath",           g_FileIOConfig.m_NVramPath );
  iniFile.WriteProfileString("Directories", "BackupPath",          g_FileIOConfig.m_romBackupPath );
  iniFile.WriteProfileString("Directories", "ScreenshotPath",      g_FileIOConfig.m_screenshotPath );
  iniFile.WriteProfileString("Directories", "AutoBootSavePath",    g_FileIOConfig.m_autoBootSavePath );
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
  iniFile.WriteProfileInt( "ROMListOptions", "DisplayMode", g_romListOptions.m_displayMode );
  iniFile.WriteProfileInt( "ROMListOptions", "SortMode", (UINT32)g_romListOptions.m_sortMode );
  iniFile.WriteProfileInt( "ROMListOptions", "ShowROMStatus", g_romListOptions.m_showROMStatus );
  iniFile.WriteProfileInt( "ROMListOptions", "HideFilteredROMs", g_romListOptions.m_hideFiltered );
  iniFile.WriteProfileInt( "ROMListOptions", "FilterMode", g_romListOptions.m_filterMode );
//  iniFile.WriteProfileInt( "ROMListOptions", "Filter_NumPlayers", g_romListOptions.m_numPlayersFilter );

  iniFile.WriteProfileFloat( "ROMListOptions", "CursorPosition", g_persistentLaunchData.m_cursorPosition );
  iniFile.WriteProfileFloat( "ROMListOptions", "PageOffset", g_persistentLaunchData.m_pageOffset );
  iniFile.WriteProfileInt(   "ROMListOptions", "SuperscrollIndex", g_persistentLaunchData.m_superscrollIndex );

		//-- Skin Options ------------------------------------------------------------------------
	iniFile.WriteProfileString( "SkinOptions", "SelectedSkin", g_skinOptions.m_currentSkin );


	//-- VMM Options ------------------------------------------------------------------------

	iniFile.WriteProfileInt( "VMMOptions", "ForceVMM", g_forcevmem );
	iniFile.WriteProfileInt( "VMMOptions", "Threshold", g_vmemThreshold );
	iniFile.WriteProfileInt( "VMMOptions", "CommitSize", g_vmemCommitSize );
	iniFile.WriteProfileInt( "VMMOptions", "Distribute", g_vmemDistribute );




}

//-------------------------------------------------------------
// RemapDriveLetters
//-------------------------------------------------------------
void RemapDriveLetters( void )
{
  MAPDRIVE( g_FileIOConfig.m_letterCMapping.c_str(), "\\??\\C:" );
  MAPDRIVE( g_FileIOConfig.m_letterEMapping.c_str(), "\\??\\E:" );
  MAPDRIVE( g_FileIOConfig.m_letterFMapping.c_str(), "\\??\\F:" );
  MAPDRIVE( g_FileIOConfig.m_letterGMapping.c_str(), "\\??\\G:" );
  MAPDRIVE( g_FileIOConfig.m_letterHMapping.c_str(), "\\??\\H:" );

  #ifdef _DEBUG
    WIN32_FIND_DATA findData;

    HANDLE h = FindFirstFile( "D:\\*", &findData );
    if( h != INVALID_HANDLE_VALUE )
    {
      do {
        PRINTMSG(( T_INFO, "D:\\%s", findData.cFileName ));
      } while( FindNextFile( h, &findData ) );
      FindClose( h );
    }

    h = FindFirstFile( "E:\\*", &findData );
    if( h != INVALID_HANDLE_VALUE )
    {
      do {
        PRINTMSG(( T_INFO, "E:\\%s", findData.cFileName ));
      } while( FindNextFile( h, &findData ) );
      FindClose( h );
    }

    h = FindFirstFile( "G:\\*", &findData );
    if( h != INVALID_HANDLE_VALUE )
    {
      do {
        PRINTMSG(( T_INFO, "G:\\%s", findData.cFileName ));
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
	static int waspressed = 0 ;
	BOOL bShotFired;
	BOOL bShotHitScreen;
	BOOL bShotMissedScreen;

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

	g_lightgunGame.hDevice = gp->m_deviceHandle ;
	g_lightgunGame.sThumbLX = gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ) ;
	g_lightgunGame.sThumbLY = gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y ) ;
	g_lightgunGame.wButtons = gp->GetGamepadDeviceState()->wButtons ;

	if ( gp->IsButtonPressed( GP_A ) )
	{
		if ( waspressed )
		{
			g_lightgunGame.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = FALSE ;
		}
		else
		{
			g_lightgunGame.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = TRUE ;
		}
		waspressed = 1;
	}
	else
	{
		g_lightgunGame.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = FALSE ;
		waspressed = 0 ;
	}


	g_lightgunGame.VerifyCalibrationState( FALSE );

	g_lightgunGame.Update( &bShotFired, &bShotHitScreen, &bShotMissedScreen );


	if ( bShotFired )
	{
		if ( bShotMissedScreen )
		{
		}
	}



	FLOAT fWidth = 640.0f ;
	FLOAT fHeight = 480.0f ;
    //FLOAT fThumbLX = (fWidth/2) + (fWidth/2)*(g_lightgun.sThumbLX+0.5f)/32767.5f;
    //FLOAT fThumbLY = (fHeight/2) - (fHeight/2)*(g_lightgun.sThumbLY+0.5f)/32767.5f;
    FLOAT fThumbLX = (g_lightgunGame.sThumbLX+0.5f)/255.5f;
    FLOAT fThumbLY = (g_lightgunGame.sThumbLY+0.5f)/-255.5f;


	*deltax = fThumbLX ;
	*deltay = fThumbLY ;


    // Lock to the expected range
  if( *deltax > 128 )
    *deltax = 128;
  else if( *deltax < -128 )
    *deltax = -128;

  if( *deltay > 128 )
    *deltay = 128;
  else if( *deltay < -128 )
    *deltay = -128;



  //PRINTMSG_TO_CONSOLE((  T_INFO, 
                        //"x: %d:%d y: %d:%d", 
                        //*deltax, 
                        //gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ),
                        //*deltay,
                        //gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y )));
}


//-------------------------------------------------------------
//	GetLightgunCalibratedPosition
//-------------------------------------------------------------
void GetLightgunCalibratedPositionOld( UINT32 player, INT32 *deltax, INT32 *deltay )
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



  PRINTMSG_TO_CONSOLE((  T_INFO, 
                        "x: %d:%d y: %d:%d", 
                        *deltax, 
                        gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ),
                        *deltay,
                        gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y )));
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

//-------------------------------------------------------------
//  RenderProgressBar
//-------------------------------------------------------------
void RenderProgressBar( INT32 left, 
                        INT32 top, 
                        INT32 right, 
                        INT32 bottom, 
                        UINT32 curValue, 
                        UINT32 maxValue, 
                        D3DCOLOR barColor, 
                        D3DCOLOR borderColor,
												D3DCOLOR backgroundColor )
{
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pD3DDevice->SetRenderState( D3DRS_SRCBLEND,					D3DBLEND_SRCALPHA );
  pD3DDevice->SetRenderState( D3DRS_DESTBLEND,				D3DBLEND_INVSRCALPHA );
  pD3DDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
  pD3DDevice->SetTexture( 0, NULL );

  #define PROGRESSBAR_CAP_WIDTH     2

  #define DRAWQUAD( left, top, right, bottom, color ) \
        pD3DDevice->Begin( D3DPT_QUADLIST ); \
          pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, color ); \
          pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, left, top, 1.0f, 1.0f ); \
          pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, color ); \
          pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, right, top, 1.0f, 1.0f ); \
          pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, color ); \
          pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, right, bottom, 1.0f, 1.0f ); \
          pD3DDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, color ); \
          pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, left, bottom, 1.0f, 1.0f ); \
        pD3DDevice->End();

    // Render background for the bar
  DRAWQUAD( left, top, right, bottom, backgroundColor );


    // Draw left "cap"
  DRAWQUAD( left, 
            top,
            left + PROGRESSBAR_CAP_WIDTH,
            bottom,
            borderColor );


    // Draw right "cap"
  DRAWQUAD( right - PROGRESSBAR_CAP_WIDTH, 
            top,
            right,
            bottom,
            borderColor );

    // Draw an outline around the bar
  DRAWQUAD( left, 
            top,
            right,
            top + PROGRESSBAR_CAP_WIDTH,
            borderColor );

  DRAWQUAD( left, 
            bottom - PROGRESSBAR_CAP_WIDTH,
            right,
            bottom,
            borderColor );

  if( maxValue == BUBBLEBAR )
  {
    #define PROGRESSBAR_BUBBLE_WIDTH   15

      // Just have a "bubble" that scrolls from left to right, reappearing on the left when it scrolls
      // off the edge
    UINT32 xOffset = (curValue << 1) % (((right-left) - (PROGRESSBAR_CAP_WIDTH<<1)) - PROGRESSBAR_BUBBLE_WIDTH);
    DRAWQUAD( left + PROGRESSBAR_CAP_WIDTH + xOffset, 
              top + PROGRESSBAR_CAP_WIDTH,
              left + PROGRESSBAR_CAP_WIDTH + xOffset + PROGRESSBAR_BUBBLE_WIDTH,
              bottom - PROGRESSBAR_CAP_WIDTH,
              barColor );
  }
  else
  {
      // Draw the bar
    UINT32 xOffset = (FLOAT)curValue * ( ( (FLOAT)((right-left) - (PROGRESSBAR_CAP_WIDTH<<1)) / (FLOAT)maxValue ) );
    DRAWQUAD( left + PROGRESSBAR_CAP_WIDTH, 
              top + PROGRESSBAR_CAP_WIDTH,
              left + PROGRESSBAR_CAP_WIDTH + xOffset,
              bottom - PROGRESSBAR_CAP_WIDTH,
              barColor );
  }
}

//-------------------------------------------------------------
//  PresentFrame
//-------------------------------------------------------------
void PresentFrame( void )
{
  g_graphicsManager.GetD3DDevice()->Present( NULL, NULL, NULL, NULL );
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

  while( !(g_inputManager.IsAnyButtonPressed() || g_inputManager.IsAnyKeyPressed()) )
  {
    g_inputManager.PollDevices();
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

