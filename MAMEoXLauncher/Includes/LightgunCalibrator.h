/**
	* \file			LightgunCalibrator.h
	* \brief		Helper class to handle lightgun calibration
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"

//= C L A S S E S ======================================================

/**
	* \class		CLightgunCalibrator
	* \brief		The lightgun calibration helper class
	*/
class CLightgunCalibrator : public CListView
{
public:

		//------------------------------------------------------------
    // Constructor
		//------------------------------------------------------------
	CLightgunCalibrator(  LPDIRECT3DDEVICE8 displayDevice, 
                        CFontSet &fontSet, 
                        LPDIRECT3DTEXTURE8 backdropTexture, 
                        LPDIRECT3DTEXTURE8 cursorTexture ) :
    CListView( displayDevice, fontSet, backdropTexture ),
    m_cursorTexture( cursorTexture ),
    m_currentInputDeviceIndex( 0 ),
    m_calibrationStep( 0 ),
    m_calibrationCompleted( FALSE )
  {
  }

    // Declared in ListView.h
  virtual void MoveCursor( CInputManager &inputManager, BOOL unused = FALSE );
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE );

  BOOL IsCalibrationCompleted( void ) const { return m_calibrationCompleted; }
protected:

  void UpdateLightgunCursorPosition( CInputManager &inputManager );


    // Texture to use for the cursor
  LPDIRECT3DTEXTURE8  m_cursorTexture;

    //! Current position of the gun pointer
  INT32               m_currentGunX;
  INT32               m_currentGunY;

  UINT32              m_currentInputDeviceIndex;
  UINT32              m_calibrationStep;

    //! Whether or not calibration is completely finished
  BOOL                m_calibrationCompleted;
};
