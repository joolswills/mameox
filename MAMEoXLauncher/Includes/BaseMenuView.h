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
#include "Skin.h"

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
  CBaseMenuView( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet ) :
          CBaseView( displayDevice, fontSet, SKINRESOURCE_INVALID ) 
  {
    memset( &m_titleArea, 0, sizeof(m_titleArea) );
    memset( &m_bodyArea, 0, sizeof(m_bodyArea) );
    memset( &m_footerArea, 0, sizeof(m_footerArea) );
  }

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CBaseMenuView( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, const RECT &totalArea ) :
          CBaseView( displayDevice, fontSet, SKINRESOURCE_INVALID ) 
  {
		SetRect( totalArea );
  }


		//------------------------------------------------------------
		// SetRect
		//! \brief		Sets the display position/size of the menu
		//------------------------------------------------------------
  virtual void SetRect( const RECT &area ) {

		m_titleArea.top = area.top;
		m_titleArea.bottom = area.top;

		if( CheckResourceValidity( SPRITE_MENU_TITLEBAR_CENTER ) )
			m_titleArea.bottom += g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_TITLEBAR_CENTER )->GetHeight();

		m_bodyArea.top = m_titleArea.bottom;
		m_bodyArea.bottom = area.bottom;

		if( CheckResourceValidity( SPRITE_MENU_FOOTER_CENTER ) )
			m_bodyArea.bottom -= g_loadedSkin->GetSkinSpriteResource( SPRITE_MENU_FOOTER_CENTER )->GetHeight();

    m_footerArea.top = m_bodyArea.bottom;
    m_footerArea.bottom = area.bottom;


    m_footerArea.left = m_titleArea.left = m_bodyArea.left = area.left;
    m_footerArea.right = m_titleArea.right = m_bodyArea.right = area.right;
  }

		//------------------------------------------------------------
		// SetBodyRect
		//! \brief		Sets only the body area for the menu
		//------------------------------------------------------------
  virtual void SetBodyRect( const RECT &area ) {
    m_bodyArea = area;
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

	const RECT &GetTitleArea( void )  const { return m_titleArea; }
	const RECT &GetBodyArea( void )   const { return m_bodyArea; }
	const RECT &GetFooterArea( void ) const { return m_footerArea; }

protected:

		//------------------------------------------------------------
		// RenderBackdrop
		//! \brief		Render the backdrop texture to m_displayDevice
		//------------------------------------------------------------
	virtual void RenderBackdrop( void );

  RECT                      m_titleArea;      //!<  Rect enclosing the are to be used for title rendering
  RECT                      m_bodyArea;       //!<  Rect enclosing the area to be used for the body text
  RECT                      m_footerArea;     //!<  Rect enclosing the area to be used for the body text
};

