/**
	* \file			OptionsScreen.cpp
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

//= I N C L U D E S ====================================================
#include "OptionsScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_FileIO.h"
#include "xbox_Network.h"
#include "xbox_Direct3DRenderer.h"
#include "GFilterManager.h"

#include "xbox_FileIO.h"		// for path info

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================


  //--- Layout defines -----------------------------------------
#define VIRTUALKEYBOARD_WIDTH     257
#define VIRTUALKEYBOARD_HEIGHT    143

#define STARTPAGE()                       if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONNAME_TEXT ) && CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONVALUE_TEXT ) ) { \
																						DWORD i = 0; \
																						const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA ); \
																						const CSkinText *nameText = g_loadedSkin->GetSkinElementText( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONNAME_TEXT ); \
																						const CSkinText *valText = g_loadedSkin->GetSkinElementText( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONVALUE_TEXT ); \
																						m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;

#define DRAWITEM( _name__, _val__ )					nameText->RenderAsOffset( m_displayDevice, _name__, area->m_left, area->m_top + (i * area->m_singleRowHeight) ); \
																						nameText->RenderAsOffset( m_displayDevice, _val__, area->m_left, area->m_top + (i * area->m_singleRowHeight) ); \
																						++i;

#define ENDPAGE()													}


	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

  // Number of seconds between valid left/right trigger readings
#define TRIGGERSWITCH_TIMEOUT   0.5f


//= G L O B A L = V A R S ==============================================
extern "C" {
  extern const char *cheatfile;
//  extern const char *history_filename;  // Defined in datafile.c
}

extern BOOL g_soundEnabled; // Sound processing override, defined in xbox_Main.cpp
//static UINT32 g_samplerates[2] = { 22700, 44100 };

extern UINT32 g_screensaverTimeout; // Defined in main.cpp

//= P R O T O T Y P E S ===============================================
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );


//= S T R U C T U R E S ===============================================

//= F U N C T I O N S ==================================================

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
COptionsScreen::COptionsScreen( LPDIRECT3DDEVICE8	displayDevice, 
                            CFontSet &fontSet, 
                            GameOptions &options ) :
  CListView( displayDevice, fontSet, ASSET_LIST_BACKDROP ),
  m_optToggleDelay( 0.0f ),
  m_triggerDelay( 0.0f ),
  m_pageNumber( 0 ),
  m_virtualKeyboardMode( FALSE )
{
  wcscpy( m_pageData[OPTPAGE_GENERAL].m_title, L"General Options" );
  m_pageData[OPTPAGE_GENERAL].m_drawFunct = ::DrawGeneralPage;
  m_pageData[OPTPAGE_GENERAL].m_changeFunct = ::ChangeGeneralPage;
  m_pageData[OPTPAGE_GENERAL].m_numItems = 5;

  wcscpy( m_pageData[OPTPAGE_SOUND].m_title, L"Sound Options" );
  m_pageData[OPTPAGE_SOUND].m_drawFunct = ::DrawSoundPage;
  m_pageData[OPTPAGE_SOUND].m_changeFunct = ::ChangeSoundPage;
  m_pageData[OPTPAGE_SOUND].m_numItems = 4;

  wcscpy( m_pageData[OPTPAGE_VIDEO].m_title, L"Video Options" );
  m_pageData[OPTPAGE_VIDEO].m_drawFunct = ::DrawVideoPage;
  m_pageData[OPTPAGE_VIDEO].m_changeFunct = ::ChangeVideoPage;
  m_pageData[OPTPAGE_VIDEO].m_numItems = 12;

  wcscpy( m_pageData[OPTPAGE_VECTOR].m_title, L"Vector Options" );
  m_pageData[OPTPAGE_VECTOR].m_drawFunct = ::DrawVectorPage;
  m_pageData[OPTPAGE_VECTOR].m_changeFunct = ::ChangeVectorPage;
  m_pageData[OPTPAGE_VECTOR].m_numItems = 6;

  wcscpy( m_pageData[OPTPAGE_NETWORK].m_title, L"Network Options" );
  m_pageData[OPTPAGE_NETWORK].m_drawFunct = ::DrawNetworkPage;
  m_pageData[OPTPAGE_NETWORK].m_changeFunct = ::ChangeNetworkPage;
  m_pageData[OPTPAGE_NETWORK].m_numItems = 5;

  wcscpy( m_pageData[OPTPAGE_DIRECTORIES_1].m_title, L"Directory Path Options [1/2]" );
  m_pageData[OPTPAGE_DIRECTORIES_1].m_drawFunct = ::DrawDirectoryPathPage1;
  m_pageData[OPTPAGE_DIRECTORIES_1].m_changeFunct = ::ChangeDirectoryPathPage1;
  m_pageData[OPTPAGE_DIRECTORIES_1].m_numItems = 6;

  wcscpy( m_pageData[OPTPAGE_DIRECTORIES_2].m_title, L"Directory Path Options [2/2]" );
  m_pageData[OPTPAGE_DIRECTORIES_2].m_drawFunct = ::DrawDirectoryPathPage2;
  m_pageData[OPTPAGE_DIRECTORIES_2].m_changeFunct = ::ChangeDirectoryPathPage2;
  m_pageData[OPTPAGE_DIRECTORIES_2].m_numItems = 14;

  wcscpy( m_pageData[OPTPAGE_ROMLIST].m_title, L"ROM List Options" );
  m_pageData[OPTPAGE_ROMLIST].m_drawFunct = ::DrawROMListPage;
  m_pageData[OPTPAGE_ROMLIST].m_changeFunct = ::ChangeROMListPage;
  m_pageData[OPTPAGE_ROMLIST].m_numItems = 10;

  RECT area = { 320 - (VIRTUALKEYBOARD_WIDTH>>1), 240 - (VIRTUALKEYBOARD_HEIGHT >> 1), 
                320 + (VIRTUALKEYBOARD_WIDTH>>1), 240 + (VIRTUALKEYBOARD_HEIGHT >> 1) };
  m_virtualKeyboard = new CVirtualKeyboardView( displayDevice, fontSet, area );
}

//---------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------
COptionsScreen::~COptionsScreen( void ) 
{
  if( m_virtualKeyboard )
    delete m_virtualKeyboard;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void COptionsScreen::MoveCursor( CInputManager &gp, BOOL useSpeedbanding )
{
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();

  if( m_virtualKeyboardMode )
  {
    m_virtualKeyboard->MoveCursor( gp );
    m_pageData[m_pageNumber].m_changeFunct( this, TRUE ); // Polls to see if the keyboard is done w/ input

    if( m_virtualKeyboard->IsInputFinished() )
    {
      gp.WaitForNoInput();
      m_virtualKeyboard->Reset();
      m_virtualKeyboardMode = FALSE;
      m_optToggleDelay = TRIGGERSWITCH_TIMEOUT; // Set the delay timer to avoid accepting buttons from the VK
      lastTime = 0;                             // Don't count time spent in VK screen
    }
    else
      return;
  }

		// General idea taken from XMAME
	if( !lastTime )
	{
			// lastTime isn't valid yet, so wait for the next frame
		lastTime = curTime;
		return;
	}
	lastTime = curTime;

		// Decrement the movement timers
	if( m_dpadCursorDelay > 0.0f )
	{
    BYTE arrowKeys[2] = { VK_UP, VK_DOWN };
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) ||
          gp.IsOneOfKeysPressed( arrowKeys, 2 )) )
			m_dpadCursorDelay = 0.0f;
	}

	if( m_optToggleDelay > 0.0f )
	{
    BYTE arrowKeys[4] = { VK_LEFT, VK_RIGHT, VK_RETURN, VK_BACK };
		m_optToggleDelay -= elapsedTime;
    if( m_optToggleDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_DPAD_RIGHT | GP_LA_LEFT | GP_LA_RIGHT | GP_A | GP_B ) ||
          gp.IsOneOfKeysPressed( arrowKeys, 4 )) )
			m_optToggleDelay = 0.0f;
	}

  if( m_triggerDelay > 0.0f )
  {
    BYTE pageKeys[2] = { VK_DELETE, VK_NEXT };
    m_triggerDelay -= elapsedTime;
    if( m_triggerDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER ) ||
          gp.IsOneOfKeysPressed( pageKeys, 2 )) )
      m_triggerDelay = 0.0f;
  }


    //-- Check the button states ----------------------------------------------------------
  if( (gp.IsButtonPressed( GP_LEFT_TRIGGER ) || gp.IsKeyPressed( VK_DELETE )) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber )
      --m_pageNumber;
    else
      m_pageNumber = OPTPAGE_LAST - 1;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( (gp.IsButtonPressed( GP_RIGHT_TRIGGER ) || gp.IsKeyPressed( VK_NEXT )) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber < OPTPAGE_LAST - 1 )
      ++m_pageNumber;
    else
      m_pageNumber = 0;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( ++m_cursorPosition >= m_pageData[m_pageNumber].m_numItems )
      m_cursorPosition = 0;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( m_cursorPosition ) 
      --m_cursorPosition;
    else
      m_cursorPosition = m_pageData[m_pageNumber].m_numItems - 1;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_LA_LEFT | GP_B ) || gp.IsKeyPressed( VK_LEFT ) || gp.IsKeyPressed( VK_BACK )) && 
            m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, FALSE );
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_RIGHT | GP_LA_RIGHT | GP_A ) || gp.IsKeyPressed( VK_RIGHT ) || gp.IsKeyPressed( VK_RETURN )) && 
            m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, TRUE );
  }
  
}



//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void COptionsScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

	if( !g_loadedSkin )
		return;

  RenderBackdrop();
	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_FOOTER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_OPTIONSSCREEN_FOOTER )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_OPTIONSSCREEN_BODY )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_HEADER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_OPTIONSSCREEN_HEADER )->Render( m_displayDevice );



		//-- Draw the help text --------------------------------------------

		// ** Start ** //
	if( CheckResourceValidity(SKINELEMENT_OPTIONSSCREEN_BUTTONINFO_START) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_OPTIONSSCREEN_BUTTONINFO_START)->Render( m_displayDevice, L"Menu" );



		//--- Draw the title bar text --------------------------------------
	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_OPTIONSSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_HEADER_TEXT ) )
		g_loadedSkin->GetSkinElementText( SKINELEMENT_OPTIONSSCREEN_HEADER_TEXT )->RenderAsOffset(	m_displayDevice, 
																																																m_pageData[m_pageNumber].m_title, 
																																																titleBarLeft, 
																																																titleBarTop );

			// Draw the spacer
	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_HEADER_DIVIDER ) )
	{
		g_loadedSkin->GetSkinElementDivider(SKINELEMENT_OPTIONSSCREEN_HEADER_DIVIDER)->RenderAsOffset( m_displayDevice, 
																																																	 titleBarLeft, 
																																																	 titleBarTop );
	}

		//--- Draw the body text -------------------------------------------
	if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA ) )
	{
		const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA );
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;

			// Render the highlight bar for the selected item
		FLOAT selectedItemYPos = area->m_top + (area->m_singleRowHeight * (UINT32)m_cursorPosition);
		if( area->m_highlightBar )
			area->m_highlightBar->RenderAsOffset( m_displayDevice, area->m_left, selectedItemYPos );

			// Draw the spacer
		if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_DIVIDER ) )
		{
			FLOAT left = 0.0f;
			FLOAT top = 0.0f;
			const CSkinElement *bodyArea = g_loadedSkin->GetSkinElement( SKINELEMENT_OPTIONSSCREEN_BODY );
			if( bodyArea )
			{
				left = bodyArea->m_left;
				top = bodyArea->m_top;
			}

			g_loadedSkin->GetSkinElementDivider(SKINELEMENT_OPTIONSSCREEN_BODY_DIVIDER)->RenderAsOffset( m_displayDevice, 
																																																	 left, 
																																																	 top );
		}


		m_pageData[m_pageNumber].m_drawFunct( this );

    //-- Draw the trigger icons -----------------------------------------------------------

			// Calculate the previous and next page indices
		UINT32 prevPageIndex = m_pageNumber ? m_pageNumber - 1 : OPTPAGE_LAST - 1;
		UINT32 nextPageIndex = m_pageNumber < OPTPAGE_LAST - 1 ? m_pageNumber + 1 : 0;

		if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_LEFT ) )
			g_loadedSkin->GetSkinElementTriggerInfo(SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_LEFT)->Render( m_displayDevice, m_pageData[prevPageIndex].m_title );

		if( CheckResourceValidity( SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_RIGHT ) )
			g_loadedSkin->GetSkinElementTriggerInfo(SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_RIGHT)->Render( m_displayDevice, m_pageData[nextPageIndex].m_title );
	}

  if( m_virtualKeyboardMode )
    m_virtualKeyboard->Draw( FALSE, FALSE );

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}

//---------------------------------------------------------------------
//	DrawGeneralPage
//---------------------------------------------------------------------
void COptionsScreen::DrawGeneralPage( void )
{
  STARTPAGE();

  DRAWITEM( L"Cheats", options.cheat ? L"Enabled" : L"Disabled" );

  WCHAR name[256] = {0};
	mbstowcs( name, cheatfile, 255 );
  DRAWITEM( L"Cheat file", name );
/*
  mbstowcs( name, history_filename, 255 );
  DRAWITEM( L"History file", name );
*/
  DRAWITEM( L"Disclaimer", options.skip_disclaimer ? L"Skipped" : L"Shown" );

  DRAWITEM( L"Game Info", options.skip_gameinfo ? L"Skipped" : L"Shown" );

  swprintf( name, L"After %lu minute%s", g_screensaverTimeout, g_screensaverTimeout > 1 ? "s" : "" );
  DRAWITEM( L"Screensaver", g_screensaverTimeout ? name : L"Disabled" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawSoundPage
//---------------------------------------------------------------------
void COptionsScreen::DrawSoundPage( void )
{
/*
    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );
*/

  WCHAR text[256] = {0};

  STARTPAGE();

  DRAWITEM( L"Sound Processing", g_soundEnabled ? L"Enabled" : L"Disabled" );

  swprintf( text, L"%lu", options.samplerate );
  DRAWITEM( L"Sample Rate", text );

  DRAWITEM( L"Audio Samples", options.use_samples ? L"Enabled" : L"Disabled" );

  DRAWITEM( L"Audio Filter", options.use_filter ? L"Enabled" : L"Disabled" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVideoPage
//---------------------------------------------------------------------
void COptionsScreen::DrawVideoPage( void )
{
/*
  g_rendererOptions.m_vsync = iniFile.GetProfileInt( "Video", "VSYNC", FALSE );       // Enable VSYNC for game rendering
  g_rendererOptions.m_syncOnlyToVSYNC
  g_rendererOptions.m_frameskip =           iniFile.GetProfileInt( "Video", "Frameskip", AUTO_FRAMESKIP );
  options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
  options.use_artwork = iniFile.GetProfileInt( "Video", "Artwork", 0 );
*/

  WCHAR text[256] = {0};
  const WCHAR filterNames[][16] = { L"None",
                                    L"Point", 
                                    L"Linear", 
                                    L"Anisotropic", 
                                    L"Quincunx", 
                                    L"Gaussian Cubic" };


  STARTPAGE();

  DRAWITEM( L"VSYNC", g_rendererOptions.m_vsync ? L"Enabled" : L"Disabled" );
    
  DRAWITEM( L"Framerate throttling", g_rendererOptions.m_throttleFramerate ? L"Enabled" : L"Disabled" );

  swprintf( text, L"Level %d", g_rendererOptions.m_frameskip );
  DRAWITEM( L"Frameskip mode", 
            g_rendererOptions.m_frameskip == AUTO_FRAMESKIP ?  L"Auto" :
            (!g_rendererOptions.m_frameskip ? L"No frameskipping" : text ) );

  DRAWITEM( L"Aspect ratio correction", g_rendererOptions.m_preserveAspectRatio ? L"Enabled" : L"Disabled" );

  DRAWITEM( L"Screen rotation", 
            g_rendererOptions.m_screenRotation == SR_0 ? L"None" : 
            (g_rendererOptions.m_screenRotation == SR_90 ? L"90 degrees" :
            (g_rendererOptions.m_screenRotation == SR_180 ? L"180 degrees" :
            L"270 degrees")) );



  swprintf( text, L"%f", options.brightness );
  DRAWITEM( L"Brightness", text );

  swprintf( text, L"%f", options.pause_bright );
  DRAWITEM( L"Paused brightness", text );

  swprintf( text, L"%f", options.gamma );
  DRAWITEM( L"Gamma", text );

//  swprintf( text, L"%lu", options.color_depth );
//  DRAWITEM( L"Color depth", text );

  DRAWITEM( L"Minification filter", filterNames[g_rendererOptions.m_minFilter] );

  DRAWITEM( L"Magnification filter", filterNames[g_rendererOptions.m_magFilter] );

	CGFilterManager oFilterManger;
	MapFilters mapFilters = oFilterManger.GetAvailableFilters();
	DRAWITEM( L"Scaling filter", mapFilters[(EFilterType)g_rendererOptions.m_FilterType].m_strName );

  if( !options.use_artwork )
    swprintf( text, L"None" );
  else
  {
    swprintf( text, 
              L"%s%s%s%s%s",
              options.use_artwork & ARTWORK_USE_BACKDROPS ? L"Backdrops" : L"",
              (options.use_artwork & ARTWORK_USE_BACKDROPS && (options.use_artwork & ARTWORK_USE_OVERLAYS || options.use_artwork & ARTWORK_USE_BEZELS)) ? L"," : L"",
              options.use_artwork & ARTWORK_USE_OVERLAYS ? L"Overlays" : L"",
              (options.use_artwork & ARTWORK_USE_OVERLAYS && options.use_artwork & ARTWORK_USE_BEZELS) ? L"," : L"",
              options.use_artwork & ARTWORK_USE_BEZELS ? L"Bezels" : L"" );
  }
  DRAWITEM( L"Artwork usage", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVectorPage
//---------------------------------------------------------------------
void COptionsScreen::DrawVectorPage( void )
{
/*
	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "Translucency", TRUE );      // 1 to enable translucency on vectors
	options.vector_width = iniFile.GetProfileInt( "VectorOptions", "VectorWidth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "VectorOptions", "VectorHeight", 480 );	    // requested height for vector games; 0 means default (480)
*/
  WCHAR text[256] = {0};

  STARTPAGE();
  swprintf( text, L"%lu", options.beam );
  DRAWITEM( L"Beam Width", text );

  swprintf( text, L"%f", options.vector_flicker );
  DRAWITEM( L"Flicker Effect", text );

  swprintf( text, L"%f", options.vector_intensity );
  DRAWITEM( L"Beam Intensity", text );

  DRAWITEM( L"Translucency", options.translucency ? L"Enabled" : L"Disabled" );

  swprintf( text, L"%lu", options.vector_width );
  DRAWITEM( L"Vector Screen Width", text );

  swprintf( text, L"%lu", options.vector_height );
  DRAWITEM( L"Vector Screen Height", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawNetworkPage
//---------------------------------------------------------------------
void COptionsScreen::DrawNetworkPage( void )
{
/*
  g_NetworkConfig.m_networkDisabled = iniFile.GetProfileInt( "Network", "DisableNetworking",  FALSE );
  g_NetworkConfig.m_IPAddr          = iniFile.GetProfileString("Network", "IPAddress",  DEFAULT_IPADDR);
  g_NetworkConfig.m_Gateway         = iniFile.GetProfileString("Network", "Gateway",    DEFAULT_GATEWAY);
  g_NetworkConfig.m_SubnetMask      = iniFile.GetProfileString("Network", "Subnet",     DEFAULT_SUBNETMASK);
  g_NetworkConfig.m_NameServer      = iniFile.GetProfileString("Network", "NameServer", DEFAULT_NAMESERVER);
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  DRAWITEM( L"Networking features", g_NetworkConfig.m_networkDisabled ? L"Disabled" : L"Enabled" );

	mbstowcs( text, g_NetworkConfig.m_IPAddr.c_str(), 255 );
  DRAWITEM( L"IP address", text );

	mbstowcs( text, g_NetworkConfig.m_Gateway.c_str(), 255 );
  DRAWITEM( L"Gateway address", text );

	mbstowcs( text, g_NetworkConfig.m_SubnetMask.c_str(), 255 );
  DRAWITEM( L"Subnet mask", text );

	mbstowcs( text, g_NetworkConfig.m_NameServer.c_str(), 255 );
  DRAWITEM( L"Name server", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawDirectoryPathPage1
//---------------------------------------------------------------------
void COptionsScreen::DrawDirectoryPathPage1( void )
{
/*
  g_FileIOConfig.m_ALTDrive           = iniFile.GetProfileString("Directories", "ALTDrive",            DEFAULT_ALTDRIVE);
  g_FileIOConfig.m_letterCMapping     = iniFile.GetProfileString( "Directories", "C_Mapping",           DEFAULT_CMAPPING );
  g_FileIOConfig.m_letterEMapping     = iniFile.GetProfileString( "Directories", "E_Mapping",           DEFAULT_EMAPPING );
  g_FileIOConfig.m_letterFMapping     = iniFile.GetProfileString( "Directories", "F_Mapping",           DEFAULT_FMAPPING );
  g_FileIOConfig.m_letterGMapping     = iniFile.GetProfileString( "Directories", "G_Mapping",           DEFAULT_GMAPPING );
  g_FileIOConfig.m_letterHMapping     = iniFile.GetProfileString( "Directories", "H_Mapping",           DEFAULT_HMAPPING );
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  mbstowcs( text, g_FileIOConfig.m_ALTDrive.c_str(), 255 );
  DRAWITEM( L"Alternate drive letter", text );

  mbstowcs( text, g_FileIOConfig.m_letterCMapping.c_str(), 255 );
  DRAWITEM( L"C:", text );

  mbstowcs( text, g_FileIOConfig.m_letterEMapping.c_str(), 255 );
  DRAWITEM( L"E:", text );

  mbstowcs( text, g_FileIOConfig.m_letterFMapping.c_str(), 255 );
  DRAWITEM( L"F:", text );

  mbstowcs( text, g_FileIOConfig.m_letterGMapping.c_str(), 255 );
  DRAWITEM( L"G:", text );

  mbstowcs( text, g_FileIOConfig.m_letterHMapping.c_str(), 255 );
  DRAWITEM( L"H:", text );

  ENDPAGE();
}


//---------------------------------------------------------------------
//	DrawDirectoryPathPage2
//---------------------------------------------------------------------
void COptionsScreen::DrawDirectoryPathPage2( void )
{
/*
  g_FileIOConfig.m_artPath            = iniFile.GetProfileString("Directories", "ArtPath",             DEFAULT_ARTPATH);
  g_FileIOConfig.m_audioPath          = iniFile.GetProfileString("Directories", "AudioPath",           DEFAULT_AUDIOPATH);
  g_FileIOConfig.m_configPath         = iniFile.GetProfileString("Directories", "ConfigPath",          DEFAULT_CONFIGPATH);
  g_FileIOConfig.m_DefaultRomListPath = iniFile.GetProfileString("Directories", "DefaultRomsListPath", DEFAULT_DEFAULTROMLISTPATH);
  g_FileIOConfig.m_generalPath        = iniFile.GetProfileString("Directories", "GeneralPath",         DEFAULT_GENERALPATH);
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString("Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH);
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString("Directories", "HiScoresPath",        DEFAULT_HISCOREPATH);
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString("Directories", "NVRamPath",           DEFAULT_NVRAMPATH);
  g_FileIOConfig.m_romBackupPath      = iniFile.GetProfileString("Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH);
  g_FileIOConfig.m_romPath0           = iniFile.GetProfileString("Directories", "RomsPath0",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath1           = iniFile.GetProfileString("Directories", "RomsPath1",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath2           = iniFile.GetProfileString("Directories", "RomsPath2",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_romPath3           = iniFile.GetProfileString("Directories", "RomsPath3",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_screenshotPath     = iniFile.GetProfileString( "Directories", "ScreenshotPath",     DEFAULT_SCREENSHOTPATH );
  g_FileIOConfig.m_autoBootSavePath   = iniFile.GetProfileString( "Directories", "AutoBootSavePath",    DEFAULT_BOOTSAVESTATE );
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  mbstowcs( text, g_FileIOConfig.m_romPath0.c_str(), 255 );
  DRAWITEM( L"ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_romPath1.c_str(), 255 );
  DRAWITEM( L"More ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_romPath2.c_str(), 255 );
  DRAWITEM( L"Yet More ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_romPath3.c_str(), 255 );
  DRAWITEM( L"Even More ROM Files", text );

  mbstowcs( text, g_FileIOConfig.m_romBackupPath.c_str(), 255 );
  DRAWITEM( L"Removed ROMs", text );

  mbstowcs( text, g_FileIOConfig.m_generalPath.c_str(), 255 );
  DRAWITEM( L"General", text );

  mbstowcs( text, g_FileIOConfig.m_artPath.c_str(), 255 );
  DRAWITEM( L"Art", text );

  mbstowcs( text, g_FileIOConfig.m_audioPath.c_str(), 255 );
  DRAWITEM( L"Samples (Audio)", text );

  mbstowcs( text, g_FileIOConfig.m_configPath.c_str(), 255 );
  DRAWITEM( L"Config (CFG's)", text );

  mbstowcs( text, g_FileIOConfig.m_HDImagePath.c_str(), 255 );
  DRAWITEM( L"HD images (CHD's)", text );

  mbstowcs( text, g_FileIOConfig.m_HiScorePath.c_str(), 255 );
  DRAWITEM( L"High scores (highscore.dat)", text );

  mbstowcs( text, g_FileIOConfig.m_NVramPath.c_str(), 255 );
  DRAWITEM( L"NVRAM, state files", text );

  mbstowcs( text, g_FileIOConfig.m_screenshotPath.c_str(), 255 );
  DRAWITEM( L"Screenshots", text );

  mbstowcs( text, g_FileIOConfig.m_autoBootSavePath.c_str(), 255 );
  DRAWITEM( L"Autoboot save states", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawROMListPage
//---------------------------------------------------------------------
void COptionsScreen::DrawROMListPage( void )
{
  static WCHAR *sortMode[] = {  L"By name", 
                                L"By ROM status",
                                L"By manufacturer", 
                                L"By year", 
                                L"By parent ROM",
                                L"By number of players", 
                                L"By favorite status",
                                L"By number of times played",
                                L"By genre" };

  static WCHAR *displayMode[] = { L"Verbose list",
                                  L"Simple list",
                                  L"Detailed single item" };

  STARTPAGE();

  DRAWITEM( L"Display mode", displayMode[g_romListOptions.m_displayMode] );

  DRAWITEM( L"Colorize ROM status", g_romListOptions.m_showROMStatus ? L"Yes" : L"No" );

  DRAWITEM( L"Sort mode", sortMode[(UINT32)g_romListOptions.m_sortMode] );

  DRAWITEM( L"Filter clones",               g_romListOptions.m_filterMode & FM_CLONE ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"slow/frameskip\"",   g_romListOptions.m_filterMode & FM_ROMSTATUS_SLOW ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"out of memory\"",    g_romListOptions.m_filterMode & FM_ROMSTATUS_OUTOFMEM ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"crashes in game\"",  g_romListOptions.m_filterMode & FM_ROMSTATUS_CRASH ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"other nonworking\"", g_romListOptions.m_filterMode & FM_ROMSTATUS_NONWORKING ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"disliked\"",         g_romListOptions.m_filterMode & FM_DISLIKED ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"working\"",          g_romListOptions.m_filterMode & FM_WORKING ? L"Yes" : L"No" );

  ENDPAGE();
}








//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeGeneralPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Cheats (enabled/disabled)
  case 0:
    options.cheat = !options.cheat;
    break;

    // Cheat file
  case 1:
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
	    m_virtualKeyboard->SetData( cheatfile );
      WaitForNoInput();
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
          // Free cheatfile, as it was strdup'd in MAMEoXUtil.cpp
        free( (void*)cheatfile );
        if( !(cheatfile = strdup( m_virtualKeyboard->GetData().c_str() )) )
          Die( m_displayDevice, "Out of memory setting the cheatfile!" );
      }
    }
    break;
/*
    // History file
  case 2:
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
	    m_virtualKeyboard->SetData( history_filename );
      WaitForNoInput();
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
          // Free cheatfile, as it was strdup'd in MAMEoXUtil.cpp
        free( (void*)history_filename );
        if( !(history_filename = strdup( m_virtualKeyboard->GetData().c_str() )) )
          Die( m_displayDevice, "Out of memory setting the history_filename!" );
      }
    }
    break;
*/
      // Disclaimer (skipped/shown)
  case 2:
    options.skip_disclaimer = !options.skip_disclaimer;
    break;

    // Game info (skipped/shown)
  case 3:
    options.skip_gameinfo = !options.skip_gameinfo;
    break;

    // Screensaver
  case 4:
    if( !movingRight )
    {
      if( g_screensaverTimeout )
        --g_screensaverTimeout;
      else
        g_screensaverTimeout = MAX_SCREENSAVER_TIMEOUT;
    }
    else
    {
      if( g_screensaverTimeout < MAX_SCREENSAVER_TIMEOUT )
        ++g_screensaverTimeout;
      else
        g_screensaverTimeout = 0;
    }
  }
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeSoundPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Sound Processing
  case 0:
    g_soundEnabled = !g_soundEnabled;
    break;

    // Sample Rate
  case 1:
    if( options.samplerate == 44100 )
      options.samplerate = 22700;
    else
      options.samplerate = 44100;
    break;

    // Audio Samples
  case 2:
    options.use_samples = !options.use_samples;
    break;

    // Audio Filter
  case 3:
    options.use_filter = !options.use_filter;
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeVideoPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {

      // VSYNC
  case 0:
    g_rendererOptions.m_vsync = !g_rendererOptions.m_vsync;
    break;

     // Framerate throttling
  case 1:
    g_rendererOptions.m_throttleFramerate = !g_rendererOptions.m_throttleFramerate;
    break;

    // Frameskip mode
  case 2:
    if( !movingRight )
    {
        // Reduce
      if( g_rendererOptions.m_frameskip == 0 )
        g_rendererOptions.m_frameskip = AUTO_FRAMESKIP;
      else if( g_rendererOptions.m_frameskip == AUTO_FRAMESKIP )
        g_rendererOptions.m_frameskip = FRAMESKIP_LEVELS - 1;
      else
        --g_rendererOptions.m_frameskip;
    }
    else
    {
        // Increment
      if( g_rendererOptions.m_frameskip == AUTO_FRAMESKIP )
        g_rendererOptions.m_frameskip = 0;
      else if( g_rendererOptions.m_frameskip == FRAMESKIP_LEVELS - 1 )
        g_rendererOptions.m_frameskip = AUTO_FRAMESKIP;
      else
        ++g_rendererOptions.m_frameskip;
    }
    break;

     // Preserve aspect ratio
  case 3:
    g_rendererOptions.m_preserveAspectRatio = !g_rendererOptions.m_preserveAspectRatio;
    break;

    // Screen rotation
  case 4:
    if( !movingRight )
    {
      if( g_rendererOptions.m_screenRotation > SR_0 )
        g_rendererOptions.m_screenRotation = (screenrotation_t)( (LONG)g_rendererOptions.m_screenRotation - 1 );
      else
        g_rendererOptions.m_screenRotation = SR_270;
    }
    else
    {
      if( g_rendererOptions.m_screenRotation < SR_270 )
        g_rendererOptions.m_screenRotation = (screenrotation_t)( (LONG)g_rendererOptions.m_screenRotation + 1 );
      else
        g_rendererOptions.m_screenRotation = SR_0;
    }
    break;

    // Brightness
  case 5:
    {
      if( !movingRight )
        options.brightness -= 0.01f;
      else
        options.brightness += 0.01f;

      if( options.brightness < 0.0f )
        options.brightness = 0.0f;
      else if( options.brightness > 1.0f )
        options.brightness = 1.0f;
    }
    break;

    // Paused brightness
  case 6:
    {
      if( !movingRight )
        options.pause_bright -= 0.01f;
      else
        options.pause_bright += 0.01f;

      if( options.pause_bright < 0.0f )
        options.pause_bright = 0.0f;
      else if( options.pause_bright > 1.0f )
        options.pause_bright = 1.0f;
    }
    break;

    // Gamma
  case 7:
    {
      if( !movingRight )
        options.gamma -= 0.01f;
      else
        options.gamma += 0.01f;

      if( options.gamma < 0.0f )
        options.gamma = 0.0f;
      else if( options.gamma > 2.0f )
        options.gamma = 2.0f;
    }
    break;
/*
    // Color depth
  case 5:
    {
      if( options.color_depth == 32 )
        options.color_depth = 15;
      else
        options.color_depth = 32;
    }
    break;    
*/

    // Minification filter
  case 8:
    if( !movingRight )
    {
      if( g_rendererOptions.m_minFilter > D3DTEXF_POINT )
        g_rendererOptions.m_minFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_minFilter - 1 );
      else
        g_rendererOptions.m_minFilter = D3DTEXF_GAUSSIANCUBIC;
    }
    else
    {
      if( g_rendererOptions.m_minFilter < D3DTEXF_GAUSSIANCUBIC )
        g_rendererOptions.m_minFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_minFilter + 1 );
      else
        g_rendererOptions.m_minFilter = D3DTEXF_POINT;
    }
    break;

    // Magnification filter
  case 9:
    if( !movingRight )
    {
      if( g_rendererOptions.m_magFilter > D3DTEXF_POINT )
        g_rendererOptions.m_magFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_magFilter - 1 );
      else
        g_rendererOptions.m_magFilter = D3DTEXF_GAUSSIANCUBIC;
    }
    else
    {
      if( g_rendererOptions.m_magFilter < D3DTEXF_GAUSSIANCUBIC )
        g_rendererOptions.m_magFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_magFilter + 1 );
      else
        g_rendererOptions.m_magFilter = D3DTEXF_POINT;
    }
    break;
	case 10:
		{
			CGFilterManager oFilterMgr;
			MapFilters mapFilters = oFilterMgr.GetAvailableFilters();

			if (!movingRight)
			{
				g_rendererOptions.m_FilterType -= 1;
			}
			else
			{
				g_rendererOptions.m_FilterType += 1;
			}

			if (g_rendererOptions.m_FilterType > mapFilters.size()-1)
			{
				g_rendererOptions.m_FilterType = 0;
			}
			else if (g_rendererOptions.m_FilterType < 0)
			{
				g_rendererOptions.m_FilterType = mapFilters.size()-1;
			}
		}
		break;
    // Artwork usage
  case 11:
    {
      DWORD artworkState[8] = { 0, 
                                ARTWORK_USE_BACKDROPS, 
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS,
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_BEZELS,
                                ARTWORK_USE_OVERLAYS,
                                ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS,
                                ARTWORK_USE_BEZELS,
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS };

        // Figure out our current state
      DWORD currentState = 0;
      for( ; artworkState[currentState] != options.use_artwork && currentState < (sizeof(artworkState) / sizeof(DWORD)); ++currentState )
        ;

        // Validate the currentState
      if( currentState == (sizeof(artworkState) / sizeof(DWORD)) )
        currentState = 0;

      if( !movingRight )
      {
        if( currentState )
          --currentState;
        else
          currentState = (sizeof(artworkState) / sizeof(DWORD)) - 1;
      }
      else
      {
        if( currentState < (sizeof(artworkState) / sizeof(DWORD)) - 1 )
          ++currentState;
        else
          currentState = 0;
      }

      options.use_artwork = artworkState[currentState];
    }
    break;

  }
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeVectorPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
  case 0:
    {
      if( !movingRight && options.beam > 1 )
        --options.beam;
      else if( movingRight && options.beam < 5 )
        ++options.beam;
    }
    break;

  case 1:
    {
      if( !movingRight )
        options.vector_flicker -= 0.01f;
      else
        options.vector_flicker += 0.01f;

      if( options.vector_flicker < 0.0f )
        options.vector_flicker = 0.0f;
      else if( options.vector_flicker > 1.0f )
        options.vector_flicker = 1.0f;
    }
    break;

  case 2:
    {
      if( !movingRight )
        options.vector_intensity -= 0.01f;
      else
        options.vector_intensity += 0.01f;

      if( options.vector_intensity < 0.0f )
        options.vector_intensity = 0.0f;
      else if( options.vector_intensity > 2.0f )
        options.vector_intensity = 2.0f;
    }
    break;

  case 3:
    {
      options.translucency = !options.translucency;
    }
    break;

  case 4:
    {
      if( !movingRight && options.vector_width > 100 )
        --options.vector_width;
      else if( movingRight && options.vector_width < 640 )
        ++options.vector_width;
    }
    break;

  case 5:
    {
      if( !movingRight && options.vector_height > 100 )
        --options.vector_height;
      else if( movingRight && options.vector_height < 480 )
        ++options.vector_height;
    }
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeNetworkPage( BOOL movingRight )
{
  if( !m_cursorPosition )
  {
    g_NetworkConfig.m_networkDisabled = !g_NetworkConfig.m_networkDisabled;
  }
  else
  {
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
      WaitForNoInput();
      switch( (DWORD)m_cursorPosition )
      {
        // IP address
      case 1:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_IPAddr );
        break;

        // Gateway address
      case 2:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_Gateway );
        break;

        // Subnet mask
      case 3:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_SubnetMask );
        break;

        // Name server
      case 4:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_NameServer );
        break;
      }
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
        switch( (DWORD)m_cursorPosition )
        {
          // IP address
        case 1:
	        g_NetworkConfig.m_IPAddr = m_virtualKeyboard->GetData();
          break;

          // Gateway address
        case 2:
	        g_NetworkConfig.m_Gateway = m_virtualKeyboard->GetData();
          break;

          // Subnet mask
        case 3:
	        g_NetworkConfig.m_SubnetMask = m_virtualKeyboard->GetData();
          break;

          // Name server
        case 4:
	        g_NetworkConfig.m_NameServer = m_virtualKeyboard->GetData();
          break;
        }
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage1
//---------------------------------------------------------------------
void COptionsScreen::ChangeDirectoryPathPage1( BOOL movingRight )
{
  if( !m_virtualKeyboardMode )
  {
    m_virtualKeyboardMode = TRUE;
    WaitForNoInput();
    switch( (DWORD)m_cursorPosition )
    {
      // Alternate drive letter
    case 0:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_ALTDrive );
      break;

      // C:
    case 1:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_letterCMapping );
      break;

      // E:
    case 2:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_letterEMapping );
      break;
    
      // F:
    case 3:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_letterFMapping );
      break;

      // G:
    case 4:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_letterGMapping );
      break;

      // H:
    case 5:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_letterHMapping );
      break;
    }
  }
  else
  {
    if( m_virtualKeyboard->IsInputAccepted() )
    {
      switch( (DWORD)m_cursorPosition )
      {
        // Alternate drive letter
      case 0:
        g_FileIOConfig.m_ALTDrive = m_virtualKeyboard->GetData();
        break;

        // C:
      case 1:
        g_FileIOConfig.m_letterCMapping = m_virtualKeyboard->GetData();
        break;

        // E:
      case 2:
        g_FileIOConfig.m_letterEMapping = m_virtualKeyboard->GetData();
        break;
      
        // F:
      case 3:
        g_FileIOConfig.m_letterFMapping = m_virtualKeyboard->GetData();
        break;

        // G:
      case 4:
        g_FileIOConfig.m_letterGMapping = m_virtualKeyboard->GetData();
        break;

        // H:
      case 5:
        g_FileIOConfig.m_letterHMapping = m_virtualKeyboard->GetData();
        break;
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage2
//---------------------------------------------------------------------
void COptionsScreen::ChangeDirectoryPathPage2( BOOL movingRight )
{
  if( !m_virtualKeyboardMode )
  {
    m_virtualKeyboardMode = TRUE;
    WaitForNoInput();
    switch( (DWORD)m_cursorPosition )
    {
      // ROM Files
    case 0:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_romPath0 );
      break;

      // More ROM Files
    case 1:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_romPath1 );
      break;

      // Yet More ROM Files
    case 2:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_romPath2 );
      break;

      // Even More ROM Files
    case 3:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_romPath3 );
      break;

      // Removed ROMs
    case 4:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_romBackupPath );
      break;

      // General
    case 5:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_generalPath );
      break;

      // Art
    case 6:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_artPath );
      break;

      // Samples (Audio)
    case 7:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_audioPath );
      break;

      // Config (CFG's)
    case 8:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_configPath );
      break;

      // HD images (CHD's)
    case 9:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_HDImagePath );
      break;

      // High scores (highscore.dat)
    case 10:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_HiScorePath );
      break;

      // NVRAM, state files
    case 11:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_NVramPath );
      break;

      // Screenshots
    case 12:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_screenshotPath );
      break;

      // Autoboot save states
    case 13:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_autoBootSavePath );
      break;
    }
  }
  else
  {
    if( m_virtualKeyboard->IsInputAccepted() )
    {
      switch( (DWORD)m_cursorPosition )
      {
        // ROM Files
      case 0:
        g_FileIOConfig.m_romPath0 = m_virtualKeyboard->GetData();
        break;

        // More ROM Files
      case 1:
        g_FileIOConfig.m_romPath1 = m_virtualKeyboard->GetData();
        break;

        // Yet More ROM Files
      case 2:
        g_FileIOConfig.m_romPath2 = m_virtualKeyboard->GetData();
        break;

        // Even More ROM Files
      case 3:
        g_FileIOConfig.m_romPath3 = m_virtualKeyboard->GetData();
        break;

        // Removed ROMs
      case 4:
        g_FileIOConfig.m_romBackupPath = m_virtualKeyboard->GetData();
        break;

        // General
      case 5:
        g_FileIOConfig.m_generalPath = m_virtualKeyboard->GetData();
        break;

        // Art
      case 6:
        g_FileIOConfig.m_artPath = m_virtualKeyboard->GetData();
        break;

        // Samples (Audio)
      case 7:
        g_FileIOConfig.m_audioPath = m_virtualKeyboard->GetData();
        break;

        // Config (CFG's)
      case 8:
        g_FileIOConfig.m_configPath = m_virtualKeyboard->GetData();
        break;

        // HD images (CHD's)
      case 9:
        g_FileIOConfig.m_HDImagePath = m_virtualKeyboard->GetData();
        break;

        // High scores (highscore.dat)
      case 10:
        g_FileIOConfig.m_HiScorePath = m_virtualKeyboard->GetData();
        break;

        // NVRAM, state files
      case 11:
        g_FileIOConfig.m_NVramPath = m_virtualKeyboard->GetData();
        break;

        // Screenshots
      case 12:
        g_FileIOConfig.m_screenshotPath = m_virtualKeyboard->GetData();
        break;

        // Autoboot save states
      case 13:
        g_FileIOConfig.m_autoBootSavePath = m_virtualKeyboard->GetData();
        break;
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeROMListPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeROMListPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Display mode
  case 0:
    if( !movingRight )
    {
      if( g_romListOptions.m_displayMode != DM_VERBOSELIST )
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(g_romListOptions.m_displayMode - 1);
      else
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(DM_LAST - 1);
    }
    else
    {
      if( g_romListOptions.m_displayMode != (ROMListDisplayMode)(DM_LAST - 1) )
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(g_romListOptions.m_displayMode + 1);
      else
        g_romListOptions.m_displayMode = DM_VERBOSELIST;
    }
    break;

    // Colorize ROM status
  case 1:
    g_romListOptions.m_showROMStatus = !g_romListOptions.m_showROMStatus;
    break;

    // Sort mode
  case 2:
    if( !movingRight )
    {
      if( g_romListOptions.m_sortMode != SM_BYNAME )
        g_romListOptions.m_sortMode = (ROMListSortMode)((UINT32)g_romListOptions.m_sortMode - 1);
      else
        g_romListOptions.m_sortMode = (ROMListSortMode)(SM_LAST - 1);
    }
    else
    {
      if( g_romListOptions.m_sortMode < SM_LAST - 1 )
        g_romListOptions.m_sortMode = (ROMListSortMode)((UINT32)g_romListOptions.m_sortMode + 1);
      else
        g_romListOptions.m_sortMode = SM_BYNAME;
    }
    break;

    // Filter clones
  case 3:
    if( g_romListOptions.m_filterMode & FM_CLONE )
      g_romListOptions.m_filterMode &= ~FM_CLONE;
    else
      g_romListOptions.m_filterMode |= FM_CLONE;
    break;

    // Filter "slow"
  case 4:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_SLOW )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_SLOW;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_SLOW;
    break;

    // Filter "out of mem"
  case 5:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_OUTOFMEM )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_OUTOFMEM;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_OUTOFMEM;
    break;

    // Filter "crashes"
  case 6:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_CRASH )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_CRASH;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_CRASH;
    break;

    // Filter "other nonworking"
  case 7:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_NONWORKING )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_NONWORKING;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_NONWORKING;
    break;

    // Filter "disliked"
  case 8:
    if( g_romListOptions.m_filterMode & FM_DISLIKED )
      g_romListOptions.m_filterMode &= ~FM_DISLIKED;
    else
      g_romListOptions.m_filterMode |= FM_DISLIKED;
    break;

    // Filter "working"
  case 9:
    if( g_romListOptions.m_filterMode & FM_WORKING )
      g_romListOptions.m_filterMode &= ~FM_WORKING;
    else
      g_romListOptions.m_filterMode |= FM_WORKING;
    break;
  }
}











//---------------------------------------------------------------------
//  DrawGeneralPage
//---------------------------------------------------------------------
void DrawGeneralPage( COptionsScreen *ptr )
{
  ptr->DrawGeneralPage();
}

//---------------------------------------------------------------------
//  DrawSoundPage
//---------------------------------------------------------------------
void DrawSoundPage( COptionsScreen *ptr )
{
  ptr->DrawSoundPage();
}

//---------------------------------------------------------------------
//  DrawVideoPage
//---------------------------------------------------------------------
void DrawVideoPage( COptionsScreen *ptr )
{
  ptr->DrawVideoPage();
}

//---------------------------------------------------------------------
//  DrawVectorPage
//---------------------------------------------------------------------
void DrawVectorPage( COptionsScreen *ptr )
{
  ptr->DrawVectorPage();
}

//---------------------------------------------------------------------
//  DrawNetworkPage
//---------------------------------------------------------------------
void DrawNetworkPage( COptionsScreen *ptr )
{
  ptr->DrawNetworkPage();
}

//---------------------------------------------------------------------
//  DrawDirectoryPathPage1
//---------------------------------------------------------------------
void DrawDirectoryPathPage1( COptionsScreen *ptr )
{
  ptr->DrawDirectoryPathPage1();
}

//---------------------------------------------------------------------
//  DrawDirectoryPathPage2
//---------------------------------------------------------------------
void DrawDirectoryPathPage2( COptionsScreen *ptr )
{
  ptr->DrawDirectoryPathPage2();
}

//---------------------------------------------------------------------
//  DrawROMListPage
//---------------------------------------------------------------------
void DrawROMListPage( COptionsScreen *ptr )
{
  ptr->DrawROMListPage();
}



//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void ChangeGeneralPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeGeneralPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void ChangeSoundPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeSoundPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void ChangeVideoPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeVideoPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void ChangeVectorPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeVectorPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void ChangeNetworkPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeNetworkPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage1
//---------------------------------------------------------------------
void ChangeDirectoryPathPage1( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeDirectoryPathPage1( movingRight );
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage2
//---------------------------------------------------------------------
void ChangeDirectoryPathPage2( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeDirectoryPathPage2( movingRight );
}

//---------------------------------------------------------------------
//  ChangeROMListPage
//---------------------------------------------------------------------
void ChangeROMListPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeROMListPage( movingRight );
}


