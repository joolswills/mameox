/**
	* \file			OptionsScreen.h
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "BaseMenuView.h"
#include "VirtualKeyboardView.h"
#include "SkinResource.h"

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
  OPTPAGE_DIRECTORIES_1,
  OPTPAGE_DIRECTORIES_2,
  OPTPAGE_ROMLIST,
  OPTPAGE_LAST
} PageID;

//= S T R U C T U R E S ================================================
class COptionsScreen;

typedef struct optionsPageData_t
{
  WCHAR   m_title[64];
  void    (*m_drawFunct)( COptionsScreen * );
  void    (*m_changeFunct)( COptionsScreen *, BOOL );
  UINT32  m_numItems;
} optionsPageData_t;


//= P R O T O T Y P E S ================================================
void DrawGeneralPage( COptionsScreen * );
void DrawSoundPage( COptionsScreen * );
void DrawVideoPage( COptionsScreen * );
void DrawVectorPage( COptionsScreen * );
void DrawNetworkPage( COptionsScreen * );
void DrawDirectoryPathPage1( COptionsScreen * );
void DrawDirectoryPathPage2( COptionsScreen * );
void DrawROMListPage( COptionsScreen * );

void ChangeGeneralPage( COptionsScreen *, BOOL direction );
void ChangeSoundPage( COptionsScreen *, BOOL direction );
void ChangeVideoPage( COptionsScreen *, BOOL direction );
void ChangeVectorPage( COptionsScreen *, BOOL direction );
void ChangeNetworkPage( COptionsScreen *, BOOL direction );
void ChangeDirectoryPathPage1( COptionsScreen *, BOOL direction );
void ChangeDirectoryPathPage2( COptionsScreen *, BOOL direction );
void ChangeROMListPage( COptionsScreen *, BOOL direction );

//= C L A S S E S ======================================================

/**
	* \class		COptionsScreen
	* \brief		The options page class
	*/
class COptionsScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  COptionsScreen( LPDIRECT3DDEVICE8	displayDevice, 
                CFontSet &fontSet, 
                GameOptions &options );

		//------------------------------------------------------------
    // Destructor
		//------------------------------------------------------------
  ~COptionsScreen( void );

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
		//! \brief		Render the list to the screen
    //!
    //! \param    clearScreen - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE );

  void DrawGeneralPage( void );
  void DrawSoundPage( void );
  void DrawVideoPage( void );
  void DrawVectorPage( void );
  void DrawNetworkPage( void );
  void DrawDirectoryPathPage1( void );
  void DrawDirectoryPathPage2( void );
  void DrawROMListPage( void );

  void ChangeGeneralPage( BOOL direction );
  void ChangeSoundPage( BOOL direction );
  void ChangeVideoPage( BOOL direction );
  void ChangeVectorPage( BOOL direction );
  void ChangeNetworkPage( BOOL direction );
  void ChangeDirectoryPathPage1( BOOL direction );
  void ChangeDirectoryPathPage2( BOOL direction );
  void ChangeROMListPage( BOOL direction );

protected:
  UINT32                    m_pageNumber;                   //!< The options page number
  FLOAT                     m_triggerDelay;                 //!< Counter used to slow down the trigger repeat rate
  FLOAT                     m_optToggleDelay;               //!< Counter used to slow down option toggle buttons

  BOOL                      m_virtualKeyboardMode;          //!< Whether or not to show the virtual keyboard
  CVirtualKeyboardView      *m_virtualKeyboard;             //!< Virtual keyboard instance

  CBaseMenuView             *m_menuRenderer;                //!<  Resizable menu renderer

  optionsPageData_t         m_pageData[OPTPAGE_LAST];
};

