/**
	* \file			BaseView.h
	* \brief		Base class for all views
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>

#include "XBFont.h"
#include "MAMEoX.h"
#include "InputManager.h"
#include "FontSet.h"


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
	CBaseView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, LPDIRECT3DTEXTURE8 backdropTexture ) :
		m_displayDevice( displayDevice ),
		m_fontSet( fontSet ),
    m_backdropTexture( backdropTexture )
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
  LPDIRECT3DTEXTURE8        m_backdropTexture;          //!<  Texture to render as the backdrop

  RenderToTextureToken_t    m_renderToTextureToken;     //!< Token for texture rendering
};
