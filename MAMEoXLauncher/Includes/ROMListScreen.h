/**
	* \file			ROMList.h
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "BaseMenuView.h"
#include "Skin.h"
#include "MAMEoX.h"

#include <vector>

//= D E F I N E S ======================================================
#define INVALID_ROM_INDEX               0xFFFFFFFF

  //! \enum   ROMStatus
  //! \brief  Enumerates the various working states for the ROMs
typedef enum ROMStatus
{
  STATUS_UNKNOWN = 0x00,
  STATUS_WORKING,
  STATUS_SLOW,
  STATUS_CRASH,
  STATUS_OUT_OF_MEMORY,
  STATUS_GENERAL_NONWORKING,
  STATUS_MAX    //!< Invalid "last item" tag
} ROMStatus;

//= G L O B A L = V A R S ==============================================


//= C L A S S E S ======================================================

/**
	* \class		CROMListScreen
	* \brief		The ROM listing class
	*/
class CROMListScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CROMListScreen( LPDIRECT3DDEVICE8	displayDevice, 
            CFontSet &fontSet, 
            MAMEDriverData_t *drivers, 
            UINT32 numDrivers ) :
    CListView( displayDevice, fontSet, ASSET_LIST_BACKDROP ),
    m_superscrollMode( FALSE ),
    m_gameSelected( FALSE ),
    m_shouldGenerateROMList( FALSE ),
    m_currentSuperscrollIndex( 0 ),
    m_favoriteStatusChangeDelay( 0.0f ),
    m_screenshotTexture( NULL )
	{
    m_options.m_displayMode = DM_VERBOSELIST;
    m_options.m_sortMode = SM_BYNAME;
    m_options.m_showROMStatus = TRUE;
    m_options.m_filterMode = FM_NONE;
    m_options.m_hideFiltered = FALSE;
//    m_options.m_numPlayersFilter = 0;

    if( !m_numDrivers )
      m_numDrivers = numDrivers;
    if( !m_driverInfoList )
      m_driverInfoList = drivers;
	}

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CROMListScreen( void ) {
    SAFE_RELEASE( m_screenshotTexture );
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
		// RefreshROMStatus
		//! \brief		Reload the romstatus.xml file
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No ROM list could be generated
		//------------------------------------------------------------
  BOOL RefreshROMStatus( void );

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
    if( GetAbsoluteCursorPosition() < m_currentSortedList.size() )
		  return m_currentSortedList[GetAbsoluteCursorPosition()]; 
    return INVALID_ROM_INDEX;
	}

		//------------------------------------------------------------
		// RemoveCurrentGameIndex
		//! \brief		Removes the currently selected index from the listing
    //!
    //! \return   BOOL - TRUE if the item was removed and the cache file updated
    //!                  FALSE otherwise
		//------------------------------------------------------------
	BOOL RemoveCurrentGameIndex( void );	
  BOOL MoveCurrentGameToBackupDir( void );

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
    m_currentSuperscrollIndex = superscrollIndex;
    if( m_currentSuperscrollIndex >= m_superscrollJumpTable.size() )
      m_currentSuperscrollIndex = 0;
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
    assert( pageOffset && cursorPos && superscrollIndex );
    *pageOffset = m_pageOffset;
    *cursorPos = m_cursorPosition;
    *superscrollIndex = m_currentSuperscrollIndex;
  }


  BOOL IsGameSelected( void ) { return m_gameSelected; }
  BOOL ShouldGenerateROMList( void ) { return m_shouldGenerateROMList; }
  BOOL ShouldRefreshROMStatus( void ) { return m_shouldGenerateROMList; }

		//------------------------------------------------------------
		// SaveROMMetadataFile
		//! \brief		Save ROM metadata to a file, should be done before exiting the ROM list
		//------------------------------------------------------------
  BOOL SaveROMMetadataFile( void );

		//------------------------------------------------------------
		// ImportCatverINI
		//! \brief		Load the catver.ini file and merge it into
    //!           the ROM metadata
		//------------------------------------------------------------
  BOOL ImportCatverINI( void );


    //! Vector of ROMStatus types marking the status of each entry in
    //! the MAME driver array. Therefore, to access this, take the
    //! UINT32 index for any given position in the currentSortedList
    //! and use that number as the index in the status array
  static std::vector<ROMStatus>                 m_ROMStatus;                //!<  Working status for a given ROM
  static std::vector<MAMEoXDriverMetadata_t>    m_driverMetadata;           //!<  Metadata for drivers (num times played, favorite status)
  static MAMEDriverData_t                       *m_driverInfoList;          //!<  Drivers supported by the MAME core
  static UINT32                                 m_numDrivers;               //!<  The total number of drivers supported by the core

protected:

  BOOL LoadScreenshotFile( UINT32 index );

    // Set the cursorPosition and pageOffset such that
    // the cursor is displayed at the passed position
  void SetAbsoluteCursorPosition( UINT32 pos );
  UINT32 GetAbsoluteCursorPosition( void ) const { return (UINT32)m_pageOffset + (UINT32)m_cursorPosition; }


    // Helper functions to render ROM data
  void DrawVerboseList( void );
  void DrawSimpleList( void );
  void DrawDetailedList( void );

    // Helper functions to save/load the rom list, with integrity checks
  BOOL SaveROMListFile( void );
  BOOL LoadROMListFile( void );
  BOOL LoadROMMetadataFile( void );
  BOOL UpdateROMMetadataFile( void );   // Convert the metadata file from an older format to the newest

		//------------------------------------------------------------
		// LoadROMStatusFile
		//! \brief		Load the XML ROM status file
		//!
		//! \return		BOOL - TRUE on success, FALSE on failure
		//------------------------------------------------------------
  BOOL LoadROMStatusFile( void );

  void UpdateSortedList( void );      // Resort the current list
  void UpdateFilteredList( void );    // Refilter the filtered list

    // Functions to set up the superscroll jump table
  void GenerateSuperscrollJumpTable( void );
  void GenerateYearSuperscrollJumpTable( void );
  void GenerateNumericSuperscrollJumpTable( void );
  void GenerateAlphanumericSuperscrollJumpTable( void );

		//------------------------------------------------------------
		// GetFriendlySuperscrollIndexString
		//! \brief		Retrieve the friendly display name for the
    //!           given superscroll index
		//!
		//! \param		ret - Buffer into which the string should be placed
    //! \param    superscrollTableIndex - Index in the jump table to query
		//------------------------------------------------------------
  void GetFriendlySuperscrollIndexStringForJumpTableIndex( CStdString *ret, UINT32 superscrollTableIndex );


		//------------------------------------------------------------
		// GetFriendlySuperscrollIndexStringForROM
		//! \brief		Retrieve the friendly display name for the
    //!           given ROM index
		//!
		//! \param		ret - Buffer into which the string should be placed
    //! \param    romIndex - Index in the m_currentSortedList to query
		//------------------------------------------------------------
  void GetFriendlySuperscrollIndexStringForROM( CStdString *ret, UINT32 romIndex );

    // Helper for ROM finding
  BOOL Helper_GenerateROMList( CStdString &path );
  BOOL Helper_MoveCurrentGameToBackupDir( CStdString &path );

    // Cursor movement helper functions
  void SuperScrollModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );
  void NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );

  BOOL                  m_superscrollMode;          //!<  Whether or not to display in superscroll mode
  ROMListOptions_t      m_options;                  //!<  Persistent options

  BOOL                  m_gameSelected;             //!<  Whether or not the user has selected a game
  BOOL                  m_shouldGenerateROMList;    //!<  Whether or not the main() funct should call GenerateROMList for us (to render directly to the screen)

  LPDIRECT3DTEXTURE8    m_screenshotTexture;          //!<  Texture for the currently loaded screenshot image
  RECT                  m_screenshotRect;             //!<  Rect containing the size of the screenshot

  FLOAT                 m_favoriteStatusChangeDelay;  //!<  Delay timer for favorite status changes

		//! Vector of integers into the MAME driver array
		//!  defining the set of available ROMs
	std::vector<UINT32>		  m_ROMListFull;
  std::vector<UINT32>     m_ROMListFiltered;
  std::vector<UINT32>     m_currentSortedList;

    //! Set of superscroll "Jump destinations"
  std::vector<UINT32>           m_superscrollJumpTable;
  UINT32                        m_currentSuperscrollIndex;  //!<  Current key position for superscroll mode
};




