/**
	* \file			xbox_FileIO.h
	* \brief		Declarations for the FileIO subsystem
	*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"
#include "fileio.h"

//= D E F I N E S ======================================================

  // NOTE: Since are set to the D:, if the program is running from
  //        the DVD drive, any file write op's will fail (eg. nvram).
  //       Therefore, in InitializeFileIO, these paths may be remapped
  //        programmatically to the Z:
#define GENERALPATH				"D:\\GENERAL"
#define ARTPATH						"D:\\ART"
#define INIPATH						"D:\\INI"
#define NVRAMPATH					"D:\\NVRAM"
#define CONFIGPATH        "D:\\CFG"
#define ROMPATH						"D:\\ROMS"
#define ROMLISTPATH				"Z:\\MAMEoX"
#define ROMLISTFILENAME		"ROMS.list"
#define INIFILENAME       "MAMEoX.ini"

  //!<  Define this to force all open() calls on the ROM directory
  //!<  to fail if they do not end in the letters "zip"
  //!<  This can drastically speed up the ROM loading process, as
  //!<  we don't waste any time searching for files that are definitely not there
#define ALL_ROMS_ZIPPED

//= P R O T O T Y P E S ================================================
	//-------------------------------------
	//	InitializeFileIO
	//! \brief		Initializes the FileIO 
	//!            subsystem
	//-------------------------------------
void InitializeFileIO( void );


	//-------------------------------------
	//	Helper_CreateDirectoryPath
	//! \brief		Create a directory path on the XBOX HDD
	//!
	//!	\param		path - The path to create (note that this is temporarily 
	//!                   modified by the function!)
	//!	\param		hasFilename - Whether or not "path" includes the name of a file
	//-------------------------------------
void Helper_CreateDirectoryPath( char *path, BOOL hasFilename );

	//-------------------------------------
	//	ComposeFilePath
	//! \brief		Returns the full file path for the given options
	//!
	//!	\param		buf - [OUT] The buffer where the file path should be stored
	//!	\param		maxLen - The maximum number of characters to store in buf
  //! \param    pathtype - The path type (see fileio.h)
  //! \param    pathindex - The path index (always set to 0)
  //! \param    filename - The filename to append to the path
  //!
  //! \return   BOOL - Operation status
  //! \retval   TRUE - Valid path returned
  //! \retval   FALSE - No path returned
	//-------------------------------------
BOOL ComposeFilePath( char *buf, UINT32 maxLen, UINT32 pathtype, UINT32 pathindex, const char *filename );

#ifdef __cplusplus
}
#endif


