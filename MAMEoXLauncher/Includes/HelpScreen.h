/**
	* \file			HelpScreen.h
	* \brief		Helper class which display the help.txt
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <vector>

#include "ListView.h"
#include "BaseMenuView.h"
#include "TextureSet.h"
#include "StdString.h"


//= D E F I N E S ======================================================

  // Layout for the list rendering
#undef LISTPOS_LEFT
#undef LISTPOS_TOP
#undef LISTPOS_RIGHT
#undef LISTPOS_BOTTOM

#define LISTPOS_LEFT    31
#define LISTPOS_TOP     95
#define LISTPOS_RIGHT   608
#define LISTPOS_BOTTOM  451


//= C L A S S E S ======================================================

/**
	* \class		CHelpScreen
	* \brief		The help viewer class
	*/
class CHelpScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CHelpScreen( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, CTextureSet &textureSet ) :
      CListView( displayDevice, fontSet, textureSet.GetBasicBackdrop() ),
      m_textureSet( textureSet ) {
    RECT area = { LISTPOS_LEFT, LISTPOS_TOP, LISTPOS_RIGHT, LISTPOS_BOTTOM };
    m_menuRenderer = new CBaseMenuView( displayDevice, fontSet, textureSet, area );
    assert( m_menuRenderer );
  }

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CHelpScreen( void ) {
    delete m_menuRenderer;
  }

		//------------------------------------------------------------
		// LoadHelpFile
		//! \brief		Load a help file
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - Help file could be loaded
		//------------------------------------------------------------
	BOOL LoadHelpFile( void );

    // Declared in ListView.h
  virtual void MoveCursor( CInputManager &gp, BOOL unused = FALSE );
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE );

protected:

		//------------------------------------------------------------
		// FileGets
		//! \brief		Read a string from the help file
		//!
		//! \return		file - Handle to opened help file
		//! \retval		buffer - [OUT] buffer in which to put the string
		//! \retval		length - max number of characters to read
		//------------------------------------------------------------
	BOOL FileGets( HANDLE file, char *buffer, UINT32 length );

  CTextureSet                 &m_textureSet;
  CBaseMenuView               *m_menuRenderer;                //!<  Resizable menu renderer
	std::vector<CStdString>		  m_data;
};
