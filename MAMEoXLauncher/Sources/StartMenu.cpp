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
#define HEADER_COLOR            D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			        D3DCOLOR_XRGB( 0, 0, 0 )
#define DARKENBACKGROUND_COLOR  D3DCOLOR_ARGB( 180, 80, 80, 80 )
#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define HELPICON_COLOR          D3DCOLOR_XRGB( 255, 255, 255 )
#define HELPBACKDROP_COLOR      D3DCOLOR_ARGB( 127, 255, 255, 255 )

#define MENU_WIDTH              242
#define MENU_HEIGHT             121
#define MENU_LEFT               320 - (MENU_WIDTH>>1)
#define MENU_TOP                280 - (MENU_HEIGHT>>1)



#define TITLEBAR_ROW            116
#define FIRSTDATA_ROW           140

#define HIGHLIGHTBAR_LEFT       34
#define HIGHLIGHTBAR_RIGHT      607
#define NAME_START              40
#define VALUE_START             240
#define TEXTBOX_RIGHT           604   // The right edge of the text box
#define COLUMN_PADDING          9     // Number of pixels to subtract from the column width before truncating text


#define TRIGGER_TOP             422
#define LEFTTRIGGER_LEFT        30
#define RIGHTTRIGGER_RIGHT      608

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CStartMenu::MoveCursor( CInputManager &gp, BOOL unused )
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
      if( m_cursorPosition < m_menuItems.size() )
        ++m_cursorPosition;
      else
        m_cursorPosition = 0;
		  
      m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
    else if( gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) )
	  {
      if( m_cursorPosition )
        --m_cursorPosition;
      else
        m_cursorPosition = m_menuItems.size() - 1;

		  m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	  }
  }


  if( gp.IsButtonPressed( GP_BACK ) )
  {
      // Quit the menu
    m_inputState = MENU_CANCELLED;
    gp.WaitForNoButton();
  }
  else if( gp.IsButtonPressed( GP_A | GP_START ) )
  {
      // Execute the selected item
    m_inputState = MENU_ACCEPTED;
    gp.WaitForNoButton();
  }
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CStartMenu::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

    // Darken the screen under the menu
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
  m_displayDevice->End();

  
    // Render the backdrop in the center of the screen
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );

    // Draw the actual menu texture
	m_displayDevice->SetTexture( 0, m_textureSet.GetStartMenu() );
  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, MENU_LEFT, MENU_TOP, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, MENU_LEFT + MENU_WIDTH, MENU_TOP, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, MENU_LEFT + MENU_WIDTH, MENU_TOP + MENU_HEIGHT, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, DARKENBACKGROUND_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, MENU_LEFT, MENU_TOP + MENU_HEIGHT, 1.0f, 1.0f );
  m_displayDevice->End();

    // Draw the highlight for the selected item
  FLOAT selectedItemYPos = (m_fontSet.LargeThinFontHeight() * m_cursorPosition);
  FLOAT fontHeight = m_fontSet.LargeThinFontHeight();
    //-- Render the highlight bar for the selected item -------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

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


  WCHAR wBuf[256];

    // Render the title
  m_fontSet.LargeThinFont().Begin();
    mbstowcs( wBuf, m_menuTitle.c_str(), 256 );
    m_fontSet.LargeThinFont().DrawText( NAME_START,
                                        TITLEBAR_ROW,
                                        HEADER_COLOR,
                                        wBuf,
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - (NAME_START+COLUMN_PADDING) );
  m_fontSet.LargeThinFont().End();


    // Render the menu items
  m_fontSet.LargeThinFont().Begin();
    std::vector<CStdString>::iterator i = m_menuItems.begin();
    for( UINT32 y = 0; i != m_menuItems.end(); ++i, y += fontHeight )
    {
      mbstowcs( wBuf, (*i).c_str(), 256 );
      m_fontSet.LargeThinFont().DrawText( NAME_START,
                                          FIRSTDATA_ROW + y,
                                          ITEM_COLOR,
                                          wBuf,
                                          XBFONT_TRUNCATED,
                                          TEXTBOX_RIGHT - (NAME_START+COLUMN_PADDING) );
    }
  m_fontSet.LargeThinFont().End();
}





