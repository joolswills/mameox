/**
	* \file			BaseMenuView.h
	* \brief		Resizable "Menu" drawable
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"

#include "BaseView.h"
#include "StdString.h"
#include "SkinResource.h"

//= D E F I N E S ======================================================

//= C L A S S E S ======================================================

/**
	* \class		CBaseMenuView
	* \brief		The base class for a resizable menu drawable
	*/
class CBaseMenuView : public CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CBaseMenuView( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, const RECT &area ) :
    CBaseView( displayDevice, fontSet, RESOURCE_INVALID ) {
      SetRect( area );
  }


		//------------------------------------------------------------
		// SetRect
		//! \brief		Sets the display position/size of the menu
		//------------------------------------------------------------
  virtual void SetRect( const RECT &area ) {

		m_titleArea.top = area.top;
		m_titleArea.bottom = area.top;

		if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_CENTER ) )
			m_titleArea.bottom += g_loadedSkin->GetSkinResourceInfo( SPRITE_MENU_TITLEBAR_CENTER )->GetHeight();

		m_bodyArea.top = m_titleArea.bottom;
		m_bodyArea.bottom = area.bottom;

		if( CheckResourceValidity( SPRITE_MENU_FOOTER_CENTER ) )
			m_bodyArea.bottom -= g_loadedSkin->GetSkinResourceInfo( SPRITE_MENU_FOOTER_CENTER )->GetHeight();

    m_titleArea.left = m_bodyArea.left = area.left;
    m_titleArea.right = m_bodyArea.right = area.right;
  }

    // This base view can be used independently, so the pure virtuals from
    // CBaseView must be defined as nop's
  virtual void MoveCursor( CInputManager &inputManager, BOOL useSpeedBanding = FALSE ) {}


		//------------------------------------------------------------
		// Draw
		//! \brief		Render the list to the screen
    //!
    //! \param    clearScreen - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
  virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE ) {
    if( clearScreen )  
	    m_displayDevice->Clear(	0L,																// Count
		  											  NULL,															// Rects to clear
			  										  D3DCLEAR_TARGET,	                // Flags
				  									  D3DCOLOR_XRGB(0,0,0),							// Color
					  								  1.0f,															// Z
						  							  0L );															// Stencil
    RenderBackdrop();

    if( flipOnCompletion )
	    m_displayDevice->Present( NULL, NULL, NULL, NULL );	
  }

protected:

		//------------------------------------------------------------
		// RenderBackdrop
		//! \brief		Render the backdrop texture to m_displayDevice
		//------------------------------------------------------------
	virtual void RenderBackdrop( void );

  RECT                      m_titleArea;      //!<  Rect enclosing the are to be used for title rendering
  RECT                      m_bodyArea;       //!<  Rect enclosing the area to be used for the body text
};

