/**
	* \file			BasePopupView.h
	* \brief		Base class for popups (VK/Start Menu)
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"

#include "BaseView.h"
#include "StdString.h"
#include "TextureSet.h"

//= D E F I N E S ======================================================
typedef enum MenuState
{
  MENU_INPROGRESS = 0x00,
  MENU_ACCEPTED,
  MENU_CANCELLED
} MenuState;

//= C L A S S E S ======================================================

/**
	* \class		CBasePopupView
	* \brief		The base class for "popup" style views
	*/
class CBasePopupView : public CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CBasePopupView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, CTextureSet &textureSet ) :
      CBaseView( displayDevice, fontSet, NULL ),
      m_textureSet( textureSet ) {
    Reset();
  }

	//------------------------------------------------------------
	// SetTitle
	//! \brief		Set the title to be used for the popup
	//------------------------------------------------------------
  void SetTitle( const CStdString &str ) { m_title = str; }

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset (most) member vars to defaults
		//------------------------------------------------------------
  virtual void Reset( void ) {
    m_inputState = MENU_INPROGRESS;
  }
  
  BOOL IsInputFinished( void ) const { return m_inputState != MENU_INPROGRESS; }
  BOOL IsInputAccepted( void ) const { return m_inputState == MENU_ACCEPTED; }
  BOOL IsInputCancelled( void ) const { return m_inputState == MENU_CANCELLED; }

		//------------------------------------------------------------
		// GetNumBodyLines
		//! \brief		Returns the number of text lines in the body
    //!           of the popup
		//------------------------------------------------------------
  virtual UINT32 GetNumBodyLines( void ) const = 0;

protected:

		//------------------------------------------------------------
		// RenderBackdrop
		//! \brief		Render the backdrop texture to m_displayDevice
    //!
    //! \param    textureHeight - Height of a line of the font that will
    //!                           be used to display items in the body
    //!                           of the popup (used to dynamically resize
    //!                           the backdrop)
		//------------------------------------------------------------
	virtual void RenderBackdrop( FLOAT fontHeight );

  RECT                      m_titleArea;      //!<  Rect enclosing the are to be used for title rendering
  RECT                      m_bodyArea;       //!<  Rect enclosing the area to be used for the body text

  CTextureSet               &m_textureSet;
  MenuState                 m_inputState;     //!<  Input state (accepted, cancelled, in progress)
  CStdString                m_title;          //!<  String to render as the title bar
};

