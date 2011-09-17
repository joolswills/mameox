/**
	* \file			xbox_Timing.h
	* \brief		Declarations for the Timing subsystem
	*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

//= D E F I N E S ======================================================

//= P R O T O T Y P E S ================================================
	//-------------------------------------
	//	InitializeTiming
	//! \brief		Initializes the Timing 
	//!            subsystem
	//!
	//! \return	- cycles_t : Current cycle count
	//-------------------------------------
cycles_t InitializeTiming( void );


#ifdef __cplusplus
}
#endif


