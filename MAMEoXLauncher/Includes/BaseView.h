/**
	* \file			BaseView.h
	* \brief		Base class for all views
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"

#include "XBFont.h"
#include "MAMEoX.h"
#include "InputManager.h"
#include "FontSet.h"
#include "Skin.h"



//= D E F I N E S ======================================================


//= C L A S S E S ======================================================

/**
	* \class		CBaseView
	* \brief		The base view class
	*/
class CBaseView
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CBaseView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, SkinResourceID_t backdropID ) :
		m_displayDevice( displayDevice ),
		m_fontSet( fontSet ),
		m_backdropTextureID( backdropID )
	{
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
	virtual void MoveCursor( CInputManager &inputManager, BOOL useSpeedBanding = FALSE ) = 0;

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the list to the screen
    //!
    //! \param    clearScreen - Clear the screen before rendering
    //! \param    flipOnCompletion - Call Present before leaving
		//------------------------------------------------------------
	virtual void Draw( BOOL clearScreen = TRUE, BOOL flipOnCompletion = TRUE ) = 0;

		//------------------------------------------------------------
		// DrawToTexture
		//! \brief		Render the list to the passed texture
    //!
    //! \param    targetTexture - The texture to render to
		//------------------------------------------------------------
  virtual void DrawToTexture( LPDIRECT3DTEXTURE8 targetTexture );

protected:
		//------------------------------------------------------------
		// LoadPNGToTexture
		//! \brief		Load a png file into a texture
    //!
    //! \param    filename - Filename to load (w/ full path!)
    //! \param    ret - [OUT] pointer to a LPDIRECT3DTEXTURE8 to
    //!                 receive the new texture
    //! \param    retRect - [OUT] pointer to a RECT structure to
    //!                     receive the dimensions of the image
		//------------------------------------------------------------
  BOOL LoadPNGToTexture( const CStdString &filename, LPDIRECT3DTEXTURE8 *ret, RECT *retRect );

		//------------------------------------------------------------
		// RenderBackdrop
		//! \brief		Render the backdrop texture to m_displayDevice
		//------------------------------------------------------------
	virtual void RenderBackdrop( void );

		//------------------------------------------------------------
		// RenderToTextureStart
		//! \brief		Start rendering to the passed texture
		//------------------------------------------------------------
  virtual BOOL RenderToTextureStart( LPDIRECT3DTEXTURE8 targetTexture );

		//------------------------------------------------------------
		// RenderToTextureStop
		//! \brief		Stop rendering to a texture
		//------------------------------------------------------------
  virtual void RenderToTextureStop( void );

	LPDIRECT3DDEVICE8			    m_displayDevice;            //!<  Display device to use in rendering
  CFontSet                  &m_fontSet;                 //!<  Set of available fonts
  SkinResourceID_t					m_backdropTextureID;        //!<  Texture to render as the backdrop

  RenderToTextureToken_t    m_renderToTextureToken;     //!< Token for texture rendering
};



