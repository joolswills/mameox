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

	// Font class from the XDK
#include "XBFont.h"

#include "System_IniFile.h"
#include "xbox_Timing.h"
#include "xbox_JoystickMouse.h"
#include "xbox_Direct3DRenderer.h"
#include "xbox_FileIO.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
}

//= S T R U C T U R E S ===============================================

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


//= F U N C T I O N S =================================================

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
// WaitForKey
//-------------------------------------------------------------
void WaitForKey( void )
{
	g_inputManager.WaitForAnyKey( 0 );
}

//-------------------------------------------------------------
// WaitForNoKey
//-------------------------------------------------------------
void WaitForNoKey( void )
{
	g_inputManager.WaitForNoKey( 0 );
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
  FontRender( 320, 300, D3DCOLOR_RGBA(255,255,255,255), memStr, 2 );
  EndFontRender();

	g_inputManager.WaitForNoKey( 0 );
	g_inputManager.WaitForAnyKey( 0 );
	g_inputManager.WaitForNoKey( 0 );
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


}	// End Extern "C"

