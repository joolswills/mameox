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

#include <XBUtil.h>


//= D E F I N E S =====================================================
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

	RESOURCE_COUNT		// Note: This must be the last member of the enum
} SkinResourceID_t;

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
		// GetSplashBackdropTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetSplashBackdropTexture( void ) { return m_splashBackdropTexture; }

		//------------------------------------------------------
		// GetMessageBackdropTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetMessageBackdropTexture( void ) { return m_messageBackdropTexture; }

		//------------------------------------------------------
		// GetListBackdropTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetListBackdropTexture( void ) { return m_listBackdropTexture; }

		//------------------------------------------------------
		// GetTVCalibrationSpritesTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetTVCalibrationSpritesTexture( void ) { return m_TVCalibrationSpritesTexture; }

		//------------------------------------------------------
		// GetLightgunCalibrationSpritesTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetLightgunCalibrationSpritesTexture( void ) { return m_lightgunCalibrationSpritesTexture; }

		//------------------------------------------------------
		// GetListSpritesTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetListSpritesTexture( void ) { return m_listSpritesTexture; }

		//------------------------------------------------------
		// GetMenuSpritesTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetMenuSpritesTexture( void ) { return m_menuSpritesTexture; }

		//------------------------------------------------------
		// GetButtonSpritesTexture
		//------------------------------------------------------
	inline LPDIRECT3DTEXTURE8 GetButtonSpritesTexture( void ) { return m_buttonSpritesTexture; }


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
};


//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================
