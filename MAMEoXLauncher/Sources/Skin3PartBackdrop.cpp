/**
	* \file			Skin3PartBackdrop.cpp
	* \brief		CSkin3PartBackdrop class implementation
	*/


//= I N C L U D E S ====================================================
#include "Skin3PartBackdrop.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkin3PartBackdrop::Render( LPDIRECT3DDEVICE8 displayDevice, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom ) const
{
	if( !CheckResourceTextureValidity( m_spriteResourceIDs[1] ) )
		return;


	displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
	displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );

	g_loadedSkin->SelectSkinTexture( displayDevice, m_spriteResourceIDs[1] );



	FLOAT centerLeft = left;
	FLOAT centerRight = right;
	if( CheckResourceValidity( m_spriteResourceIDs[0] ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( m_spriteResourceIDs[0] );
		centerLeft += desc->GetWidth();

		FLOAT l = m_flipHorizontal ? centerLeft : left;
		FLOAT r = m_flipHorizontal ? left : centerLeft;
		FLOAT t = m_flipVertical ? bottom : top;
		FLOAT b = m_flipVertical ? top : bottom;
		desc->Render( displayDevice, l, t, r, b );
	}

	if( CheckResourceValidity( m_spriteResourceIDs[2] ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( m_spriteResourceIDs[2] );
		centerRight -= desc->GetWidth();

		FLOAT l = m_flipHorizontal ? right : centerRight;
		FLOAT r = m_flipHorizontal ? centerRight : right;
		FLOAT t = m_flipVertical ? bottom : top;
		FLOAT b = m_flipVertical ? top : bottom;
		desc->Render( displayDevice, l, t, r, b );
	}

	if( CheckResourceValidity( m_spriteResourceIDs[1] ) )
	{
		const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( m_spriteResourceIDs[1] );

		FLOAT l = m_flipHorizontal ? centerRight : centerLeft;
		FLOAT r = m_flipHorizontal ? centerLeft : centerRight;
		FLOAT t = m_flipVertical ? bottom : top;
		FLOAT b = m_flipVertical ? top : bottom;
		desc->Render( displayDevice, l, t, r, b );
	}
}	
