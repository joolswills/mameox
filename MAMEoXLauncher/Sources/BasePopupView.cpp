/**
	* \file			BasePopupView.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "BasePopupView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================
  //--- Layout defines -----------------------------------------
#define DARKENBACKGROUND_COLOR  D3DCOLOR_ARGB( 120, 0, 0, 0 )

#define TITLEBAR_ROW            menuTop + 5
#define FIRSTDATA_ROW           bodyTop + 3

#define HIGHLIGHTBAR_LEFT       menuLeft + 4
#define HIGHLIGHTBAR_RIGHT      menuLeft + menuWidth - 4
#define NAME_START              menuLeft + 9


//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================
 
//---------------------------------------------------------------------
//  RenderBackdrop
//---------------------------------------------------------------------
void CBasePopupView::RenderBackdrop( FLOAT fontHeight )
{
    //--- Darken the screen under the menu ----------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  m_displayDevice->SetTexture( 0, NULL );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
  m_displayDevice->End();
  

    //-- Draw the actual menu texture ------------------------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );

  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

  m_displayDevice->SetTexture( 0, m_textureSet.GetMenuParts() );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );


  FLOAT menuLeft    = 320.0f - (m_textureSet.GetMenuTitleWidth() / 2.0f);
  FLOAT menuTop     = 240.0f - (( m_textureSet.GetMenuTitleHeight() + m_textureSet.GetMenuBottomHeight() + (GetNumBodyLines() * fontHeight)) / 2.0f);
  FLOAT menuWidth   = m_textureSet.GetMenuTitleWidth();

  FLOAT bodyTop     = menuTop + m_textureSet.GetMenuTitleHeight();
  FLOAT bodyBottom  = bodyTop + (GetNumBodyLines() * fontHeight);

  m_titleArea.left    = HIGHLIGHTBAR_LEFT;
  m_titleArea.top     = TITLEBAR_ROW;
  m_titleArea.right   = HIGHLIGHTBAR_RIGHT;
  m_titleArea.bottom  = bodyTop;

  m_bodyArea.top      = FIRSTDATA_ROW;
  m_bodyArea.left     = HIGHLIGHTBAR_LEFT;
  m_bodyArea.bottom   = bodyBottom;
  m_bodyArea.right    = HIGHLIGHTBAR_RIGHT;


  m_displayDevice->Begin( D3DPT_QUADLIST );
      // 1) Draw the "top"
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeft(), m_textureSet.GetMenuTitleBarTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, menuTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRight(), m_textureSet.GetMenuTitleBarTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, menuTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRight(), m_textureSet.GetMenuTitleBarBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeft(), m_textureSet.GetMenuTitleBarBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyTop, 1.0f, 1.0f );

      // 2) Draw the body, stretching to fit the desired height
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeft(), m_textureSet.GetMenuBodyTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRight(), m_textureSet.GetMenuBodyTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRight(), m_textureSet.GetMenuBodyBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeft(), m_textureSet.GetMenuBodyBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom, 1.0f, 1.0f );

      // 3) Draw the tail
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomLeft(), m_textureSet.GetMenuBottomTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomRight(), m_textureSet.GetMenuBottomTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomRight(), m_textureSet.GetMenuBottomBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom + m_textureSet.GetMenuBottomHeight(), 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomLeft(), m_textureSet.GetMenuBottomBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom + m_textureSet.GetMenuBottomHeight(), 1.0f, 1.0f );

  m_displayDevice->End();
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     FALSE );
}

