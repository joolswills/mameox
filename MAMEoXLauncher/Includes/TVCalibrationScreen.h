/**
	* \file			TVCalibrationScreen.h
	* \brief		Helper class which takes care of displaying
	*           the TV calibration routines
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "BaseView.h"
#include "SkinResource.h"

//= D E F I N E S ======================================================
typedef enum tvcalibrationstep
{
  TVCS_SET_UPPERLEFT,
  TVCS_SET_LOWERRIGHT,
  TVCS_COMPLETED
} tvcalibrationstep;

//= S T R U C T U R E S ================================================

//= P R O T O T Y P E S ================================================

//= C L A S S E S ======================================================

/**
	* \class		CTVCalibrationScreen
	* \brief		The TV calibration screen class
	*/
class CTVCalibrationScreen : public CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
  CTVCalibrationScreen( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet ) :
    CBaseView( displayDevice, fontSet, RESOURCE_INVALID )
  {
      Reset();
  }

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset everything to default values
		//------------------------------------------------------------
  void Reset( void );

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


		//------------------------------------------------------------
		// AreScreenSettingsChanged
		//! \brief		Returns whether or not the screen settings have changed
		//------------------------------------------------------------
  BOOL AreScreenSettingsChanged( void ) { return m_screenSettingsChangedFlag; }

  BOOL IsCalibrationCompleted( void ) const { return m_currentStep == TVCS_COMPLETED; }


protected:
  FLOAT               m_dpadCursorDelay;            //!< Delay to slow down dpad input
  BOOL                m_screenSettingsChangedFlag;  //!< Whether or not the screen settings have changed
  tvcalibrationstep   m_currentStep;                //!< The step in the calibration sequence
  RECT                m_screenRect;                 //!< The screen rectangle defined by the user
  POINT               m_cursorPos;                  //!< The current cursor position
};

