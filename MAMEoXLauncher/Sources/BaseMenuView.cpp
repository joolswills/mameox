/**
	* \file			BaseMenuView.cpp
	* \brief		Resizable "Menu" drawable
	*
	*/

//= I N C L U D E S ====================================================
#include "BaseMenuView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================
 
//---------------------------------------------------------------------
//  RenderBackdrop
//---------------------------------------------------------------------
void CBaseMenuView::RenderBackdrop( void )
{
    //--- Darken the screen under the menu ----------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  m_displayDevice->SetTexture( 0, NULL );

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


  m_displayDevice->Begin( D3DPT_QUADLIST );

    FLOAT centerLeft = m_titleArea.left + m_textureSet.GetMenuTitleBarLeftWidth();
    FLOAT centerRight = m_titleArea.right - m_textureSet.GetMenuTitleBarRightWidth();

      // Draw the titlebar left
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeftLeft(), m_textureSet.GetMenuTitleBarLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_titleArea.left, m_titleArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeftRight(), m_textureSet.GetMenuTitleBarLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_titleArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeftRight(), m_textureSet.GetMenuTitleBarLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_titleArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeftLeft(), m_textureSet.GetMenuTitleBarLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_titleArea.left, m_titleArea.bottom, 1.0f, 1.0f );

      // Draw the titlebar center
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarCenterLeft(), m_textureSet.GetMenuTitleBarCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_titleArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarCenterLeft(), m_textureSet.GetMenuTitleBarCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_titleArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarCenterRight(), m_textureSet.GetMenuTitleBarCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_titleArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarCenterLeft(), m_textureSet.GetMenuTitleBarCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_titleArea.bottom, 1.0f, 1.0f );

      // Draw the titlebar right
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRightLeft(), m_textureSet.GetMenuTitleBarRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_titleArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRightRight(), m_textureSet.GetMenuTitleBarRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_titleArea.right, m_titleArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRightRight(), m_textureSet.GetMenuTitleBarRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_titleArea.right, m_titleArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRightLeft(), m_textureSet.GetMenuTitleBarRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_titleArea.bottom, 1.0f, 1.0f );
    


      // Draw the body left
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeftLeft(), m_textureSet.GetMenuBodyLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.left, m_bodyArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeftRight(), m_textureSet.GetMenuBodyLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_bodyArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeftRight(), m_textureSet.GetMenuBodyLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_bodyArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeftLeft(), m_textureSet.GetMenuBodyLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.left, m_bodyArea.bottom, 1.0f, 1.0f );

      // Draw the body center
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyCenterLeft(), m_textureSet.GetMenuBodyCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_bodyArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyCenterLeft(), m_textureSet.GetMenuBodyCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_bodyArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyCenterRight(), m_textureSet.GetMenuBodyCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_bodyArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyCenterLeft(), m_textureSet.GetMenuBodyCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, m_bodyArea.bottom, 1.0f, 1.0f );

      // Draw the body right
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRightLeft(), m_textureSet.GetMenuBodyRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_bodyArea.top, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRightRight(), m_textureSet.GetMenuBodyRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.right, m_bodyArea.top, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRightRight(), m_textureSet.GetMenuBodyRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.right, m_bodyArea.bottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRightLeft(), m_textureSet.GetMenuBodyRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, m_bodyArea.bottom, 1.0f, 1.0f );
    


      // Draw the footer left
    FLOAT footerTop = m_bodyArea.bottom;
    FLOAT footerBottom = m_bodyArea.bottom + m_textureSet.GetMenuFooterHeight();

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterLeftLeft(), m_textureSet.GetMenuFooterLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.left, footerTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterLeftRight(), m_textureSet.GetMenuFooterLeftTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, footerTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterLeftRight(), m_textureSet.GetMenuFooterLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, footerBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterLeftLeft(), m_textureSet.GetMenuFooterLeftBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.left, footerBottom, 1.0f, 1.0f );

      // Draw the footer center
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterCenterLeft(), m_textureSet.GetMenuFooterCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, footerTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterCenterLeft(), m_textureSet.GetMenuFooterCenterTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, footerTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterCenterRight(), m_textureSet.GetMenuFooterCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, footerBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterCenterLeft(), m_textureSet.GetMenuFooterCenterBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerLeft, footerBottom, 1.0f, 1.0f );

      // Draw the footer right
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterRightLeft(), m_textureSet.GetMenuFooterRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, footerTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterRightRight(), m_textureSet.GetMenuFooterRightTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.right, footerTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterRightRight(), m_textureSet.GetMenuFooterRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_bodyArea.right, footerBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuFooterRightLeft(), m_textureSet.GetMenuFooterRightBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, centerRight, footerBottom, 1.0f, 1.0f );

  m_displayDevice->End();
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
}

