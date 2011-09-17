/**
	* \file			xbox_Miscelaneous.c
	* \brief		Implementation of the "miscelaneous" section of
	*           osdepend.h
	*/

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

#include "MAMEoX.h"
#include "DebugLogger.h"

#include "mame.h"
#include "cpuexec.h"
#include "palette.h"
#include "common.h"
#include "driver.h"

#include <stdio.h>
#include <stdarg.h>


#include "virtualmemory.h"


//= D E F I N E S  =====================================================
//#define ENABLE_LOGERROR     // Enable the logerror function (this can spit out a _lot_ of data)


//= G L O B A L   V A R S  =====================================================

VIRTMEM_LOC g_virtualmem_locs[NUM_VIRTMEM] ;
HANDLE g_vmemfile ;
DWORD g_pagefileSize ;
unsigned int g_vmemThreshold = VMEM_DEFAULT_THRESHOLD;
unsigned int g_vmemCommitSize ;
unsigned int g_vmemDistribute ;
unsigned int g_forcevmem = 0 ;


//= P R O T O T Y P E S ===============================================
BOOL osd_vmm_unloadLRUpage( void );       // Defined in MAMEoXUtil.cpp
int fatalerror( const char *fmt, ... );   // Defined in xbox_Main.cpp


//= F U N C T I O N S ==================================================

typedef struct VMMINIT
{
	char name[9] ;
	DWORD threshold ;
	DWORD commit ;
	DWORD distribute ;
} vmminit ;

vmminit* loadVmSettings( unsigned int *numloaded )
{
	FILE *infile ;
	unsigned char *buf ;
	unsigned char *ptr, *end ;
	unsigned int filesize ;
	unsigned int numtoalloc ;
	unsigned int numrecs ;
	vmminit *newrecs ;
	unsigned char name[2000] ;
	unsigned char *nameptr ;
	unsigned int threshold, commit, distribute ;

	infile = fopen( "D:\\vm.txt", "rb" ) ;

	if ( numloaded )
		*numloaded = 0 ;

	if ( !infile )
	{
		fatalerror( "Could not open D:\\vm.txt !\nPlease make sure VM.TXT is in the same directory as MAMEOX.XBE" );
		return NULL ;
	}

	fseek( infile, 0, SEEK_END ) ;
	filesize = ftell( infile ) ;
	fseek( infile, 0, SEEK_SET ) ;

	if ( filesize == 0 )
	{
		fatalerror( "D:\\vm.txt is 0 bytes!\nPlease replace it with a valid version." );
		return NULL ;
	}

	buf = malloc( filesize +1 ) ;

	fread( buf, filesize, 1, infile ) ;
	buf[filesize] = 0 ;

	ptr = buf ;
	numtoalloc = 0 ;

	while ( *ptr )
	{
		numtoalloc++ ;
		ptr = strchr( ptr, '\n' ) ;

		if ( ptr == 0 )
			break ;
		else
			ptr++ ;
	}
	

	newrecs = (vmminit*)( malloc( numtoalloc * (sizeof( vmminit ) ) ) ) ;

	ptr = buf ;

	numrecs = 0 ;

	while ( ptr && *ptr )
	{
		end = strchr( ptr, '\n' ) ;

		if ( end )
			*end = 0 ;

		memset( name, 0, 2000 ) ;
		threshold = 0 ;
		commit = 0 ;
		distribute = 0 ;

		sscanf( ptr, "%s %u %u %u", name, &threshold, &commit, &distribute ) ;

		if ( end == 0 )
			ptr = 0 ;
		else 
			ptr = end+1 ;

		nameptr = name ;

		while ( *nameptr == ' ' )
			nameptr++ ;

		if ( strchr( nameptr, ' ' ) )
			*strchr( nameptr, ' ' ) = 0 ;

		if ( ( strlen(nameptr) == 0 ) || ( strlen(nameptr) > 8 ) || ( strchr( nameptr, ';' ) ) )
			continue ;

		threshold *= 0x100000 ;

		if ( threshold == 0 )
			threshold = 0x400000 ;

		commit *= 0x10000 ;

		if ( commit >= 0x400000 )
			commit = 0x200000 ;
		else
			if ( commit == 0 )
				commit = 0x80000 ;

		if ( distribute == 0 )
			distribute = 0xFFFF ;


		newrecs[numrecs].commit = commit ;
		newrecs[numrecs].distribute = distribute ;
		newrecs[numrecs].threshold = threshold ;
		strcpy( newrecs[numrecs].name, nameptr ) ;

		numrecs++ ;
	}

	if ( numloaded )
		*numloaded = numrecs ;

	return newrecs  ;
}

void InitVirtualMem( DWORD romIndex )
{
	DWORD baseptr ;
	int i ;
	char *drivername ;
	vmminit *initvars ; //[] = 
	/*
	{
		{ "alien3",   4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsom",    4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsoma",   4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsomjr1", 4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsomu",   4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsomur1", 4*1024*1024, 0x200000, 0xFFFF },
		{ "ddsomj",   4*1024*1024, 0x200000, 0xFFFF },
		{ "gunbird2", 4*1024*1024, 0x80000,  0xFFFF },
		{ "dragnblz", 4*1024*1024, 0xC0000,  0x02 },
		{ "dungeonm", 4*1024*1024, 0x200000, 0xFFFF },
		{ "elvactr",  4*1024*1024, 0x200000, 0xFFFF },
		{ "mvsc",     4*1024*1024, 0x180000, 0xFFFF },
		{ "mvsca",    4*1024*1024, 0x180000, 0xFFFF },
		{ "mvsch",    4*1024*1024, 0x180000, 0xFFFF },
		{ "mvscj",    4*1024*1024, 0x180000, 0xFFFF },
		{ "mvscjr1",  4*1024*1024, 0x180000, 0xFFFF },
		{ "sf1",      4*1024*1024, 0x1C0000, 0xFFFF },
		{ "sf1jp",    4*1024*1024, 0x1C0000, 0xFFFF },
		{ "sf1p",     4*1024*1024, 0x1C0000, 0xFFFF },
		{ "sf1us",    4*1024*1024, 0x1C0000, 0xFFFF },
		{ "sxyreact", 4*1024*1024, 0xC0000,  0xFFFF },
		{ "mslug3",   8*1024*1024, 0x100000, 0xFFFF },
		{ "mslug3n",  8*1024*1024, 0x100000, 0xFFFF },
		{ "dungeonm", 4*1024*1024, 0x200000, 0xFFFF },
		{ "kaiserkn", 4*1024*1024, 0x140000, 0xFFFF },
		{ "drifto94", 4*1024*1024, 0x140000, 0xFFFF },
		{ "guwange",  4*1024*1024, 0x180000, 0xFFFF },
		{ "lastbld2", 4*1024*1024, 0x200000, 0xFFFF },
		{ "1944",     4*1024*1024, 0x180000, 0xFFFF },
		{ "vasara",   4*1024*1024, 0x140000, 0xFFFF },
		{ "batrider", 4*1024*1024, 0x180000, 0xFFFF },
		{ "cyvern",   4*1024*1024, 0x160000, 0xFFFF },
		{ "dbz",      4*1024*1024, 0xC0000,  0xFFFF },
		{ "dbz2",     4*1024*1024, 0x100000, 0xFFFF },
		{ "esprade",  4*1024*1024, 0x200000, 0xFFFF },
		{ "mmatrix",  4*1024*1024, 0x180000, 0xFFFF },
		{ "msh",      4*1024*1024, 0x180000, 0xFFFF },
		{ "mshvsf",   4*1024*1024, 0x180000, 0xFFFF },
		{ "soldivid", 4*1024*1024, 0xA0000,  0xFFFF },
		{ "preisle2", 8*1024*1024, 0x100000, 0xFFFF },
		{ "preisl2n", 8*1024*1024, 0x100000, 0xFFFF },

	} ;
	*/

	unsigned int numinits = 0 ; //sizeof( initvars) / sizeof( vmminit ) ;

	drivername = drivers[romIndex]->name ;

	initvars = loadVmSettings( &numinits ) ;

	if ( g_forcevmem == 0 ) //If we have not forced some settings from the INI file, set defaults here
	{
		g_vmemDistribute = VMEM_DEFAULT_DISTRIBUTE ; 
		g_vmemThreshold = VMEM_DEFAULT_THRESHOLD ; 
		g_vmemCommitSize = VMEM_DEFAULT_COMMIT ;

		for ( i = 0 ; i < numinits ; i++ )
		{
			if ( strcmp( drivername, initvars[i].name ) == 0 )
			{
				g_vmemThreshold = initvars[i].threshold ;
				g_vmemCommitSize = initvars[i].commit ;
				g_vmemDistribute = initvars[i].distribute ;
				break ;
			}
		}
	}


	if ( initvars )
		free(initvars) ;

	g_vmemfile = CreateFile( "Z:\\xxpagefile.sys", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	if ( g_vmemfile == INVALID_HANDLE_VALUE )
	{
		fatalerror( "Virtual memory initialization failed! Could not create Z:\\xxpagefile.sys" );
		return ;
	}

	if ( ( g_pagefileSize = SetFilePointer( g_vmemfile, 0, 0, FILE_END ) ) == INVALID_SET_FILE_POINTER )
	{
		fatalerror( "Virtual memory initialization failed! Could not find end of file" );
		return ;
	}

	if ( g_pagefileSize < PAGEFILE_INIT_SIZE )
	{
		if ( SetFilePointer( g_vmemfile, PAGEFILE_INIT_SIZE, 0, FILE_BEGIN ) != PAGEFILE_INIT_SIZE )
		{
			fatalerror( "Virtual memory initialization failed! Could not set file pointer" );
			return ;
		}

		if ( SetEndOfFile( g_vmemfile ) == 0 )
		{
			fatalerror( "Virtual memory initialization failed! Could not set end of file" );
			return ;
		}

		g_pagefileSize = PAGEFILE_INIT_SIZE ;
	}


	baseptr = (DWORD)( VirtualAlloc( NULL, VIRTUAL_MEMORY_TOTAL_SIZE, MEM_RESERVE, PAGE_NOACCESS) ) ;      //enough for 128 segments of 4mb each

	if ( baseptr == 0 )
	{
		fatalerror( "Virtual memory initialization failed! Could not reserve %u bytes of memory.", VIRTUAL_MEMORY_TOTAL_SIZE );
		return ;
	}

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		g_virtualmem_locs[i].baseAddress = baseptr + ( VIRTUAL_MEMORY_CHUNK*i)  ;

		g_virtualmem_locs[i].fileoffset = VIRTUAL_MEMORY_CHUNK*i ;

		g_virtualmem_locs[i].commitSize = g_vmemCommitSize;

		g_virtualmem_locs[i].numpages = g_virtualmem_locs[i].commitSize / VIRTUAL_MEMORY_PAGESIZE ;

		g_virtualmem_locs[i].allocatedPages = NULL ; //(DWORD*)malloc( sizeof(DWORD) * g_virtualmem_locs[i].numpages ) ;

		g_virtualmem_locs[i].totalSize = 0 ;
		g_virtualmem_locs[i].used = 0 ;
		g_virtualmem_locs[i].head = NULL ;
		g_virtualmem_locs[i].readonly = 0 ;
		g_virtualmem_locs[i].hitnum = 0 ;
	}


}

void FreeVirtualMemory( void *ptr )
{
	int numblocks ;
	int found = 0 ;
	int i, j, block ;

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		if ( g_virtualmem_locs[i].baseAddress == (DWORD)ptr )
		{
			found = 1 ;
			numblocks = g_virtualmem_locs[i].totalSize / VIRTUAL_MEMORY_CHUNK ;

			for ( j = 0 ; j < g_virtualmem_locs[i].numpages ; j++ )
			{
				VirtualFree( (LPVOID)( g_virtualmem_locs[i].allocatedPages[j]), VIRTUAL_MEMORY_PAGESIZE, MEM_DECOMMIT );
				g_virtualmem_locs[i].allocatedPages[j] = 0 ;
			}

			free( g_virtualmem_locs[i].allocatedPages ) ;
			free( g_virtualmem_locs[i].head ) ;

			for ( block = 0 ; block < numblocks ; block++ )
			{
				g_virtualmem_locs[i+block].hitnum = 0 ;
				g_virtualmem_locs[i+block].used = 0 ;
				g_virtualmem_locs[i+block].totalSize = 0 ;
				g_virtualmem_locs[i+block].commitSize = 0 ;
				g_virtualmem_locs[i+block].head = NULL ;
				g_virtualmem_locs[i+block].allocatedPages = NULL ;

			}

			break ;
		}
	}

	if ( found == 0 )
	{
	}

}

void* AllocateVirtualMemory( unsigned int size )
{
	unsigned int numblocks ;
	int startBlock = -1 ;
	MEMORY_BASIC_INFORMATION meminfo ;
	int allfree = 0 ;
	DWORD newfilelength ;
	int i, block, j ;
	DWORD *newhead, *newpages ;
	DWORD numpages ;
    MEMORYSTATUS memStatus;

    GlobalMemoryStatus( &memStatus );


	//since there is no osd_free (why the heckfire not??) we must check to see what has been completely decommitted
	//so we can keep our structures clean

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		if (g_virtualmem_locs[i].totalSize )
		{
			allfree = 1 ;
			numblocks = g_virtualmem_locs[i].totalSize / VIRTUAL_MEMORY_CHUNK ;

			for ( block = 0 ; block < numblocks ; block++ )
			{
				VirtualQuery( (LPVOID)( g_virtualmem_locs[i+block].baseAddress ), &meminfo, VIRTUAL_MEMORY_CHUNK ) ;

				if ( ( meminfo.RegionSize != VIRTUAL_MEMORY_CHUNK ) || ( meminfo.State == MEM_COMMIT ) )
				{
					allfree = 0 ;
					break ;
				}
			}

			if ( allfree )
			{
				FreeVirtualMemory( g_virtualmem_locs[i].baseAddress ) ;
			}
			
		}
	}

	numblocks = size / VIRTUAL_MEMORY_CHUNK ;

	if ( size % VIRTUAL_MEMORY_CHUNK )
	{
		numblocks++ ;
	}

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		for ( j = 0 ; j < numblocks ; j++ )
		{
			if ( ( i+j >= NUM_VIRTMEM ) || ( g_virtualmem_locs[i+j].used ) )
			{
				break ;
			}
		}

		if ( j == numblocks )
		{
			startBlock = i ;
			break ;
		}
	}

	if ( startBlock < 0 )
	{
		//we couldn't find a long enough block of contiguous space
		return NULL;
	}

	if ( ( numblocks * g_vmemCommitSize ) > memStatus.dwAvailPhys )
	{
		return NULL ;
	}

	g_virtualmem_locs[startBlock].totalSize = numblocks * VIRTUAL_MEMORY_CHUNK ;

	newfilelength = g_virtualmem_locs[(startBlock+numblocks)-1].fileoffset + VIRTUAL_MEMORY_CHUNK ;

	if ( newfilelength > g_pagefileSize )
	{
		if ( SetFilePointer( g_vmemfile, newfilelength, 0, FILE_BEGIN ) != newfilelength )
		{
			return NULL;
		}

		if ( SetEndOfFile( g_vmemfile ) == 0 )
		{
			return NULL;
		}

		g_pagefileSize = newfilelength ;
	}


	numpages = numblocks * ( g_vmemCommitSize / VIRTUAL_MEMORY_PAGESIZE )  ;

	newhead = (DWORD*)malloc( sizeof(DWORD) ) ;
	newpages = (DWORD*)malloc( sizeof(DWORD) * numpages ) ;

	*newhead = 0 ;

	i = startBlock ;


	for ( j = 0 ; j < numpages ; j++ )
	{
		VirtualAlloc( (LPVOID)( g_virtualmem_locs[i].baseAddress + ( j*VIRTUAL_MEMORY_PAGESIZE)), VIRTUAL_MEMORY_PAGESIZE, MEM_COMMIT, PAGE_READWRITE);
		newpages[j] = (DWORD)( g_virtualmem_locs[i].baseAddress + ( j*VIRTUAL_MEMORY_PAGESIZE) ) ;
	}

	for ( i = startBlock ; i < startBlock+numblocks ; i++ )
	{
		g_virtualmem_locs[i].used = 1 ;
		g_virtualmem_locs[i].head = newhead ;
		g_virtualmem_locs[i].allocatedPages = newpages ;
		g_virtualmem_locs[i].numpages = numpages ;
		g_virtualmem_locs[i].commitSize = 0 ;

	}

	g_virtualmem_locs[startBlock].commitSize = numpages * VIRTUAL_MEMORY_PAGESIZE ;

	return (void*)(g_virtualmem_locs[startBlock].baseAddress) ;
}

void xbox_distribute_vmm() 
{
	int i,j ;
	DWORD totalWantedVMM ;
	DWORD bitfield ;
	DWORD numsections ;
	DWORD addAmount ;
	DWORD addPages ;
    MEMORYSTATUS memStatus;
	MEMORY_BASIC_INFORMATION meminfo ;
	DWORD *newpages ;
	DWORD pagesToIncrease ;
	DWORD numblocks ;
	DWORD pagecounter ;
	DWORD *pagepointer ;
	DWORD numread ;
	float pct ;

    GlobalMemoryStatus( &memStatus );

	totalWantedVMM = 0 ;
	bitfield = 1 ;
	numsections = 0 ;

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		if ( g_virtualmem_locs[i].commitSize < g_virtualmem_locs[i].totalSize )
		{
			if ( bitfield & g_vmemDistribute )
			{
				numsections++ ;
				totalWantedVMM += ( g_virtualmem_locs[i].totalSize - g_virtualmem_locs[i].commitSize ) ;
			}

			bitfield <<= 1 ;
		}
	}


	//addAmount = ( memStatus.dwAvailPhys - 0x100000 ) / numsections ;
	//addPages = addAmount / VIRTUAL_MEMORY_PAGESIZE ;
	//addAmount = addPages * VIRTUAL_MEMORY_PAGESIZE ;

	bitfield = 1 ;

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		if ( g_virtualmem_locs[i].commitSize < g_virtualmem_locs[i].totalSize )
		{
			if ( bitfield & g_vmemDistribute )
			{

				pct = (float)( g_virtualmem_locs[i].totalSize - g_virtualmem_locs[i].commitSize ) / (float)totalWantedVMM ;
				addAmount = (DWORD) ( ( (float)( memStatus.dwAvailPhys - 0x100000 )) * pct ) ;
				addPages = addAmount / VIRTUAL_MEMORY_PAGESIZE ;
				addAmount = addPages * VIRTUAL_MEMORY_PAGESIZE ;

				if ( addPages )
				{

					if ( ( g_virtualmem_locs[i].totalSize - g_virtualmem_locs[i].commitSize ) > addAmount )
						pagesToIncrease = addPages ;
					else
						pagesToIncrease = ( g_virtualmem_locs[i].totalSize - g_virtualmem_locs[i].commitSize ) / VIRTUAL_MEMORY_PAGESIZE;

					newpages = (DWORD*)realloc( g_virtualmem_locs[i].allocatedPages, sizeof(DWORD) * ( ( g_virtualmem_locs[i].commitSize/VIRTUAL_MEMORY_PAGESIZE ) + pagesToIncrease ) ) ;

					numblocks = g_virtualmem_locs[i].totalSize / VIRTUAL_MEMORY_CHUNK ;


					pagecounter = pagesToIncrease ;
					pagepointer = newpages + g_virtualmem_locs[i].numpages ;

					for ( j = 0 ; j < ( g_virtualmem_locs[i].totalSize / VIRTUAL_MEMORY_PAGESIZE ) ; j++ )
					{
						VirtualQuery( (LPVOID)( g_virtualmem_locs[i].baseAddress + ( j*VIRTUAL_MEMORY_PAGESIZE )), &meminfo, VIRTUAL_MEMORY_PAGESIZE ) ;

						if ( meminfo.State != MEM_COMMIT )
						{

							*pagepointer = g_virtualmem_locs[i].baseAddress + ( j*VIRTUAL_MEMORY_PAGESIZE ) ;
							VirtualAlloc( (LPVOID)( *pagepointer), VIRTUAL_MEMORY_PAGESIZE, MEM_COMMIT, PAGE_READWRITE);

							SetFilePointer( g_vmemfile, g_virtualmem_locs[i].fileoffset + ( j*VIRTUAL_MEMORY_PAGESIZE ), 0, FILE_BEGIN ) ;
							ReadFile( g_vmemfile, (void*)(*pagepointer), VIRTUAL_MEMORY_PAGESIZE, &numread, NULL ) ;

							pagepointer++ ;
							pagecounter-- ;

							if( pagecounter == 0 )
							{
								break ;
							}
						}

					}


					g_virtualmem_locs[i].commitSize += ( pagesToIncrease * VIRTUAL_MEMORY_PAGESIZE );

					for ( j = i ; j < i+numblocks ; j++ )
					{
						g_virtualmem_locs[j].allocatedPages = newpages ;
						g_virtualmem_locs[j].numpages += pagesToIncrease ;
					}
				}
			}

			bitfield <<= 1 ;
		}
	}

}



//---------------------------------------------------------------------
//  osd_malloc_debug
//---------------------------------------------------------------------
void *osd_malloc_debug( size_t size, const char *filename, unsigned int line, const char *function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret  ; 

  if ( size > g_vmemThreshold )
  {
	  ret = AllocateVirtualMemory( size ) ;

	  if ( ret == NULL )
	  {
		MEMORYSTATUS memStatus;
		GlobalMemoryStatus( &memStatus );
		fatalerror( "Virtual memory allocation failed! Could not allocate %u virtual memory\nPhysical memory = %u\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
		return NULL ;
	  }
	  else
	  {
		  return ret ;
	  }
  }

  ret = malloc( size );
    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = malloc( size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Malloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_debug
//---------------------------------------------------------------------
void *osd_calloc_debug( size_t num, size_t size, const char *filename, unsigned int line, const char *function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = calloc( num, size );

  if ( size > 1000000 )
  {
	  int i ;
	  i = 0 ;
  }
    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = calloc( num, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Calloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_debug
//---------------------------------------------------------------------
void *osd_realloc_debug( void *memblock, size_t size, const char *filename, unsigned int line, const char *function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

  if ( size > 1000000 )
  {
	  int i ;
	  i = 0 ;
  }

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = realloc( memblock, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Realloc failed! (Out of Memory)\nRequested %lu bytes, %lu free\nCall made at %s() - %s:%lu", size, memStatus.dwAvailPhys, function, filename, line );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_malloc_retail
//---------------------------------------------------------------------
void *osd_malloc_retail( size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret  ; 

  if ( size > g_vmemThreshold )
  {
	  ret = AllocateVirtualMemory( size ) ;

	  if ( ret == NULL )
	  {
		MEMORYSTATUS memStatus;
		GlobalMemoryStatus( &memStatus );
		fatalerror( "Virtual memory allocation failed! Could not allocate %u virtual memory\nPhysical memory = %u", size, memStatus.dwAvailPhys );
		return NULL ;
	  }
	  else
	  {
		  return ret ;
	  }
  }

  ret = malloc( size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = malloc( size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Malloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_retail
//---------------------------------------------------------------------
void *osd_calloc_retail( size_t num, size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = calloc( num, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = calloc( num, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Calloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_retail
//---------------------------------------------------------------------
void *osd_realloc_retail( void *memblock, size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

    // Attempt to release a virtual page back to the system to free up some RAM
  while( !ret && osd_vmm_unloadLRUpage()  )
    ret = realloc( memblock, size );

  if( !ret )
  {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus( &memStatus );
    fatalerror( "Realloc failed! (Out of Memory)\nRequested %lu bytes, %lu free", size, memStatus.dwAvailPhys );
  }

  return ret;
}


//---------------------------------------------------------------------
//	osd_display_loading_rom_message
//---------------------------------------------------------------------
int osd_display_loading_rom_message( const char *name, struct rom_load_data *romdata )
{
	/* called while loading ROMs. It is called a last time with name == 0 to signal */
	/* that the ROM loading process is finished. */
	/* return non-zero to abort loading */

	WCHAR title[128];
  WCHAR memory[64];
  MEMORYSTATUS memStatus;

	PRINTMSG(( T_TRACE, "osd_display_loading_rom_message" ));

	if( name )
	{
		wcscpy( title, L"Loading \"" );
		mbstowcs( &title[wcslen(title)], name, 32 );
		swprintf( &title[wcslen(title)], L"\" (%d/ %d)", romdata->romsloaded, romdata->romstotal );
	}
	else
		wcscpy( title, L"Loading complete!" );


  GlobalMemoryStatus( &memStatus );
  swprintf( memory, L"Available: %lu / Total: %lu", memStatus.dwAvailPhys, memStatus.dwTotalPhys );

	BeginFontRender( TRUE, FONTTYPE_DEFAULT );
		#ifdef _DEBUG
			FontRender( 320, 50, D3DCOLOR_XRGB( 255, 255, 255 ), L"MAMEoX version " LVERSION_STRING, 2 );
		#endif
		FontRender( 320, 200, D3DCOLOR_XRGB( 255, 255, 255 ), title, 2 );
    FontRender( 320, 383, D3DCOLOR_XRGB( 255, 255, 255 ), L"Memory Status", 2 );
    FontRender( 320, 408, D3DCOLOR_XRGB( 255, 255, 255 ), memory, 2 );
  EndFontRender( FALSE );


  #define PROGRESSBAR_CAP_COLOR     D3DCOLOR_XRGB( 255, 255, 255 )
  #define PROGRESSBAR_BAR_COLOR     D3DCOLOR_XRGB( 110, 120, 200 )
  #define PROGRESSBAR_BCK_COLOR     D3DCOLOR_XRGB( 0, 0, 0 )

  #define PROGRESSBAR_WIDTH         410
  #define PROGRESSBAR_HEIGHT        20

  if( name )
    RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                        240 - (PROGRESSBAR_HEIGHT >> 1), 
                        320 + (PROGRESSBAR_WIDTH>>1), 
                        240 + (PROGRESSBAR_HEIGHT >> 1), 
                        romdata->romsloaded, 
                        romdata->romstotal, 
                        PROGRESSBAR_BAR_COLOR, 
                       PROGRESSBAR_CAP_COLOR, 
                       PROGRESSBAR_BCK_COLOR );
  else
    RenderProgressBar( 320 - (PROGRESSBAR_WIDTH>>1),
                        240 - (PROGRESSBAR_HEIGHT >> 1), 
                        320 + (PROGRESSBAR_WIDTH>>1), 
                        240 + (PROGRESSBAR_HEIGHT >> 1), 
                        100, 
                        100, 
                        PROGRESSBAR_BAR_COLOR, 
                       PROGRESSBAR_CAP_COLOR, 
                       PROGRESSBAR_BCK_COLOR );

  PresentFrame();

	return 0;
}

//---------------------------------------------------------------------
//	osd_pause
//---------------------------------------------------------------------
void osd_pause( int paused )
{
}

//---------------------------------------------------------------------
//	logerror
//---------------------------------------------------------------------
void __cdecl logerror( const char *fmt, ... )
{
#ifdef ENABLE_LOGERROR
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

	PRINTMSG(( T_ERROR, buf ));
#endif
}

//---------------------------------------------------------------------
//	osd_print_error
//---------------------------------------------------------------------
void osd_print_error( const char *fmt, ... )
{
  wchar_t wBuf[1024];
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

  mbstowcs( wBuf, buf, 1023 );

    // Display the error to the user
	BeginFontRender( TRUE, FONTTYPE_DEFAULT );
    FontRender( 320, 50, D3DCOLOR_RGBA( 255, 255, 255, 255 ), L"Error:", 2 /*XBFONT_CENTER_X*/ );
	FontRender( 320, 75, D3DCOLOR_RGBA( 255, 255, 255, 255 ), wBuf, 2 /*XBFONT_CENTER_X*/ );
	FontRender( 320, 408, D3DCOLOR_RGBA( 255, 255, 255, 255), L"Press any button to continue.", 2 /*XBFONT_CENTER_X*/ );
	EndFontRender( TRUE );

  WaitForNoButton();     // Ensure that the user sees the message
  WaitForAnyButton();
  WaitForNoButton();     // Ensure that we don't bounce right back into the ROM that brought up the error
}

//---------------------------------------------------------------------
//	osd_autobootsavestate
//---------------------------------------------------------------------
void osd_autobootsavestate( const char *gameName )
{
    // This function is called by the MAME core as soon
    // as a ROM is up and running. It check to see if
    // an autoboot state exists for the passed gameName,
    // and schedule a load if one does exist.
  static char filename[64];
  UINT32 i = 0;
  sprintf( filename, "%s-autoboot.sta", gameName );

    // Check all of the registered STATE directories for
    // the autoboot file.
  for( ; i < osd_get_path_count( FILETYPE_STATE ); ++i )
  {
    if( osd_get_path_info( FILETYPE_STATE, i, filename ) == PATH_IS_FILE )
    {
        // Schedule a load
	    cpu_loadsave_schedule_file( LOADSAVE_LOAD, filename );
      return;
    }
  }
}

//---------------------------------------------------------------------
//	osd_die
//---------------------------------------------------------------------
void osd_die( const char *fmt, ... )
{
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

  fatalerror( buf );
}

#undef free
#include <memory.h>

void osd_free( void *ptr )
{
	int i ;

	for ( i = 0 ; i < NUM_VIRTMEM ; i++ )
	{
		if ( g_virtualmem_locs[i].baseAddress == (DWORD)ptr )
		{
			FreeVirtualMemory( ptr ) ;
			return ;
		}
	}

	free( ptr ) ;
}

//============================================================
//	osd_alloc_executable
//============================================================

void *osd_alloc_executable(size_t size)
{
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}



//============================================================
//	osd_free_executable
//============================================================

void osd_free_executable(void *ptr)
{
	VirtualFree(ptr, 0, MEM_RELEASE);
}
