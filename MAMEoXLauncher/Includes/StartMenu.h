/**
	* \file			StartMenu.h
	* \brief		Modal start menu
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <vector>

#include "BasePopupView.h"


//= D E F I N E S ======================================================



//= C L A S S E S ======================================================

/**
	* \class		CStartMenu
	* \brief		The generic modal menu window
	*/
class CStartMenu : public CBasePopupView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CStartMenu( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, CTextureSet &textureSet ) :
    CBasePopupView( displayDevice, fontSet, textureSet ) {
      Reset();
    }

	//------------------------------------------------------------
	// AddMenuItem
	//! \brief		Add a menu string to the menu
	//------------------------------------------------------------
  void AddMenuItem( const CStdString &str ) {
    m_menuItems.push_back( str );
  }

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset (most) member vars to defaults
		//------------------------------------------------------------
  virtual void Reset( void ) {
    CBasePopupView::Reset();

		m_cursorPosition = 0;
 		m_dpadCursorDelay = 0.0f;
    m_buttonDelay = 0.0f;
    m_menuItems.clear();
  }

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The CInputManager containing the
		//!                current state of the user's joystick(s)
    //! \param    useSpeedBanding - Whether or not to enable
    //!              "speed bands" (cursor acceleration)
		//------------------------------------------------------------
	virtual void MoveCursor( CInputManager &inputManager, BOOL unused = FALSE );

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the list to the screen
    //!
    //! \param    clearScreen - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE );

		//------------------------------------------------------------
		// GetNumBodyLines
		//! \brief		Returns the number of text lines in the body
    //!           of the popup
		//------------------------------------------------------------
  UINT32 GetNumBodyLines( void ) const { return m_menuItems.size(); }
  
  UINT32 GetCursorPosition( void ) const { return m_cursorPosition; }

protected:

	FLOAT									    m_dpadCursorDelay;
	FLOAT									    m_buttonDelay;

  UINT32                    m_cursorPosition;

  std::vector<CStdString>   m_menuItems;      //!< Strings to be displayed as menu items
};

