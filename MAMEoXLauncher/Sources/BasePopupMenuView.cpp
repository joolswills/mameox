/**
	* \file			BasePopupView.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "BasePopupMenuView.h"
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
void CBasePopupView::RenderBackdrop( FLOAT fontHeight )
{
	if( !g_loadedSkin )
		return;

    //--- Darken the screen under the menu ----------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  m_displayDevice->SetTexture( 0, NULL );

	D3DCOLOR overlayColor = g_loadedSkin->GetSkinColor( COLOR_POPUP_DARKENOVERLAY );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, overlayColor );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, overlayColor );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, overlayColor );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, overlayColor );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
  m_displayDevice->End();  

    // Use the fontHeight to calculate the correct area for the backdrop
	UINT32 halfheight = fontHeight * GetNumBodyLines();
	if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_CENTER ) )
		halfheight += (UINT32)g_loadedSkin->GetSkinResourceInfo( SPRITE_MENU_TITLEBAR_CENTER )->GetHeight();
	
	if( CheckResourceValidity( SPRITE_MENU_FOOTER_CENTER ) )
		halfheight += (UINT32)g_loadedSkin->GetSkinResourceInfo( SPRITE_MENU_FOOTER_CENTER )->GetHeight();
	halfheight >>= 1;
	
  RECT area = { m_titleArea.left, 240 - halfheight,
                m_titleArea.right, 240 + halfheight };
  SetRect( area );  
  CBaseMenuView::RenderBackdrop();
}

