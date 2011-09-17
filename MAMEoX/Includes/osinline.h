/**
	* \file		osinline.h
	* \brief	Inline functions
	*/

#pragma once


//= I N C L U D E S =========================================================
#include "osd_cpu.h"



//= D E F I N E S ===========================================================
/*
#define osd_pend	osd_pend
#define pdo16			osd_pdo16
#define pdt16			osd_pdt16
#define pdt16np		osd_pdt16np

#ifdef __cplusplus
extern "C" {
#endif

void osd_pend(void);
void osd_pdo16( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode );
void osd_pdt16( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode );
void osd_pdt16np( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode );

#ifdef __cplusplus
}
#endif
*/

//= F U N C T I O N S =======================================================
/*
#define vec_mult		inline_vec_mult
	//-------------------------------------------------------------
	// vec_mult
	//-------------------------------------------------------------
INLINE INT32 inline_vec_mult( INT32 x, INT32 y ) {
    INT32 result;

    __asm {
        mov eax, x
        imul y
        mov result, edx
    }

    return result;
}
*/



