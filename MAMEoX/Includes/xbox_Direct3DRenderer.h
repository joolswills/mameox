/**
	* \file			xbox_Direct3DRenderer.h
	* \brief		Declarations for the Direct3D rendering subsystem
	*/

#pragma once


//= I N C L U D E S ====================================================
#ifdef __cplusplus
#include <Xtl.h>
#include "GraphicsManager.h"
#include "XBFont.h"

extern "C" {
#endif

#include "osd_cpu.h"
#include "osdepend.h"

#include "mame.h"
#include "common.h"

#ifdef __cplusplus
}
#endif

//= D E F I N E S ======================================================
  // The default screen usage percentages, see g_screenXPercentage comments below
#define DEFAULT_SCREEN_X_PERCENTAGE     0.85f
#define DEFAULT_SCREEN_Y_PERCENTAGE     0.85f

// 16bpp color conversion
#define WIN_COLOR16_RSRC_SHIFT	3
#define WIN_COLOR16_GSRC_SHIFT	3
#define WIN_COLOR16_BSRC_SHIFT	3
#define WIN_COLOR16_RDST_SHIFT	10
#define WIN_COLOR16_GDST_SHIFT	5
#define WIN_COLOR16_BDST_SHIFT	0

// 32bpp color conversion
#define WIN_COLOR32_RDST_SHIFT	16
#define WIN_COLOR32_GDST_SHIFT	8
#define WIN_COLOR32_BDST_SHIFT	0


#define WIN_COLOR16( _r__, _g__, _b__ ) \
											(UINT16)( (( ((UINT8)(_r__)) >> WIN_COLOR16_RSRC_SHIFT) << WIN_COLOR16_RDST_SHIFT) | \
														    (( ((UINT8)(_g__)) >> WIN_COLOR16_GSRC_SHIFT) << WIN_COLOR16_GDST_SHIFT) | \
															  (( ((UINT8)(_b__)) >> WIN_COLOR16_BSRC_SHIFT) << WIN_COLOR16_BDST_SHIFT) )

#define WIN_COLOR32( _r__, _g__, _b__ ) \
                      (UINT32)( (((UINT8)(_r__)) << WIN_COLOR32_RDST_SHIFT) | \
                                (((UINT8)(_g__)) << WIN_COLOR32_GDST_SHIFT) | \
                                (((UINT8)(_b__)) << WIN_COLOR32_BDST_SHIFT) )


//= G L O B A L = V A R S ==============================================
  //! Whether or not to use aspect ratio correction code
extern BOOL g_preserveAspectRatio;

//= P R O T O T Y P E S ================================================
#ifdef __cplusplus

	//-------------------------------------------------------------
	//	InitializeD3DRenderer
	//! \brief		Initializes the D3DRenderer subsystem. Must be
	//!            called before any other D3DRenderer calls.
  //!
  //! \param    gman - A CGraphicsManager instance for the output device
  //! \param    fnt - A pointer to the font to be used for output
	//-------------------------------------------------------------
void InitializeD3DRenderer( CGraphicsManager &gman, CXBFont *fnt );


	//-------------------------------------------------------------
	//	SetScreenUsage
	//! \brief		Set how much of the screen is utilized
  //!
  //! \param    xPercentage - Usage in the X direction
  //! \param    yPercentage - Usage in the Y direction
	//-------------------------------------------------------------
void SetScreenUsage( FLOAT xPercentage = DEFAULT_SCREEN_X_PERCENTAGE, FLOAT yPercentage = DEFAULT_SCREEN_Y_PERCENTAGE );

	//-------------------------------------------------------------
	//	GetScreenUsage
	//! \brief		Query how much of the screen is utilized
  //!
  //! \param    xPercentage - [OUT] Usage in the X direction
  //! \param    yPercentage - [OUT] Usage in the Y direction
	//-------------------------------------------------------------
void GetScreenUsage( FLOAT *xPercentage, FLOAT *yPercentage );

extern "C" {
#endif
	

	//-------------------------------------------------------------
	//	D3DRendererCreateSession
	//! \brief		Create a D3D rendering session
  //!
  //! \param    params - The osd_create_params struct passed to
  //!                    osd_create_display()
  //!
  //! \return   BOOL - Operation status
  //! \retval   TRUE - Operation succeded
  //! \retval   FALSE - Operation failed
	//-------------------------------------------------------------
BOOL D3DRendererCreateSession( struct osd_create_params *params );


	//-------------------------------------------------------------
	//	D3DRendererDestroySession
	//! \brief		Destroy the current D3D rendering session
	//-------------------------------------------------------------
void D3DRendererDestroySession( void );

	//-------------------------------------------------------------
	//	D3DRendererSetOutputRect
	//! \brief		Set a new desired output rectangle
  //!
  //! \param    left - The left hand side
  //! \param    top - The top side
  //! \param    right - The right hand side, inclusive
  //! \param    bottom - The bottom side, inclusive
	//-------------------------------------------------------------
void D3DRendererSetOutputRect( INT32 left, INT32 top, INT32 right, INT32 bottom );


	//-------------------------------------------------------------
	//	D3DRendererRender
	//! \brief		Render the passed bitmap
  //!
  //! \param    bitmap - The bitmap passed via the osd_update_video_and_audio
  //!                    mame_display parameter.
  //! \param    bounds - The bounds rectangle passed via the mame_display parameter.
  //! \param    vector_dirty_pixels - List of dirty pixels for vector rendering, or NULL
	//-------------------------------------------------------------
BOOL D3DRendererRender(	struct mame_bitmap *bitmap,
												const struct rectangle *bounds,
												void *vector_dirty_pixels );

#ifdef __cplusplus
}
#endif

