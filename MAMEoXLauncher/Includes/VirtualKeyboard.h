/**
	* \file			VirtualKeyboard.h
	* \brief		Virtual keyboard class
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <vector>

#include "MAMEoX.h"
#include "InputManager.h"
#include "StdString.h"
#include "FontSet.h"

//= D E F I N E S ======================================================
#define VK_SCREEN_WIDTH         256.0f    //!< Width of the VK window
#define VK_SCREEN_HEIGHT        128.0f    //!< Height of the VK window

typedef enum VKInputState
{
  VKS_INPROGRESS = 0x00,
  VKS_ACCEPTED,
  VKS_CANCELLED
} VKInputState;


//= C L A S S E S ======================================================

/**
	* \class		CVirtualKeyboard
	* \brief		The virtual keyboard class
	*/
class CVirtualKeyboard
{
public:

		//------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------
	CVirtualKeyboard( LPDIRECT3DDEVICE8	displayDevice, CFontSet &font );

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CVirtualKeyboard( void );

		//------------------------------------------------------------
		// Reset
		//! \brief		Reset (most) member vars to defaults to allow
    //!           the VK to be reused for a different input session
		//------------------------------------------------------------
  void Reset( void ) {
		m_cursorPositionX = m_cursorPositionY = 0;
    m_dataDrawStartPosition = 0;
		m_dpadCursorDelay = 0.0f;
    m_buttonDelay = 0.0f;
    m_inputState = VKS_INPROGRESS;
    m_minChars = 0;
    m_maxChars = 1024;
    m_data = "";
  }

		//------------------------------------------------------------
		// MoveCursor
		//! \brief		Move the cursor based on user input
		//!
		//! \param		gp - The CGamepad containing the 
		//!                current state of the user's joystick
		//------------------------------------------------------------
	void MoveCursor( CInputManager &gp );

		//------------------------------------------------------------
		// Draw
		//! \brief		Render the current list to the user
		//------------------------------------------------------------
	void Draw( void );
  
		//------------------------------------------------------------
		// GetTexture
		//! \brief		Retrieve the rendered texture
		//------------------------------------------------------------
  LPDIRECT3DTEXTURE8 GetTexture( void ) { return m_renderTarget; }

  void SetData( const CStdString &data ) { SetData( data.c_str() ); }
  void SetData( const char *data ) {
    m_dataDrawStartPosition = 0;
    m_data = data; 
    if( m_data.size() > m_maxDisplayableChars )
      m_dataDrawStartPosition = (m_data.size() - (m_maxDisplayableChars - 1));
  }

  BOOL IsInputFinished( void ) const { return m_inputState != VKS_INPROGRESS; }
  BOOL IsInputAccepted( void ) const { return m_inputState == VKS_ACCEPTED; }
  BOOL IsInputCancelled( void ) const { return m_inputState == VKS_CANCELLED; }
  const CStdString &GetData( void ) const { return m_data; }

protected:
  LPDIRECT3DTEXTURE8        m_renderTarget;   //!< Texture to receive the virtual keyboard
  LPDIRECT3DVERTEXBUFFER8   m_backdropVB;     //!< Vertex buffer used to render the backdrop

	FLOAT									    m_dpadCursorDelay;
	FLOAT									    m_buttonDelay;
	LPDIRECT3DDEVICE8			    m_displayDevice;
	CFontSet							    &m_fontSet;
  FLOAT                     m_textHeight, m_textWidth;

  UINT32                    m_cursorPositionX;
  UINT32                    m_cursorPositionY;
  UINT32                    m_dataDrawStartPosition;  //!< Position to start rendering the data text from
  UINT32                    m_maxDisplayableChars;    //!< Maximum number of characters to display on the data line

  VKInputState              m_inputState;     //!< Input state (accepted, cancelled, in progress)
  UINT32                    m_minChars;       //!< Minimum string size
  UINT32                    m_maxChars;       //!< Maximum string size

  CStdString                m_data;           //!< Text that has been entered
};
