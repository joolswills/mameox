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
#include "MAMEoX.h"
#include "Gamepad.h"
#include <vector>

//= D E F I N E S ======================================================
#define NUM_SUPERSCROLL_CHARS           27 //!< Each letter + #
#define INVALID_SUPERSCROLL_JUMP_IDX    0xFFFFFFFF
#define INVALID_ROM_INDEX               0xFFFFFFFF

#define CURRENTROMLIST()                (m_allowClones ? m_ROMListWithClones : m_ROMListNoClones)
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
	CROMList( LPDIRECT3DDEVICE8	displayDevice, CXBFont &font, MAMEDriverData_t *drivers, UINT32 numDrivers ) :
		m_displayDevice( displayDevice ),
		m_font( font ),
		m_gameListCursorPosition( 0.0f ),
		m_dpadCursorDelay( 0.0f ),
		m_gameListCursorSpeedBandTimeout( 0.0f ),
		m_gameListPageOffset( 0.0f ),
    m_superscrollMode( FALSE ),
		m_allowClones( TRUE ),
		m_additionalinfo( TRUE ),
    m_superscrollCharacterIdx( 0 ),
    m_driverInfoList( drivers ),
    m_numDrivers( numDrivers ),
    m_gameSelected( FALSE ),
    m_shouldGenerateROMList( FALSE )
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
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No ROM list could be generated
		//------------------------------------------------------------
	BOOL GenerateROMList( void );

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The CGamepad instance associated with
		//!                the controlling gamepad
		//------------------------------------------------------------
	void MoveCursor( CGamepad	&gp );

		//------------------------------------------------------------
		// GetHeaderLine
		//! \brief		Return a headerline with  specific size
    //!
    //! \param    size - Size of the header line
    //! \return   Return the headerline
		//------------------------------------------------------------
	virtual WCHAR *GetHeaderLine( FLOAT width );

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the current list to the user
    //!
    //! \param    opaque - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL opaque = TRUE, BOOL flipOnCompletion = TRUE );

		//------------------------------------------------------------
		// GetCurrentGameIndex
		//! \brief		Returns the index into the global drivers array
		//!            of the currently selected item.
		//------------------------------------------------------------
	UINT32 GetCurrentGameIndex( void ) { 
    if( (ULONG)m_gameListPageOffset + (ULONG)m_gameListCursorPosition < CURRENTROMLIST().size() )
		  return CURRENTROMLIST()[ (ULONG)m_gameListPageOffset + (ULONG)m_gameListCursorPosition ]; 
    return INVALID_ROM_INDEX;
	}

		//------------------------------------------------------------
		// RemoveCurrentGameIndex
		//! \brief		Removes the currently selected index from the listing
		//------------------------------------------------------------
	void RemoveCurrentGameIndex( void );
	
		//------------------------------------------------------------
		// GetCursorPosition
		//! \brief		Retrieves all cursor position information
    //!
    //! \param    pageOffset - [OUT] Pointer to var to receive the page offset
    //! \param    cursorPos - [OUT] Pointer to var to receive the cursor position
    //! \param    superscrollIndex - [OUT] Pointer to var to receive the superscroll index
		//------------------------------------------------------------
  void GetCursorPosition( FLOAT *pageOffset, FLOAT *cursorPos, UINT32 *superscrollIndex ) const {
    *pageOffset = m_gameListPageOffset;
    *cursorPos = m_gameListCursorPosition;
    *superscrollIndex = m_superscrollCharacterIdx;
  }

		//------------------------------------------------------------
		// SetCursorPosition
		//! \brief		Sets all cursor position information
    //!
    //! \param    pageOffset - page offset
    //! \param    cursorPos - cursor position
    //! \param    superscrollIndex - superscroll index
		//------------------------------------------------------------
  void SetCursorPosition( FLOAT pageOffset, FLOAT cursorPos, UINT32 superscrollIndex ) {
    m_gameListPageOffset = pageOffset;
    m_gameListCursorPosition = cursorPos;
    m_superscrollCharacterIdx = superscrollIndex;
  }

  BOOL IsGameSelected( void ) { return m_gameSelected; }
  BOOL ShouldGenerateROMList( void ) { return m_shouldGenerateROMList; }

protected:
    // Helper functions to save/load the rom list, with integrity checks
  BOOL SaveROMListFile( void );
  BOOL LoadROMListFile( void );


		//------------------------------------------------------------
		// DrawZipData
		//! \brief		Present some indication that a new zip file has
		//!            been found to the user
		//!
		//! \param		fileName - The filename of the zip file
		//! \param		index - The 0 based index of the zip file
		//------------------------------------------------------------
	void DrawZipData( const char *fileName, DWORD index );


		//------------------------------------------------------------
		// DrawZipCheckProgress
		//! \brief		Present some indication that a zip file is
		//!            being checked against known zips (this
    //!            operation is bounded, unlike DrawZipData,
    //!            so we can show a real progressbar)
		//!
		//! \param		index - The 0 based index of the zip file
		//------------------------------------------------------------
	void DrawZipCheckProgress( DWORD index );


    // Cursor movement helper functions
  void SuperScrollModeMoveCursor( CGamepad &gp, FLOAT elapsedTime );
  void NormalModeMoveCursor( CGamepad &gp, FLOAT elapsedTime );
  void GenerateSuperscrollJumpTable( void );

  UINT32                m_numDrivers;               //!<  The total number of drivers supported by the core
  MAMEDriverData_t      *m_driverInfoList;          //!<  Drivers supported by the MAME core
  BOOL                  m_superscrollMode;          //!<  Whether or not to display in superscroll mode
	BOOL									m_allowClones;							//!<	Whether or not to allow clones
	BOOL									m_additionalinfo;						//!<	Game list with additional informations
  UINT32                m_superscrollCharacterIdx;  //!<  Character for superscroll mode
  UINT32                m_superscrollJumpTable[NUM_SUPERSCROLL_CHARS]; //!<  Jump indices for superscroll

	FLOAT									m_gameListPageOffset;				//!< Offset of the first item from the top of the list
	FLOAT									m_gameListCursorPosition;		//!< Cursor position within the current list page
	FLOAT									m_dpadCursorDelay;

	FLOAT									m_gameListCursorSpeedBandTimeout;


	LPDIRECT3DDEVICE8			m_displayDevice;
	CXBFont								&m_font;

  BOOL                  m_gameSelected;             //!<  Whether or not the user has selected a game
  BOOL                  m_shouldGenerateROMList;    //!<  Whether or not the main() funct should call GenerateROMList for us (to render directly to the screen)

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<UINT32>		m_ROMListWithClones;		
  std::vector<UINT32>   m_ROMListNoClones;
};




