/**
	* \file			VirtualKeyboard.cpp
	* \brief		Virtual keyboard class
	*
	*/

//= I N C L U D E S ====================================================
#include "VirtualKeyboard.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================

  //--- Layout defines -----------------------------------------
#define HEADER_COLOR            D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			        D3DCOLOR_XRGB( 0, 0, 0 )
#define DARKENBACKGROUND_COLOR  D3DCOLOR_ARGB( 160, 0, 0, 0 )
#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 255, 125, 129, 162 )

#define TITLEBAR_ROW            m_titleArea.top
#define FIRSTDATA_ROW           m_bodyArea.top

#define HIGHLIGHTBAR_LEFT       m_bodyArea.left
#define HIGHLIGHTBAR_RIGHT      m_bodyArea.right
#define NAME_START              m_bodyArea.left + 7
#define DATA_START              NAME_START + 4
#define TEXTBOX_RIGHT           HIGHLIGHTBAR_RIGHT   // The right edge of the text box
#define COLUMN_PADDING          9     // Number of pixels to subtract from the column width before truncating text


	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

//= G L O B A L = V A R S ==============================================

static WCHAR  *g_keyboardData[4] = { L"1234567890",
                                     L"ABCDEFGHIJKLM",
                                     L"NOPQRSTUVWXYZ",
                                     L".:/\\@" };

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
CVirtualKeyboard::CVirtualKeyboard( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, CTextureSet &textureSet ) :
  CBasePopupView( displayDevice, fontSet, textureSet )
{
  Reset();
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CVirtualKeyboard::MoveCursor( CInputManager &gp, BOOL unused )
{
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
	if( !lastTime )
	{
			// lastTime isn't valid yet, so wait for the next frame
		lastTime = curTime;
		return;
	}
	lastTime = curTime;

		// Decrement the dpad movement timer
	if( m_dpadCursorDelay > 0.0f )
	{
		m_dpadCursorDelay -= elapsedTime;
    
		if( m_dpadCursorDelay < 0.0f || !gp.IsOneOfButtonsPressed( GP_DPAD_MASK | GP_LA_MASK ) )
			m_dpadCursorDelay = 0.0f;
	}

  if( m_buttonDelay > 0.0f )
  {
    m_buttonDelay -= elapsedTime;
    if( m_buttonDelay < 0.0f || !gp.IsOneOfButtonsPressed( GP_A | GP_B ) )
      m_buttonDelay = 0.0f;
  }



  if( m_dpadCursorDelay == 0.0f )
  {
    if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) )
	  {
      if( m_cursorPositionY < GetNumBodyLines() - 1 )
        ++m_cursorPositionY;
      else
        m_cursorPositionY = 0;

        // Force m_cursorPositionX to be in range
      if( m_cursorPositionY < 4 )
      {
        if( m_cursorPositionX >= wcslen( g_keyboardData[m_cursorPositionY] ) )
          m_cursorPositionX = wcslen( g_keyboardData[m_cursorPositionY] ) - 1;
      }
//      else
//        m_cursorPositionX = 0;

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) )
	  {
      if( m_cursorPositionY )
        --m_cursorPositionY;
      else
        m_cursorPositionY = GetNumBodyLines() - 1;

        // Force m_cursorPositionX to be in range
      if( m_cursorPositionY < 4 )
      {
        if( m_cursorPositionX >= wcslen( g_keyboardData[m_cursorPositionY] ) )
          m_cursorPositionX = wcslen( g_keyboardData[m_cursorPositionY] ) - 1;
      }

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    if( gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_LA_LEFT ) )
	  {
      if( m_cursorPositionY < 4 )
      {
        if( m_cursorPositionX )
          --m_cursorPositionX;
        else
          m_cursorPositionX = wcslen( g_keyboardData[m_cursorPositionY] ) - 1;
      }
		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_RIGHT | GP_LA_RIGHT ) )
	  {
      if( m_cursorPositionY < 4 )
      {
        if( m_cursorPositionX < wcslen( g_keyboardData[m_cursorPositionY] ) - 1 )
          ++m_cursorPositionX;
        else
          m_cursorPositionX = 0;
      }

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
  }


  if( m_buttonDelay == 0.0f )
  {
    if( gp.IsButtonPressed( GP_A ) )
    {       
      m_buttonDelay = DPADCURSORMOVE_TIMEOUT;
     
        // See if the cursor is on OK or Cancel
      if( m_cursorPositionY == 5 )
      {
        m_inputState = MENU_ACCEPTED;
        return;
      }
      else if( m_cursorPositionY == 4 )
      {
        m_inputState = MENU_CANCELLED;
        return;
      }

        // Add the new char
      char newChar[2] = {0};
      wctomb( &newChar[0], g_keyboardData[m_cursorPositionY][m_cursorPositionX] );
      m_data += newChar;

      if( m_data.length() >= (m_maxDisplayableChars - 1) )
        ++m_dataDrawStartPosition;
    }
    else if( gp.IsButtonPressed( GP_B ) )
    {
        // Remove the last character from the data string
      m_buttonDelay = DPADCURSORMOVE_TIMEOUT;
      if( m_data.length() )
      {
        m_data = m_data.substr( 0, m_data.length() - 1 );
        if( m_dataDrawStartPosition )
          --m_dataDrawStartPosition;
      }
    }
  }

  if( gp.IsButtonPressed( GP_BACK ) )
    m_inputState = MENU_CANCELLED;
  else if( gp.IsButtonPressed( GP_START ) )
    m_inputState = MENU_ACCEPTED;
}


//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CVirtualKeyboard::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

    // Figure out the width of the widest char that needs to be displayed
  #define CHAR_PADDING  4.0f
  FLOAT characterWidth = m_fontSet.FixedWidthFont().GetTextWidth( L"W" ) + CHAR_PADDING;
  FLOAT fontHeight  = m_fontSet.FixedWidthFontHeight();

  RenderBackdrop( fontHeight );

    //-- Render the highlight bar for the selected item -------------------------------------
  FLOAT selectedItemYPos = (fontHeight * (ULONG)m_cursorPositionY);

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

    // Highlight the entire bar for cancel/ok
  if( m_cursorPositionY >= 4 )
  {
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, FIRSTDATA_ROW + selectedItemYPos + fontHeight, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, FIRSTDATA_ROW + selectedItemYPos + fontHeight, 1.0f, 1.0f );
    m_displayDevice->End();
  }
  else
  {
      // Highlight only the selected letter
    FLOAT selectedItemXPos = (characterWidth * (ULONG)m_cursorPositionX) - (CHAR_PADDING / 2.0f);
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 
                                        DATA_START + selectedItemXPos, 
                                        FIRSTDATA_ROW + selectedItemYPos, 
                                        1.0f, 
                                        1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 
                                        DATA_START + selectedItemXPos + characterWidth + 1, 
                                        FIRSTDATA_ROW + selectedItemYPos, 
                                        1.0f, 
                                        1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 
                                        DATA_START + selectedItemXPos + characterWidth + 1, 
                                        FIRSTDATA_ROW + selectedItemYPos + fontHeight + 1, 
                                        1.0f, 
                                        1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 
                                        DATA_START + selectedItemXPos, 
                                        FIRSTDATA_ROW + selectedItemYPos + fontHeight + 1, 
                                        1.0f, 
                                        1.0f );
    m_displayDevice->End();
  }

    // Render the data
  WCHAR wBuf[256];
  m_fontSet.FixedWidthFont().Begin();
    mbstowcs( wBuf, &m_data.c_str()[m_dataDrawStartPosition], 256 );
    m_fontSet.FixedWidthFont().DrawText( NAME_START,
                                        TITLEBAR_ROW,
                                        HEADER_COLOR,
                                        wBuf,
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - NAME_START );
  m_fontSet.FixedWidthFont().End();


    // Render the virtual keys
  m_fontSet.FixedWidthFont().Begin();
    for( UINT32 y = 0; y < 4; ++y )
    {
      for( UINT32 x = 0; x < wcslen(g_keyboardData[y]); ++x )
      {
        wBuf[1] = L'\0';
        wBuf[0] = g_keyboardData[y][x];
        m_fontSet.FixedWidthFont().DrawText( DATA_START + (characterWidth * x) + (CHAR_PADDING / 2.0f), 
                                            FIRSTDATA_ROW + (fontHeight * y),
                                            ITEM_COLOR,
                                            wBuf );
      }
    }

    m_fontSet.FixedWidthFont().DrawText( DATA_START,
                                        FIRSTDATA_ROW + (fontHeight * 4),
                                        ITEM_COLOR,
                                        L"Cancel" );
    
    m_fontSet.FixedWidthFont().DrawText( DATA_START,
                                        FIRSTDATA_ROW + (fontHeight * 5),
                                        ITEM_COLOR,
                                        L"OK" );
  m_fontSet.FixedWidthFont().End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


