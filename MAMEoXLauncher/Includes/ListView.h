/**
	* \file			ListView.h
	* \brief		Base class for all list views
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>

#include "XBFont.h"
#include "MAMEoX.h"
#include "InputManager.h"
#include "FontSet.h"


//= D E F I N E S ======================================================


//= C L A S S E S ======================================================

/**
	* \class		CListView
	* \brief		The list base class
	*/
class CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CListView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, LPDIRECT3DTEXTURE8 backdropTexture ) :
		m_displayDevice( displayDevice ),
		m_fontSet( fontSet ),
    m_backdropTexture( backdropTexture ),
		m_cursorPosition( 0.0f ),
		m_dpadCursorDelay( 0.0f ),
		m_pageOffset( 0.0f ),
    m_numLinesInList( 0 ),
    m_maxPageSize( 10 )
	{
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
	virtual void MoveCursor( CInputManager &inputManager, BOOL useSpeedBanding = FALSE );

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the list to the screen
    //!
    //! \param    clearScreen - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE ) = 0;

		//------------------------------------------------------------
		// DrawToTexture
		//! \brief		Render the list to the passed texture
    //!
    //! \param    targetTexture - The texture to render to
		//------------------------------------------------------------
  virtual void DrawToTexture( LPDIRECT3DTEXTURE8 targetTexture );

		//------------------------------------------------------------
		// SetCursorPosition
		//! \brief		Set the current cursor position
    //!
    //! \param    pageOffset - Page offset
    //! \param    cursorPos - Cursor position within the page
		//------------------------------------------------------------
  void SetCursorPosition( FLOAT pageOffset, FLOAT cursorPos ) {
    m_pageOffset = pageOffset;
    m_cursorPosition = cursorPos;
  }

		//------------------------------------------------------------
		// GetCursorPosition
		//! \brief		Get the current cursor position
    //!
    //! \param    pageOffset - [OUT] variable to receive the page offset
    //! \param    cursorPos - [OUT] variable to receive the cursor position
		//------------------------------------------------------------
  void GetCursorPosition( FLOAT *pageOffset, FLOAT *cursorPos ) const {
    if( pageOffset )
      *pageOffset = m_pageOffset;

    if( cursorPos )
      *cursorPos = m_cursorPosition;
  }

protected:

		//------------------------------------------------------------
		// GetCurrentPageSize
		//! \brief		Get the number of items in the current page
    //!
    //! \return   DWORD - Number of items in the current page
		//------------------------------------------------------------
  DWORD GetCurrentPageSize( void ) {
	  DWORD pageSize = (m_numLinesInList < m_maxPageSize ? m_numLinesInList : m_maxPageSize);
	  ULONG absListIDX = (ULONG)m_pageOffset;
	  if( absListIDX > (m_numLinesInList - pageSize) )
	  {
			  // The current page offset is invalid (due to list shrinkage), reset it and
			  //  set the cursor position to the last item in the list
      absListIDX = (m_numLinesInList - pageSize);
      m_pageOffset = (FLOAT)absListIDX;
		  m_cursorPosition = (FLOAT)(pageSize - 1);
	  }

    return pageSize;
  }

		//------------------------------------------------------------
		// RenderBackdrop
		//! \brief		Render the backdrop texture to m_displayDevice
		//------------------------------------------------------------
	virtual void RenderBackdrop( void );

		//------------------------------------------------------------
		// RenderToTextureStart
		//! \brief		Start rendering to the passed texture
		//------------------------------------------------------------
  virtual BOOL RenderToTextureStart( LPDIRECT3DTEXTURE8 targetTexture );

		//------------------------------------------------------------
		// RenderToTextureStop
		//! \brief		Stop rendering to a texture
		//------------------------------------------------------------
  virtual void RenderToTextureStop( void );

	LPDIRECT3DDEVICE8			    m_displayDevice;            //!<  Display device to use in rendering
  CFontSet                  &m_fontSet;                 //!<  Set of available fonts
  LPDIRECT3DTEXTURE8        m_backdropTexture;          //!<  Texture to render as the backdrop


   
  DWORD                     m_numLinesInList;           //!<  The total number of items in the list
  DWORD                     m_maxPageSize;              //!<  The maximum number of items to put on a page

	FLOAT                     m_pageOffset;               //!< Offset of the first item in the page
  FLOAT                     m_cursorPosition;           //!< Cursor position within the current page
  FLOAT                     m_dpadCursorDelay;          //!< Timeout between valid dpad readings
  FLOAT                     m_timeElapsed;              //!< Time elapsed between MoveCursor calls

	FLOAT									    m_cursorSpeedBandTimeout;   //!< Timeout value for determining the current speed band

  RenderToTextureToken_t    m_renderToTextureToken;     //!< Token for texture rendering
};
