/**
	* \file			xbox_Main.cpp
	* \brief		MAMEoX entry point
	*
	*/


//= I N C L U D E S ====================================================
#include "MAMEoX.h"

#include <stdarg.h>
#include <stdio.h>
#include <crtdbg.h>

#include <XGraphics.h>		// For XGWriteSurfaceToFile

#ifdef _PROFILER
#include <xbdm.h>
#endif

#include "InputManager.h"
#include "GraphicsManager.h"
#include "System_IniFile.h"
#include "xbox_FileIO.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_Timing.h"
#include "DebugLogger.h"
#include "xbox_Network.h"
#include "FontSet.h"

#include "ROMListScreen.h"
#include "HelpScreen.h"
#include "OptionsScreen.h"
#include "LightgunCalibrationScreen.h"
#include "StartMenuView.h"
#include "ScreensaverScreen.h"
#include "TVCalibrationScreen.h"
#include "SkinChooserScreen.h"

//= D E F I N E S =====================================================

  // Width of the START menu in pixels
#define STARTMENU_WIDTH     215


  // The maximum number of times to attempt to generate a working driver.list file
#define MAX_DRIVERLIST_GENERATION_ATTEMPTS    2

  // The deadzone for the screen usage percentage control (right analog)
#define SCREENRANGE_DEADZONE    15000


typedef enum viewmode 
{
  VIEW_ROMLIST,
  VIEW_OPTIONS,
  VIEW_LIGHTGUNCALIBRATOR,
  VIEW_HELP,
  VIEW_SCREENSETUP,
	VIEW_SKINCHOOSER
} viewmode;


  // User data tags for the start menu
typedef enum startmenuitemdata
{
  MI_REBOOT,
  MI_HELP,
  MI_OPTIONS,
  MI_LIGHTGUNCALIBRATION,
  MI_REMOVESELECTEDROM,
  MI_SCANFORROMS,
  MI_REFRESHROMMETADATA,
  MI_COPYSYSTEMFILESFROMDVD,
  MI_ROMLIST,
  MI_SCREENSETUP,
	MI_SKINCHOOSER
} startmenuitemdata;

//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
  FLOAT         tu, tv;   // The texture coordinates
};

//= G L O B A L = V A R S =============================================
  // Defined in MAMEoXUtil.cpp
extern CInputManager			g_inputManager;
extern CGraphicsManager	  g_graphicsManager;
extern CFontSet           g_fontSet;

  // XBE Launch data
static DWORD              g_launchDataType;
static LAUNCH_DATA        g_launchData;

static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

  //! The data for each driver
static MAMEDriverData_t   *g_driverData = NULL;
UINT32                    g_screensaverTimeout = 0; //!< Time before the screensaver turns on

extern "C" {
  // Fake options struct for load/store options
GameOptions options;

  // Fake filenames for load/store options
const char *cheatfile = NULL;
const char *history_filename = NULL;
}
  // Fake D3D renderer options
RendererOptions_t    g_rendererOptions;

  // Fake sound override toggle
BOOL g_soundEnabled = TRUE;

extern "C" {

unsigned int g_vmemThreshold = 0x400000;
unsigned int g_vmemCommitSize = 0x100000;
unsigned int g_vmemDistribute = 0xFFFF;
unsigned int g_forcevmem = 0 ;
}


  // Fake filter functions for GFilterManager (so it can be used to list
  //   filter names in the OptionsScreen)
void AdMame2x32( UINT8 *, UINT32, UINT8 *, UINT8 *, UINT32, INT32, INT32 ) {}
void AdMame2x( UINT8 *, UINT32, UINT8 *, UINT8 *, UINT32, INT32, INT32 ) {}



//= P R O T O T Y P E S ===============================================
BOOL CreateBackdrop( FLOAT xPosition, FLOAT yPosition, FLOAT xUsage, FLOAT yUsage );
void DestroyBackdrop( void );
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
extern "C" void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems );
static BOOL Helper_LoadDriverInfoFile( void );
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice );
static void Helper_SetStartMenuItems( CStartMenuView &startMenu, viewmode currentViewMode );
static void Helper_SaveOptionsAndReboot( LPDIRECT3DDEVICE8 pD3DDevice, CROMListScreen &, CSkinChooserScreen & );
static BOOL Helper_CopySystemFilesFromDVD( LPDIRECT3DDEVICE8 pD3DDevice );


//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();
	PRINTMSG(( T_INFO, "\r\nStartup\r\n" ));

  Enable128MegCaching();

  LoadOptions();      // Must be done before inputManager goes up (for lightgun calib data)
  SaveOptions();      // Write out a default INI if none existed
	InitializeFileIO(); // Initialize fileIO (must be done after LoadOptions!)

  srand( time(NULL) );

		// Initialize the graphics subsystem
	g_graphicsManager.Create( TRUE, FALSE );
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();
	pD3DDevice->SetRenderState( D3DRS_CULLMODE,         D3DCULL_NONE );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING,         FALSE );
  pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
  pD3DDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );

    // Set up texture engine
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );


    // Create the fonts or die
  if( !g_fontSet.Create() )
  {
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    DWORD retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
    Die( pD3DDevice, "Failed to launch the dashboard! 0x%X", retVal );
  }

		// Initialize the input subsystem
	g_inputManager.Create( 4, 0, TRUE );  // 4 controllers, no memory cards, keyboards allowed

	// Intialize the various MAME OSD-specific subsystems
	InitializeTiming();


  if( !g_NetworkConfig.m_networkDisabled )
    InitializeNetwork();
  else
  {
      // If any paths have been set to samba, revert them
      // to the defaults
    if( g_FileIOConfig.m_artPath.Left(6) == "smb://" )
      g_FileIOConfig.m_artPath = DEFAULT_ARTPATH;

    if( g_FileIOConfig.m_audioPath.Left(6) == "smb://" )
      g_FileIOConfig.m_audioPath = DEFAULT_AUDIOPATH;
    
    if( g_FileIOConfig.m_configPath.Left(6) == "smb://" )
      g_FileIOConfig.m_configPath = DEFAULT_CONFIGPATH;
        
    if( g_FileIOConfig.m_generalPath.Left(6) == "smb://" )
      g_FileIOConfig.m_generalPath = DEFAULT_GENERALPATH;
    
    if( g_FileIOConfig.m_HDImagePath.Left(6) == "smb://" )
      g_FileIOConfig.m_HDImagePath = DEFAULT_HDIMAGEPATH;
    
    if( g_FileIOConfig.m_HiScorePath.Left(6) == "smb://" )
      g_FileIOConfig.m_HiScorePath = DEFAULT_HISCOREPATH;
    
    if( g_FileIOConfig.m_NVramPath.Left(6) == "smb://" )
      g_FileIOConfig.m_NVramPath = DEFAULT_NVRAMPATH;
    
    if( g_FileIOConfig.m_romBackupPath.Left(6) == "smb://" )
      g_FileIOConfig.m_romBackupPath = DEFAULT_ROMBACKUPPATH;
    
    if( g_FileIOConfig.m_romPath0.Left(6) == "smb://" )
      g_FileIOConfig.m_romPath0 = DEFAULT_ROMPATH;

    if( g_FileIOConfig.m_romPath1.Left(6) == "smb://" )
      g_FileIOConfig.m_romPath1 = DEFAULT_ROMPATH;

    if( g_FileIOConfig.m_romPath2.Left(6) == "smb://" )
      g_FileIOConfig.m_romPath2 = DEFAULT_ROMPATH;

    if( g_FileIOConfig.m_romPath3.Left(6) == "smb://" )
      g_FileIOConfig.m_romPath3 = DEFAULT_ROMPATH;
  }

		// Skin chooser
		// NOTE: The skin chooser _must_ be initialized before any object that uses
		//       the g_loadedSkin object!
	CSkinChooser skinChooser;
	skinChooser.m_options = g_skinOptions;
	if( !skinChooser.FindSkins() || !g_loadedSkin )
  {
    Die( pD3DDevice, 
         "The Media/Resource.xpr file is missing\n"
         "or damaged.\n"
         "The original file may be found in the\n"
         "Media directory of the MAMEoX package\n"
         "(it is autogenerated during the build\n"
         "process, so you may have to rebuild\n"
				 "to obtain it.)\n"
         "Please place the correct file in the\n"
         "Media directory on your XBOX and restart." );
  }



    // Wait for input to give the debugger a chance to attach
  CHECKRAM();

    // Get the launch data
  DWORD ret = XGetLaunchInfo( &g_launchDataType, &g_launchData );
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;

    // See who launched us
  if( ret != ERROR_SUCCESS || 
      g_launchDataType != LDT_TITLE || 
      mameoxLaunchData->m_command == LAUNCH_RUN_AS_IF_REBOOTED )
  {
		  // Show a splash screen if we were started from the dashboard and not from MAMEoX
	  ShowSplashScreen( pD3DDevice );

      // Create the MAME driver list
    if( g_launchDataType == LDT_TITLE && mameoxLaunchData->m_command == LAUNCH_RUN_AS_IF_REBOOTED )
    {
        // Preserve the number of driver list generation attempts so we'll bomb out
      DWORD generationAttempts = mameoxLaunchData->m_driverListGenerationAttempts;
      memset( mameoxLaunchData, 0, sizeof(*mameoxLaunchData) );
      mameoxLaunchData->m_driverListGenerationAttempts = generationAttempts;
    }
    else
      memset( mameoxLaunchData, 0, sizeof(*mameoxLaunchData) );
    mameoxLaunchData->m_command = LAUNCH_CREATE_MAME_GAME_LIST;

      // Try to load the MAME driver info, asking MAMEoX to produce
      // it if we can't
    if( !Helper_LoadDriverInfoFile() )
    {
      if( ++mameoxLaunchData->m_driverListGenerationAttempts > MAX_DRIVERLIST_GENERATION_ATTEMPTS )
        Die( pD3DDevice, "Failed to generate the driver list file.\nPerhaps you're out of disk space?" );

      ShowLoadingScreen( pD3DDevice );
      DWORD retVal = XLaunchNewImage( "D:\\MAMEoX.xbe", &g_launchData );
      Die( pD3DDevice, "Failed to launch D:\\MAMEoX.xbe! 0x%X", retVal );
    }

      // If we get this far, we know that the driver list hasn't been regenerated,
      // so we can load up the last known cursor position data
    mameoxLaunchData->m_cursorPosition   = g_persistentLaunchData.m_cursorPosition;
    mameoxLaunchData->m_pageOffset       = g_persistentLaunchData.m_pageOffset;
    mameoxLaunchData->m_superscrollIndex = g_persistentLaunchData.m_superscrollIndex;
  }
  else
  {
      // Load the driver info file
    if( !Helper_LoadDriverInfoFile() )
      Die( pD3DDevice, "Could not load driver info file!" );
  }

  // At this point the MAMEoX process is guaranteed to have run, setting up
  // our totalMAMEGames member, as well as producing the driver info file

    // Check the clock and throw up a warning screen if it hasn't been set
    // A number of MAME ROMs require a valid clock setting to emulate correctly
	time_t ltime;
	time(&ltime);
	if( !localtime(&ltime) )
  {
    while( !g_inputManager.IsAnyButtonPressed() && !g_inputManager.IsAnyKeyPressed() )
    {
      g_inputManager.PollDevices();
      DrawProgressbarMessage( pD3DDevice, 
                              "Your system clock is not set!\n\n"
                              "This can cause some games to\n"
                              "run improperly or crash!\n"
                              "Please reboot your XBOX and set\n"
                              "the system clock in the dashboard.", 
                              NULL, 
                              0xFFFFFFFF, 
                              0 );
    }
	  g_inputManager.WaitForNoInput();        
  }
  




    //--- Create the views -------------------------------------------------------
  CHelpScreen help( pD3DDevice, g_fontSet );
	if( !help.LoadHelpFile() )
  {
    Die( pD3DDevice, 
         "The Media/help.txt file is missing or damaged.\n"
         "The original file may be found in the Media directory\n"
         "of the MAMEoX package.\n"
         "Please place the correct file in the Media\n"
         "directory on your XBOX and restart." );
  }


		// Load/Generate the ROM listing
  CROMListScreen romList( pD3DDevice, 
                    g_fontSet,
                    g_driverData, 
                    mameoxLaunchData->m_totalMAMEGames );
	if( !romList.LoadROMList( TRUE ) )
		Die( pD3DDevice, "Could not generate ROM list!" );
  romList.SetOptions( g_romListOptions );

    // Reposition the romList cursor to its previous value
  romList.SetCursorPosition(  mameoxLaunchData->m_cursorPosition, 
                              mameoxLaunchData->m_pageOffset,
                              mameoxLaunchData->m_superscrollIndex );

    // Grab the current screen usage so we can render a border
  FLOAT xPercentage, yPercentage, xPos, yPos;
  GetScreenPosition( &xPos, &yPos );
  GetScreenUsage( &xPercentage, &yPercentage );
  CreateBackdrop( xPos, yPos, xPercentage, yPercentage );

  COptionsScreen optionsPage( pD3DDevice,
                              g_fontSet,
                              options );

  CLightgunCalibrationScreen lightgunCalibrator( pD3DDevice, g_fontSet );

  RECT area = { 320 - (STARTMENU_WIDTH>>1), 0, 320 + (STARTMENU_WIDTH>>1), 480 };
  CStartMenuView startMenu( pD3DDevice,
                        g_fontSet,
                        area );
  Helper_SetStartMenuItems( startMenu, VIEW_ROMLIST );


    // Screensaver object
  CScreensaverScreen screenSaver( pD3DDevice, g_fontSet, g_driverData, mameoxLaunchData->m_totalMAMEGames );
  screenSaver.FindScreenshots();

    // TV calibration object
  CTVCalibrationScreen TVCalibrationScreen( pD3DDevice, g_fontSet );

		// Skin chooser view object
	CSkinChooserScreen skinChooserScreen( pD3DDevice, g_fontSet, skinChooser );
  skinChooserScreen.SetOptions( g_skinOptions );



    //-- Initialize the rendering engine -------------------------------
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );

    // Create a texture for the render target
  LPDIRECT3DTEXTURE8 renderTargetTexture = NULL;
  if( FAILED( D3DXCreateTexture(  pD3DDevice,
                                  640, 
                                  480, 
                                  1, 
                                  0L, 
                                  D3DFMT_LIN_X8R8G8B8, 
                                  D3DPOOL_DEFAULT, 
                                  &renderTargetTexture ) ) )
  {
    Die( pD3DDevice, "Fatal error while creating render target texture!" );
  }


    // The view that should be displayed
  viewmode  currentView = VIEW_ROMLIST;
  BOOL      showStartMenu = FALSE;

	UINT64 lastTime = osd_cycles();

    // Store the current screen rotation value so we can
    // rotate as soon as we notice that the option is changed
  screenrotation_t oldRotation = g_rendererOptions.m_screenRotation;

    // Right analog movement mode. If this is false, right analog
    // stretches the screen. If true, it moves it.
  BOOL rightAnalogMovesScreen = FALSE;

    // Note: Setting g_screensaverTimeout to 0 disables it
  #define SCREENSAVERDELAYCYCLES      ((UINT64)g_screensaverTimeout * (UINT64)60 * osd_cycles_per_second())
  UINT64 screensaverTimeout = SCREENSAVERDELAYCYCLES;
  UINT32 lastScreensaverTimeout = g_screensaverTimeout;

		//--- Main loop ------------------------------------------------------
	while( 1 )
	{
		g_inputManager.PollDevices();

	  UINT64 curTime = osd_cycles();
    UINT64 elapsedTicks = curTime - lastTime;
	  DOUBLE elapsedTime = (DOUBLE)elapsedTicks / (DOUBLE)osd_cycles_per_second();

    if( g_inputManager.IsAnyInput() )
    {
      BOOL eatInput = (screensaverTimeout == 0 && g_screensaverTimeout);
      screensaverTimeout = SCREENSAVERDELAYCYCLES;
      if( eatInput )
      {
        g_inputManager.WaitForNoInput();
        continue;
      }
    }
    else if( screensaverTimeout && g_screensaverTimeout )
    {
      if( elapsedTicks >= screensaverTimeout )
        screensaverTimeout = 0;
      else
        screensaverTimeout -= elapsedTicks;
    }    
	  lastTime = curTime;

			// Reboot on LT+RT+Black
    BYTE rebootSequence[3] = { VK_LMENU, VK_LCONTROL, VK_DELETE };
    if( g_inputManager.IsButtonPressed( GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER | GP_BLACK ) ||
        g_inputManager.AreAllOfKeysPressed( rebootSequence, 3 ) )
      Helper_SaveOptionsAndReboot( pD3DDevice, romList, skinChooserScreen );


    if( g_inputManager.IsOnlyButtonPressed( GP_LEFT_ANALOG ) || g_inputManager.IsOnlyKeyPressed( VK_PRINT ) )
    {
      static UINT32 screenshotIndex = 0;

        // Take a screenshot
      LPDIRECT3DSURFACE8 target;
      pD3DDevice->GetRenderTarget( &target );

      CStdString filename;
      filename.Format( "D:\\screenshots\\_%lu.bmp", screenshotIndex++ );
      XGWriteSurfaceToFile( target, filename.c_str() );

      SAFE_RELEASE( target );
    }

    if( !showStartMenu && 
        (g_inputManager.IsOnlyButtonPressed( GP_START ) || g_inputManager.IsOnlyKeyPressed( VK_ESCAPE )) )
    {
        // Enter the main menu
      showStartMenu = TRUE;
      g_inputManager.WaitForNoButton();
    }
    else if( g_inputManager.IsOnlyButtonPressed( GP_X ) || g_inputManager.IsOnlyKeyPressed( VK_F1 ) )
    {
      if( currentView != VIEW_HELP )
        currentView = VIEW_HELP;
      else
        currentView = VIEW_ROMLIST;

      startMenu.Reset();
      Helper_SetStartMenuItems( startMenu, currentView );
      g_inputManager.WaitForNoButton();
    }

    if( oldRotation != g_rendererOptions.m_screenRotation ||
        TVCalibrationScreen.AreScreenSettingsChanged() )
    {
      FLOAT xPos, yPos, xPercentage, yPercentage;
      GetScreenUsage( &xPercentage, &yPercentage );
      GetScreenPosition( &xPos, &yPos );

      DestroyBackdrop();
      CreateBackdrop( xPos, yPos, xPercentage, yPercentage );
      oldRotation = g_rendererOptions.m_screenRotation;
      TVCalibrationScreen.Reset();
    }



			// Move the cursor position and render
    pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
    pD3DDevice->SetTransform( D3DTS_VIEW, &matWorld );
    pD3DDevice->SetTransform( D3DTS_PROJECTION, &matWorld );

	  pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	  pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	  pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

    pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
		pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
		pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );
    
      // Render whatever screen happens to be active to the
      // texture
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET,	                // Flags
                        D3DCOLOR_XRGB(105,105,105),				// Color
											  1.0f,															// Z
											  0L );															// Stencil

    switch( currentView )
    {
      // *** VIEW_ROMLIST *** //
    case VIEW_ROMLIST:
      if( !showStartMenu )
  		  romList.MoveCursor( g_inputManager );
		  romList.DrawToTexture( renderTargetTexture );
      break;

      // *** VIEW_OPTIONS *** //
    case VIEW_OPTIONS:
      if( !showStartMenu )
        optionsPage.MoveCursor( g_inputManager );
      optionsPage.DrawToTexture( renderTargetTexture );
      break;

      // *** VIEW_LIGHTGUNCALIBRATOR *** //
    case VIEW_LIGHTGUNCALIBRATOR:
      if( !showStartMenu )
        lightgunCalibrator.MoveCursor( g_inputManager );
      lightgunCalibrator.DrawToTexture( renderTargetTexture );
      if( lightgunCalibrator.IsCalibrationCompleted() )
      {
        currentView = VIEW_ROMLIST;
        lightgunCalibrator.Reset();
        startMenu.Reset();
        Helper_SetStartMenuItems( startMenu, currentView );
      }      
      break;

      // *** VIEW_HELP *** //
    case VIEW_HELP:
      if( !showStartMenu )
        help.MoveCursor( g_inputManager );
      help.DrawToTexture( renderTargetTexture );
      break;

      // *** VIEW_SCREENSETUP *** //
    case VIEW_SCREENSETUP:
      if( !showStartMenu )
        TVCalibrationScreen.MoveCursor( g_inputManager );
      if( TVCalibrationScreen.IsCalibrationCompleted() )
      {
        if( TVCalibrationScreen.AreScreenSettingsChanged() )
        {
          FLOAT xPos, yPos, xPercentage, yPercentage;
          GetScreenUsage( &xPercentage, &yPercentage );
          GetScreenPosition( &xPos, &yPos );
          DestroyBackdrop();
          CreateBackdrop( xPos, yPos, xPercentage, yPercentage );
        }

        currentView = VIEW_ROMLIST;
        TVCalibrationScreen.Reset();
        startMenu.Reset();
        Helper_SetStartMenuItems( startMenu, currentView );
      }      
      break;


      // *** VIEW_SKINCHOOSER *** //
		case VIEW_SKINCHOOSER:
      if( !showStartMenu )
        skinChooserScreen.MoveCursor( g_inputManager );
      skinChooserScreen.DrawToTexture( renderTargetTexture );
			break;
    }

      // Now render the texture to the screen 
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                        D3DCOLOR_XRGB(105,105,105),				// Color
											  1.0f,															// Z
											  0L );															// Stencil




    pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX1 );
    pD3DDevice->SetStreamSource(	0,												  // Stream number
																	g_pD3DVertexBuffer,	        // Stream data
																	sizeof(CUSTOMVERTEX) );		  // Vertex stride
	  pD3DDevice->SetTexture( 0, renderTargetTexture );
    pD3DDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

      // Render the TV calibration screen (has to run fullscreen)
    if( currentView == VIEW_SCREENSETUP )
      TVCalibrationScreen.Draw( TRUE, FALSE );

      // Now render the start menu over that, if necessary
    if( showStartMenu )
    {
      startMenu.MoveCursor( g_inputManager );
      startMenu.Draw( FALSE, FALSE );
      if( startMenu.IsInputFinished() )
      {
        showStartMenu = FALSE;
        if( startMenu.IsInputAccepted() )
        {
          switch( startMenu.GetUserDataAtCursorPosition() )
          {
            // *** MI_REBOOT *** //
          case MI_REBOOT:
            Helper_SaveOptionsAndReboot( pD3DDevice, romList, skinChooserScreen );
            break;

            // *** MI_HELP *** //
          case MI_HELP:
            currentView = VIEW_HELP;
            break;

            // *** MI_OPTIONS *** //
          case MI_OPTIONS:
              // Update the rom list options for display in the options page
            g_romListOptions = romList.GetOptions();
            currentView = VIEW_OPTIONS;
            break;

            // *** MI_LIGHTGUNCALIBRATION *** //
          case MI_LIGHTGUNCALIBRATION:
            currentView = VIEW_LIGHTGUNCALIBRATOR;
            break;

            // *** MI_REMOVESELECTEDROM *** //
          case MI_REMOVESELECTEDROM:
            romList.MoveCurrentGameToBackupDir();
            break;

            // *** MI_SCANFORROMS *** //
          case MI_SCANFORROMS:
            romList.GenerateROMList();
            break;

            // *** MI_REFRESHROMMETADATA *** //
          case MI_REFRESHROMMETADATA:
            romList.RefreshROMStatus();
            romList.ImportCatverINI();
            break;

            // *** MI_COPYSYSTEMFILESFROMDVD *** //
          case MI_COPYSYSTEMFILESFROMDVD:
            Helper_CopySystemFilesFromDVD( pD3DDevice );
            break;  

            // *** MI_ROMLIST *** //
          case MI_ROMLIST:
            romList.SetOptions( g_romListOptions );
            currentView = VIEW_ROMLIST;            
            break;

            // *** MI_SCREENSETUP *** //
          case MI_SCREENSETUP:
            currentView = VIEW_SCREENSETUP;
            break;

            // *** MI_SKINCHOOSER *** //
					case MI_SKINCHOOSER:
						currentView = VIEW_SKINCHOOSER;
						break;
          }
        }
        startMenu.Reset();
        Helper_SetStartMenuItems( startMenu, currentView );
      }
    }

      // If the screensaver option is changed from "disabled" to anything, we
      // have to reset the screensaverTimeout timer to avoid "blinking" the
      // screensaver
    if( lastScreensaverTimeout != g_screensaverTimeout )
    {
      screensaverTimeout = SCREENSAVERDELAYCYCLES;
      lastScreensaverTimeout = g_screensaverTimeout;
    }
    else if( g_screensaverTimeout && screensaverTimeout == 0.0 )
    {
        // Render the screensaver
      screenSaver.Draw( TRUE, FALSE );
    }

      // Render the debug console
    RenderDebugConsole( pD3DDevice );

    pD3DDevice->Present( NULL, NULL, NULL, NULL );


      // Run the user selected ROM
    if( romList.IsGameSelected() )
    {
      ShowLoadingScreen( pD3DDevice );

        // Pack info to be passed to MAMEoX
      mameoxLaunchData->m_gameIndex = romList.GetCurrentGameIndex();
      romList.GetCursorPosition(  &mameoxLaunchData->m_cursorPosition, 
                                  &mameoxLaunchData->m_pageOffset,
                                  &mameoxLaunchData->m_superscrollIndex );
      mameoxLaunchData->m_command = LAUNCH_RUN_GAME;

      g_romListOptions = romList.GetOptions();
      romList.GetCursorPosition(  &g_persistentLaunchData.m_cursorPosition, 
                                  &g_persistentLaunchData.m_pageOffset,
                                  &g_persistentLaunchData.m_superscrollIndex );

			g_skinOptions = skinChooserScreen.GetOptions();

      SaveOptions();
      romList.SaveROMMetadataFile();
      DWORD retVal = XLaunchNewImage( "D:\\MAMEoX.xbe", &g_launchData );
      Die( pD3DDevice, "Failed to launch D:\\MAMEoX.xbe! 0x%X", retVal );
    }
    else if( romList.ShouldGenerateROMList() )
    {
      romList.GenerateROMList();
		  WaitForNoButton();
    }

	}
}



//-------------------------------------------------------------
// Helper_SetStartMenuItems
//-------------------------------------------------------------
static void Helper_SetStartMenuItems( CStartMenuView &startMenu, viewmode currentView )
{
  switch( currentView )
  {
    // *** VIEW_ROMLIST *** //
  case VIEW_ROMLIST:
    startMenu.SetTitle( ":: ROM List ::" );
    startMenu.AddMenuItem( "Help", MI_HELP );
    startMenu.AddMenuItem( "Options Menu", MI_OPTIONS );
    startMenu.AddMenuItem( "Screen Setup", MI_SCREENSETUP );
      // Check to see if a lightgun is inserted at all
    {
      BOOL showLightgunCalibratorOpt = FALSE;
      for( UINT32 i = 0; i < 4; ++i )
      {
        CGamepad *gp = g_inputManager.GetGamepad( i );
        if( gp && gp->IsLightgun() )
        {
          showLightgunCalibratorOpt = TRUE;
          break;
        }
      }
      if( showLightgunCalibratorOpt )
        startMenu.AddMenuItem( "Lightgun Calibration", MI_LIGHTGUNCALIBRATION );
    }

    startMenu.AddMenuItem( "Remove Selected ROM", MI_REMOVESELECTEDROM );
    startMenu.AddMenuItem( "Scan for ROMs", MI_SCANFORROMS );
    startMenu.AddMenuItem( "Refresh ROM metadata", MI_REFRESHROMMETADATA );

      // Check to see if we're running off a DVD
      // This is sort of a hack, we depend on the config path
      // being set to the ALTDrive, as it's one of the directories
      // that must be writable (and will be reassigned if we're running
      // on readonly media)
    if( g_FileIOConfig.m_configPath[0] == g_FileIOConfig.m_ALTDrive[0] )
      startMenu.AddMenuItem( "Copy system files from DVD", MI_COPYSYSTEMFILESFROMDVD );

		//startMenu.AddMenuItem( "Choose Skin", MI_SKINCHOOSER );

    break;

    // *** VIEW_OPTIONS *** //
  case VIEW_OPTIONS:
    startMenu.SetTitle( ":: Options Menu ::" );
    startMenu.AddMenuItem( "ROM List", MI_ROMLIST );
    startMenu.AddMenuItem( "Help", MI_HELP );
    break;

    // *** VIEW_LIGHTGUNCALIBRATOR *** //
  case VIEW_LIGHTGUNCALIBRATOR:
    startMenu.SetTitle( ":: Lightgun Calibrator ::" );
    startMenu.AddMenuItem( "ROM List", MI_ROMLIST );
    startMenu.AddMenuItem( "Help", MI_HELP );
    break;

    // *** VIEW_HELP *** //
  case VIEW_HELP:
    startMenu.SetTitle( ":: Help ::" );
    startMenu.AddMenuItem( "ROM List", MI_ROMLIST );
    break;

    // *** VIEW_SCREENSETUP *** //
  case VIEW_SCREENSETUP:
    startMenu.SetTitle( ":: Screen Setup ::" );
    startMenu.AddMenuItem( "ROM List", MI_ROMLIST );
    startMenu.AddMenuItem( "Help", MI_HELP );
    break;


    // *** VIEW_SKINCHOOSER *** //
  case VIEW_SKINCHOOSER:
    startMenu.SetTitle( ":: Skin Chooser ::" );
    startMenu.AddMenuItem( "ROM List", MI_ROMLIST );
    break;
  }

  startMenu.AddMenuItem( "Return to dashboard", MI_REBOOT );
}


//-------------------------------------------------------------
// Helper_LoadDriverInfoFile
//-------------------------------------------------------------
static BOOL Helper_LoadDriverInfoFile( void )
{
	DWORD BytesRead = 0;
	DWORD len;
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  CStdString driverListFile = DEFAULT_MAMEOXSYSTEMPATH "\\" DRIVERLISTFILENAME;
	PRINTMSG(( T_INFO, "Load driver list: %s", driverListFile.c_str() ));

  osd_file *file = osd_fopen( FILETYPE_MAMEOX_SYSTEM, 0, DRIVERLISTFILENAME, "r" );
	if( !file )
	{
		PRINTMSG(( T_ERROR, "Could not open file %s!", driverListFile.c_str() ));
		return FALSE;
	}

    // Grab the entire file at once so we can check its signature
  osd_fseek( file, 0, SEEK_END );
  INT32 fileSize = osd_ftell( file );
  osd_fseek( file, 0, SEEK_SET );
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize );
  if( !fileData )
  {
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              driverListFile.c_str(), 
              fileSize ));
    return FALSE;
  }

    // Read the entire file into memory
  if( osd_fread( file, fileData, fileSize ) != fileSize )
  {
    free( fileData );
    osd_fclose( file );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to read file %s!", driverListFile.c_str() ));
    return FALSE;
  }
  osd_fclose( file );

    // Check the signature
  if( memcmp( fileData, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Bad file signature!" ));
    return FALSE;
  }

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not calculate driver list signature!" ));
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Could not allocate memory for driver list signature!" ));
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed to calculate data signature!" ));
    return FALSE;
  }
  
    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Failed getting signature!" ));
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG(( T_ERROR, "Data signature mismatch!" ));
    return FALSE;
  }
  free( sigData );


    // Define a macro to "read" a block of data and ensure that we're not reading past the end of
    //  the file
  #define READDATA( _data__, _dataSize__, _dataType__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      if( g_driverData ) \
        free( g_driverData ); \
      free( fileData ); \
      DeleteFile( driverListFile.c_str() ); \
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) && !((_data__) = (_dataType__##*)malloc( (_dataSize__) )) ) \
      { \
        if( g_driverData ) \
          free( g_driverData ); \
        free( fileData ); \
        DeleteFile( driverListFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) )); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

  #define READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize ) \
    { \
      if( g_driverData ) \
        free( g_driverData ); \
      free( fileData ); \
      DeleteFile( driverListFile.c_str() ); \
      PRINTMSG(( T_ERROR, "Attempt to read beyond the end of the file!" )); \
      return FALSE; \
    } \
    else \
    { \
    if( !(_data__) ) \
      { \
        if( g_driverData ) \
          free( g_driverData ); \
        free( fileData ); \
        DeleteFile( driverListFile.c_str() ); \
        PRINTMSG(( T_ERROR, "Attempt to read into NULL destination buffer!" )); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }


    // Read in the number of drivers
  READDATA_NOMALLOC( &mameoxLaunchData->m_totalMAMEGames, sizeof(mameoxLaunchData->m_totalMAMEGames) );

    // Read in the driver list
    // The calloc is important here, so if this is ever changed to a new, ensure that all
    // string members are NULL'd out in the constructor of MAMEDriverData_t
  g_driverData = (MAMEDriverData_t*)calloc( 1, sizeof(MAMEDriverData_t) * mameoxLaunchData->m_totalMAMEGames );
  for( DWORD i = 0; i < mameoxLaunchData->m_totalMAMEGames; ++i )
  {
    READDATA_NOMALLOC( &g_driverData[i].m_index, sizeof( g_driverData[i].m_index ) );

      // Read the filename length
    READDATA_NOMALLOC( &len, sizeof( len ) );

      // Read the filename data
    READDATA( g_driverData[i].m_romFileName, len, char );

      // Read the description length
    READDATA_NOMALLOC( &len, sizeof( len ) );

      // Read the description data
    READDATA( g_driverData[i].m_description, len, char );

      // Read the clone status
    READDATA_NOMALLOC( &g_driverData[i].m_isClone, sizeof(g_driverData[i].m_isClone) );

			// Read the clonename length
    READDATA_NOMALLOC( &len, sizeof( len ) );

      // Read the clonename
    READDATA( g_driverData[i].m_cloneFileName, len, char );

			// Read the manufacturer length
    READDATA_NOMALLOC( &len, sizeof( len ) );

      // Read the manufacturer
    READDATA( g_driverData[i].m_manufacturer, len, char );

			// Read the year length
    READDATA_NOMALLOC( &len, sizeof( len ) );

      // Read the year
    READDATA( g_driverData[i].m_year, len, char );

      // Read the number of players
    READDATA_NOMALLOC( &g_driverData[i].m_numPlayers, sizeof(g_driverData[i].m_numPlayers) );

    READDATA_NOMALLOC( &g_driverData[i].m_screenOrientation, sizeof(g_driverData[i].m_screenOrientation) );
  }
  free( fileData );

  return TRUE;
}

//-------------------------------------------------------------
//	Die
//-------------------------------------------------------------
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... )
{
	char buf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

	PRINTMSG(( T_ERROR, "Die: %s", buf ));

		// Display the error to the user
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	g_fontSet.DefaultFont().Begin();
  	
	  WCHAR wBuf[1024];
	  mbstowcs( wBuf, buf, strlen(buf) + 1 );

		g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_XRGB( 255, 255, 255), wBuf, XBFONT_CENTER_X );
		g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_XRGB( 255, 255, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	g_fontSet.DefaultFont().End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );

	g_inputManager.WaitForNoButton( 0 );
	g_inputManager.WaitForAnyButton( 0 );
	g_inputManager.WaitForNoButton( 0 );

    // Reboot
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  DWORD retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );

  g_fontSet.DefaultFont().Begin();
    swprintf( wBuf, L"Failed to launch the dashboard! 0x%X", retVal );
		g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_XRGB( 255, 255, 255), wBuf, XBFONT_CENTER_X );
		g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_XRGB( 255, 255, 255), L"You need to power off manually!", XBFONT_CENTER_X );
	g_fontSet.DefaultFont().End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );
}



//-------------------------------------------------------------
//  CreateBackdrop
//-------------------------------------------------------------
BOOL CreateBackdrop( FLOAT xPos, FLOAT yPos, FLOAT xUsage, FLOAT yUsage )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }

    // Create the vertex buffer
  g_graphicsManager.GetD3DDevice()->CreateVertexBuffer( (sizeof(CUSTOMVERTEX) << 2),
																		                    D3DUSAGE_WRITEONLY,
																		                    D3DFVF_XYZ | D3DFVF_TEX1,
																		                    D3DPOOL_MANAGED,
																		                    &g_pD3DVertexBuffer );

	CUSTOMVERTEX *pVertices;
	g_pD3DVertexBuffer->Lock( 0,										// Offset to lock
														0,										// Size to lock
														(BYTE**)&pVertices,		// ppbData
														0 );									// Flags

      //-- Draw the backdrop -------------------------------------------------
		pVertices[0].pos.x = -xUsage;
		pVertices[0].pos.y = yUsage;
		pVertices[0].pos.z = 1.0f;

		pVertices[1].pos.x = xUsage;
		pVertices[1].pos.y = yUsage;
		pVertices[1].pos.z = 1.0f;
		
		pVertices[2].pos.x = xUsage;
		pVertices[2].pos.y = -yUsage;
		pVertices[2].pos.z = 1.0f;
		
		pVertices[3].pos.x = -xUsage;
		pVertices[3].pos.y = -yUsage;
		pVertices[3].pos.z = 1.0f;

		pVertices[0].pos.x += xPos;
		pVertices[0].pos.y += yPos;
		pVertices[1].pos.x += xPos;
		pVertices[1].pos.y += yPos;
		pVertices[2].pos.x += xPos;
		pVertices[2].pos.y += yPos;
		pVertices[3].pos.x += xPos;
		pVertices[3].pos.y += yPos;
    

    #define TU_LEFT       0.0f
    #define TU_RIGHT      640.0f
    #define TV_TOP        0.0f
    #define TV_BOTTOM     480.0f

    switch( g_rendererOptions.m_screenRotation )
    {
    case SR_0:
      pVertices[0].tu = TU_LEFT;
      pVertices[0].tv = TV_TOP;
      pVertices[1].tu = TU_RIGHT;
      pVertices[1].tv = TV_TOP;
      pVertices[2].tu = TU_RIGHT;
      pVertices[2].tv = TV_BOTTOM;
      pVertices[3].tu = TU_LEFT;
      pVertices[3].tv = TV_BOTTOM;
      break;

    case SR_90:
      pVertices[0].tu = TU_LEFT;
      pVertices[0].tv = TV_BOTTOM;
      pVertices[1].tu = TU_LEFT;
      pVertices[1].tv = TV_TOP;
      pVertices[2].tu = TU_RIGHT;
      pVertices[2].tv = TV_TOP;
      pVertices[3].tu = TU_RIGHT;
      pVertices[3].tv = TV_BOTTOM;
      break;

    case SR_180:
      pVertices[0].tu = TU_RIGHT;
      pVertices[0].tv = TV_BOTTOM;
      pVertices[1].tu = TU_LEFT;
      pVertices[1].tv = TV_BOTTOM;
      pVertices[2].tu = TU_LEFT;
      pVertices[2].tv = TV_TOP;
      pVertices[3].tu = TU_RIGHT;
      pVertices[3].tv = TV_TOP;
      break;

    case SR_270:
      pVertices[0].tu = TU_RIGHT;
      pVertices[0].tv = TV_TOP;
      pVertices[1].tu = TU_RIGHT;
      pVertices[1].tv = TV_BOTTOM;
      pVertices[2].tu = TU_LEFT;
      pVertices[2].tv = TV_BOTTOM;
      pVertices[3].tu = TU_LEFT;
      pVertices[3].tv = TV_TOP;
      break;
    }


	g_pD3DVertexBuffer->Unlock();


  return TRUE;
}

//-------------------------------------------------------------
//  DestroyBackdrop
//-------------------------------------------------------------
void DestroyBackdrop( void )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }
}



//-------------------------------------------------------------
//	ShowSplashScreen
//-------------------------------------------------------------
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
    // Create the vertex buffer
  struct CUSTOMVERTEX
  {
	  D3DXVECTOR3   pos;      // The transformed position for the vertex
    FLOAT         tu, tv;   // The texture coordinates
  };

	pD3DDevice->SetRenderState( D3DRS_CULLMODE,         D3DCULL_NONE );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING,         FALSE );
  pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  pD3DDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );

    // Set up texture engine
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );


  #define TEXTFADE_MINIMUM            0.0f
  #define TEXTFADE_MAXIMUM            110.0f
  #define TEXTFADE_FRAMES_PER_STEP    0.35f
  FLOAT textFadeColor = TEXTFADE_MINIMUM;
  FLOAT fadeDirection = 1.0f;

	D3DCOLOR normalTextColor = D3DCOLOR_XRGB( 0, 0, 0 );
	D3DCOLOR importantTextColor = D3DCOLOR_XRGB( 90, 105, 195 );

	if( CheckResourceValidity(SKINELEMENT_SPLASHSCREEN_TEXT) )
		normalTextColor = g_loadedSkin->GetSkinElementText(SKINELEMENT_SPLASHSCREEN_TEXT)->m_color;


	if( CheckResourceValidity(SKINELEMENT_SPLASHSCREEN_IMPORTANT_TEXT) )
		importantTextColor = g_loadedSkin->GetSkinElementText( SKINELEMENT_SPLASHSCREEN_IMPORTANT_TEXT )->m_color;

    //-- Set up the credits ------------------------------------------
  #define CREDITS_FRAMES_PER_STEP     0.5f;
  const WCHAR credits[] = L"       The MAMEoX team:"\
                          L"       Programming - Erik Abair, opcode, XPort" \
                          L"       Testing - falz, enkak, noodle1009, bmetz" \
                          L"       Graphical design - r4dius, Stephen Cameron" \
                          L"       Special thanks to Klaus of www.wolfsoft.de, and to MAME developers everywhere";

  FLOAT creditsPosition = 0.0f;
  FLOAT creditsLength, creditsHeight;
  g_fontSet.DefaultFont().GetTextExtent( credits, &creditsLength, &creditsHeight );

    // Create the credits texture
  LPDIRECT3DTEXTURE8 creditsTexture = g_fontSet.DefaultFont().CreateTexture(	credits, 
																																							D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
																																							D3DCOLOR_ARGB( 0xFF, 0, 0, 0 ) );

    // Area onscreen for the credits text
    // 115.0f,208.0f -> 524.0f,228.0f
  #define CREDITSBOX_LEFT       115.0f
  #define CREDITSBOX_RIGHT      524.0f
  #define CREDITSBOX_TOP        208.0f
  #define CREDITSBOX_BOTTOM     228.0f


    //-- Display everything -------------------------------------------------------
  while( !g_inputManager.IsAnyButtonPressed() && !g_inputManager.IsAnyKeyPressed() )
  {
    g_inputManager.PollDevices();

      // Bounce the fade color back and forth between MIN and MAX
    textFadeColor += fadeDirection / TEXTFADE_FRAMES_PER_STEP;
    if( textFadeColor <= TEXTFADE_MINIMUM )
    {
      fadeDirection = 1.0f;
      textFadeColor = TEXTFADE_MINIMUM;
    }
    else if( textFadeColor >= TEXTFADE_MAXIMUM )
    {
      fadeDirection = -1.0f;
      textFadeColor = TEXTFADE_MAXIMUM;
    }

		  // Clear the backbuffer
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET,	                // Flags
                        D3DCOLOR_ARGB(0,0,0,0),			      // Color
											  1.0f,															// Z
											  0L );															// Stencil


    pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		g_loadedSkin->SelectSkinTexture( pD3DDevice, ASSET_SPLASH_BACKDROP );
    pD3DDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
    pD3DDevice->Begin( D3DPT_QUADLIST );
		{
			const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( ASSET_SPLASH_BACKDROP );

      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );

      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );

      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );

      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
		}
    pD3DDevice->End();


      // Scroll the credits
    creditsPosition += 1.0f / CREDITS_FRAMES_PER_STEP;
    if( creditsPosition >= creditsLength )
      creditsPosition = (-(CREDITSBOX_RIGHT - CREDITSBOX_LEFT) + 1.0f);

    pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	  pD3DDevice->SetTexture( 0, creditsTexture );
    pD3DDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
    pD3DDevice->Begin( D3DPT_QUADLIST );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, creditsPosition, 0.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, CREDITSBOX_LEFT, CREDITSBOX_TOP, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, creditsPosition + (CREDITSBOX_RIGHT - CREDITSBOX_LEFT), 0.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, CREDITSBOX_RIGHT, CREDITSBOX_TOP, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, creditsPosition + (CREDITSBOX_RIGHT - CREDITSBOX_LEFT), creditsHeight );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, CREDITSBOX_RIGHT, CREDITSBOX_BOTTOM, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, creditsPosition, creditsHeight );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, CREDITSBOX_LEFT, CREDITSBOX_BOTTOM, 1.0f, 1.0f );
    pD3DDevice->End();




    g_fontSet.DefaultFont().Begin();    
      g_fontSet.DefaultFont().DrawText(  548, 
                        150, 
                        normalTextColor,
                        L"Version " LVERSION_STRING L" " LBUILDCONFIG_STRING, 
                        XBFONT_RIGHT );

	    g_fontSet.DefaultFont().DrawText(  320, 
                        410, 
                        D3DCOLOR_XRGB( (UINT8)textFadeColor, (UINT8)textFadeColor, (UINT8)textFadeColor ), 
                        L"Insert Coin", 
                        XBFONT_CENTER_X );
    g_fontSet.DefaultFont().End();

    g_fontSet.LargeThinFont().Begin();
			g_fontSet.LargeThinFont().DrawText( 320, 245, normalTextColor, L"MAME is distributed under the MAME license.", XBFONT_CENTER_X ); 
			g_fontSet.LargeThinFont().DrawText( 320, 265, importantTextColor, L"See www.mame.net/readme.html or", XBFONT_CENTER_X );
			g_fontSet.LargeThinFont().DrawText( 320, 285, importantTextColor, L"MAME/docs/mame.txt for details", XBFONT_CENTER_X );
	        
			g_fontSet.LargeThinFont().DrawText( 320, 315, normalTextColor, L"Portions of MAMEoX based on:", XBFONT_CENTER_X );
			g_fontSet.LargeThinFont().DrawText( 320, 335, importantTextColor, L"\"MAMEX(b5): updated by superfro,", XBFONT_CENTER_X );
			g_fontSet.LargeThinFont().DrawText( 320, 355, importantTextColor, L"original port by opcode\"", XBFONT_CENTER_X );
    g_fontSet.LargeThinFont().End();


    pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

	g_inputManager.WaitForNoInput();

  SAFE_RELEASE( creditsTexture );
}

//-------------------------------------------------------------
//	Helper_SaveOptionsAndReboot
//-------------------------------------------------------------
static void Helper_SaveOptionsAndReboot( LPDIRECT3DDEVICE8 pD3DDevice, CROMListScreen &romList, CSkinChooserScreen &skinChooserScreen )
{

	DrawProgressbarMessage( pD3DDevice, "Saving settings...", NULL, 0xFFFFFFFF, 0xFFFFFFFF );

  g_romListOptions = romList.GetOptions();
  romList.GetCursorPosition(  &g_persistentLaunchData.m_cursorPosition, 
                              &g_persistentLaunchData.m_pageOffset,
                              &g_persistentLaunchData.m_superscrollIndex );

	g_skinOptions = skinChooserScreen.GetOptions();


  SaveOptions();
  romList.SaveROMMetadataFile();

	DrawProgressbarMessage( pD3DDevice, "Returning to dashboard...", NULL, 0xFFFFFFFF, 0xFFFFFFFF );

  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  DWORD retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
  Die( pD3DDevice, "Failed to launch the dashboard! 0x%X", retVal );
}

//-------------------------------------------------------------
//	Helper_CopySystemFilesFromDVD
//-------------------------------------------------------------
static BOOL Helper_CopySystemFilesFromDVD( LPDIRECT3DDEVICE8 pD3DDevice )
{
    // The DVD is linked as the "D:", so just copy files from there to
    // the correct system path (which is already automatically set to
    // use altdrive)

  #define COPYDIR( dirName ) { \
      CStdString sourceStr, tempStr; \
      sourceStr = (dirName); \
      if( sourceStr[0] == g_FileIOConfig.m_ALTDrive[0] ) { \
        sourceStr[0] = 'd'; \
        sourceStr += "\\*"; \
        WIN32_FIND_DATA findData; \
        HANDLE h = FindFirstFile( sourceStr.c_str(), &findData ); \
        if( h != INVALID_HANDLE_VALUE ) { \
          UINT32 i = 0; \
          char message[128]; \
          sprintf( message, "Copying %.32s directory...", dirName ); \
          do { \
            sourceStr = (dirName); \
            sourceStr[0] = 'd'; \
            sourceStr += "\\"; \
            sourceStr += findData.cFileName; \
            tempStr = (dirName); \
            tempStr += "\\"; \
            tempStr += findData.cFileName; \
            DrawProgressbarMessage( pD3DDevice, message, findData.cFileName, i++, 0 ); \
            CopyFile( sourceStr.c_str(), tempStr.c_str(), TRUE ); \
            PRINTMSG(( T_INFO, "%s", findData.cFileName )); \
          } while( FindNextFile( h, &findData ) ); \
          FindClose( h ); \
        } \
      } \
    }

  COPYDIR( g_FileIOConfig.m_artPath );
  COPYDIR( g_FileIOConfig.m_generalPath );
  COPYDIR( g_FileIOConfig.m_HiScorePath );
  COPYDIR( g_FileIOConfig.m_audioPath );
  COPYDIR( g_FileIOConfig.m_screenshotPath );

  return TRUE;
}

extern "C" {

//-------------------------------------------------------------
//	SetScreenUsage
//-------------------------------------------------------------
void SetScreenUsage( FLOAT xPercentage, FLOAT yPercentage )
{
  g_rendererOptions.m_screenUsageX = xPercentage;
  g_rendererOptions.m_screenUsageY = yPercentage;
}

//-------------------------------------------------------------
//	GetScreenUsage
//-------------------------------------------------------------
void GetScreenUsage( FLOAT *xPercentage, FLOAT *yPercentage )
{
  if( xPercentage )
    *xPercentage = g_rendererOptions.m_screenUsageX;
  if( yPercentage )
    *yPercentage = g_rendererOptions.m_screenUsageY;
}

//-------------------------------------------------------------
//	SetScreenPosition
//-------------------------------------------------------------
void SetScreenPosition( FLOAT xOffset, FLOAT yOffset )
{
  g_rendererOptions.m_screenOffsetX = xOffset;
  g_rendererOptions.m_screenOffsetY = yOffset;
}

//-------------------------------------------------------------
//	GetScreenPosition
//-------------------------------------------------------------
void GetScreenPosition( FLOAT *xOffset, FLOAT *yOffset )
{
  if( xOffset )
    *xOffset = g_rendererOptions.m_screenOffsetX;
  if( yOffset )
    *yOffset = g_rendererOptions.m_screenOffsetY;
}


//-------------------------------------------------------------
//	RenderMessageBackdrop
//-------------------------------------------------------------
inline void RenderMessageBackdrop( LPDIRECT3DDEVICE8 pD3DDevice )
{
  // Render the backdrop texture
	pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

  pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );


	g_loadedSkin->SelectSkinTexture( pD3DDevice, ASSET_MESSAGE_BACKDROP );
  pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX0 );
  pD3DDevice->Begin( D3DPT_QUADLIST );
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( ASSET_MESSAGE_BACKDROP );

    FLOAT xPercentage, yPercentage;
    GetScreenUsage( &xPercentage, &yPercentage );
 
    pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, -xPercentage, yPercentage, 1.0f, 1.0f );

    pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, xPercentage, yPercentage, 1.0f, 1.0f );

    pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, xPercentage, -yPercentage, 1.0f, 1.0f );

    pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
    pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, -xPercentage, -yPercentage, 1.0f, 1.0f );
	}
  pD3DDevice->End();
	pD3DDevice->SetTexture( 0, NULL );
}

//-------------------------------------------------------------
//	ShowLoadingScreen
//-------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	RenderMessageBackdrop( pD3DDevice );

	D3DCOLOR textColor = D3DCOLOR_XRGB( 0, 0, 0 );
	if( CheckResourceValidity(SKINELEMENT_MESSAGESCREEN_TEXT) )
		textColor = g_loadedSkin->GetSkinElementText(SKINELEMENT_MESSAGESCREEN_TEXT)->m_color;

  g_fontSet.DefaultFont().Begin();
		g_fontSet.DefaultFont().DrawText( 320, 
																			240, 
																			textColor,  
																			L"Loading. Please wait...", 
																			XBFONT_CENTER_X | XBFONT_CENTER_Y );
  g_fontSet.DefaultFont().End();


  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  pD3DDevice->PersistDisplay();
}


//---------------------------------------------------------------------
//	DrawProgressbarMessage
//---------------------------------------------------------------------
void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems )
{
	pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil

	RenderMessageBackdrop( pD3DDevice );

  WCHAR wBuf[256];

	g_fontSet.DefaultFont().Begin();	
		D3DCOLOR textColor = D3DCOLOR_XRGB( 0, 0, 0 );
		if( CheckResourceValidity(SKINELEMENT_MESSAGESCREEN_TEXT) )
			textColor = g_loadedSkin->GetSkinElementText(SKINELEMENT_MESSAGESCREEN_TEXT)->m_color;

    if( message )
    {
      mbstowcs( wBuf, message, 256 );

			if( currentItem != 0xFFFFFFFF )
				g_fontSet.DefaultFont().DrawText( 320, 200, textColor, wBuf, XBFONT_CENTER_X );
			else
				g_fontSet.DefaultFont().DrawText( 320, 240, textColor, wBuf, XBFONT_CENTER_X | XBFONT_CENTER_Y );
    } 	
		  // Draw the current filename
    if( itemName )
    {
	    mbstowcs( wBuf, itemName, 256 );
	    g_fontSet.DefaultFont().DrawText( 320, 258, textColor, wBuf, XBFONT_CENTER_X );
    }
	g_fontSet.DefaultFont().End();


  if( currentItem != 0xFFFFFFFF )
  {
		D3DCOLOR endColor = D3DCOLOR_XRGB( 255, 255, 255 );
		D3DCOLOR barColor = D3DCOLOR_XRGB( 110, 120, 200 );
		D3DCOLOR backgroundColor = D3DCOLOR_RGBA( 190, 190, 190, 100 );


		if( CheckResourceValidity(SKINELEMENT_MESSAGESCREEN_PROGRESSBAR) )
		{
			const CSkinProgressbar *b = g_loadedSkin->GetSkinElementProgressbar(SKINELEMENT_MESSAGESCREEN_PROGRESSBAR);
			endColor = b->m_endColor;
			barColor = b->m_barColor;
			backgroundColor = b->m_backgroundColor;
		}

    #define PROGRESSBAR_WIDTH         410
    #define PROGRESSBAR_HEIGHT        20

    RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                       240 - (PROGRESSBAR_HEIGHT >> 1), 
                       320 + (PROGRESSBAR_WIDTH>>1), 
                       240 + (PROGRESSBAR_HEIGHT >> 1), 
                       currentItem, 
                       (!totalItems ? BUBBLEBAR : totalItems), 
                       barColor, 
                       endColor, 
											 backgroundColor );
  }

	pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

//---------------------------------------------------------------------
//	fatalerror
//---------------------------------------------------------------------
int fatalerror( const char *fmt, ... )
{
  wchar_t wBuf[1024];
  char buf[1024];

  va_list arg;
  va_start( arg, fmt );
  vsprintf( buf, fmt, arg );
  va_end( arg );

  mbstowcs( wBuf, buf, 1023 );

	g_inputManager.WaitForNoButton();

	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

  while( !(g_inputManager.IsAnyButtonPressed() || g_inputManager.IsAnyKeyPressed()) )
  {
    g_inputManager.PollDevices();

		  // Display the error to the user
	  pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
											  D3DCOLOR_XRGB(0,0,0),							// Color
											  1.0f,															// Z
											  0L );															// Stencil

	  g_fontSet.DefaultFont().Begin();  	
      g_fontSet.DefaultFont().DrawText( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Fatal Error:", XBFONT_CENTER_X );
			g_fontSet.DefaultFont().DrawText( 320, 75, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, XBFONT_CENTER_X );
			g_fontSet.DefaultFont().DrawText( 320, 408, D3DCOLOR_RGBA( 255, 255, 255, 255), L"Press any button to continue.", XBFONT_CENTER_X );
		g_fontSet.DefaultFont().End();

	  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

  g_inputManager.WaitForNoButton();

    // Go to the dashboard
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  DWORD retVal = XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
  Die( pD3DDevice, "Failed to launch the dashboard! 0x%X", retVal );

    // Execution should never get here
  return 0;
}


#undef free
#include <memory.h>

void osd_free( void *ptr )
{
	free( ptr ) ;
}




}	// End Extern "C"



