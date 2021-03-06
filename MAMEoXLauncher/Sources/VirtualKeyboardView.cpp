/**
	* \file			VirtualKeyboard.cpp
	* \brief		Virtual keyboard class
	*
	*/

//= I N C L U D E S ====================================================
#include "VirtualKeyboardView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================

  //--- Layout defines -----------------------------------------
#define TITLEBAR_ROW            m_titleArea.top + 6
#define FIRSTDATA_ROW           m_bodyArea.top + 3

#define HIGHLIGHTBAR_LEFT       m_bodyArea.left + 4
#define HIGHLIGHTBAR_RIGHT      m_bodyArea.right - 4
#define NAME_START              m_bodyArea.left + 9
#define DATA_START              NAME_START + 4
#define TEXTBOX_RIGHT           HIGHLIGHTBAR_RIGHT   // The right edge of the text box
#define COLUMN_PADDING          9     // Number of pixels to subtract from the column width before truncating text


	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.2f
#define KEYBOARDINPUT_TIMEOUT   0.2f

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
CVirtualKeyboardView::CVirtualKeyboardView( LPDIRECT3DDEVICE8	displayDevice, CFontSet &fontSet, const RECT &area ) :
  CBasePopupView( displayDevice, fontSet, area )
{
  Reset();
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CVirtualKeyboardView::MoveCursor( CInputManager &gp, BOOL unused )
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
    if( m_buttonDelay < 0.0f || !(gp.IsOneOfButtonsPressed( GP_A | GP_B ) || gp.IsAnyKeyPressed()) )
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


        // Check the keyboard
    if( gp.IsKeyPressed( VK_BACK ) )
    {
      m_buttonDelay = KEYBOARDINPUT_TIMEOUT;
      if( m_data.length() )
      {
        m_data = m_data.substr( 0, m_data.length() - 1 );
        if( m_dataDrawStartPosition )
          --m_dataDrawStartPosition;
      }
    }
    else
    {
        // Colon (PERIOD on a German KB, Semicolon on a US)
      BYTE keys[2] = { VK_OEM_PERIOD, VK_OEM_1 };
      if( gp.IsOneOfKeysPressed( keys, 2 ) )
      {
        m_buttonDelay = KEYBOARDINPUT_TIMEOUT;
        m_data += ":";
        if( m_data.length() >= (m_maxDisplayableChars - 1) )
          ++m_dataDrawStartPosition;
      }

        // Slash (German / is handled elsewhere)
      if( gp.IsKeyPressed( VK_OEM_2 ) )
      {
        m_buttonDelay = KEYBOARDINPUT_TIMEOUT;
        m_data += "/";
        if( m_data.length() >= (m_maxDisplayableChars - 1) )
          ++m_dataDrawStartPosition;
      }

        // Backslash (German: VK_OEM_MINUS, US: VK_OEM_5)
      keys[0] = VK_OEM_MINUS;
      keys[1] = VK_OEM_5;
      if( gp.IsOneOfKeysPressed( keys, 2 ) )
      {
        m_buttonDelay = KEYBOARDINPUT_TIMEOUT;
        m_data += "\\";
        if( m_data.length() >= (m_maxDisplayableChars - 1) )
          ++m_dataDrawStartPosition;
      }


      BYTE keyIndex = 'A';
      for( ; keyIndex < 'Z'; ++keyIndex )
      {
        if( gp.IsKeyPressed( keyIndex ) )
        {
          m_buttonDelay = KEYBOARDINPUT_TIMEOUT;
            // Add the new char
          char newChar[2] = {0};
          newChar[0] = (char)keyIndex;

            // Handle the @ symbol (German KB)
          if( keyIndex == 'Q' && gp.IsKeyPressed( VK_RMENU ) )
            newChar[0] = '@';

          m_data += newChar;

          if( m_data.length() >= (m_maxDisplayableChars - 1) )
            ++m_dataDrawStartPosition;
          break;
        }
      }

      keys[0] = VK_LSHIFT;
      keys[1] = VK_RSHIFT;
      for( keyIndex = '0'; keyIndex < '9'; ++keyIndex )
      {
        if( gp.IsKeyPressed( keyIndex ) )
        {
          m_buttonDelay = KEYBOARDINPUT_TIMEOUT;

            // Add the new char
          char newChar[2] = {0};
          newChar[0] = (char)keyIndex;

            // Handle the @ symbol
          if( keyIndex == '2' && gp.IsOneOfKeysPressed( keys, 2 ) )
            newChar[0] = '@';
          else if( keyIndex == '7' && gp.IsOneOfKeysPressed( keys, 2 ) )
          {
              // German '/' key
            newChar[0] = '/';
          }

          m_data += newChar;

          if( m_data.length() >= (m_maxDisplayableChars - 1) )
            ++m_dataDrawStartPosition;
          break;
        }
      }
    }
  }


  if( gp.IsButtonPressed( GP_BACK ) || gp.IsKeyPressed( VK_DELETE ) )
    m_inputState = MENU_CANCELLED;
  else if( gp.IsButtonPressed( GP_START ) || gp.IsKeyPressed( VK_RETURN ) )
    m_inputState = MENU_ACCEPTED;
}


//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CVirtualKeyboardView::Draw( BOOL clearScreen, BOOL flipOnCompletion )
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
  FLOAT selectedItemYPos = (fontHeight * (UINT32)m_cursorPositionY);

	if( CheckResourceValidity( SKINELEMENT_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR ) )
	{
		FLOAT left, top, right, bottom;

		if( m_cursorPositionY >= 4 )
		{
				// Highlight the entire bar for cancel/ok
			left = HIGHLIGHTBAR_LEFT;
			top = FIRSTDATA_ROW + selectedItemYPos;
			right = HIGHLIGHTBAR_RIGHT;
			bottom = FIRSTDATA_ROW + selectedItemYPos + fontHeight;
		}
		else
		{
				// Highlight only the selected letter
			FLOAT selectedItemXPos = (characterWidth * (UINT32)m_cursorPositionX) - (CHAR_PADDING / 2.0f);
			left = DATA_START + selectedItemXPos;
			top = FIRSTDATA_ROW + selectedItemYPos;
			right = DATA_START + selectedItemXPos + characterWidth + 1;
			bottom = FIRSTDATA_ROW + selectedItemYPos + fontHeight + 1;
		}

		g_loadedSkin->GetSkinElementHighlightbar(SKINELEMENT_VIRTUALKEYBOARD_BODY_HIGHLIGHTBAR)->RenderAtRect(	m_displayDevice,
																																																						left,
																																																						top,
																																																						right,
																																																						bottom );
  }


	
	if( CheckResourceValidity( SKINELEMENT_VIRTUALKEYBOARD_HEADER_TEXT ) )
	{
	  WCHAR wBuf[256];
    mbstowcs( wBuf, &m_data.c_str()[m_dataDrawStartPosition], 256 );

		g_loadedSkin->GetSkinElementText(SKINELEMENT_VIRTUALKEYBOARD_HEADER_TEXT)->RenderAtRect(	m_displayDevice,
																																															wBuf,
																																															NAME_START,
																																															TITLEBAR_ROW,
																																															TEXTBOX_RIGHT,
																																															TITLEBAR_ROW + 20,
																																															XBFONT_TRUNCATED );
	}

	if( CheckResourceValidity( SKINELEMENT_VIRTUALKEYBOARD_BODY_TEXT ) )
	{
		const CSkinText *t = g_loadedSkin->GetSkinElementText(SKINELEMENT_VIRTUALKEYBOARD_BODY_TEXT);

    // Render the virtual keys
    for( UINT32 y = 0; y < 4; ++y )
    {
      for( UINT32 x = 0; x < wcslen(g_keyboardData[y]); ++x )
      {
				WCHAR wBuf[4];
        wBuf[1] = L'\0';
        wBuf[0] = g_keyboardData[y][x];

				t->RenderAtRect(	m_displayDevice, 
													wBuf,
													DATA_START + (characterWidth * x) + (CHAR_PADDING / 2.0f), 
													FIRSTDATA_ROW + (fontHeight * y),
													DATA_START + (characterWidth * (x+1)) + (CHAR_PADDING / 2.0f), 
													FIRSTDATA_ROW + (fontHeight * (y+1)) );
      }
    }


		t->RenderAtRect(	m_displayDevice, 
											L"Cancel",
											DATA_START,
											FIRSTDATA_ROW + (fontHeight * 4),
											VALUE_UNBOUNDED,
											VALUE_UNBOUNDED );


		t->RenderAtRect(	m_displayDevice, 
											L"OK",
											DATA_START,
											FIRSTDATA_ROW + (fontHeight * 5),
											VALUE_UNBOUNDED,
											VALUE_UNBOUNDED );
	}

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}


