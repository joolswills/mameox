/**
	* \file			StartMenu.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "StartMenu.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

//= D E F I N E S ======================================================


  //--- Layout defines -----------------------------------------
#define HEADER_COLOR          D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 0, 0, 0 )
#define HIGHLIGHTBAR_COLOR    D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define HELPICON_COLOR        D3DCOLOR_XRGB( 255, 255, 255 )
#define HELPBACKDROP_COLOR    D3DCOLOR_ARGB( 127, 255, 255, 255 )

#define TITLEBAR_ROW          116
#define FIRSTDATA_ROW         140

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    607
#define NAME_START            40
#define VALUE_START           240
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text


#define TRIGGER_TOP           422
#define LEFTTRIGGER_LEFT      30
#define RIGHTTRIGGER_RIGHT    608



	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CStartMenu::MoveCursor( CInputManager &gp, BOOL unused )
{
  /*
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
      if( m_cursorPositionY < 2 )
        ++m_cursorPositionY;
      else if( m_cursorPositionY == 2 )
      {
        ++m_cursorPositionY;

          // Force m_cursorPositionX to be in range
        if( m_cursorPositionX > wcslen( g_keyboardData[3] ) + 1 )
          m_cursorPositionX = wcslen( g_keyboardData[3] ) + 1;
      }

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) )
	  {
      if( m_cursorPositionY )
        --m_cursorPositionY;
		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    if( gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_LA_LEFT ) )
	  {
      if( m_cursorPositionX )
        --m_cursorPositionX;
		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_RIGHT | GP_LA_RIGHT ) )
	  {
      if( m_cursorPositionY < 3 )
      {
        if( m_cursorPositionX < wcslen( g_keyboardData[m_cursorPositionY] ) - 1 )
          ++m_cursorPositionX;
      }
      else
      {
          // Allow the cursor position to go to OK and Cancel
        if( m_cursorPositionX < wcslen( g_keyboardData[3] ) + 1 )
          ++m_cursorPositionX;
      }
		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
  }


  if( m_buttonDelay == 0.0f )
  {
    if( gp.IsButtonPressed( GP_A ) )
    {
      m_buttonDelay = DPADCURSORMOVE_TIMEOUT;
      if( m_cursorPositionY == 3 )
      {
        if( m_cursorPositionX == wcslen( g_keyboardData[3] ) )
        {
          m_inputState = VKS_ACCEPTED;
          return;
        }
        else if( m_cursorPositionX == wcslen( g_keyboardData[3] ) + 1 )
        {
          m_inputState = VKS_CANCELLED;
          return;
        }
      }

      char newChar[2] = {0};
      wctomb( &newChar[0], g_keyboardData[m_cursorPositionY][m_cursorPositionX] );
      m_data += newChar;

      if( m_data.length() >= (m_maxDisplayableChars - 1) )
        ++m_dataDrawStartPosition;
    }
    else if( gp.IsButtonPressed( GP_B ) )
    {
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
    m_inputState = VKS_CANCELLED;
  else if( gp.IsButtonPressed( GP_START ) )
    m_inputState = VKS_ACCEPTED;
  */
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CStartMenu::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  /*
  RenderToTextureToken_t token;
  D3DVIEWPORT8 vp = { 0, 0, VK_SCREEN_WIDTH, VK_SCREEN_HEIGHT, 0.0f, 1.0f };
  RenderToTextureStart( token, m_displayDevice, m_renderTarget, vp );

	m_displayDevice->Clear(	0L,																// Count
		  										NULL,															// Rects to clear
			  									D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  								BORDER_COLOR,				// Color
					  							1.0f,															// Z
						  						0L );															// Stencil

    // Render the backdrop
  m_displayDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
  m_displayDevice->SetStreamSource(	0,												  // Stream number
																    m_backdropVB,	                // Stream data
																    sizeof(CUSTOMVERTEX) );		  // Vertex stride
  m_displayDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );

	m_fontSet.FixedWidthFont().Begin();

    WCHAR buf[256];
    #define X_OFFSET    30

    mbstowcs( buf, m_data.c_str(), 255 );
    m_fontSet.FixedWidthFont().DrawText( 15, 10, SELECTED_ITEM_COLOR, &buf[m_dataDrawStartPosition] );

    FLOAT keyWidth = m_textWidth + 4;
    FLOAT keyHeight = m_textHeight + 2;
    #define ITEMCOLOR() (x == m_cursorPositionX && m_cursorPositionY == y) ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR
    buf[1] = L'\0';

    // Row 1: 1234567890.
    // Row 2: ABCDEFGHIJK
    // Row 3: LMNOPQRSTUV
    // Row 4: WXYZ:/ OK Cancel
    for( UINT32 y = 0; y < 4; ++y )
      for( UINT32 x = 0; x < wcslen(g_keyboardData[y]); ++x )
      {
        buf[0] = g_keyboardData[y][x];
        m_fontSet.FixedWidthFont().DrawText( (x * keyWidth) + X_OFFSET, (y * keyHeight) + 35, ITEMCOLOR(), buf );
      }

      
      
    m_fontSet.FixedWidthFont().DrawText(  ((wcslen(g_keyboardData[3]) + 1) * keyWidth) + X_OFFSET, 
                      (3*keyHeight) + 35, 
                      ( m_cursorPositionY == 3 && m_cursorPositionX == wcslen(g_keyboardData[3]) ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR ),
                      L"OK" );

    m_fontSet.FixedWidthFont().DrawText(  ((wcslen(g_keyboardData[3]) + 3) * keyWidth) + X_OFFSET, 
                      (3*keyHeight) + 35, 
                      ( m_cursorPositionY == 3 && m_cursorPositionX == wcslen(g_keyboardData[3]) + 1 ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR ), 
                      L"CANCEL" );

	m_fontSet.FixedWidthFont().End();

  RenderToTextureStop( token );
  */
}





