/**
	* \file			SkinHightlightBar.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinHighlightBar.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================


//---------------------------------------------------------------------
//	RenderAsOffset
//---------------------------------------------------------------------
void CSkinHighlightBar::RenderAtRect( LPDIRECT3DDEVICE8 displayDevice, 
																			FLOAT left, 
																			FLOAT top, 
																			FLOAT right, 
																			FLOAT bottom ) const
{
  displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  displayDevice->SetTexture( 0, NULL );

  displayDevice->Begin( D3DPT_QUADLIST );
    displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[0] );
    displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, top, 1.0f, 1.0f );
    
    displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[1] );
    displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, top, 1.0f, 1.0f );
    
    displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[2] );
    displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, bottom, 1.0f, 1.0f );

    displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, m_cornerColors[3] );
    displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, bottom, 1.0f, 1.0f );
  displayDevice->End();
}	

//---------------------------------------------------------------------
//	RenderAsOffset
//---------------------------------------------------------------------
void CSkinHighlightBar::RenderAsOffset( LPDIRECT3DDEVICE8 displayDevice, FLOAT x, FLOAT y ) const
{
	x += m_left;
	y += m_top;

	RenderAtRect( displayDevice, x, y, x + GetWidth(), y + GetHeight() );
}


//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinHighlightBar::ParseINI(	CSystem_IniFile &iniFile, 
																	const CStdString &sectionName, 
																	const CStdString &parentEntryName, 
																	const CStdString &childEntryName ) 
{
	CStdString entryName = parentEntryName;

	if( entryName == "" )
		entryName = childEntryName;
	else
		entryName += "." + childEntryName;

	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, entryName + ".Area", "" ) );
	if( rct.left != VALUE_INVALID && rct.top != VALUE_INVALID && rct.right != VALUE_INVALID && rct.bottom != VALUE_INVALID )
	{
		m_left = rct.left;
		m_top = rct.top;
		m_right = rct.right;
		m_bottom = rct.bottom;
	}

		// Parse the color, allowing either one color for all 4 corners, or individual colors
	BOOL colorSetup = FALSE;
	CStdString temp = iniFile.GetProfileString( sectionName, entryName + ".Color", "" );
	if( temp != "" )
	{
		m_cornerColors[0] = m_cornerColors[1] = m_cornerColors[2] = m_cornerColors[3] = StringToColor( temp );
		colorSetup = TRUE;
	}
	else
	{
		temp = iniFile.GetProfileString( sectionName, entryName + ".Color.UpperLeft", "" );
		if( temp != "" )
			colorSetup = TRUE;
		m_cornerColors[0] = StringToColor( temp );


		temp = iniFile.GetProfileString( sectionName, entryName + ".Color.UpperRight", "" );
		if( temp != "" )
			colorSetup = TRUE;
		m_cornerColors[1] = StringToColor( temp );

		temp = iniFile.GetProfileString( sectionName, entryName + ".Color.LowerRight", "" );
		if( temp != "" )
			colorSetup = TRUE;
		m_cornerColors[2] = StringToColor( temp );

		temp = iniFile.GetProfileString( sectionName, entryName + ".Color.LowerLeft", "" );
		if( temp != "" )
			colorSetup = TRUE;
		m_cornerColors[3] = StringToColor( temp );
	}


	return colorSetup || (m_left != VALUE_INVALID && m_right != VALUE_INVALID && m_top != VALUE_INVALID && m_bottom != VALUE_INVALID );
}


