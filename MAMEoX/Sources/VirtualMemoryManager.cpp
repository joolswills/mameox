/**
  * \file     VirtualMemoryManager.cpp
  * \brief    Generic virtual memory manager class
  *
  */


//= I N C L U D E S ===========================================================
#include "VirtualMemoryManager.h"
#include <algorithm>

//= D E F I N E S =============================================================
#define FINDPAGE( _addr__ )          (void*)(((UINT32)(_addr__)) - (((UINT32)(_addr__)) & 4096))


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
//  Create
//------------------------------------------------------
void *CVirtualMemoryManager::Malloc( UINT32 size )
{
  vmmpage_t page;
  memset( &page, 0, sizeof(page) );

  page.m_address = VirtualAlloc(  NULL,                       // Region to reserve or commit (NULL = system defined)
                                  size,                       // Size of the region
                                  MEM_RESERVE | MEM_TOP_DOWN, // Flags (just reserve the address space, do it from the highest possible addr down)
                                  PAGE_READWRITE );           // Access protection (rw)
  page.m_size = size;

  m_virtualPages.push_back( page );
  std::sort( m_virtualPages.begin(), m_virtualPages.end() );

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
    std::list<vmmpage_t*>::iterator j;
    j = std::find( m_committedPages.begin(), m_committedPages.end(), &virtualPage );
    if( j != m_committedPages.end() )
      m_committedPages.erase( j );
  }

  if( !VirtualFree( virtualPage.m_address, 0, MEM_RELEASE ) )
    fatalerror( "Failed to free virtual page with address\n0x%X", virtualPage.m_address );

  m_virtualPages.erase( i );

}

//------------------------------------------------------
//  AccessAddressRange
//------------------------------------------------------
BOOL CVirtualMemoryManager::AccessAddressRange( void *base, UINT32 size )
{
  if( !base )
    return FALSE;

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
    std::list<vmmpage_t*>::iterator j;
    j = std::find( m_committedPages.begin(), m_committedPages.end(), &virtualPage );
    if( j != m_committedPages.end() )
    {
        // The page is already committed, move it to the head of the list to tag
        // this access.
      m_committedPages.erase( j );
      m_committedPages.push_front( &virtualPage );
      return FALSE;
    }

      // The page has not been committed, increment the fault counter, release a
      // committed page (if necessary), and commit this one
    if( virtualPage.m_pageFaultCount < 0xFFFFFFFF )
      ++virtualPage.m_pageFaultCount;

      // Try to commit the page, releasing a committed page everytime we fail
    void *ptr = NULL;
    while( !(ptr = VirtualAlloc(  virtualPage.m_address,                     // region to reserve or commit
                                  virtualPage.m_size,                        // size of region
                                  MEM_COMMIT | MEM_TOP_DOWN | MEM_NOZERO,    // type of allocation
                                  PAGE_READWRITE )) && 
           m_committedPages.size() )
    {
        // The commit failed, so unload the least recently used page
      if( !m_committedPages.size() )
        fatalerror( "Out of Memory: VirtualAlloc failed and\nthere are no valid pages to unload!" );

      j = m_committedPages.end();
      --j;
  
      if( !VirtualFree( (*j)->m_address, (*j)->m_size, MEM_DECOMMIT ) )
        fatalerror( "Failed to free virtual page with address\n0x%X", (*j)->m_address );

      (*j)->m_committed = FALSE;
      m_committedPages.pop_back();
    }

    if( !ptr )
    {
      PRINTMSG( T_INFO, 
                "Out of Memory: Failed to load virtual page w/ address 0x%X, size 0x%X", 
                virtualPage.m_address,
                virtualPage.m_size );
      return FALSE;
    }

      // The page has been successfully committed, add it to the head of the
      // committedPages list
    m_committedPages.push_front( &virtualPage );
  }

  return TRUE;
}












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
