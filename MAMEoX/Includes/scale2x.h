/**
* \file			scale2x.h
* \brief		Declarations for the scale2x algorithms
*/

#pragma once

//= D E F I N E S ======================================================
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned u32;

//= P R O T O T Y P E S ================================================
void AdMame2x(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */, u8 *dstPtr, u32 dstPitch, int width, int height);
void AdMame2x32(u8 *srcPtr, u32 srcPitch, u8 * /* deltaPtr */,u8 *dstPtr, u32 dstPitch, int width, int height);
