/**
	* \file			SkinScrollArea.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinScrollArea.h"
#include "Skin.h"
#include "DebugLogger.h"

//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================


//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinScrollArea::ParseINI(	CSystem_IniFile &iniFile, 
																const CStdString &sectionName, 
																const CStdString &entryName, 
																const CSkinElement *parent ) 
{
	if( parent )
	{
		m_left		= parent->m_left;
		m_top			= parent->m_top;
		m_right		= parent->m_right;
		m_bottom	= parent->m_bottom;
	}

	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, entryName + ".ScrollArea.Area", "" ) );
	if( rct.left != VALUE_INVALID && rct.top != VALUE_INVALID && rct.right != VALUE_INVALID && rct.bottom != VALUE_INVALID )
	{
		m_left = rct.left;
		m_top = rct.top;
		m_right = rct.right;
		m_bottom = rct.bottom;
	}

	m_singleRowHeight = iniFile.GetProfileInt( sectionName, entryName + ".ScrollArea.SingleRow.Height", 0 );


	CSkinHighlightBar hb;
	if( hb.ParseINI( iniFile, sectionName, entryName + ".ScrollArea" ) )
		m_highlightBar = new CSkinHighlightBar( hb );

	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID && m_right != VALUE_INVALID && m_bottom != VALUE_INVALID );
}


