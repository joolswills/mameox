/**
	* \file			SkinMenuBody.h
	* \brief		
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "SkinElement.h"
#include "Skin3PartBackdrop.h"
#include "SkinSpriteResource.h"



#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================



//= C L A S S E S ====================================================

/**
 * \class		CSkinMenuBody
 * \brief   Holds positioning info for a menu body
 */
class CSkinMenuBody : public CSkin3PartBackdrop
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinMenuBody( void ) : CSkin3PartBackdrop( SKINELEMENT_MENU_FOOTER, 
																							SPRITE_MENU_BODY_LEFTSIDE, 
																							SPRITE_MENU_BODY_CENTER, 
																							SPRITE_MENU_BODY_RIGHTSIDE ) {}

		//------------------------------------------------------
		//	Constructor (CSkinSpriteResource)
		//------------------------------------------------------
	CSkinMenuBody( const CSkinMenuBody &a ) : CSkin3PartBackdrop( a ) {}


		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName );
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

