/**
	* \file			SkinChooserScreen.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "SkinChooserScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"
#include "System_IniFile.h"

#include <string>
#include <vector>
#include <algorithm>


//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================

  //--- Layout defines -----------------------------------------
#define HEADER_COLOR          D3DCOLOR_XRGB( 1, 1, 1 )
#define HELPITEM_COLOR        D3DCOLOR_XRGB( 20, 20, 20 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 1, 1, 1 )
#define ITEM_WARNING_COLOR    D3DCOLOR_XRGB( 100, 100, 0 )
#define ITEM_NONWORKING_COLOR D3DCOLOR_XRGB( 240, 20, 20 )

#define HIGHLIGHTBAR_COLOR      D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define SCROLLICON_COLOR        D3DCOLOR_XRGB( 255, 255, 255 )
#define SPACER_COLOR            D3DCOLOR_ARGB( 100, 255, 255, 255 )
#define NOSCREENSHOT_COLOR      D3DCOLOR_XRGB( 1, 1, 1 )
#define NOSCREENSHOTTEXT_COLOR  D3DCOLOR_XRGB( 255, 255, 255 )

#define TITLEBAR_ROW          99
#define FIRSTDATA_ROW         124

#define HIGHLIGHTBAR_LEFT     34
#define HIGHLIGHTBAR_RIGHT    605

#define SPACER_WIDTH          2

#define NAME_COLUMN           42
#define MANUFACTURER_COLUMN   305
#define YEAR_COLUMN           437
#define NUMPLAYERS_COLUMN     506
#define CLONE_COLUMN          535 
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text

#define SCROLLUP_TOP          122
#define SCROLLUP_RIGHT        608
#define SCROLLUP_LEFT         (SCROLLUP_RIGHT - 32)
#define SCROLLUP_BOTTOM       (SCROLLUP_TOP + 32)

#define SCROLLDOWN_BOTTOM     451
#define SCROLLDOWN_TOP        (SCROLLDOWN_BOTTOM - 32)
#define SCROLLDOWN_RIGHT      608
#define SCROLLDOWN_LEFT       (SCROLLDOWN_RIGHT - 32)

  //-- Button help messages ------
#define HELP_START_ICON_X   200
#define HELP_START_ICON_Y   40
#define HELP_START_TEXT_X   (HELP_START_ICON_X + m_textureSet.GetSTARTButtonWidth() + 4)
#define HELP_START_TEXT_Y   (HELP_START_ICON_Y + 5)


#define DETAIL_SCREENSHOT_RIGHT   SCROLLUP_LEFT
#define DETAIL_SCREENSHOT_TOP     127
#define DETAIL_SCREENSHOT_LEFT    (DETAIL_SCREENSHOT_RIGHT - 128)
#define DETAIL_SCREENSHOT_BOTTOM  (DETAIL_SCREENSHOT_TOP + ( (FLOAT)(DETAIL_SCREENSHOT_RIGHT - DETAIL_SCREENSHOT_LEFT) * 3.0f / 4.0f))

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							3

	// Timeout values for the cursor movement acceleration bands
	// Values are measured in seconds
#define SBTIMEOUT_FASTEST				3.0f
#define SBTIMEOUT_FASTER				2.0f
#define SBTIMEOUT_FAST					1.0f

	// Multiplier values for the cursor movement acceleration bands
	// Values are multiplied against the cursorVelocity
#define SBMULTIPLIER_FASTEST		5.0f
#define SBMULTIPLIER_FASTER			2.5f
#define SBMULTIPLIER_FAST				1.5f

	// Analog trigger deadzone
#define DEADZONE								0.25f
#define DEADZONE_RECTIFIER			1.0f / (1.0f - DEADZONE)
#define CURSOR_SPEED            0.8f                // The cursor velocity modifier

	// Number of seconds between valid DPAD readings
#define DPADCURSORMOVE_TIMEOUT	0.20f



//= G L O B A L = V A R S ==============================================



//= P R O T O T Y P E S ================================================
extern "C" void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems ); // Defined in main.cpp

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	FindSkins
//---------------------------------------------------------------------
BOOL CSkinChooserScreen::FindSkins( void )
{
	CStdString path = g_FileIOConfig.m_skinPath;

	std::vector< CStdString > zipFileNames;
	WIN32_FIND_DATA findData;

  HANDLE findHandle = FindFirstFile( (path + "\\*").c_str(), &findData );
  if( findHandle == INVALID_HANDLE_VALUE )
  {
    PRINTMSG(( T_INFO, "Could not find files!" ));
    return FALSE;
  }

		// Search the subdirectories for skin.ini files and preview.png's
	do {
    CStdString basepath = findData.cFileName;
		if( basepath[0] == '.' )
			continue;

		CStdString fullBasePath = path + "\\" + basepath;
		CStdString iniFile = fullBasePath + "\\skin.ini";
		
			// See if there's a skin.ini file
		osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, iniFile.c_str(), "r" );
		if( file )
		{
			osd_fclose( file );

			PRINTMSG(( T_INFO, "Skin %s found!", basepath.c_str() ));

				// Try to load a preview for this skin
			RECT rct = { 0, 0, 0, 0 };
			LPDIRECT3DTEXTURE8 texture = NULL;
			CStdString previewFile = fullBasePath + "\\preview.png";

			PRINTMSG(( T_INFO, "Load texture: %s", previewFile.c_str() ));

		  if( !LoadPNGToTexture( previewFile, &texture, &rct ) )
				texture = NULL;

			PRINTMSG(( T_INFO, "Texture addr 0x%X!", texture ));

				// We have a skin on our hands
			m_skinResourceVector.push_back( new CSkinResource( basepath.c_str(), texture, rct ) );

			PRINTMSG(( T_INFO, "Added skin!" ));
		}

	} while( FindNextFile( findHandle, &findData ) );
  FindClose( findHandle );


	m_numLinesInList = m_skinResourceVector.size();
	PRINTMSG(( T_INFO, "Found %lu skins.", m_skinResourceVector.size() ));

	return TRUE;
}

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CSkinChooserScreen::MoveCursor( CInputManager &gp, BOOL useSpeedBanding )
{
   	// Handle user input
  if( gp.IsButtonPressed( GP_A ) || gp.IsKeyPressed( VK_RETURN )  )
	{
			// Load the selected skin, if it's not the current one
	}

		// General idea taken from XMAME
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
    BYTE vkArrowKeys[2] = { VK_UP, VK_DOWN };
		m_dpadCursorDelay -= elapsedTime;
    if( m_dpadCursorDelay < 0.0f || 
        (!gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_DPAD_DOWN | GP_LA_UP | GP_LA_DOWN ) &&
         !gp.IsOneOfKeysPressed( vkArrowKeys, 2 )) )
			m_dpadCursorDelay = 0.0f;
	}


  NormalModeMoveCursor( gp, elapsedTime );
}

//---------------------------------------------------------------------
//  NormalModeMoveCursor
//---------------------------------------------------------------------
void CSkinChooserScreen::NormalModeMoveCursor( CInputManager &gp, FLOAT elapsedTime )
{
		// The combined trigger offset, scaled to the range [-1.0f,1.0f]
  FLOAT cursorVelocity = 0.0f;

    // Loop through all the gamepads, checking the right and left triggers
    // until one with an offset is found
  for( UINT32 i = 0; i < 4 && cursorVelocity == 0.0f; ++i )
  {
    CGamepad *gamepad;
    if( (gamepad = gp.GetGamepad( i )) )
      cursorVelocity = (FLOAT)(gamepad->GetAnalogButtonState( GP_RIGHT_TRIGGER ) - gamepad->GetAnalogButtonState( GP_LEFT_TRIGGER ));
  }

  cursorVelocity /= 256.0f;

		// Reset the speed band timeout
	if( cursorVelocity < 0.99f && cursorVelocity > -0.99f )
		m_cursorSpeedBandTimeout = 0.0f;
	else
	{
		m_cursorSpeedBandTimeout += elapsedTime;
		if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTEST )
			m_cursorSpeedBandTimeout = SBTIMEOUT_FASTEST;
	}

		// DPAD overrides the triggers
  if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that subtracting 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = -1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
	else
	{
			// Handle the dead zone
		if( cursorVelocity > -DEADZONE && cursorVelocity < DEADZONE )
				return;

			// Rescale the cursor velocity to be [-CURSOR_SPEED,CURSOR_SPEED] without the deadzone
    if( cursorVelocity > 0 )
      cursorVelocity -= DEADZONE;
    else
      cursorVelocity += DEADZONE;
		cursorVelocity *= DEADZONE_RECTIFIER;
    cursorVelocity *= CURSOR_SPEED;
	}

	  // Apply speed bands
	if( m_cursorSpeedBandTimeout == SBTIMEOUT_FASTEST )
		cursorVelocity *= SBMULTIPLIER_FASTEST;
	else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTER )
		cursorVelocity *= SBMULTIPLIER_FASTER;
	else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FAST )
		cursorVelocity *= SBMULTIPLIER_FAST;


	DWORD pageSize = (m_skinResourceVector.size() < MAXPAGESIZE ? m_skinResourceVector.size() : MAXPAGESIZE);
	ULONG pageHalfwayPoint = (pageSize >> 1);
	ULONG maxPageOffset = m_skinResourceVector.size() - pageSize;

	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_cursorPosition < pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) < pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (ULONG)(cursorVelocity + m_pageOffset) <= maxPageOffset )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) <= (FLOAT)maxPageOffset )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity -= ((FLOAT)maxPageOffset - m_pageOffset);
					m_pageOffset = (FLOAT)maxPageOffset;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (ULONG)m_pageOffset > maxPageOffset )
			m_pageOffset = (FLOAT)maxPageOffset;
		if( (ULONG)m_cursorPosition > (pageSize - 1) )
			m_cursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (ULONG)m_cursorPosition > pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) > pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (LONG)(cursorVelocity + m_pageOffset) >= 0 )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) >= 0.0f )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity += m_pageOffset;
					m_pageOffset = 0.0f;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (LONG)m_pageOffset < 0 )
			m_pageOffset = 0.0f;
		if( (LONG)m_cursorPosition < 0 )
			m_cursorPosition = 0.0f;
	}
}


//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CSkinChooserScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil



    // Render the backdrop texture
  RenderBackdrop();
  m_menuRenderer->Draw( FALSE, FALSE );


    //-- Draw the help text --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );

  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

  m_displayDevice->SetTexture( 0, m_textureSet.GetButtonIcons() );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );


  FLOAT ulX, ulY;

    //-- START button ------------------------------------------------
  ulX = HELP_START_ICON_X;
  ulY = HELP_START_ICON_Y;
  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetSTARTButtonLeft(), m_textureSet.GetSTARTButtonTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, ulX, ulY, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetSTARTButtonRight(), m_textureSet.GetSTARTButtonTop() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, ulX + m_textureSet.GetSTARTButtonWidth(), ulY, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetSTARTButtonRight(), m_textureSet.GetSTARTButtonBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, ulX + m_textureSet.GetSTARTButtonWidth(), ulY + m_textureSet.GetSTARTButtonHeight(), 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetSTARTButtonLeft(), m_textureSet.GetSTARTButtonBottom() );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, ulX, ulY + m_textureSet.GetSTARTButtonHeight(), 1.0f, 1.0f );
  m_displayDevice->End();


    // Now render the text messages
	m_fontSet.LargeThinFont().Begin();
    m_fontSet.LargeThinFont().DrawText( HELP_START_TEXT_X,
                                        HELP_START_TEXT_Y,
                                        HELPITEM_COLOR,
                                        L"Menu" );
  m_fontSet.LargeThinFont().End();

  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     FALSE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    FALSE );
  m_displayDevice->SetTexture( 0, NULL );

	DrawSkinList();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}



//---------------------------------------------------------------------
//	DrawSkinList
//---------------------------------------------------------------------
void CSkinChooserScreen::DrawSkinList( void )
{
	WCHAR name[256];

	m_fontSet.SmallThinFont().Begin();
		mbstowcs( name, "Skin Chooser", 255 );
		m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, 
																				TITLEBAR_ROW, 
																				HEADER_COLOR, 
																				name, 
																				XBFONT_TRUNCATED,
																				TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );
	m_fontSet.SmallThinFont().End();

    // Render the highlight bar for the selected item
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  m_displayDevice->SetTexture( 0, NULL );

	FLOAT entryHeight = (DETAIL_SCREENSHOT_BOTTOM - DETAIL_SCREENSHOT_TOP + 10);
  FLOAT selectedItemYPos = DETAIL_SCREENSHOT_TOP + entryHeight * (ULONG)m_cursorPosition;

  m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, selectedItemYPos, 1.0f, 1.0f );
    
    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_RIGHT, selectedItemYPos + entryHeight, 1.0f, 1.0f );

    m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, HIGHLIGHTBAR_COLOR );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, HIGHLIGHTBAR_LEFT, selectedItemYPos + entryHeight, 1.0f, 1.0f );
  m_displayDevice->End();

	for( int i = (ULONG)m_pageOffset; i < (ULONG)m_pageOffset + GetCurrentPageSize(); ++i )
	{

		CSkinResource *currentResource = m_skinResourceVector[i];
		LPDIRECT3DTEXTURE8 previewTexture = currentResource->GetPreviewTexture();


		FLOAT screenshotLeft = DETAIL_SCREENSHOT_LEFT;
		FLOAT screenshotRight = DETAIL_SCREENSHOT_RIGHT;

		FLOAT screenshotTop = DETAIL_SCREENSHOT_TOP + 5 + ((i - m_pageOffset) * entryHeight);
		FLOAT screenshotBottom = screenshotTop + (DETAIL_SCREENSHOT_BOTTOM - DETAIL_SCREENSHOT_TOP);


			// Display the screenshot
		m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     FALSE );
		m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    FALSE );
		if( previewTexture )
		{
			m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
			m_displayDevice->SetTexture( 0, previewTexture );

			m_displayDevice->Begin( D3DPT_QUADLIST );      
				const RECT &rct = currentResource->GetPreviewTextureRect();
				m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, rct.left, rct.top );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotTop, 1.0f, 1.0f );
	      
				m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, rct.right, rct.top );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotTop, 1.0f, 1.0f );
	      
				m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, rct.right, rct.bottom );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotBottom, 1.0f, 1.0f );

				m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, rct.left, rct.bottom );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotBottom, 1.0f, 1.0f );
			m_displayDevice->End();
		}
		else
		{
				// Render a black rect where the screenshot would go and put "No Screenshot" text in it
			m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
			m_displayDevice->SetTexture( 0, NULL );

			m_displayDevice->Begin( D3DPT_QUADLIST );      
				m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotTop, 1.0f, 1.0f );
	      
				m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotTop, 1.0f, 1.0f );
	      
				m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotRight, screenshotBottom, 1.0f, 1.0f );

				m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, NOSCREENSHOT_COLOR );
				m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, screenshotLeft, screenshotBottom, 1.0f, 1.0f );
			m_displayDevice->End();

				// Draw the screenshot index
			m_fontSet.SmallThinFont().Begin();
				swprintf( name, L"[%lu]", 0 );
				m_fontSet.SmallThinFont().DrawText( screenshotLeft + ((screenshotRight - screenshotLeft) / 2.0f), 
																						screenshotTop + ((screenshotBottom - screenshotTop) / 2.0f), 
																						NOSCREENSHOTTEXT_COLOR, 
																						L"No Screenshots",
																						XBFONT_CENTER_X );
			m_fontSet.SmallThinFont().End();
		}


		m_fontSet.SmallThinFont().Begin();
			mbstowcs( name, currentResource->GetSkinName().c_str(), 255 );
			m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, 
																					screenshotTop + entryHeight / 2.0f, 
																					HEADER_COLOR, 
																					name, 
																					XBFONT_TRUNCATED | XBFONT_CENTER_Y,
																					TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );

		m_fontSet.SmallThinFont().End();
	}





    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHATESTENABLE,     TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,    TRUE );
  m_displayDevice->SetRenderState( D3DRS_ALPHAREF,            0x08 );
  m_displayDevice->SetRenderState( D3DRS_ALPHAFUNC,           D3DCMP_GREATEREQUAL );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    // Draw scroll up icon
  if( (UINT32)m_pageOffset )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconRight(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_RIGHT, SCROLLUP_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollUpIconLeft(), m_textureSet.GetScrollUpIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLUP_LEFT, SCROLLUP_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

    // Draw scroll down icon
  if( (UINT32)m_pageOffset + 3 < m_numLinesInList - 1 )
  {
	  m_displayDevice->SetTexture( 0, m_textureSet.GetScrollIconMasks() );
    m_displayDevice->Begin( D3DPT_QUADLIST );
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconTop() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_TOP, 1.0f, 1.0f );
      
      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconRight(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_RIGHT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );

      m_displayDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, SCROLLICON_COLOR );
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_textureSet.GetScrollDownIconLeft(), m_textureSet.GetScrollDownIconBottom() );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, SCROLLDOWN_LEFT, SCROLLDOWN_BOTTOM, 1.0f, 1.0f );
    m_displayDevice->End();
  }

  m_displayDevice->SetTexture( 0, NULL );
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}




