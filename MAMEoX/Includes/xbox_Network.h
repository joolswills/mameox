/**
* \file			xbox_Network.h
* \brief		Declarations for the Network subsystem
*/

#pragma once

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"
#include "StdString.h"

//= S T R U C T U R E S ===============================================
struct SNetworkConfig
{
  CStdString m_IPAddr;
  CStdString m_SubnetMask;
  CStdString m_Gateway;
  CStdString m_NameServer;
};

//= D E F I N E S ======================================================
#define DEFAULT_IPADDR     ""
#define DEFAULT_SUBNETMASK ""
#define DEFAULT_GATEWAY    ""
#define DEFAULT_NAMESERVER ""

//= G L O B A L = V A R S ==============================================
extern CStdString g_szTitleIPAddr;
extern SNetworkConfig g_NetworkConfig;

//= P R O T O T Y P E S ================================================
//--------------------------------------------------------------------------
//	InitializeNetwork
//! \brief		Initializes the Network subsystem
//--------------------------------------------------------------------------
BOOL InitializeNetwork( void );