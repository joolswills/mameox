/**
	* \file			SkinProgressbar.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinProgressbar.h"
#include "Skin.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================




//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinProgressbar::Render( LPDIRECT3DDEVICE8 displayDevice ) const
{
}	

//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinProgressbar::ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &parentEntryName ) 
{
	CStdString entryName = parentEntryName;
	if( entryName == "" )
		entryName = "ProgressBar";
	else
		entryName += ".ProgressBar";

	m_endColor = StringToColor( iniFile.GetProfileString( sectionName, entryName + ".EndColor", "" ) );
	m_barColor = StringToColor( iniFile.GetProfileString( sectionName, entryName + ".BarColor", "" ) );
	m_backgroundColor = StringToColor( iniFile.GetProfileString( sectionName, entryName + ".BackgroundColor", "" ) );

	return TRUE;
}


