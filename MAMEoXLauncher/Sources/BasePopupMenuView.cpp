/**
	* \file			BasePopupView.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "BasePopupMenuView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================
 
//---------------------------------------------------------------------
//  RenderBackdrop
//---------------------------------------------------------------------
void CBasePopupView::RenderBackdrop( FLOAT fontHeight )
{
	if( !CheckResourceValidity(SKINELEMENT_POPUPOPTIONS_DIFFUSEOVERLAY)  )
		return;

    //--- Darken the screen under the menu ----------------------------------------
	g_loadedSkin->GetSkinElementDiffuseOverlay(SKINELEMENT_POPUPOPTIONS_DIFFUSEOVERLAY)->Render( m_displayDevice );

    // Use the fontHeight to calculate the correct area for the backdrop
	UINT32 halfheight = fontHeight * GetNumBodyLines();
	if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_CENTER ) )
		halfheight += (UINT32)g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_TITLEBAR_CENTER )->GetHeight();
	
	if( CheckResourceValidity( SPRITE_MENU_FOOTER_CENTER ) )
		halfheight += (UINT32)g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_FOOTER_CENTER )->GetHeight();
	halfheight >>= 1;
	
  RECT area = { m_titleArea.left, 240 - halfheight,
                m_titleArea.right, 240 + halfheight };
  SetRect( area );  
  CBaseMenuView::RenderBackdrop();
}

