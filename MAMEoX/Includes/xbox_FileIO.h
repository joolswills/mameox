/**
	* \file			xbox_FileIO.h
	* \brief		Declarations for the FileIO subsystem
	*/

#pragma once

//= I N C L U D E S ====================================================

#include "MAMEoX.h"
#include "osdepend.h"
#include "osd_cpu.h"
#include "fileio.h"
#include "StdString.h"

//= D E F I N E S ======================================================

  // NOTE: Since are set to the D:, if the program is running from
  //        the DVD drive, any file write op's will fail (eg. nvram).
  //       Therefore, in InitializeFileIO(), these paths may be remapped
  //        programmatically to the ALTDRIVE drive

  //! Alternate drive letter if the D: is not writable
#define DEFAULT_ALTDRIVE            "T"

#define DEFAULT_GENERALPATH				  "D:\\GENERAL"
#define DEFAULT_ARTPATH						  "D:\\ARTWORK"
#define DEFAULT_INIPATH						  "U:\\INI"
#define DEFAULT_NVRAMPATH					  "D:\\NVRAM"
#define DEFAULT_CONFIGPATH          "D:\\CFG"
#define DEFAULT_ROMPATH						  "D:\\ROMS"
#define DEFAULT_AUDIOPATH           "D:\\SAMPLES"
#define DEFAULT_DEFAULTROMLISTPATH  "T:\\ROMS"
#define DEFAULT_ROMBACKUPPATH       "D:\\ROMS\\BACKUP"
#define DEFAULT_HDIMAGEPATH         "D:\\HDIMAGES"
#define DEFAULT_HISCOREPATH         "D:\\HISCORES"
#define DEFAULT_SCREENSHOTPATH      "D:\\SCREENSHOTS"
  

#define DEFAULT_CMAPPING            "\\Device\\Harddisk0\\Partition1"
#define DEFAULT_EMAPPING            "\\Device\\Harddisk0\\Partition1"
#define DEFAULT_FMAPPING            "\\Device\\Harddisk0\\Partition6"
#define DEFAULT_GMAPPING            "\\Device\\Cdrom0"
#define DEFAULT_HMAPPING            "\\Device\\Harddisk0\\Partition6"


#define DRIVERLISTFILENAME  "DRIVERS.list"
#define ROMLISTFILENAME		  "ROMS.list"
#define INIFILENAME         "MAMEoX.ini"
#define ROMSTATUSFILENAME   "ROMStatus.xml"

  //!<  Define this to force all open() calls on the ROM directory
  //!<  to fail if they do not end in the letters "zip"
  //!<  This can drastically speed up the ROM loading process, as
  //!<  we don't waste any time searching for files that are definitely not there
#define ALL_ROMS_ZIPPED

//= S T R U C T U R E S ===============================================
struct FileIOConfig_t
{
  CStdString m_ALTDrive;
  CStdString m_GeneralPath;
  CStdString m_ArtPath;
  CStdString m_IniPath;
  CStdString m_NVramPath;
  CStdString m_ConfigPath;
  CStdString m_RomPath0;
  CStdString m_RomPath1;
  CStdString m_RomPath2;
  CStdString m_RomPath3;
  CStdString m_AudioPath;
  CStdString m_RomBackupPath;
  CStdString m_HDImagePath;
  CStdString m_HiScorePath;
  CStdString m_DefaultRomListPath;
  CStdString m_ScreenshotPath;

  CStdString m_LetterCMapping;
  CStdString m_LetterEMapping;
  CStdString m_LetterFMapping;
  CStdString m_LetterGMapping;
  CStdString m_LetterHMapping;

  BOOL       m_recursiveSearching;    //!< Whether or not to recursively search ROM/other paths

  FileIOConfig_t( void ) {
    // Default the paths
    m_ALTDrive            = DEFAULT_ALTDRIVE;
    m_GeneralPath         = DEFAULT_GENERALPATH;
    m_ArtPath             = DEFAULT_ARTPATH;
    m_IniPath             = DEFAULT_INIPATH;
    m_NVramPath           = DEFAULT_NVRAMPATH;
    m_ConfigPath          = DEFAULT_CONFIGPATH;
    m_RomPath0            = DEFAULT_ROMPATH;
    m_RomPath1            = DEFAULT_ROMPATH;
    m_RomPath2            = DEFAULT_ROMPATH;
    m_RomPath3            = DEFAULT_ROMPATH;
    m_AudioPath           = DEFAULT_AUDIOPATH;
    m_DefaultRomListPath  = DEFAULT_DEFAULTROMLISTPATH;
    m_RomBackupPath       = DEFAULT_ROMBACKUPPATH;
    m_HDImagePath         = DEFAULT_HDIMAGEPATH;
    m_HiScorePath         = DEFAULT_HISCOREPATH;
    m_ScreenshotPath      = DEFAULT_SCREENSHOTPATH;

    m_LetterCMapping      = DEFAULT_CMAPPING;
    m_LetterEMapping      = DEFAULT_EMAPPING;
    m_LetterFMapping      = DEFAULT_FMAPPING;
    m_LetterGMapping      = DEFAULT_GMAPPING;
    m_LetterHMapping      = DEFAULT_HMAPPING;
  }

  void MakeLower( void ) {
    m_ALTDrive.MakeLower();
    m_GeneralPath.MakeLower();
    m_ArtPath.MakeLower();
    m_IniPath.MakeLower();
    m_NVramPath.MakeLower();
    m_ConfigPath.MakeLower();
    m_RomPath0.MakeLower();
    m_RomPath1.MakeLower();
    m_RomPath2.MakeLower();
    m_RomPath3.MakeLower();
    m_AudioPath.MakeLower();
    m_DefaultRomListPath.MakeLower();
    m_RomBackupPath.MakeLower();
    m_HDImagePath.MakeLower();
    m_HiScorePath.MakeLower();
    m_ScreenshotPath.MakeLower();

    m_LetterCMapping.MakeLower();
    m_LetterEMapping.MakeLower();
    m_LetterFMapping.MakeLower();
    m_LetterGMapping.MakeLower();
    m_LetterHMapping.MakeLower();
  }
};

//= G L O B A L = V A R S ==============================================
extern const char       *g_ROMListPath;
extern const char       *g_ROMBackupPath;
extern FileIOConfig_t   g_FileIOConfig;

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


