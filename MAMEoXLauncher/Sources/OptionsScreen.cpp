/**
	* \file			OptionsScreen.cpp
	* \brief		Helper class which takes care of displaying
	*           the INI file options and allowing the user to change them
	*
	*/

//= I N C L U D E S ====================================================
#include "OptionsScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_FileIO.h"
#include "xbox_Network.h"
#include "xbox_Direct3DRenderer.h"
#include "GFilterManager.h"

#include "xbox_FileIO.h"		// for path info

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================


  //--- Layout defines -----------------------------------------
#define VIRTUALKEYBOARD_WIDTH     250
#define VIRTUALKEYBOARD_HEIGHT    143


  // Layout for the list rendering
#define LISTPOS_LEFT    31
#define LISTPOS_TOP     95
#define LISTPOS_RIGHT   608
#define LISTPOS_BOTTOM  451


#define HEADER_COLOR          D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 0, 0, 0 )
#define HIGHLIGHTBAR_COLOR    D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define HELPICON_COLOR        D3DCOLOR_XRGB( 255, 255, 255 )
#define HELPBACKDROP_COLOR    D3DCOLOR_ARGB( 127, 255, 255, 255 )

#define TITLEBAR_ROW          99
#define FIRSTDATA_ROW         124

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    606
#define NAME_START            42
#define VALUE_START           240
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text


#define TRIGGER_BOTTOM        450
#define TRIGGER_TOP           TRIGGER_BOTTOM - m_textureSet.GetTriggerIconHeight()
#define LEFTTRIGGER_LEFT      32
#define RIGHTTRIGGER_RIGHT    608


  // Text offsets for left/right trigger help messages
/*
#define LEFTTRIGGER_LINEONE_START     58
#define LEFTTRIGGERTEXT_START     65

#define RIGHTTRIGGER_LINEONE_END      582
#define RIGHTTRIGGER_LINETWO_END      574

#define TRIGGERTEXT_LINEONE_ROW       419
#define TRIGGERTEXT_LINETWO_ROW       432
*/

#define LEFTTRIGGERTEXT_START             62
#define RIGHTTRIGGERTEXT_END              579
#define TRIGGERTEXT_ROW                   426

#define STARTPAGE()                       DWORD i = 0; FLOAT fontHeight = m_fontSet.SmallThinFontHeight()
#define DRAWITEM( _name__, _val__ )       m_fontSet.SmallThinFont().DrawText( NAME_START,   FIRSTDATA_ROW+(i*fontHeight), ITEM_COLOR, _name__, XBFONT_TRUNCATED, VALUE_START - (NAME_START+COLUMN_PADDING) ); \
                                          m_fontSet.SmallThinFont().DrawText( VALUE_START,  FIRSTDATA_ROW+(i*fontHeight), ITEM_COLOR, _val__, XBFONT_TRUNCATED, TEXTBOX_RIGHT - (VALUE_START+COLUMN_PADDING) ); \
                                          ++i;
#define ENDPAGE()



	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							18


	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f

  // Number of seconds between valid left/right trigger readings
#define TRIGGERSWITCH_TIMEOUT   0.5f


//= G L O B A L = V A R S ==============================================
extern "C" {
  extern const char *cheatfile;
//  extern const char *history_filename;  // Defined in datafile.c
}

extern BOOL g_soundEnabled; // Sound processing override, defined in xbox_Main.cpp
//static UINT32 g_samplerates[2] = { 22700, 44100 };

extern UINT32 g_screensaverTimeout; // Defined in main.cpp

//= P R O T O T Y P E S ===============================================
void Die( LPDIRECT3DDEVICE8 pD3DDevice, const char *fmt, ... );


//= S T R U C T U R E S ===============================================

//= F U N C T I O N S ==================================================

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
COptionsScreen::COptionsScreen( LPDIRECT3DDEVICE8	displayDevice, 
                            CFontSet &fontSet, 
                            CTextureSet &textureSet, 
                            GameOptions &options ) :
  CListView( displayDevice, fontSet, textureSet.GetBasicBackdrop() ),
  m_textureSet( textureSet ),
  m_optToggleDelay( 0.0f ),
  m_triggerDelay( 0.0f ),
  m_pageNumber( 0 ),
  m_virtualKeyboardMode( FALSE )
{
  wcscpy( m_pageData[OPTPAGE_GENERAL].m_title, L"General Options" );
  m_pageData[OPTPAGE_GENERAL].m_drawFunct = ::DrawGeneralPage;
  m_pageData[OPTPAGE_GENERAL].m_changeFunct = ::ChangeGeneralPage;
  m_pageData[OPTPAGE_GENERAL].m_numItems = 5;

  wcscpy( m_pageData[OPTPAGE_SOUND].m_title, L"Sound Options" );
  m_pageData[OPTPAGE_SOUND].m_drawFunct = ::DrawSoundPage;
  m_pageData[OPTPAGE_SOUND].m_changeFunct = ::ChangeSoundPage;
  m_pageData[OPTPAGE_SOUND].m_numItems = 4;

  wcscpy( m_pageData[OPTPAGE_VIDEO].m_title, L"Video Options" );
  m_pageData[OPTPAGE_VIDEO].m_drawFunct = ::DrawVideoPage;
  m_pageData[OPTPAGE_VIDEO].m_changeFunct = ::ChangeVideoPage;
  m_pageData[OPTPAGE_VIDEO].m_numItems = 12;

  wcscpy( m_pageData[OPTPAGE_VECTOR].m_title, L"Vector Options" );
  m_pageData[OPTPAGE_VECTOR].m_drawFunct = ::DrawVectorPage;
  m_pageData[OPTPAGE_VECTOR].m_changeFunct = ::ChangeVectorPage;
  m_pageData[OPTPAGE_VECTOR].m_numItems = 6;

  wcscpy( m_pageData[OPTPAGE_NETWORK].m_title, L"Network Options" );
  m_pageData[OPTPAGE_NETWORK].m_drawFunct = ::DrawNetworkPage;
  m_pageData[OPTPAGE_NETWORK].m_changeFunct = ::ChangeNetworkPage;
  m_pageData[OPTPAGE_NETWORK].m_numItems = 5;

  wcscpy( m_pageData[OPTPAGE_DIRECTORIES_1].m_title, L"Directory Path Options [1/2]" );
  m_pageData[OPTPAGE_DIRECTORIES_1].m_drawFunct = ::DrawDirectoryPathPage1;
  m_pageData[OPTPAGE_DIRECTORIES_1].m_changeFunct = ::ChangeDirectoryPathPage1;
  m_pageData[OPTPAGE_DIRECTORIES_1].m_numItems = 6;

  wcscpy( m_pageData[OPTPAGE_DIRECTORIES_2].m_title, L"Directory Path Options [2/2]" );
  m_pageData[OPTPAGE_DIRECTORIES_2].m_drawFunct = ::DrawDirectoryPathPage2;
  m_pageData[OPTPAGE_DIRECTORIES_2].m_changeFunct = ::ChangeDirectoryPathPage2;
  m_pageData[OPTPAGE_DIRECTORIES_2].m_numItems = 14;

  wcscpy( m_pageData[OPTPAGE_ROMLIST].m_title, L"ROM List Options" );
  m_pageData[OPTPAGE_ROMLIST].m_drawFunct = ::DrawROMListPage;
  m_pageData[OPTPAGE_ROMLIST].m_changeFunct = ::ChangeROMListPage;
  m_pageData[OPTPAGE_ROMLIST].m_numItems = 9;

  RECT area = { 320 - (VIRTUALKEYBOARD_WIDTH>>1), 240 - (VIRTUALKEYBOARD_HEIGHT >> 1), 
                320 + (VIRTUALKEYBOARD_WIDTH>>1), 240 + (VIRTUALKEYBOARD_HEIGHT >> 1) };
  m_virtualKeyboard = new CVirtualKeyboard( displayDevice, m_fontSet, m_textureSet, area );

  area.left = LISTPOS_LEFT;
  area.top = LISTPOS_TOP;
  area.right = LISTPOS_RIGHT;
  area.bottom = LISTPOS_BOTTOM;
  m_menuRenderer = new CBaseMenuView( displayDevice, fontSet, textureSet, area );
  assert( m_menuRenderer );
}

//---------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------
COptionsScreen::~COptionsScreen( void ) 
{
  if( m_virtualKeyboard )
    delete m_virtualKeyboard;

  delete m_menuRenderer;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void COptionsScreen::MoveCursor( CInputManager &gp, BOOL useSpeedbanding )
{
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	FLOAT elapsedTime = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();

  if( m_virtualKeyboardMode )
  {
    m_virtualKeyboard->MoveCursor( gp );
    m_pageData[m_pageNumber].m_changeFunct( this, TRUE ); // Polls to see if the keyboard is done w/ input

    if( m_virtualKeyboard->IsInputFinished() )
    {
      gp.WaitForNoInput();
      m_virtualKeyboard->Reset();
      m_virtualKeyboardMode = FALSE;
      m_optToggleDelay = TRIGGERSWITCH_TIMEOUT; // Set the delay timer to avoid accepting buttons from the VK
      lastTime = 0;                             // Don't count time spent in VK screen
    }
    else
      return;
  }

		// General idea taken from XMAME
	if( !lastTime )
	{
			// lastTime isn't valid yet, so wait for the next frame
		lastTime = curTime;
		return;
	}
	lastTime = curTime;

		// Decrement the movement timers
	if( m_dpadCursorDelay > 0.0f )
	{
    BYTE arrowKeys[2] = { VK_UP, VK_DOWN };
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) ||
          gp.IsOneOfKeysPressed( arrowKeys, 2 )) )
			m_dpadCursorDelay = 0.0f;
	}

	if( m_optToggleDelay > 0.0f )
	{
    BYTE arrowKeys[4] = { VK_LEFT, VK_RIGHT, VK_RETURN, VK_BACK };
		m_optToggleDelay -= elapsedTime;
    if( m_optToggleDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_DPAD_RIGHT | GP_LA_LEFT | GP_LA_RIGHT | GP_A | GP_B ) ||
          gp.IsOneOfKeysPressed( arrowKeys, 4 )) )
			m_optToggleDelay = 0.0f;
	}

  if( m_triggerDelay > 0.0f )
  {
    BYTE pageKeys[2] = { VK_DELETE, VK_NEXT };
    m_triggerDelay -= elapsedTime;
    if( m_triggerDelay < 0.0f || 
        !(gp.IsOneOfButtonsPressed( GP_LEFT_TRIGGER | GP_RIGHT_TRIGGER ) ||
          gp.IsOneOfKeysPressed( pageKeys, 2 )) )
      m_triggerDelay = 0.0f;
  }


    //-- Check the button states ----------------------------------------------------------
  if( (gp.IsButtonPressed( GP_LEFT_TRIGGER ) || gp.IsKeyPressed( VK_DELETE )) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber )
      --m_pageNumber;
    else
      m_pageNumber = OPTPAGE_LAST - 1;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( (gp.IsButtonPressed( GP_RIGHT_TRIGGER ) || gp.IsKeyPressed( VK_NEXT )) && m_triggerDelay == 0.0f )
  {
    if( m_pageNumber < OPTPAGE_LAST - 1 )
      ++m_pageNumber;
    else
      m_pageNumber = 0;

    m_triggerDelay = TRIGGERSWITCH_TIMEOUT;
    m_cursorPosition = 0;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( ++m_cursorPosition >= m_pageData[m_pageNumber].m_numItems )
      m_cursorPosition = 0;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) && m_dpadCursorDelay == 0.0f )
	{
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
    if( m_cursorPosition ) 
      --m_cursorPosition;
    else
      m_cursorPosition = m_pageData[m_pageNumber].m_numItems - 1;
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_LEFT | GP_LA_LEFT | GP_B ) || gp.IsKeyPressed( VK_LEFT ) || gp.IsKeyPressed( VK_BACK )) && 
            m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, FALSE );
  }
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_RIGHT | GP_LA_RIGHT | GP_A ) || gp.IsKeyPressed( VK_RIGHT ) || gp.IsKeyPressed( VK_RETURN )) && 
            m_optToggleDelay == 0.0f )
	{
		m_optToggleDelay = TRIGGERSWITCH_TIMEOUT;
    m_pageData[m_pageNumber].m_changeFunct( this, TRUE );
  }
  
}



//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void COptionsScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

  RenderBackdrop();
  m_menuRenderer->Draw( FALSE, FALSE );


  FLOAT fontHeight = m_fontSet.SmallThinFontHeight();
  FLOAT selectedItemYPos = (fontHeight * (ULONG)m_cursorPosition);

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



    //-- Render the page's text -----------------------------------------------------------
	m_fontSet.SmallThinFont().Begin();

    m_fontSet.SmallThinFont().DrawText( NAME_START,
                                        TITLEBAR_ROW,
                                        HEADER_COLOR,
                                        m_pageData[m_pageNumber].m_title,
                                        XBFONT_TRUNCATED,
                                        TEXTBOX_RIGHT - (NAME_START+COLUMN_PADDING) );


    m_pageData[m_pageNumber].m_drawFunct( this );

	m_fontSet.SmallThinFont().End();


    // Calculate the previous and next page indices
  UINT32 prevPageIndex = m_pageNumber ? m_pageNumber - 1 : OPTPAGE_LAST - 1;
  UINT32 nextPageIndex = m_pageNumber < OPTPAGE_LAST - 1 ? m_pageNumber + 1 : 0;

    // Calculate the width of each trigger text item
/*
  UINT32 leftTriggerLineOneLength = m_fontSet.SmallThinFont().GetTextWidth( L"Left Trigger" );
  UINT32 leftTriggerLineTwoLength = m_fontSet.SmallThinFont().GetTextWidth( m_pageData[prevPageIndex].m_title );
  UINT32 rightTriggerLineOneLength = m_fontSet.SmallThinFont().GetTextWidth( L"Right Trigger" );
  UINT32 rightTriggerLineTwoLength = m_fontSet.SmallThinFont().GetTextWidth( m_pageData[nextPageIndex].m_title );

  FLOAT leftTriggerLength = max( leftTriggerLineOneLength, leftTriggerLineTwoLength );
  FLOAT rightTriggerLength = max( rightTriggerLineOneLength, rightTriggerLineTwoLength );
*/

  FLOAT leftTriggerLength = m_fontSet.SmallThinFont().GetTextWidth( m_pageData[prevPageIndex].m_title );
  FLOAT rightTriggerLength = m_fontSet.SmallThinFont().GetTextWidth( m_pageData[nextPageIndex].m_title );

    //-- Render the left/right trigger icons ------------------------------------------------------
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
  m_displayDevice->Begin( D3DPT_QUADLIST );      

      // Draw left trigger "head"
    FLOAT leftTriggerIconRight  = LEFTTRIGGER_LEFT + (m_textureSet.GetTriggerIconRight() - m_textureSet.GetTriggerIconLeft()) + 1.0f;
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconLeft(), m_textureSet.GetTriggerIconTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGER_LEFT, TRIGGER_TOP, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconRight(), m_textureSet.GetTriggerIconTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconRight, TRIGGER_TOP, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconRight(), m_textureSet.GetTriggerIconBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconRight, TRIGGER_BOTTOM, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconLeft(), m_textureSet.GetTriggerIconBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGER_LEFT, TRIGGER_BOTTOM, 1.0f, 1.0f );

      // Draw the "tail"
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailLeft(), m_textureSet.GetTriggerIconTailTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGERTEXT_START + leftTriggerLength, TRIGGER_TOP, 1.0f, 1.0f );

    FLOAT leftTriggerIconTailRight = (LEFTTRIGGERTEXT_START + leftTriggerLength) + m_textureSet.GetTriggerIconTailRight() - m_textureSet.GetTriggerIconTailLeft();
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailRight(), m_textureSet.GetTriggerIconTailTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconTailRight, TRIGGER_TOP, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailRight(), m_textureSet.GetTriggerIconTailBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconTailRight, TRIGGER_BOTTOM, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailLeft(), m_textureSet.GetTriggerIconTailBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGERTEXT_START + leftTriggerLength, TRIGGER_BOTTOM, 1.0f, 1.0f );



      // Right trigger, same deal, but flip the textures
    FLOAT rightTriggerIconLeft = RIGHTTRIGGER_RIGHT - (m_textureSet.GetTriggerIconRight() - m_textureSet.GetTriggerIconLeft());
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconRight(), m_textureSet.GetTriggerIconTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconLeft, TRIGGER_TOP, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconLeft(), m_textureSet.GetTriggerIconTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGER_RIGHT, TRIGGER_TOP, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconLeft(), m_textureSet.GetTriggerIconBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGER_RIGHT, TRIGGER_BOTTOM, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconRight(), m_textureSet.GetTriggerIconBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconLeft, TRIGGER_BOTTOM, 1.0f, 1.0f );

      // Draw the "tail"
    FLOAT rightTriggerIconTailLeft = (RIGHTTRIGGERTEXT_END - rightTriggerLength) - (m_textureSet.GetTriggerIconTailRight() - m_textureSet.GetTriggerIconTailLeft());
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailRight(), m_textureSet.GetTriggerIconTailTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconTailLeft, TRIGGER_TOP, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailLeft(), m_textureSet.GetTriggerIconTailTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGERTEXT_END - rightTriggerLength, TRIGGER_TOP, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailLeft(), m_textureSet.GetTriggerIconTailBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGERTEXT_END - rightTriggerLength, TRIGGER_BOTTOM, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPICON_COLOR );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetTriggerIconTailRight(), m_textureSet.GetTriggerIconTailBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconTailLeft, TRIGGER_BOTTOM, 1.0f, 1.0f );



  m_displayDevice->End();

    //--- Draw the text backdrops -----------------------------------------------------------------------------
  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );  

  m_displayDevice->Begin( D3DPT_QUADLIST );      

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconRight, TRIGGER_TOP + 1, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGERTEXT_START + leftTriggerLength + 1, TRIGGER_TOP + 1, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, LEFTTRIGGERTEXT_START + leftTriggerLength + 1, TRIGGER_BOTTOM + 1, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, leftTriggerIconRight, TRIGGER_BOTTOM + 1, 1.0f, 1.0f );




    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGERTEXT_END - rightTriggerLength, TRIGGER_TOP + 1, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconLeft, TRIGGER_TOP + 1, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, rightTriggerIconLeft, TRIGGER_BOTTOM + 1, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HELPBACKDROP_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, RIGHTTRIGGERTEXT_END - rightTriggerLength, TRIGGER_BOTTOM + 1, 1.0f, 1.0f );

  m_displayDevice->End();




    // Finally, draw the text for the left/right trigger
	m_fontSet.SmallThinFont().Begin();
/*
    m_fontSet.SmallThinFont().DrawText( LEFTTRIGGER_LINEONE_START, 
                                        TRIGGERTEXT_LINEONE_ROW, 
                                        HEADER_COLOR, 
                                        L"Left Trigger", 
                                        0 );
*/
    m_fontSet.SmallThinFont().DrawText( LEFTTRIGGERTEXT_START, 
                                        TRIGGERTEXT_ROW, 
                                        HEADER_COLOR, 
                                        m_pageData[prevPageIndex].m_title, 
                                        0 );
/*
    m_fontSet.SmallThinFont().DrawText( RIGHTTRIGGER_LINEONE_END - rightTriggerLineOneLength, 
                                        TRIGGERTEXT_LINEONE_ROW, 
                                        HEADER_COLOR, 
                                        L"Right Trigger", 
                                        XBFONT_RIGHT );
*/
    m_fontSet.SmallThinFont().DrawText( RIGHTTRIGGERTEXT_END, 
                                        TRIGGERTEXT_ROW, 
                                        HEADER_COLOR, 
                                        m_pageData[nextPageIndex].m_title, 
                                        XBFONT_RIGHT );  

	m_fontSet.SmallThinFont().End();


    // Clean up settings
  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

  if( m_virtualKeyboardMode )
    m_virtualKeyboard->Draw( FALSE, FALSE );

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}

//---------------------------------------------------------------------
//	DrawGeneralPage
//---------------------------------------------------------------------
void COptionsScreen::DrawGeneralPage( void )
{
/*
  options.cheat = iniFile.GetProfileInt( "General", "CheatsEnabled", FALSE );
  cheatfile = strdup( iniFile.GetProfileString( "General", "CheatFilename", "cheat.dat" ).c_str() );
  if( !cheatfile )
    options.cheat = FALSE;
  history_filename = strdup( iniFile.GetProfileString( "General", "HistoryFilename", "history.dat" ).c_str() );  

    // 1 to skip the disclaimer screen at startup
  options.skip_disclaimer = iniFile.GetProfileInt( "General", "SkipDisclaimer", FALSE );

    // 1 to skip the game info screen at startup
	options.skip_gameinfo = iniFile.GetProfileInt( "General", "SkipGameInfo", FALSE );
*/
  STARTPAGE();

  DRAWITEM( L"Cheats", options.cheat ? L"Enabled" : L"Disabled" );

  WCHAR name[256] = {0};
	mbstowcs( name, cheatfile, 255 );
  DRAWITEM( L"Cheat file", name );
/*
  mbstowcs( name, history_filename, 255 );
  DRAWITEM( L"History file", name );
*/
  DRAWITEM( L"Disclaimer", options.skip_disclaimer ? L"Skipped" : L"Shown" );

  DRAWITEM( L"Game Info", options.skip_gameinfo ? L"Skipped" : L"Shown" );

  swprintf( name, L"After %lu minute%s", g_screensaverTimeout, g_screensaverTimeout > 1 ? "s" : "" );
  DRAWITEM( L"Screensaver", g_screensaverTimeout ? name : L"Disabled" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawSoundPage
//---------------------------------------------------------------------
void COptionsScreen::DrawSoundPage( void )
{
/*
    // sound sample playback rate, in Hz
  options.samplerate = iniFile.GetProfileInt( "Sound", "SampleRate", 44100 );
    // 1 to enable external .wav samples
  options.use_samples = iniFile.GetProfileInt( "Sound", "UseSamples", TRUE );
    // 1 to enable FIR filter on final mixer output
  options.use_filter = iniFile.GetProfileInt( "Sound", "UseFilter", TRUE );
*/

  WCHAR text[256] = {0};

  STARTPAGE();

  DRAWITEM( L"Sound Processing", g_soundEnabled ? L"Enabled" : L"Disabled" );

  swprintf( text, L"%lu", options.samplerate );
  DRAWITEM( L"Sample Rate", text );

  DRAWITEM( L"Audio Samples", options.use_samples ? L"Enabled" : L"Disabled" );

  DRAWITEM( L"Audio Filter", options.use_filter ? L"Enabled" : L"Disabled" );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVideoPage
//---------------------------------------------------------------------
void COptionsScreen::DrawVideoPage( void )
{
/*
  g_rendererOptions.m_vsync = iniFile.GetProfileInt( "Video", "VSYNC", FALSE );       // Enable VSYNC for game rendering
  g_rendererOptions.m_syncOnlyToVSYNC
  g_rendererOptions.m_frameskip =           iniFile.GetProfileInt( "Video", "Frameskip", AUTO_FRAMESKIP );
  options.brightness = iniFile.GetProfileFloat( "Video", "Brightness", 1.0f );		    // brightness of the display
  options.pause_bright = iniFile.GetProfileFloat( "Video", "PauseBrightness", 0.65f );     // brightness when in pause
	options.gamma = iniFile.GetProfileFloat( "Video", "Gamma", 1.0f );			        // gamma correction of the display
	options.color_depth = iniFile.GetProfileInt( "Video", "ColorDepth", 32 );
  options.use_artwork = iniFile.GetProfileInt( "Video", "Artwork", 0 );
*/

  WCHAR text[256] = {0};
  const WCHAR filterNames[][16] = { L"None",
                                    L"Point", 
                                    L"Linear", 
                                    L"Anisotropic", 
                                    L"Quincunx", 
                                    L"Gaussian Cubic" };


  STARTPAGE();

  DRAWITEM( L"VSYNC", g_rendererOptions.m_vsync ? L"Enabled" : L"Disabled" );
    
  DRAWITEM( L"Framerate throttling", g_rendererOptions.m_throttleFramerate ? L"Enabled" : L"Disabled" );

  swprintf( text, L"Level %d", g_rendererOptions.m_frameskip );
  DRAWITEM( L"Frameskip mode", 
            g_rendererOptions.m_frameskip == AUTO_FRAMESKIP ?  L"Auto" :
            (!g_rendererOptions.m_frameskip ? L"No frameskipping" : text ) );

  DRAWITEM( L"Aspect ratio correction", g_rendererOptions.m_preserveAspectRatio ? L"Enabled" : L"Disabled" );

  DRAWITEM( L"Screen rotation", 
            g_rendererOptions.m_screenRotation == SR_0 ? L"None" : 
            (g_rendererOptions.m_screenRotation == SR_90 ? L"90 degrees" :
            (g_rendererOptions.m_screenRotation == SR_180 ? L"180 degrees" :
            L"270 degrees")) );



  swprintf( text, L"%f", options.brightness );
  DRAWITEM( L"Brightness", text );

  swprintf( text, L"%f", options.pause_bright );
  DRAWITEM( L"Paused brightness", text );

  swprintf( text, L"%f", options.gamma );
  DRAWITEM( L"Gamma", text );

//  swprintf( text, L"%lu", options.color_depth );
//  DRAWITEM( L"Color depth", text );

  DRAWITEM( L"Minification filter", filterNames[g_rendererOptions.m_minFilter] );

  DRAWITEM( L"Magnification filter", filterNames[g_rendererOptions.m_magFilter] );

	CGFilterManager oFilterManger;
	MapFilters mapFilters = oFilterManger.GetAvailableFilters();
	DRAWITEM( L"Scaling filter", mapFilters[(EFilterType)g_rendererOptions.m_FilterType].m_strName );

  if( !options.use_artwork )
    swprintf( text, L"None" );
  else
  {
    swprintf( text, 
              L"%s%s%s%s%s",
              options.use_artwork & ARTWORK_USE_BACKDROPS ? L"Backdrops" : L"",
              (options.use_artwork & ARTWORK_USE_BACKDROPS && (options.use_artwork & ARTWORK_USE_OVERLAYS || options.use_artwork & ARTWORK_USE_BEZELS)) ? L"," : L"",
              options.use_artwork & ARTWORK_USE_OVERLAYS ? L"Overlays" : L"",
              (options.use_artwork & ARTWORK_USE_OVERLAYS && options.use_artwork & ARTWORK_USE_BEZELS) ? L"," : L"",
              options.use_artwork & ARTWORK_USE_BEZELS ? L"Bezels" : L"" );
  }
  DRAWITEM( L"Artwork usage", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawVectorPage
//---------------------------------------------------------------------
void COptionsScreen::DrawVectorPage( void )
{
/*
	options.beam = iniFile.GetProfileInt( "VectorOptions", "BeamWidth", 2 );			            // vector beam width
	options.vector_flicker = iniFile.GetProfileFloat( "VectorOptions", "FlickerEffect", 0.5f );	  // vector beam flicker effect control
	options.vector_intensity = iniFile.GetProfileFloat( "VectorOptions", "BeamIntensity", 1.5f );  // vector beam intensity
	options.translucency = iniFile.GetProfileInt( "VectorOptions", "Translucency", TRUE );      // 1 to enable translucency on vectors
	options.vector_width = iniFile.GetProfileInt( "VectorOptions", "VectorWidth", 640 );	      // requested width for vector games; 0 means default (640)
	options.vector_height = iniFile.GetProfileInt( "VectorOptions", "VectorHeight", 480 );	    // requested height for vector games; 0 means default (480)
*/
  WCHAR text[256] = {0};

  STARTPAGE();
  swprintf( text, L"%lu", options.beam );
  DRAWITEM( L"Beam Width", text );

  swprintf( text, L"%f", options.vector_flicker );
  DRAWITEM( L"Flicker Effect", text );

  swprintf( text, L"%f", options.vector_intensity );
  DRAWITEM( L"Beam Intensity", text );

  DRAWITEM( L"Translucency", options.translucency ? L"Enabled" : L"Disabled" );

  swprintf( text, L"%lu", options.vector_width );
  DRAWITEM( L"Vector Screen Width", text );

  swprintf( text, L"%lu", options.vector_height );
  DRAWITEM( L"Vector Screen Height", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawNetworkPage
//---------------------------------------------------------------------
void COptionsScreen::DrawNetworkPage( void )
{
/*
  g_NetworkConfig.m_networkDisabled = iniFile.GetProfileInt( "Network", "DisableNetworking",  FALSE );
  g_NetworkConfig.m_IPAddr          = iniFile.GetProfileString("Network", "IPAddress",  DEFAULT_IPADDR);
  g_NetworkConfig.m_Gateway         = iniFile.GetProfileString("Network", "Gateway",    DEFAULT_GATEWAY);
  g_NetworkConfig.m_SubnetMask      = iniFile.GetProfileString("Network", "Subnet",     DEFAULT_SUBNETMASK);
  g_NetworkConfig.m_NameServer      = iniFile.GetProfileString("Network", "NameServer", DEFAULT_NAMESERVER);
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  DRAWITEM( L"Networking features", g_NetworkConfig.m_networkDisabled ? L"Disabled" : L"Enabled" );

	mbstowcs( text, g_NetworkConfig.m_IPAddr.c_str(), 255 );
  DRAWITEM( L"IP address", text );

	mbstowcs( text, g_NetworkConfig.m_Gateway.c_str(), 255 );
  DRAWITEM( L"Gateway address", text );

	mbstowcs( text, g_NetworkConfig.m_SubnetMask.c_str(), 255 );
  DRAWITEM( L"Subnet mask", text );

	mbstowcs( text, g_NetworkConfig.m_NameServer.c_str(), 255 );
  DRAWITEM( L"Name server", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawDirectoryPathPage1
//---------------------------------------------------------------------
void COptionsScreen::DrawDirectoryPathPage1( void )
{
/*
  g_FileIOConfig.m_ALTDrive           = iniFile.GetProfileString("Directories", "ALTDrive",            DEFAULT_ALTDRIVE);
  g_FileIOConfig.m_LetterCMapping     = iniFile.GetProfileString( "Directories", "C_Mapping",           DEFAULT_CMAPPING );
  g_FileIOConfig.m_LetterEMapping     = iniFile.GetProfileString( "Directories", "E_Mapping",           DEFAULT_EMAPPING );
  g_FileIOConfig.m_LetterFMapping     = iniFile.GetProfileString( "Directories", "F_Mapping",           DEFAULT_FMAPPING );
  g_FileIOConfig.m_LetterGMapping     = iniFile.GetProfileString( "Directories", "G_Mapping",           DEFAULT_GMAPPING );
  g_FileIOConfig.m_LetterHMapping     = iniFile.GetProfileString( "Directories", "H_Mapping",           DEFAULT_HMAPPING );
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  mbstowcs( text, g_FileIOConfig.m_ALTDrive.c_str(), 255 );
  DRAWITEM( L"Alternate drive letter", text );

  mbstowcs( text, g_FileIOConfig.m_LetterCMapping.c_str(), 255 );
  DRAWITEM( L"C:", text );

  mbstowcs( text, g_FileIOConfig.m_LetterEMapping.c_str(), 255 );
  DRAWITEM( L"E:", text );

  mbstowcs( text, g_FileIOConfig.m_LetterFMapping.c_str(), 255 );
  DRAWITEM( L"F:", text );

  mbstowcs( text, g_FileIOConfig.m_LetterGMapping.c_str(), 255 );
  DRAWITEM( L"G:", text );

  mbstowcs( text, g_FileIOConfig.m_LetterHMapping.c_str(), 255 );
  DRAWITEM( L"H:", text );

  ENDPAGE();
}


//---------------------------------------------------------------------
//	DrawDirectoryPathPage2
//---------------------------------------------------------------------
void COptionsScreen::DrawDirectoryPathPage2( void )
{
/*
  g_FileIOConfig.m_ArtPath            = iniFile.GetProfileString("Directories", "ArtPath",             DEFAULT_ARTPATH);
  g_FileIOConfig.m_AudioPath          = iniFile.GetProfileString("Directories", "AudioPath",           DEFAULT_AUDIOPATH);
  g_FileIOConfig.m_ConfigPath         = iniFile.GetProfileString("Directories", "ConfigPath",          DEFAULT_CONFIGPATH);
  g_FileIOConfig.m_DefaultRomListPath = iniFile.GetProfileString("Directories", "DefaultRomsListPath", DEFAULT_DEFAULTROMLISTPATH);
  g_FileIOConfig.m_GeneralPath        = iniFile.GetProfileString("Directories", "GeneralPath",         DEFAULT_GENERALPATH);
  g_FileIOConfig.m_HDImagePath        = iniFile.GetProfileString("Directories", "HDImagePath",         DEFAULT_HDIMAGEPATH);
  g_FileIOConfig.m_HiScorePath        = iniFile.GetProfileString("Directories", "HiScoresPath",        DEFAULT_HISCOREPATH);
  g_FileIOConfig.m_NVramPath          = iniFile.GetProfileString("Directories", "NVRamPath",           DEFAULT_NVRAMPATH);
  g_FileIOConfig.m_RomBackupPath      = iniFile.GetProfileString("Directories", "BackupPath",          DEFAULT_ROMBACKUPPATH);
  g_FileIOConfig.m_RomPath0           = iniFile.GetProfileString("Directories", "RomsPath0",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath1           = iniFile.GetProfileString("Directories", "RomsPath1",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath2           = iniFile.GetProfileString("Directories", "RomsPath2",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_RomPath3           = iniFile.GetProfileString("Directories", "RomsPath3",           DEFAULT_ROMPATH);
  g_FileIOConfig.m_ScreenshotPath     = iniFile.GetProfileString( "Directories", "ScreenshotPath",     DEFAULT_SCREENSHOTPATH );
  g_FileIOConfig.m_AutoBootSavePath   = iniFile.GetProfileString( "Directories", "AutoBootSavePath",    DEFAULT_BOOTSAVESTATE );
*/
  WCHAR text[256] = {0};

  STARTPAGE();

  mbstowcs( text, g_FileIOConfig.m_RomPath0.c_str(), 255 );
  DRAWITEM( L"ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_RomPath1.c_str(), 255 );
  DRAWITEM( L"More ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_RomPath2.c_str(), 255 );
  DRAWITEM( L"Yet More ROM Files", text );
  mbstowcs( text, g_FileIOConfig.m_RomPath3.c_str(), 255 );
  DRAWITEM( L"Even More ROM Files", text );

  mbstowcs( text, g_FileIOConfig.m_RomBackupPath.c_str(), 255 );
  DRAWITEM( L"Removed ROMs", text );

  mbstowcs( text, g_FileIOConfig.m_GeneralPath.c_str(), 255 );
  DRAWITEM( L"General", text );

  mbstowcs( text, g_FileIOConfig.m_ArtPath.c_str(), 255 );
  DRAWITEM( L"Art", text );

  mbstowcs( text, g_FileIOConfig.m_AudioPath.c_str(), 255 );
  DRAWITEM( L"Samples (Audio)", text );

  mbstowcs( text, g_FileIOConfig.m_ConfigPath.c_str(), 255 );
  DRAWITEM( L"Config (CFG's)", text );

  mbstowcs( text, g_FileIOConfig.m_HDImagePath.c_str(), 255 );
  DRAWITEM( L"HD images (CHD's)", text );

  mbstowcs( text, g_FileIOConfig.m_HiScorePath.c_str(), 255 );
  DRAWITEM( L"High scores (highscore.dat)", text );

  mbstowcs( text, g_FileIOConfig.m_NVramPath.c_str(), 255 );
  DRAWITEM( L"NVRAM, state files", text );

  mbstowcs( text, g_FileIOConfig.m_ScreenshotPath.c_str(), 255 );
  DRAWITEM( L"Screenshots", text );

  mbstowcs( text, g_FileIOConfig.m_AutoBootSavePath.c_str(), 255 );
  DRAWITEM( L"Autoboot save states", text );

  ENDPAGE();
}

//---------------------------------------------------------------------
//	DrawROMListPage
//---------------------------------------------------------------------
void COptionsScreen::DrawROMListPage( void )
{
  static WCHAR *sortMode[] = {  L"By name", 
                                L"By ROM status",
                                L"By manufacturer", 
                                L"By year", 
                                L"By parent ROM",
                                L"By number of players", 
                                L"By favorite status",
                                L"By number of times played",
                                L"By genre" };

  static WCHAR *displayMode[] = { L"Verbose list",
                                  L"Simple list",
                                  L"Detailed single item" };

  STARTPAGE();

  DRAWITEM( L"Display mode", displayMode[g_romListOptions.m_displayMode] );

  DRAWITEM( L"Colorize ROM status", g_romListOptions.m_showROMStatus ? L"Yes" : L"No" );

  DRAWITEM( L"Sort mode", sortMode[(UINT32)g_romListOptions.m_sortMode] );

  DRAWITEM( L"Filter clones",               g_romListOptions.m_filterMode & FM_CLONE ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"slow/frameskip\"",   g_romListOptions.m_filterMode & FM_ROMSTATUS_SLOW ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"out of memory\"",    g_romListOptions.m_filterMode & FM_ROMSTATUS_OUTOFMEM ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"crashes in game\"",  g_romListOptions.m_filterMode & FM_ROMSTATUS_CRASH ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"other nonworking\"", g_romListOptions.m_filterMode & FM_ROMSTATUS_NONWORKING ? L"Yes" : L"No" );
  DRAWITEM( L"Filter \"disliked\"",         g_romListOptions.m_filterMode & FM_DISLIKED ? L"Yes" : L"No" );

  ENDPAGE();
}








//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeGeneralPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Cheats (enabled/disabled)
  case 0:
    options.cheat = !options.cheat;
    break;

    // Cheat file
  case 1:
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
	    m_virtualKeyboard->SetData( cheatfile );
      WaitForNoInput();
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
          // Free cheatfile, as it was strdup'd in MAMEoXUtil.cpp
        free( (void*)cheatfile );
        if( !(cheatfile = strdup( m_virtualKeyboard->GetData().c_str() )) )
          Die( m_displayDevice, "Out of memory setting the cheatfile!" );
      }
    }
    break;
/*
    // History file
  case 2:
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
	    m_virtualKeyboard->SetData( history_filename );
      WaitForNoInput();
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
          // Free cheatfile, as it was strdup'd in MAMEoXUtil.cpp
        free( (void*)history_filename );
        if( !(history_filename = strdup( m_virtualKeyboard->GetData().c_str() )) )
          Die( m_displayDevice, "Out of memory setting the history_filename!" );
      }
    }
    break;
*/
      // Disclaimer (skipped/shown)
  case 2:
    options.skip_disclaimer = !options.skip_disclaimer;
    break;

    // Game info (skipped/shown)
  case 3:
    options.skip_gameinfo = !options.skip_gameinfo;
    break;

    // Screensaver
  case 4:
    if( !movingRight )
    {
      if( g_screensaverTimeout )
        --g_screensaverTimeout;
      else
        g_screensaverTimeout = MAX_SCREENSAVER_TIMEOUT;
    }
    else
    {
      if( g_screensaverTimeout < MAX_SCREENSAVER_TIMEOUT )
        ++g_screensaverTimeout;
      else
        g_screensaverTimeout = 0;
    }
  }
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeSoundPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Sound Processing
  case 0:
    g_soundEnabled = !g_soundEnabled;
    break;

    // Sample Rate
  case 1:
    if( options.samplerate == 44100 )
      options.samplerate = 22700;
    else
      options.samplerate = 44100;
    break;

    // Audio Samples
  case 2:
    options.use_samples = !options.use_samples;
    break;

    // Audio Filter
  case 3:
    options.use_filter = !options.use_filter;
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeVideoPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {

      // VSYNC
  case 0:
    g_rendererOptions.m_vsync = !g_rendererOptions.m_vsync;
    break;

     // Framerate throttling
  case 1:
    g_rendererOptions.m_throttleFramerate = !g_rendererOptions.m_throttleFramerate;
    break;

    // Frameskip mode
  case 2:
    if( !movingRight )
    {
        // Reduce
      if( g_rendererOptions.m_frameskip == 0 )
        g_rendererOptions.m_frameskip = AUTO_FRAMESKIP;
      else if( g_rendererOptions.m_frameskip == AUTO_FRAMESKIP )
        g_rendererOptions.m_frameskip = FRAMESKIP_LEVELS - 1;
      else
        --g_rendererOptions.m_frameskip;
    }
    else
    {
        // Increment
      if( g_rendererOptions.m_frameskip == AUTO_FRAMESKIP )
        g_rendererOptions.m_frameskip = 0;
      else if( g_rendererOptions.m_frameskip == FRAMESKIP_LEVELS - 1 )
        g_rendererOptions.m_frameskip = AUTO_FRAMESKIP;
      else
        ++g_rendererOptions.m_frameskip;
    }
    break;

     // Preserve aspect ratio
  case 3:
    g_rendererOptions.m_preserveAspectRatio = !g_rendererOptions.m_preserveAspectRatio;
    break;

    // Screen rotation
  case 4:
    if( !movingRight )
    {
      if( g_rendererOptions.m_screenRotation > SR_0 )
        g_rendererOptions.m_screenRotation = (screenrotation_t)( (LONG)g_rendererOptions.m_screenRotation - 1 );
      else
        g_rendererOptions.m_screenRotation = SR_270;
    }
    else
    {
      if( g_rendererOptions.m_screenRotation < SR_270 )
        g_rendererOptions.m_screenRotation = (screenrotation_t)( (LONG)g_rendererOptions.m_screenRotation + 1 );
      else
        g_rendererOptions.m_screenRotation = SR_0;
    }
    break;

    // Brightness
  case 5:
    {
      if( !movingRight )
        options.brightness -= 0.01f;
      else
        options.brightness += 0.01f;

      if( options.brightness < 0.0f )
        options.brightness = 0.0f;
      else if( options.brightness > 1.0f )
        options.brightness = 1.0f;
    }
    break;

    // Paused brightness
  case 6:
    {
      if( !movingRight )
        options.pause_bright -= 0.01f;
      else
        options.pause_bright += 0.01f;

      if( options.pause_bright < 0.0f )
        options.pause_bright = 0.0f;
      else if( options.pause_bright > 1.0f )
        options.pause_bright = 1.0f;
    }
    break;

    // Gamma
  case 7:
    {
      if( !movingRight )
        options.gamma -= 0.01f;
      else
        options.gamma += 0.01f;

      if( options.gamma < 0.0f )
        options.gamma = 0.0f;
      else if( options.gamma > 2.0f )
        options.gamma = 2.0f;
    }
    break;
/*
    // Color depth
  case 5:
    {
      if( options.color_depth == 32 )
        options.color_depth = 15;
      else
        options.color_depth = 32;
    }
    break;    
*/

    // Minification filter
  case 8:
    if( !movingRight )
    {
      if( g_rendererOptions.m_minFilter > D3DTEXF_POINT )
        g_rendererOptions.m_minFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_minFilter - 1 );
      else
        g_rendererOptions.m_minFilter = D3DTEXF_GAUSSIANCUBIC;
    }
    else
    {
      if( g_rendererOptions.m_minFilter < D3DTEXF_GAUSSIANCUBIC )
        g_rendererOptions.m_minFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_minFilter + 1 );
      else
        g_rendererOptions.m_minFilter = D3DTEXF_POINT;
    }
    break;

    // Magnification filter
  case 9:
    if( !movingRight )
    {
      if( g_rendererOptions.m_magFilter > D3DTEXF_POINT )
        g_rendererOptions.m_magFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_magFilter - 1 );
      else
        g_rendererOptions.m_magFilter = D3DTEXF_GAUSSIANCUBIC;
    }
    else
    {
      if( g_rendererOptions.m_magFilter < D3DTEXF_GAUSSIANCUBIC )
        g_rendererOptions.m_magFilter = (D3DTEXTUREFILTERTYPE)( (LONG)g_rendererOptions.m_magFilter + 1 );
      else
        g_rendererOptions.m_magFilter = D3DTEXF_POINT;
    }
    break;
	case 10:
		{
			CGFilterManager oFilterMgr;
			MapFilters mapFilters = oFilterMgr.GetAvailableFilters();

			if (!movingRight)
			{
				g_rendererOptions.m_FilterType -= 1;
			}
			else
			{
				g_rendererOptions.m_FilterType += 1;
			}

			if (g_rendererOptions.m_FilterType > mapFilters.size()-1)
			{
				g_rendererOptions.m_FilterType = 0;
			}
			else if (g_rendererOptions.m_FilterType < 0)
			{
				g_rendererOptions.m_FilterType = mapFilters.size()-1;
			}
		}
		break;
    // Artwork usage
  case 11:
    {
      DWORD artworkState[8] = { 0, 
                                ARTWORK_USE_BACKDROPS, 
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS,
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_BEZELS,
                                ARTWORK_USE_OVERLAYS,
                                ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS,
                                ARTWORK_USE_BEZELS,
                                ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS };

        // Figure out our current state
      DWORD currentState = 0;
      for( ; artworkState[currentState] != options.use_artwork && currentState < (sizeof(artworkState) / sizeof(DWORD)); ++currentState )
        ;

        // Validate the currentState
      if( currentState == (sizeof(artworkState) / sizeof(DWORD)) )
        currentState = 0;

      if( !movingRight )
      {
        if( currentState )
          --currentState;
        else
          currentState = (sizeof(artworkState) / sizeof(DWORD)) - 1;
      }
      else
      {
        if( currentState < (sizeof(artworkState) / sizeof(DWORD)) - 1 )
          ++currentState;
        else
          currentState = 0;
      }

      options.use_artwork = artworkState[currentState];
    }
    break;

  }
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeVectorPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
  case 0:
    {
      if( !movingRight && options.beam > 1 )
        --options.beam;
      else if( movingRight && options.beam < 5 )
        ++options.beam;
    }
    break;

  case 1:
    {
      if( !movingRight )
        options.vector_flicker -= 0.01f;
      else
        options.vector_flicker += 0.01f;

      if( options.vector_flicker < 0.0f )
        options.vector_flicker = 0.0f;
      else if( options.vector_flicker > 1.0f )
        options.vector_flicker = 1.0f;
    }
    break;

  case 2:
    {
      if( !movingRight )
        options.vector_intensity -= 0.01f;
      else
        options.vector_intensity += 0.01f;

      if( options.vector_intensity < 0.0f )
        options.vector_intensity = 0.0f;
      else if( options.vector_intensity > 2.0f )
        options.vector_intensity = 2.0f;
    }
    break;

  case 3:
    {
      options.translucency = !options.translucency;
    }
    break;

  case 4:
    {
      if( !movingRight && options.vector_width > 100 )
        --options.vector_width;
      else if( movingRight && options.vector_width < 640 )
        ++options.vector_width;
    }
    break;

  case 5:
    {
      if( !movingRight && options.vector_height > 100 )
        --options.vector_height;
      else if( movingRight && options.vector_height < 480 )
        ++options.vector_height;
    }
    break;
  }
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeNetworkPage( BOOL movingRight )
{
  if( !m_cursorPosition )
  {
    g_NetworkConfig.m_networkDisabled = !g_NetworkConfig.m_networkDisabled;
  }
  else
  {
    if( !m_virtualKeyboardMode )
    {
      m_virtualKeyboardMode = TRUE;
      WaitForNoInput();
      switch( (DWORD)m_cursorPosition )
      {
        // IP address
      case 1:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_IPAddr );
        break;

        // Gateway address
      case 2:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_Gateway );
        break;

        // Subnet mask
      case 3:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_SubnetMask );
        break;

        // Name server
      case 4:
	      m_virtualKeyboard->SetData( g_NetworkConfig.m_NameServer );
        break;
      }
    }
    else
    {
      if( m_virtualKeyboard->IsInputAccepted() )
      {
        switch( (DWORD)m_cursorPosition )
        {
          // IP address
        case 1:
	        g_NetworkConfig.m_IPAddr = m_virtualKeyboard->GetData();
          break;

          // Gateway address
        case 2:
	        g_NetworkConfig.m_Gateway = m_virtualKeyboard->GetData();
          break;

          // Subnet mask
        case 3:
	        g_NetworkConfig.m_SubnetMask = m_virtualKeyboard->GetData();
          break;

          // Name server
        case 4:
	        g_NetworkConfig.m_NameServer = m_virtualKeyboard->GetData();
          break;
        }
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage1
//---------------------------------------------------------------------
void COptionsScreen::ChangeDirectoryPathPage1( BOOL movingRight )
{
  if( !m_virtualKeyboardMode )
  {
    m_virtualKeyboardMode = TRUE;
    WaitForNoInput();
    switch( (DWORD)m_cursorPosition )
    {
      // Alternate drive letter
    case 0:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_ALTDrive );
      break;

      // C:
    case 1:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_LetterCMapping );
      break;

      // E:
    case 2:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_LetterEMapping );
      break;
    
      // F:
    case 3:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_LetterFMapping );
      break;

      // G:
    case 4:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_LetterGMapping );
      break;

      // H:
    case 5:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_LetterHMapping );
      break;
    }
  }
  else
  {
    if( m_virtualKeyboard->IsInputAccepted() )
    {
      switch( (DWORD)m_cursorPosition )
      {
        // Alternate drive letter
      case 0:
        g_FileIOConfig.m_ALTDrive = m_virtualKeyboard->GetData();
        break;

        // C:
      case 1:
        g_FileIOConfig.m_LetterCMapping = m_virtualKeyboard->GetData();
        break;

        // E:
      case 2:
        g_FileIOConfig.m_LetterEMapping = m_virtualKeyboard->GetData();
        break;
      
        // F:
      case 3:
        g_FileIOConfig.m_LetterFMapping = m_virtualKeyboard->GetData();
        break;

        // G:
      case 4:
        g_FileIOConfig.m_LetterGMapping = m_virtualKeyboard->GetData();
        break;

        // H:
      case 5:
        g_FileIOConfig.m_LetterHMapping = m_virtualKeyboard->GetData();
        break;
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage2
//---------------------------------------------------------------------
void COptionsScreen::ChangeDirectoryPathPage2( BOOL movingRight )
{
  if( !m_virtualKeyboardMode )
  {
    m_virtualKeyboardMode = TRUE;
    WaitForNoInput();
    switch( (DWORD)m_cursorPosition )
    {
      // ROM Files
    case 0:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_RomPath0 );
      break;

      // More ROM Files
    case 1:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_RomPath1 );
      break;

      // Yet More ROM Files
    case 2:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_RomPath2 );
      break;

      // Even More ROM Files
    case 3:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_RomPath3 );
      break;

      // Removed ROMs
    case 4:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_RomBackupPath );
      break;

      // General
    case 5:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_GeneralPath );
      break;

      // Art
    case 6:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_ArtPath );
      break;

      // Samples (Audio)
    case 7:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_AudioPath );
      break;

      // Config (CFG's)
    case 8:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_ConfigPath );
      break;

      // HD images (CHD's)
    case 9:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_HDImagePath );
      break;

      // High scores (highscore.dat)
    case 10:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_HiScorePath );
      break;

      // NVRAM, state files
    case 11:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_NVramPath );
      break;

      // Screenshots
    case 12:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_ScreenshotPath );
      break;

      // Autoboot save states
    case 13:
      m_virtualKeyboard->SetData( g_FileIOConfig.m_AutoBootSavePath );
      break;
    }
  }
  else
  {
    if( m_virtualKeyboard->IsInputAccepted() )
    {
      switch( (DWORD)m_cursorPosition )
      {
        // ROM Files
      case 0:
        g_FileIOConfig.m_RomPath0 = m_virtualKeyboard->GetData();
        break;

        // More ROM Files
      case 1:
        g_FileIOConfig.m_RomPath1 = m_virtualKeyboard->GetData();
        break;

        // Yet More ROM Files
      case 2:
        g_FileIOConfig.m_RomPath2 = m_virtualKeyboard->GetData();
        break;

        // Even More ROM Files
      case 3:
        g_FileIOConfig.m_RomPath3 = m_virtualKeyboard->GetData();
        break;

        // Removed ROMs
      case 4:
        g_FileIOConfig.m_RomBackupPath = m_virtualKeyboard->GetData();
        break;

        // General
      case 5:
        g_FileIOConfig.m_GeneralPath = m_virtualKeyboard->GetData();
        break;

        // Art
      case 6:
        g_FileIOConfig.m_ArtPath = m_virtualKeyboard->GetData();
        break;

        // Samples (Audio)
      case 7:
        g_FileIOConfig.m_AudioPath = m_virtualKeyboard->GetData();
        break;

        // Config (CFG's)
      case 8:
        g_FileIOConfig.m_ConfigPath = m_virtualKeyboard->GetData();
        break;

        // HD images (CHD's)
      case 9:
        g_FileIOConfig.m_HDImagePath = m_virtualKeyboard->GetData();
        break;

        // High scores (highscore.dat)
      case 10:
        g_FileIOConfig.m_HiScorePath = m_virtualKeyboard->GetData();
        break;

        // NVRAM, state files
      case 11:
        g_FileIOConfig.m_NVramPath = m_virtualKeyboard->GetData();
        break;

        // Screenshots
      case 12:
        g_FileIOConfig.m_ScreenshotPath = m_virtualKeyboard->GetData();
        break;

        // Autoboot save states
      case 13:
        g_FileIOConfig.m_AutoBootSavePath = m_virtualKeyboard->GetData();
        break;
      }
    }
  }
}

//---------------------------------------------------------------------
//  ChangeROMListPage
//---------------------------------------------------------------------
void COptionsScreen::ChangeROMListPage( BOOL movingRight )
{
  switch( (DWORD)m_cursorPosition )
  {
    // Display mode
  case 0:
    if( !movingRight )
    {
      if( g_romListOptions.m_displayMode != DM_VERBOSELIST )
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(g_romListOptions.m_displayMode - 1);
      else
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(DM_LAST - 1);
    }
    else
    {
      if( g_romListOptions.m_displayMode != (ROMListDisplayMode)(DM_LAST - 1) )
        g_romListOptions.m_displayMode = (ROMListDisplayMode)(g_romListOptions.m_displayMode + 1);
      else
        g_romListOptions.m_displayMode = DM_VERBOSELIST;
    }
    break;

    // Colorize ROM status
  case 1:
    g_romListOptions.m_showROMStatus = !g_romListOptions.m_showROMStatus;
    break;

    // Sort mode
  case 2:
    if( !movingRight )
    {
      if( g_romListOptions.m_sortMode != SM_BYNAME )
        g_romListOptions.m_sortMode = (ROMListSortMode)((UINT32)g_romListOptions.m_sortMode - 1);
      else
        g_romListOptions.m_sortMode = (ROMListSortMode)(SM_LAST - 1);
    }
    else
    {
      if( g_romListOptions.m_sortMode < SM_LAST - 1 )
        g_romListOptions.m_sortMode = (ROMListSortMode)((UINT32)g_romListOptions.m_sortMode + 1);
      else
        g_romListOptions.m_sortMode = SM_BYNAME;
    }
    break;

    // Filter clones
  case 3:
    if( g_romListOptions.m_filterMode & FM_CLONE )
      g_romListOptions.m_filterMode &= ~FM_CLONE;
    else
      g_romListOptions.m_filterMode |= FM_CLONE;
    break;

    // Filter "slow"
  case 4:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_SLOW )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_SLOW;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_SLOW;
    break;

    // Filter "out of mem"
  case 5:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_OUTOFMEM )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_OUTOFMEM;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_OUTOFMEM;
    break;

    // Filter "crashes"
  case 6:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_CRASH )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_CRASH;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_CRASH;
    break;

    // Filter "other nonworking"
  case 7:
    if( g_romListOptions.m_filterMode & FM_ROMSTATUS_NONWORKING )
      g_romListOptions.m_filterMode &= ~FM_ROMSTATUS_NONWORKING;
    else
      g_romListOptions.m_filterMode |= FM_ROMSTATUS_NONWORKING;
    break;

    // Filter "disliked"
  case 8:
    if( g_romListOptions.m_filterMode & FM_DISLIKED )
      g_romListOptions.m_filterMode &= ~FM_DISLIKED;
    else
      g_romListOptions.m_filterMode |= FM_DISLIKED;
    break;
  }
}











//---------------------------------------------------------------------
//  DrawGeneralPage
//---------------------------------------------------------------------
void DrawGeneralPage( COptionsScreen *ptr )
{
  ptr->DrawGeneralPage();
}

//---------------------------------------------------------------------
//  DrawSoundPage
//---------------------------------------------------------------------
void DrawSoundPage( COptionsScreen *ptr )
{
  ptr->DrawSoundPage();
}

//---------------------------------------------------------------------
//  DrawVideoPage
//---------------------------------------------------------------------
void DrawVideoPage( COptionsScreen *ptr )
{
  ptr->DrawVideoPage();
}

//---------------------------------------------------------------------
//  DrawVectorPage
//---------------------------------------------------------------------
void DrawVectorPage( COptionsScreen *ptr )
{
  ptr->DrawVectorPage();
}

//---------------------------------------------------------------------
//  DrawNetworkPage
//---------------------------------------------------------------------
void DrawNetworkPage( COptionsScreen *ptr )
{
  ptr->DrawNetworkPage();
}

//---------------------------------------------------------------------
//  DrawDirectoryPathPage1
//---------------------------------------------------------------------
void DrawDirectoryPathPage1( COptionsScreen *ptr )
{
  ptr->DrawDirectoryPathPage1();
}

//---------------------------------------------------------------------
//  DrawDirectoryPathPage2
//---------------------------------------------------------------------
void DrawDirectoryPathPage2( COptionsScreen *ptr )
{
  ptr->DrawDirectoryPathPage2();
}

//---------------------------------------------------------------------
//  DrawROMListPage
//---------------------------------------------------------------------
void DrawROMListPage( COptionsScreen *ptr )
{
  ptr->DrawROMListPage();
}



//---------------------------------------------------------------------
//  ChangeGeneralPage
//---------------------------------------------------------------------
void ChangeGeneralPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeGeneralPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeSoundPage
//---------------------------------------------------------------------
void ChangeSoundPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeSoundPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeVideoPage
//---------------------------------------------------------------------
void ChangeVideoPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeVideoPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeVectorPage
//---------------------------------------------------------------------
void ChangeVectorPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeVectorPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeNetworkPage
//---------------------------------------------------------------------
void ChangeNetworkPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeNetworkPage( movingRight );
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage1
//---------------------------------------------------------------------
void ChangeDirectoryPathPage1( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeDirectoryPathPage1( movingRight );
}

//---------------------------------------------------------------------
//  ChangeDirectoryPathPage2
//---------------------------------------------------------------------
void ChangeDirectoryPathPage2( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeDirectoryPathPage2( movingRight );
}

//---------------------------------------------------------------------
//  ChangeROMListPage
//---------------------------------------------------------------------
void ChangeROMListPage( COptionsScreen *ptr, BOOL movingRight )
{
  ptr->ChangeROMListPage( movingRight );
}


