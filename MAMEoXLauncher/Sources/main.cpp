/**
	* \file			xbox_Main.cpp
	* \brief		MAMEoX entry point
	*
	*/


//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <XGraphics.h>
#include <stdarg.h>
#include <stdio.h>
#include <crtdbg.h>

#ifdef _PROFILER
#include <xbdm.h>
#endif

#include "MAMEoX.h"
#include "Resource.h"

#include "InputManager.h"
#include "GraphicsManager.h"
#include "ROMList.h"
#include "Help.h"
#include "OptionsPage.h"
#include "System_IniFile.h"
#include "xbox_FileIO.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_Timing.h"
#include "DebugLogger.h"
#include "xbox_Network.h"
#include "FontSet.h"

//= D E F I N E S =====================================================

  // The maximum number of times to attempt to generate a working driver.list file
#define MAX_DRIVERLIST_GENERATION_ATTEMPTS    2

	// Number of seconds between valid X button readings
#define TOGGLEBUTTON_TIMEOUT	0.50f

  // The deadzone for the screen usage percentage control (right analog)
#define SCREENRANGE_DEADZONE    15000

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

BYTE                      *g_pResourceSysMemData = NULL;
BYTE                      *g_pResourceVidMemData = NULL;


static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

  //! The data for each driver
static MAMEDriverData_t   *g_driverData = NULL;


extern "C" {
  // Fake options struct for load/store options
GameOptions options;

  // Fake cheatfile name for load/store options
const char *cheatfile = NULL;
}
  
  // Fake D3D renderer options
RendererOptions_t    g_rendererOptions;

  // Fake sound override toggle
BOOL g_soundEnabled = TRUE;

//= P R O T O T Y P E S ===============================================
BOOL CreateBackdrop( FLOAT xPosition, FLOAT yPosition, FLOAT xUsage, FLOAT yUsage );
void DestroyBackdrop( void );
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );
static BOOL Helper_LoadDriverInfoFile( void );
static HRESULT LoadPackedResources( void );
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice );

//= F U N C T I O N S =================================================


#define TEST_LIGHTGUN 0


//-------------------------------------------------------------
//	main
//-------------------------------------------------------------
void __cdecl main( void )
{
		// Start up the debug logger thread
	DebugLoggerInit();

		// Initialize the graphics subsystem
	g_graphicsManager.Create();
	LPDIRECT3DDEVICE8 pD3DDevice = g_graphicsManager.GetD3DDevice();

    // Create the fonts or die
  if( !g_fontSet.Create() )
  {
    LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
    XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
  }

  LoadOptions();

		// Initialize the input subsystem
	g_inputManager.Create( 4, 0 );  // 4 controllers, no memory cards


	// Intialize the various MAME OSD-specific subsystems
	InitializeFileIO();
	InitializeTiming();

  if( !g_NetworkConfig.m_networkDisabled )
    InitializeNetwork();
  else
  {
      // If any paths have been set to samba, revert them
      // to the defaults
    if( g_FileIOConfig.m_ArtPath.Left(6) == "smb://" )
      g_FileIOConfig.m_ArtPath = DEFAULT_ARTPATH;

    if( g_FileIOConfig.m_AudioPath.Left(6) == "smb://" )
      g_FileIOConfig.m_AudioPath = DEFAULT_AUDIOPATH;
    
    if( g_FileIOConfig.m_ConfigPath.Left(6) == "smb://" )
      g_FileIOConfig.m_ConfigPath = DEFAULT_CONFIGPATH;
    
    if( g_FileIOConfig.m_DefaultRomListPath.Left(6) == "smb://" )
      g_FileIOConfig.m_DefaultRomListPath = DEFAULT_DEFAULTROMLISTPATH;
    
    if( g_FileIOConfig.m_GeneralPath.Left(6) == "smb://" )
      g_FileIOConfig.m_GeneralPath = DEFAULT_GENERALPATH;
    
    if( g_FileIOConfig.m_HDImagePath.Left(6) == "smb://" )
      g_FileIOConfig.m_HDImagePath = DEFAULT_HDIMAGEPATH;
    
    if( g_FileIOConfig.m_HiScorePath.Left(6) == "smb://" )
      g_FileIOConfig.m_HiScorePath = DEFAULT_HISCOREPATH;
    
    if( g_FileIOConfig.m_NVramPath.Left(6) == "smb://" )
      g_FileIOConfig.m_NVramPath = DEFAULT_NVRAMPATH;
    
    if( g_FileIOConfig.m_RomBackupPath.Left(6) == "smb://" )
      g_FileIOConfig.m_RomBackupPath = DEFAULT_ROMBACKUPPATH;
    
    if( g_FileIOConfig.m_RomPath.Left(6) == "smb://" )
      g_FileIOConfig.m_RomPath = DEFAULT_ROMPATH;
  }

  SaveOptions(); 


  if( FAILED( LoadPackedResources() ) )
  {
    Die( pD3DDevice, 
         "The Media/Resource.xpr file is missing\n"
         "or damaged.\n"
         "The original file may be found in the\n"
         "Media directory of the MAMEoX package\n"
         "(it is autogenerated during the build\n"
         "process, so you may have to rebuild\n"
         "to obtain it.\n"
         "Please place the correct file in the\n"
         "Media directory on your XBOX and restart." );
  }

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
      XLaunchNewImage( "D:\\MAMEoX.xbe", &g_launchData );
		  Die( pD3DDevice, "Could not execute MAMEoX.xbe!" );
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

    // Wait for controller 0 to be inserted
  RequireController( 0 );

#if TEST_LIGHTGUN

    // Create the vertex buffer
  g_graphicsManager.GetD3DDevice()->CreateVertexBuffer( (sizeof(CUSTOMVERTEX) << 2),
																		                    D3DUSAGE_WRITEONLY,
																		                    D3DFVF_XYZ | D3DFVF_TEX1,
																		                    D3DPOOL_MANAGED,
																		                    &g_pD3DVertexBuffer );

	CUSTOMVERTEX *pVertices;


  while( 1 )
  {
  	WCHAR wBuf[1024];
    WCHAR calibBuf[1024];

		g_inputManager.PollDevices();
    RequireController( 0 );

    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                        D3DCOLOR_XRGB(105,105,105),				// Color
											  1.0f,															// Z
											  0L );															// Stencil

		g_pD3DVertexBuffer->Lock( 0,										// Offset to lock
															0,										// Size to lock
															(BYTE**)&pVertices,		// ppbData
															0 );									// Flags

      FLOAT lx = (FLOAT)((g_inputManager.sThumbLX - 300)) / 32767.0f;
      FLOAT rx = (FLOAT)((g_inputManager.sThumbLX + 300)) / 32767.0f;

      FLOAT ty = (FLOAT)((g_inputManager.sThumbLY - 300)) / 32767.0f;
      FLOAT by = (FLOAT)((g_inputManager.sThumbLY + 300)) / 32767.0f;

        //-- Draw the backdrop -------------------------------------------------
		  pVertices[0].pos.x = lx;
		  pVertices[0].pos.y = ty;
		  pVertices[0].pos.z = 1.0f;
      pVertices[0].tu = 0.0f;
      pVertices[0].tv = 0.0f;

		  pVertices[1].pos.x = rx;
		  pVertices[1].pos.y = ty;
		  pVertices[1].pos.z = 1.0f;
      pVertices[1].tu = 1.0f;
      pVertices[1].tv = 0.0f;
  		
		  pVertices[2].pos.x = rx;
		  pVertices[2].pos.y = by;
		  pVertices[2].pos.z = 1.0f;
      pVertices[2].tu = 1.0f;
      pVertices[2].tv = 1.0f;
  		
		  pVertices[3].pos.x = lx;
		  pVertices[3].pos.y = by;
		  pVertices[3].pos.z = 1.0f;
      pVertices[3].tu = 0.0f;
      pVertices[3].tv = 1.0f;

    g_pD3DVertexBuffer->Unlock();

    g_fontSet.DefaultFont().Begin();

    if( g_inputManager.bAnalogButtons[XINPUT_GAMEPAD_B] > 10 )
    {
      osd_joystick_start_calibration();
      WaitForNoButton();
      const char *ptr = osd_joystick_calibrate_next();
      if( !ptr )
        osd_joystick_end_calibration();
      else
     	  mbstowcs( calibBuf, ptr, strlen(ptr) + 1 );
    }
    if( g_inputManager.bAnalogButtons[XINPUT_GAMEPAD_A] > 10 && g_calibrationStep )
    {
      osd_joystick_calibrate();
      WaitForNoButton();
      const char *ptr = osd_joystick_calibrate_next();
      if( !ptr )
        osd_joystick_end_calibration();
      else
     	  mbstowcs( calibBuf, ptr, strlen(ptr) + 1 );
    }
    if( g_inputManager.bAnalogButtons[XINPUT_GAMEPAD_X] > 10 )
    {
      SaveOptions();
    }

      g_fontSet.DefaultFont().DrawText( 320, 180, D3DCOLOR_RGBA( 255, 255, 255, 255), calibBuf, XBFONT_CENTER_X );

      swprintf( wBuf, L"LX: %d", g_inputManager.sThumbLX );
	    g_fontSet.DefaultFont().DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );

      swprintf( wBuf, L"LY: %d", g_inputManager.sThumbLY );
	    g_fontSet.DefaultFont().DrawText( 320, 100, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );



      int deltaX, deltaY;
      osd_lightgun_read( 0, &deltaX, &deltaY );

      swprintf( wBuf, L"LX: %d", deltaX );
	    g_fontSet.DefaultFont().DrawText( 320, 280, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );

      swprintf( wBuf, L"LY: %d", deltaY );
	    g_fontSet.DefaultFont().DrawText( 320, 300, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );

    g_fontSet.DefaultFont().End();

    pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX1 );
    pD3DDevice->SetStreamSource(	0,												  // Stream number
																	g_pD3DVertexBuffer,					// Stream data
																	sizeof(CUSTOMVERTEX) );		  // Vertex stride

    pD3DDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

    g_graphicsManager.GetD3DDevice()->Present( NULL, NULL, NULL, NULL );
  }
#endif




		// Load the Help file
  CHelp help( pD3DDevice, 
              g_fontSet, 
              (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_HelpScreenBackdrop_OFFSET] );
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
  CROMList romList( pD3DDevice, 
                    g_fontSet,
                    (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_ROMListScreenBackdrop_OFFSET],
                    (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_ROMListScreenBackdrop_NoAdditionalInfo_OFFSET],
                    (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_ROMListScrollUpMask_OFFSET],
                    (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_ROMListScrollDownMask_OFFSET],
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

  COptionsPage optionsPage( pD3DDevice,
                            g_fontSet,
                            (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_OptionsScreenBackdrop_OFFSET],
                            options );

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

    // Toggle for whether or not we're in a given mode
  BOOL optionsMode = FALSE;
  BOOL helpMode = FALSE;
	FLOAT toggleButtonTimeout = 0.0f;
	UINT64 lastTime = osd_cycles();


    // Store the current screen rotation value so we can
    // rotate as soon as we notice that the option is changed
  screenrotation_t oldRotation = g_rendererOptions.m_screenRotation;

    // Right analog movement mode. If this is false, right analog
    // stretches the screen. If true, it moves it.
  BOOL rightAnalogMovesScreen = FALSE;

		//--- Main loop ------------------------------------------------------
	while( 1 )
	{
		g_inputManager.PollDevices();
    RequireController( 0 );

	  UINT64 curTime = osd_cycles();
	  FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
	  lastTime = curTime;

		  // Decrement the dpad movement timer
	  if( toggleButtonTimeout > 0.0f )
	  {
		  toggleButtonTimeout -= elapsedTime;
		  if( toggleButtonTimeout < 0.0f )
			  toggleButtonTimeout = 0.0f;
	  }


			// Reboot on LT+RT+Black
    if( g_inputManager.IsButtonPressed( GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER | GP_BLACK ) )
		{
      g_romListOptions = romList.GetOptions();
      romList.GetCursorPosition(  &g_persistentLaunchData.m_cursorPosition, 
                                  &g_persistentLaunchData.m_pageOffset,
                                  &g_persistentLaunchData.m_superscrollIndex );
      SaveOptions();
      LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
      XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
		}



    if( g_inputManager.IsOnlyButtonPressed( GP_B | GP_Y ) && toggleButtonTimeout == 0.0f )
    {
        // Toggle options mode
      optionsMode = !optionsMode;
      toggleButtonTimeout = TOGGLEBUTTON_TIMEOUT;
    }
    else if( g_inputManager.IsOnlyButtonPressed( GP_X ) && toggleButtonTimeout == 0.0f )
    {
        // Toggle help mode
      helpMode = !helpMode;
      toggleButtonTimeout = TOGGLEBUTTON_TIMEOUT;
    }
    else if( g_inputManager.IsButtonPressed( GP_RIGHT_ANALOG ) && toggleButtonTimeout == 0.0f )
    {
        // Toggle the right analog stick mode
      rightAnalogMovesScreen = !rightAnalogMovesScreen;
      toggleButtonTimeout = TOGGLEBUTTON_TIMEOUT;
    }
    else if( g_inputManager.IsOneOfButtonsPressed( GP_RA_LEFT | GP_RA_RIGHT | GP_RA_UP | GP_RA_DOWN ) ||
             oldRotation != g_rendererOptions.m_screenRotation )
    {
      FLOAT xPos, yPos;
      GetScreenPosition( &xPos, &yPos );
      FLOAT xPercentage, yPercentage;
      GetScreenUsage( &xPercentage, &yPercentage );

      if( rightAnalogMovesScreen )
      {
        if( g_inputManager.IsButtonPressed( GP_RA_LEFT ) )
          xPos -= 0.00025f;
        else if( g_inputManager.IsButtonPressed( GP_RA_RIGHT ) )
          xPos += 0.00025f;

        if( g_inputManager.IsButtonPressed( GP_RA_DOWN ) )
          yPos -= 0.00025f;
        else if( g_inputManager.IsButtonPressed( GP_RA_UP ) )
          yPos += 0.00025f;

        if( xPos < -0.5f )
          xPos = -0.5f;
        else if( xPos > 0.5f )
          xPos = 0.5f;

        if( yPos < -0.5f )
          yPos = -0.5f;
        else if( yPos > 0.5f )
          yPos = 0.5f;

        SetScreenPosition( xPos, yPos );
      }
      else
      {

        if( g_inputManager.IsButtonPressed( GP_RA_LEFT ) )
          xPercentage -= 0.00025f;
        else if( g_inputManager.IsButtonPressed( GP_RA_RIGHT ) )
          xPercentage += 0.00025f;

        if( g_inputManager.IsButtonPressed( GP_RA_DOWN ) )
          yPercentage -= 0.00025f;
        else if( g_inputManager.IsButtonPressed( GP_RA_UP ) )
          yPercentage += 0.00025f;

        if( xPercentage < 0.25f )
          xPercentage = 0.25f;
        else if( xPercentage > 1.0f )
          xPercentage = 1.0f;

        if( yPercentage < 0.25f )
          yPercentage = 0.25f;
        else if( yPercentage > 1.0f )
          yPercentage = 1.0f;

        SetScreenUsage( xPercentage, yPercentage );
      }

      DestroyBackdrop();
      CreateBackdrop( xPos, yPos, xPercentage, yPercentage );
      oldRotation = g_rendererOptions.m_screenRotation;
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
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
    
      // Render whatever screen happens to be active to the
      // texture
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET,	                // Flags
                        D3DCOLOR_XRGB(105,105,105),				// Color
											  1.0f,															// Z
											  0L );															// Stencil



    if( optionsMode )
    {
      optionsPage.MoveCursor( g_inputManager );
      optionsPage.DrawToTexture( renderTargetTexture );
    }
    else if( helpMode )
    {
      help.MoveCursor( g_inputManager );
      help.DrawToTexture( renderTargetTexture );
    }
    else
    {
		  romList.MoveCursor( g_inputManager );
		  romList.DrawToTexture( renderTargetTexture );
    }



      // Now render the texture to the screen 
    pD3DDevice->Clear(	0L,																// Count
											  NULL,															// Rects to clear
											  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                        D3DCOLOR_XRGB(105,105,105),				// Color
											  1.0f,															// Z
											  0L );															// Stencil

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
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
    pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );

    pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX1 );
    pD3DDevice->SetStreamSource(	0,												  // Stream number
																	g_pD3DVertexBuffer,	        // Stream data
																	sizeof(CUSTOMVERTEX) );		  // Vertex stride
	  pD3DDevice->SetTexture( 0, renderTargetTexture );
    pD3DDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

    pD3DDevice->Present( NULL, NULL, NULL, NULL );


      // Run the user selected ROM
    if( romList.IsGameSelected() )
    {
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
      SaveOptions();
      ShowLoadingScreen( pD3DDevice );
      XLaunchNewImage( "D:\\MAMEoX.xbe", &g_launchData );
		  Die( pD3DDevice, "Could not execute MAMEoX.xbe!" );
    }
    else if( romList.ShouldGenerateROMList() )
    {
      romList.GenerateROMList();
		  WaitForNoButton();
    }
	}
}

//-------------------------------------------------------------
// Helper_LoadDriverInfoFile
//-------------------------------------------------------------
static BOOL Helper_LoadDriverInfoFile( void )
{
	DWORD BytesRead = 0;
	DWORD len;
  MAMEoXLaunchData_t *mameoxLaunchData = (MAMEoXLaunchData_t*)g_launchData.Data;
  std::string		driverListFile = g_ROMListPath;
	driverListFile += "\\";
	driverListFile += DRIVERLISTFILENAME;

	PRINTMSG( T_INFO, "Load driver list: %s", driverListFile.c_str() );
	HANDLE hFile = CreateFile(	driverListFile.c_str(),
															GENERIC_READ,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		PRINTMSG( T_ERROR, "Could not open file %s!", driverListFile.c_str() );
		return FALSE;
	}

    // Grab the entire file at once so we can check its signature
  LARGE_INTEGER fileSize;
  if( !GetFileSizeEx( hFile, &fileSize ) || fileSize.HighPart )
  {
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not get filesize for %s!", driverListFile.c_str() );
    return FALSE;
  }
  
  UCHAR *fileData = (UCHAR*)malloc( fileSize.LowPart );
  if( !fileData )
  {
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, 
              "Could not malloc space for %s (%lu bytes required)!", 
              driverListFile.c_str(), 
              fileSize.LowPart );
    return FALSE;
  }

    // Read the entire file into memory
  if( !ReadFile( hFile, fileData, fileSize.LowPart, &BytesRead, NULL ) || BytesRead != fileSize.LowPart )
  {
    free( fileData );
    CloseHandle( hFile );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to read file %s!", driverListFile.c_str() );
    return FALSE;
  }
  CloseHandle( hFile );

    // Check the signature
  if( memcmp( fileData, DRIVERLIST_FILESTAMP, sizeof(DRIVERLIST_FILESTAMP) - 1 ) )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Bad file signature!" );
    return FALSE;
  }

    // Test the data signature
  HANDLE sigHandle = XCalculateSignatureBegin( XCALCSIG_FLAG_SAVE_GAME );
  if( sigHandle == INVALID_HANDLE_VALUE )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not calculate driver list signature!" );
    return FALSE;
  }

  DWORD sigSize = XCalculateSignatureGetSize( XCALCSIG_FLAG_SAVE_GAME );
  void *sigData = calloc( 1, sigSize );
  if( !sigData )
  {
    free( fileData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Could not allocate memory for driver list signature!" );
    return FALSE;
  }

  const BYTE *listData = fileData + ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  DWORD listDataSize = fileSize.LowPart - ((sizeof(DRIVERLIST_FILESTAMP) - 1) + sigSize);
  if( XCalculateSignatureUpdate( sigHandle, listData, listDataSize ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed to calculate data signature!" );
    return FALSE;
  }
  
    // Grab the signature
  if( XCalculateSignatureEnd( sigHandle, sigData ) != ERROR_SUCCESS )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Failed getting signature!" );
    return FALSE;
  }

    // Test the signature
  if( memcmp( (fileData + (sizeof(DRIVERLIST_FILESTAMP)-1)), sigData, sigSize ) )
  {
    free( fileData );
    free( sigData );
		DeleteFile( driverListFile.c_str() );
    PRINTMSG( T_ERROR, "Data signature mismatch!" );
    return FALSE;
  }
  free( sigData );


    // Define a macro to "read" a block of data and ensure that we're not reading past the end of
    //  the file
  #define READDATA( _data__, _dataSize__, _dataType__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize.LowPart ) \
    { \
      if( g_driverData ) \
        free( g_driverData ); \
      free( fileData ); \
      DeleteFile( driverListFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
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
        PRINTMSG( T_ERROR, "Failed to malloc data array. %lu bytes requested!", (_dataSize__) ); \
        return FALSE; \
      } \
      memcpy( (_data__), listData, (_dataSize__) ); \
      listData += (_dataSize__); \
    }

  #define READDATA_NOMALLOC( _data__, _dataSize__ ) \
    if( (DWORD)((listData + (_dataSize__)) - fileData) > fileSize.LowPart ) \
    { \
      if( g_driverData ) \
        free( g_driverData ); \
      free( fileData ); \
      DeleteFile( driverListFile.c_str() ); \
      PRINTMSG( T_ERROR, "Attempt to read beyond the end of the file!" ); \
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
        PRINTMSG( T_ERROR, "Attempt to read into NULL destination buffer!" ); \
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

	PRINTMSG( T_ERROR, "Die: %s", buf );

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

	  g_fontSet.DefaultFont().DrawText( 320, 60, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );
	  g_fontSet.DefaultFont().DrawText( 320, 320, D3DCOLOR_RGBA( 255, 125, 125, 255), L"Press any button to reboot.", XBFONT_CENTER_X );

	g_fontSet.DefaultFont().End();
	pD3DDevice->Present( NULL, NULL, NULL, NULL );

	g_inputManager.WaitForNoButton( 0 );
	g_inputManager.WaitForAnyButton( 0 );
	g_inputManager.WaitForNoButton( 0 );

    // Reboot
  LD_LAUNCH_DASHBOARD LaunchData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };
  XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchData );
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

//-----------------------------------------------------------------------------
// LoadPackedResources
//-----------------------------------------------------------------------------
static HRESULT LoadPackedResources( void )
{
  FILE *file = fopen( "D:\\Media\\Resource.xpr", "rb" );
  if( !file )
    return E_FAIL;

    // Read in and verify the XPR magic header
  XPR_HEADER xprh;
  if( fread( &xprh, sizeof(XPR_HEADER), 1, file ) != 1 )
  {
    fclose( file );
    return E_FAIL;
  }

  if( xprh.dwMagic != XPR_MAGIC_VALUE )
  {
    PRINTMSG( T_INFO, "ERROR: Invalid Xbox Packed Resource (.xpr) file" );
    fclose( file );
    return E_INVALIDARG;
  }

    // Compute memory requirements
  DWORD dwSysMemDataSize = xprh.dwHeaderSize - sizeof(XPR_HEADER);
  DWORD dwVidMemDataSize = xprh.dwTotalSize - xprh.dwHeaderSize;

    // Allocate memory
  g_pResourceSysMemData = new BYTE[dwSysMemDataSize];
  g_pResourceVidMemData = (BYTE*)D3D_AllocContiguousMemory( dwVidMemDataSize, D3DTEXTURE_ALIGNMENT );

    // Read in the data from the file
  if( fread( g_pResourceSysMemData, dwSysMemDataSize, 1, file ) != 1 ||
      fread( g_pResourceVidMemData, dwVidMemDataSize, 1, file ) != 1 )
  {
    delete[] g_pResourceSysMemData;
    D3D_FreeContiguousMemory( g_pResourceVidMemData );
    fclose( file );
    return E_FAIL;
  }

  fclose( file );
  
    // Loop over resources, calling Register()
  BYTE *pData = g_pResourceSysMemData;

  for( DWORD i = 0; i < resource_NUM_RESOURCES; ++i )
  {
    LPDIRECT3DRESOURCE8 pResource = (LPDIRECT3DRESOURCE8)pData;
    pResource->Register( g_pResourceVidMemData );

    switch( pResource->GetType() )
    {
      case D3DRTYPE_TEXTURE:       
        pData += sizeof(D3DTexture);       
        break;

      case D3DRTYPE_VOLUMETEXTURE: 
        pData += sizeof(D3DVolumeTexture); 
        break;

      case D3DRTYPE_CUBETEXTURE:   
        pData += sizeof(D3DCubeTexture);   
        break;

      case D3DRTYPE_VERTEXBUFFER:  
        pData += sizeof(D3DVertexBuffer);  
        break;

      case D3DRTYPE_INDEXBUFFER:   
        pData += sizeof(D3DIndexBuffer);   
        break;

      case D3DRTYPE_PALETTE:       
        pData += sizeof(D3DPalette);       
        break;

      default:
        return E_FAIL;
    }
  }
  return S_OK;
}

//-------------------------------------------------------------
//	ShowSplashScreen
//-------------------------------------------------------------
static void ShowSplashScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
  RequireController( 0 );
  CGamepad *gp = g_inputManager.GetGamepad( 0 );

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
	pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
  pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC,        D3DCMP_GREATEREQUAL );
  pD3DDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
  pD3DDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );

  LPDIRECT3DTEXTURE8 pTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_SplashScreenBackdrop_OFFSET];
  #define TEXTFADE_MINIMUM            0.0f
  #define TEXTFADE_MAXIMUM            110.0f
  #define TEXTFADE_FRAMES_PER_STEP    10.0f
  FLOAT textFadeColor = TEXTFADE_MINIMUM;
  FLOAT fadeDirection = 1.0f;

  #define NORMAL_TEXT_COLOR           D3DCOLOR_XRGB( 0, 0, 0 )
  #define IMPORTANT_TEXT_COLOR        D3DCOLOR_XRGB( 90, 105, 195 )
                        

    //-- Set up the credits ------------------------------------------
  #define CREDITS_FRAMES_PER_STEP     10.0f;
  const WCHAR credits[] = L"       The MAMEoX team:"\
                          L"       Programming - Erik Abair, opcode, luckyMIC, ips" \
                          L"       Testing - falz, enkak, noodle1009" \
                          L"       Graphical design - r4dius, Stephen Cameron" \
                          L"       Special thanks to MAME developers everywhere";

  FLOAT creditsPosition = 0.0f;
  FLOAT creditsLength, creditsHeight;
  g_fontSet.DefaultFont().GetTextExtent( credits, &creditsLength, &creditsHeight );

    // Create the credits texture
  LPDIRECT3DTEXTURE8 creditsTexture = g_fontSet.DefaultFont().CreateTexture( credits, 
                                                            D3DCOLOR_ARGB( 0, 0, 0, 0 ), 
                                                            D3DCOLOR_ARGB( 0xFF, 0, 0, 0 ) );

    // Area onscreen for the credits text
    // 115.0f,208.0f -> 524.0f,228.0f
  #define CREDITSBOX_LEFT       115.0f
  #define CREDITSBOX_RIGHT      524.0f
  #define CREDITSBOX_TOP        208.0f
  #define CREDITSBOX_BOTTOM     228.0f


    //-- Display everything -------------------------------------------------------
  while( !gp->IsAnyButtonPressed() )
  {
    RequireController( 0 );

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
	  pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	  pD3DDevice->SetTexture( 0, pTexture );
    pD3DDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
    pD3DDevice->Begin( D3DPT_QUADLIST );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );
      pD3DDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
      pD3DDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
    pD3DDevice->End();


      // Scroll the credits
    creditsPosition += 1.0f / CREDITS_FRAMES_PER_STEP;
    if( creditsPosition >= creditsLength )
      creditsPosition = (-(CREDITSBOX_RIGHT - CREDITSBOX_LEFT) + 1.0f);

	  pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
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
                        NORMAL_TEXT_COLOR,
                        L"Version " LVERSION_STRING L" " LBUILDCONFIG_STRING, 
                        XBFONT_RIGHT );

	    g_fontSet.DefaultFont().DrawText(  320, 
                        410, 
                        D3DCOLOR_XRGB( (UINT8)textFadeColor, (UINT8)textFadeColor, (UINT8)textFadeColor ), 
                        L"Insert Coin", 
                        XBFONT_CENTER_X );
    g_fontSet.DefaultFont().End();

    g_fontSet.LargeThinFont().Begin();
      g_fontSet.LargeThinFont().DrawText( 130, 240, NORMAL_TEXT_COLOR, L"MAME is distributed under the MAME license." ); 
      g_fontSet.LargeThinFont().DrawText( 130, 260, IMPORTANT_TEXT_COLOR, L"See www.mame.net/readme.html or" );
      g_fontSet.LargeThinFont().DrawText( 135, 280, IMPORTANT_TEXT_COLOR, L"MAME/docs/mame.txt for details" );
        
      g_fontSet.LargeThinFont().DrawText( 130, 316, NORMAL_TEXT_COLOR, L"Portions of MAMEoX based on:" );
      g_fontSet.LargeThinFont().DrawText( 130, 334, IMPORTANT_TEXT_COLOR, L"\"MAMEX(b5): updated by superfro," );
      g_fontSet.LargeThinFont().DrawText( 135, 354, IMPORTANT_TEXT_COLOR, L"original port by opcode\"" );      
    g_fontSet.LargeThinFont().End();


    pD3DDevice->Present( NULL, NULL, NULL, NULL );
  }

	g_inputManager.WaitForNoButton( 0 );

  SAFE_RELEASE( creditsTexture );
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
//	ShowLoadingScreen
//-------------------------------------------------------------
void ShowLoadingScreen( LPDIRECT3DDEVICE8 pD3DDevice )
{
  LPDIRECT3DVERTEXBUFFER8 pVertexBuffer = NULL;

  pD3DDevice->CreateVertexBuffer( (sizeof(CUSTOMVERTEX) << 2),
							                    D3DUSAGE_WRITEONLY,
																	D3DFVF_XYZ | D3DFVF_TEX1,
																	D3DPOOL_MANAGED,
																	&pVertexBuffer );

	CUSTOMVERTEX *pVertices;
	pVertexBuffer->Lock( 0,										// Offset to lock
											 0,										// Size to lock
											 (BYTE**)&pVertices,		// ppbData
											 0 );									// Flags
		pVertices[0].pos.x = -0.9f;
		pVertices[0].pos.y = 0.9f;
		pVertices[0].pos.z = 1.0f;
    pVertices[0].tu = 0.0f;
    pVertices[0].tv = 0.0f;

		pVertices[1].pos.x = 0.9f;
		pVertices[1].pos.y = 0.9f;
		pVertices[1].pos.z = 1.0f;
    pVertices[1].tu = 1.0f;
    pVertices[1].tv = 0.0f;

		pVertices[2].pos.x = 0.9f;
		pVertices[2].pos.y = -0.9f;
		pVertices[2].pos.z = 1.0f;
    pVertices[2].tu = 1.0f;
    pVertices[2].tv = 1.0f;

		pVertices[3].pos.x = -0.9f;
		pVertices[3].pos.y = -0.9f;
		pVertices[3].pos.z = 1.0f;
    pVertices[3].tu = 0.0f;
    pVertices[3].tv = 1.0f;
	pVertexBuffer->Unlock();

  LPDIRECT3DTEXTURE8 pTexture = (LPDIRECT3DTEXTURE8)&g_pResourceSysMemData[resource_MessageScreenBackdrop_OFFSET];

	pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

  pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

		// Clear the backbuffer
  pD3DDevice->Clear(	0L,																// Count
											NULL,															// Rects to clear
											D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
                      D3DCOLOR_XRGB(0,0,0),							// Color
											1.0f,															// Z
											0L );															// Stencil


  pD3DDevice->SetStreamSource(	0,												// Stream number
																pVertexBuffer,					// Stream data
																sizeof(CUSTOMVERTEX) );		// Vertex stride
	pD3DDevice->SetTexture( 0, pTexture );
  pD3DDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

  g_fontSet.DefaultFont().Begin();
  g_fontSet.DefaultFont().DrawText( 320, 240, D3DCOLOR_RGBA( 255, 255, 255, 255),   L"Loading. Please wait...", XBFONT_CENTER_X );
  g_fontSet.DefaultFont().End();


  pD3DDevice->Present( NULL, NULL, NULL, NULL );
  pD3DDevice->PersistDisplay();
}

}	// End Extern "C"














#if TEST_LIGHTGUN

static UINT32                   g_calibrationStep = 0;
static UINT32                   g_calibrationJoynum = 0;

void osd_joystick_start_calibration( void )
{
/* Preprocessing for joystick calibration. Returns 0 on success */
  const XINPUT_CAPABILITIES *gp;
  UINT32 i = 0;

  g_calibrationStep = 0;

    // Search for the first connected gun
  for( ; i < 4; ++i )
  {
    gp = GetGamepadCaps( 0 );  
    if( gp && gp->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
    {
      g_calibrationJoynum = i;
      return;
    }
  }
}

const char *osd_joystick_calibrate_next( void )
{
/* Prepare the next calibration step. Return a description of this step. */
/* (e.g. "move to upper left") */
  char retString[128];

    // When we hit 3, switch over to the next gun to be calibrated,
    //  or return NULL to exit the process
  if( g_calibrationStep == 3 )
  {
    const XINPUT_CAPABILITIES *gp;
    ++g_calibrationJoynum;
    for( ; g_calibrationJoynum < 4; ++g_calibrationJoynum )
    {
      gp = GetGamepadCaps( g_calibrationJoynum );
      if( gp && gp->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
          // Found another gun
        g_calibrationStep = 0;
        break;
      }
    }

    if( g_calibrationJoynum == 4 )
      return NULL;
  }

  sprintf( retString, "Gun %d: ", g_calibrationJoynum + 1 );
  switch( g_calibrationStep++ )
  {
  case 0:
    strcat( retString, "Upper left" );
    break;

  case 1:
    strcat( retString, "Center" );
    break;

  case 2:
    strcat( retString, "Lower right" );
    break;
  }

	return retString;
}

void osd_joystick_calibrate( void )
{
/* Get the actual joystick calibration data for the current position */

  if( g_calibrationStep && g_calibrationStep < 4 )
  {
	  const XINPUT_GAMEPAD *gp;
    if( (gp = GetGamepadState( g_calibrationJoynum )) )
    {
      g_calibrationData[g_calibrationJoynum].m_xData[g_calibrationStep-1] = gp->sThumbLX;
      g_calibrationData[g_calibrationJoynum].m_yData[g_calibrationStep-1] = gp->sThumbLY;
    }
    PRINTMSG( T_INFO, "CALIB: STEP %d: %d, %d\n", g_calibrationStep - 1, gp->sThumbLX, gp->sThumbLY );
  }
}

void osd_joystick_end_calibration( void )
{
/* Postprocessing (e.g. saving joystick data to config) */
  UINT32 i = 0;

  for( ; i < 3; ++i )
  {
    g_calibrationData[i].m_xData[0] -= g_calibrationData[i].m_xData[1];
    g_calibrationData[i].m_xData[2] -= g_calibrationData[i].m_xData[1];
    g_calibrationData[i].m_xData[0] *= -1;  //!< Negate so that < 0 values stay < 0

    g_calibrationData[i].m_yData[0] -= g_calibrationData[i].m_yData[1];
    g_calibrationData[i].m_yData[2] -= g_calibrationData[i].m_yData[1];
    g_calibrationData[i].m_yData[2] *= -1;  //!< Negate so that < 0 values stay < 0
  }
}

void osd_lightgun_read(int player, int *deltax, int *deltay)
{
	const XINPUT_GAMEPAD *gp;

	if( (gp = GetGamepadState( player )) )
  {
    lightgunCalibration_t *calibData = &g_calibrationData[player];

    *deltax = gp->sThumbLX - calibData->m_xData[1];
    *deltay = -1 * (gp->sThumbLY - calibData->m_yData[1]);

      // Map from -128 to 128
    if( gp->sThumbLX < 0 )
      *deltax = (int)((FLOAT)*deltax * 128.0f / ((FLOAT)calibData->m_xData[0]+1.0f));
    else
      *deltax = (int)((FLOAT)*deltax * 128.0f / ((FLOAT)calibData->m_xData[2]+1.0f));

    if( gp->sThumbLY > 0 )
      *deltay = (int)((FLOAT)*deltay * 128.0f / ((FLOAT)calibData->m_yData[0]+1.0f));
    else
      *deltay = (int)((FLOAT)*deltay * 128.0f / ((FLOAT)calibData->m_yData[2]+1.0f));

      // Lock to the expected range
    if( *deltax > 128 )
      *deltax = 128;
    else if( *deltax < -128 )
      *deltax = -128;

    if( *deltay > 128 )
      *deltay = 128;
    else if( *deltay < -128 )
      *deltay = -128;
  }
  else  
	  *deltax = *deltay = 0;
}

#endif
