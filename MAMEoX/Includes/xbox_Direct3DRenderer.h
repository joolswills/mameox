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

#define AUTO_FRAMESKIP                  -1 
#define FRAMESKIP_LEVELS			12 



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


typedef enum screenrotation_t
{
  SR_0 = 0x00,      // 0 degrees (normal)
  SR_90,            // 90 degrees CW
  SR_180,           // 180 degrees
  SR_270            // 270 degrees CW
} screenrotation_t;

//= S T R U C T U R E S ================================================
typedef struct RendererOptions_t
{
  FLOAT                 m_screenUsageX;     //!< Screen usage along the X axis [0.25,1.0]
  FLOAT                 m_screenUsageY;     //!< Screen usage along the Y axis [0.25,1.0]
  FLOAT                 m_screenOffsetX;    //!< Offset of the screen from the upper left hand corner (0,0)
  FLOAT                 m_screenOffsetY;    //!< Offset of the screen from the upper left hand corner (0,0)

  INT32                 m_frameskip;        //!< Frameskip level, -1 == auto, 0 = no frameskip

    //! Whether or not to use aspect ratio correction code
  BOOL                  m_preserveAspectRatio;    

    //! Whether or not to sync to vertical retrace
  BOOL                  m_vsync;

    //! Whether or not we should try to stay at the games desired refresh rate
  BOOL                  m_throttleFramerate;

    //! Level of screen rotation
  screenrotation_t      m_screenRotation;

    //! Minimization filter op
  D3DTEXTUREFILTERTYPE  m_minFilter;

    //! Maximization filter op
  D3DTEXTUREFILTERTYPE  m_magFilter;

} RendererOptions_t;

//= G L O B A L = V A R S ==============================================
#ifdef __cplusplus
extern "C" {
#endif

extern RendererOptions_t    g_rendererOptions;

#ifdef __cplusplus
}
#endif

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

extern "C" {


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

	//-------------------------------------------------------------
	//	SetScreenPosition
	//! \brief		Set the offset of the upper left hand corner of
  //!            the rendering quad
  //!
  //! \param    xOffset - Offset from the left side of the screen
  //! \param    yOffset - Offset from the top side of the screen
	//-------------------------------------------------------------
void SetScreenPosition( FLOAT xOffset = 0.0f, FLOAT yOffset = 0.0f );

	//-------------------------------------------------------------
	//	GetScreenPosition
	//! \brief		Get the offset of the upper left hand corner of
  //!            the rendering quad
  //!
  //! \param    xOffset - [OUT] Offset in the X direction
  //! \param    yOffset - [OUT] Offset in the Y direction
	//-------------------------------------------------------------
void GetScreenPosition( FLOAT *xPercentage, FLOAT *yPercentage );

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

