/**
  * \file     VirtualMemoryManager.cpp
  * \brief    Generic virtual memory manager class
  *
  */

#pragma once

//= I N C L U D E S ===========================================================
#include "MAMEoX.h"
#include "osd_cpu.h"

#include <vector>
#include <list>

//= D E F I N E S =============================================================


//= S T R U C T U R E S =======================================================
  //! \struct   vmmpage_t
  //! \brief    
typedef struct vmmpage_t
{
  UINT32      m_pageFaultCount;   //!<  The number of times this page has been accessed without being loaded

  void        *m_address;         //!<  The virtual address of this page
  UINT32      m_size;             //!<  The size of this page
  BOOL        m_committed;        //!<  Whether or not this page is in physical memory
} vmmpage_t;


BOOL operator<( const vmmpage_t &a, const vmmpage_t &b );
BOOL operator==( const vmmpage_t &a, const vmmpage_t &b );

//= C L A S S E S =============================================================
  //! \class    CVirtualMemoryManager
  //! \brief    Generic virtual memory manager class
class CVirtualMemoryManager
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
  CVirtualMemoryManager( void );

		//------------------------------------------------------
		//	Malloc
    //! \brief    Allocates a block of virtual RAM
    //!
    //! \param    size - The size of the block to allocate
    //!
    //! \return   void * - Address at which the ram can be
    //!                    accessed.
    //!
    //! \warning  You MUST call AccessAddressRange before
    //!           using the returned pointer!
		//------------------------------------------------------
  void *Malloc( UINT32 size );

		//------------------------------------------------------
		//	Free
    //! \brief    Frees a block of allocated virtual RAM
    //!
    //! \param    ptr - A void * previously obtained via
    //!                 a CVirtualMemoryManager::Malloc
    //!                 call
		//------------------------------------------------------
  void Free( void *ptr );

		//------------------------------------------------------
		//	AccessAddressRange
    //! \brief    Prepare access to a virtual memory range
    //!
    //! \param    ptr - A void * previously obtained via
    //!                 a CVirtualMemoryManager::Malloc
    //!                 call
    //! \param    size - Currently ignored
    //!
    //! \return   BOOL - Whether or not the access has
    //!                  been granted. A return value
    //!                  of FALSE indicates a fatal error.
		//------------------------------------------------------
  BOOL AccessAddressRange( void *ptr, UINT32 size );

		//------------------------------------------------------
		//	UnloadLRUPage
    //! \brief    Unload the least recently used page from
    //!           RAM in order to free some memory back to
    //!           the system
    //!
    //! \return   BOOL - Whether or not a RAM block was
    //!                   released.
		//------------------------------------------------------
  BOOL UnloadLRUPage( void );

  BOOL MakePageReadOnly( void *ptr );
  BOOL MakePageReadWrite( void *ptr );

  #ifdef _DEBUG
  void PrintInfo( void );
  #endif

  UINT32 GetNumVirtualPages( void ) const { return m_virtualPages.size(); }
  UINT32 GetNumCommittedPages( void ) const { return m_committedAddresses.size(); }

protected:
  std::vector<vmmpage_t>      m_virtualPages;   //!<  The vmmpage_t structs describing virtual memory

    //!  The address param from vmmpage_t structs describing the committed virtual pages
    //!   This is sorted such that the LRU page is at the head of the list
  std::list<UINT32>           m_committedAddresses; 
};

