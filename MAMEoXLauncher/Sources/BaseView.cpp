/**
	* \file			BaseView.cpp
	* \brief		Base class for views
	*
	*/

//= I N C L U D E S ====================================================
#include "BaseView.h"
#include "DebugLogger.h"
#include "XBFont.h"
#include "xbox_FileIO.h"


#include <string>

//= D E F I N E S ======================================================


//= G L O B A L = V A R S ==============================================

//= S T R U C T U R E S ===============================================


//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	DrawToTexture
//---------------------------------------------------------------------
void CBaseView::DrawToTexture( LPDIRECT3DTEXTURE8 targetTexture )
{
  if( !targetTexture || !RenderToTextureStart( targetTexture ) )
    return;

  Draw( TRUE, FALSE );

  RenderToTextureStop();
}


//---------------------------------------------------------------------
//  LoadPNGToTexture
//---------------------------------------------------------------------
BOOL CBaseView::LoadPNGToTexture( const CStdString &filename, LPDIRECT3DTEXTURE8 *ret, RECT *retRect )
{
  if( !ret )
    return FALSE;

  if( filename == "" || !retRect )
    return FALSE;



	PNGFile_t pngFile;
	if( !LoadPNGFile( filename, &pngFile ) )
		return FALSE;

    // Release the old screenshot data
  SAFE_RELEASE( *ret );

	memset( retRect, 0, sizeof(*retRect) );
	retRect->right = pngFile.m_imageWidth;
	retRect->bottom = pngFile.m_imageHeight;

	*ret = CreateTextureFromPNGFile( pngFile, NULL );
	ClosePNGFile( pngFile );
	return (*ret != NULL);
}

//---------------------------------------------------------------------
//	RenderBackdrop
//---------------------------------------------------------------------
void CBaseView::RenderBackdrop( void )
{
	if( !CheckResourceValidity( m_backdropTextureID ) )
		return;

  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );
	g_loadedSkin->SelectSkinResourceTexture( m_displayDevice, m_backdropTextureID );
	
	g_loadedSkin->GetSkinResourceInfo( m_backdropTextureID )->Render( m_displayDevice, 0.0f, 0.0f, 640.0f, 480.0f );

	m_displayDevice->SetTexture( 0, NULL );
}



//---------------------------------------------------------------------
//	RenderToTextureStart
//---------------------------------------------------------------------
BOOL CBaseView::RenderToTextureStart( LPDIRECT3DTEXTURE8 targetTexture )
{
  D3DVIEWPORT8 vp = { 0, 0, 640, 480, 0.0f, 1.0f };
  return ::RenderToTextureStart( m_renderToTextureToken, m_displayDevice, targetTexture, vp );
}

//---------------------------------------------------------------------
//	RenderToTextureStop
//---------------------------------------------------------------------
void CBaseView::RenderToTextureStop( void )
{
  ::RenderToTextureStop( m_renderToTextureToken );
}












