/**
	* \file			Skin.h
	* \brief		CSkin class
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "PNGFunctions.h"

#include <XBUtil.h>

#include "SkinElement.h"
#include "SkinSpriteResource.h"
#include "SkinMenuFooter.h"
#include "SkinMenuBody.h"
#include "SkinMenuHeader.h"
#include "SkinHighlightBar.h"
#include "SkinIcon.h"
#include "SkinText.h"
#include "SkinScrollArea.h"
#include "SkinButtonInfo.h"
#include "SkinScreenshot.h"
#include "SkinDiffuseOverlay.h"
#include "SkinDivider.h"
#include "SkinTriggerInfo.h"
#include "SkinProgressbar.h"


#ifdef __cplusplus

//= D E F I N E S =====================================================

#ifdef _DEBUG
#define CHECK_CAST( c, type )			assert( !c || c->m_type == type )
#else
#define CHECK_CAST( c, type )
#endif

typedef enum SkinElementID_t {
	SKINELEMENT_POPUPOPTIONS_DIFFUSEOVERLAY,


	SKINELEMENT_ROMLISTSCREEN_HEADER,
	SKINELEMENT_ROMLISTSCREEN_HEADER_SIMPLE_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_MANUFACTERER_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_YEAR_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_NUMPLAYERS_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_VERBOSELIST_CLONE_TEXT,
	SKINELEMENT_ROMLISTSCREEN_HEADER_DETAIL_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA,

	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWORKING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMWARNING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_ROMNONWORKING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_MANUFACTURER_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_YEAR_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_NUMPLAYERS_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_VERBOSE_CLONE_TEXT,

	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWORKING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMWARNING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_SIMPLE_ROMNONWORKING_NAME_TEXT,

	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWORKING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMWARNING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_ROMNONWORKING_NAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_NUMPLAYERS_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_MANUFACTURER_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_YEAR_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_PARENT_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_GENRE_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_VERSIONADDED_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_TIMESPLAYED_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FAVORITESTATUS_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_FILENAME_TEXT,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLAREA_SINGLEROW_DETAIL_SCREENSHOT,

	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_UP,
	SKINELEMENT_ROMLISTSCREEN_BODY_SCROLLICON_DOWN,
	SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_MANUFACTURER_DIVIDER,
	SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_YEAR_DIVIDER,
	SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_NUMPLAYERS_DIVIDER,
	SKINELEMENT_ROMLISTSCREEN_BODY_VERBOSE_CLONE_DIVIDER,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_A,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_B,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_X,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_Y,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_WHITE,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_BLACK,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_TRIGGER_LEFT,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_TRIGGER_RIGHT,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_ANALOG_LEFT,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_ANALOG_RIGHT,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_BACK,
	SKINELEMENT_ROMLISTSCREEN_BUTTONINFO_START,
	SKINELEMENT_ROMLISTSCREEN_FOOTER,


	SKINELEMENT_HELPSCREEN_HEADER,
	SKINELEMENT_HELPSCREEN_HEADER_TEXT,
	SKINELEMENT_HELPSCREEN_BODY,
	SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA,
	SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA_SINGLEROW_TEXT,
	SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_UP,
	SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_DOWN,
	SKINELEMENT_HELPSCREEN_BUTTONINFO_START,
	SKINELEMENT_HELPSCREEN_FOOTER,

	SKINELEMENT_OPTIONSSCREEN_HEADER,
	SKINELEMENT_OPTIONSSCREEN_HEADER_TEXT,
	SKINELEMENT_OPTIONSSCREEN_BODY,
	SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA,
	SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONNAME_TEXT,
	SKINELEMENT_OPTIONSSCREEN_BODY_SCROLLAREA_SINGLEROW_OPTIONVALUE_TEXT,
	SKINELEMENT_OPTIONSSCREEN_BODY_DIVIDER,
	SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_LEFT,
	SKINELEMENT_OPTIONSSCREEN_BODY_TRIGGERINFO_RIGHT,
	SKINELEMENT_OPTIONSSCREEN_BUTTONINFO_START,
	SKINELEMENT_OPTIONSSCREEN_FOOTER,

	SKINELEMENT_SKINCHOOSERSCREEN_HEADER,
	SKINELEMENT_SKINCHOOSERSCREEN_HEADER_TEXT,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAME_TEXT,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_DESCRIPTION_TEXT,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAMEWITHOUTDESCRIPTION_TEXT,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_SCREENSHOT,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_UP,
	SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_DOWN,
	SKINELEMENT_SKINCHOOSERSCREEN_BUTTONINFO_START,
	SKINELEMENT_SKINCHOOSERSCREEN_FOOTER,

	SKINELEMENT_SCREENSAVEROPTIONS_TEXT,

	SKINELEMENT_MESSAGESCREEN_TEXT,
	SKINELEMENT_MESSAGESCREEN_PROGRESSBAR,

	SKINELEMENT_SPLASHSCREEN_TEXT,
	SKINELEMENT_SPLASHSCREEN_IMPORTANT_TEXT,

	SKINELEMENT_VIRTUALKEYBOARD_HEADER_TEXT,
	SKINELEMENT_VIRTUALKEYBOARD_BODY_TEXT,
	SKINELEMENT_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR,

	SKINELEMENT_STARTMENU_HEADER_TEXT,
	SKINELEMENT_STARTMENU_BODY_TEXT,
	SKINELEMENT_STARTMENU_BODY_HIGHLIGHTBAR,

	SKINELEMENT_COUNT,								// Note: This must be the last "real" member of the enum
	SKINELEMENT_INVALID = 0x7FFFFFFF
} SkinElementID_t;


//= C L A S S E S ====================================================

/**
 * \class   CSkin
 * \brief   Utility class which loads skin INI files and all their
 *          required graphics resources.
 */
class CSkin
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkin( const CStdString &basePath, const CStdString &author, const CStdString &description, LPDIRECT3DTEXTURE8 previewTexture, const RECT &previewTextureSize ) : 
			m_skinName( basePath ),
			m_previewTexture( previewTexture ),
			m_previewTextureRect( previewTextureSize ),
			m_author( author ),
			m_description( description )
	{
		m_splashBackdropTexture = m_messageBackdropTexture = m_listBackdropTexture = 
			m_TVCalibrationSpritesTexture = m_lightgunCalibrationSpritesTexture = m_listSpritesTexture =	
			m_menuSpritesTexture = m_buttonSpritesTexture = NULL;

		memset( m_spriteResourceArray, 0, sizeof(m_spriteResourceArray) );
		memset( m_spriteElementArray, 0, sizeof(m_spriteElementArray) );
	}

		//------------------------------------------------------
		//	Destructor
		//------------------------------------------------------
	~CSkin( void ) {
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
		for( int i = 0; i < SKINRESOURCE_COUNT; ++i )
		{
			if( m_spriteResourceArray[i] )
			{
				delete m_spriteResourceArray[i];
				m_spriteResourceArray[i] = NULL;
			}
		}
	}

		//------------------------------------------------------
		// GetSkinName
		//------------------------------------------------------
	const CStdString &GetSkinName( void ) const { return m_skinName;	}

		//------------------------------------------------------
		// GetSkinAuthor
		//------------------------------------------------------
	const CStdString &GetSkinAuthor( void ) const { return m_author;	}

		//------------------------------------------------------
		// GetSkinDescription
		//------------------------------------------------------
	const CStdString &GetSkinDescription( void ) const { return m_description;	}

		//------------------------------------------------------
		// GetPreviewTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 GetPreviewTexture( void ) { return m_previewTexture;	}

		//------------------------------------------------------
		// GetPreviewTextureRect
		//------------------------------------------------------
	const RECT &GetPreviewTextureRect( void ) const { return m_previewTextureRect; }


		//------------------------------------------------------
		// GetSkinSpriteResource
		//------------------------------------------------------
	inline const CSkinSpriteResource *GetSkinSpriteResource( SkinResourceID_t id ) const { return m_spriteResourceArray[id]; }

		//------------------------------------------------------
		// GetSkinElement
		//------------------------------------------------------
	inline const CSkinElement *GetSkinElement( SkinElementID_t id ) const { return m_spriteElementArray[id]; }

		//------------------------------------------------------
		// GetSkinElementHighlightbar
		//------------------------------------------------------
	inline const CSkinHighlightBar *GetSkinElementHighlightbar( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_HIGHLIGHTBAR );
		return (CSkinHighlightBar*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementText
		//------------------------------------------------------
	inline const CSkinText *GetSkinElementText( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_TEXT );
		return (CSkinText*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementScrollArea
		//------------------------------------------------------
	inline const CSkinScrollArea *GetSkinElementScrollArea( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_SCROLLAREA );
		return (CSkinScrollArea*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementButtonInfo
		//------------------------------------------------------
	inline const CSkinButtonInfo *GetSkinElementButtonInfo( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_BUTTONINFO );
		return (CSkinButtonInfo*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementScreenshot
		//------------------------------------------------------
	inline const CSkinScreenshot *GetSkinElementScreenshot( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_SCREENSHOT );
		return (CSkinScreenshot*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementDiffuseOverlay
		//------------------------------------------------------
	inline const CSkinDiffuseOverlay *GetSkinElementDiffuseOverlay( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_DIFFUSEOVERLAY );
		return (CSkinDiffuseOverlay*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementTriggerInfo
		//------------------------------------------------------
	inline const CSkinTriggerInfo *GetSkinElementTriggerInfo( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_TRIGGERINFO );
		return (CSkinTriggerInfo*)m_spriteElementArray[id]; 
	}

		//------------------------------------------------------
		// GetSkinElementProgressbar
		//------------------------------------------------------
	inline const CSkinProgressbar *GetSkinElementProgressbar( SkinElementID_t id ) const { 
		CHECK_CAST( m_spriteElementArray[id], SKINELEMENT_PROGRESSBAR );
		return (CSkinProgressbar*)m_spriteElementArray[id]; 
	}


		//------------------------------------------------------
		// GetSkinTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 GetSkinTexture( SkinResourceID_t id ) const;


		//------------------------------------------------------
		// SelectSkinTexture
		//------------------------------------------------------
	void SelectSkinTexture( LPDIRECT3DDEVICE8 displayDevice, SkinResourceID_t id ) const {
			// Calling this everytime is terribly inefficient, but it's only
			// done in the Launcher UI (which is not time-critical)
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );
		displayDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		displayDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

		displayDevice->SetTexture( 0, GetSkinTexture( id ) );
	}

protected:
		//------------------------------------------------------
		//	LoadSkinTexture
		//------------------------------------------------------
	LPDIRECT3DTEXTURE8 LoadSkinTexture( CSystem_IniFile &iniFile,
																			const CStdString &sectionName, 
																			const CStdString &colorChannelEntry, 
																			const CStdString &alphaChannelEntry );

	CStdString							m_skinName;
	CStdString							m_author;
	CStdString							m_description;
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
	CSkinSpriteResource								*m_spriteResourceArray[SKINRESOURCE_COUNT];
	CSkinElement											*m_spriteElementArray[SKINELEMENT_COUNT];
};





//= G L O B A L = V A R S =============================================
extern CSkin			*g_loadedSkin;

//= P R O T O T Y P E S ===============================================


//= F U N C T I O N S =================================================
//------------------------------------------------------
//	CheckResourceValidity
//------------------------------------------------------
inline BOOL CheckResourceValidity( SkinResourceID_t id ) {
	return g_loadedSkin && g_loadedSkin->GetSkinSpriteResource( id ) && g_loadedSkin->GetSkinTexture( id );
}

//------------------------------------------------------
//	CheckResourceValidity
//------------------------------------------------------
inline BOOL CheckResourceValidity( SkinElementID_t id ) {
	return g_loadedSkin && g_loadedSkin->GetSkinElement( id );
}

//------------------------------------------------------
//	CheckResourceTextureValidity
//------------------------------------------------------
inline BOOL CheckResourceTextureValidity( SkinResourceID_t id ) {
	return g_loadedSkin && g_loadedSkin->GetSkinTexture( id );
}


#endif		// __cplusplus

