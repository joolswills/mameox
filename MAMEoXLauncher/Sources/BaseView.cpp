/**
	* \file			StartMenu.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "BaseView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================


//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	DrawToTexture
//---------------------------------------------------------------------
void CBaseView::DrawToTexture( LPDIRECT3DTEXTURE8 targetTexture )
{
  if( !targetTexture || !RenderToTextureStart( targetTexture ) )
    return;

  Draw( TRUE, FALSE );

  RenderToTextureStop();
}

//---------------------------------------------------------------------
//	RenderBackdrop
//---------------------------------------------------------------------
void CBaseView::RenderBackdrop( void )
{
  if( !m_backdropTexture )
    return;

    // Render the highlight bar for the selected item
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_displayDevice->SetTexture( 0, m_backdropTexture );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
  m_displayDevice->Begin( D3DPT_QUADLIST );

                    // Write the diffuse color
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
  m_displayDevice->End();
	m_displayDevice->SetTexture( 0, NULL );
}



//---------------------------------------------------------------------
//	RenderToTextureStart
//---------------------------------------------------------------------
BOOL CBaseView::RenderToTextureStart( LPDIRECT3DTEXTURE8 targetTexture )
{
  D3DVIEWPORT8 vp = { 0, 0, 640, 480, 0.0f, 1.0f };
  return ::RenderToTextureStart( m_renderToTextureToken, m_displayDevice, targetTexture, vp );
}

//---------------------------------------------------------------------
//	RenderToTextureStop
//---------------------------------------------------------------------
void CBaseView::RenderToTextureStop( void )
{
  ::RenderToTextureStop( m_renderToTextureToken );
}