/**
	* \file			Help.h
	* \brief		Helper class which display the help.txt
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include <vector>

//= C L A S S E S ======================================================

/**
	* \class		CHelp
	* \brief		The help viewer class
	*/
class CHelp : public CListView
{
public:

	CHelp( LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet, LPDIRECT3DTEXTURE8 backdropTexture ) :
      CListView( displayDevice, fontSet, backdropTexture ) {
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

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<std::string>		m_Help;
};
