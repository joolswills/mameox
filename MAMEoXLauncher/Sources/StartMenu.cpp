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
#define DARKENBACKGROUND_COLOR  D3DCOLOR_ARGB( 160, 0, 0, 0 )
#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 180, 165, 169, 202 )

#define TITLEBAR_ROW            menuTop + 5
#define FIRSTDATA_ROW           menuTop + 25

#define HIGHLIGHTBAR_LEFT       menuLeft + 2
#define HIGHLIGHTBAR_RIGHT      menuLeft + menuWidth - 2
#define NAME_START              menuLeft + 9
#define TEXTBOX_RIGHT           HIGHLIGHTBAR_RIGHT   // The right edge of the text box
#define COLUMN_PADDING          9     // Number of pixels to subtract from the column width before truncating text

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
      if( m_cursorPosition < (m_menuItems.size() - 1) )
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


  if( gp.IsOneOfButtonsPressed( GP_BACK | GP_START ) )
  {
      // Quit the menu
    m_inputState = MENU_CANCELLED;
    gp.WaitForNoButton();
  }
  else if( gp.IsButtonPressed( GP_A ) )
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
  m_displayDevice->SetTexture( 0, NULL );

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
  

    //-- Draw the actual menu texture ------------------------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );

  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

  m_displayDevice->SetTexture( 0, m_textureSet.GetMenuParts() );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );


  FLOAT fontHeight  = m_fontSet.SmallThinFontHeight();
  FLOAT menuLeft    = 320.0f - (m_textureSet.GetMenuTitleWidth() / 2.0f);
  FLOAT menuTop     = 240.0f - (( m_textureSet.GetMenuTitleHeight() + m_textureSet.GetMenuBottomHeight() + (m_menuItems.size() * fontHeight)) / 2.0f);
  FLOAT menuWidth   = m_textureSet.GetMenuTitleWidth();

  FLOAT bodyTop     = menuTop + m_textureSet.GetMenuTitleHeight();
  FLOAT bodyBottom  = bodyTop + (m_menuItems.size() * fontHeight);

  m_displayDevice->Begin( D3DPT_QUADLIST );
      // 1) Draw the "top"
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeft(), m_textureSet.GetMenuTitleBarTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, menuTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRight(), m_textureSet.GetMenuTitleBarTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, menuTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarRight(), m_textureSet.GetMenuTitleBarBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuTitleBarLeft(), m_textureSet.GetMenuTitleBarBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyTop, 1.0f, 1.0f );

      // 2) Draw the body, stretching to fit the desired height
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeft(), m_textureSet.GetMenuBodyTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyTop, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRight(), m_textureSet.GetMenuBodyTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyTop, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyRight(), m_textureSet.GetMenuBodyBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBodyLeft(), m_textureSet.GetMenuBodyBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom, 1.0f, 1.0f );

      // 3) Draw the tail
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomLeft(), m_textureSet.GetMenuBottomTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomRight(), m_textureSet.GetMenuBottomTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomRight(), m_textureSet.GetMenuBottomBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft + menuWidth, bodyBottom + m_textureSet.GetMenuBottomHeight(), 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetMenuBottomLeft(), m_textureSet.GetMenuBottomBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, menuLeft, bodyBottom + m_textureSet.GetMenuBottomHeight(), 1.0f, 1.0f );

  m_displayDevice->End();
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     FALSE );

    //-- Render the highlight bar for the selected item -------------------------------------
  FLOAT selectedItemYPos = (m_fontSet.SmallThinFontHeight() * (ULONG)m_cursorPosition);
  m_displayDevice->SetTexture( 0, NULL );
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
  m_fontSet.SmallThinFont().Begin();
    mbstowcs( wBuf, m_menuTitle.c_str(), 256 );
    m_fontSet.SmallThinFont().DrawText( NAME_START,
                                        TITLEBAR_ROW,
                                        HEADER_COLOR,
                                        wBuf,
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - (NAME_START+COLUMN_PADDING) );
  m_fontSet.SmallThinFont().End();


    // Render the menu items
  m_fontSet.SmallThinFont().Begin();
    std::vector<CStdString>::iterator i = m_menuItems.begin();
    for( UINT32 y = 0; i != m_menuItems.end(); ++i, y += fontHeight )
    {
      mbstowcs( wBuf, (*i).c_str(), 256 );
      m_fontSet.SmallThinFont().DrawText( NAME_START,
                                          FIRSTDATA_ROW + y,
                                          ITEM_COLOR,
                                          wBuf,
                                          XBFONT_TRUNCATED,
                                          TEXTBOX_RIGHT - (NAME_START+COLUMN_PADDING) );
    }
  m_fontSet.SmallThinFont().End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}





