/**
	* \file			xbox_Direct3DRenderer.cpp
	* \brief		Direct3D rendering subsystem
	*
	*	\note			Major portions taken from XMAME
	*/

//= I N C L U D E S ====================================================
#include <Xtl.h>

#include <stdarg.h>
#include <stdio.h>

#include "MAMEoX.h"
#include "DebugLogger.h"
#include "GraphicsManager.h"
#include "xbox_Direct3DRenderer.h"


	// Font class from the XDK
#include "XBFont.h"

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "vidhrdw/vector.h"     // For vector blitting
}

//= S T R U C T U R E S ===============================================
struct CUSTOMVERTEX
{
	D3DXVECTOR3   pos;  // The transformed position for the vertex
  FLOAT         tu, tv;   // The texture coordinates
};



//= D E F I N E S =====================================================


  // The color to clear the backbuffer to (set to non-black for debugging)
#define D3D_CLEAR_COLOR                 D3DCOLOR_XRGB(0,0,0)

//= G L O B A L = V A R S =============================================

	// The creation parameters passed to osd_create_display
static struct osd_create_params		g_createParams = {0};

static LPDIRECT3DDEVICE8		      g_pD3DDevice = NULL;
static CXBFont							      *g_font = NULL;

static LPDIRECT3DTEXTURE8		      g_pTexture = NULL;
static LPDIRECT3DVERTEXBUFFER8    g_pD3DVertexBuffer = NULL;

	// Defines what portion of the texture is actually rendered to.
  // This rect is converted to TU/TV values in CreateRenderingQuad()
static RECT                       g_textureRenderingArea = {0,0,0,0};

  // Defines the percentage of the total screen area that should actually be used
  // This is required because TV's have some overscan area that is not actually
  // visible
static FLOAT                      g_screenXPercentage = DEFAULT_SCREEN_X_PERCENTAGE;
static FLOAT                      g_screenYPercentage = DEFAULT_SCREEN_Y_PERCENTAGE;

  // Generic D3D Renderer options
RendererOptions_t    g_rendererOptions;

		//! The locked region for us to render to
static D3DLOCKED_RECT             g_d3dLockedRect;


extern "C" {
extern UINT32	g_pal32Lookup[65536];
}

//= P R O T O T Y P E S ===============================================
static void Helper_RenderDirect16( void *destination, struct mame_bitmap *bitmap, const struct rectangle *bounds );
static void Helper_RenderDirect32( void *destination, struct mame_bitmap *bitmap, const struct rectangle *bounds );
static void Helper_RenderPalettized16( void *destination, struct mame_bitmap *bitmap, const struct rectangle *bounds );
static void Helper_RenderVectors( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds, vector_pixel_t *vectorList );
static BOOL CreateTexture( void );
static BOOL CreateRenderingQuad( void );



//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	InitializeD3DRenderer
//-------------------------------------------------------------
void InitializeD3DRenderer( CGraphicsManager &gman, CXBFont *fnt )
{
	g_pD3DDevice = gman.GetD3DDevice();
	g_font = fnt;
}

//-------------------------------------------------------------
//	SetScreenUsage
//-------------------------------------------------------------
void SetScreenUsage( FLOAT xPercentage, FLOAT yPercentage )
{
  g_screenXPercentage = xPercentage;
  g_screenYPercentage = yPercentage;
}

//-------------------------------------------------------------
//	GetScreenUsage
//-------------------------------------------------------------
void GetScreenUsage( FLOAT *xPercentage, FLOAT *yPercentage )
{
  if( xPercentage )
    *xPercentage = g_screenXPercentage;
  if( yPercentage )
    *yPercentage = g_screenYPercentage;
}


extern "C" {

//-------------------------------------------------------------
//	D3DRendererCreateSession
//-------------------------------------------------------------
BOOL D3DRendererCreateSession( struct osd_create_params *params )
{
	PRINTMSG( T_TRACE, "D3DRendererCreateSession" );


#ifdef _PROFILER
    // Since this function will be called when starting to load the function,
    // we can do a memory usage check here
  MEMORYSTATUS memStatus;
  GlobalMemoryStatus(  &memStatus );

  PRINTMSG( T_INFO, "Memory status" );
  PRINTMSG( T_INFO, "Physical:" );
  PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPhys );
  PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPhys );
  PRINTMSG( T_INFO, "Page File:" );
  PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPageFile );
  PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPageFile );
  PRINTMSG( T_INFO, "Virtual:" );
  PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailVirtual );
  PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalVirtual );
#endif

    // Store the creation params
	memcpy( &g_createParams, params, sizeof(g_createParams) );

    // Fill out the orientation from the game driver
  g_createParams.orientation = (Machine->gamedrv->flags & ORIENTATION_MASK);

		// Flip the width and height
	if( g_createParams.orientation & ORIENTATION_SWAP_XY )
	{
		INT32 temp = g_createParams.height;
		g_createParams.height = g_createParams.width;
		g_createParams.width = temp;

		temp = g_createParams.aspect_x;
		g_createParams.aspect_x = g_createParams.aspect_y;
		g_createParams.aspect_y = temp;
	}


    // Assume that the game will render to the entire requested area
  g_textureRenderingArea.right = g_createParams.width;
  g_textureRenderingArea.bottom = g_createParams.height;


		//-- Initialize the texture ---------------------------------------
  if( !CreateTexture() )
    return FALSE;

    // Create the quad that will be used to render the texture
  if( !CreateRenderingQuad() )
    return FALSE;

    //-- Initialize the rendering engine -------------------------------
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );
  g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
  g_pD3DDevice->SetTransform( D3DTS_VIEW, &matWorld );
  g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matWorld );


	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
  g_pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );



	return TRUE;
}

//-------------------------------------------------------------
//	D3DRendererDestroySession
//-------------------------------------------------------------
void D3DRendererDestroySession( void )
{
	PRINTMSG( T_TRACE, "D3DRendererDestroySession" );

	g_pD3DDevice->SetTexture( 0, NULL );

  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }

	if( g_pTexture )
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}
}

//-------------------------------------------------------------
//	D3DRendererSetOutputRect
//-------------------------------------------------------------
void D3DRendererSetOutputRect( INT32 left, INT32 top, INT32 right, INT32 bottom )
{
    // 1) Tear down the preexisting texture
  D3DRendererDestroySession();  

    // 2) Set up the new creation params
	if( g_createParams.orientation & ORIENTATION_SWAP_XY )
  {
      // Need to flip the width and height
    g_createParams.width = bottom;
    g_createParams.height = right;

    g_textureRenderingArea.left = top;
    g_textureRenderingArea.top = left;
    g_textureRenderingArea.right = bottom;
    g_textureRenderingArea.bottom = right;
  }
  else
  {
    g_createParams.width = right;
    g_createParams.height = bottom;

    g_textureRenderingArea.left = left;
    g_textureRenderingArea.top = top;
    g_textureRenderingArea.right = right;
    g_textureRenderingArea.bottom = bottom;
  }


    // 3) Create a new texture
  CreateTexture();

    // 4) Create a new quad
  CreateRenderingQuad();  
}


//-------------------------------------------------------------
//	D3DRendererRender
//-------------------------------------------------------------
BOOL D3DRendererRender(	struct mame_bitmap *bitmap,
												const struct rectangle *bounds,
												void *vector_dirty_pixels )
{
		// Clear the screen
	g_pD3DDevice->Clear(	0L,																// Count
												NULL,															// Rects to clear
												D3DCLEAR_TARGET,	// Flags
												D3D_CLEAR_COLOR,							// Color
												1.0f,															// Z
												0L );															// Stencil

	if( !bitmap )
	{
			// Present the cleared screen
		g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

		return TRUE;
	}

  if( vector_dirty_pixels )
  {
    Helper_RenderVectors( g_d3dLockedRect.pBits, bitmap, bounds, (vector_pixel_t*)vector_dirty_pixels );
  }
  else
  {
		  // Blit the bitmap to the texture
	  if( bitmap->depth == 15 || bitmap->depth == 16 )
	  {
		  if( g_createParams.video_attributes & VIDEO_RGB_DIRECT )
		  {
				  // Destination buffer is in 15 bit X1R5G5B5
        Helper_RenderDirect16( g_d3dLockedRect.pBits, bitmap, bounds );
		  }
		  else
		  {
				  // Have to translate the colors through the palette lookup table
			  Helper_RenderPalettized16( g_d3dLockedRect.pBits, bitmap, bounds );
		  }
	  }
	  else
	  {
		  Helper_RenderDirect32( g_d3dLockedRect.pBits, bitmap, bounds );
	  }	
  }

  g_pD3DDevice->DrawPrimitive( D3DPT_QUADLIST, 0, 1 );
  g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return TRUE;
}

} // End Extern "C"



//-------------------------------------------------------------
//	Helper_RenderDirect16
//-------------------------------------------------------------
static void Helper_RenderDirect16( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds )
{
	struct rectangle bounds = *bnds;
	++bounds.max_x;
	++bounds.max_y;

	if( g_createParams.orientation & ORIENTATION_SWAP_XY )
  {
    UINT16	*sourceBuffer = (UINT16*)bitmap->base;
	  sourceBuffer += (bounds.min_y * bitmap->rowpixels) + (bounds.min_x << 2);

      // SwapXY 
    UINT16 *destBuffer = (UINT16*)dest;
    destBuffer += bounds.min_y;  // The bounds.min_y value gives us our starting X coord
    destBuffer += (bounds.min_x * g_createParams.width); // The bounds.min_x value gives us our starting Y coord

      // Render, treating sourceBuffer as normal (x and y not swapped)
    for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
    {
      UINT16	*offset = destBuffer;
      UINT16  *sourceOffset = sourceBuffer;

			for( UINT32 x = bounds.min_x; x < bounds.max_x; ++x )
      {
        *offset = *(sourceOffset++);
        offset += g_createParams.width;   // Increment the output Y value
      }

      sourceBuffer += bitmap->rowpixels;
      ++destBuffer;          // Come left ("down") one row
    }
  }
  else
	{ 
			// Destination buffer is in 32 bit X8R8G8B8
	  UINT8		*sourceBuffer = (UINT8*)bitmap->base;
	  sourceBuffer += (bounds.min_y * bitmap->rowbytes) + (bounds.min_x << 1);
    UINT8 *destBuffer = (UINT8*)dest;
    destBuffer += ((bounds.min_y * g_createParams.width) + bounds.min_x) << 1;
    UINT32 scanLen = (bounds.max_x - bounds.min_x) << 1;

		for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
		{
      memcpy( destBuffer, sourceBuffer, scanLen );
			destBuffer += (g_createParams.width << 1);
			sourceBuffer += bitmap->rowbytes;
		}
  }
}

//-------------------------------------------------------------
//	Helper_RenderDirect32
//-------------------------------------------------------------
static void Helper_RenderDirect32( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds )
{
	struct rectangle bounds = *bnds;
	++bounds.max_x;
	++bounds.max_y;

		// 32 bit direct
  if( !(g_createParams.video_attributes & VIDEO_RGB_DIRECT) )
  {
		PRINTMSG( T_ERROR, "32 bit palettized mode not supported!" );
    return;
  }

	if( g_createParams.orientation & ORIENTATION_SWAP_XY )
  {
    UINT32	*sourceBuffer = (UINT32*)bitmap->base;
	  sourceBuffer += (bounds.min_y * bitmap->rowpixels) + (bounds.min_x << 2);

      // SwapXY 
    UINT32 *destBuffer = (UINT32*)dest;
    destBuffer += bounds.min_y;  // The bounds.min_y value gives us our starting X coord
    destBuffer += (bounds.min_x * g_createParams.width); // The bounds.min_x value gives us our starting Y coord

      // Render, treating sourceBuffer as normal (x and y not swapped)
    for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
    {
      UINT32	*offset = destBuffer;
      UINT32  *sourceOffset = sourceBuffer;

			for( UINT32 x = bounds.min_x; x < bounds.max_x; ++x )
      {
        *offset = *(sourceOffset++);
        offset += g_createParams.width;   // Increment the output Y value
      }

      sourceBuffer += bitmap->rowpixels;
      ++destBuffer;          // Come left ("down") one row
    }
  }
  else
	{ 
			// Destination buffer is in 32 bit X8R8G8B8
	  UINT8		*sourceBuffer = (UINT8*)bitmap->base;
	  sourceBuffer += (bounds.min_y * bitmap->rowbytes) + (bounds.min_x << 2);
    UINT8 *destBuffer = (UINT8*)dest;
    destBuffer += ((bounds.min_y * g_createParams.width) + bounds.min_x) << 2;
    UINT32 scanLen = (bounds.max_x - bounds.min_x) << 2;

		for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
		{
      memcpy( destBuffer, sourceBuffer, scanLen );
			destBuffer += (g_createParams.width << 2);
			sourceBuffer += bitmap->rowbytes;
		}
  }
}



//-------------------------------------------------------------
//	Helper_RenderPalettized16
//-------------------------------------------------------------
static void Helper_RenderPalettized16( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds )
{
	struct rectangle bounds = *bnds;
	++bounds.max_x;
	++bounds.max_y;

		// bitmap format is 16 bit indices into the palette
		// Destination buffer is in 32 bit X8R8G8B8
	if( g_createParams.orientation & ORIENTATION_SWAP_XY )
	{ 
    UINT16 *sourceBuffer = (UINT16*)bitmap->base;
    sourceBuffer += (bounds.min_y * bitmap->rowpixels) + bounds.min_x;

      // SwapXY
    UINT32 *destBuffer = (UINT32*)dest;
    destBuffer += bounds.min_y;  // The bounds.min_y value gives us our starting X coord
    destBuffer += (bounds.min_x * g_createParams.width); // The bounds.min_x value gives us our starting Y coord

      // Render, treating sourceBuffer as normal (x and y not swapped)
    for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
    {
      UINT32	*offset = destBuffer;
      UINT16  *sourceOffset = sourceBuffer;

			for( UINT32 x = bounds.min_x; x < bounds.max_x; ++x )
      {
          // Offset is in RGBX format	
        *offset = g_pal32Lookup[ *(sourceOffset++) ];

          // Skip to the next row
        offset += g_createParams.width;   // Increment the output Y value
      }

      sourceBuffer += bitmap->rowpixels;
      ++destBuffer;          // Come left ("down") one row
    }
	}
	else
	{
		UINT16 *sourceBuffer = (UINT16*)bitmap->base;
    sourceBuffer += (bounds.min_y * bitmap->rowpixels) + bounds.min_x;

		UINT32	*destBuffer = (UINT32*)dest;
    destBuffer += (bounds.min_y * g_createParams.width + bounds.min_x);

		for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
		{
			UINT32	*offset = destBuffer;
			UINT16  *sourceOffset = sourceBuffer;

			for( UINT32 x = bounds.min_x; x < bounds.max_x; ++x )
			{
          // Offset is in RGBX format	
        *(offset++) = g_pal32Lookup[ *(sourceOffset++) ];
			}

			destBuffer += g_createParams.width;
      sourceBuffer += bitmap->rowpixels;
		}
	}
}
    


//-------------------------------------------------------------
//	Helper_RenderVectors
//-------------------------------------------------------------
static void Helper_RenderVectors( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds, vector_pixel_t *vectorList )
{
  UINT32 *destBuf = (UINT32*)dest;
  if( !bitmap || !bitmap->base || !destBuf )
    return;

  if( bitmap->depth == 32 )
  {
    UINT32 *sourceBuf = (UINT32*)bitmap->base;

	  if( g_createParams.orientation & ORIENTATION_SWAP_XY )
    {
	    while( *vectorList != VECTOR_PIXEL_END )
	    {
		    vector_pixel_t coords = *(vectorList++);
		    INT32 x = VECTOR_PIXEL_X( coords );
		    INT32 y = VECTOR_PIXEL_Y( coords );
        destBuf[ (bnds->min_y + y ) + ((bnds->min_x + x) * g_createParams.width) ] = sourceBuf[ (bnds->min_x + x) + (bnds->min_y + y) * bitmap->rowpixels ];
	    }
    }
    else
    {
	    while( *vectorList != VECTOR_PIXEL_END )
	    {
		    vector_pixel_t coords = *(vectorList++);
		    INT32 x = VECTOR_PIXEL_X( coords );
		    INT32 y = VECTOR_PIXEL_Y( coords );
        destBuf[ (bnds->min_x + x ) + ((bnds->min_y + y) * g_createParams.width) ] = sourceBuf[ (bnds->min_x + x) + (bnds->min_y + y) * bitmap->rowpixels ];
	    }
    }
  }
  else
  {
    UINT16 *sourceBuf = (UINT16*)bitmap->base;

	  if( g_createParams.orientation & ORIENTATION_SWAP_XY )
    {
	    while( *vectorList != VECTOR_PIXEL_END )
	    {
		    vector_pixel_t coords = *(vectorList++);
		    INT32 x = VECTOR_PIXEL_X( coords );
		    INT32 y = VECTOR_PIXEL_Y( coords );
        UINT32 color = g_pal32Lookup[ sourceBuf[ (bnds->min_x + x) + (bnds->min_y + y) * bitmap->rowpixels ] ];
        destBuf[ (bnds->min_y + y ) + ((bnds->min_x + x) * g_createParams.width) ] = color;
	    }
    }
    else
    {
	    while( *vectorList != VECTOR_PIXEL_END )
	    {
		    vector_pixel_t coords = *(vectorList++);
		    INT32 x = VECTOR_PIXEL_X( coords );
		    INT32 y = VECTOR_PIXEL_Y( coords );
        UINT32 color = g_pal32Lookup[ sourceBuf[ (bnds->min_x + x) + (bnds->min_y + y) * bitmap->rowpixels ] ];
        destBuf[ (bnds->min_x + x ) + ((bnds->min_y + y) * g_createParams.width) ] = color;
	    }
    }
  }
}






//-------------------------------------------------------------
//  CreateTexture
//-------------------------------------------------------------
static BOOL CreateTexture( void )
{
	if( g_pTexture )
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}

		// Create the texture	
	D3DFORMAT textureFormat;

		// Use 32 bit color for palettized or 32 bit sessions
	if( !(g_createParams.video_attributes & VIDEO_RGB_DIRECT) || g_createParams.depth >= 24 )
		textureFormat = D3DFMT_LIN_X8R8G8B8;
	else if( g_createParams.depth >= 15 )
		textureFormat = D3DFMT_LIN_X1R5G5B5;

  if( (D3DXCreateTexture( g_pD3DDevice,
                          g_createParams.width,
                          g_createParams.height,
													1,									// Mip levels
                          0,                  // Usage
													textureFormat,      // Format
													0,		              // Pool (unused)
                          &g_pTexture )) != S_OK )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus(  &memStatus );

    PRINTMSG( T_ERROR, "Failed to create texture" );
    PRINTMSG( T_INFO, "Memory status" );
    PRINTMSG( T_INFO, "Physical:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPhys );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPhys );
    PRINTMSG( T_INFO, "Page File:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailPageFile );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalPageFile );
    PRINTMSG( T_INFO, "Virtual:" );
    PRINTMSG( T_INFO, "         Avail: %lu", memStatus.dwAvailVirtual );
    PRINTMSG( T_INFO, "         Total: %lu", memStatus.dwTotalVirtual );

    osd_print_error(  "Out of memory\n%lu available of %lu total.",
                      memStatus.dwAvailPhys, 
                      memStatus.dwTotalPhys );
		return FALSE;
	}

		// Grab the surface description
	D3DSURFACE_DESC desc;
  g_pTexture->GetLevelDesc( 0, &desc );

    // Since the texture may have taken width and height values other than the
    // ones we specified, make sure they're correct now. These values are used
    // as the stride for the Render* functions
  g_createParams.width = desc.Width;
  g_createParams.height = desc.Height;

	g_pD3DDevice->SetTexture( 0, g_pTexture );

  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, g_rendererOptions.m_minFilter );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, g_rendererOptions.m_magFilter );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE  );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
  g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP );


    // Lock the region so we can render to it later
	g_pTexture->LockRect(	0, &g_d3dLockedRect, NULL, 0 );


  return TRUE;
}

//-------------------------------------------------------------
//  CreateRenderingQuad
//-------------------------------------------------------------
static BOOL CreateRenderingQuad( void )
{
  if( g_pD3DVertexBuffer )
  {
    g_pD3DVertexBuffer->Release();
    g_pD3DVertexBuffer = NULL;
  }

    // Create the vertex buffer
	g_pD3DDevice->CreateVertexBuffer( sizeof(CUSTOMVERTEX) << 2,
																		D3DUSAGE_WRITEONLY,
																		D3DFVF_XYZ | D3DFVF_TEX1,
																		D3DPOOL_MANAGED,
																		&g_pD3DVertexBuffer );




    // Calculate the TU/TV coords based on the rendering area
  FLOAT tu_l, tv_t, tu_r, tv_b;
  tu_l = (FLOAT)g_textureRenderingArea.left+1;
  tu_r = (FLOAT)(g_textureRenderingArea.right);
  tv_t = (FLOAT)g_textureRenderingArea.top;
  tv_b = (FLOAT)(g_textureRenderingArea.bottom+1);

  FLOAT xpos = g_screenXPercentage;
  FLOAT ypos = g_screenYPercentage;

  if( g_rendererOptions.m_screenRotation == SR_0 || g_rendererOptions.m_screenRotation == SR_180 )
  {

    if( g_rendererOptions.m_preserveAspectRatio )
    {
        // Aspect ratio
      double screenRatio = 640.0/480.0;

        // The desired aspect ratio for the game
	    double aspectRatio = (double)g_createParams.aspect_x / (double)g_createParams.aspect_y;
      if( g_createParams.video_attributes & VIDEO_PIXEL_ASPECT_RATIO_1_2 )
        aspectRatio /= 2.0;
      else if( g_createParams.video_attributes & VIDEO_PIXEL_ASPECT_RATIO_2_1 )
        aspectRatio *= 2.0;

        // The native screenRatio is 4/3
        // so multiplying x by the desired aspect ratio will actually give us (x*4/3)*(aspectRatio)
        // Therefore we have to first counteract the real screen ratio before applying the desired aspect ratio

      if( aspectRatio > screenRatio )
      {
          // scale down y
        ypos /= aspectRatio * screenRatio; 
      }
      else if( aspectRatio < screenRatio )
      {
          // Scale down x
        xpos *= aspectRatio / screenRatio;
      }
    }
  }
  else
  {
      // We're rendering sideways, so the aspect ratio of the monitor is different
    if( g_rendererOptions.m_preserveAspectRatio )
    {
        // Aspect ratio
      double screenRatio = 480.0/640.0;

        // The desired aspect ratio for the game
	    double aspectRatio = (double)g_createParams.aspect_x / (double)g_createParams.aspect_y;
      if( g_createParams.video_attributes & VIDEO_PIXEL_ASPECT_RATIO_1_2 )
        aspectRatio *= 2.0;
      else if( g_createParams.video_attributes & VIDEO_PIXEL_ASPECT_RATIO_2_1 )
        aspectRatio /= 2.0;

        // The native screenRatio is 3/4
        // so multiplying x by the desired aspect ratio will actually give us (x*3/4)*(aspectRatio)
        // Therefore we have to first counteract the real screen ratio before applying the desired aspect ratio

      if( aspectRatio > screenRatio )
      {
          // scale down y
        xpos /= aspectRatio * screenRatio; 
      }
      else if( aspectRatio < screenRatio )
      {
          // Scale down x
        ypos *= aspectRatio / screenRatio;
      }
    }
  }



	CUSTOMVERTEX *pVertices;
	g_pD3DVertexBuffer->Lock( 0,										// Offset to lock
														0,										// Size to lock
														(BYTE**)&pVertices,		// ppbData
														0 );									// Flags


    switch( g_rendererOptions.m_screenRotation )
    {
    case SR_0:
      pVertices[0].tu = tu_l;
      pVertices[0].tv = tv_t;
      pVertices[1].tu = tu_r;
      pVertices[1].tv = tv_t;
      pVertices[2].tu = tu_r;
      pVertices[2].tv = tv_b;
      pVertices[3].tu = tu_l;
      pVertices[3].tv = tv_b;
      break;

    case SR_90:
      pVertices[0].tu = tu_r;
      pVertices[0].tv = tv_t;
      pVertices[1].tu = tu_r;
      pVertices[1].tv = tv_b;
      pVertices[2].tu = tu_l;
      pVertices[2].tv = tv_b;
      pVertices[3].tu = tu_l;
      pVertices[3].tv = tv_t;
      break;

    case SR_180:
      pVertices[0].tu = tu_r;
      pVertices[0].tv = tv_b;
      pVertices[1].tu = tu_l;
      pVertices[1].tv = tv_b;
      pVertices[2].tu = tu_l;
      pVertices[2].tv = tv_t;
      pVertices[3].tu = tu_r;
      pVertices[3].tv = tv_t;
      break;

    case SR_270:
      pVertices[0].tu = tu_l;
      pVertices[0].tv = tv_b;
      pVertices[1].tu = tu_l;
      pVertices[1].tv = tv_t;
      pVertices[2].tu = tu_r;
      pVertices[2].tv = tv_t;
      pVertices[3].tu = tu_r;
      pVertices[3].tv = tv_b;
      break;
    }

		pVertices[0].pos.x = -xpos;
		pVertices[0].pos.y = ypos;
		pVertices[0].pos.z = 1.0f;

		pVertices[1].pos.x = xpos;
		pVertices[1].pos.y = ypos;
		pVertices[1].pos.z = 1.0f;
		
		pVertices[2].pos.x = xpos;
		pVertices[2].pos.y = -ypos;
		pVertices[2].pos.z = 1.0f;
		
		pVertices[3].pos.x = -xpos;
		pVertices[3].pos.y = -ypos;
		pVertices[3].pos.z = 1.0f;

	  if( g_createParams.orientation & ORIENTATION_FLIP_X )
    {
      pVertices[0].pos.x *= -1.0f;
      pVertices[1].pos.x *= -1.0f;
      pVertices[2].pos.x *= -1.0f;
      pVertices[3].pos.x *= -1.0f;
    }
    if( g_createParams.orientation & ORIENTATION_FLIP_Y )
    {
      pVertices[0].pos.y *= -1.0f;
      pVertices[1].pos.y *= -1.0f;
      pVertices[2].pos.y *= -1.0f;
      pVertices[3].pos.y *= -1.0f;
    }

	g_pD3DVertexBuffer->Unlock();



  g_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX1 );
  g_pD3DDevice->SetStreamSource(	0,												  // Stream number
																	g_pD3DVertexBuffer,					// Stream data
																	sizeof(CUSTOMVERTEX) );		  // Vertex stride



  return TRUE;
}

