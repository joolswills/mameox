/**
	* \file			SkinButtonInfo.h
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "SkinSpriteResource.h"
#include "SkinText.h"
#include "SkinIcon.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================

//= C L A S S E S ====================================================


/**
 * \class		CSkinButtonInfo
 * \brief   Renders a button info icon/text pair
 */
class CSkinButtonInfo : public CSkinIcon
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinButtonInfo( SkinResourceID_t spriteResourceID ) : CSkinIcon( spriteResourceID ) {
		m_type = SKINELEMENT_BUTTONINFO;
		m_text = NULL;
	}

		//------------------------------------------------------
		//	Constructor (CSkinButtonInfo)
		//------------------------------------------------------
	CSkinButtonInfo( const CSkinButtonInfo &a ) : CSkinIcon( a ) {
		if( a.m_text )
			m_text = new CSkinText( *a.m_text );
		else
			m_text = NULL;
	}

		//------------------------------------------------------
		//	Destructor
		//------------------------------------------------------
	~CSkinButtonInfo( void ) {
		if( m_text )
			delete m_text;
	}

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName );


	CSkinText						*m_text;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

