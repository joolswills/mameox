/**
	* \file			ScreensaverScreen.cpp
	* \brief		Helper class which takes care of generating/loading/displaying
	*           a list of all available ROMs
	*/

//= I N C L U D E S ====================================================
#include "ScreensaverScreen.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include "xbox_FileIO.h"		// for path info
#include "xbox_Direct3DRenderer.h" // For Set/GetScreenUsage
#include "smbhandler.h"
#include "System_IniFile.h"

#include <string>
#include <vector>
#include <algorithm>

extern "C" {
#include "osdepend.h"
#include "driver.h"
}

//= S T R U C T U R E S ===============================================

//= D E F I N E S ======================================================
#define TICKS_PER_SCREENSHOT      15 * osd_cycles_per_second()

//= G L O B A L = V A R S ==============================================
  // Static member initialization
MAMEDriverData_t                        *CScreensaverScreen::m_driverInfoList = NULL;
UINT32                                  CScreensaverScreen::m_numDrivers = 0;

//= P R O T O T Y P E S ================================================
inline BOOL Helper_FindScreenshotDriverName( const char *filename, const MAMEDriverData_t &driver );

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	Draw
//---------------------------------------------------------------------
void CScreensaverScreen::Draw( BOOL clearScreen, BOOL flipOnCompletion )
{
  if( clearScreen )  
	  m_displayDevice->Clear(	0L,																// Count
		  											NULL,															// Rects to clear
			  										D3DCLEAR_TARGET,	                // Flags
				  									D3DCOLOR_XRGB(0,0,0),							// Color
					  								1.0f,															// Z
						  							0L );															// Stencil

	if( !g_loadedSkin )
		return;

  static textMessageX = 0, textMessageY = 0;
  static UINT64 lastTime = 0;
	UINT64 curTime = osd_cycles();
	UINT64 elapsedTime = curTime - lastTime;
	lastTime = curTime;

	if( elapsedTime >= m_displayTimeout )
  {
      // Load a screenshot file. The rand() allows us to display a
			// black frame every so often, just in case all of the
			// screenshots are the same image
    if( m_screenshotFiles.size() && (rand() % (m_screenshotFiles.size() + 1)) )
    {
      UINT32 randIndex = rand() % m_screenshotFiles.size();
      CStdString str = g_FileIOConfig.m_screenshotPath;
      str += "\\";
      str += m_screenshotFiles[ randIndex ];
      LoadPNGToTexture( str, &m_screenshotTexture, &m_screenshotRect );

        // Look up the m_screenshotOrientation from the driver list
        // Have to do a linear search as the list is not sorted at all
      UINT32 i = 0;
      for( ; i < m_numDrivers; ++i )
      {
        if( Helper_FindScreenshotDriverName( m_screenshotFiles[randIndex].c_str(), m_driverInfoList[i] ) )
        {
          m_screenshotOrientation = m_driverInfoList[i].m_screenOrientation;
          break;
        }
      }
    }
    else
    {
      SAFE_RELEASE( m_screenshotTexture );
      m_screenshotRect.left = m_screenshotRect.top = 0;
      m_screenshotRect.right = 640;
      m_screenshotRect.bottom = 480;
      m_screenshotOrientation = 0;

        // Move the text message around
      textMessageX = rand() % 320 + 160;
      textMessageY = rand() % 240 + 120;
    }

    CalculateRenderingQuad();
    m_displayTimeout = TICKS_PER_SCREENSHOT;
  }
  else
    m_displayTimeout -= elapsedTime;

  if( m_screenshotTexture )
  {
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    m_displayDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_displayDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEX0 );
	  m_displayDevice->SetTexture( 0, m_screenshotTexture );

    m_displayDevice->Begin( D3DPT_QUADLIST );      

        // Apply screenshot rotations
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_renderingTextureCoords[0][0], m_renderingTextureCoords[0][1] );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_renderingQuadCoords[0][0], m_renderingQuadCoords[0][1], 1.0f, 1.0f );
      
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_renderingTextureCoords[1][0], m_renderingTextureCoords[1][1] );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_renderingQuadCoords[1][0], m_renderingQuadCoords[1][1], 1.0f, 1.0f );
      
      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_renderingTextureCoords[2][0], m_renderingTextureCoords[2][1] );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_renderingQuadCoords[2][0], m_renderingQuadCoords[2][1], 1.0f, 1.0f );

      m_displayDevice->SetVertexData2f( D3DVSDE_TEXCOORD0, m_renderingTextureCoords[3][0], m_renderingTextureCoords[3][1] );
      m_displayDevice->SetVertexData4f( D3DVSDE_VERTEX, m_renderingQuadCoords[3][0], m_renderingQuadCoords[3][1], 1.0f, 1.0f );
    m_displayDevice->End();
  }
	else if( CheckResourceValidity( SKINELEMENT_SCREENSAVEROPTIONS_TEXT ) )
  {
      // Render something
		g_loadedSkin->GetSkinElementText(SKINELEMENT_SCREENSAVEROPTIONS_TEXT)->Render(	m_displayDevice, 
																																										m_screenshotFiles.size() ? L"Screensaver mode" : L"Screensaver active but there\nare no screenshots to display.", 
																																										textMessageX, 
																																										textMessageY );
  }

  if( flipOnCompletion )
	  m_displayDevice->Present( NULL, NULL, NULL, NULL );	
}

//---------------------------------------------------------------------
//	FindScreenshots
//---------------------------------------------------------------------
void CScreensaverScreen::FindScreenshots( void )
{
  WIN32_FIND_DATA findData;

  m_screenshotFiles.clear();

  CStdString findPath = g_FileIOConfig.m_screenshotPath;
  findPath += "\\*.png";

  HANDLE h = FindFirstFile( findPath.c_str(), &findData );
  if( h != INVALID_HANDLE_VALUE )
  {
    do {
      PRINTMSG(( T_INFO, "Found screenshot: %s", findData.cFileName ));
      m_screenshotFiles.push_back( findData.cFileName );
    } while( FindNextFile( h, &findData ) );
    FindClose( h );
  }
}



//-------------------------------------------------------------
//  CreateRenderingQuad
//-------------------------------------------------------------
void CScreensaverScreen::CalculateRenderingQuad( void )
{
    // Calculate the TU/TV coords based on the rendering area
  FLOAT tu_l, tv_t, tu_r, tv_b;
  tu_l = (FLOAT)m_screenshotRect.left;
  tu_r = (FLOAT)m_screenshotRect.right;
  tv_t = (FLOAT)m_screenshotRect.top;
  tv_b = (FLOAT)m_screenshotRect.bottom;

  FLOAT xpos, ypos;
  GetScreenUsage( &xpos, &ypos );

  if( g_rendererOptions.m_preserveAspectRatio )
  {
    FLOAT xPercentage, yPercentage;
    GetScreenUsage( &xPercentage, &yPercentage );

    if( g_rendererOptions.m_screenRotation == SR_0 || g_rendererOptions.m_screenRotation == SR_180 )
    {
        // Hack: Assume that swap XY means we're using 3:4 aspect, otherwise 4:3

        // Aspect ratio
      double screenRatio = 640.0/480.0;
      //double screenRatio = (640.0 * xPercentage) / (480.0 * yPercentage);

        // The desired aspect ratio for the game
      double aspectRatio = ( m_screenshotOrientation & ORIENTATION_SWAP_XY ) ? (3.0/4.0) : (4.0/3.0);

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
    else
    {
        // Hack: Assume that swap XY means we're using 3:4 aspect, otherwise 4:3
        // Aspect ratio
      double screenRatio = 480.0/640.0;
      //double screenRatio = (480.0 * yPercentage) / (640.0 * xPercentage);

        // The desired aspect ratio for the game
      double aspectRatio = ( m_screenshotOrientation & ORIENTATION_SWAP_XY ) ? (3.0/4.0) : (4.0/3.0);

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

    // Handle screen rotation and 
  switch( g_rendererOptions.m_screenRotation )
  {
  case SR_0:
    m_renderingTextureCoords[0][0] = tu_l;
    m_renderingTextureCoords[0][1] = tv_t;
    m_renderingTextureCoords[1][0] = tu_r;
    m_renderingTextureCoords[1][1] = tv_t;
    m_renderingTextureCoords[2][0] = tu_r;
    m_renderingTextureCoords[2][1] = tv_b;
    m_renderingTextureCoords[3][0] = tu_l;
    m_renderingTextureCoords[3][1] = tv_b;
    break;

  case SR_90:
    m_renderingTextureCoords[0][0] = tu_r;
    m_renderingTextureCoords[0][1] = tv_t;
    m_renderingTextureCoords[1][0] = tu_r;
    m_renderingTextureCoords[1][1] = tv_b;
    m_renderingTextureCoords[2][0] = tu_l;
    m_renderingTextureCoords[2][1] = tv_b;
    m_renderingTextureCoords[3][0] = tu_l;
    m_renderingTextureCoords[3][1] = tv_t;
    break;

  case SR_180:
    m_renderingTextureCoords[0][0] = tu_r;
    m_renderingTextureCoords[0][1] = tv_b;
    m_renderingTextureCoords[1][0] = tu_l;
    m_renderingTextureCoords[1][1] = tv_b;
    m_renderingTextureCoords[2][0] = tu_l;
    m_renderingTextureCoords[2][1] = tv_t;
    m_renderingTextureCoords[3][0] = tu_r;
    m_renderingTextureCoords[3][1] = tv_t;
    break;

  case SR_270:
    m_renderingTextureCoords[0][0] = tu_l;
    m_renderingTextureCoords[0][1] = tv_b;
    m_renderingTextureCoords[1][0] = tu_l;
    m_renderingTextureCoords[1][1] = tv_t;
    m_renderingTextureCoords[2][0] = tu_r;
    m_renderingTextureCoords[2][1] = tv_t;
    m_renderingTextureCoords[3][0] = tu_r;
    m_renderingTextureCoords[3][1] = tv_b;
    break;
  }

	m_renderingQuadCoords[0][0] = -xpos;
	m_renderingQuadCoords[0][1] = ypos;
	m_renderingQuadCoords[1][0] = xpos;
	m_renderingQuadCoords[1][1] = ypos;
	m_renderingQuadCoords[2][0] = xpos;
	m_renderingQuadCoords[2][1] = -ypos;
	m_renderingQuadCoords[3][0] = -xpos;
	m_renderingQuadCoords[3][1] = -ypos;

	m_renderingQuadCoords[0][0] += g_rendererOptions.m_screenOffsetX;
	m_renderingQuadCoords[0][1] += g_rendererOptions.m_screenOffsetY;
	m_renderingQuadCoords[1][0] += g_rendererOptions.m_screenOffsetX;
	m_renderingQuadCoords[1][1] += g_rendererOptions.m_screenOffsetY;
	m_renderingQuadCoords[2][0] += g_rendererOptions.m_screenOffsetX;
	m_renderingQuadCoords[2][1] += g_rendererOptions.m_screenOffsetY;
	m_renderingQuadCoords[3][0] += g_rendererOptions.m_screenOffsetX;
	m_renderingQuadCoords[3][1] += g_rendererOptions.m_screenOffsetY;
}

//---------------------------------------------------------------------
//  Helper_FindScreenshotDriverName
//---------------------------------------------------------------------
inline BOOL Helper_FindScreenshotDriverName( const char *filename, const MAMEDriverData_t &driver )
{
  const char *extension = strrchr( filename, '.' );  
  return !strnicmp( filename, driver.m_romFileName, extension - filename < 4 ? extension - filename : 4 );
}

