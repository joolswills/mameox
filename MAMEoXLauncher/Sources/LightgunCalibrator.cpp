/**
	* \file			LightgunCalibrator.cpp
	* \brief		Helper class to handle lightgun calibration
	*
	*/

//= I N C L U D E S ====================================================
#include "LightgunCalibrator.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//  MoveCursor
//---------------------------------------------------------------------
void CLightgunCalibrator::MoveCursor( CInputManager &inputManager, BOOL unused )
{
/*
  if( inputManager.IsButtonPressed( GP_B ) )
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
*/

  /*
lightgunCalibration_t    g_calibrationData[4] = { {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767},
                                                  {-32767,0,32767,32767,0,-32767} };
  */


  const XINPUT_CAPABILITIES *gp;

    // Make sure we've got a lightgun to calibrate
  if( !(gp = GetGamepadCaps( m_currentInputDeviceIndex )) || gp->SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
  {
      // Find the first attached lightgun
    for( m_currentInputDeviceIndex = 0; m_currentInputDeviceIndex < 4; ++m_currentInputDeviceIndex )
    {
      if( (gp = GetGamepadCaps( m_currentInputDeviceIndex )) && gp->SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
        break;
    }

    if( m_currentInputDeviceIndex == 4 )
    {
      m_currentInputDeviceIndex = 0;
      m_calibrationCompleted = TRUE;
      return;
    }
  }

    // Update the cursor position
  const XINPUT_GAMEPAD *gpState = inputManager.GetGamepadDeviceState( m_currentInputDeviceIndex );
  if( gpState )
  {
    m_currentGunX = gpState->sThumbLX;
    m_currentGunY = gpState->sThumbLY;
  }

}

//---------------------------------------------------------------------
//  Draw
//---------------------------------------------------------------------
void CLightgunCalibrator::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

    // Render the backdrop texture
  RenderBackdrop();


  #define CURSOR_COLOR                  D3DCOLOR_RGBA( 255, 100, 100, 255 )
  static WCHAR *calibrationStepText[] = { L"Upper Left", L"Center", L"Lower Right" };
  WCHAR wBuf[256];

  m_fontSet.DefaultFont().Begin();

    m_fontSet.DefaultFont().DrawText( 320, 180, D3DCOLOR_RGBA( 255, 255, 255, 255), calibrationStepText[m_calibrationStep], XBFONT_CENTER_X );

    swprintf( wBuf, L"LX: %d", m_currentGunX );
	  m_fontSet.DefaultFont().DrawText( 320, 80, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );

    swprintf( wBuf, L"LY: %d", m_currentGunX );
	  m_fontSet.DefaultFont().DrawText( 320, 100, D3DCOLOR_RGBA( 255, 255, 255, 255), wBuf, XBFONT_CENTER_X );

  m_fontSet.DefaultFont().End();


    // Render the cursor
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

  m_displayDevice->SetTexture( 0, m_cursorTexture );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_currentGunX - 16, m_currentGunY - 16, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_currentGunX + 16, m_currentGunY - 16, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_currentGunX + 16, m_currentGunY + 16, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_currentGunX - 16, m_currentGunY + 16, 1.0f, 1.0f );
  m_displayDevice->End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


//---------------------------------------------------------------------
//  UpdateLightgunCursorPosition
//---------------------------------------------------------------------
void CLightgunCalibrator::UpdateLightgunCursorPosition( CInputManager &inputManager )
{
	const XINPUT_GAMEPAD *gp;
  if( (gp = inputManager.GetGamepadDeviceState( m_currentInputDeviceIndex )) )
  {
    lightgunCalibration_t &calibData = g_calibrationData[m_currentInputDeviceIndex];

    m_currentGunX = gp->sThumbLX - calibData.m_xData[1];
    m_currentGunY = -1 * (gp->sThumbLY - calibData.m_yData[1]);

      // Map from -128 to 128
    if( gp->sThumbLX < 0 )
      m_currentGunX = (int)((FLOAT)m_currentGunX * 128.0f / ((FLOAT)calibData.m_xData[0]+1.0f));
    else
      m_currentGunX = (int)((FLOAT)m_currentGunX * 128.0f / ((FLOAT)calibData.m_xData[2]+1.0f));

    if( gp->sThumbLY > 0 )
      m_currentGunY = (int)((FLOAT)m_currentGunY * 128.0f / ((FLOAT)calibData.m_yData[0]+1.0f));
    else
      m_currentGunY = (int)((FLOAT)m_currentGunY * 128.0f / ((FLOAT)calibData.m_yData[2]+1.0f));

      // Lock to the expected range
    if( m_currentGunX > 128 )
      m_currentGunX = 128;
    else if( m_currentGunX < -128 )
      m_currentGunX = -128;

    if( m_currentGunY > 128 )
      m_currentGunY = 128;
    else if( m_currentGunY < -128 )
      m_currentGunY = -128;
  }
  else  
	  m_currentGunX = m_currentGunY = 0;
}



#if 0


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
#endif

