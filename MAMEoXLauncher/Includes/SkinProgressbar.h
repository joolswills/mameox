/**
	* \file			SkinProgressbar.h
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
 * \class		CSkinProgressbar
 */
class CSkinProgressbar : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinProgressbar( void ) : CSkinElement( SKINELEMENT_PROGRESSBAR ) {
		m_endColor = D3DCOLOR_ARGB( 0,0,0,0 );
		m_barColor = D3DCOLOR_ARGB( 0,0,0,0 );
		m_backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 );
	}

		//------------------------------------------------------
		//	Constructor (CSkinProgressbar)
		//------------------------------------------------------
	CSkinProgressbar( const CSkinProgressbar &a ) : CSkinElement( a ) {
		m_endColor = a.m_endColor;
		m_barColor = a.m_barColor;
		m_backgroundColor = a.m_backgroundColor;
	}


		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	virtual void Render( LPDIRECT3DDEVICE8 displayDevice ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName );


	D3DCOLOR		m_endColor;
	D3DCOLOR		m_barColor;
	D3DCOLOR		m_backgroundColor;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

