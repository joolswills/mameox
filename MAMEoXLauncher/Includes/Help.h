/**
	* \file			Help.h
	* \brief		Helper class which display the help.txt
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include "XBFont.h"
#include "MAMEoX.h"
#include "Gamepad.h"
#include <vector>

//= D E F I N E S ======================================================

#define HELPFILENAME		  "D:\\Media\\help.txt"

//= C L A S S E S ======================================================

/**
	* \class		CHelp
	* \brief		The ROM listing class
	*/
class CHelp
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CHelp( LPDIRECT3DDEVICE8	displayDevice, CXBFont &font ) :
		m_displayDevice( displayDevice ),
		m_font( font ),
		m_helpCursorPosition( 0.0f ),
		m_dpadCursorDelay( 0.0f ),
		m_helpCursorSpeedBandTimeout( 0.0f ),
		m_helpPageOffset( 0.0f )
	{
	}

	BOOL FileGets( HANDLE file, char *buffer, UINT32 length );

		//------------------------------------------------------------
		// LoadHelpFile
		//! \brief		Load a help file
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - Help file could be loaded
		//------------------------------------------------------------
	BOOL LoadHelpFile();

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The CGamepad containing the 
		//!                current state of the user's joystick
		//------------------------------------------------------------
	void MoveCursor( CGamepad &gp );

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the current list to the user
    //!
    //! \param    opaque - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL opaque = TRUE, BOOL flipOnCompletion = TRUE );

  void SetCursorPosition( FLOAT pageOffset, FLOAT cursorPos ) {
    m_helpPageOffset = pageOffset;
    m_helpCursorPosition = cursorPos;
  }

protected:

	FLOAT									m_helpPageOffset;				//!< Offset of the first item from the top of the list
	FLOAT									m_helpCursorPosition;		//!< Cursor position within the current list page
	FLOAT									m_dpadCursorDelay;

	FLOAT									m_helpCursorSpeedBandTimeout;

	LPDIRECT3DDEVICE8			m_displayDevice;
	CXBFont								&m_font;

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<std::string>		m_Help;
};
