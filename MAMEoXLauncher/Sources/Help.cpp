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
#define HELPFILENAME		        "D:\\Media\\help.txt"

#define NORMAL_ITEM_COLOR				D3DCOLOR_XRGB( 190, 215, 255 )

	// Maximum number of items to render on the screen at once
#define MAXPAGESIZE							22

#define X_POS		( 25 )
#define Y_POS		( 25 )
#define WIDTH		( 640 - (X_POS<<1) )

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

	m_Help.clear();

	std::string		HelpFile = HELPFILENAME;

	PRINTMSG( T_INFO, "Load help file: %s", HelpFile.c_str() );
	HANDLE hFile = CreateFile(	HelpFile.c_str(),
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
		std::string line;
		int tst = 0;
		while( FileGets( hFile, buffer, 4096 ) ) //&& tst++ < 286 )
		{
			line = buffer;

			m_Help.push_back( line );
		}

		CloseHandle( hFile );

    m_numLinesInList = m_Help.size();
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

		buffer[ i++ ] = c;
	}

	return( FALSE );
}

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CHelp::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
	WCHAR name[256];

		// Display the error to the user
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,	// Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

  RenderBackdrop();

	m_fontSet.FixedWidthFont().Begin();

		    // Render the text
	    FLOAT yPos = Y_POS;
	    DWORD pageSize = (m_Help.size() < MAXPAGESIZE ? m_Help.size() : MAXPAGESIZE);
	    ULONG absListIDX = (ULONG)m_pageOffset;
	    if( absListIDX > (m_Help.size() - pageSize) )
	    {
			    // The current page offset is invalid (due to list shrinkage), reset it and
			    //  set the cursor position to the last item in the list
        absListIDX = (m_Help.size() - pageSize);
        m_pageOffset = (FLOAT)absListIDX;
		    m_cursorPosition = (FLOAT)(pageSize - 1);
	    }

	    for( DWORD i = 0; i < pageSize; ++i )
	    {
		    mbstowcs( name, m_Help[absListIDX++].c_str(), 255 );
        name[wcslen(name)-1] = L'\0';

			    // Render the selected item as bright white
		    m_fontSet.FixedWidthFont().DrawText( X_POS, yPos, NORMAL_ITEM_COLOR, name, XBFONT_TRUNCATED, WIDTH );

			    // Inc the Y position
		    yPos += 20;
	    }

      #if defined(_PROFILER) || defined(_DEBUG)
        MEMORYSTATUS memStatus;
        GlobalMemoryStatus( &memStatus );

        WCHAR memStr[256];
        swprintf( memStr, 
                  L"Memory: %lu/%lu",
                  memStatus.dwAvailPhys, 
                  memStatus.dwTotalPhys );

        m_fontSet.FixedWidthFont().DrawText( 256, yPos, D3DCOLOR_RGBA(100,220,220,255), memStr, XBFONT_CENTER_X );
      #endif

	m_fontSet.FixedWidthFont().End();

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}
