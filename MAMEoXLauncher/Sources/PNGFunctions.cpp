/**
	* \file			PNGFunctions.cpp
	* \brief		Helper functions for PNG file manipulation
	*
	*/

//= I N C L U D E S ====================================================
#include "PNGFunctions.h"
#include "xbox_FileIO.h"

#include "GraphicsManager.h"

//= G L O B A L = V A R S =============================================
extern CGraphicsManager	  g_graphicsManager;


//= P R O T O T Y P E S ================================================
static void Helper_PNGRead( png_structp pngStruct, png_bytep buf, png_size_t size );

//= F U N C T I O N S ==================================================

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



//---------------------------------------------------------------------
//	CreateTextureFromPNGFile
//---------------------------------------------------------------------
LPDIRECT3DTEXTURE8 CreateTextureFromPNGFile( PNGFile_t &colorFileData, BYTE *alphaChannel )
{
	LPDIRECT3DTEXTURE8 ret;

	PRINTMSG(( T_INFO, "Create Texture from file w/ size %lux%lu alpha 0x%X", colorFileData.m_imageWidth, colorFileData.m_imageHeight, alphaChannel ));

    // Create a new texture and read the data into it
  if( (D3DXCreateTexture( g_graphicsManager.GetD3DDevice(),
                          colorFileData.m_imageWidth,
                          colorFileData.m_imageHeight,
													1,									// Mip levels
                          0,                  // Usage
													alphaChannel ? D3DFMT_LIN_A8R8G8B8 : D3DFMT_LIN_X8R8G8B8,	// Format
													0,		              // Pool (unused)
                          &ret )) != S_OK )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );

    PRINTMSG(( T_ERROR, "Failed to create texture for skin" ));
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
		return FALSE;
	}


		// Grab the surface description
	D3DSURFACE_DESC desc;
  ret->GetLevelDesc( 0, &desc );

    // Lock the region so we can render to it later
  D3DLOCKED_RECT lockedRect;
	ret->LockRect(	0, &lockedRect, NULL, 0 );
  memset( lockedRect.pBits, 0, desc.Width * desc.Height * sizeof(DWORD) );

  UINT32 y;
	switch( colorFileData.m_infoPtr->color_type )
  {
    // *** PNG_COLOR_TYPE_PALETTE *** //
  case PNG_COLOR_TYPE_PALETTE:
    {
      switch( colorFileData.m_infoPtr->bit_depth )
      {
        // Monochrome
      case 1:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 3); ++x )
            {      
              for( INT32 i = 7; i >= 0; --i )
              {
                BOOL palIndex = (*src & (1 << i)) ? 1 : 0;
                png_color &color = colorFileData.m_infoPtr->palette[palIndex];

								if( alphaSrc )
	                *(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
								else
	                *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
              }

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 2:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 2); ++x )
            {      
              BYTE palIndex = (*src >> 6) & 0x03;
              png_color color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
	              *(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 4) & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = (*src >> 2) & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x03;
              color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      case 4:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < (colorFileData.m_imageWidth >> 1); ++x )
            {
              BYTE palIndex = (*src >> 4) & 0x07;
              png_color &color = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );

              palIndex = *src & 0x07;
              png_color &color2 = colorFileData.m_infoPtr->palette[palIndex];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color2.red, color2.green, color2.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color2.red, color2.green, color2.blue );

              ++src;
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
            destRow += desc.Width;
          }
        }
        break;

      default:
        {
					BYTE *alphaSrc = alphaChannel;
          D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
          for( y = 0; y < colorFileData.m_imageHeight; ++y )
          {
            D3DCOLOR *dest = destRow;
            BYTE *src = (BYTE*)colorFileData.m_data[y];
            for( UINT32 x = 0; x < colorFileData.m_imageWidth; ++x )
            {      
              png_color &color = colorFileData.m_infoPtr->palette[*(src++)];
							if( alphaSrc )
								*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), color.red, color.green, color.blue );
							else
	              *(dest++) = D3DCOLOR_XRGB( color.red, color.green, color.blue );
            }
            free( colorFileData.m_data[y] );
            colorFileData.m_data[y] = NULL;
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
			BYTE *alphaSrc = alphaChannel;
      D3DCOLOR *destRow = (D3DCOLOR*)lockedRect.pBits;
      for( y = 0; y < colorFileData.m_imageHeight; ++y )
      {
        D3DCOLOR *dest = destRow;
        BYTE *src = (BYTE*)colorFileData.m_data[y];
        for( UINT32 x = 0; x < colorFileData.m_imageWidth; ++x )
        {      
					if( alphaSrc )
						*(dest++) = D3DCOLOR_ARGB( *(alphaSrc++), *(src), *(src+1), *(src+2) );
					else
	          *(dest++) = D3DCOLOR_XRGB( *(src), *(src+1), *(src+2) );
          src += 3;
        }
        free( colorFileData.m_data[y] );
        colorFileData.m_data[y] = NULL;
        destRow += desc.Width;
      }
    }
    break;
  }

	return ret;
}





//---------------------------------------------------------------------
//	CreateAlphaChannelFromPNGFile
//---------------------------------------------------------------------
BOOL CreateAlphaChannelFromPNGFile( PNGFile_t &alphaFileData, BYTE **alphaChannel )
{
	if( !alphaChannel )
		return FALSE;

		// Convert the PNG file to just alpha bytes
	if( !(*alphaChannel = (BYTE*)malloc( alphaFileData.m_imageWidth * alphaFileData.m_imageHeight )) )
	{
		PRINTMSG(( T_INFO, "Out of memory allocating space for alphaChannel, alpha data will be ignored!" ));
	}
	else
	{
		UINT32 y;
		switch( alphaFileData.m_infoPtr->color_type )
		{
			// *** PNG_COLOR_TYPE_PALETTE *** //
		case PNG_COLOR_TYPE_PALETTE:
			{
				switch( alphaFileData.m_infoPtr->bit_depth )
				{
					// Monochrome
				case 1:
					{
						BYTE *dest = *alphaChannel;
						for( y = 0; y < alphaFileData.m_imageHeight; ++y )
						{
							BYTE *src = (BYTE*)alphaFileData.m_data[y];
							for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 3); ++x )
							{      
								for( INT32 i = 7; i >= 0; --i )
								{
									BOOL palIndex = (*src & (1 << i)) ? 1 : 0;
									*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;
								}
								++src;
							}
							free( alphaFileData.m_data[y] );
							alphaFileData.m_data[y] = NULL;
						}
					}
					break;

				case 2:
					{
						BYTE *dest = *alphaChannel;
						for( y = 0; y < alphaFileData.m_imageHeight; ++y )
						{
							BYTE *src = (BYTE*)alphaFileData.m_data[y];
							for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 2); ++x )
							{      
								BYTE palIndex = (*src >> 6) & 0x03;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								palIndex = (*src >> 4) & 0x03;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								palIndex = (*src >> 2) & 0x03;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								palIndex = (*src) & 0x03;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								++src;
							}
							free( alphaFileData.m_data[y] );
							alphaFileData.m_data[y] = NULL;
						}
					}
					break;

				case 4:
					{
						BYTE *dest = *alphaChannel;
						for( y = 0; y < alphaFileData.m_imageHeight; ++y )
						{
							BYTE *src = (BYTE*)alphaFileData.m_data[y];
							for( UINT32 x = 0; x < (alphaFileData.m_imageWidth >> 1); ++x )
							{
								BYTE palIndex = (*src >> 4) & 0x07;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								palIndex = *src & 0x07;
								*(dest++) = alphaFileData.m_infoPtr->palette[palIndex].green;

								++src;
							}
							free( alphaFileData.m_data[y] );
							alphaFileData.m_data[y] = NULL;
						}
					}
					break;

				default:
					{
						BYTE *dest = *alphaChannel;
						for( y = 0; y < alphaFileData.m_imageHeight; ++y )
						{
							BYTE *src = (BYTE*)alphaFileData.m_data[y];
							for( UINT32 x = 0; x < alphaFileData.m_imageWidth; ++x )
								*(dest++) = alphaFileData.m_infoPtr->palette[*(src++)].green;

							free( alphaFileData.m_data[y] );
							alphaFileData.m_data[y] = NULL;
						}
					}
					break;
				}
			}
			break;

			// *** PNG_COLOR_TYPE_RGB *** //
		case PNG_COLOR_TYPE_RGB:
			{
				BYTE *dest = *alphaChannel;
				for( y = 0; y < alphaFileData.m_imageHeight; ++y )
				{
					BYTE *src = (BYTE*)alphaFileData.m_data[y];
					for( UINT32 x = 0; x < alphaFileData.m_imageWidth; ++x )
					{      
							// Grab the green channel
						*(dest++) = *(src+1);
						src += 3;
					}
					free( alphaFileData.m_data[y] );
					alphaFileData.m_data[y] = NULL;
				}
			}
			break;
		}
	}

	return TRUE;
}
