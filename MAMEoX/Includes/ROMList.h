/**
	* \file			ROMList.h
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include "XBFont.h"
#include <vector>

//= D E F I N E S ======================================================
#define NUM_SUPERSCROLL_CHARS           27 //!< Each letter + #
#define INVALID_SUPERSCROLL_JUMP_IDX    0xFFFFFFFF
#define INVALID_ROM_INDEX               0xFFFFFFFF
/*
typedef enum SortMethod {
	BY_NAME = 0,
	BY_DATE,
	BY_GENRE
} SortMethod;
*/



//= C L A S S E S ======================================================

/**
	* \class		CROMList
	* \brief		The ROM listing class
	*/
class CROMList
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CROMList( LPDIRECT3DDEVICE8	displayDevice, CXBFont &font ) :
		m_displayDevice( displayDevice ),
		m_font( font ),
		m_gameListCursorPosition( 0.0f ),
		m_dpadCursorDelay( 0.0f ),
		m_gameListCursorSpeedBandTimeout( 0.0f ),
		m_gameListPageOffset( 0.0f ),
    m_superscrollMode( FALSE ),
    m_superscrollCharacterIdx( 0 )
	{
    for( UINT32 i = 0; i < NUM_SUPERSCROLL_CHARS; ++i )
      m_superscrollJumpTable[i] = INVALID_SUPERSCROLL_JUMP_IDX;
	}

		//------------------------------------------------------------
		// LoadROMList
		//! \brief		Load (or generate and save) a ROM list file
		//!
		//! \param		bGenerate - Whether or not to generate a ROM list
		//!                       if no list exists
		//! \param		allowClones - Whether or not to allow clones in the
		//!                          listing
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No ROM list could be loaded or generated
		//------------------------------------------------------------
	BOOL LoadROMList( BOOL bGenerate = FALSE, BOOL allowClones = TRUE );

		//------------------------------------------------------------
		// GenerateROMList
		//! \brief		Generate and store a ROM list file
		//!
		//! \param		allowClones - Whether or not to allow clones in the
		//!                          listing
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No ROM list could be generated
		//------------------------------------------------------------
	BOOL GenerateROMList( BOOL allowClones = TRUE );

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The XINPUT_GAMEPAD struct containing the 
		//!                current state of the user's joystick
		//------------------------------------------------------------
	void MoveCursor( const XINPUT_GAMEPAD	&gp );


		//------------------------------------------------------------
		// Draw
		//! \brief		Render the current list to the user
		//------------------------------------------------------------
	virtual void Draw( void );

		//------------------------------------------------------------
		// DrawZipData
		//! \brief		Present some indication that a new zip file has
		//!            been found to the user
		//!
		//! \param		fileName - The filename of the zip file
		//! \param		index - The 0 based index of the zip file
		//------------------------------------------------------------
	virtual void DrawZipData( const char *fileName, DWORD index );
	

		//------------------------------------------------------------
		// GetCurrentGameIndex
		//! \brief		Returns the index into the global drivers array
		//!            of the currently selected item.
		//!
		//! \param		fileName - The filename of the zip file
		//! \param		index - The 0 based index of the zip file
		//------------------------------------------------------------
	UINT32 GetCurrentGameIndex( void ) { 
    if( m_ROMList.size() )
		  return m_ROMList[ (ULONG)m_gameListPageOffset + (ULONG)m_gameListCursorPosition]; 
    return INVALID_ROM_INDEX;
	}

		//------------------------------------------------------------
		// RemoveCurrentGameIndex
		//! \brief		Removes the currently selected index from the listing
		//------------------------------------------------------------
	void RemoveCurrentGameIndex( void );
	
protected:

    // Cursor movement helper functions
  void SuperScrollModeMoveCursor( const XINPUT_GAMEPAD	&gp, FLOAT elapsedTime );
  void NormalModeMoveCursor( const XINPUT_GAMEPAD &gp, FLOAT elapsedTime );
  void GenerateSuperscrollJumpTable( void );

  BOOL                  m_superscrollMode;          //!<  Whether or not to display in superscroll mode
  UINT32                m_superscrollCharacterIdx;  //!<  Character for superscroll mode
  UINT32                m_superscrollJumpTable[NUM_SUPERSCROLL_CHARS]; //!<  Jump indices for superscroll

	FLOAT									m_gameListPageOffset;				//!< Offset of the first item from the top of the list
	FLOAT									m_gameListCursorPosition;		//!< Cursor position within the current list page
	FLOAT									m_dpadCursorDelay;

	FLOAT									m_gameListCursorSpeedBandTimeout;


	LPDIRECT3DDEVICE8			m_displayDevice;
	CXBFont								&m_font;

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<UINT32>		m_ROMList;		
};




