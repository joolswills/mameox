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
	if( !CheckResourceTextureValidity( SPRITE_MENU_BODY_CENTER ) )
		return;

    //-- Draw the menu texture ------------------------------------------------------------
	m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
	m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	g_loadedSkin->SelectSkinTexture( m_displayDevice, SPRITE_MENU_BODY_CENTER );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );



  FLOAT centerLeft = m_titleArea.left;
  FLOAT centerRight = m_titleArea.right;
	if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_LEFTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_TITLEBAR_LEFTSIDE );
		centerLeft += desc->GetWidth();
		desc->Render( m_displayDevice, (FLOAT)m_titleArea.left, (FLOAT)m_titleArea.top, centerLeft, (FLOAT)m_titleArea.bottom );
	}

	if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_RIGHTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_TITLEBAR_RIGHTSIDE );
		centerRight -= desc->GetWidth();
		desc->Render( m_displayDevice, centerRight, (FLOAT)m_titleArea.top, (FLOAT)m_titleArea.right, (FLOAT)m_titleArea.bottom );
	}

	if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_CENTER ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_TITLEBAR_CENTER );
		desc->Render( m_displayDevice, centerLeft, (FLOAT)m_titleArea.top, centerRight, (FLOAT)m_titleArea.bottom );
	}

  centerLeft = m_bodyArea.left;
  centerRight = m_bodyArea.right;
	if( CheckResourceValidity( SPRITE_MENU_BODY_LEFTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_BODY_LEFTSIDE );
		centerLeft += desc->GetWidth();

		desc->Render( m_displayDevice, (FLOAT)m_bodyArea.left, (FLOAT)m_bodyArea.top, centerLeft, (FLOAT)m_bodyArea.bottom );
	}

	if( CheckResourceValidity( SPRITE_MENU_BODY_RIGHTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_BODY_RIGHTSIDE );
		centerRight -= desc->GetWidth();

		desc->Render( m_displayDevice, centerRight, (FLOAT)m_bodyArea.top, (FLOAT)m_bodyArea.right, (FLOAT)m_bodyArea.bottom );
	}

	if( CheckResourceValidity( SPRITE_MENU_BODY_CENTER ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_BODY_CENTER );
		desc->Render( m_displayDevice, centerLeft, (FLOAT)m_bodyArea.top, centerRight, (FLOAT)m_bodyArea.bottom );
	}

  centerLeft = m_bodyArea.left;
  centerRight = m_bodyArea.right;
	if( CheckResourceValidity( SPRITE_MENU_FOOTER_LEFTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_FOOTER_LEFTSIDE );
		centerLeft += desc->GetWidth();

		desc->Render( m_displayDevice, (FLOAT)m_bodyArea.left, (FLOAT)m_bodyArea.bottom, centerLeft, (FLOAT)m_bodyArea.bottom + desc->GetHeight() );
	}

	if( CheckResourceValidity( SPRITE_MENU_FOOTER_RIGHTSIDE ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_FOOTER_RIGHTSIDE );
		centerRight -= desc->GetWidth();

		desc->Render( m_displayDevice, centerRight, (FLOAT)m_bodyArea.bottom, (FLOAT)m_bodyArea.right, (FLOAT)m_bodyArea.bottom + desc->GetHeight() );
	}

	if( CheckResourceValidity( SPRITE_MENU_FOOTER_CENTER ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_FOOTER_CENTER );
		desc->Render( m_displayDevice, centerLeft, (FLOAT)m_bodyArea.bottom, centerRight, (FLOAT)m_bodyArea.bottom + desc->GetHeight() );
	}

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

