/**
	* \file			SkinElement.h
	* \brief		CSkinElement base class for skin objects
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "System_IniFile.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================
typedef enum SkinElementType_t
{
	SKINELEMENT_BASE,
	SKINELEMENT_SPRITE,				// Sprite resource definition
	SKINELEMENT_MENU_FOOTER,
	SKINELEMENT_MENU_BODY,
	SKINELEMENT_MENU_HEADER,
	SKINELEMENT_HIGHLIGHTBAR,
	SKINELEMENT_TEXT,
	SKINELEMENT_SCROLLAREA,
	SKINELEMENT_BUTTONINFO,
	SKINELEMENT_SCREENSHOT,
	SKINELEMENT_DIFFUSEOVERLAY,
	SKINELEMENT_DIVIDER,
	SKINELEMENT_TRIGGERINFO,
	SKINELEMENT_PROGRESSBAR
} SkinElementType_t;


	// Define for invalid right/bottom entries
#define VALUE_INVALID			-1.0f
#define VALUE_UNBOUNDED		-2.0f
#define VALUE_AUTO				-3.0f

//= C L A S S E S ====================================================

/**
 * \class  CSkinElement
 * \brief  Base class for all skin elements
 */
class CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinElement( void ) : 
		m_left( VALUE_INVALID ), 
		m_top( VALUE_INVALID ), 
		m_right( VALUE_INVALID ), 
		m_bottom( VALUE_INVALID ),
		m_type( SKINELEMENT_BASE )
	{}

		//------------------------------------------------------
		//	Constructor (SkinElementType_t)
		//------------------------------------------------------
	CSkinElement( SkinElementType_t type ) : 
		m_left( VALUE_INVALID ), 
		m_top( VALUE_INVALID ), 
		m_right( VALUE_INVALID ), 
		m_bottom( VALUE_INVALID ),
		m_type( type )
	{}

		//------------------------------------------------------
		//	Constructor (CSkinElement)
		//------------------------------------------------------
	CSkinElement( const CSkinElement &a ) {	
		m_left = a.m_left;
		m_top = a.m_top;
		m_right = a.m_right;
		m_bottom = a.m_bottom;
		m_type = a.m_type;
	}

	inline FLOAT GetWidth( void ) const { return m_right - m_left; }
	inline FLOAT GetHeight( void ) const { return m_bottom - m_top; }

		//------------------------------------------------------
		//------------------------------------------------------
	virtual void Render( LPDIRECT3DDEVICE8 displayDevice ) const {}

		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName ) { return FALSE; }

		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI(	CSystem_IniFile &iniFile, 
													const CStdString &sectionName, 
													const CStdString &entryName ) { return FALSE; }



		//------------------------------------------------------
		//------------------------------------------------------
	D3DCOLOR StringToColor( const CStdString &argbString );

		//------------------------------------------------------
		//------------------------------------------------------
	RECT StringToRect( const CStdString &rectString );

		//------------------------------------------------------
		//------------------------------------------------------
	POINT StringToPoint( const CStdString &pointString );


		// The element's actual type
	SkinElementType_t			m_type;

	FLOAT									m_left;
	FLOAT									m_top;
	FLOAT									m_right;
	FLOAT									m_bottom;
};



//= G L O B A L = V A R S =============================================
extern const char		*g_skinDelimiters;


//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

