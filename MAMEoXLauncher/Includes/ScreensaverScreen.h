/**
	* \file			ScreensaverScreen.h
	* \brief		Screensaver rendering class
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "SkinResource.h"
#include "MAMEoX.h"

#include <vector>

//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================


//= C L A S S E S ======================================================

/**
	* \class		CScreensaverScreen
	* \brief		The screensaver display class
	*/
class CScreensaverScreen : public CListView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CScreensaverScreen( LPDIRECT3DDEVICE8	displayDevice, 
                      CFontSet &fontSet,    
                      MAMEDriverData_t *drivers, 
                      UINT32 numDrivers ) :
    CListView( displayDevice, fontSet, RESOURCE_INVALID ),
    m_displayTimeout( 0 )
  {
    if( !m_numDrivers )
      m_numDrivers = numDrivers;
    if( !m_driverInfoList )
      m_driverInfoList = drivers;
	}

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CScreensaverScreen( void ) {
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
  virtual void MoveCursor( CInputManager &inputManager, BOOL useSpeedBanding = FALSE ) {}

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the current list to the user
    //!
    //! \param    opaque - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL opaque = TRUE, BOOL flipOnCompletion = TRUE );

  void FindScreenshots( void );
  void CalculateRenderingQuad( void );

  static MAMEDriverData_t                       *m_driverInfoList;          //!<  Drivers supported by the MAME core
  static UINT32                                 m_numDrivers;               //!<  The total number of drivers supported by the core
protected:
  UINT64                      m_displayTimeout;           //!<  Time before image should be swapped

	LPDIRECT3DTEXTURE8					m_screenshotTexture;				//!<  Texture to be displayed as the screensaver

  DWORD                       m_screenshotOrientation;
  RECT                        m_screenshotRect;
  FLOAT                       m_renderingQuadCoords[4][2];
  FLOAT                       m_renderingTextureCoords[4][2];

  std::vector<CStdString>     m_screenshotFiles;          //!<  The screenshot filenames that will be displayed
};




