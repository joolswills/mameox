/**
	* \file			HelpScreen.cpp
	* \brief		Helper class which display the help.txt
	*
	*/

//= I N C L U D E S ====================================================
#include "HelpScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================
#define HELPFILENAME		    "D:\\Media\\help.txt"

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );              // Defined in main.cpp
void DestroyBackdrop( void );                                   // Defined in main.cpp
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	LoadHelpFile
//---------------------------------------------------------------------
BOOL CHelpScreen::LoadHelpFile( void )
{
	PRINTMSG(( T_TRACE, "LoadHelpFile" ));

	m_data.clear();

	PRINTMSG(( T_INFO, "Load help file: " HELPFILENAME ));
	HANDLE hFile = CreateFile(	HELPFILENAME,
															GENERIC_READ,
															0,
															NULL,
															OPEN_EXISTING,
															FILE_ATTRIBUTE_NORMAL,
															NULL );

	if( hFile != INVALID_HANDLE_VALUE )
	{
		m_displayDevice->Clear(	0L,																// Count
														NULL,															// Rects to clear
														D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
														D3DCOLOR_XRGB(0,0,0),							// Color
														1.0f,															// Z
														0L );				                      // Stencil

		m_fontSet.FixedWidthFont().Begin();
		  m_fontSet.FixedWidthFont().DrawText(	320, 
																						230, 
																						D3DCOLOR_RGBA( 255, 255, 255, 255 ), 
																						L"Loading Help file", 
																						XBFONT_CENTER_X );
		  m_fontSet.FixedWidthFont().End();
		m_displayDevice->Present( NULL, NULL, NULL, NULL );	

			// Read in the Help file
		char buffer[ 4096 ];
		CStdString line;
		int tst = 0;
		while( FileGets( hFile, buffer, 4096 ) ) //&& tst++ < 286 )
		{
			line = buffer;

			m_data.push_back( line );
		}

		CloseHandle( hFile );

    m_numLinesInList = m_data.size();
    m_maxPageSize = 15;

		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
//	FileGets
//---------------------------------------------------------------------
BOOL CHelpScreen::FileGets( HANDLE file, char *buffer, UINT32 length )
{
	char c;
	int i = 0;
	DWORD BytesRead = 0;

	while( ReadFile( file, &c, sizeof( c ), &BytesRead , NULL ) && BytesRead > 0 )
	{
		if( i == length - 1 )
		{
			buffer[ i++ ] = '\0';
			return( TRUE );
		}
		if( c == '\n' )
		{
//			buffer[ i++ ] = c;    // The \n will be ignored anyway [EBA]
			buffer[ i++ ] = '\0';
			return( TRUE );
		}
    else if( c != '\r' )
		  buffer[ i++ ] = c;
	}

	return( FALSE );
}


//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CHelpScreen::MoveCursor( CInputManager &gp, BOOL unused )
{
    // Keep the cursor situated at the halfway point so that any
    // DPAD movement will move the entire screen
	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA );
	if( area )
		m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;
	else
		m_maxPageSize = 15;

	DWORD pageSize = (m_data.size() < m_maxPageSize ? m_data.size() : m_maxPageSize);
  m_cursorPosition = (pageSize >> 1);

  CListView::MoveCursor( gp, unused );
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CHelpScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

  FLOAT textHeight = m_fontSet.SmallThinFontHeight();

	if( !g_loadedSkin )
		return;

    // Render the backdrop texture
  RenderBackdrop();

	if( CheckResourceValidity( SKINELEMENT_HELPSCREEN_FOOTER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_FOOTER )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_HELPSCREEN_BODY ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_BODY )->Render( m_displayDevice );

	if( CheckResourceValidity( SKINELEMENT_HELPSCREEN_HEADER ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_HEADER )->Render( m_displayDevice );




		//-- Draw the help text --------------------------------------------

		// ** Start ** //
	if( CheckResourceValidity(SKINELEMENT_HELPSCREEN_BUTTONINFO_START) )
		g_loadedSkin->GetSkinElementButtonInfo(SKINELEMENT_HELPSCREEN_BUTTONINFO_START)->Render( m_displayDevice, L"Menu" );


		//-- Draw the title bar --------------------------------------------
	FLOAT titleBarLeft = 0.0f;
	FLOAT titleBarTop = 0.0f;
	if( CheckResourceValidity( SKINELEMENT_HELPSCREEN_HEADER ) )
	{
		const CSkinElement *titleBarArea = g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_HEADER );
		titleBarLeft = titleBarArea->m_left;
		titleBarTop =  titleBarArea->m_top;
	}

	if( CheckResourceValidity( SKINELEMENT_HELPSCREEN_HEADER_TEXT ) )
		g_loadedSkin->GetSkinElementText( SKINELEMENT_HELPSCREEN_HEADER_TEXT )->RenderAsOffset( m_displayDevice, 
																																														L"Help!",
																																														titleBarLeft,
																																														titleBarTop );


	if( !CheckResourceValidity( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA ) ||
			!CheckResourceValidity( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA_SINGLEROW_TEXT ) )
	{
		PRINTMSG(( T_INFO, "SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA or SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA_SINGLEROW_TEXT invalid!" ));
		return;
	}


		// Render the actual help text
	const CSkinScrollArea *area = g_loadedSkin->GetSkinElementScrollArea( SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA );
	m_maxPageSize = area->GetHeight() / area->m_singleRowHeight;

	const CSkinText *text = g_loadedSkin->GetSkinElementText(SKINELEMENT_HELPSCREEN_BODY_SCROLLAREA_SINGLEROW_TEXT);

  FLOAT pageSize = GetCurrentPageSize();
	UINT32 absListIDX = (UINT32)m_pageOffset;
	FLOAT yOffset = area->m_top;
	for( UINT32 i = 0; i < pageSize; ++i, yOffset += area->m_singleRowHeight )
	{
    WCHAR wBuf[256];
		mbstowcs( wBuf, m_data[ absListIDX++ ].c_str(), 255 );
		text->RenderAsOffset( m_displayDevice, wBuf, area->m_left, yOffset );			
	}

    //-- Render the scroll up and/or scroll down icons --------------------------------------------
	if( (UINT32)m_pageOffset && CheckResourceValidity( SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_UP ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_UP )->Render( m_displayDevice );

	if( (UINT32)m_pageOffset < (m_numLinesInList - (UINT32)pageSize) && CheckResourceValidity( SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_DOWN ) )
		g_loadedSkin->GetSkinElement( SKINELEMENT_HELPSCREEN_BODY_SCROLLICON_DOWN )->Render( m_displayDevice );


  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}
