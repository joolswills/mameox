/**
	* \file			StartMenu.h
	* \brief		Modal start menu
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <vector>

#include "BaseView.h"
#include "StdString.h"

//= D E F I N E S ======================================================
typedef enum MenuState
{
  MENU_INPROGRESS = 0x00,
  MENU_ACCEPTED,
  MENU_CANCELLED
} MenuState;

//= C L A S S E S ======================================================

/**
	* \class		CStartMenu
	* \brief		The generic modal menu window
	*/
class CStartMenu : public CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CStartMenu( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, LPDIRECT3DTEXTURE8 backdropTexture ) :
    CBaseView( displayDevice, fontSet, backdropTexture ) {
    Reset();
  }

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset (most) member vars to defaults to allow
    //!           the VK to be reused for a different input session
		//------------------------------------------------------------
  void Reset( void ) {
		m_cursorPosition = 0;
 		m_dpadCursorDelay = 0.0f;
    m_buttonDelay = 0.0f;
    m_inputState = MENU_INPROGRESS;
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

  
  BOOL IsInputFinished( void ) const { return m_inputState != MENU_INPROGRESS; }
  BOOL IsInputAccepted( void ) const { return m_inputState == MENU_ACCEPTED; }
  BOOL IsInputCancelled( void ) const { return m_inputState == MENU_CANCELLED; }

protected:

	FLOAT									    m_dpadCursorDelay;
	FLOAT									    m_buttonDelay;

  UINT32                    m_cursorPosition;

  MenuState                 m_inputState;     //!< Input state (accepted, cancelled, in progress)
};
