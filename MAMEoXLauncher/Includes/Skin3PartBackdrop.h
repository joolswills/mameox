/**
	* \file			SkinElement.h
	* \brief		CSkinElement base class for skin objects
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "SkinElement.h"
#include "SkinSpriteResource.h"



#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================



//= C L A S S E S ====================================================

/**
 * \class		CSkin3PartBackdrop
 * \brief   Base class that handles rendering 3 part backdrops (left/center/right)
 */
class CSkin3PartBackdrop : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkin3PartBackdrop( SkinElementType_t type, 
											SkinResourceID_t left, 
											SkinResourceID_t center, 
											SkinResourceID_t right, 
											BOOL flipHorizontal = FALSE, 
											BOOL flipVertical = FALSE ) : 
			CSkinElement( type )	
	{
		m_spriteResourceIDs[0] = left;
		m_spriteResourceIDs[1] = center;
		m_spriteResourceIDs[2] = right;
		m_flipHorizontal = flipHorizontal;
		m_flipVertical = flipVertical;
	}

		//------------------------------------------------------
		//	Constructor (CSkinSpriteResource)
		//------------------------------------------------------
	CSkin3PartBackdrop( const CSkin3PartBackdrop &a ) : CSkinElement( a ) 
	{
		memcpy( m_spriteResourceIDs, a.m_spriteResourceIDs, sizeof(m_spriteResourceIDs) );
		m_flipHorizontal = a.m_flipHorizontal;
		m_flipVertical = a.m_flipVertical;
	}


		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice ) const { Render( displayDevice, m_left, m_top, m_right, m_bottom ); }

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render( LPDIRECT3DDEVICE8 displayDevice, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom ) const;


	SkinResourceID_t		m_spriteResourceIDs[3];

	BOOL								m_flipHorizontal;
	BOOL								m_flipVertical;
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

