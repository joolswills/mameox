/**
	* \file			SkinMenuHeader.h
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "SkinElement.h"
#include "Skin3PartBackdrop.h"
#include "SkinSpriteResource.h"
#include "SkinText.h"



#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================



//= C L A S S E S ====================================================

/**
 * \class		CSkinMenuHeader
 * \brief   Holds positioning info for a menu header
 */
class CSkinTriggerInfo : public CSkin3PartBackdrop
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinTriggerInfo( BOOL reversed = FALSE ) : CSkin3PartBackdrop( SKINELEMENT_TRIGGERINFO,
																								reversed ? SPRITE_LIST_TRIGGERICON_TAIL : SPRITE_LIST_TRIGGERICON_HEAD, 
																								SPRITE_LIST_TRIGGERICON_CENTER, 
																								reversed ? SPRITE_LIST_TRIGGERICON_HEAD : SPRITE_LIST_TRIGGERICON_TAIL,
																								reversed ) 
	{
		m_text = NULL;
	}

		//------------------------------------------------------
		//	Constructor (CSkinTriggerInfo)
		//------------------------------------------------------
	CSkinTriggerInfo( const CSkinTriggerInfo &a ) : CSkin3PartBackdrop( a ) {
		if( a.m_text )
			m_text = new CSkinText( *a.m_text );
		else
			m_text = NULL;
	}

		//------------------------------------------------------
		//	Destructor
		//------------------------------------------------------
	~CSkinTriggerInfo( void ) {
		if( m_text )
			delete m_text;
	}

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const;

		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &parentName ) ;



	CSkinText						*m_text;
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

