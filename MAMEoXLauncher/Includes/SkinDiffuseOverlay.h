/**
	* \file			SkinDiffuseOverlay.h
	*/
#pragma once



//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "SkinElement.h"
#include "SkinSpriteResource.h"
#include "SkinHighlightBar.h"



#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================



//= C L A S S E S ====================================================

/**
 * \class		CSkinDiffuseOverlay
 * \brief   Handles rendering the backdrop for popup menus
 */
class CSkinDiffuseOverlay : public CSkinHighlightBar
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinDiffuseOverlay( void ) : CSkinHighlightBar() {
		m_type = SKINELEMENT_DIFFUSEOVERLAY;
	}

		//------------------------------------------------------
		//	Constructor (CSkinDiffuseOverlay)
		//------------------------------------------------------
	CSkinDiffuseOverlay( const CSkinDiffuseOverlay &a ) : CSkinHighlightBar( a ) {}

		//------------------------------------------------------
		//	Render
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice ) const { RenderAsOffset( displayDevice, 0.0f, 0.0f ); }

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName ) { return ParseINI( iniFile, sectionName, "" ); }

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) { return CSkinHighlightBar::ParseINI( iniFile, sectionName, entryName, "DiffuseOverlay" ); }
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

