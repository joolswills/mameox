/**
	* \file			SkinDivider.h
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
 * \class		CSkinDivider
 * \brief   
 */
class CSkinDivider : public CSkinHighlightBar
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinDivider( void ) : CSkinHighlightBar() {
		m_type = SKINELEMENT_DIVIDER;
	}

		//------------------------------------------------------
		//	Constructor (CSkinDivider)
		//------------------------------------------------------
	CSkinDivider( const CSkinDivider &a ) : CSkinHighlightBar( a ) {}

		//------------------------------------------------------
		//	RenderAsOffset 
		//------------------------------------------------------
	void RenderAsOffset( LPDIRECT3DDEVICE8 displayDevice, FLOAT x, FLOAT y ) const {
		CSkinHighlightBar::RenderAsOffset( displayDevice, x, y );
	}

		//------------------------------------------------------
		//	Render
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice ) const {
		CSkinHighlightBar::RenderAtRect( displayDevice, m_left, m_top, m_right, m_bottom );
	}

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName ) { return ParseINI( iniFile, sectionName, "" ); }

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) { return CSkinHighlightBar::ParseINI( iniFile, sectionName, entryName, "Divider" ); }

};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

