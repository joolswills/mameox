/**
	* \file			SkinTriggerInfo.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinTriggerInfo.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinTriggerInfo::Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const
{
		// Determine the size of the text string
	FLOAT textWidth = 0;
	if( m_text )
		textWidth = m_text->GetTextWidth( str );

		// Set the body area
	FLOAT left = m_left;
	FLOAT right = m_right;
	FLOAT bottom = m_bottom;

		// Note that this will fail miserably if neither left nor right is set to a real val
	if( left == VALUE_AUTO || left == VALUE_UNBOUNDED )
		left = right - textWidth;
	else if( right == VALUE_AUTO || right == VALUE_UNBOUNDED )
		right = left + textWidth;

	if( bottom == VALUE_AUTO || bottom == VALUE_UNBOUNDED && CheckResourceValidity(SPRITE_LIST_TRIGGERICON_CENTER) )
		bottom = m_top + g_loadedSkin->GetSkinSpriteResource(SPRITE_LIST_TRIGGERICON_CENTER)->GetHeight();

		// Render the backdrop
	CSkin3PartBackdrop::Render( displayDevice, left, m_top, right, bottom );

		// Render the text
	if( m_text )
		m_text->RenderAsOffset( displayDevice, str, left, m_top );
}	

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinTriggerInfo::ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &parentName ) 
{
	CStdString entryName = parentName;
	if( entryName == "" )
		entryName = "TriggerInfo";
	else
		entryName += ".TriggerInfo";

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


	CSkinText temp;
	if( temp.ParseINI( iniFile, sectionName, entryName ) )
		m_text = new CSkinText( temp );

	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID );
}


