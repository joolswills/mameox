/**
	* \file		TextureSet.h
  *         Simple texture manager class
	*/

#pragma once

#ifdef __cplusplus

//= I N C L U D E S ===========================================================
#include "MAMEoX.h"
#include <XGraphics.h>
#include <stdio.h>

extern "C" {
#include "osd_cpu.h"
}
#include "Resource.h"


//= D E F I N E S =============================================================


  //-- Position of assets within the conglomerate "icon mask" texture ---
#define ASSET_SCROLLDOWN_LEFT         0.0f
#define ASSET_SCROLLDOWN_TOP          0.0f
#define ASSET_SCROLLDOWN_RIGHT        32.0f
#define ASSET_SCROLLDOWN_BOTTOM       30.0f

#define ASSET_TRIGGERICON_LEFT        33.0f
#define ASSET_TRIGGERICON_TOP         0.0f
#define ASSET_TRIGGERICON_RIGHT       63.0f
#define ASSET_TRIGGERICON_BOTTOM      26.0f

#define ASSET_SCROLLUP_LEFT           0.0f
#define ASSET_SCROLLUP_TOP            30.0f
#define ASSET_SCROLLUP_RIGHT          32.0f
#define ASSET_SCROLLUP_BOTTOM         61.0f

#define ASSET_TRIGGERICONTAIL_LEFT    33.0f
#define ASSET_TRIGGERICONTAIL_TOP     27.0f
#define ASSET_TRIGGERICONTAIL_RIGHT   63.0f
#define ASSET_TRIGGERICONTAIL_BOTTOM  53.0f



  //-- Position of assets within the conglomerate "menu parts" texture ---
#define ASSET_MENUITEM_LEFT_LEFT        4.0f
#define ASSET_MENUITEM_LEFT_RIGHT       22.0f
#define ASSET_MENUITEM_CENTER_LEFT      26.0f
#define ASSET_MENUITEM_CENTER_RIGHT     38.0f
#define ASSET_MENUITEM_RIGHT_LEFT       42.0f
#define ASSET_MENUITEM_RIGHT_RIGHT      60.0f

#define ASSET_MENUITEM_TITLEBAR_TOP     4.0f
#define ASSET_MENUITEM_TITLEBAR_BOTTOM  32.0f

#define ASSET_MENUITEM_BODY_TOP         36.0f
#define ASSET_MENUITEM_BODY_BOTTOM      44.0f

#define ASSET_MENUITEM_FOOTER_TOP       46.0f
#define ASSET_MENUITEM_FOOTER_BOTTOM    60.0f


  //-- Position of button icons within the buttons conglomerate
#define ASSET_BUTTON_TOPROW_TOP         2.0f
#define ASSET_BUTTON_TOPROW_BOTTOM      33.0f
#define ASSET_BUTTON_BOTTOMROW_TOP      34.0f
#define ASSET_BUTTON_BOTTOMROW_BOTTOM   65.0f

#define ASSET_BUTTON_LEFTCOLUMN_LEFT    2.0f
#define ASSET_BUTTON_LEFTCOLUMN_RIGHT   34.0f
#define ASSET_BUTTON_RIGHTCOLUMN_LEFT   35.0f
#define ASSET_BUTTON_RIGHTCOLUMN_RIGHT  67.0f

#define ASSET_BUTTONICON_START_LEFT     68.0f
#define ASSET_BUTTONICON_START_TOP      2.0f
#define ASSET_BUTTONICON_START_RIGHT    98.0f
#define ASSET_BUTTONICON_START_BOTTOM   40.0f


#define ASSET_BUTTONICON_X_LEFT         ASSET_BUTTON_RIGHTCOLUMN_LEFT
#define ASSET_BUTTONICON_X_TOP          ASSET_BUTTON_TOPROW_TOP
#define ASSET_BUTTONICON_X_RIGHT        ASSET_BUTTON_RIGHTCOLUMN_RIGHT
#define ASSET_BUTTONICON_X_BOTTOM       ASSET_BUTTON_TOPROW_BOTTOM

#define ASSET_BUTTONICON_Y_LEFT         ASSET_BUTTON_RIGHTCOLUMN_LEFT
#define ASSET_BUTTONICON_Y_TOP          ASSET_BUTTON_BOTTOMROW_TOP
#define ASSET_BUTTONICON_Y_RIGHT        ASSET_BUTTON_RIGHTCOLUMN_RIGHT
#define ASSET_BUTTONICON_Y_BOTTOM       ASSET_BUTTON_BOTTOMROW_BOTTOM

#define ASSET_BUTTONICON_A_LEFT         ASSET_BUTTON_LEFTCOLUMN_LEFT
#define ASSET_BUTTONICON_A_TOP          ASSET_BUTTON_TOPROW_TOP
#define ASSET_BUTTONICON_A_RIGHT        ASSET_BUTTON_LEFTCOLUMN_RIGHT
#define ASSET_BUTTONICON_A_BOTTOM       ASSET_BUTTON_TOPROW_BOTTOM

#define ASSET_BUTTONICON_B_LEFT         ASSET_BUTTON_LEFTCOLUMN_LEFT
#define ASSET_BUTTONICON_B_TOP          ASSET_BUTTON_BOTTOMROW_TOP
#define ASSET_BUTTONICON_B_RIGHT        ASSET_BUTTON_LEFTCOLUMN_RIGHT
#define ASSET_BUTTONICON_B_BOTTOM       ASSET_BUTTON_BOTTOMROW_BOTTOM



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

  LPDIRECT3DTEXTURE8 GetMenuParts( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_MenuParts_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetButtonIcons( void ) {
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_ButtonIcons_OFFSET];
  }

  LPDIRECT3DTEXTURE8 GetTVCalibrationCursorMask( void ) {    
    return (LPDIRECT3DTEXTURE8)&m_pResourceSysMemData[resource_TVCalibrationCursorMask_OFFSET];
  }



    //--- Asset position functions -------------------------------------

    // Scroll Icon Mask positions
  FLOAT GetScrollDownIconLeft( void ) const { return ASSET_SCROLLDOWN_LEFT; }
  FLOAT GetScrollDownIconTop( void ) const { return ASSET_SCROLLDOWN_TOP; }
  FLOAT GetScrollDownIconRight( void ) const { return ASSET_SCROLLDOWN_RIGHT; }
  FLOAT GetScrollDownIconBottom( void ) const { return ASSET_SCROLLDOWN_BOTTOM; }
  FLOAT GetScrollDownIconWidth( void ) const { return ASSET_SCROLLDOWN_RIGHT - ASSET_SCROLLDOWN_LEFT; }
  FLOAT GetScrollDownIconHeight( void ) const { return ASSET_SCROLLDOWN_BOTTOM - ASSET_SCROLLDOWN_TOP; }

  FLOAT GetTriggerIconLeft( void ) const { return ASSET_TRIGGERICON_LEFT; }
  FLOAT GetTriggerIconTop( void ) const { return ASSET_TRIGGERICON_TOP; }
  FLOAT GetTriggerIconRight( void ) const { return ASSET_TRIGGERICON_RIGHT; }
  FLOAT GetTriggerIconBottom( void ) const { return ASSET_TRIGGERICON_BOTTOM; }
  FLOAT GetTriggerIconWidth( void ) const { return ASSET_TRIGGERICON_RIGHT - ASSET_TRIGGERICON_LEFT; }
  FLOAT GetTriggerIconHeight( void ) const { return ASSET_TRIGGERICON_BOTTOM - ASSET_TRIGGERICON_TOP; }

  FLOAT GetScrollUpIconLeft( void ) const { return ASSET_SCROLLUP_LEFT; }
  FLOAT GetScrollUpIconTop( void ) const { return ASSET_SCROLLUP_TOP; }
  FLOAT GetScrollUpIconRight( void ) const { return ASSET_SCROLLUP_RIGHT; }
  FLOAT GetScrollUpIconBottom( void ) const { return ASSET_SCROLLUP_BOTTOM; }
  FLOAT GetScrollUpIconWidth( void ) const { return ASSET_SCROLLUP_RIGHT - ASSET_SCROLLUP_LEFT; }
  FLOAT GetScrollUpIconHeight( void ) const { return ASSET_SCROLLUP_BOTTOM - ASSET_SCROLLUP_TOP; }

  FLOAT GetTriggerIconTailLeft( void ) const { return ASSET_TRIGGERICONTAIL_LEFT; }
  FLOAT GetTriggerIconTailTop( void ) const { return ASSET_TRIGGERICONTAIL_TOP; }
  FLOAT GetTriggerIconTailRight( void ) const { return ASSET_TRIGGERICONTAIL_RIGHT; }
  FLOAT GetTriggerIconTailBottom( void ) const { return ASSET_TRIGGERICONTAIL_BOTTOM; }
  FLOAT GetTriggerIconTailWidth( void ) const { return ASSET_TRIGGERICONTAIL_RIGHT - ASSET_TRIGGERICONTAIL_LEFT; }
  FLOAT GetTriggerIconTailHeight( void ) const { return ASSET_TRIGGERICONTAIL_BOTTOM - ASSET_TRIGGERICONTAIL_TOP; }

    // MenuParts positions
  FLOAT GetMenuTitleBarLeftLeft( void )   const { return ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuTitleBarLeftRight( void )  const { return ASSET_MENUITEM_LEFT_RIGHT; }
  FLOAT GetMenuTitleBarLeftTop( void )    const { return ASSET_MENUITEM_TITLEBAR_TOP; }
  FLOAT GetMenuTitleBarLeftBottom( void ) const { return ASSET_MENUITEM_TITLEBAR_BOTTOM; }
  FLOAT GetMenuTitleBarLeftWidth( void )  const { return ASSET_MENUITEM_LEFT_RIGHT - ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuTitleBarHeight( void )     const { return ASSET_MENUITEM_TITLEBAR_BOTTOM - ASSET_MENUITEM_TITLEBAR_TOP; }

  FLOAT GetMenuTitleBarCenterLeft( void )   const { return ASSET_MENUITEM_CENTER_LEFT; }
  FLOAT GetMenuTitleBarCenterRight( void )  const { return ASSET_MENUITEM_CENTER_RIGHT; }
  FLOAT GetMenuTitleBarCenterTop( void )    const { return ASSET_MENUITEM_TITLEBAR_TOP; }
  FLOAT GetMenuTitleBarCenterBottom( void ) const { return ASSET_MENUITEM_TITLEBAR_BOTTOM; }
  FLOAT GetMenuTitleBarCenterWidth( void )  const { return ASSET_MENUITEM_CENTER_RIGHT - ASSET_MENUITEM_CENTER_LEFT; }

  FLOAT GetMenuTitleBarRightLeft( void )   const { return ASSET_MENUITEM_RIGHT_LEFT; }
  FLOAT GetMenuTitleBarRightRight( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT; }
  FLOAT GetMenuTitleBarRightTop( void )    const { return ASSET_MENUITEM_TITLEBAR_TOP; }
  FLOAT GetMenuTitleBarRightBottom( void ) const { return ASSET_MENUITEM_TITLEBAR_BOTTOM; }
  FLOAT GetMenuTitleBarRightWidth( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT - ASSET_MENUITEM_RIGHT_LEFT; }

    // menu body sections
  FLOAT GetMenuBodyLeftLeft( void )   const { return ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuBodyLeftRight( void )  const { return ASSET_MENUITEM_LEFT_RIGHT; }
  FLOAT GetMenuBodyLeftTop( void )    const { return ASSET_MENUITEM_BODY_TOP; }
  FLOAT GetMenuBodyLeftBottom( void ) const { return ASSET_MENUITEM_BODY_BOTTOM; }
  FLOAT GetMenuBodyLeftWidth( void )  const { return ASSET_MENUITEM_LEFT_RIGHT - ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuBodyHeight( void )     const { return ASSET_MENUITEM_BODY_BOTTOM - ASSET_MENUITEM_BODY_TOP; }

  FLOAT GetMenuBodyCenterLeft( void )   const { return ASSET_MENUITEM_CENTER_LEFT; }
  FLOAT GetMenuBodyCenterRight( void )  const { return ASSET_MENUITEM_CENTER_RIGHT; }
  FLOAT GetMenuBodyCenterTop( void )    const { return ASSET_MENUITEM_BODY_TOP; }
  FLOAT GetMenuBodyCenterBottom( void ) const { return ASSET_MENUITEM_BODY_BOTTOM; }
  FLOAT GetMenuBodyCenterWidth( void )  const { return ASSET_MENUITEM_CENTER_RIGHT - ASSET_MENUITEM_CENTER_LEFT; }

  FLOAT GetMenuBodyRightLeft( void )   const { return ASSET_MENUITEM_RIGHT_LEFT; }
  FLOAT GetMenuBodyRightRight( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT; }
  FLOAT GetMenuBodyRightTop( void )    const { return ASSET_MENUITEM_BODY_TOP; }
  FLOAT GetMenuBodyRightBottom( void ) const { return ASSET_MENUITEM_BODY_BOTTOM; }
  FLOAT GetMenuBodyRightWidth( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT - ASSET_MENUITEM_RIGHT_LEFT; }

    // menu footer sections
  FLOAT GetMenuFooterLeftLeft( void )   const { return ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuFooterLeftRight( void )  const { return ASSET_MENUITEM_LEFT_RIGHT; }
  FLOAT GetMenuFooterLeftTop( void )    const { return ASSET_MENUITEM_FOOTER_TOP; }
  FLOAT GetMenuFooterLeftBottom( void ) const { return ASSET_MENUITEM_FOOTER_BOTTOM; }
  FLOAT GetMenuFooterLeftWidth( void )  const { return ASSET_MENUITEM_LEFT_RIGHT - ASSET_MENUITEM_LEFT_LEFT; }
  FLOAT GetMenuFooterHeight( void )     const { return ASSET_MENUITEM_FOOTER_BOTTOM - ASSET_MENUITEM_FOOTER_TOP; }

  FLOAT GetMenuFooterCenterLeft( void )   const { return ASSET_MENUITEM_CENTER_LEFT; }
  FLOAT GetMenuFooterCenterRight( void )  const { return ASSET_MENUITEM_CENTER_RIGHT; }
  FLOAT GetMenuFooterCenterTop( void )    const { return ASSET_MENUITEM_FOOTER_TOP; }
  FLOAT GetMenuFooterCenterBottom( void ) const { return ASSET_MENUITEM_FOOTER_BOTTOM; }
  FLOAT GetMenuFooterCenterWidth( void )  const { return ASSET_MENUITEM_CENTER_RIGHT - ASSET_MENUITEM_CENTER_LEFT; }

  FLOAT GetMenuFooterRightLeft( void )   const { return ASSET_MENUITEM_RIGHT_LEFT; }
  FLOAT GetMenuFooterRightRight( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT; }
  FLOAT GetMenuFooterRightTop( void )    const { return ASSET_MENUITEM_FOOTER_TOP; }
  FLOAT GetMenuFooterRightBottom( void ) const { return ASSET_MENUITEM_FOOTER_BOTTOM; }
  FLOAT GetMenuFooterRightWidth( void )  const { return ASSET_MENUITEM_RIGHT_RIGHT - ASSET_MENUITEM_RIGHT_LEFT; }

  FLOAT GetXButtonLeft( void ) const    { return ASSET_BUTTONICON_X_LEFT; }
  FLOAT GetXButtonTop( void ) const     { return ASSET_BUTTONICON_X_TOP; }
  FLOAT GetXButtonRight( void ) const   { return ASSET_BUTTONICON_X_RIGHT; }
  FLOAT GetXButtonBottom( void ) const  { return ASSET_BUTTONICON_X_BOTTOM; }

  FLOAT GetYButtonLeft( void ) const    { return ASSET_BUTTONICON_Y_LEFT; }
  FLOAT GetYButtonTop( void ) const     { return ASSET_BUTTONICON_Y_TOP; }
  FLOAT GetYButtonRight( void ) const   { return ASSET_BUTTONICON_Y_RIGHT; }
  FLOAT GetYButtonBottom( void ) const  { return ASSET_BUTTONICON_Y_BOTTOM; }

  FLOAT GetAButtonLeft( void ) const    { return ASSET_BUTTONICON_A_LEFT; }
  FLOAT GetAButtonTop( void ) const     { return ASSET_BUTTONICON_A_TOP; }
  FLOAT GetAButtonRight( void ) const   { return ASSET_BUTTONICON_A_RIGHT; }
  FLOAT GetAButtonBottom( void ) const  { return ASSET_BUTTONICON_A_BOTTOM; }

  FLOAT GetBButtonLeft( void ) const    { return ASSET_BUTTONICON_B_LEFT; }
  FLOAT GetBButtonTop( void ) const     { return ASSET_BUTTONICON_B_TOP; }
  FLOAT GetBButtonRight( void ) const   { return ASSET_BUTTONICON_B_RIGHT; }
  FLOAT GetBButtonBottom( void ) const  { return ASSET_BUTTONICON_B_BOTTOM; }

  FLOAT GetButtonWidth( void ) const    { return ASSET_BUTTONICON_B_RIGHT - ASSET_BUTTONICON_B_LEFT; }
  FLOAT GetButtonHeight( void ) const   { return ASSET_BUTTONICON_B_BOTTOM - ASSET_BUTTONICON_B_TOP; }

  FLOAT GetSTARTButtonLeft( void ) const    { return ASSET_BUTTONICON_START_LEFT; }
  FLOAT GetSTARTButtonTop( void ) const     { return ASSET_BUTTONICON_START_TOP; }
  FLOAT GetSTARTButtonRight( void ) const   { return ASSET_BUTTONICON_START_RIGHT; }
  FLOAT GetSTARTButtonBottom( void ) const  { return ASSET_BUTTONICON_START_BOTTOM; }
  FLOAT GetSTARTButtonWidth( void ) const    { return ASSET_BUTTONICON_START_RIGHT - ASSET_BUTTONICON_START_LEFT; }
  FLOAT GetSTARTButtonHeight( void ) const   { return ASSET_BUTTONICON_START_BOTTOM - ASSET_BUTTONICON_START_TOP; }


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
      PRINTMSG(( T_INFO, "ERROR: Invalid Xbox Packed Resource (.xpr) file" ));
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


