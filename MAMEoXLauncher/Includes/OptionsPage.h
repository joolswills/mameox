/**
	* \file			OptionsPage.h
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

#pragma once

#ifdef __cplusplus


//= I N C L U D E S ====================================================
#include <Xtl.h>
#include "XBFont.h"
#include "MAMEoX.h"
#include "Gamepad.h"

extern "C" {
#include "mame.h"
}

#include <vector>

//= D E F I N E S ======================================================
typedef enum PageID
{
  OPTPAGE_GENERAL = 0x00,
  OPTPAGE_SOUND,
  OPTPAGE_VIDEO,
  OPTPAGE_VECTOR,
  OPTPAGE_NETWORK,
  OPTPAGE_DIRECTORIES,
  OPTPAGE_LAST
} PageID;

//= S T R U C T U R E S ================================================
class COptionsPage;

typedef struct optionsPageData_t
{
  WCHAR   m_title[64];
  void    (*m_drawFunct)( COptionsPage * );
  void    (*m_changeFunct)( COptionsPage *, BOOL );
  UINT32  m_numItems;
} optionsPageData_t;


//= P R O T O T Y P E S ================================================
void DrawGeneralPage( COptionsPage * );
void DrawSoundPage( COptionsPage * );
void DrawVideoPage( COptionsPage * );
void DrawVectorPage( COptionsPage * );
void DrawNetworkPage( COptionsPage * );
void DrawDirectoryPathPage( COptionsPage * );

void ChangeGeneralPage( COptionsPage *, BOOL direction );
void ChangeSoundPage( COptionsPage *, BOOL direction );
void ChangeVideoPage( COptionsPage *, BOOL direction );
void ChangeVectorPage( COptionsPage *, BOOL direction );
void ChangeNetworkPage( COptionsPage *, BOOL direction );
void ChangeDirectoryPathPage( COptionsPage *, BOOL direction );

//= C L A S S E S ======================================================

/**
	* \class		COptionsPage
	* \brief		The options page class
	*/
class COptionsPage
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	COptionsPage( LPDIRECT3DDEVICE8	displayDevice, CXBFont &font, GameOptions &options ) :
		m_displayDevice( displayDevice ),
		m_font( font ),
		m_cursorPosition( 0 ),
		m_dpadCursorDelay( 0.0f ),
    m_optToggleDelay( 0.0f ),
    m_triggerDelay( 0.0f ),
    m_pageNumber( 0 )
	{
    wcscpy( m_pageData[OPTPAGE_GENERAL].m_title, L"General Options" );
    m_pageData[OPTPAGE_GENERAL].m_drawFunct = ::DrawGeneralPage;
    m_pageData[OPTPAGE_GENERAL].m_changeFunct = ::ChangeGeneralPage;
    m_pageData[OPTPAGE_GENERAL].m_numItems = 4;

    wcscpy( m_pageData[OPTPAGE_SOUND].m_title, L"Sound Options" );
    m_pageData[OPTPAGE_SOUND].m_drawFunct = ::DrawSoundPage;
    m_pageData[OPTPAGE_SOUND].m_changeFunct = ::ChangeSoundPage;
    m_pageData[OPTPAGE_SOUND].m_numItems = 3;

    wcscpy( m_pageData[OPTPAGE_VIDEO].m_title, L"Video Options" );
    m_pageData[OPTPAGE_VIDEO].m_drawFunct = ::DrawVideoPage;
    m_pageData[OPTPAGE_VIDEO].m_changeFunct = ::ChangeVideoPage;
    m_pageData[OPTPAGE_VIDEO].m_numItems = 8;

    wcscpy( m_pageData[OPTPAGE_VECTOR].m_title, L"Vector Options" );
    m_pageData[OPTPAGE_VECTOR].m_drawFunct = ::DrawVectorPage;
    m_pageData[OPTPAGE_VECTOR].m_changeFunct = ::ChangeVectorPage;
    m_pageData[OPTPAGE_VECTOR].m_numItems = 6;

    wcscpy( m_pageData[OPTPAGE_NETWORK].m_title, L"Network Options" );
    m_pageData[OPTPAGE_NETWORK].m_drawFunct = ::DrawNetworkPage;
    m_pageData[OPTPAGE_NETWORK].m_changeFunct = ::ChangeNetworkPage;
    m_pageData[OPTPAGE_NETWORK].m_numItems = 5;

    wcscpy( m_pageData[OPTPAGE_DIRECTORIES].m_title, L"Directory Path Options" );
    m_pageData[OPTPAGE_DIRECTORIES].m_drawFunct = ::DrawDirectoryPathPage;
    m_pageData[OPTPAGE_DIRECTORIES].m_changeFunct = ::ChangeDirectoryPathPage;
    m_pageData[OPTPAGE_DIRECTORIES].m_numItems = 15;
  }

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

  void DrawGeneralPage( void );
  void DrawSoundPage( void );
  void DrawVideoPage( void );
  void DrawVectorPage( void );
  void DrawNetworkPage( void );
  void DrawDirectoryPathPage( void );

  void ChangeGeneralPage( BOOL direction );
  void ChangeSoundPage( BOOL direction );
  void ChangeVideoPage( BOOL direction );
  void ChangeVectorPage( BOOL direction );
  void ChangeNetworkPage( BOOL direction );
  void ChangeDirectoryPathPage( BOOL direction );

protected:
  UINT32                m_pageNumber;         //!< The options page number
	UINT32								m_cursorPosition;	    //!< Cursor position within the current list page
	FLOAT									m_dpadCursorDelay;    //!< Counter used to slow down the dpad repeat rate
  FLOAT                 m_triggerDelay;       //!< Counter used to slow down the trigger repeat rate
  FLOAT                 m_optToggleDelay;     //!< Counter used to slow down option toggle buttons

  optionsPageData_t     m_pageData[OPTPAGE_LAST];

	LPDIRECT3DDEVICE8			m_displayDevice;
	CXBFont								&m_font;
};



#endif    // #ifdef __cplusplus



