/**
	* \file		GraphicsManager.h
	*
	*/

#ifndef _GRAPHICSMANAGER_H__
#define _GRAPHICSMANAGER_H__


//= I N C L U D E S ===========================================================
#include "MAMEoX.h"


//= C L A S S E S =============================================================
class CGraphicsManager
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CGraphicsManager( void ) {
		memset( this, NULL, sizeof(*this) );
	}


		//------------------------------------------------------
		//	Create
		//------------------------------------------------------
	BOOL Create( BOOL enableVSYNC ) {
		if( m_created )
			return FALSE;


		if( !(m_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
			return FALSE;

    D3DPRESENT_PARAMETERS params; 
    memset( &params, 0, sizeof(params) );
    
    params.BackBufferWidth				= 640;
    params.BackBufferHeight				= 480;
    params.BackBufferFormat				= D3DFMT_X8R8G8B8;
    params.BackBufferCount				= 1;
    params.EnableAutoDepthStencil = FALSE;
    params.AutoDepthStencilFormat = D3DFMT_D24S8;
    params.SwapEffect							= D3DSWAPEFFECT_FLIP;
    params.FullScreen_RefreshRateInHz = 60;
    params.FullScreen_PresentationInterval = enableVSYNC ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
//    params.FullScreen_PresentationInterval = enableVSYNC ? D3DPRESENT_INTERVAL_ONE_OR_IMMEDIATE : D3DPRESENT_INTERVAL_IMMEDIATE;


      // Since many games run at 60Hz, it's easier to be at 60Hz for proper sound emulation
    if( XGetVideoStandard() == XC_VIDEO_STANDARD_PAL_I && !(XGetVideoFlags() & XC_VIDEO_FLAGS_PAL_60Hz) )
      params.Flags = D3DPRESENTFLAG_EMULATE_REFRESH_RATE;

		if( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,										// Adapter
															D3DDEVTYPE_HAL,												// Devtype
															NULL,																	// unused
															D3DCREATE_HARDWARE_VERTEXPROCESSING,	// BehaviorFlags
															&params,															// Presentation parameters
															&m_pD3DDevice ) < 0 )
		{
			return FALSE;
		}


		return TRUE;
	}

		//------------------------------------------------------
		//	GetD3DDevice
		//------------------------------------------------------
	LPDIRECT3DDEVICE8 GetD3DDevice( void ) { return m_pD3DDevice; }

protected:
	static BOOL								m_created;					//!<	Whether or not this singleton has been created
	LPDIRECT3D8								m_pD3D;							//!<	Main Direct3D interface
	LPDIRECT3DDEVICE8					m_pD3DDevice;				//!<	The rendering device
};

#endif

