/**
* \file			xbox_Network.c
* \brief		Implementation of the xbox Network interface
*           
*/

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

#include "DebugLogger.h"
#include "Undocumented.h"
#include "System_IniFile.h"

#include "xbox_Network.h"

#include <XTL.h>

//= G L O B A L = V A R S ==============================================
SNetworkConfig g_NetworkConfig; // Holds network config from ini file
CStdString g_szTitleIPAddr;    // IP Address after initialzing net

//= P R O T O T Y P E S ================================================
BOOL IsEthernetConnected();

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	InitializeNetwork
// 
// Thanks to XBoxMediaPlayer for this code!
//---------------------------------------------------------------------
BOOL InitializeNetwork( void )
{
  g_szTitleIPAddr[0] = 0;

  if (!IsEthernetConnected())
    return FALSE;

  // if local address is specified
  if ((!g_NetworkConfig.m_IPAddr.IsEmpty()) &&
      (!g_NetworkConfig.m_SubnetMask.IsEmpty()) &&
      (!g_NetworkConfig.m_Gateway.IsEmpty()))
  {
    // Thanks and credits to Team Evox for the description of the 
    // XNetConfigParams structure.

    TXNetConfigParams configParams;   

    OutputDebugString("Loading network configuration...\n");
    XNetLoadConfigParams( (LPBYTE) &configParams );
    OutputDebugString("Ready.\n");

    BOOL bXboxVersion2 = (configParams.V2_Tag == 0x58425632 );	// "XBV2"
    BOOL bDirty = FALSE;

    OutputDebugString("User local address: ");
    OutputDebugString(g_NetworkConfig.m_IPAddr);
    OutputDebugString("\n");

    if (bXboxVersion2)
    {
      if (configParams.V2_IP != inet_addr(g_NetworkConfig.m_IPAddr))
      {
        configParams.V2_IP = inet_addr(g_NetworkConfig.m_IPAddr);
        bDirty = TRUE;
      }
    }
    else
    {
      if (configParams.V1_IP != inet_addr(g_NetworkConfig.m_IPAddr))
      {
        configParams.V1_IP = inet_addr(g_NetworkConfig.m_IPAddr);
        bDirty = TRUE;
      }
    }

    OutputDebugString("User subnet mask: ");
    OutputDebugString(g_NetworkConfig.m_SubnetMask);
    OutputDebugString("\n");

    if (bXboxVersion2)
    {
      if (configParams.V2_Subnetmask != inet_addr(g_NetworkConfig.m_SubnetMask))
      {
        configParams.V2_Subnetmask = inet_addr(g_NetworkConfig.m_SubnetMask);
        bDirty = TRUE;
      }
    }
    else
    {
      if (configParams.V1_Subnetmask != inet_addr(g_NetworkConfig.m_SubnetMask))
      {
        configParams.V1_Subnetmask = inet_addr(g_NetworkConfig.m_SubnetMask);
        bDirty = TRUE;
      }
    }

    OutputDebugString("User gateway address: ");
    OutputDebugString(g_NetworkConfig.m_Gateway.c_str());
    OutputDebugString("\n");

    if (bXboxVersion2)
    {
      if (configParams.V2_Defaultgateway != inet_addr(g_NetworkConfig.m_Gateway))
      {
        configParams.V2_Defaultgateway = inet_addr(g_NetworkConfig.m_Gateway);
        bDirty = TRUE;
      }
    }
    else
    {
      if (configParams.V1_Defaultgateway != inet_addr(g_NetworkConfig.m_Gateway))
      {
        configParams.V1_Defaultgateway = inet_addr(g_NetworkConfig.m_Gateway);
        bDirty = TRUE;
      }
    }

    if (configParams.Flag != (0x04|0x08) )
    {
      configParams.Flag = 0x04 | 0x08;
      bDirty = TRUE;
    }

    if (bDirty)
    {
      OutputDebugString("Updating network configuration...\n");
      XNetSaveConfigParams( (LPBYTE) &configParams );
      OutputDebugString("Ready.\n");
    }
  }

  XNetStartupParams xnsp;
  memset(&xnsp, 0, sizeof(xnsp));
  xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);

  // Bypass security so that we may connect to 'untrusted' hosts
  xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
  // create more memory for networking
  xnsp.cfgPrivatePoolSizeInPages = 64; // == 256kb, default = 12 (48kb)
  xnsp.cfgEnetReceiveQueueLength = 16; // == 32kb, default = 8 (16kb)
  xnsp.cfgIpFragMaxSimultaneous = 16; // default = 4
  xnsp.cfgIpFragMaxPacketDiv256 = 32; // == 8kb, default = 8 (2kb)
  xnsp.cfgSockMaxSockets = 64; // default = 64
  xnsp.cfgSockDefaultRecvBufsizeInK = 128; // default = 16
  xnsp.cfgSockDefaultSendBufsizeInK = 128; // default = 16
  INT err = XNetStartup(&xnsp);

  XNADDR xna;
  DWORD dwState;
  do
  {
    dwState = XNetGetTitleXnAddr(&xna);
    Sleep(500);
  } while (dwState==XNET_GET_XNADDR_PENDING);

  // Convert the addr to a string
  XNetInAddrToString(xna.ina, g_szTitleIPAddr.GetBuffer(32), 32);

  WSADATA WsaData;
  err = WSAStartup( MAKEWORD(2,2), &WsaData );
  
  return ( err == NO_ERROR );
}

BOOL IsEthernetConnected()
{
  if (!(XNetGetEthernetLinkStatus() & XNET_ETHERNET_LINK_ACTIVE))
    return FALSE;

  return TRUE;
}