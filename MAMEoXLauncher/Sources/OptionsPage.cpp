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

#include "xbox_FileIO.h"		// for path info

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================

#define NORMAL_ITEM_COLOR				D3DCOLOR_RGBA( 100, 255, 100, 255 )
#define SELECTED_ITEM_COLOR			D3DCOLOR_RGBA( 255, 255, 255, 255 )
#define ITEMCOLOR()             i == (ULONG)m_cursorPosition ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							18

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

  // Analog stick deadzone
#define STICK_DEADZONE          0.35f

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

  // Number of seconds between valid left/right trigger readings
#define TRIGGERSWITCH_TIMEOUT   0.5f

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
//	MoveCursor
//---------------------------------------------------------------------
void COptionsPage::MoveCursor( const XINPUT_GAMEPAD	&gp )
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

		// Decrement the dpad movement timer
	if( m_dpadCursorDelay > 0.0f )
	{
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || !gp.wButtons )
			m_dpadCursorDelay = 0.0f;
	}

  if( m_triggerDelay > 0.0f )
  {
    m_triggerDelay -= elapsedTime;
    if( m_triggerDelay < 0.0f || 
        (gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] <= 120 &&
         gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] <= 120 ) )
      m_triggerDelay = 0.0f;
  }

  if( gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 120 && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber )
      --m_pageNumber;
    else
      m_pageNumber = OPTPAGE_LAST - 1;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 120 && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber < OPTPAGE_LAST - 1 )
      ++m_pageNumber;
    else
      m_pageNumber = 0;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) || gp.sThumbLY < (-32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( ++m_cursorPosition >= m_pageData[m_pageNumber].m_numItems )
      m_cursorPosition = 0;
  }
  else if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_UP) || gp.sThumbLY > (32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( m_cursorPosition ) 
      --m_cursorPosition;
    else
      m_cursorPosition = m_pageData[m_pageNumber].m_numItems - 1;
  }
  else if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) || gp.sThumbLX < (-32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, FALSE );
  }
  else if( ((gp.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) || gp.sThumbLX > (32767.0f * STICK_DEADZONE)) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
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
    m_pageData[m_pageNumber].m_drawFunct( this );

    UINT32 prev = m_pageNumber ? m_pageNumber - 1 : OPTPAGE_LAST - 1;
    UINT32 next = m_pageNumber < OPTPAGE_LAST - 1 ? m_pageNumber + 1 : 0;

    m_font.DrawText( 80,  384, SELECTED_ITEM_COLOR, L"<-", 0 );
    m_font.DrawText( 103, 384, SELECTED_ITEM_COLOR, m_pageData[prev].m_title, 0 );

    m_font.DrawText( 545, 384, SELECTED_ITEM_COLOR, L"->", XBFONT_RIGHT );
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
  m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), m_pageData[OPTPAGE_GENERAL].m_title, XBFONT_CENTER_X );

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

  DWORD i = 0;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Cheats", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.cheat ? L"Enabled" : L"Disabled", 0 );

  ++i;
  WCHAR name[256] = {0};
	mbstowcs( name, cheatfile, 255 );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Cheat file", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), name, 0 );

  ++i;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Disclaimer", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.skip_disclaimer ? L"Skipped" : L"Shown", 0 );

  ++i;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Game Info", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.skip_gameinfo ? L"Skipped" : L"Shown", 0 );
}

//---------------------------------------------------------------------
//	DrawSoundPage
//---------------------------------------------------------------------
void COptionsPage::DrawSoundPage( void )
{
  m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), m_pageData[OPTPAGE_SOUND].m_title, XBFONT_CENTER_X );

/*
    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );
*/

  WCHAR text[256] = {0};
  swprintf( text, L"%lu", options.samplerate );

  DWORD i = 0;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Sample Rate", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Audio Samples", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.use_samples ? L"Enabled" : L"Disabled", 0 );

  ++i;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Audio Filter", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.use_filter ? L"Enabled" : L"Disabled", 0 );
}

//---------------------------------------------------------------------
//	DrawVideoPage
//---------------------------------------------------------------------
void COptionsPage::DrawVideoPage( void )
{
  m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), m_pageData[OPTPAGE_VIDEO].m_title, XBFONT_CENTER_X );

/*
	options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
*/

  WCHAR text[256] = {0};

  DWORD i = 0;
  swprintf( text, L"%f", options.brightness );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Brightness", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%f", options.pause_bright );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Paused Brightness", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%f", options.gamma );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Gamma", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%lu", options.color_depth );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Color Depth", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

}

//---------------------------------------------------------------------
//	DrawVectorPage
//---------------------------------------------------------------------
void COptionsPage::DrawVectorPage( void )
{
  m_font.DrawText( 320, 40, D3DCOLOR_RGBA( 255, 255, 255, 255 ), m_pageData[OPTPAGE_VECTOR].m_title, XBFONT_CENTER_X );

/*
	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "Translucency", TRUE );      // 1 to enable translucency on vectors
	options.vector_width = iniFile.GetProfileInt( "VectorOptions", "VectorWidth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "VectorOptions", "VectorHeight", 480 );	    // requested height for vector games; 0 means default (480)
*/
  WCHAR text[256] = {0};

  DWORD i = 0;
  swprintf( text, L"%lu", options.beam );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Beam Width", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%f", options.vector_flicker );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Flicker Effect", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%f", options.vector_intensity );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Beam Intensity", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Translucency", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), options.translucency ? L"Enabled" : L"Disabled", 0 );

  ++i;
  swprintf( text, L"%lu", options.vector_width );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Vector Screen Width", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );

  ++i;
  swprintf( text, L"%lu", options.vector_height );
  m_font.DrawText( 80,  (i*20)+64, ITEMCOLOR(), L"Vector Screen Height", 0 );
  m_font.DrawText( 460, (i*20)+64, ITEMCOLOR(), text, 0 );
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
