/**
	* \file			StartMenu.cpp
	* \brief		Modal start menu
	*
	*/

//= I N C L U D E S ====================================================
#include "StartMenuView.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "ListView.h"		// For DPAD timeout define

#include <string>

//= D E F I N E S ======================================================


  //--- Layout defines -----------------------------------------
#define TITLEBAR_ROW            m_titleArea.top + 4
#define FIRSTDATA_ROW           m_bodyArea.top

#define HIGHLIGHTBAR_LEFT       m_bodyArea.left + 3
#define HIGHLIGHTBAR_RIGHT      m_bodyArea.right - 3
#define NAME_START              m_bodyArea.left + 9
#define TEXTBOX_RIGHT           HIGHLIGHTBAR_RIGHT   // The right edge of the text box
#define COLUMN_PADDING          9     // Number of pixels to subtract from the column width before truncating text

//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CStartMenuView::MoveCursor( CInputManager &gp, BOOL unused )
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
    BYTE arrowKeys[2] = { VK_UP, VK_DOWN };
		m_dpadCursorDelay -= elapsedTime;
    
		if( m_dpadCursorDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_MASK | GP_LA_MASK ) || gp.IsOneOfKeysPressed( arrowKeys, 2 )) )
			m_dpadCursorDelay = 0.0f;
	}

  if( m_dpadCursorDelay == 0.0f )
  {
    if( gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN ) )
	  {
      if( m_cursorPosition < (m_menuItems.size() - 1) )
        ++m_cursorPosition;
      else
        m_cursorPosition = 0;
		  
      m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )  )
	  {
      if( m_cursorPosition )
        --m_cursorPosition;
      else
        m_cursorPosition = m_menuItems.size() - 1;

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
  }


  if( gp.IsOneOfButtonsPressed( GP_BACK | GP_START | GP_B ) || gp.IsKeyPressed( VK_ESCAPE ) )
  {
      // Quit the menu
    m_inputState = MENU_CANCELLED;
    gp.WaitForNoButton();
  }
  else if( gp.IsButtonPressed( GP_A ) || gp.IsKeyPressed( VK_RETURN ) )
  {
      // Execute the selected item
    m_inputState = MENU_ACCEPTED;
    gp.WaitForNoButton();
  }
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CStartMenuView::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil
	
	if( !g_loadedSkin )
		return;


	FLOAT fontHeight = 20.0f;
	if( CheckResourceValidity( SKINELEMENT_STARTMENU_BODY_TEXT ) ) 
	{
		const CSkinText *t = g_loadedSkin->GetSkinElementText(SKINELEMENT_STARTMENU_BODY_TEXT);
		fontHeight = t->GetFontHeight();
	}

  RenderBackdrop( fontHeight );

    //-- Render the highlight bar for the selected item -------------------------------------
	if( CheckResourceValidity( SKINELEMENT_STARTMENU_BODY_HIGHLIGHTBAR ) )
	{
		FLOAT selectedItemYPos = (m_fontSet.SmallThinFontHeight() * (UINT32)m_cursorPosition);
		
		g_loadedSkin->GetSkinElementHighlightbar(SKINELEMENT_STARTMENU_BODY_HIGHLIGHTBAR)->RenderAtRect(	m_displayDevice,
																																																			HIGHLIGHTBAR_LEFT,
																																																			FIRSTDATA_ROW + selectedItemYPos,
																																																			HIGHLIGHTBAR_RIGHT,
																																																			FIRSTDATA_ROW + selectedItemYPos + fontHeight );
	}

	if( CheckResourceValidity( SKINELEMENT_STARTMENU_HEADER_TEXT ) ) 
	{
		WCHAR wBuf[256];
    mbstowcs( wBuf, m_title.c_str(), 256 );

		g_loadedSkin->GetSkinElementText(SKINELEMENT_STARTMENU_HEADER_TEXT)->RenderAtRect(	m_displayDevice,
																																												wBuf,
																																												NAME_START,
																																												TITLEBAR_ROW,
																																												TEXTBOX_RIGHT,
																																												VALUE_AUTO,
																																												XBFONT_TRUNCATED );
	}


	if( CheckResourceValidity( SKINELEMENT_STARTMENU_BODY_TEXT ) ) 
	{
		WCHAR wBuf[256];
		const CSkinText *t = g_loadedSkin->GetSkinElementText(SKINELEMENT_STARTMENU_BODY_TEXT);

			// Render the menu items
    std::vector< std::pair<CStdString,UINT32> >::iterator i = m_menuItems.begin();
    for( UINT32 y = 0; i != m_menuItems.end(); ++i, y += fontHeight )
    {
      mbstowcs( wBuf, (*i).first.c_str(), 256 );
			t->RenderAtRect(	m_displayDevice,
												wBuf,
												NAME_START,
												FIRSTDATA_ROW + y,
												TEXTBOX_RIGHT,
												VALUE_AUTO,
												XBFONT_TRUNCATED );
		}
	}

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}





