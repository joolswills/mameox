/**
	* \file			Help.h
	* \brief		Helper class which display the help.txt
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <vector>

#include "ListView.h"
#include "TextureSet.h"
#include "StdString.h"

//= C L A S S E S ======================================================

/**
	* \class		CHelp
	* \brief		The help viewer class
	*/
class CHelp : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CHelp( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, CTextureSet &textureSet ) :
      CListView( displayDevice, fontSet, textureSet.GetBasicBackdrop() ),
      m_textureSet( textureSet ) {
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
	std::vector<CStdString>		  m_data;
};
