/**
	* \file			VirtualKeyboard.h
	* \brief		Virtual keyboard class
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include <vector>

#include "BasePopupMenuView.h"

//= D E F I N E S ======================================================

//= C L A S S E S ======================================================

/**
	* \class		CVirtualKeyboardView
	* \brief		The virtual keyboard class
	*/
class CVirtualKeyboardView : public CBasePopupView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CVirtualKeyboardView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, const RECT &area );

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset (most) member vars to defaults to allow
    //!           the VK to be reused for a different input session
		//------------------------------------------------------------
  virtual void Reset( void ) {
    CBasePopupView::Reset();
		m_cursorPositionX = m_cursorPositionY = 0;
    m_dataDrawStartPosition = 0;
		m_dpadCursorDelay = 0.0f;
    m_buttonDelay = 0.0f;
    m_minChars = 0;
    m_maxChars = 1024;
    m_data = "";

    m_maxDisplayableChars = ((m_titleArea.right - m_titleArea.left) - 12.0f) / m_fontSet.FixedWidthFont().GetTextWidth( L"W" );
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
  UINT32 GetNumBodyLines( void ) const { return 6; } // 4 lines of alphanumerics, 2 lines for OK/Cancel


  void SetData( const CStdString &data ) { 
    m_dataDrawStartPosition = 0;
    m_data = data; 
    if( m_data.size() > m_maxDisplayableChars )
      m_dataDrawStartPosition = (m_data.size() - (m_maxDisplayableChars - 1));
  }

  const CStdString &GetData( void ) const { return m_data; }

protected:
	FLOAT									    m_dpadCursorDelay;
	FLOAT									    m_buttonDelay;

  UINT32                    m_cursorPositionX;
  UINT32                    m_cursorPositionY;
  UINT32                    m_dataDrawStartPosition;  //!< Position to start rendering the data text from
  UINT32                    m_maxDisplayableChars;    //!< Maximum number of characters to display on the data line

  UINT32                    m_minChars;       //!< Minimum string size
  UINT32                    m_maxChars;       //!< Maximum string size

  CStdString                m_data;           //!< Text that has been entered
};
