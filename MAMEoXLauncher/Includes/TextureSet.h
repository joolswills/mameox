/**
	* \file		TextureSet.h
  *         Simple texture manager class
	*/

#pragma once

#ifdef __cplusplus

//= I N C L U D E S ===========================================================
#include <Xtl.h>
#include <XGraphics.h>
#include <stdio.h>

#include "MAMEoX.h"
#include "osd_cpu.h"
#include "Resource.h"


//= D E F I N E S =============================================================


  //-- Position of assets within the conglomerate "icon mask" texture ---
#define ASSET_SCROLLDOWN_LEFT         0.0f
#define ASSET_SCROLLDOWN_TOP          0.0f
#define ASSET_SCROLLDOWN_RIGHT        31.0f
#define ASSET_SCROLLDOWN_BOTTOM       29.0f

#define ASSET_TRIGGERICON_LEFT        33.0f
#define ASSET_TRIGGERICON_TOP         0.0f
#define ASSET_TRIGGERICON_RIGHT       63.0f
#define ASSET_TRIGGERICON_BOTTOM      26.0f

#define ASSET_SCROLLUP_LEFT           0.0f
#define ASSET_SCROLLUP_TOP            31.0f
#define ASSET_SCROLLUP_RIGHT          31.0f
#define ASSET_SCROLLUP_BOTTOM         60.0f

#define ASSET_TRIGGERICONTAIL_LEFT    33.0f
#define ASSET_TRIGGERICONTAIL_TOP     28.0f
#define ASSET_TRIGGERICONTAIL_RIGHT   60.0f
#define ASSET_TRIGGERICONTAIL_BOTTOM  54.0f


//= C L A S S E S =============================================================

  //! \class    CTextureSet
  //! \brief    Texture manager class
class CTextureSet
{
public:

    //-------------------------------------------------------------------
	  //	Create
	  //! \brief		Creates the font objects
    //! \return   BOOL - pass or failure
    //-------------------------------------------------------------------
  BOOL Create( void ) {
    return LoadPackedResources();
  }

    //--- Texture data functions --------------------------------------------
  LPDIRECT3DTEXTURE8 GetROMListScreenBackdrop( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_ROMListScreenBackdrop_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetBasicBackdrop( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_BasicBackdrop_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetSplashScreenBackdrop( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_SplashScreenBackdrop_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetMessageScreenBackdrop( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_MessageScreenBackdrop_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetLightgunCursorMask( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_LightgunCursorMask_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetScrollIconMasks( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_ScrollIconMasks_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetStartMenu( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_StartMenu_OFFSET];
  }

    //--- Asset position functions -------------------------------------
  FLOAT GetScrollDownIconLeft( void ) const { return ASSET_SCROLLDOWN_LEFT; }
  FLOAT GetScrollDownIconTop( void ) const { return ASSET_SCROLLDOWN_TOP; }
  FLOAT GetScrollDownIconRight( void ) const { return ASSET_SCROLLDOWN_RIGHT; }
  FLOAT GetScrollDownIconBottom( void ) const { return ASSET_SCROLLDOWN_BOTTOM; }

  FLOAT GetTriggerIconLeft( void ) const { return ASSET_TRIGGERICON_LEFT; }
  FLOAT GetTriggerIconTop( void ) const { return ASSET_TRIGGERICON_TOP; }
  FLOAT GetTriggerIconRight( void ) const { return ASSET_TRIGGERICON_RIGHT; }
  FLOAT GetTriggerIconBottom( void ) const { return ASSET_TRIGGERICON_BOTTOM; }

  FLOAT GetScrollUpIconLeft( void ) const { return ASSET_SCROLLUP_LEFT; }
  FLOAT GetScrollUpIconTop( void ) const { return ASSET_SCROLLUP_TOP; }
  FLOAT GetScrollUpIconRight( void ) const { return ASSET_SCROLLUP_RIGHT; }
  FLOAT GetScrollUpIconBottom( void ) const { return ASSET_SCROLLUP_BOTTOM; }

  FLOAT GetTriggerIconTailLeft( void ) const { return ASSET_TRIGGERICONTAIL_LEFT; }
  FLOAT GetTriggerIconTailTop( void ) const { return ASSET_TRIGGERICONTAIL_TOP; }
  FLOAT GetTriggerIconTailRight( void ) const { return ASSET_TRIGGERICONTAIL_RIGHT; }
  FLOAT GetTriggerIconTailBottom( void ) const { return ASSET_TRIGGERICONTAIL_BOTTOM; }



protected:

    //-------------------------------------------------------------------
    // LoadPackedResources
    //-------------------------------------------------------------------
  BOOL LoadPackedResources( void ) {
    if( m_initialized )
      return TRUE;

    FILE *file = fopen( "D:\\Media\\Resource.xpr", "rb" );
    if( !file )
      return FALSE;

      // Read in and verify the XPR magic header
    XPR_HEADER xprh;
    if( fread( &xprh, sizeof(XPR_HEADER), 1, file ) != 1 )
    {
      fclose( file );
      return FALSE;
    }

    if( xprh.dwMagic != XPR_MAGIC_VALUE )
    {
      PRINTMSG( T_INFO, "ERROR: Invalid Xbox Packed Resource (.xpr) file" );
      fclose( file );
      return E_INVALIDARG;
    }

      // Compute memory requirements
    DWORD dwSysMemDataSize = xprh.dwHeaderSize - sizeof(XPR_HEADER);
    DWORD dwVidMemDataSize = xprh.dwTotalSize - xprh.dwHeaderSize;

      // Allocate memory
    m_pResourceSysMemData = new BYTE[dwSysMemDataSize];
    m_pResourceVidMemData = (BYTE*)D3D_AllocContiguousMemory( dwVidMemDataSize, D3DTEXTURE_ALIGNMENT );

      // Read in the data from the file
    if( fread( m_pResourceSysMemData, dwSysMemDataSize, 1, file ) != 1 ||
        fread( m_pResourceVidMemData, dwVidMemDataSize, 1, file ) != 1 )
    {
      delete[] m_pResourceSysMemData;
      D3D_FreeContiguousMemory( m_pResourceVidMemData );
      fclose( file );
      return FALSE;
    }

    fclose( file );
    
      // Loop over resources, calling Register()
    BYTE *pData = m_pResourceSysMemData;

    for( DWORD i = 0; i < resource_NUM_RESOURCES; ++i )
    {
      LPDIRECT3DRESOURCE8 pResource = (LPDIRECT3DRESOURCE8)pData;
      pResource->Register( m_pResourceVidMemData );

      switch( pResource->GetType() )
      {
        case D3DRTYPE_TEXTURE:       
          pData += sizeof(D3DTexture);       
          break;

        case D3DRTYPE_VOLUMETEXTURE: 
          pData += sizeof(D3DVolumeTexture); 
          break;

        case D3DRTYPE_CUBETEXTURE:   
          pData += sizeof(D3DCubeTexture);   
          break;

        case D3DRTYPE_VERTEXBUFFER:  
          pData += sizeof(D3DVertexBuffer);  
          break;

        case D3DRTYPE_INDEXBUFFER:   
          pData += sizeof(D3DIndexBuffer);   
          break;

        case D3DRTYPE_PALETTE:       
          pData += sizeof(D3DPalette);       
          break;

        default:
          return FALSE;
      }
    }

    m_initialized = TRUE;
    return TRUE;
  }

  BOOL  m_initialized;
  BYTE  *m_pResourceSysMemData;
  BYTE  *m_pResourceVidMemData;
};

#endif    // ifdef __cplusplus


