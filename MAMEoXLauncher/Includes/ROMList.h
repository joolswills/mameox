/**
	* \file			ROMList.h
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "TextureSet.h"
#include "MAMEoX.h"
#include <vector>

//= D E F I N E S ======================================================
#define NUM_SUPERSCROLL_CHARS           27 //!< Each letter + #
#define INVALID_SUPERSCROLL_JUMP_IDX    0xFFFFFFFF
#define INVALID_ROM_INDEX               0xFFFFFFFF

//= G L O B A L = V A R S ==============================================


//= C L A S S E S ======================================================

/**
	* \class		CROMList
	* \brief		The ROM listing class
	*/
class CROMList : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CROMList( LPDIRECT3DDEVICE8	displayDevice, 
            CFontSet &fontSet, 
            CTextureSet &textureSet,
            MAMEDriverData_t *drivers, 
            UINT32 numDrivers ) :
    CListView( displayDevice, fontSet, NULL ),
    m_textureSet( textureSet ),
    m_superscrollMode( FALSE ),
    m_superscrollCharacterIdx( 0 ),
    m_driverInfoList( drivers ),
    m_numDrivers( numDrivers ),
    m_gameSelected( FALSE ),
    m_shouldGenerateROMList( FALSE )
	{
    for( UINT32 i = 0; i < NUM_SUPERSCROLL_CHARS; ++i )
      m_superscrollJumpTable[i] = INVALID_SUPERSCROLL_JUMP_IDX;

    m_options.m_displayClones = TRUE;
    m_options.m_verboseMode = TRUE;
    m_options.m_sortMode = SM_BYNAME;
	}

		//------------------------------------------------------------
		// SetOptions
		//! \brief		Set the display options
		//!
		//! \param		opts - ROMListOptions_t struct containing new options
		//------------------------------------------------------------
  void SetOptions( const ROMListOptions_t &opts ) { 
    m_options = opts;
    UpdateSortedList();
  }

		//------------------------------------------------------------
		// GetOptions
		//! \brief		Get the current display options
		//!
		//! \return		const ROMListOptions_t & - ROMListOptions_t struct containing options
		//------------------------------------------------------------
  const ROMListOptions_t &GetOptions( void ) { return m_options; }

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
		//! \param		gp - The CInputManager containing the
		//!                current state of the user's joystick(s)
    //! \param    useSpeedBanding - Whether or not to enable
    //!              "speed bands" (cursor acceleration)
		//------------------------------------------------------------
	virtual void MoveCursor( CInputManager &inputManager, BOOL useSpeedBanding = FALSE );

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
    if( (ULONG)m_pageOffset + (ULONG)m_cursorPosition < m_currentSortedList.size() )
		  return m_currentSortedList[ (ULONG)m_pageOffset + (ULONG)m_cursorPosition ]; 
    return INVALID_ROM_INDEX;
	}

		//------------------------------------------------------------
		// RemoveCurrentGameIndex
		//! \brief		Removes the currently selected index from the listing
		//------------------------------------------------------------
	void RemoveCurrentGameIndex( void );
	
		//------------------------------------------------------------
		// SetCursorPosition
		//! \brief		Sets all cursor position information
    //!
    //! \param    pageOffset - page offset
    //! \param    cursorPos - cursor position
    //! \param    superscrollIndex - superscroll index
		//------------------------------------------------------------
  void SetCursorPosition( FLOAT pageOffset, FLOAT cursorPos, UINT32 superscrollIndex ) {
    m_pageOffset = pageOffset;
    m_cursorPosition = cursorPos;
    m_superscrollCharacterIdx = superscrollIndex;
  }

		//------------------------------------------------------------
		// GetCursorPosition
		//! \brief		Retrieves all cursor position information
    //!
    //! \param    pageOffset - [OUT] Pointer to var to receive the page offset
    //! \param    cursorPos - [OUT] Pointer to var to receive the cursor position
    //! \param    superscrollIndex - [OUT] Pointer to var to receive the superscroll index
		//------------------------------------------------------------
  void GetCursorPosition( FLOAT *pageOffset, FLOAT *cursorPos, UINT32 *superscrollIndex ) const {
    *pageOffset = m_pageOffset;
    *cursorPos = m_cursorPosition;
    *superscrollIndex = m_superscrollCharacterIdx;
  }


  BOOL IsGameSelected( void ) { return m_gameSelected; }
  BOOL ShouldGenerateROMList( void ) { return m_shouldGenerateROMList; }

protected:

    // Set the cursorPosition and pageOffset such that
    // the cursor is displayed at the passed position
  void SetAbsoluteCursorPosition( UINT32 pos );


    // Helper functions to save/load the rom list, with integrity checks
  BOOL SaveROMListFile( void );
  BOOL LoadROMListFile( void );

  void UpdateSortedList( void );
  void GenerateSuperscrollJumpTable( void );

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
  void SuperScrollModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );
  void NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );

  UINT32                m_numDrivers;               //!<  The total number of drivers supported by the core
  MAMEDriverData_t      *m_driverInfoList;          //!<  Drivers supported by the MAME core
  BOOL                  m_superscrollMode;          //!<  Whether or not to display in superscroll mode
  ROMListOptions_t      m_options;                  //!<  Persistent options
  UINT32                m_superscrollCharacterIdx;  //!<  Character for superscroll mode
  UINT32                m_superscrollJumpTable[NUM_SUPERSCROLL_CHARS]; //!<  Jump indices for superscroll

  BOOL                  m_gameSelected;             //!<  Whether or not the user has selected a game
  BOOL                  m_shouldGenerateROMList;    //!<  Whether or not the main() funct should call GenerateROMList for us (to render directly to the screen)

  CTextureSet           &m_textureSet;              //!<  The texture set object storing all the useable textures (and their info) for the game

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<UINT32>		m_ROMListWithClones;		
  std::vector<UINT32>   m_ROMListNoClones;

  std::vector<UINT32>   m_currentSortedList;
};




