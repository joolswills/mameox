/**
	* \file			SkinMenuFooter.h
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
 * \class		CSkinMenuFooter
 * \brief   Holds positioning info for a menu footer and provides rendering functions
 */
class CSkinMenuFooter : public CSkin3PartBackdrop
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinMenuFooter( void ) : CSkin3PartBackdrop( SKINELEMENT_MENU_FOOTER, 
																								SPRITE_MENU_FOOTER_LEFTSIDE, 
																								SPRITE_MENU_FOOTER_CENTER, 
																								SPRITE_MENU_FOOTER_RIGHTSIDE )	{}

		//------------------------------------------------------
		//	Constructor (CSkinSpriteResource)
		//------------------------------------------------------
	CSkinMenuFooter( const CSkinMenuFooter &a ) : CSkin3PartBackdrop( a ) {}


		//------------------------------------------------------
		//------------------------------------------------------
	virtual BOOL ParseINI( CSystem_IniFile &iniFile, const CStdString &sectionName ) ;
};



//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

#endif		// __cplusplus

