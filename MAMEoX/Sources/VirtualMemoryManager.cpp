/**
  * \file     VirtualMemoryManager.cpp
  * \brief    Generic virtual memory manager class
  *
  */


//= I N C L U D E S ===========================================================
#include "VirtualMemoryManager.h"
#include <algorithm>

#include "DebugLogger.h"

//= D E F I N E S =============================================================
#define FINDPAGE( _addr__ )          (void*)(((UINT32)(_addr__)) - (((UINT32)(_addr__)) & 4096))

  // Define to print log info on every VMM call
#define LOGVMM

//= P R O T O T Y P E S =======================================================
extern "C" int fatalerror( const char *fmt, ... );

//= G L O B A L = V A R S =====================================================



//= F U N C T I O N S =========================================================

//------------------------------------------------------
//  Constructor
//------------------------------------------------------
CVirtualMemoryManager::CVirtualMemoryManager( void )
{
}

//------------------------------------------------------
//  Malloc
//------------------------------------------------------
void *CVirtualMemoryManager::Malloc( UINT32 size )
{
  vmmpage_t page;
  memset( &page, 0, sizeof(page) );

  page.m_address = VirtualAlloc(  NULL,                       // Region to reserve or commit (NULL = system defined)
                                  size,                       // Size of the region
                                  MEM_RESERVE,                // Flags (just reserve the address space)
                                  PAGE_READWRITE );           // Access protection (rw)
  page.m_size = size;

  m_virtualPages.push_back( page );
  std::sort( m_virtualPages.begin(), m_virtualPages.end() );

  #if (defined LOGVMM && defined _DEBUG)
  PRINTMSG( T_INFO, "Malloc: %lu bytes: 0x%X", size, page.m_address );
  #endif

  if( !page.m_address )
    PRINTMSG( T_ERROR, "VirtualAlloc failed: 0x%X", GetLastError() );

  return page.m_address;
}

//------------------------------------------------------
//  Free
//------------------------------------------------------
void CVirtualMemoryManager::Free( void *base )
{
  if( !base )
    return;

  vmmpage_t page;
  page.m_address = FINDPAGE( base );

  std::vector<vmmpage_t>::iterator i;
  i = std::find( m_virtualPages.begin(), m_virtualPages.end(), page );

  if( i == m_virtualPages.end() )
  {
    PRINTMSG( T_ERROR, "Attempt to Free nonvirtual address 0x%X!", base );
    return;
  }

    // See if the page is committed and remove it from the committed list
  vmmpage_t &virtualPage = (*i);

  if( virtualPage.m_committed )
  {
    std::list<UINT32>::iterator j = std::find( m_committedAddresses.begin(), m_committedAddresses.end(), (UINT32)(virtualPage.m_address) );
    if( j != m_committedAddresses.end() )
      m_committedAddresses.erase( j );
  }

  if( !VirtualFree( virtualPage.m_address, 0, MEM_RELEASE ) )
    fatalerror( "Failed to free virtual page with address\n0x%X", virtualPage.m_address );

  m_virtualPages.erase( i );

  #if (defined LOGVMM && defined _DEBUG)
  PRINTMSG( T_INFO, "Free: 0x%X", base );
  #endif
}

//------------------------------------------------------
//  AccessAddressRange
//------------------------------------------------------
BOOL CVirtualMemoryManager::AccessAddressRange( void *base, UINT32 size )
{
  if( !base )
  {
    PRINTMSG( T_ERROR, "AccessAddressRange called on NULL base!" );
    return FALSE;
  }

  vmmpage_t page;
  page.m_address = FINDPAGE( base );

  std::vector<vmmpage_t>::iterator i;
  i = std::find( m_virtualPages.begin(), m_virtualPages.end(), page );

  if( i == m_virtualPages.end() )
  {
    PRINTMSG( T_ERROR, "AccessAddressRange called on invalid address 0x%X!", base );
    return FALSE;
  }


    // See if the page is already committed
  vmmpage_t &virtualPage = (*i);
  if( !virtualPage.m_committed )
  {
    std::list<UINT32>::iterator j = std::find(  m_committedAddresses.begin(), 
                                                m_committedAddresses.end(), 
                                                (UINT32)(virtualPage.m_address) );
    if( j != m_committedAddresses.end() )
    {
        // Something has gone terribly wrong, as this page is marked not committed, yet
        // is in the list...
      m_committedAddresses.erase( j );
      PRINTMSG( T_ERROR, "Page in committed list marked as decommitted! 0x%X", virtualPage.m_address );
    }

      // The page has not been committed, increment the fault counter, release a
      // committed page (if necessary), and commit this one
    if( virtualPage.m_pageFaultCount < 0xFFFFFFFF )
      ++virtualPage.m_pageFaultCount;

      // Try to commit the page, releasing a committed page everytime we fail
    void *ptr = NULL;
    while( !(ptr = VirtualAlloc(  virtualPage.m_address,       // region to reserve or commit
                                  virtualPage.m_size,          // size of region
                                  MEM_COMMIT | MEM_NOZERO,     // type of allocation
                                  PAGE_READWRITE )) && 
           m_committedAddresses.size() )
    {
      if( !UnloadLRUPage() )
        fatalerror( "Out of Memory: VirtualAlloc failed and\nthere are no valid pages to unload!" );
    }

    if( !ptr )
    {
      PRINTMSG( T_INFO, 
                "Out of Memory: Failed to load virtual page w/ address 0x%X, size 0x%X", 
                virtualPage.m_address,
                virtualPage.m_size );
      return FALSE;
    }

      // The page has been successfully committed, add it to the tail (MRU) of the
      // committedPages list
    m_committedAddresses.push_back( (UINT32)virtualPage.m_address );
    virtualPage.m_committed = TRUE;
  }
  else
  {
      // The item is already committed, just move it to the tail of the list
    std::list<UINT32>::iterator j = std::find(  m_committedAddresses.begin(), 
                                                m_committedAddresses.end(), 
                                                (UINT32)(virtualPage.m_address) );
    if( j == m_committedAddresses.end() )
      PRINTMSG( T_ERROR, "Page in marked as committed but not in committed list! 0x%X", virtualPage.m_address );
    else
      m_committedAddresses.erase( j );

    m_committedAddresses.push_back( (UINT32)(virtualPage.m_address) );
  }

  #if (defined LOGVMM && defined _DEBUG)
  PRINTMSG( T_INFO, "AccessAddressRange: 0x%X, num page faults: %lu", virtualPage.m_address, virtualPage.m_pageFaultCount );
  DEBUGGERCHECKRAM();
  #endif


  return TRUE;
}


//------------------------------------------------------
//  UnloadLRUPage
//------------------------------------------------------
BOOL CVirtualMemoryManager::MakePageReadOnly( void *base )
{
  if( !base )
  {
    PRINTMSG( T_ERROR, "MakePageReadOnly called on NULL base!" );
    return FALSE;
  }

  vmmpage_t page;
  page.m_address = FINDPAGE( base );

  std::vector<vmmpage_t>::iterator i = std::find( m_virtualPages.begin(), 
                                                  m_virtualPages.end(), 
                                                  page );
  if( i == m_virtualPages.end() )
  {
    PRINTMSG( T_ERROR, "MakePageReadOnly called on invalid address 0x%X!", base );
    return FALSE;
  }


    // See if the page is already committed
  vmmpage_t &virtualPage = (*i);

  DWORD oldProtection;
  return VirtualProtect(  virtualPage.m_address,
                          virtualPage.m_size,
                          PAGE_READONLY,
                          &oldProtection );
}

//------------------------------------------------------
//  UnloadLRUPage
//------------------------------------------------------
BOOL CVirtualMemoryManager::MakePageReadWrite( void *base )
{
  if( !base )
  {
    PRINTMSG( T_ERROR, "MakePageReadWrite called on NULL base!" );
    return FALSE;
  }

  vmmpage_t page;
  page.m_address = FINDPAGE( base );

  std::vector<vmmpage_t>::iterator i = std::find( m_virtualPages.begin(), 
                                                  m_virtualPages.end(), 
                                                  page );
  if( i == m_virtualPages.end() )
  {
    PRINTMSG( T_ERROR, "MakePageReadWrite called on invalid address 0x%X!", base );
    return FALSE;
  }


    // See if the page is already committed
  vmmpage_t &virtualPage = (*i);

  DWORD oldProtection;
  return VirtualProtect(  virtualPage.m_address,
                          virtualPage.m_size,
                          PAGE_READWRITE,
                          &oldProtection );
}

//------------------------------------------------------
//  UnloadLRUPage
//------------------------------------------------------
BOOL CVirtualMemoryManager::UnloadLRUPage( void )
{
  #if (defined LOGVMM && defined _DEBUG)
  PRINTMSG( T_INFO, "UnloadLRUPage %lu pages allocated", m_committedAddresses.size() );
  DEBUGGERCHECKRAM();
  #endif

    // The commit failed, so unload the least recently used page
  if( !m_committedAddresses.size() )
  {
    PRINTMSG( T_INFO, "UnloadLRUPage called but no pages allocated" );
    return FALSE;
  }

  std::list<UINT32>::iterator j = m_committedAddresses.begin();

  vmmpage_t page;
  page.m_address = (void*)(*j);
  std::vector<vmmpage_t>::iterator it = std::find( m_virtualPages.begin(), m_virtualPages.end(), page );
  if( it == m_virtualPages.end() )
  {
    PRINTMSG( T_ERROR, "Failed to lookup vmmpage_t for committed address 0x%X!", *j );
    m_committedAddresses.pop_back();
    return FALSE;
  }

  if( !VirtualFree( (*it).m_address, (*it).m_size, MEM_DECOMMIT ) )
  {
    PRINTMSG( T_ERROR, "Failed to free virtual page with address\n0x%X", (*it).m_address );
    return FALSE;
  }

  (*it).m_committed = FALSE;
  m_committedAddresses.pop_front();

  #if (defined LOGVMM && defined _DEBUG)
  PRINTMSG( T_INFO, "UnloadLRUPage %lu pages allocated after free", m_committedAddresses.size() );
  DEBUGGERCHECKRAM();
  #endif

  return TRUE;
}


//------------------------------------------------------
//  PrintInfo
//------------------------------------------------------
#ifdef _DEBUG
void CVirtualMemoryManager::PrintInfo( void )
{
  DEBUGGERCHECKRAM();
  PRINTMSG( T_INFO, "Total pages: %lu Committed pages: %lu", m_virtualPages.size(), m_committedAddresses.size() );

  std::vector<vmmpage_t>::iterator i = m_virtualPages.begin();
  for( ; i != m_virtualPages.end(); ++i )
  {
    PRINTMSG( T_INFO, 
              "Addr: 0x%X  Size: %lu   Committed: %d   Faults: %lu",
              (*i).m_address,
              (*i).m_size,
              (*i).m_committed,
              (*i).m_pageFaultCount );
  }
}
#endif






//------------------------------------------------------
//  operator< (const vmmpage_t &, const vmmpage_t &)
//------------------------------------------------------
BOOL operator<( const vmmpage_t &a, const vmmpage_t &b )
{
  return a.m_address < b.m_address;
}

//------------------------------------------------------
//  operator== (const vmmpage_t &, const vmmpage_t &)
//------------------------------------------------------
BOOL operator==( const vmmpage_t &a, const vmmpage_t &b )
{
  return a.m_address == b.m_address;
}
