/**
	* \file			SkinResource.h
	* \brief		CSkinResource class
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "PNGFunctions.h"

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

	RESOURCE_COUNT,		// Note: This must be the last "real" member of the enum
	RESOURCE_INVALID = 0x7FFFFFFF
} SkinResourceID_t;




	// Enum containing all of the skin colors
typedef enum SkinColorID_t {
	COLOR_POPUP_DARKENOVERLAY,

	COLOR_HELPSCREEN_TITLEBAR_TEXT,
	COLOR_HELPSCREEN_BODY_TEXT,
	COLOR_HELPSCREEN_BUTTONICON_TEXT,

	COLOR_OPTIONSSCREEN_TITLEBAR_TEXT,
	COLOR_OPTIONSSCREEN_BODY_TEXT,
	COLOR_OPTIONSSCREEN_BODY_HIGHLIGHTBAR,
	COLOR_OPTIONSSCREEN_BODY_DIVIDER,
	COLOR_OPTIONSSCREEN_BODY_TRIGGERICON_TEXT,
	COLOR_OPTIONSSCREEN_BUTTONICON_TEXT,

	COLOR_ROMLISTSCREEN_TITLEBAR_TEXT,
	COLOR_ROMLISTSCREEN_BODY_TEXT,
	COLOR_ROMLISTSCREEN_ROMWORKING_TEXT,
	COLOR_ROMLISTSCREEN_ROMWARNING_TEXT,
	COLOR_ROMLISTSCREEN_ROMNONWORKING_TEXT,
	COLOR_ROMLISTSCREEN_BODY_HIGHLIGHTBAR,
	COLOR_ROMLISTSCREEN_BODY_DIVIDER,
	COLOR_ROMLISTSCREEN_SCREENSHOT_BACKGROUND,
	COLOR_ROMLISTSCREEN_SCREENSHOT_TEXT,
	COLOR_ROMLISTSCREEN_BUTTONICON_TEXT,

	COLOR_SCREENSAVER_TEXT,

	COLOR_SKINCHOOSERSCREEN_TITLEBAR_TEXT,
	COLOR_SKINCHOOSERSCREEN_BODY_TEXT,
	COLOR_SKINCHOOSERSCREEN_BODY_HIGHLIGHTBAR,
	COLOR_SKINCHOOSERSCREEN_BODY_DIVIDER,
	COLOR_SKINCHOOSERSCREEN_SCREENSHOT_BACKGROUND,
	COLOR_SKINCHOOSERSCREEN_SCREENSHOT_TEXT,
	COLOR_SKINCHOOSERSCREEN_BUTTONICON_TEXT,

	COLOR_STARTMENU_TITLEBAR_TEXT,
	COLOR_STARTMENU_BODY_TEXT,
	COLOR_STARTMENU_BODY_HIGHLIGHTBAR,

	COLOR_VIRTUALKEYBOARD_TITLEBAR_TEXT,
	COLOR_VIRTUALKEYBOARD_BODY_TEXT,
	COLOR_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR,

	COLOR_MESSAGESCREEN_TEXT,
	COLOR_MESSAGESCREEN_PROGRESSBAR_END,
	COLOR_MESSAGESCREEN_PROGRESSBAR_BAR,
	COLOR_MESSAGESCREEN_PROGRESSBAR_BACKGROUND,

	COLOR_SPLASHSCREEN_TEXT,
	COLOR_SPLASHSCREEN_IMPORTANT_TEXT,



	COLOR_COUNT,		// Note: This must be the last "real" member of the enum
	COLOR_INVALID = 0x7FFFFFFF
} SkinColorID_t;




//= C L A S S E S ====================================================

/**
 * \struct  SkinResourceInfo_t
 * \brief   Holds positioning info for a sprite
 */
typedef struct SkinResourceInfo_t
{
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	SkinResourceInfo_t( void ) : 
		m_left( -1.0f ), 
		m_top( -1.0f ), 
		m_right( -1.0f ), 
		m_bottom( -1.0f ) 
	{	}

		//------------------------------------------------------
		//	Constructor (SkinResourceInfo_t)
		//------------------------------------------------------
	SkinResourceInfo_t( const SkinResourceInfo_t &a ) {	
		m_left = a.m_left;
		m_top = a.m_top;
		m_right = a.m_right;
		m_bottom = a.m_bottom;
	}


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
		//	Render  (Singe Diffuse variant)
		//------------------------------------------------------
	inline void Render( LPDIRECT3DDEVICE8 displayDevice, 
											int colorRegister, 
											D3DCOLOR color, 
											FLOAT left, 
											FLOAT top ) const
	{
		Render( displayDevice, colorRegister, color, left, top, left + GetWidth(), top + GetHeight() );
	}

		//------------------------------------------------------
		//	Render  (Singe Diffuse variant)
		//------------------------------------------------------
	inline void Render( LPDIRECT3DDEVICE8 displayDevice, 
											int colorRegister, 
											D3DCOLOR color, 
											FLOAT left, 
											FLOAT top, 
											FLOAT right, 
											FLOAT bottom ) const
	{
    displayDevice->Begin( D3DPT_QUADLIST );      
      displayDevice->SetVertexDataColor( colorRegister, color );
			displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_left, m_top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, top, 1.0f, 1.0f );
      
      displayDevice->SetVertexDataColor( colorRegister, color );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_right, m_top );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, top, 1.0f, 1.0f );
      
      displayDevice->SetVertexDataColor( colorRegister, color );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_right, m_bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, right, bottom, 1.0f, 1.0f );

      displayDevice->SetVertexDataColor( colorRegister, color );
      displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_left, m_bottom );
      displayDevice->SetVertexData4f( D3DVSDE_VERTEX, left, bottom, 1.0f, 1.0f );
    displayDevice->End();
	}




	inline FLOAT GetWidth( void ) const { return m_right - m_left; }
	inline FLOAT GetHeight( void ) const { return m_bottom - m_top; }

	FLOAT			m_left;
	FLOAT			m_top;
	FLOAT			m_right;
	FLOAT			m_bottom;
} SkinResourceInfo_t;

/**
 * \class   CSkinResource
 * \brief   Utility class which loads skin INI files and all their
 *          required graphics resources.
 */
class CSkinResource
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinResource( const char *basePath, LPDIRECT3DTEXTURE8 previewTexture, const RECT &previewTextureSize ) : 
			m_skinName( basePath ),
			m_previewTexture( previewTexture ),
			m_previewTextureRect( previewTextureSize )
	{
		m_splashBackdropTexture = m_messageBackdropTexture = m_listBackdropTexture = 
			m_TVCalibrationSpritesTexture = m_lightgunCalibrationSpritesTexture = m_listSpritesTexture =	
			m_menuSpritesTexture = m_buttonSpritesTexture = NULL;

		memset( m_spriteInfoArray, 0, sizeof(m_spriteInfoArray) );
	}

		//------------------------------------------------------
		//	Destructor
		//------------------------------------------------------
	~CSkinResource( void ) {
		SAFE_RELEASE( m_previewTexture );

		UnloadSkin();
	}



		//------------------------------------------------------
		//	LoadSkin
		//!	\brief		Initialize this instance with the skin.ini
		//!						 file to be found in basePath.
		//!
		//! \param		basePath - Path containing skin files
		//! \param		errorReport - [OUT] Pointer to a variable
		//!            into which detailed error information should
		//!            be placed.
		//------------------------------------------------------
  BOOL LoadSkin( CStdString *errorReport );

		//------------------------------------------------------
		//	UnloadSkin
		//!	\brief		Unload skin data other than the preview
		//!						and name.
		//------------------------------------------------------
	void UnloadSkin( void ) {
			// Free the textures
		SAFE_RELEASE( m_splashBackdropTexture );
		SAFE_RELEASE( m_messageBackdropTexture );
		SAFE_RELEASE( m_listBackdropTexture );
		SAFE_RELEASE( m_TVCalibrationSpritesTexture );
		SAFE_RELEASE( m_lightgunCalibrationSpritesTexture );
		SAFE_RELEASE( m_listSpritesTexture );
		SAFE_RELEASE( m_menuSpritesTexture );
		SAFE_RELEASE( m_buttonSpritesTexture );

			// Free the sprite info structs
		for( int i = 0; i < RESOURCE_COUNT; ++i )
		{
			if( m_spriteInfoArray[i] )
			{
				delete m_spriteInfoArray[i];
				m_spriteInfoArray[i] = NULL;
			}
		}
	}

		//------------------------------------------------------
		// GetSkinName
		//------------------------------------------------------
	const CStdString &GetSkinName( void ) const { return m_skinName;	}

		//------------------------------------------------------
		// GetPreviewTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 GetPreviewTexture( void ) { return m_previewTexture;	}

		//------------------------------------------------------
		// GetPreviewTextureRect
		//------------------------------------------------------
	const RECT &GetPreviewTextureRect( void ) const { return m_previewTextureRect; }


		//------------------------------------------------------
		// GetSkinResourceInfo
		//------------------------------------------------------
	inline const SkinResourceInfo_t *GetSkinResourceInfo( SkinResourceID_t id ) const { return m_spriteInfoArray[id]; }

		//------------------------------------------------------
		// GetSkinResourceTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 GetSkinResourceTexture( SkinResourceID_t id ) const;


		//------------------------------------------------------
		// SelectSkinResourceTexture
		//------------------------------------------------------
	void SelectSkinResourceTexture( LPDIRECT3DDEVICE8 displayDevice, SkinResourceID_t id ) const {
			// Calling this everytime is terribly inefficient, but it's only
			// done in the Launcher UI (which is not time-critical)
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		displayDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

		displayDevice->SetTexture( 0, GetSkinResourceTexture( id ) );
	}

		//------------------------------------------------------
		// GetSkinColor
		//------------------------------------------------------
	inline D3DCOLOR GetSkinColor( SkinColorID_t id ) const { return m_spriteColorArray[id]; }


protected:
		//------------------------------------------------------
		//	LoadSkinTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 LoadSkinTexture( CSystem_IniFile &iniFile,
																			const CStdString &sectionName, 
																			const CStdString &colorChannelEntry, 
																			const CStdString &alphaChannelEntry );

	CStdString							m_skinName;
	LPDIRECT3DTEXTURE8			m_previewTexture;
	RECT										m_previewTextureRect;

	LPDIRECT3DTEXTURE8			m_splashBackdropTexture;
	LPDIRECT3DTEXTURE8			m_messageBackdropTexture;
	LPDIRECT3DTEXTURE8			m_listBackdropTexture;
	LPDIRECT3DTEXTURE8			m_TVCalibrationSpritesTexture;
	LPDIRECT3DTEXTURE8			m_lightgunCalibrationSpritesTexture;
	LPDIRECT3DTEXTURE8			m_listSpritesTexture;
	LPDIRECT3DTEXTURE8			m_menuSpritesTexture;
	LPDIRECT3DTEXTURE8			m_buttonSpritesTexture;

		// Array of sprite info structs defining where a particular sprite
		// can be found within its asset texture
	SkinResourceInfo_t				*m_spriteInfoArray[RESOURCE_COUNT];

	D3DCOLOR									m_spriteColorArray[COLOR_COUNT];
};


//= G L O B A L = V A R S =============================================
extern CSkinResource			*g_loadedSkin;

//= P R O T O T Y P E S ===============================================


//= F U N C T I O N S =================================================
//------------------------------------------------------
//	CheckResourceValidity
//------------------------------------------------------
inline BOOL CheckResourceValidity( SkinResourceID_t id ) {
	return g_loadedSkin && g_loadedSkin->GetSkinResourceInfo( id ) && g_loadedSkin->GetSkinResourceTexture( id );
}

//------------------------------------------------------
//	CheckResourceTextureValidity
//------------------------------------------------------
inline BOOL CheckResourceTextureValidity( SkinResourceID_t id ) {
	return g_loadedSkin && g_loadedSkin->GetSkinResourceTexture( id );
}


#endif		// __cplusplus

