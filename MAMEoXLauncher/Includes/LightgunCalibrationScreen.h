/**
	* \file			LightgunCalibrationScreen.h
	* \brief		Helper class to handle lightgun calibration
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "Skin.h"

//= C L A S S E S ======================================================

/**
	* \class		CLightgunCalibrationScreen
	* \brief		The lightgun calibration helper class
	*/
class CLightgunCalibrationScreen : public CListView
{
public:

		//------------------------------------------------------------
    // Constructor
		//------------------------------------------------------------
	CLightgunCalibrationScreen(  LPDIRECT3DDEVICE8 displayDevice, CFontSet &fontSet ) :
    CListView( displayDevice, fontSet, SKINRESOURCE_INVALID ),
    m_currentInputDeviceIndex( 0 ),
    m_calibrationStep( 0 ),
    m_calibrationCompleted( FALSE ),
    m_isFirstFrame( TRUE )
  {
  }

		//------------------------------------------------------------
    //  Reset
		//------------------------------------------------------------
  void Reset( void ) {
    m_currentInputDeviceIndex = 0;
    m_calibrationStep = 0;
    m_calibrationCompleted = FALSE;
    m_isFirstFrame = TRUE;
  }

    // Declared in ListView.h
  virtual void MoveCursor( CInputManager &inputManager, BOOL unused = FALSE );
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE );

  BOOL IsCalibrationCompleted( void ) const { return m_calibrationCompleted; }
protected:


		//------------------------------------------------------------
		// FindNextGun
		//! \brief		Search for a gun device w/ an index greater than
    //!           m_currentInputDeviceIndex
		//!
		//! \return		BOOL - Whether or not a device was found
		//------------------------------------------------------------
  BOOL FindNextGun( void );

  void GetCalibratedCursorPosition( CInputManager &inputManager );

    //! Current position of the gun pointer
  INT32               m_currentGunX;
  INT32               m_currentGunY;
  INT32               m_currentGunCalibratedX;
  INT32               m_currentGunCalibratedY;
  DWORD               m_currentGunFlags;

  UINT32              m_currentInputDeviceIndex;
  UINT32              m_calibrationStep;

    //! Whether or not calibration is completely finished
  BOOL                m_calibrationCompleted;
  BOOL                m_isFirstFrame; //!< Whether or not this is the first frame for the selected gun
};
