/**
	* \file			SkinChooserScreen.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "SkinChooserScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "ListView.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"
#include "System_IniFile.h"

#include <string>
#include <vector>
#include <algorithm>


//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================



//= P R O T O T Y P E S ================================================
extern "C" void DrawProgressbarMessage( LPDIRECT3DDEVICE8 pD3DDevice, const char *message, const char *itemName, DWORD currentItem, DWORD totalItems ); // Defined in main.cpp

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	SetCursorToSelectedSkin
//---------------------------------------------------------------------
void CSkinChooserScreen::SetCursorToSelectedSkin( void )
{
	for( int i = 0; i < m_skinChooser.m_skinResourceVector.size(); ++i )
	{
		if( m_skinChooser.m_options.m_currentSkin == m_skinChooser.m_skinResourceVector[i]->GetSkinName() )
		{
			SetAbsoluteCursorPosition( i );
			return;
		}
	}
}

//---------------------------------------------------------------------
//  SetAbsoluteCursorPosition
//---------------------------------------------------------------------
void CSkinChooserScreen::SetAbsoluteCursorPosition( UINT32 pos )
{
  if( pos >= m_numLinesInList )
    return;


	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA );
	if( area )
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;
	else
		m_maxPageSize = 3;


    // Jump the cursor to the selected position
	UINT32 pageSize = (m_numLinesInList < m_maxPageSize ? m_numLinesInList : m_maxPageSize);
	UINT32 pageHalfwayPoint = (pageSize >> 1);
	UINT32 maxPageOffset = m_numLinesInList - pageSize;

    // Put the page offset at absoluteCursorPos - pageHalwayPoint, or 0
  if( pos <= pageHalfwayPoint || m_numLinesInList < m_maxPageSize )
  {
    m_pageOffset = 0.0f;
    m_cursorPosition = (FLOAT)pos;
  }
  else if( pos - pageHalfwayPoint > maxPageOffset )
  {
    m_pageOffset = maxPageOffset;
    m_cursorPosition = pos - maxPageOffset;
  }
  else
  {
    m_pageOffset = (FLOAT)(pos - pageHalfwayPoint);
    m_cursorPosition = (FLOAT)pageHalfwayPoint;
  }
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
		CSkin *selectedSkin = m_skinChooser.m_skinResourceVector[GetAbsoluteCursorPosition()];
		if( selectedSkin->GetSkinName() != m_skinChooser.m_options.m_currentSkin )
		{
			CSkin *oldSkin = g_loadedSkin;
			if( g_loadedSkin )
				g_loadedSkin->UnloadSkin();

			g_loadedSkin = selectedSkin;
			g_loadedSkin->LoadSkin( NULL );	// Todo:Check the error code, reload the old skin, etc...
			m_skinChooser.m_options.m_currentSkin = selectedSkin->GetSkinName();
		}
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

	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA );
	if( area )
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;
	else
		m_maxPageSize = 3;

	DWORD pageSize = (m_skinChooser.m_skinResourceVector.size() < m_maxPageSize ? m_skinChooser.m_skinResourceVector.size() : m_maxPageSize );
	UINT32 pageHalfwayPoint = (pageSize >> 1);
	UINT32 maxPageOffset = m_skinChooser.m_skinResourceVector.size() - pageSize;

	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (UINT32)m_cursorPosition < pageHalfwayPoint )
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
			if( (UINT32)(cursorVelocity + m_pageOffset) <= maxPageOffset )
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
		if( (UINT32)m_pageOffset > maxPageOffset )
			m_pageOffset = (FLOAT)maxPageOffset;
		if( (UINT32)m_cursorPosition > (pageSize - 1) )
			m_cursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (UINT32)m_cursorPosition > pageHalfwayPoint )
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

	if( !g_loadedSkin )
		return;

    // Render the backdrop texture
	RenderBackdrop();

	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_FOOTER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_FOOTER )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_BODY )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_HEADER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_HEADER )->Render( m_displayDevice );


		//-- Draw the help text --------------------------------------------

		// ** Start ** //
	if( CheckResourceValidity(SKINELEMENT_SKINCHOOSERSCREEN_BUTTONINFO_START) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_SKINCHOOSERSCREEN_BUTTONINFO_START)->Render( m_displayDevice, L"Menu" );


	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_HEADER_TEXT ) )
		g_loadedSkin->GetSkinElementText( SKINELEMENT_SKINCHOOSERSCREEN_HEADER_TEXT )->RenderAsOffset( m_displayDevice, 
																																														L"Skin Chooser",
																																														titleBarLeft,
																																														titleBarTop );

		// Render the body
	if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA ) )
	{
		const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA );
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;

			// Render the highlight bar for the selected item
		FLOAT selectedItemYPos = area->m_top + (area->m_singleRowHeight * (UINT32)m_cursorPosition);
		if( area->m_highlightBar )
			area->m_highlightBar->RenderAsOffset( m_displayDevice, area->m_left, selectedItemYPos );



			//-- Draw screenshot + name pairs -------------------------------------------------
		FLOAT currentItemYOffset = area->m_top;
		for( int i = (UINT32)m_pageOffset; i < (UINT32)m_pageOffset + GetCurrentPageSize(); ++i, currentItemYOffset += area->m_singleRowHeight )
		{
			CSkin *currentResource = m_skinChooser.m_skinResourceVector[i];

			if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_SCREENSHOT ) )
			{
				g_loadedSkin->GetSkinElementScreenshot(SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_SCREENSHOT)->RenderAsOffset( m_displayDevice,
																																																																		currentResource->GetPreviewTexture(),
																																																																		currentResource->GetPreviewTextureRect(),
																																																																		area->m_left, 
																																																																		currentItemYOffset );
			}

			if( !currentResource->GetSkinDescription().IsEmpty() )
			{
				WCHAR text[256];

					// Render the name
				if( currentResource->GetSkinAuthor().size() )
				{
					WCHAR skinName[64] = L"";
					WCHAR skinAuthor[64] = L"";
					mbstowcs( skinName, currentResource->GetSkinName().c_str(), 64 );
					mbstowcs( skinAuthor, currentResource->GetSkinAuthor().c_str(), 64 );
					swprintf( text, L"%s by %s", skinName, skinAuthor );
				}
				else
					mbstowcs( text, currentResource->GetSkinName().c_str(), 255 );

				if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAME_TEXT ) )
					g_loadedSkin->GetSkinElementText(SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAME_TEXT)->RenderAsOffset( m_displayDevice, text, area->m_left, currentItemYOffset );


					// Render the description
				if( currentResource->GetSkinDescription().size() )
				{
					mbstowcs( text, currentResource->GetSkinDescription().c_str(), 255 );			
					if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_DESCRIPTION_TEXT ) )
						g_loadedSkin->GetSkinElementText(SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_DESCRIPTION_TEXT)->RenderAsOffset( m_displayDevice, text, area->m_left, currentItemYOffset );			
				}
			}
			else
			{
				WCHAR text[256];
				if( currentResource->GetSkinAuthor().size() )
				{
					WCHAR skinName[64] = L"";
					WCHAR skinAuthor[64] = L"";
					mbstowcs( skinName, currentResource->GetSkinName().c_str(), 64 );
					mbstowcs( skinAuthor, currentResource->GetSkinAuthor().c_str(), 64 );
					swprintf( text, L"%s by %s", skinName, skinAuthor );
				}
				else
					mbstowcs( text, currentResource->GetSkinName().c_str(), 255 );

				if( CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAMEWITHOUTDESCRIPTION_TEXT ) )
					g_loadedSkin->GetSkinElementText(SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLAREA_SINGLEROW_NAMEWITHOUTDESCRIPTION_TEXT)->RenderAsOffset( m_displayDevice, text, area->m_left, currentItemYOffset );
			}
		}


			//-- Render the scroll up and/or scroll down icons --------------------------------------------
		if( (UINT32)m_pageOffset && CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_UP ) )
			g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_UP )->Render( m_displayDevice );

		UINT32 pageSize = (m_numLinesInList < m_maxPageSize ? m_numLinesInList : m_maxPageSize);
		if( (UINT32)m_pageOffset < (m_numLinesInList - pageSize) && CheckResourceValidity( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_DOWN ) )
			g_loadedSkin->GetSkinElement( SKINELEMENT_SKINCHOOSERSCREEN_BODY_SCROLLICON_DOWN )->Render( m_displayDevice );
	}

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}





