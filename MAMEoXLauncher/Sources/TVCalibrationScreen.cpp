/**
	* \file			TVCalibrationScreen.cpp
	* \brief		Helper class which takes care of displaying
	*           the TV calibration routines
	*
	*/

//= I N C L U D E S ====================================================
#include "TVCalibrationScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_Direct3DRenderer.h"

#include "xbox_FileIO.h"		// for path info

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================


  //--- Layout defines -----------------------------------------
#define TEXT_COLOR			      D3DCOLOR_XRGB( 10,10,10 )
#define BACKDROP_COLOR        D3DCOLOR_XRGB(243,243,243)
#define ICON_COLOR            D3DCOLOR_XRGB( 0, 0, 0 )
#define SCREENOUTLINE_COLOR   D3DCOLOR_XRGB( 150, 150, 150 )

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	  0.075f
#define ANALOGCURSORMOVE_TIMEOUT	0.025f
 

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ===============================================


//= S T R U C T U R E S ===============================================

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	Reset
//---------------------------------------------------------------------
void CTVCalibrationScreen::Reset( void )
{
  m_screenSettingsChangedFlag = FALSE;
  m_currentStep = TVCS_SET_UPPERLEFT;

  m_dpadCursorDelay = 0.0f;

    // Calculate rect position from the usage/offset globals
  FLOAT xPos, yPos;
  FLOAT xPercentage, yPercentage;

  GetScreenPosition( &xPos, &yPos );
  GetScreenUsage( &xPercentage, &yPercentage );
  
  m_screenRect.left = (UINT32)(320.0 + (xPos*320.0) - (320.0*xPercentage));
  m_screenRect.right = (UINT32)(320.0 + (xPos*320.0) + (320.0*xPercentage));

  m_screenRect.top = (UINT32)(240.0 + (yPos*240.0) - (240.0*yPercentage));
  m_screenRect.bottom = (UINT32)(240.0 + (yPos*240.0) + (240.0*yPercentage));


  m_cursorPos.x = m_screenRect.left;
  m_cursorPos.y = m_screenRect.top;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CTVCalibrationScreen::MoveCursor( CInputManager &gp, BOOL useSpeedbanding )
{
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
    BYTE arrowKeys[4] = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };

		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN | GP_DPAD_LEFT | GP_DPAD_RIGHT | GP_LA_LEFT | GP_LA_RIGHT ) ||
          gp.IsOneOfKeysPressed( arrowKeys, 4 )) )
			m_dpadCursorDelay = 0.0f;
	}
  else
  {
    if( gp.IsOneOfButtonsPressed( GP_LA_LEFT | GP_DPAD_LEFT ) || gp.IsKeyPressed( VK_LEFT ) )
    {
      UINT32 bound = (m_currentStep == TVCS_SET_UPPERLEFT ? 0 : 320);
      if( m_cursorPos.x > bound )
        --m_cursorPos.x;

      if( gp.IsButtonPressed( GP_LA_LEFT ) )
        m_dpadCursorDelay = ANALOGCURSORMOVE_TIMEOUT;
      else
        m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    }

    if( gp.IsOneOfButtonsPressed( GP_LA_RIGHT | GP_DPAD_RIGHT ) || gp.IsKeyPressed( VK_RIGHT ) )
    {
      UINT32 bound = (m_currentStep == TVCS_SET_UPPERLEFT ? 320 : 640);
      if( m_cursorPos.x < bound )
        ++m_cursorPos.x;

      if( gp.IsButtonPressed( GP_LA_RIGHT ) )
        m_dpadCursorDelay = ANALOGCURSORMOVE_TIMEOUT;
      else
        m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    }

    if( gp.IsOneOfButtonsPressed( GP_LA_UP | GP_DPAD_UP ) || gp.IsKeyPressed( VK_UP ) )
    {
      UINT32 bound = (m_currentStep == TVCS_SET_UPPERLEFT ? 0 : 240);
      if( m_cursorPos.y > bound )
        --m_cursorPos.y;

      if( gp.IsButtonPressed( GP_LA_UP )  )
        m_dpadCursorDelay = ANALOGCURSORMOVE_TIMEOUT;
      else
        m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    }

    if( gp.IsOneOfButtonsPressed( GP_LA_DOWN | GP_DPAD_DOWN ) || gp.IsKeyPressed( VK_DOWN ) )
    {
      UINT32 bound = (m_currentStep == TVCS_SET_UPPERLEFT ? 240 : 480);
      if( m_cursorPos.y < bound )
        ++m_cursorPos.y;

      if( gp.IsButtonPressed( GP_LA_DOWN ) )
        m_dpadCursorDelay = ANALOGCURSORMOVE_TIMEOUT;
      else
        m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    }
  }

    // Reset to the default values
  if( gp.IsButtonPressed( GP_Y ) || gp.IsKeyPressed( VK_TAB ) )
  {
    m_screenRect.left = (UINT32)(320 - (320*DEFAULT_SCREEN_X_PERCENTAGE));
    m_screenRect.right = (UINT32)(320 + (320*DEFAULT_SCREEN_X_PERCENTAGE));

    m_screenRect.top = (UINT32)(240 - (240*DEFAULT_SCREEN_Y_PERCENTAGE));
    m_screenRect.bottom = (UINT32)(240 + (240*DEFAULT_SCREEN_Y_PERCENTAGE));

    m_cursorPos.x = m_screenRect.left;
    m_cursorPos.y = m_screenRect.top;

    m_currentStep = TVCS_SET_UPPERLEFT;
    gp.WaitForNoButton();
  }

  if( gp.IsButtonPressed( GP_A ) || gp.IsKeyPressed( VK_RETURN ) )
  {
    switch( m_currentStep )
    {
    default:
      // *** TVCS_COMPLETED *** //
    case TVCS_COMPLETED:
      break;

      // *** TVCS_SET_UPPERLEFT *** //
    case TVCS_SET_UPPERLEFT:
      m_currentStep = TVCS_SET_LOWERRIGHT;
      m_screenRect.left = m_cursorPos.x;
      m_screenRect.top = m_cursorPos.y;
      m_cursorPos.x = m_screenRect.right - 32;
      m_cursorPos.y = m_screenRect.bottom - 32;
      break;

      // *** TVCS_SET_LOWERRIGHT *** //
    case TVCS_SET_LOWERRIGHT:
      {
        m_currentStep = TVCS_COMPLETED;
        m_screenRect.right = m_cursorPos.x + 32;
        m_screenRect.bottom = m_cursorPos.y + 32;
        m_screenSettingsChangedFlag = TRUE;

          // Calculate the screen usage + position
        FLOAT xPos, yPos;
        FLOAT xPercentage, yPercentage;

        xPos = -(320.0f - ( m_screenRect.left + ((m_screenRect.right - m_screenRect.left) / 2.0f) )) / 320.0f;
        yPos = (240.0f - ( m_screenRect.top + ((m_screenRect.bottom - m_screenRect.top) / 2.0f)  )) / 320.0f;

        xPercentage = (FLOAT)(m_screenRect.right - m_screenRect.left) / 640.0f;
        yPercentage = (FLOAT)(m_screenRect.bottom - m_screenRect.top) / 480.0f;

        SetScreenPosition( xPos, yPos );
        SetScreenUsage( xPercentage, yPercentage );  
      }
      break;
    }

    gp.WaitForNoButton();
  }

    // Back out without storing changes
  if( gp.IsOneOfButtonsPressed( GP_B | GP_BACK ) || gp.IsKeyPressed( VK_DELETE ) )
  {
    m_currentStep = TVCS_COMPLETED;
    gp.WaitForNoButton();
  }
}



//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CTVCalibrationScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									BACKDROP_COLOR,							      // Color
					  								1.0f,															// Z
						  							0L );															// Stencil

  RenderBackdrop();


    //-- Render a box for the screenspace -------------------------------------
  if( m_currentStep == TVCS_SET_LOWERRIGHT )
  {
    m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCREENOUTLINE_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_screenRect.left, m_screenRect.top, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCREENOUTLINE_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x + 32.0f,  m_screenRect.top, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCREENOUTLINE_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x + 32.0f,  m_cursorPos.y + 32.0f, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCREENOUTLINE_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_screenRect.left,  m_cursorPos.y + 32.0f, 1.0f, 1.0f );
    m_displayDevice->End();
  }

    //-- Render the cursor ----------------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );
  m_displayDevice->SetTexture( 0, m_textureSet.GetTVCalibrationCursorMask() );

  FLOAT tu_l, tv_t, tu_r, tv_b;
  switch( m_currentStep )
  {
  case TVCS_SET_UPPERLEFT:
    tu_l = 0.0f;
    tv_t = 0.0f;
    tu_r = 1.0f;
    tv_b = 1.0f;
    break;

  case TVCS_SET_LOWERRIGHT:
    tu_l = 1.0f;
    tv_t = 1.0f;
    tu_r = 0.0f;
    tv_b = 0.0f;
    break;
  }

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, ICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, tu_l, tv_t );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x, m_cursorPos.y, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, ICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, tu_r, tv_t );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x + 32.0f,  m_cursorPos.y, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, ICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, tu_r, tv_b );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x + 32.0f,  m_cursorPos.y + 32.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, ICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, tu_l, tv_b );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_cursorPos.x,  m_cursorPos.y + 32.0f, 1.0f, 1.0f );
  m_displayDevice->End();

  m_displayDevice->SetTexture( 0, NULL );



    //-- Render the page's text -----------------------------------------------------------
	m_fontSet.DefaultFont().Begin();

    switch( m_currentStep )
    {
    case TVCS_SET_UPPERLEFT:
      m_fontSet.DefaultFont().DrawText( 320,
                                        200,
                                        TEXT_COLOR,
                                        L"Use the left analog stick to set\n the upper left corner of the screen.",
                                        XBFONT_CENTER_X );
      break;

    case TVCS_SET_LOWERRIGHT:
      m_fontSet.DefaultFont().DrawText( 320,
                                        200,
                                        TEXT_COLOR,
                                        L"Use the left analog stick to set\n the lower right corner of the screen.",
                                        XBFONT_CENTER_X );
      break;
    }

    m_fontSet.DefaultFont().DrawText( 320,
                                      270,
                                      TEXT_COLOR,
                                      L"Button Functions:\n   A: Accept\n   B: Cancel\n   Y: Reset to default values",
                                      XBFONT_CENTER_X );


	m_fontSet.DefaultFont().End();



    // Clean up settings
  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}
