/**
	* \file			SkinChooserScreen.h
	* \brief		Helper class which takes care of displaying the list
	*           of available skins
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "BaseMenuView.h"

#include "SkinChooser.h"
#include "SkinResource.h"
#include "MAMEoX.h"

#include <vector>

//= D E F I N E S ======================================================
#define INVALID_INDEX               0xFFFFFFFF

  // Layout for the list rendering
#undef LISTPOS_LEFT
#undef LISTPOS_TOP
#undef LISTPOS_RIGHT
#undef LISTPOS_BOTTOM
#define LISTPOS_LEFT    31
#define LISTPOS_TOP     95
#define LISTPOS_RIGHT   608
#define LISTPOS_BOTTOM  451

//= G L O B A L = V A R S ==============================================


//= C L A S S E S ======================================================
/**
	* \class		CSkinChooserScreen
	* \brief		Helper class which takes care of displaying the list
	*           of available skins
	*/
class CSkinChooserScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CSkinChooserScreen( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, CSkinChooser &skinChooser ) :
    CListView( displayDevice, fontSet, ASSET_LIST_BACKDROP ),
		m_skinChooser( skinChooser )
	{
		m_maxPageSize = 3;
		m_numLinesInList = m_skinChooser.m_skinResourceVector.size();


    RECT area = { LISTPOS_LEFT, LISTPOS_TOP, LISTPOS_RIGHT, LISTPOS_BOTTOM };
    m_menuRenderer = new CBaseMenuView( displayDevice, fontSet, area );
    assert( m_menuRenderer );		

		SetCursorToSelectedSkin();
	}

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CSkinChooserScreen( void ) {
    delete m_menuRenderer;
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
		//! \brief		Render the current list to the user
    //!
    //! \param    opaque - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL opaque = TRUE, BOOL flipOnCompletion = TRUE );


		//------------------------------------------------------------
		// SetOptions
		//! \brief		Set the display options
		//!
		//! \param		opts - SkinOptions_t struct containing new options
		//------------------------------------------------------------
  void SetOptions( const SkinOptions_t &opts ) { 
    m_skinChooser.m_options = opts;
		SetCursorToSelectedSkin();
  }

		//------------------------------------------------------------
		// GetOptions
		//! \brief		Get the current display options
		//!
		//! \return		const SkinOptions_t & - SkinOptions_t struct containing options
		//------------------------------------------------------------
  const SkinOptions_t &GetOptions( void ) { return m_skinChooser.m_options; }

protected:

	void SetAbsoluteCursorPosition( UINT32 pos );
  UINT32 GetAbsoluteCursorPosition( void ) const { return (UINT32)m_pageOffset + (UINT32)m_cursorPosition; }

     // Cursor movement helper functions
  void NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );
	void DrawSkinList( void );

	void SetCursorToSelectedSkin( void );	

  CBaseMenuView										*m_menuRenderer;    //!<  Resizable menu renderer

	CSkinChooser										&m_skinChooser;
};


