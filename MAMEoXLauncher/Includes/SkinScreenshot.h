/**
	* \file			SkinScreenshot.h
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"
#include "SkinSpriteResource.h"
#include "SkinText.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================

//= C L A S S E S ====================================================


/**
 * \class		CSkinScreenshot
 * \brief   Renders a screenshot icon/text pair
 */
class CSkinScreenshot : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinScreenshot( void ) : CSkinElement( SKINELEMENT_SCREENSHOT ) {
		m_text = NULL;
		m_cornerColors[0] = m_cornerColors[1] = m_cornerColors[2] = m_cornerColors[3] = D3DCOLOR_ARGB( 120, 0, 0, 0 );
	}

		//------------------------------------------------------
		//	Constructor (CSkinScreenshot)
		//------------------------------------------------------
	CSkinScreenshot( const CSkinScreenshot &a ) : CSkinElement( a ) {
		memcpy( m_cornerColors, a.m_cornerColors, sizeof(m_cornerColors) );

		if( a.m_text )
			m_text = new CSkinText( *a.m_text );
		else
			m_text = NULL;
	}

		//------------------------------------------------------
		//	Destructor
		//------------------------------------------------------
	~CSkinScreenshot( void ) {
		if( m_text )
			delete m_text;
	}

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render(	LPDIRECT3DDEVICE8 displayDevice, 
								LPDIRECT3DTEXTURE8 screenshot, 
								const RECT &screenshotTextureRect,
								BOOL swapXY = FALSE, 
								BOOL flipX = FALSE, 
								BOOL flipY = FALSE ) const
	{ 
		Render( displayDevice, NULL, screenshot, screenshotTextureRect, swapXY, flipX, flipY ); 
	}

		//------------------------------------------------------
		//	Render 
		//------------------------------------------------------
	void Render(	LPDIRECT3DDEVICE8 displayDevice, 
								const WCHAR *str, 
								LPDIRECT3DTEXTURE8 screenshot, 
								const RECT &screenshotTextureRect, 
								BOOL swapXY = FALSE, 
								BOOL flipX = FALSE, 
								BOOL flipY = FALSE ) const
	{
		RenderAtRect( displayDevice, 
									str, 
									screenshot, 
									screenshotTextureRect,
									m_left,
									m_top,
									m_right,
									m_bottom,
									swapXY,
									flipX,
									flipY );
	}

		//------------------------------------------------------
		//	RenderAsOffset
		//------------------------------------------------------
	void RenderAsOffset(	LPDIRECT3DDEVICE8 displayDevice, 
												LPDIRECT3DTEXTURE8 screenshot, 
												const RECT &screenshotTextureRect, 
												FLOAT x, 
												FLOAT y, 
												BOOL swapXY = FALSE, 
												BOOL flipX = FALSE, 
												BOOL flipY = FALSE ) const 
	{ 
		RenderAsOffset( displayDevice, NULL, screenshot, screenshotTextureRect, x, y, swapXY, flipX, flipY ); 
	}

		//------------------------------------------------------
		//	RenderAsOffset
		//------------------------------------------------------
	void RenderAsOffset(	LPDIRECT3DDEVICE8 displayDevice, 
												const WCHAR *str, 
												LPDIRECT3DTEXTURE8 screenshot, 
												const RECT &screenshotTextureRect, 
												FLOAT x, 
												FLOAT y, 
												BOOL swapXY = FALSE, 
												BOOL flipX = FALSE, 
												BOOL flipY = FALSE ) const 
	{
		RenderAtRect( displayDevice, 
									str, 
									screenshot, 
									screenshotTextureRect,
									x + m_left,
									y + m_top,
									x + m_right,
									y + m_bottom,
									swapXY,
									flipX,
									flipY );
	}


		//------------------------------------------------------
		//	RenderAtRect
		//------------------------------------------------------
	void RenderAtRect(	LPDIRECT3DDEVICE8 displayDevice, 
											const WCHAR *str, 
											LPDIRECT3DTEXTURE8 screenshot, 
											const RECT &screenshotTextureRect, 
											FLOAT left, 
											FLOAT top, 
											FLOAT right, 
											FLOAT bottom, 
											BOOL swapXY = FALSE, 
											BOOL flipX = FALSE, 
											BOOL flipY = FALSE ) const;

		//------------------------------------------------------
		//	ParseINI
		//------------------------------------------------------
	virtual BOOL ParseINI(	CSystem_IniFile &iniFile, const CStdString &sectionName, const CStdString &entryName );



	D3DCOLOR						m_cornerColors[4];	// The color of each corner, starting from upper-left, in clockwise order
	CSkinText						*m_text;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

