/**
	* \file			SkinIcon.h
	* \brief		CSkinIcon helper class 
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "SkinSpriteResource.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================

//= C L A S S E S ====================================================


/**
 * \class		CSkinIcon
 * \brief   Renders a single icon
 */
class CSkinIcon : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinIcon( SkinResourceID_t spriteResourceID ) : 
			CSkinElement( SKINELEMENT_SPRITE ),
			m_spriteResourceID( spriteResourceID )
	{
	}

		//------------------------------------------------------
		//	Constructor (CSkinIcon)
		//------------------------------------------------------
	CSkinIcon( const CSkinIcon &a ) : CSkinElement( a ) {
		m_spriteResourceID = a.m_spriteResourceID;
	}


		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	virtual void Render( LPDIRECT3DDEVICE8 displayDevice ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName );


	SkinResourceID_t		m_spriteResourceID;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

