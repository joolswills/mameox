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


//= P R O T O T Y P E S ================================================
#ifdef __cplusplus

	//-------------------------------------------------------------
	//	InitializeD3DRenderer
	//! \brief		Initializes the D3DRenderer subsystem. Must be
	//!            called before any other D3DRenderer calls
	//-------------------------------------------------------------
void InitializeD3DRenderer( CGraphicsManager &gman, CXBFont *fnt );



extern "C" {
#endif
	

	//-------------------------------------------------------------
	//	D3DRendererCreateSession
	//! \brief		Create a D3D rendering session
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
	//-------------------------------------------------------------
void D3DRendererSetOutputRect( INT32 left, INT32 top, INT32 right, INT32 bottom );


	//-------------------------------------------------------------
	//	D3DRendererRender
	//! \brief		Render the passed bitmap
	//-------------------------------------------------------------
BOOL D3DRendererRender(	struct mame_bitmap *bitmap,
												const struct rectangle *bounds,
												void *vector_dirty_pixels );

#ifdef __cplusplus
}
#endif

