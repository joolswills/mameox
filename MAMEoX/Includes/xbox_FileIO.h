/**
	* \file			xbox_FileIO.h
	* \brief		Declarations for the FileIO subsystem
	*/

#pragma once

//= I N C L U D E S ====================================================

#include "osdepend.h"
#include "osd_cpu.h"
#include "fileio.h"
#include "StdString.h"
#include <xtl.h>

//= D E F I N E S ======================================================

  // NOTE: Since are set to the D:, if the program is running from
  //        the DVD drive, any file write op's will fail (eg. nvram).
  //       Therefore, in InitializeFileIO(), these paths may be remapped
  //        programmatically to the ALTDRIVE drive

  //! Alternate drive letter if the D: is not writable
#define DEFAULT_ALTDRIVE            "T"

#define DEFAULT_GENERALPATH				  "D:\\GENERAL"
#define DEFAULT_ARTPATH						  "D:\\ART"
#define DEFAULT_INIPATH						  "U:\\INI"
#define DEFAULT_NVRAMPATH					  "D:\\NVRAM"
#define DEFAULT_CONFIGPATH          "D:\\CFG"
#define DEFAULT_ROMPATH						  "D:\\ROMS"
#define DEFAULT_AUDIOPATH           "D:\\SAMPLES"
#define DEFAULT_DEFAULTROMLISTPATH  "D:\\ROMS"
#define DEFAULT_ROMBACKUPPATH       "D:\\ROMS\\BACKUP"
#define DEFAULT_HDIMAGEPATH         "D:\\HDIMAGES"
#define DEFAULT_HISCOREPATH         "D:\\HISCORES"
  
#define DRIVERLISTFILENAME  "DRIVERS.list"
#define ROMLISTFILENAME		  "ROMS.list"
#define INIFILENAME         "MAMEoX.ini"

  //!<  Define this to force all open() calls on the ROM directory
  //!<  to fail if they do not end in the letters "zip"
  //!<  This can drastically speed up the ROM loading process, as
  //!<  we don't waste any time searching for files that are definitely not there
#define ALL_ROMS_ZIPPED

//= S T R U C T U R E S ===============================================
struct SFileIOConfig
{
  CStdString m_ALTDrive;
  CStdString m_GeneralPath;
  CStdString m_ArtPath;
  CStdString m_IniPath;
  CStdString m_NVramPath;
  CStdString m_ConfigPath;
  CStdString m_RomPath;
  CStdString m_AudioPath;
  CStdString m_DefaultRomListPath;
  CStdString m_RomBackupPath;
  CStdString m_HDImagePath;
  CStdString m_HiScorePath;

  SFileIOConfig()
  {
    // Default the paths
    m_ALTDrive           = DEFAULT_ALTDRIVE;
    m_GeneralPath        = DEFAULT_GENERALPATH;
    m_ArtPath            = DEFAULT_ARTPATH;
    m_IniPath            = DEFAULT_INIPATH;
    m_NVramPath          = DEFAULT_NVRAMPATH;
    m_ConfigPath         = DEFAULT_CONFIGPATH;
    m_RomPath            = DEFAULT_ROMPATH;
    m_AudioPath          = DEFAULT_AUDIOPATH;
    m_DefaultRomListPath = DEFAULT_DEFAULTROMLISTPATH;
    m_RomBackupPath      = DEFAULT_ROMBACKUPPATH;
    m_HDImagePath        = DEFAULT_HDIMAGEPATH;
    m_HiScorePath        = DEFAULT_HISCOREPATH;
  }
};

//= G L O B A L = V A R S ==============================================
extern const char *g_ROMListPath;
extern const char *g_ROMBackupPath;
extern SFileIOConfig g_FileIOConfig;

//= P R O T O T Y P E S ================================================
	//--------------------------------------------------------------------------
	//	InitializeFileIO
	//! \brief		Initializes the FileIO 
	//!            subsystem
	//--------------------------------------------------------------------------
void InitializeFileIO( void );


	//--------------------------------------------------------------------------
	//	Helper_CreateDirectoryPath
	//! \brief		Create a directory path on the XBOX HDD
	//!
	//!	\param		path - The path to create (note that this is temporarily 
	//!                   modified by the function, but set back to the original
  //!                   value on exit.)
	//!	\param		hasFilename - Whether or not "path" includes the name of a file
	//--------------------------------------------------------------------------
void Helper_CreateDirectoryPath( const char *path, BOOL hasFilename );

	//--------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
BOOL ComposeFilePath( char *buf, UINT32 maxLen, UINT32 pathtype, UINT32 pathindex, const char *filename );


