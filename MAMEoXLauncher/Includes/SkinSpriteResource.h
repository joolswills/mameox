/**
	* \file			SkinSpriteResource.h
	* \brief		CSkinSpriteResource helper class, holds information
	*           about a sprite resource and provides rendering functions.
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "PNGFunctions.h"

#include "SkinElement.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================
	// Enum containing all of the graphics Resources (sprite/screen images)
typedef enum SkinResourceID_t {
	ASSET_SPLASH_BACKDROP,

	ASSET_MESSAGE_BACKDROP,

	ASSET_LIST_BACKDROP,

	SPRITE_TVCALIBRATION_CURSOR,

	SPRITE_LIGHTGUNCALIBRATION_CURSOR,
	SPRITE_LIGHTGUNCALIBRATION_TARGET,

	SPRITE_LIST_SCROLLICON_DOWN,
	SPRITE_LIST_SCROLLICON_UP,
	SPRITE_LIST_TRIGGERICON_HEAD,
	SPRITE_LIST_TRIGGERICON_CENTER,
	SPRITE_LIST_TRIGGERICON_TAIL,

	SPRITE_MENU_TITLEBAR_LEFTSIDE,
	SPRITE_MENU_TITLEBAR_CENTER,
	SPRITE_MENU_TITLEBAR_RIGHTSIDE,
	SPRITE_MENU_BODY_LEFTSIDE,
	SPRITE_MENU_BODY_CENTER,
	SPRITE_MENU_BODY_RIGHTSIDE,
	SPRITE_MENU_FOOTER_LEFTSIDE,
	SPRITE_MENU_FOOTER_CENTER,
	SPRITE_MENU_FOOTER_RIGHTSIDE,

	SPRITE_BUTTON_A,
	SPRITE_BUTTON_B,
	SPRITE_BUTTON_X,
	SPRITE_BUTTON_Y,
	SPRITE_BUTTON_BLACK,
	SPRITE_BUTTON_WHITE,
	SPRITE_BUTTON_START,
	SPRITE_BUTTON_BACK,
	SPRITE_BUTTON_LEFTTRIGGER,
	SPRITE_BUTTON_RIGHTTRIGGER,
	SPRITE_BUTTON_LEFTANALOG,
	SPRITE_BUTTON_RIGHTANALOG,

	SKINRESOURCE_COUNT,		// Note: This must be the last "real" member of the enum
	SKINRESOURCE_INVALID = 0x7FFFFFFF
} SkinResourceID_t;


//= C L A S S E S ====================================================

/**
 * \class		CSkinSpriteResource
 * \brief   Holds positioning info for a sprite and has rendering
 *          helper functions.
 */
class CSkinSpriteResource : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinSpriteResource( void ) : CSkinElement( SKINELEMENT_SPRITE )	{}

		//------------------------------------------------------
		//	Constructor (CSkinSpriteResource)
		//------------------------------------------------------
	CSkinSpriteResource( const CSkinSpriteResource &a ) : CSkinElement( a ) {}


		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	inline void Render( LPDIRECT3DDEVICE8 displayDevice, 
											FLOAT left, 
											FLOAT top ) const
	{
		Render( displayDevice, left, top, left + GetWidth(), top + GetHeight() );
	}

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	inline void Render( LPDIRECT3DDEVICE8 displayDevice, 
											FLOAT left, 
											FLOAT top, 
											FLOAT right, 
											FLOAT bottom ) const
	{
    displayDevice->Begin( D3DPT_QUADLIST );      
			displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_left, m_top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, top, 1.0f, 1.0f );
      
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_right, m_top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, top, 1.0f, 1.0f );
      
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_right, m_bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, bottom, 1.0f, 1.0f );

      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_left, m_bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, bottom, 1.0f, 1.0f );
    displayDevice->End();
	}



		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI(	CSystem_IniFile &iniFile, 
													const CStdString &sectionName, 
													const CStdString &entryName ) 
	{
		CStdString fullEntryName = entryName;
		fullEntryName += ".FileOffset.";

		m_left		= iniFile.GetProfileInt( sectionName, fullEntryName + "left", VALUE_INVALID );
		m_right		= iniFile.GetProfileInt( sectionName, fullEntryName + "right", VALUE_INVALID );
		m_top			= iniFile.GetProfileInt( sectionName, fullEntryName + "top", VALUE_INVALID );
		m_bottom	= iniFile.GetProfileInt( sectionName, fullEntryName + "bottom", VALUE_INVALID );

		int width = iniFile.GetProfileInt( sectionName, fullEntryName + "width", VALUE_INVALID );
		int height = iniFile.GetProfileInt( sectionName, fullEntryName + "height", VALUE_INVALID );

		if( m_right == VALUE_INVALID && width != VALUE_INVALID )
			m_right = m_left + width;

		if( m_bottom == VALUE_INVALID && height != VALUE_INVALID )
			m_bottom = m_top + height;

		return (m_left != VALUE_INVALID && m_right != VALUE_INVALID && m_top != VALUE_INVALID && m_bottom != VALUE_INVALID);
	}


};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

