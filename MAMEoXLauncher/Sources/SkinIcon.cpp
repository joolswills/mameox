/**
	* \file			SkinIcon.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinIcon.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinIcon::Render( LPDIRECT3DDEVICE8 displayDevice ) const
{
	if( !CheckResourceValidity( m_spriteResourceID ) )
	{
		PRINTMSG(( T_ERROR, "Missing sprite resource: %d", m_spriteResourceID ));
		return;
	}


  displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
	displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	g_loadedSkin->SelectSkinTexture( displayDevice, m_spriteResourceID );

  displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );

	const CSkinSpriteResource *desc = g_loadedSkin->GetSkinSpriteResource( m_spriteResourceID );

	if( m_right >= 0 && m_bottom >= 0 )
		desc->Render( displayDevice, m_left, m_top, m_right, m_bottom );
	else
		desc->Render( displayDevice, m_left, m_top );
}	

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinIcon::ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) 
{
	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, entryName + ".Area", "" ) );
	if( rct.left != VALUE_INVALID && rct.top != VALUE_INVALID && rct.right != VALUE_INVALID && rct.bottom != VALUE_INVALID )
	{
		m_left = rct.left;
		m_top = rct.top;
		m_right = rct.right;
		m_bottom = rct.bottom;
	}

	POINT pt = StringToPoint( iniFile.GetProfileString( sectionName, entryName + ".Position", "" ) );
	if( pt.x != VALUE_INVALID && pt.y != VALUE_INVALID )
	{
		m_left = pt.x;
		m_top = pt.y;
		m_right = VALUE_AUTO;
		m_bottom = VALUE_AUTO;
	}


	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID );
}


