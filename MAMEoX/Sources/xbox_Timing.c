/**
	* \file			xbox_Timing.c
	* \brief		Implementation of the "Timing" section of osdepend.h
	*/
//= I N C L U D E S ====================================================
#include <Xtl.h>

#include "osd_cpu.h"
#include "osdepend.h"

#include "DebugLogger.h"


//= G L O B A L = V A R S =============================================
cycles_t				g_cyclesPerSecond = 0;


//= F U N C T I O N S =================================================

//-------------------------------------------------------------
//	init_cycle_counter
//-------------------------------------------------------------
cycles_t InitializeTiming( void )
{
	LARGE_INTEGER frequency;

	if( QueryPerformanceFrequency( &frequency ) )
		g_cyclesPerSecond = frequency.QuadPart;
	else
		PRINTMSG( T_ERROR, "QueryPerformanceFrequency failed!" );

	// log the results
	PRINTMSG( T_INFO, "cycles/second = %lu\n", (DWORD)g_cyclesPerSecond);

	// return the current cycle count
	return osd_cycles();
}


//-------------------------------------------------------------
//	osd_cycles
//-------------------------------------------------------------
cycles_t osd_cycles( void )
{
	LARGE_INTEGER performance_count;
	QueryPerformanceCounter( &performance_count );

	return (cycles_t)performance_count.QuadPart;
}	


//-------------------------------------------------------------
//	osd_cycles_per_second
//-------------------------------------------------------------
cycles_t osd_cycles_per_second( void )
{
	return g_cyclesPerSecond;
}

//-------------------------------------------------------------
//	osd_profiling_ticks
//-------------------------------------------------------------
cycles_t osd_profiling_ticks( void )
{
	volatile cycles_t result;
	volatile cycles_t *presult = &result;

	__asm {
		rdtsc
		mov ebx, presult
		mov [ebx],eax
		mov [ebx+4],edx
	}

	return result;
}



