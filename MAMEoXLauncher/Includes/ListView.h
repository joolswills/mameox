/**
	* \file			ListView.h
	* \brief		Base class for all list views
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>

#include "BaseView.h"


//= D E F I N E S ======================================================


//= C L A S S E S ======================================================

/**
	* \class		CListView
	* \brief		The list base class
	*/
class CListView : public CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CListView( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, LPDIRECT3DTEXTURE8 backdropTexture ) :
    CBaseView( displayDevice, fontSet, backdropTexture ),
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
   
  DWORD                     m_numLinesInList;           //!<  The total number of items in the list
  DWORD                     m_maxPageSize;              //!<  The maximum number of items to put on a page

	FLOAT                     m_pageOffset;               //!< Offset of the first item in the page
  FLOAT                     m_cursorPosition;           //!< Cursor position within the current page
  FLOAT                     m_dpadCursorDelay;          //!< Timeout between valid dpad readings
  FLOAT                     m_timeElapsed;              //!< Time elapsed between MoveCursor calls

	FLOAT									    m_cursorSpeedBandTimeout;   //!< Timeout value for determining the current speed band
};


