/**
	* \file			OptionsPage.h
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "VirtualKeyboard.h"

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
class COptionsPage : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  COptionsPage( LPDIRECT3DDEVICE8	displayDevice, 
                CFontSet &fontSet, 
                LPDIRECT3DTEXTURE8 backdropTexture,
                GameOptions &options );

		//------------------------------------------------------------
    // Destructor
		//------------------------------------------------------------
  ~COptionsPage( void );

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The CGamepad containing the 
		//!                current state of the user's joystick
    //! \param    useSpeedBanding - Whether or not to enable
    //!              "speed bands" (cursor acceleration)
		//------------------------------------------------------------
	virtual void MoveCursor( CGamepad &gp, BOOL useSpeedBanding = FALSE );

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
  void DrawDirectoryPathPage( void );

  void ChangeGeneralPage( BOOL direction );
  void ChangeSoundPage( BOOL direction );
  void ChangeVideoPage( BOOL direction );
  void ChangeVectorPage( BOOL direction );
  void ChangeNetworkPage( BOOL direction );
  void ChangeDirectoryPathPage( BOOL direction );

protected:
  UINT32                    m_pageNumber;                   //!< The options page number
  FLOAT                     m_triggerDelay;                 //!< Counter used to slow down the trigger repeat rate
  FLOAT                     m_optToggleDelay;               //!< Counter used to slow down option toggle buttons

  BOOL                      m_virtualKeyboardMode;          //!< Whether or not to show the virtual keyboard
  CVirtualKeyboard          *m_virtualKeyboard;             //!< Virtual keyboard instance
  LPDIRECT3DVERTEXBUFFER8   m_virtualKeyboardVertexBuffer;

  optionsPageData_t         m_pageData[OPTPAGE_LAST];
};

