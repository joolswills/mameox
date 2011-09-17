/**
	* \file			LightgunCalibratorScreen.cpp
	* \brief		Helper class to handle lightgun calibration
	*
	*/

//= I N C L U D E S ====================================================
#include "LightgunCalibrationScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}

#include "Lightgun.h"

CLightgun g_lightgun ;

//= D E F I N E S ======================================================
  // The number of calibration steps per device
#define NUM_CALIBRATIONSTEPS    3

  // Note: These values are halved for the cursor
#define TARGET_WIDTH    64
#define TARGET_HEIGHT   64

#define TARGET_UL_X   76 - (TARGET_WIDTH >> 1)
#define TARGET_UL_Y   57 - (TARGET_WIDTH >> 1)

#define TARGET_C_X    320 - (TARGET_WIDTH>>1)
#define TARGET_C_Y    240 - (TARGET_HEIGHT>>1)

#define TARGET_LR_X   555
#define TARGET_LR_Y   400

#define TEXTCOLOR                     D3DCOLOR_XRGB( 10,10,10 )
#define CURSOR_COLOR                  D3DCOLOR_RGBA( 255, 100, 100, 255 )

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//  MoveCursor
//---------------------------------------------------------------------

LPD3DXSPRITE			MenuSprite = NULL;

void ColorBar( int xpos, int ypos, int xlen, int ylen, DWORD color )
{
	LPDIRECT3DTEXTURE8	WhiteTexture;
	
	D3D__pDevice->BeginScene() ;
	D3D__pDevice->EndScene() ;
	// Create the texture
	D3DXCreateTexture(D3D__pDevice, 1, 1, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &WhiteTexture);
	
	D3DSURFACE_DESC desc;
    WhiteTexture->GetLevelDesc(0, &desc);


	RECT rectSource;
	rectSource.top = 0;
	rectSource.left = 0;
	rectSource.bottom = 0;
	rectSource.right  = 0 ;

	D3DLOCKED_RECT d3dlr;
	WhiteTexture->LockRect(0, &d3dlr, &rectSource, 0);

	memcpy( d3dlr.pBits, &color, sizeof(color) ) ;

	// Unlock our texture
	WhiteTexture->UnlockRect(0);

	if ( MenuSprite == NULL )
		D3DXCreateSprite(D3D__pDevice, &MenuSprite);


	D3DXVECTOR2 vecScale( 1, 1 ) ;
	D3DXVECTOR2 vecTranslate( (float)xpos, (float)ypos ) ;
	D3DXCOLOR d3color(1.0, 1.0, 1.0, 1.0);

	rectSource.top = 0;
	rectSource.left = 0;
	rectSource.bottom = ylen - 1 ;
	rectSource.right  = xlen - 1;
	
	MenuSprite->Begin() ;
	MenuSprite->Draw(WhiteTexture, &rectSource, &vecScale, NULL, 0, &vecTranslate, d3color);
	MenuSprite->End() ;

	//MenuSprite->Release() ;
	WhiteTexture->Release() ;
}


void CLightgunCalibrationScreen::MoveCursor( CInputManager &inputManager, BOOL unused )
{
	int menuChoice = 0 ;
	int doingCenter = 1 ;
	int foundgun ;
	int waspressed = 0 ;
    SHORT centerX;
    SHORT centerY;
    SHORT ulX;
    SHORT ulY;
	SHORT minX, minY, maxX, maxY ;
	SHORT posX, posY ;
	int firsttime = 1 ;
	BOOL changeValues = TRUE ;
	const XINPUT_CAPABILITIES *gpCaps;

	maxX = maxY = -32000 ;
	minX = minY = 32000 ;
	posX = 0 ;
	posY = 0 ;



	while ( 1 )
	{

	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(243,243,243),				// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

    // Render the backdrop texture
	  RenderBackdrop();

		WCHAR wBuf[256];

		m_fontSet.DefaultFont().Begin();


		swprintf( wBuf, L"Calibrate Lightgun" );
		m_fontSet.DefaultFont().DrawText( 320, 160, TEXTCOLOR, wBuf, XBFONT_CENTER_X );



		foundgun = 0 ;

		inputManager.PollDevices() ;

		// Check all ports for a lightgun
		if( !(gpCaps = GetGamepadCaps( m_currentInputDeviceIndex )) || gpCaps->SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
		{
			m_currentInputDeviceIndex = 0;
			if( !FindNextGun() )
			{
				m_currentInputDeviceIndex = 0;
				m_calibrationCompleted = TRUE;
				return;
			}
		}

		CGamepad *gp = inputManager.GetGamepad( m_currentInputDeviceIndex );

		if( !gp )
		{
			PRINTMSG(( T_ERROR, "Could not retrieve CGamepad object for index %d", m_currentInputDeviceIndex ));
			return;
		}

		g_lightgun.hDevice = gp->m_deviceHandle ;
		g_lightgun.sThumbLX = gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_X ) ;
		g_lightgun.sThumbLY = gp->GetAnalogAxisState( GP_ANALOG_LEFT, GP_AXIS_Y ) ;
		g_lightgun.wButtons = gp->GetGamepadDeviceState()->wButtons ;

		if ( gp->IsButtonPressed( GP_A ) )
		{
			if ( waspressed )
			{
				g_lightgun.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = FALSE ;
			}
			else
			{
				g_lightgun.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = TRUE ;
			}
			waspressed = 1;
		}
		else
		{
			g_lightgun.bPressedAnalogButtons[XINPUT_GAMEPAD_A] = FALSE ;
			waspressed = 0 ;
		}


		m_currentGunFlags = gp->GetLightgunFlags();

		foundgun = 1 ;


		if ( foundgun == 0 )
		{
			m_calibrationCompleted = TRUE;
			return ;
		}

		if ( firsttime )
		{
			if ( changeValues )
				g_lightgun.ResetCalibrationOffsets( ) ;
			//sprintfx( "reset calibration\r\n") ;
			firsttime = 0 ;
		}

	    FLOAT fTargetX, fTargetY;

		if ( doingCenter )
		{
			swprintf( wBuf, L"Aim at the Center Colored Target and Shoot" );
			m_fontSet.DefaultFont().DrawText( 320, 190, TEXTCOLOR, wBuf, XBFONT_CENTER_X );

	        g_lightgun.GetCalibrationTargetCoords( &fTargetX, &fTargetY, NULL, NULL );

	        //ColorBar( 640.0f *VIDEOMODES[m_xboxVidmode].multx/2 - 8, 480.0f *VIDEOMODES[m_xboxVidmode].multy/2 - 8, 16, 16, 0xffFFffFF);
	        //ColorBar( fTargetX-8, fTargetY-8, 16, 16, 0xffFFffFF);
		}
		else if (doingCenter == 0 )
		{
			swprintf( wBuf, L"Aim at the UpperLeft Color Target and Shoot" ) ;
			m_fontSet.DefaultFont().DrawText( 320, 190, TEXTCOLOR, wBuf, XBFONT_CENTER_X );
	        g_lightgun.GetCalibrationTargetCoords( NULL, NULL, &fTargetX, &fTargetY);
	        //ColorBar( fTargetX-8, fTargetY-8, 16, 16, 0xffFFffFF);
	        //ColorBar( 48, 48, 16, 16, 0xffFFffFF);
		}







		// Make sure the gun is properly calibrated. Note that this is called every
		// frame in case the state of the display changes
		g_lightgun.VerifyCalibrationState( FALSE );

		// Set the vibration motors
		//m_Lightgun.SetVibrationMotors( m_wLeftMotorSpeed, m_wRightMotorSpeed );


		BOOL bShotFired;
		BOOL bShotHitScreen;
		BOOL bShotMissedScreen;
		g_lightgun.Update( &bShotFired, &bShotHitScreen, &bShotMissedScreen );


		FLOAT fWidth = 640.0f ;
		FLOAT fHeight = 480.0f ;
		FLOAT fThumbLX = (fWidth/2) + (fWidth/2)*(g_lightgun.sThumbLX+0.5f)/32767.5f;
		FLOAT fThumbLY = (fHeight/2) - (fHeight/2)*(g_lightgun.sThumbLY+0.5f)/32767.5f;

	    //ColorBar( posX-10, posY-10, 20,20, 0xFF00FF00 );
		if ( !changeValues && ( g_lightgun.wButtons & XINPUT_LIGHTGUN_ONSCREEN  ) )
		{
			//ColorBar( fThumbLX-1, fThumbLY-8, 2, 16, 0xFFFF0000);
			//ColorBar( fThumbLX-8, fThumbLY-1, 16, 2, 0xFFFF0000);
		}

		//sprintfx( "x=%d, y=%d  MIN(%d,%d) MAX(%d,%d) POS(%d,%d)\r\n", m_lightgun.sThumbLX, m_lightgun.sThumbLY, minX, minY, maxX, maxY, posX, posY) ;

		swprintf( wBuf, L"x=%d, y=%d", g_lightgun.sThumbLX, g_lightgun.sThumbLY) ;
		m_fontSet.DefaultFont().DrawText( 320, 390, TEXTCOLOR, wBuf, XBFONT_CENTER_X );

	  m_fontSet.DefaultFont().End();

		// Check if the lightgun is onscreen and the trigger was pulled
		if( bShotHitScreen )
		{
			// Calibrate the center point
			if( doingCenter )
			{
				centerX = g_lightgun.sThumbLX;
				centerY = g_lightgun.sThumbLY;

				//sprintfx( "HIT  x=%d, y=%d\r\n", m_lightgun.sThumbLX, m_lightgun.sThumbLY) ;

				// Set the new calibration offsets
				if ( changeValues )
					g_lightgun.SetCalibrationOffsets( centerX, 
												  centerY,
												  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_X,
												  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_Y );

				doingCenter = 0;
			}
			else if ( doingCenter == 0 ) // Calibrate the upperleft point
			{
				ulX = g_lightgun.sThumbLX;
				ulY = g_lightgun.sThumbLY;

				// Set the new calibration offsets and save them for future retrieval
				if ( changeValues )
					g_lightgun.SetCalibrationOffsets( centerX, 
												  centerY,
												  ulX,
												  ulY );

				doingCenter = 2 ;
				//sprintfx( "HIT  x=%d, y=%d\r\n", m_lightgun.sThumbLX, m_lightgun.sThumbLY) ;
				//return ;
			}
		}

  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	if( CheckResourceValidity( SPRITE_LIGHTGUNCALIBRATION_CURSOR ) )
	{
		m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );
		g_loadedSkin->SelectSkinTexture( m_displayDevice, SPRITE_LIGHTGUNCALIBRATION_CURSOR );
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_LIGHTGUNCALIBRATION_CURSOR );

			// Display a target/cursor if the gun is pointed at the screen
		if(  m_currentGunFlags & XINPUT_LIGHTGUN_ONSCREEN )
		{
			m_displayDevice->Begin( D3DPT_QUADLIST );


				switch( doingCenter )
				{
				case 1:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X, TARGET_C_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X + TARGET_WIDTH, TARGET_C_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X + TARGET_WIDTH, TARGET_C_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X, TARGET_C_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;

				case 0:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X, TARGET_UL_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X + TARGET_WIDTH, TARGET_UL_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X + TARGET_WIDTH, TARGET_UL_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X, TARGET_UL_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;

	/*
				case 2:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X, TARGET_LR_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X + TARGET_WIDTH, TARGET_LR_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X + TARGET_WIDTH, TARGET_LR_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X, TARGET_LR_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;
	*/

				case (NUM_CALIBRATIONSTEPS - 1):
					{

						FLOAT fWidth = 640.0f ;
						FLOAT fHeight = 480.0f ;
						FLOAT x = (fWidth/2) + (fWidth/2)*(g_lightgun.sThumbLX+0.5f)/32767.5f;
						FLOAT y = (fHeight/2) - (fHeight/2)*(g_lightgun.sThumbLY+0.5f)/32767.5f;
							// Map the cursor to screen coords
						//x = ((x+128.0f) * 640.0f / 256.0f);
						//y = ((y+128.0f) * 480.0f / 256.0f);
	          
						x -= (TARGET_WIDTH >> 2);
						y -= (TARGET_HEIGHT >> 2);

						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x, y, 1.0f, 1.0f );
	          
						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x + (TARGET_WIDTH>>1), y, 1.0f, 1.0f );
	          
						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x + (TARGET_WIDTH>>1), y + (TARGET_HEIGHT>>1), 1.0f, 1.0f );

						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x, y + (TARGET_HEIGHT>>1), 1.0f, 1.0f );
					}
					break;
				}
		m_displayDevice->End();
		}
	}
		m_displayDevice->Present( NULL, NULL, NULL, NULL );

		if( inputManager.IsButtonPressed( GP_B ) )
		{
			break ;
		}
		else if( inputManager.IsButtonPressed( GP_A ) ) 
		{
		}


	}

	m_calibrationCompleted = TRUE;

}

void CLightgunCalibrationScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
}

#if 0 //remove old MoveCursor and Draw methods

void CLightgunCalibrationScreen::MoveCursor( CInputManager &inputManager, BOOL unused )
{
  const XINPUT_CAPABILITIES *gpCaps;
    // Make sure we've got a lightgun to calibrate
  if( !(gpCaps = GetGamepadCaps( m_currentInputDeviceIndex )) || gpCaps->SubType != XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
  {
    m_currentInputDeviceIndex = 0;
    if( !FindNextGun() )
    {
      m_currentInputDeviceIndex = 0;
      m_calibrationCompleted = TRUE;
      return;
    }
  }

  CGamepad *gp = inputManager.GetGamepad( m_currentInputDeviceIndex );
  if( !gp )
  {
    PRINTMSG(( T_ERROR, "Could not retrieve CGamepad object for index %d", m_currentInputDeviceIndex ));
    return;
  }

  lightgunCalibration_t &calibData = g_calibrationData[m_currentInputDeviceIndex];

    // Update the cursor position
  m_currentGunFlags = gp->GetLightgunFlags();
  const XINPUT_GAMEPAD *gpState = gp->GetGamepadDeviceState();
  if( gpState && gp->IsLightgunPointedAtScreen() )
  {
    m_currentGunX = gpState->sThumbLX;
    m_currentGunY = gpState->sThumbLY;
  }
  else
    m_currentGunX = m_currentGunY = 0;


    // If this is the first frame for the selected
    // gun, and if the gun is already calibrated,
    // start up at calibration step NUM_CALIBRATIONSTEPS - 1
  if( m_isFirstFrame )
  {
    m_isFirstFrame = FALSE;
    if( calibData.m_xData[2] == TRUE )
      m_calibrationStep = NUM_CALIBRATIONSTEPS - 1;
  }

  if( gp->IsButtonPressed( GP_B ) )
  {
      // Throw away calibration data for this gun
    memset( &calibData, 0, sizeof(calibData) );
    gp->SetLightgunCalibration( 0, 
                                0, 
                                0, 
                                0 );
      // Reset calibration step to 0
    m_calibrationStep = 0;
  }
  else if( gp->IsButtonPressed( GP_A ) &&
           gp->IsLightgunPointedAtScreen() )
  {
      // Store the value
    if( m_calibrationStep < (NUM_CALIBRATIONSTEPS - 1) )
    {
      calibData.m_xData[m_calibrationStep] = m_currentGunX;
      calibData.m_yData[m_calibrationStep] = m_currentGunY;
    }

      // Move on to the next step
    if( ++m_calibrationStep == NUM_CALIBRATIONSTEPS )
    {
        // This gun is finished, try to find another one,
        // quitting if we can't
      m_calibrationStep = 0;
      ++m_currentInputDeviceIndex;
      if( !FindNextGun() )
      {
        m_currentInputDeviceIndex = 0;
        m_calibrationCompleted = TRUE;
        gp->WaitForNoButton();
        return;        
      }
    }
    else
      gp->WaitForNoButton();  // Wait for the trigger to be released

    if( m_calibrationStep == 1 )
    {
      gp->SetLightgunCalibration( XINPUT_LIGHTGUN_CALIBRATION_CENTER_X - calibData.m_xData[CALIB_C], 
                                  XINPUT_LIGHTGUN_CALIBRATION_CENTER_Y - calibData.m_yData[CALIB_C], 
                                  0, 
                                  0 );
    }
    else if( m_calibrationStep == (NUM_CALIBRATIONSTEPS - 1) )
    {
        // We've collected UL and center values, throw them to the gun
      calibData.m_xData[2] = TRUE;
      calibData.m_yData[2] = gp->GetLightgunFlags();
      gp->SetLightgunCalibration( XINPUT_LIGHTGUN_CALIBRATION_CENTER_X - calibData.m_xData[CALIB_C], 
                                  XINPUT_LIGHTGUN_CALIBRATION_CENTER_Y - calibData.m_yData[CALIB_C], 
                                  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_X - calibData.m_xData[CALIB_UL], 
                                  XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_Y - calibData.m_yData[CALIB_UL] );
    }
  }

  GetCalibratedCursorPosition( inputManager );
}

//---------------------------------------------------------------------
//  Draw
//---------------------------------------------------------------------
void CLightgunCalibrationScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(243,243,243),				// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

    // Render the backdrop texture
  RenderBackdrop();


  #define TEXTCOLOR                     D3DCOLOR_XRGB( 10,10,10 )
  #define CURSOR_COLOR                  D3DCOLOR_RGBA( 255, 100, 100, 255 )
  static WCHAR *calibrationStepText[NUM_CALIBRATIONSTEPS] = { L"Shoot the center target",
                                                              L"Shoot the upper left target", 
//                                                              L"Shoot the lower right corner",
                                                              L"Shoot anywhere to accept" };
  WCHAR wBuf[256];

  m_fontSet.DefaultFont().Begin();

    if( m_currentGunFlags & XINPUT_LIGHTGUN_ONSCREEN )
    {
      swprintf( wBuf, L"X: %d maps to %d", m_currentGunX, m_currentGunCalibratedX );
	    m_fontSet.DefaultFont().DrawText( 320, 80, TEXTCOLOR, wBuf, XBFONT_CENTER_X );

      swprintf( wBuf, L"Y: %d maps to %d", m_currentGunY, -m_currentGunCalibratedY );
	    m_fontSet.DefaultFont().DrawText( 320, 100, TEXTCOLOR, wBuf, XBFONT_CENTER_X );
    }

    swprintf( wBuf, L"Gun in port %d", m_currentInputDeviceIndex + 1 );
    m_fontSet.DefaultFont().DrawText( 320, 160, TEXTCOLOR, wBuf, XBFONT_CENTER_X );

    m_fontSet.DefaultFont().DrawText( 320, 180, TEXTCOLOR, calibrationStepText[m_calibrationStep], XBFONT_CENTER_X );

    m_fontSet.DefaultFont().DrawText( 320, 280, TEXTCOLOR, L"Press B to recalibrate.", XBFONT_CENTER_X );

  m_fontSet.DefaultFont().End();


    // Render the cursor
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	if( CheckResourceValidity( SPRITE_LIGHTGUNCALIBRATION_CURSOR ) )
	{
		m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );
		g_loadedSkin->SelectSkinTexture( m_displayDevice, SPRITE_LIGHTGUNCALIBRATION_CURSOR );
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_LIGHTGUNCALIBRATION_CURSOR );

			// Display a target/cursor if the gun is pointed at the screen
		if(  m_currentGunFlags & XINPUT_LIGHTGUN_ONSCREEN )
		{
			m_displayDevice->Begin( D3DPT_QUADLIST );


				switch( m_calibrationStep )
				{
				case 0:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X, TARGET_C_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X + TARGET_WIDTH, TARGET_C_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X + TARGET_WIDTH, TARGET_C_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_C_X, TARGET_C_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;

				case 1:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X, TARGET_UL_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X + TARGET_WIDTH, TARGET_UL_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X + TARGET_WIDTH, TARGET_UL_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_UL_X, TARGET_UL_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;

	/*
				case 2:
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X, TARGET_LR_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X + TARGET_WIDTH, TARGET_LR_Y, 1.0f, 1.0f );
	        
					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X + TARGET_WIDTH, TARGET_LR_Y + TARGET_HEIGHT, 1.0f, 1.0f );

					m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
					m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
					m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, TARGET_LR_X, TARGET_LR_Y + TARGET_HEIGHT, 1.0f, 1.0f );
					break;
	*/

				case (NUM_CALIBRATIONSTEPS - 1):
					{
						FLOAT x = m_currentGunCalibratedX;
						FLOAT y = -m_currentGunCalibratedY;  // Y values are negated for MAME

							// Map the cursor to screen coords
						x = ((x+128.0f) * 640.0f / 256.0f);
						y = ((y+128.0f) * 480.0f / 256.0f);
	          
						x -= (TARGET_WIDTH >> 2);
						y -= (TARGET_HEIGHT >> 2);

						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_top );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x, y, 1.0f, 1.0f );
	          
						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_top );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x + (TARGET_WIDTH>>1), y, 1.0f, 1.0f );
	          
						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_right, desc->m_bottom );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x + (TARGET_WIDTH>>1), y + (TARGET_HEIGHT>>1), 1.0f, 1.0f );

						m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, CURSOR_COLOR );
						m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, desc->m_left, desc->m_bottom );
						m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, x, y + (TARGET_HEIGHT>>1), 1.0f, 1.0f );
					}
					break;
				}
			m_displayDevice->End();
		}
	}

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}

#endif // 0 remove old MoveCursor and Draw methods

//---------------------------------------------------------------------
//  FindNextGun
//---------------------------------------------------------------------
BOOL CLightgunCalibrationScreen::FindNextGun( void )
{
    // Find the first attached lightgun
  for( ; m_currentInputDeviceIndex < 4; ++m_currentInputDeviceIndex )
  {
    const XINPUT_CAPABILITIES *gpCaps;
    if( (gpCaps = GetGamepadCaps( m_currentInputDeviceIndex )) && gpCaps->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
    {
      m_isFirstFrame = TRUE;
      return TRUE;
    }
  }

  return FALSE;
}

//---------------------------------------------------------------------
//  GetCalibratedCursorPosition
//---------------------------------------------------------------------
void CLightgunCalibrationScreen::GetCalibratedCursorPosition( CInputManager &inputManager )
{
  lightgunCalibration_t &calibData = g_calibrationData[m_currentInputDeviceIndex];

    // Don't bother if we're not pointing at the screen
  if( !inputManager.GetGamepad(m_currentInputDeviceIndex)->IsLightgunPointedAtScreen() )
  {
    m_currentGunCalibratedX = m_currentGunCalibratedY = 0;
    return;
  }

  m_currentGunCalibratedX = -1 * (int)((FLOAT)m_currentGunX * 128.0f / (FLOAT)XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_X );
  m_currentGunCalibratedY = (int)((FLOAT)m_currentGunY * 128.0f / (FLOAT)XINPUT_LIGHTGUN_CALIBRATION_UPPERLEFT_Y );

    // Lock to the expected range
  if( m_currentGunCalibratedX > 128 )
    m_currentGunCalibratedX = 128;
  else if( m_currentGunCalibratedX < -128 )
    m_currentGunCalibratedX = -128;

  if( m_currentGunCalibratedY > 128 )
    m_currentGunCalibratedY = 128;
  else if( m_currentGunCalibratedY < -128 )
    m_currentGunCalibratedY = -128;
}




