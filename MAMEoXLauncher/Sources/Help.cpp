/**
	* \file			Help.cpp
	* \brief		Helper class which display the help.txt
	*
	*/

//= I N C L U D E S ====================================================
#include "Help.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}


//= D E F I N E S ======================================================
#define HELPFILENAME		    "D:\\Media\\help.txt"

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							14

  //--- Layout defines -----------------------------------------
#define HEADER_COLOR          D3DCOLOR_XRGB( 0, 0, 0 )
#define ITEM_COLOR			      D3DCOLOR_XRGB( 0, 0, 0 )
#define HIGHLIGHTBAR_COLOR    D3DCOLOR_ARGB( 180, 175, 179, 212 )
#define SCROLLICON_COLOR      D3DCOLOR_XRGB( 255, 255, 255 )

#define TITLEBAR_ROW          116
#define FIRSTDATA_ROW         142

#define NAME_COLUMN           40
#define MANUFACTURER_COLUMN   305
#define YEAR_COLUMN           460
#define CLONE_COLUMN          530 
#define TEXTBOX_RIGHT         604   // The right edge of the text box
#define COLUMN_PADDING        9     // Number of pixels to subtract from the column width before truncating text

#define SCROLLUP_TOP          137
#define SCROLLUP_RIGHT        608
#define SCROLLUP_LEFT         SCROLLUP_RIGHT - 32
#define SCROLLUP_BOTTOM       SCROLLUP_TOP + 32

#define SCROLLDOWN_BOTTOM     448
#define SCROLLDOWN_TOP        SCROLLDOWN_BOTTOM - 32
#define SCROLLDOWN_RIGHT      608
#define SCROLLDOWN_LEFT       SCROLLDOWN_RIGHT - 32

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================
BOOL CreateBackdrop( FLOAT xUsage, FLOAT yUsage );              // Defined in main.cpp
void DestroyBackdrop( void );                                   // Defined in main.cpp
void Die( LPDIRECT3DDEVICE8 m_displayDevice, const char *fmt, ... ); // Defined in main.cpp

//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	LoadHelpFile
//---------------------------------------------------------------------
BOOL CHelp::LoadHelpFile( void )
{
	PRINTMSG( T_TRACE, "LoadHelpFile" );

	m_data.clear();

	PRINTMSG( T_INFO, "Load help file: " HELPFILENAME );
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
		  m_fontSet.FixedWidthFont().DrawText( 320, 
                                        60, 
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
    m_maxPageSize = MAXPAGESIZE;

		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------
//	FileGets
//---------------------------------------------------------------------
BOOL CHelp::FileGets( HANDLE file, char *buffer, UINT32 length )
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
void CHelp::MoveCursor( CInputManager &gp, BOOL unused )
{
    // Keep the cursor situated at the halfway point so that any
    // DPAD movement will move the entire screen
	DWORD pageSize = (m_data.size() < MAXPAGESIZE ? m_data.size() : MAXPAGESIZE);
  m_cursorPosition = (pageSize >> 1);

  CListView::MoveCursor( gp, unused );
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CHelp::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

  FLOAT textHeight = m_fontSet.SmallThinFontHeight();

    // Render the backdrop texture
  RenderBackdrop();

	m_fontSet.SmallThinFont().Begin();

	  m_fontSet.SmallThinFont().DrawText( NAME_COLUMN, TITLEBAR_ROW, HEADER_COLOR, L"Help!" );

	m_fontSet.SmallThinFont().End();

  m_fontSet.FixedWidthFont().Begin();

		  // Render the help text
	  FLOAT yPos = 0.0f;
    FLOAT pageSize = GetCurrentPageSize();
	  ULONG absListIDX = (ULONG)m_pageOffset;

	  for( DWORD i = 0; i < pageSize; ++i )
	  {
      WCHAR wBuf[256];
		  mbstowcs( wBuf, m_data[ absListIDX++ ].c_str(), 255 );
		  m_fontSet.FixedWidthFont().DrawText(  NAME_COLUMN,
                                            FIRSTDATA_ROW + yPos,
                                            ITEM_COLOR,
                                            wBuf,
                                            XBFONT_TRUNCATED,
                                            TEXTBOX_RIGHT - (NAME_COLUMN + COLUMN_PADDING) );
			  // Inc the Y position
		  yPos += textHeight;
	  }

	m_fontSet.FixedWidthFont().End();



    //-- Render the scroll up and/or scroll down icons --------------------------------------------
  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_displayDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA );
  m_displayDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
  m_displayDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX0 );

    // Draw scroll up icon
  if( (DWORD)m_pageOffset )
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

  if( (DWORD)m_pageOffset < (m_numLinesInList - (DWORD)pageSize) )
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



  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}
