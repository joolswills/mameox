/**
	* \file			SkinMenuFooter.cpp
	* \brief		CSkinMenuFooter class implementation
	*
	*/


//= I N C L U D E S ====================================================
#include "SkinMenuFooter.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinMenuFooter::ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName ) 
{ 
	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, "Footer.Area", "" ) );
	m_left = rct.left;
	m_top = rct.top;
	m_right = rct.right;
	m_bottom = rct.bottom;

	return (m_left != VALUE_INVALID && m_right != VALUE_INVALID && m_top != VALUE_INVALID && m_bottom != VALUE_INVALID);
}

