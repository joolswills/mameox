#define PAGEFILE_INIT_SIZE 0x18000000 //384MB
#define VIRTUAL_MEMORY_PAGE_BITS 16 
#define VIRTUAL_MEMORY_PAGE_PER_CHUNK_BITS 6 
#define VIRTUAL_MEMORY_PAGESIZE ( 1 << VIRTUAL_MEMORY_PAGE_BITS ) //64KB
#define VIRTUAL_MEMORY_PAGES_PER_CHUNK ( 1<<VIRTUAL_MEMORY_PAGE_PER_CHUNK_BITS ) //64 This value must be ( 1 << ? ) for use in masking
#define VIRTUAL_MEMORY_PAGE_MASK (VIRTUAL_MEMORY_PAGES_PER_CHUNK-1)
#define VIRTUAL_MEMORY_CHUNK (VIRTUAL_MEMORY_PAGESIZE * VIRTUAL_MEMORY_PAGES_PER_CHUNK ) //4MB
#define VIRTUAL_MEMORY_COMMIT_SIZE 0x80000 //512kb
#define VIRTUAL_MEMORY_TOTAL_SIZE  0x20000000 //512MB
#define NUM_VIRTMEM ( VIRTUAL_MEMORY_TOTAL_SIZE/VIRTUAL_MEMORY_CHUNK )

#define VMEM_DEFAULT_THRESHOLD  0x7800000 //120mb
#define VMEM_DEFAULT_COMMIT     0x400000   //4MB
#define VMEM_DEFAULT_DISTRIBUTE 0xFFFF    //distribute evenly among all blocks

typedef struct {
	DWORD baseAddress ;
	DWORD totalSize ;
	DWORD commitSize ;
	DWORD *allocatedPages ;
	DWORD numpages ;
	DWORD *head ;
	DWORD fileoffset ;
	unsigned char readonly ;
	unsigned char used ;
	DWORD hitnum ;
} VIRTMEM_LOC ;


extern VIRTMEM_LOC g_virtualmem_locs[NUM_VIRTMEM] ;
extern HANDLE g_vmemfile ;
extern unsigned int g_pagefileSize ;
extern unsigned int g_vmemThreshold ;
extern unsigned int g_vmemCommitSize ;
extern unsigned int g_vmemDistribute ;
extern unsigned int g_forcevmem  ;
