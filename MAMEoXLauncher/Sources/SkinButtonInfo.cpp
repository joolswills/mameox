/**
	* \file			SkinButtonInfo.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinButtonInfo.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinButtonInfo::Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const
{
	CSkinIcon::Render( displayDevice );

		// Render the help text
	if( m_text )
		m_text->Render( displayDevice, str );
}	

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinButtonInfo::ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) 
{
	if( !CSkinIcon::ParseINI( iniFile, sectionName, entryName ) )
		return FALSE;


	CSkinText temp;
	if( temp.ParseINI( iniFile, sectionName, entryName ) )
		m_text = new CSkinText( temp );


	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID );
}


