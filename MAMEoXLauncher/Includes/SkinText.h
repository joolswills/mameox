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

#include "SkinElement.h"
#include "FontSet.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================


//= C L A S S E S ====================================================


/**
 * \class		CSkinText
 * \brief   Renders a text box
 */
class CSkinText : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinText( void ) : CSkinElement( SKINELEMENT_TEXT )
	{
		m_color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
		m_fontType = FONTTYPE_DEFAULT;
		m_fontFlags = 0;
	}

		//------------------------------------------------------
		//	Constructor (CSkinIcon)
		//------------------------------------------------------
	CSkinText( const CSkinText &a ) : CSkinElement( a ) 
	{
		m_color = a.m_color;
		m_fontType = a.m_fontType;
		m_fontFlags = a.m_fontFlags;
	}


		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const;

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str, FLOAT x, FLOAT y ) const;

		//------------------------------------------------------
		//	RenderAtRect
		//------------------------------------------------------
	void RenderAtRect(	LPDIRECT3DDEVICE8 displayDevice, 
											const WCHAR *str, 
											FLOAT left, 
											FLOAT top, 
											FLOAT right, 
											FLOAT bottom, 
											UINT32 flags = -1 ) const;

		//------------------------------------------------------
		//	RenderAsOffset
		//------------------------------------------------------
	void RenderAsOffset( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str, FLOAT x, FLOAT y ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	BOOL ParseINI(	CSystem_IniFile &iniFile, 
									const CStdString &sectionName, 
									const CStdString &parentEntryName, 
									const CSkinElement *parentObject );

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	BOOL ParseINI(	CSystem_IniFile &iniFile, 
									const CStdString &sectionName, 
									const CStdString &parentEntryName ) { return ParseINI( iniFile, sectionName, parentEntryName, NULL ); }

		//------------------------------------------------------
		//	GetTextWidth
		//------------------------------------------------------
	UINT32 GetTextWidth( const WCHAR *str );


	D3DCOLOR			m_color;
	fonttype			m_fontType;
	UINT32				m_fontFlags;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

