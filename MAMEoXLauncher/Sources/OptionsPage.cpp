/**
	* \file			OptionsPage.cpp
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

//= I N C L U D E S ====================================================
#include "OptionsPage.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_FileIO.h"
#include "xbox_Network.h"

#include "xbox_FileIO.h"		// for path info

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================

#define NORMAL_ITEM_COLOR				D3DCOLOR_XRGB( 100, 255, 100 )
#define SELECTED_ITEM_COLOR			D3DCOLOR_XRGB( 255, 255, 255 )
#define HELP_TEXT_COLOR         D3DCOLOR_XRGB( 100, 220, 220 )
#define ITEMCOLOR()             i == (ULONG)m_cursorPosition ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							18

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

  // Number of seconds between valid left/right trigger readings
#define TRIGGERSWITCH_TIMEOUT   0.5f


#define STARTPAGE()                       DWORD i = 0
#define DRAWITEM( _name__, _val__ )       m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), _name__, XBFONT_TRUNCATED, 500 ); \
                                          m_font.DrawText( 360, (i*20)+64, ITEMCOLOR(), _val__, XBFONT_TRUNCATED, 500 ); \
                                          ++i;
#define ENDPAGE()


//= G L O B A L = V A R S ==============================================
extern "C" {
  extern const char *cheatfile;
}

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//  DrawGeneralPage
//---------------------------------------------------------------------
void DrawGeneralPage( COptionsPage *ptr )
{
  ptr->DrawGeneralPage();
}

//---------------------------------------------------------------------
//  DrawSoundPage
//---------------------------------------------------------------------
void DrawSoundPage( COptionsPage *ptr )
{
  ptr->DrawSoundPage();
}

//---------------------------------------------------------------------
//  DrawVideoPage
//---------------------------------------------------------------------
void DrawVideoPage( COptionsPage *ptr )
{
  ptr->DrawVideoPage();
}

//---------------------------------------------------------------------
//  DrawVectorPage
//---------------------------------------------------------------------
void DrawVectorPage( COptionsPage *ptr )
{
  ptr->DrawVectorPage();
}

//---------------------------------------------------------------------
//  DrawNetworkPage
//---------------------------------------------------------------------
void DrawNetworkPage( COptionsPage *ptr )
{
  ptr->DrawNetworkPage();
}

//---------------------------------------------------------------------
//  DrawDirectoryPathPage
//---------------------------------------------------------------------
void DrawDirectoryPathPage( COptionsPage *ptr )
{
  ptr->DrawDirectoryPathPage();
}



//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void ChangeGeneralPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeGeneralPage( direction );
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void ChangeSoundPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeSoundPage( direction );
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void ChangeVideoPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeVideoPage( direction );
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void ChangeVectorPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeVectorPage( direction );
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void ChangeNetworkPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeNetworkPage( direction );
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage
//---------------------------------------------------------------------
void ChangeDirectoryPathPage( COptionsPage *ptr, BOOL direction )
{
  ptr->ChangeDirectoryPathPage( direction );
}






//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void COptionsPage::MoveCursor( CGamepad	&gp )
{
		// General idea taken from XMAME
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
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
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        !gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) )
			m_dpadCursorDelay = 0.0f;
	}

	if( m_optToggleDelay > 0.0f )
	{
		m_optToggleDelay -= elapsedTime;
    if( m_optToggleDelay < 0.0f || 
        !gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_DPAD_RIGHT | GP_LA_LEFT | GP_LA_RIGHT ) )
			m_optToggleDelay = 0.0f;
	}

  if( m_triggerDelay > 0.0f )
  {
    m_triggerDelay -= elapsedTime;
    if( m_triggerDelay < 0.0f || !gp.IsOneOfButtonsPressed( GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER ) )
      m_triggerDelay = 0.0f;
  }


    //-- Check the button states ----------------------------------------------------------
  if( gp.IsButtonPressed( GP_LEFT_TRIGGER ) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber )
      --m_pageNumber;
    else
      m_pageNumber = OPTPAGE_LAST - 1;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( gp.IsButtonPressed( GP_RIGHT_TRIGGER ) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber < OPTPAGE_LAST - 1 )
      ++m_pageNumber;
    else
      m_pageNumber = 0;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( ++m_cursorPosition >= m_pageData[m_pageNumber].m_numItems )
      m_cursorPosition = 0;
  }
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( m_cursorPosition ) 
      --m_cursorPosition;
    else
      m_cursorPosition = m_pageData[m_pageNumber].m_numItems - 1;
  }
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_LA_LEFT ) && m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, FALSE );
  }
  else if( gp.IsOneOfButtonsPressed( GP_DPAD_RIGHT | GP_LA_RIGHT ) && m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, TRUE );
  }
  
}



//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void COptionsPage::Draw( BOOL opaque, BOOL flipOnCompletion )
{
		// Display the error to the user
  if( opaque )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

	m_font.Begin();
    m_font.DrawText( 320, 40, D3DCOLOR_XRGB( 255, 255, 255 ), m_pageData[m_pageNumber].m_title, XBFONT_CENTER_X );
    m_pageData[m_pageNumber].m_drawFunct( this );

    UINT32 prev = m_pageNumber ? m_pageNumber - 1 : OPTPAGE_LAST - 1;
    UINT32 next = m_pageNumber < OPTPAGE_LAST - 1 ? m_pageNumber + 1 : 0;

    m_font.DrawText( 103, 360, HELP_TEXT_COLOR, L"Left Trigger", 0 );
    m_font.DrawText( 80,  384, HELP_TEXT_COLOR, L"<-", 0 );
    m_font.DrawText( 103, 384, SELECTED_ITEM_COLOR, m_pageData[prev].m_title, 0 );

    m_font.DrawText( 520, 360, HELP_TEXT_COLOR, L"Right Trigger", XBFONT_RIGHT );
    m_font.DrawText( 545, 384, HELP_TEXT_COLOR, L"->", XBFONT_RIGHT );
    m_font.DrawText( 520, 384, SELECTED_ITEM_COLOR, m_pageData[next].m_title, XBFONT_RIGHT );  
	m_font.End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}

//---------------------------------------------------------------------
//	DrawGeneralPage
//---------------------------------------------------------------------
void COptionsPage::DrawGeneralPage( void )
{
/*
  options.cheat = iniFile.GetProfileInt( "General", "CheatsEnabled", FALSE );
  cheatfile = strdup( iniFile.GetProfileString( "General", "CheatFilename", "cheat.dat" ).c_str() );
  if( !cheatfile )
    options.cheat = FALSE;

    // 1 to skip the disclaimer screen at startup
  options.skip_disclaimer = iniFile.GetProfileInt( "General", "SkipDisclaimer", FALSE );

    // 1 to skip the game info screen at startup
	options.skip_gameinfo = iniFile.GetProfileInt( "General", "SkipGameInfo", FALSE );
*/
  STARTPAGE();

  DRAWITEM( L"Cheats", options.cheat ? L"Enabled" : L"Disabled" );

  WCHAR name[256] = {0};
	mbstowcs( name, cheatfile, 255 );
  DRAWITEM( L"Cheat file", name );

  DRAWITEM( L"Disclaimer", options.skip_disclaimer ? L"Skipped" : L"Shown" );

  DRAWITEM( L"Game Info", options.skip_gameinfo ? L"Skipped" : L"Shown" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawSoundPage
//---------------------------------------------------------------------
void COptionsPage::DrawSoundPage( void )
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

  swprintf( text, L"%lu", options.samplerate );
  DRAWITEM( L"Sample Rate", text );

  DRAWITEM( L"Audio Samples", options.use_samples ? L"Enabled" : L"Disabled" );

  DRAWITEM( L"Audio Filter", options.use_filter ? L"Enabled" : L"Disabled" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVideoPage
//---------------------------------------------------------------------
void COptionsPage::DrawVideoPage( void )
{
/*
	options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
*/

  WCHAR text[256] = {0};

  STARTPAGE();

  swprintf( text, L"%f", options.brightness );
  DRAWITEM( L"Brightness", text );

  swprintf( text, L"%f", options.pause_bright );
  DRAWITEM( L"Paused Brightness", text );

  swprintf( text, L"%f", options.gamma );
  DRAWITEM( L"Gamma", text );

  swprintf( text, L"%lu", options.color_depth );
  DRAWITEM( L"Color Depth", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVectorPage
//---------------------------------------------------------------------
void COptionsPage::DrawVectorPage( void )
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
void COptionsPage::DrawNetworkPage( void )
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
//	DrawDirectoryPathPage
//---------------------------------------------------------------------
void COptionsPage::DrawDirectoryPathPage( void )
{
/*
  g_FileIOConfig.m_ALTDrive           = iniFile.GetProfileString("Directories", "ALTDrive",            DEFAULT_ALTDRIVE);
  g_FileIOConfig.m_ArtPath            = iniFile.GetProfileString("Directories", "ArtPath",             DEFAULT_ARTPATH);
  g_FileIOConfig.m_AudioPath          = iniFile.GetProfileString("Directories", "AudioPath",           DEFAULT_AUDIOPATH);
  g_FileIOConfig.m_ConfigPath         = iniFile.GetProfileString("Directories", "ConfigPath",          DEFAULT_CONFIGPATH);
  g_FileIOConfig.m_DefaultRomListPath = iniFile.GetProfileString("Directories", "DefaultRomsListPath", DEFAULT_DEFAULTROMLISTPATH);
  g_FileIOConfig.m_GeneralPath        = iniFile.GetProfileString("Directories", "GeneralPath",         DEFAULT_GENERALPATH);
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString("Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH);
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString("Directories", "HiScoresPath",        DEFAULT_HISCOREPATH);
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString("Directories", "NVRamPath",           DEFAULT_NVRAMPATH);
  g_FileIOConfig.m_RomBackupPath      = iniFile.GetProfileString("Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH);
  g_FileIOConfig.m_RomPath            = iniFile.GetProfileString("Directories", "RomsPath",            DEFAULT_ROMPATH);
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  mbstowcs( text, g_FileIOConfig.m_ALTDrive.c_str(), 255 );
  DRAWITEM( L"Alternate drive letter", text );

  mbstowcs( text, g_FileIOConfig.m_RomPath.c_str(), 255 );
  DRAWITEM( L"ROM Files", text );

  mbstowcs( text, g_FileIOConfig.m_RomBackupPath.c_str(), 255 );
  DRAWITEM( L"Removed ROMs", text );

  mbstowcs( text, g_FileIOConfig.m_GeneralPath.c_str(), 255 );
  DRAWITEM( L"General", text );

  mbstowcs( text, g_FileIOConfig.m_ArtPath.c_str(), 255 );
  DRAWITEM( L"Art", text );

  mbstowcs( text, g_FileIOConfig.m_AudioPath.c_str(), 255 );
  DRAWITEM( L"Samples (Audio)", text );

  mbstowcs( text, g_FileIOConfig.m_ConfigPath.c_str(), 255 );
  DRAWITEM( L"Config (CFG's)", text );

  mbstowcs( text, g_FileIOConfig.m_HDImagePath.c_str(), 255 );
  DRAWITEM( L"HD images (CHD's)", text );

  mbstowcs( text, g_FileIOConfig.m_HiScorePath.c_str(), 255 );
  DRAWITEM( L"High scores (highscore.dat)", text );

  mbstowcs( text, g_FileIOConfig.m_NVramPath.c_str(), 255 );
  DRAWITEM( L"NVRAM, state files", text );





  ENDPAGE();
}



//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void COptionsPage::ChangeGeneralPage( BOOL direction )
{
  switch( m_cursorPosition )
  {
  case 0:
    options.cheat = !options.cheat;
    break;

  case 1:
    break;

  case 2:
    options.skip_disclaimer = !options.skip_disclaimer;
    break;

  case 3:
    options.skip_gameinfo = !options.skip_gameinfo;
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void COptionsPage::ChangeSoundPage( BOOL direction )
{
  switch( m_cursorPosition )
  {
  case 0:
    break;

  case 1:
    options.use_samples = !options.use_samples;
    break;

  case 2:
    options.use_filter = !options.use_filter;
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void COptionsPage::ChangeVideoPage( BOOL direction )
{
  switch( m_cursorPosition )
  {
  case 0:
    {
      if( !direction )
        options.brightness -= 0.01f;
      else
        options.brightness += 0.01f;

      if( options.brightness < 0.0f )
        options.brightness = 0.0f;
      else if( options.brightness > 1.0f )
        options.brightness = 1.0f;
    }
    break;

  case 1:
    {
      if( !direction )
        options.pause_bright -= 0.01f;
      else
        options.pause_bright += 0.01f;

      if( options.pause_bright < 0.0f )
        options.pause_bright = 0.0f;
      else if( options.pause_bright > 1.0f )
        options.pause_bright = 1.0f;
    }
    break;

  case 2:
    {
      if( !direction )
        options.gamma -= 0.01f;
      else
        options.gamma += 0.01f;

      if( options.gamma < 0.0f )
        options.gamma = 0.0f;
      else if( options.gamma > 2.0f )
        options.gamma = 2.0f;
    }
    break;

  case 3:
    {
      if( options.color_depth == 32 )
        options.color_depth = 15;
      else
        options.color_depth = 32;
    }
    break;    
  }
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void COptionsPage::ChangeVectorPage( BOOL direction )
{
  switch( m_cursorPosition )
  {
  case 0:
    {
      if( !direction && options.beam > 1 )
        --options.beam;
      else if( direction && options.beam < 5 )
        ++options.beam;
    }
    break;

  case 1:
    {
      if( !direction )
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
      if( !direction )
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
      if( !direction && options.vector_width > 100 )
        --options.vector_height;
      else if( direction && options.vector_width < 640 )
        ++options.vector_height;
    }
    break;

  case 5:
    {
      if( !direction && options.vector_height > 100 )
        --options.vector_height;
      else if( direction && options.vector_height < 480 )
        ++options.vector_height;
    }
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void COptionsPage::ChangeNetworkPage( BOOL direction )
{
  // Mostly read-only until virtual keyboard is written
  switch( m_cursorPosition )
  {
  case 0:
    g_NetworkConfig.m_networkDisabled = !g_NetworkConfig.m_networkDisabled;
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage
//---------------------------------------------------------------------
void COptionsPage::ChangeDirectoryPathPage( BOOL direction )
{
  // Read-only until virtual keyboard is written
}


