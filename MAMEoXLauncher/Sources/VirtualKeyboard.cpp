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

#define NORMAL_ITEM_COLOR				D3DCOLOR_XRGB( 0, 0, 58 )
#define SELECTED_ITEM_COLOR     D3DCOLOR_XRGB( 255, 255, 255 )
#define BORDER_COLOR            D3DCOLOR_XRGB( 0, 191, 243 )
#define BACKDROP_COLOR          D3DCOLOR_XRGB( 105, 105, 105 )

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.3f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

#define X_POS		( 80 )
#define Y_POS		( 25 )
#define WIDTH		( 512 - (X_POS<<1) )

//= G L O B A L = V A R S ==============================================

static WCHAR  *g_keyboardData[4] = { L"1234567890.",
                                     L"ABCDEFGHIJK",
                                     L"LMNOPQRSTUV",
                                     L"WXYZ:/" };

//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;      // The transformed position for the vertex
  DWORD         diffuse;  // The diffuse color
};

//= P R O T O T Y P E S ================================================
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
CVirtualKeyboard::CVirtualKeyboard( LPDIRECT3DDEVICE8	displayDevice, CXBFont &font ) :
	m_displayDevice( displayDevice ),
	m_font( font ),
	m_cursorPositionX( 0 ),
  m_cursorPositionY( 0 ),
	m_dpadCursorDelay( 0.0f ),
	m_buttonDelay( 0.0f ),
  m_inputState( VKS_INPROGRESS ),
  m_minChars( 0 ),
  m_maxChars( 1024 )
{
  m_font.GetTextExtent( L"Wijg,", &m_textWidth, &m_textHeight );
  m_textWidth = m_font.GetTextWidth( L"W" );

    // Create a vertex buffer to render the backdrop image to the renderTargetTexture
  m_displayDevice->CreateVertexBuffer(  (sizeof(CUSTOMVERTEX) << 2),
																        D3DUSAGE_WRITEONLY,
																	      D3DFVF_XYZ | D3DFVF_DIFFUSE,
																	      D3DPOOL_MANAGED,
																	      &m_backdropVB );

	CUSTOMVERTEX *pVertices;
	m_backdropVB->Lock( 0,										// Offset to lock
														  0,										// Size to lock
														  (BYTE**)&pVertices,		// ppbData
														  0 );									// Flags

    #define XPOS 0.96f
    #define YPOS 0.9f

		pVertices[0].pos.x = -XPOS;
		pVertices[0].pos.y = YPOS;
		pVertices[0].pos.z = 1.0f;
    pVertices[0].diffuse = BACKDROP_COLOR;

		pVertices[1].pos.x = XPOS;
		pVertices[1].pos.y = YPOS;
		pVertices[1].pos.z = 1.0f;
    pVertices[1].diffuse = BACKDROP_COLOR;
  	
		pVertices[2].pos.x = XPOS;
		pVertices[2].pos.y = -YPOS;
		pVertices[2].pos.z = 1.0f;
    pVertices[2].diffuse = BACKDROP_COLOR;
  	
		pVertices[3].pos.x = -XPOS;
		pVertices[3].pos.y = -YPOS;
		pVertices[3].pos.z = 1.0f;
    pVertices[3].diffuse = BACKDROP_COLOR;

  m_backdropVB->Unlock();


  if( FAILED( D3DXCreateTexture(  displayDevice,
                                  256, 
                                  128, 
                                  1, 
                                  0L, 
                                  D3DFMT_LIN_X8R8G8B8, 
                                  D3DPOOL_DEFAULT, 
                                  &m_renderTarget ) ) )
  {
    Die( displayDevice, "Fatal error while creating virtual keyboard target texture!" );
  }
}

//------------------------------------------------------------
// Destructor
//------------------------------------------------------------
CVirtualKeyboard::~CVirtualKeyboard( void ) 
{
  SAFE_RELEASE( m_renderTarget );
  SAFE_RELEASE( m_backdropVB );
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CVirtualKeyboard::MoveCursor( CGamepad &gp )
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
    }
    else if( gp.IsButtonPressed( GP_B ) )
    {
      m_buttonDelay = DPADCURSORMOVE_TIMEOUT;
      if( m_data.length() )
        m_data = m_data.substr( 0, m_data.length() - 1 );
    }
  }
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CVirtualKeyboard::Draw( void )
{
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

	m_font.Begin();

    WCHAR buf[256];
    #define X_OFFSET    30

    mbstowcs( buf, m_data.c_str(), 255 );
    m_font.DrawText( X_OFFSET, 10, SELECTED_ITEM_COLOR, buf );

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
        m_font.DrawText( (x * keyWidth) + X_OFFSET, (y * keyHeight) + 35, ITEMCOLOR(), buf );
      }

      
      
    m_font.DrawText(  ((wcslen(g_keyboardData[3]) + 1) * keyWidth) + X_OFFSET, 
                      (3*keyHeight) + 35, 
                      ( m_cursorPositionY == 3 && m_cursorPositionX == wcslen(g_keyboardData[3]) ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR ),
                      L"OK" );

    m_font.DrawText(  ((wcslen(g_keyboardData[3]) + 3) * keyWidth) + X_OFFSET, 
                      (3*keyHeight) + 35, 
                      ( m_cursorPositionY == 3 && m_cursorPositionX == wcslen(g_keyboardData[3]) + 1 ? SELECTED_ITEM_COLOR : NORMAL_ITEM_COLOR ), 
                      L"CANCEL" );

	m_font.End();

  RenderToTextureEnd( token );
}
