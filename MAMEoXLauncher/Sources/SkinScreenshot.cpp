/**
	* \file			SkinScreenshot.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinScreenshot.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================


//---------------------------------------------------------------------
//	RenderAtRect
//---------------------------------------------------------------------
void CSkinScreenshot::RenderAtRect( LPDIRECT3DDEVICE8 displayDevice, 
																		const WCHAR *str, 
																		LPDIRECT3DTEXTURE8 screenshot, 
																		const RECT &screenshotTextureRect,
																		FLOAT left,
																		FLOAT top,
																		FLOAT right,
																		FLOAT bottom,
																		BOOL swapXY,
																		BOOL flipX,
																		BOOL flipY ) const
{

    // Display the screenshot
  displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    FALSE );

  FLOAT screenshotLeft = left;
  FLOAT screenshotRight = right;
  FLOAT screenshotTop = top;
  FLOAT screenshotBottom = bottom;

		// Flip the width and height
  if( swapXY )
	{
      // Set the height = to the standard width
    screenshotBottom = screenshotTop + (right-left);

      // Calculate left based on the new aspect ratio
    screenshotLeft = screenshotRight - ( (FLOAT)(right - left) * 3.0f / 4.0f);
	}

  if( screenshot )
  {
		displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
		displayDevice->SetTexture( 0, screenshot );
		displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    displayDevice->Begin( D3DPT_QUADLIST );      

        // Apply screenshot rotations
      POINT ul, ur, br, bl;
      if( swapXY )
      {
        ul.x = flipX ? screenshotTextureRect.left : screenshotTextureRect.right;
        ul.y = flipY ? screenshotTextureRect.top : screenshotTextureRect.bottom;

        ur.x = flipX ? screenshotTextureRect.left : screenshotTextureRect.right;
        ur.y = flipY ? screenshotTextureRect.bottom : screenshotTextureRect.top;

        br.x = flipX ? screenshotTextureRect.right : screenshotTextureRect.left;
        br.y = flipY ? screenshotTextureRect.bottom : screenshotTextureRect.top;

        bl.x = flipX ? screenshotTextureRect.right : screenshotTextureRect.left;
        bl.y = flipY ? screenshotTextureRect.top : screenshotTextureRect.bottom;
      }
      else
      {
        ul.x = flipX ? screenshotTextureRect.left : screenshotTextureRect.right;
        ul.y = flipY ? screenshotTextureRect.top : screenshotTextureRect.bottom;

        ur.x = flipX ? screenshotTextureRect.left : screenshotTextureRect.right;
        ur.y = flipY ? screenshotTextureRect.bottom : screenshotTextureRect.top;

        br.x = flipX ? screenshotTextureRect.right : screenshotTextureRect.left;
        br.y = flipY ? screenshotTextureRect.bottom : screenshotTextureRect.top;

        bl.x = flipX ? screenshotTextureRect.right : screenshotTextureRect.left;
        bl.y = flipY ? screenshotTextureRect.top : screenshotTextureRect.bottom;

        bl.x = ul.x = screenshotTextureRect.left;
        ur.y = ul.y = screenshotTextureRect.top;
        
        br.x = ur.x = screenshotTextureRect.right; 
        br.y = bl.y = screenshotTextureRect.bottom;

        if( flipX )
        {
          UINT32 temp = bl.x;
          bl.x = ul.x = ur.x;
          br.x = ur.x = temp;
        }

        if( flipY )
        {
          UINT32 temp = bl.y;
          bl.y = br.y = ul.y;
          ul.y = ur.y = temp;
        }
      }

			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[0] );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, screenshotTextureRect.left, screenshotTextureRect.top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotTop, 1.0f, 1.0f );
      
			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[1] );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, screenshotTextureRect.right, screenshotTextureRect.top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotTop, 1.0f, 1.0f );
      
			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[2] );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, screenshotTextureRect.right, screenshotTextureRect.bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotBottom, 1.0f, 1.0f );

			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[3] );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, screenshotTextureRect.left, screenshotTextureRect.bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotBottom, 1.0f, 1.0f );
    displayDevice->End();

  }
  else
  {
      // Render a rect where the screenshot would go
		displayDevice->SetTexture( 0, NULL );
		displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

		displayDevice->Begin( D3DPT_QUADLIST );      
			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[0] );
			displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_left, m_top, 1.0f, 1.0f );
	    
			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[1] );
			displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_right, m_top, 1.0f, 1.0f );
	    
			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[2] );
			displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_right, m_bottom, 1.0f, 1.0f );

			displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[3] );
			displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_left, m_bottom, 1.0f, 1.0f );
		displayDevice->End();
  }

		// Render the help text
	if( m_text )
	{
		if( !screenshot )
			m_text->RenderAsOffset( displayDevice, L"No Screenshot", left, top );
		else if( str )
			m_text->RenderAsOffset( displayDevice, str, left, top );
	}
}


//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinScreenshot::ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) 
{
	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Area", "" ) );
	if( rct.left != VALUE_INVALID && rct.top != VALUE_INVALID && rct.right != VALUE_INVALID && rct.bottom != VALUE_INVALID )
	{
		m_left = rct.left;
		m_top = rct.top;
		m_right = rct.right;
		m_bottom = rct.bottom;
	}

		// Parse the color, allowing either one color for all 4 corners, or individual colors
	CStdString temp = iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Color", "" );
	if( temp != "" )
	{
		m_cornerColors[0] = m_cornerColors[1] = m_cornerColors[2] = m_cornerColors[3] = StringToColor( temp );
	}
	else
	{
		temp = iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Color.UpperLeft", "" );
		m_cornerColors[0] = StringToColor( temp );

		temp = iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Color.UpperRight", "" );
		m_cornerColors[1] = StringToColor( temp );

		temp = iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Color.LowerRight", "" );
		m_cornerColors[2] = StringToColor( temp );

		temp = iniFile.GetProfileString( sectionName, entryName + ".Screenshot.Color.LowerLeft", "" );
		m_cornerColors[3] = StringToColor( temp );
	}

	CSkinText text;
	if( text.ParseINI( iniFile, sectionName, entryName + ".Screenshot" ) )
		m_text = new CSkinText( text );


	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID && m_right != VALUE_INVALID && m_bottom != VALUE_INVALID );
}


