/**
	* \file			SkinChooserScreen.h
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "BaseMenuView.h"

#include "SkinResource.h"
#include "TextureSet.h"
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
	* \brief		The skin chooser class
	*/
class CSkinChooserScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CSkinChooserScreen( LPDIRECT3DDEVICE8	displayDevice, 
														CFontSet &fontSet, 
														CTextureSet &textureSet ) :
    CListView( displayDevice, fontSet, textureSet.GetBasicBackdrop() ),
    m_textureSet( textureSet ),
		m_currentSkin( NULL )
	{
		m_maxPageSize = 3;
    RECT area = { LISTPOS_LEFT, LISTPOS_TOP, LISTPOS_RIGHT, LISTPOS_BOTTOM };
    m_menuRenderer = new CBaseMenuView( displayDevice, fontSet, textureSet, area );
    assert( m_menuRenderer );		
	}

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CSkinChooserScreen( void ) {
		std::vector<CSkinResource*>::iterator i = m_skinResourceVector.begin();
		for( ; i != m_skinResourceVector.end(); ++i )
			delete (*i);

    delete m_menuRenderer;
  }
		//------------------------------------------------------------
		// FindSkins
		//! \brief		Find any valid skins in the skin directory.
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No skins could be found.
		//------------------------------------------------------------
	BOOL FindSkins( void );

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
    m_options = opts;
		SetCursorToSelectedSkin();
  }

		//------------------------------------------------------------
		// GetOptions
		//! \brief		Get the current display options
		//!
		//! \return		const SkinOptions_t & - SkinOptions_t struct containing options
		//------------------------------------------------------------
  const SkinOptions_t &GetOptions( void ) { return m_options; }



		//------------------------------------------------------------
		// GetCurrentSkin
		//! \brief		Get the currently loaded skin
		//!
		//! \return		const CSkinResource & - The CSkinResource containing the
		//!																		currently selected skin.
		//------------------------------------------------------------
	const CSkinResource *GetCurrentSkin( void ) { return m_currentSkin; }

protected:

	void SetAbsoluteCursorPosition( UINT32 pos );
  UINT32 GetAbsoluteCursorPosition( void ) const { return (UINT32)m_pageOffset + (UINT32)m_cursorPosition; }

     // Cursor movement helper functions
  void NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime );
	void DrawSkinList( void );

	void SetCursorToSelectedSkin( void );	

  CTextureSet											&m_textureSet;      //!<  The texture set object storing all the useable textures (and their info) for the game
  CBaseMenuView										*m_menuRenderer;    //!<  Resizable menu renderer

	SkinOptions_t										m_options;

	CSkinResource										*m_currentSkin;			//!<	The currently loaded skin
	std::vector<CSkinResource*>			m_skinResourceVector;
};




