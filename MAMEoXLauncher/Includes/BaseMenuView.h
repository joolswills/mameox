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
#include "TextureSet.h"

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
  CBaseMenuView( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, CTextureSet &textureSet, const RECT &area ) :
    CBaseView( displayDevice, fontSet, NULL ),
    m_textureSet( textureSet ) {
      SetRect( area );
  }


		//------------------------------------------------------------
		// SetRect
		//! \brief		Sets the display position/size of the menu
		//------------------------------------------------------------
  virtual void SetRect( const RECT &area ) {
      // Make sure the area is big enough to display something useful
    assert( area.bottom - area.top > m_textureSet.GetMenuTitleBarHeight() + m_textureSet.GetMenuFooterHeight() );
    assert( area.right - area.left > m_textureSet.GetMenuTitleBarLeftRight() - m_textureSet.GetMenuTitleBarLeftLeft() );

      // Determine the layout areas
    m_titleArea.top = area.top;
    m_titleArea.bottom = area.top + m_textureSet.GetMenuTitleBarHeight();

    m_bodyArea.top = m_titleArea.bottom;
    m_bodyArea.bottom = area.bottom - m_textureSet.GetMenuFooterHeight();

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

  CTextureSet               &m_textureSet;
};

