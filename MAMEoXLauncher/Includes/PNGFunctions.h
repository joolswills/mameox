/**
	* \file			PNGFunctions.cpp
	* \brief		Helper functions for PNG file manipulation
	*
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "xbox_png.h"

#ifdef __cplusplus

//= S T R U C T U R E S ================================================
typedef struct PNGFile_t {
	UINT						m_imageWidth;
	UINT						m_imageHeight;
  png_structp			m_pngPtr;
	png_infop				m_infoPtr;
	png_bytep				*m_data;
} PNGFile_t;




//= P R O T O T Y P E S  ===============================================

	//------------------------------------------------------------
	//	LoadPNGFile
	//------------------------------------------------------------
BOOL LoadPNGFile(	const CStdString &filename, PNGFile_t *ret );

	//------------------------------------------------------------
	//	ClosePNGFile
	//------------------------------------------------------------
void ClosePNGFile( PNGFile_t &file );

	//------------------------------------------------------------
	//	CreateTextureFromPNGFile
	//------------------------------------------------------------
LPDIRECT3DTEXTURE8 CreateTextureFromPNGFile( PNGFile_t &colorFileData, BYTE *alphaChannel );

	//------------------------------------------------------------
	//	CreateAlphaChannelFromPNGFile
	//!
	//! \brief	Create an array of alpha bytes (0-255) from the
	//!					passed, loaded file.
	//!
	//! \param		alphaFileData - Loaded PNG file to be converted to
	//!														alpha data
	//!
	//! \param		alphaChannel - [OUT] address of a BYTE * to receive
	//!														the newly allocated alpha data.
	//!
	//! \warning	The caller is responsible for freeing the data
	//!						allocated to alphaChannel via free()!			
	//!
	//! \note			This function only uses the GREEN color channel to
	//!						determine the alpha. Red + blue data is thrown away.
	//------------------------------------------------------------
BOOL CreateAlphaChannelFromPNGFile( PNGFile_t &alphaFileData, BYTE **alphaChannel );


#endif		// __cplusplus

