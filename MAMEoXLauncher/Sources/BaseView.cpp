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

//= P R O T O T Y P E S ================================================
static void Helper_PNGRead( png_structp pngStruct, png_bytep buf, png_size_t size );

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

    // Create a new texture and read the data into it
  if( (D3DXCreateTexture( m_displayDevice,
                          retRect->right,
                          retRect->bottom,
													1,									// Mip levels
                          0,                  // Usage
													D3DFMT_LIN_X8R8G8B8,// Format
													0,		              // Pool (unused)
                          ret )) != S_OK )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    PRINTMSG(( T_ERROR, "Failed to create texture for screenshot" ));
    PRINTMSG(( T_INFO, "Memory status" ));
    PRINTMSG(( T_INFO, "Physical:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailPhys ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalPhys ));
    PRINTMSG(( T_INFO, "Page File:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailPageFile ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalPageFile ));
    PRINTMSG(( T_INFO, "Virtual:" ));
    PRINTMSG(( T_INFO, "         Avail: %lu", memStatus.dwAvailVirtual ));
    PRINTMSG(( T_INFO, "         Total: %lu", memStatus.dwTotalVirtual ));

		ClosePNGFile( pngFile );
		return FALSE;
	}


		// Grab the surface description
	D3DSURFACE_DESC desc;
  (*ret)->GetLevelDesc( 0, &desc );

    // Lock the region so we can render to it later
  D3DLOCKED_RECT lockedRect;
	(*ret)->LockRect(	0, &lockedRect, NULL, 0 );
  memset( lockedRect.pBits, 0, desc.Width * desc.Height * sizeof(DWORD) );

  UINT32 y;
	switch( pngFile.m_infoPtr->color_type )
  {
    // *** PNG_COLOR_TYPE_PALETTE *** //
  case PNG_COLOR_TYPE_PALETTE:
    {
      switch( pngFile.m_infoPtr->bit_depth )
      {
        // Monochrome
      case 1:
        {
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < retRect->bottom; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)pngFile.m_data[y];
            for( UINT32 x = 0; x < (retRect->right >> 3); ++x )
            {      
              for( INT32 i = 7; i >= 0; --i )
              {
                BOOL palIndex = (*src & (1 << i)) ? 1 : 0;
                png_color &color = pngFile.m_infoPtr->palette[palIndex];
                *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
              }

              ++src;
            }
            free( pngFile.m_data[y] );
            pngFile.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 2:
        {
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < retRect->bottom; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)pngFile.m_data[y];
            for( UINT32 x = 0; x < (retRect->right >> 2); ++x )
            {      
              BYTE palIndex = (*src >> 6) & 0x03;
              png_color color = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 4) & 0x03;
              color = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 2) & 0x03;
              color = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x03;
              color = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              ++src;
            }
            free( pngFile.m_data[y] );
            pngFile.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 4:
        {
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < retRect->bottom; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)pngFile.m_data[y];
            for( UINT32 x = 0; x < (retRect->right >> 1); ++x )
            {
              BYTE palIndex = (*src >> 4) & 0x07;
              png_color &color = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x07;
              png_color &color2 = pngFile.m_infoPtr->palette[palIndex];
              *(dest++) = D3DCOLOR_XRGB( color2.red, color2.green, color2.blue );

              ++src;
            }
            free( pngFile.m_data[y] );
            pngFile.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      default:
        {
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < retRect->bottom; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)pngFile.m_data[y];
            for( UINT32 x = 0; x < retRect->right; ++x )
            {      
              png_color &color = pngFile.m_infoPtr->palette[*(src++)];
              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
            }
            free( pngFile.m_data[y] );
            pngFile.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;
      }
    }
    break;

    // *** PNG_COLOR_TYPE_RGB *** //
  case PNG_COLOR_TYPE_RGB:
    {
      D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
      for( y = 0; y < retRect->bottom; ++y )
      {
        D3DCOLOR *dest = destRow;
        BYTE *src = (BYTE*)pngFile.m_data[y];
        for( UINT32 x = 0; x < retRect->right; ++x )
        {      
          *(dest++) = D3DCOLOR_XRGB( *(src), *(src+1), *(src+2) );
          src += 3;
        }
        free( pngFile.m_data[y] );
        pngFile.m_data[y] = NULL;
        destRow += desc.Width;
      }
    }
    break;
  }

	ClosePNGFile( pngFile );

	return TRUE;
}

//---------------------------------------------------------------------
//	RenderBackdrop
//---------------------------------------------------------------------
void CBaseView::RenderBackdrop( void )
{
  if( !m_backdropTexture )
    return;

  m_displayDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_displayDevice->SetTexture( 0, m_backdropTexture );
  m_displayDevice->SetVertexShader( D3DFVF_XYZRHW | D3DFVF_TEX0 );

	m_displayDevice->Begin( D3DPT_QUADLIST );
    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 0.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 0.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 0.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 1.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 640.0f, 480.0f, 1.0f, 1.0f );

    m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, 0.0f, 1.0f );
    m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, 0.0f, 480.0f, 1.0f, 1.0f );
  m_displayDevice->End();

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










//---------------------------------------------------------------------
//  LoadPNGFile
//---------------------------------------------------------------------
BOOL LoadPNGFile(	const CStdString &filename, PNGFile_t *ret )
{
	if( filename == "" || !ret )
		return FALSE;


  osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, filename.c_str(), "r" );
  if( !file )
    return FALSE;

    // Read the png signature
  {
    #define PNG_BYTES_TO_CHECK 4 
    char buf[PNG_BYTES_TO_CHECK];

   if( osd_fread( file, buf, PNG_BYTES_TO_CHECK ) != PNG_BYTES_TO_CHECK )
   {
     osd_fclose( file );
     return FALSE;
   }

   if( png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK ) )
   {
     osd_fclose( file );
     return FALSE;
   }
  }


  /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
  ret->m_pngPtr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if( !ret->m_pngPtr )
  {
    osd_fclose( file );
    return FALSE;
  }

  /* Allocate/initialize the memory for image information.  REQUIRED. */
  ret->m_infoPtr = png_create_info_struct( ret->m_pngPtr );
  if( !ret->m_infoPtr )
  {
    osd_fclose( file );
    png_destroy_read_struct( &ret->m_pngPtr, png_infopp_NULL, png_infopp_NULL );
    return FALSE;
  }

  /* Set error handling if you are using the setjmp/longjmp method (this is
  * the normal method of doing things with libpng).  REQUIRED unless you
  * set up your own error handlers in the png_create_read_struct() earlier.
  */

  if( setjmp( png_jmpbuf(ret->m_pngPtr) ) )
  {
    /* Free all of the memory associated with the ret->m_pngPtr and ret->m_infoPtr */
    png_destroy_read_struct( &ret->m_pngPtr, &ret->m_infoPtr, png_infopp_NULL );
    osd_fclose( file );

      /* If we get here, we had a problem reading the file */
    return FALSE;
  }

  /* If you are using replacement read functions, instead of calling
  * png_init_io() here you would call:
  */
  png_set_read_fn( ret->m_pngPtr, (void *)file, Helper_PNGRead );
  /* where user_io_ptr is a structure you want available to the callbacks */


  /* If we have already read some of the signature */
  png_set_sig_bytes( ret->m_pngPtr, PNG_BYTES_TO_CHECK );

  int bit_depth, color_type, interlace_type;
  /* The call to png_read_info() gives us all of the information from the
  * PNG file before the first IDAT (image data chunk).  REQUIRED
  */
  png_read_info(ret->m_pngPtr, ret->m_infoPtr);

  png_get_IHDR( ret->m_pngPtr, 
                ret->m_infoPtr, 
                (png_uint_32*)&ret->m_imageWidth, 
                (png_uint_32*)&ret->m_imageHeight, 
                &bit_depth, 
                &color_type,
                &interlace_type, 
                int_p_NULL, 
                int_p_NULL );


  /* Some suggestions as to how to get a screen gamma value */
  DOUBLE screen_gamma = 1.0;  /* A good guess for Mac systems */

  /* Tell libpng to handle the gamma conversion for you.  The final call
  * is a good guess for PC generated images, but it should be configurable
  * by the user at run time by the user.  It is strongly suggested that
  * your application support gamma correction.
  */

  int intent;

  if (png_get_sRGB(ret->m_pngPtr, ret->m_infoPtr, &intent))
    png_set_gamma(ret->m_pngPtr, screen_gamma, 0.45455);
  else
  {
    double image_gamma;
    if (png_get_gAMA(ret->m_pngPtr, ret->m_infoPtr, &image_gamma))
        png_set_gamma(ret->m_pngPtr, screen_gamma, image_gamma);
    else
        png_set_gamma(ret->m_pngPtr, screen_gamma, 0.45455);
  }


  /* Optional call to gamma correct and add the background to the palette
  * and update info structure.  REQUIRED if you are expecting libpng to
  * update the palette for you (ie you selected such a transform above).
  */
  png_read_update_info(ret->m_pngPtr, ret->m_infoPtr);

  ret->m_data = (png_bytep*)calloc( 1, ret->m_imageHeight * sizeof(png_bytep) );
  if( !ret->m_data )
  {
    PRINTMSG(( T_ERROR, "Out of memory allocating pngFile.m_data array!" ));
    png_destroy_read_struct( &ret->m_pngPtr, &ret->m_infoPtr, png_infopp_NULL );
    osd_fclose( file );
    return FALSE;
  }

		// Allocate all the data rows
  UINT32 y;
  for( y = 0; y < ret->m_imageHeight; ++y )
  {
    if( !(ret->m_data[y] = (png_bytep)malloc( png_get_rowbytes( ret->m_pngPtr, ret->m_infoPtr ))) )
    {
      PRINTMSG(( T_ERROR, "Out of memory allocating ret->m_data[%lu]!", y ));
      png_destroy_read_struct( &ret->m_pngPtr, &ret->m_infoPtr, png_infopp_NULL );
      osd_fclose( file );
      return FALSE;
    }
  }


  png_read_image(ret->m_pngPtr, ret->m_data );

    /* read rest of file, and get additional chunks in ret->m_infoPtr - REQUIRED */
  png_read_end( ret->m_pngPtr, ret->m_infoPtr );
  osd_fclose( file );

  return TRUE;
}

//------------------------------------------------------------
//	ClosePNGFile
//------------------------------------------------------------
void ClosePNGFile( PNGFile_t &file ) 
{
	if( file.m_data )
	{
		for( int i = 0; i < file.m_imageHeight; ++i )
		{
			if( file.m_data[i] )
				free( file.m_data[i] );
		}

		free( file.m_data );
	}

    /* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct( &file.m_pngPtr, &file.m_infoPtr, png_infopp_NULL );

	memset( &file, 0, sizeof(file) );
}


//---------------------------------------------------------------------
//  Helper_PNGRead
//---------------------------------------------------------------------
static void Helper_PNGRead( png_structp pngStruct, png_bytep buf, png_size_t size )
{
  osd_file *file = (osd_file*)png_get_io_ptr( pngStruct );
  osd_fread( file, buf, size );
}




