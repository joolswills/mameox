/**
	* \file			SkinText.cpp
	*/

//= I N C L U D E S ====================================================
#include "SkinText.h"
#include "Skin.h"
#include "DebugLogger.h"
#include "FontSet.h"

//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================
extern CFontSet              g_fontSet;          // The global font manager

//= P R O T O T Y P E S ===============================================


//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinText::Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str ) const
{ 
	FLOAT x = m_left;
	FLOAT y = m_top;

	if( m_fontFlags & XBFONT_CENTER_X && m_right >= m_left )
		x += (m_right - m_left) / 2.0f;

	if( m_fontFlags & XBFONT_CENTER_Y && m_bottom >= m_top )
		y += (m_bottom - m_top) / 2.0f;

	Render( displayDevice, str, x, y ); 
}


//---------------------------------------------------------------------
//	Render
//---------------------------------------------------------------------
void CSkinText::Render( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str, FLOAT x, FLOAT y ) const
{
	FLOAT maxWidth = 0;
	UINT32 flags = m_fontFlags;

	if( flags & XBFONT_TRUNCATED && m_right >= m_left )
	{
		maxWidth = m_right - m_left;
		maxWidth -= g_fontSet.GetFont( m_fontType ).GetTextWidth( L" ..." );
	}
	else
		flags &= ~XBFONT_TRUNCATED;

//if( !(m_fontFlags & XBFONT_CENTER_Y)  )
//	y += g_fontSet.GetFont( m_fontType ).GetFontHeight();


	g_fontSet.GetFont( m_fontType ).DrawText( x, y, m_color, str, flags, maxWidth );
}	

//---------------------------------------------------------------------
//	RenderAtRect
//---------------------------------------------------------------------
void CSkinText::RenderAtRect(	LPDIRECT3DDEVICE8 displayDevice, 
															const WCHAR *str, 
															FLOAT left, 
															FLOAT top, 
															FLOAT right, 
															FLOAT bottom, 
															UINT32 flags ) const
{
	if( flags == -1 )
		flags = m_fontFlags;

	FLOAT maxWidth = 0;

	if( flags & XBFONT_CENTER_X && right >= left )
		left += (right - left) / 2.0f;

	if( flags & XBFONT_CENTER_Y && bottom >= top )
		top += (bottom - top) / 2.0f;

	if( flags & XBFONT_TRUNCATED && m_right >= m_left )
	{
		maxWidth = m_right - m_left;
		maxWidth -= g_fontSet.GetFont( m_fontType ).GetTextWidth( L" ..." );
	}
	else
		flags &= ~XBFONT_TRUNCATED;


/*
	if( !(flags & XBFONT_CENTER_Y)  )
		top += g_fontSet.GetFont( m_fontType ).GetFontHeight();
*/

	g_fontSet.GetFont( m_fontType ).DrawText( left, top, m_color, str, flags, maxWidth );
}


//---------------------------------------------------------------------
//	RenderAsOffset
//---------------------------------------------------------------------
void CSkinText::RenderAsOffset( LPDIRECT3DDEVICE8 displayDevice, const WCHAR *str, FLOAT x, FLOAT y ) const
{ 
	FLOAT xOff = m_left;
	FLOAT yOff = m_top;

	if( m_fontFlags & XBFONT_CENTER_X && m_right >= m_left )
		xOff += (m_right - m_left) / 2.0f;

	if( m_fontFlags & XBFONT_CENTER_Y && m_bottom >= m_top )
		yOff += (m_bottom - m_top) / 2.0f;

	Render( displayDevice, str, x + xOff, y + yOff ); 
}


//---------------------------------------------------------------------
//	ParseINI
//---------------------------------------------------------------------
BOOL CSkinText::ParseINI(	CSystem_IniFile &iniFile, 
													const CStdString &sectionName, 
													const CStdString &parentEntryName, 
													const CSkinElement *parent ) 
{
	if( parent )
	{
		m_left		= parent->m_left;
		m_top			= parent->m_top;
		m_right		= parent->m_right;
		m_bottom	= parent->m_bottom;
	}


	CStdString entryName = parentEntryName;
	if( entryName == "" )
		entryName = "Text";
	else
		entryName += ".Text";

	RECT rct = StringToRect( iniFile.GetProfileString( sectionName, entryName + ".Area", "" ) );
	if( rct.left != VALUE_INVALID && rct.top != VALUE_INVALID && rct.right != VALUE_INVALID && rct.bottom != VALUE_INVALID )
	{
		m_left = rct.left;
		m_top = rct.top;
		m_right = rct.right;
		m_bottom = rct.bottom;
	}

	POINT pt = StringToPoint( iniFile.GetProfileString( sectionName, entryName + ".Position", "" ) );
	if( pt.x != VALUE_INVALID && pt.y != VALUE_INVALID )
	{
		m_left = pt.x;
		m_top = pt.y;
		m_right = VALUE_UNBOUNDED;
		m_bottom = VALUE_UNBOUNDED;
	}

	BOOL colorSetup = FALSE;
	CStdString temp = iniFile.GetProfileString( sectionName, entryName + ".Color", "" );
	if( temp != "" )
	{
		m_color = StringToColor( temp );
		colorSetup = TRUE;
	}


		// Parse the font
	temp = iniFile.GetProfileString( sectionName, entryName + ".Font", "standard" );
	temp.MakeLower();
	if( temp == "fixedwidth" )
		m_fontType = FONTTYPE_FIXEDWIDTH;
	else if( temp == "largethin" )
		m_fontType = FONTTYPE_LARGETHIN;
	else if( temp == "smallthin" )
		m_fontType = FONTTYPE_SMALLTHIN;
	else if( temp == "standard" )
		m_fontType = FONTTYPE_DEFAULT;


		// Parse the flags
	m_fontFlags = 0;
	if( m_right != VALUE_UNBOUNDED )
		m_fontFlags |= XBFONT_TRUNCATED;

	temp = iniFile.GetProfileString( sectionName, entryName + ".Options", "" );
	if( temp.size() )
	{
		temp.MakeLower();

		if( strstr( temp.c_str(), "centervertical" ) )
			m_fontFlags |= XBFONT_CENTER_Y;

		if( strstr( temp.c_str(), "centerhorizontal" ) )
			m_fontFlags |= XBFONT_CENTER_X;
	}

	return (m_left != VALUE_INVALID && m_top != VALUE_INVALID ) || colorSetup;
}


//---------------------------------------------------------------------
//	GetTextWidth
//---------------------------------------------------------------------
UINT32 CSkinText::GetTextWidth( const WCHAR *str ) const
{
	if( !str )
		return 0;

	return g_fontSet.GetFont( m_fontType ).GetTextWidth( str );
}

//---------------------------------------------------------------------
//	GetFontHeight
//---------------------------------------------------------------------
UINT32 CSkinText::GetFontHeight( void ) const
{
	return g_fontSet.GetFont( m_fontType ).GetFontHeight();
}

