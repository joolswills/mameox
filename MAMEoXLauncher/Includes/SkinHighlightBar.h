/**
	* \file			SkinHightlightBar.h
	*/
#pragma once



//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "SkinElement.h"
#include "SkinSpriteResource.h"



#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================



//= C L A S S E S ====================================================

/**
 * \class		CSkinHighlightBar
 * \brief   Handles rendering highlight bars (translucent quads)
 */
class CSkinHighlightBar : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinHighlightBar( void ) : CSkinElement( SKINELEMENT_HIGHLIGHTBAR ) {
		m_cornerColors[0] = m_cornerColors[1] = m_cornerColors[2] = m_cornerColors[3] = D3DCOLOR_ARGB( 120, 0, 0, 0 );
	}

		//------------------------------------------------------
		//	Constructor (CSkinHighlightBar)
		//------------------------------------------------------
	CSkinHighlightBar( const CSkinHighlightBar &a ) : CSkinElement( a ) {
		memcpy( m_cornerColors, a.m_cornerColors, sizeof(m_cornerColors) );
	}

		//------------------------------------------------------
		//	RenderAtRect
		//------------------------------------------------------
	void RenderAtRect( LPDIRECT3DDEVICE8 displayDevice, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom ) const;

		//------------------------------------------------------
		//	RenderAsOffset
		//------------------------------------------------------
	void RenderAsOffset( LPDIRECT3DDEVICE8 displayDevice, FLOAT x, FLOAT y ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName ) { 
		return ParseINI( iniFile, sectionName, entryName, "HighlightBar" ); 
	}


	D3DCOLOR			m_cornerColors[4];	// The color of each corner, starting from upper-left, in clockwise order


protected:
		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	BOOL ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &parentEntryName, const CStdString &childEntryName );
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

