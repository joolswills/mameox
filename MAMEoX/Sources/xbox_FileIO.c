/**
	* \file			xbox_FileIO.c
	* \brief		Implementation of the "File I/O" section of
	*           osdepend.h
	*/

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"

#include "DebugLogger.h"

#include "fileio.h"
#include "xbox_FileIO.h"

#include <crtdbg.h>
#include <stdio.h>

//= D E F I N E S ======================================================

  //!<  Helper macro, calls Helper_CreateOrOpenSystemPath on the passed path
#define CREATEOROPENPATH( _pth__ )  tempStr = strdup( _pth__ ); \
                                    if( !Helper_CreateOrOpenSystemPath( &tempStr ) ) { \
                                      PRINTMSG( T_ERROR, "Failed to create or open system path %s!", tempStr ); \
                                      _RPT1( _CRT_ERROR, "Could not create or open system path %s!\n", tempStr ); \
                                    }


//= S T R U C T U R E S ================================================

  // Since the XBOX doesn't do any disk IO buffering, we can dramatically
  //  improve performance by doing a 1 time load of the file and keeping
  //  it in memory until unloaded
typedef struct _osd_file {
  HANDLE      m_fileHandle;     //!<  The XBOX file handle associated w/ the file
  UINT8       *m_data;          //!<  The buffered file data
  UINT8       *m_filePointer;   //!<  The current read/write head position
  UINT64      m_fileSize;       //!<  The total file size
  BOOL        m_dirtyFlag;      //!<  Whether or not the file should be flushed to disk on close
} _osd_file;




//= G L O B A L = V A R S ==============================================

const char *g_pathNames[FILETYPE_end] = {NULL};


//= P R O T O T Y P E S ================================================
  //! Create one of the system directories (GENERALPATH, etc...) or
  //!  open it if it already exists. Also, if the path cannot be created
  //!  on the D:, do so on the Z: instead
static BOOL Helper_CreateOrOpenSystemPath( char **path );

void Helper_CreateDirectoryPath( char *path, BOOL has_filename );
static void Helper_ConvertSlashToBackslash( char *path );
static BOOL Helper_PreloadFile( _osd_file *dest, HANDLE openedFile );


//= F U N C T I O N S ==================================================


//---------------------------------------------------------------------
//	InitializeFileIO
//---------------------------------------------------------------------
void InitializeFileIO( void )
{
  char *tempStr = NULL;
	PRINTMSG( T_TRACE, "InitializeFileIO" );


    // Try to create/open all of the paths. If any fail,
    //     switch the drive letter to 'Z', as we must be
    //     running off of a DVD


    // Make sure the rom list path is available
  CREATEOROPENPATH( ROMLISTPATH );
  free( tempStr );

  CREATEOROPENPATH( ROMPATH );
	g_pathNames[FILETYPE_RAW] = g_pathNames[FILETYPE_ROM] = 
															g_pathNames[FILETYPE_ROM_NOCRC] = 
															tempStr;

  CREATEOROPENPATH( INIPATH );
	g_pathNames[FILETYPE_INI] = tempStr;

  CREATEOROPENPATH( NVRAMPATH );
	g_pathNames[FILETYPE_HIGHSCORE] = g_pathNames[FILETYPE_HIGHSCORE_DB] = 
																		g_pathNames[FILETYPE_NVRAM] = 
																		g_pathNames[FILETYPE_STATE] =
																		g_pathNames[FILETYPE_MEMCARD] = 
																		tempStr;

  CREATEOROPENPATH( ARTPATH );
	g_pathNames[FILETYPE_IMAGE] = g_pathNames[FILETYPE_IMAGE_DIFF] =
																g_pathNames[FILETYPE_ARTWORK] = 
																tempStr;

  CREATEOROPENPATH( AUDIOPATH );
  g_pathNames[FILETYPE_SAMPLE] = tempStr;

  CREATEOROPENPATH( CONFIGPATH );
  g_pathNames[FILETYPE_CONFIG] = tempStr;

  CREATEOROPENPATH( GENERALPATH );
	g_pathNames[FILETYPE_INPUTLOG] =  g_pathNames[FILETYPE_SCREENSHOT] = 
																	  g_pathNames[FILETYPE_HISTORY] = 
																	  g_pathNames[FILETYPE_CHEAT] = 
																	  g_pathNames[FILETYPE_LANGUAGE] = 
																	  g_pathNames[FILETYPE_CTRLR] = 
                                    tempStr;
}

//---------------------------------------------------------------------
//	osd_get_path_count
//---------------------------------------------------------------------
int osd_get_path_count( int pathtype )
{
	/* Return the number of paths for a given type */
	PRINTMSG( T_TRACE, "osd_get_path_count: pathtype: %d", pathtype );
//EZ_TRACE();

	switch( pathtype )
	{
	case FILETYPE_RAW:
	case FILETYPE_ROM:
	case FILETYPE_ROM_NOCRC:

	case FILETYPE_IMAGE:
	case FILETYPE_IMAGE_DIFF:
	case FILETYPE_SAMPLE:
	case FILETYPE_ARTWORK:

	case FILETYPE_NVRAM:
	case FILETYPE_HIGHSCORE:
	case FILETYPE_HIGHSCORE_DB:
	case FILETYPE_CONFIG:
	case FILETYPE_INPUTLOG:
	case FILETYPE_STATE:
	case FILETYPE_MEMCARD:
	case FILETYPE_SCREENSHOT:
	case FILETYPE_HISTORY:
	case FILETYPE_CHEAT:
	case FILETYPE_LANGUAGE:
	case FILETYPE_CTRLR:
	case FILETYPE_INI:
		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------
//	osd_get_path_info
//---------------------------------------------------------------------
int osd_get_path_info( int pathtype, int pathindex, const char *filename )
{
	/* Get information on the existence of a file */
	DWORD attribs;
	char *fullPath;
	
	PRINTMSG( T_TRACE, "osd_get_path_info" );

  #ifdef ALL_ROMS_ZIPPED
      // Don't bother searching for anything in the ROMs directory
      // if it doesn't end with .zip
    if( pathtype == FILETYPE_ROM || pathtype == FILETYPE_ROM_NOCRC )
    {
      if( stricmp( &filename[strlen(filename)-4], ".zip" ) )
        return PATH_NOT_FOUND;
    }
  #endif

	fullPath = (char*)malloc( strlen(g_pathNames[pathtype]) + strlen(filename) + 2 );
	if( !fullPath )
	{
		PRINTMSG( T_ERROR, "Malloc failed!" );
		return PATH_NOT_FOUND;
	}

	sprintf( fullPath, "%s\\%s", g_pathNames[pathtype], filename );
	Helper_ConvertSlashToBackslash( fullPath );

	attribs = GetFileAttributes( fullPath );
	free( fullPath );

	if( attribs < 0 )
		return PATH_NOT_FOUND;
	else if( attribs & FILE_ATTRIBUTE_DIRECTORY )
		return PATH_IS_DIRECTORY;

	return PATH_IS_FILE;
}

//---------------------------------------------------------------------
//	osd_fopen
//---------------------------------------------------------------------
osd_file *osd_fopen( int pathtype, int pathindex, const char *filename, const char *mode )
{
	/* 
			Attempt to open a file with the given name and 
			mode using the specified path type 
	*/

	DWORD dwCreationDisposition = 0;
	DWORD dwDesiredAccess = 0;	
	osd_file *ret;

	PRINTMSG( T_TRACE, "osd_fopen" );

  


	ret = (osd_file*)malloc( sizeof(osd_file) );
	if( !ret )
	{
		PRINTMSG( T_ERROR, "Malloc failed creating osd_file instance!" );
		return NULL;
	}


	if( strchr( mode, 'r' ) )
	{
		dwCreationDisposition = OPEN_EXISTING;
		dwDesiredAccess = GENERIC_READ;
	}
	if( strchr( mode, 'w' ) )
	{
		dwCreationDisposition = CREATE_ALWAYS;
		dwDesiredAccess = GENERIC_WRITE;
	}
	if (strchr(mode, '+'))
	{
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
	}

	
	{
    HANDLE h;
		char *fullPath = (char*)malloc( strlen(g_pathNames[pathtype]) + strlen(filename) + 2 );
		sprintf( fullPath, "%s\\%s", g_pathNames[pathtype], filename );
		Helper_ConvertSlashToBackslash( fullPath );


		h = CreateFile(	fullPath,
										dwDesiredAccess,
										0,
										NULL,
										dwCreationDisposition,
										FILE_ATTRIBUTE_NORMAL,
										NULL );

		if( h == INVALID_HANDLE_VALUE )
		{
				// See why it failed //
			DWORD err = GetLastError();

				// Return if read only or some other error
			if( !(dwDesiredAccess & GENERIC_WRITE) || err != ERROR_PATH_NOT_FOUND )
			{
				PRINTMSG( T_ERROR, "Failed opening file %s: 0x%X!", fullPath, err );
        _RPT2( _CRT_WARN, "Failed opening file %s: 0x%X!\n", fullPath, err );
				free( fullPath );
        free( ret );
				return NULL;
			}

				// create the directory path and try again
			Helper_CreateDirectoryPath( fullPath, TRUE );
			h = CreateFile(	fullPath,
											dwDesiredAccess,
											0,
											NULL,
											dwCreationDisposition,
											FILE_ATTRIBUTE_NORMAL,
											NULL );

				// if that doesn't work, we give up
			if( h == INVALID_HANDLE_VALUE )
			{
				PRINTMSG( T_ERROR, "Failed opening file %s: 0x%X!", fullPath, err );
        _RPT2( _CRT_WARN, "Failed opening file %s: 0x%X!\n", fullPath, err );
				free( fullPath );
        free( ret );
				return NULL;
			}
		}
		free( fullPath );

      // Load the file into RAM
    if( !Helper_PreloadFile( ret, h ) )
    {
      free( ret );
      CloseHandle( h );
      return NULL;
    }
	}

	return ret;
}


//---------------------------------------------------------------------
//	osd_fseek
//---------------------------------------------------------------------
INT32 osd_fseek( osd_file *file, INT64 offset, int whence )
{
	  // Seek within a file
  if( !file )
    return 1;

  switch( whence )
  {
    // *** SEEK_SET *** //
  case SEEK_SET:
      // Nothing to do
    break;

    // *** SEEK_CUR *** //
  case SEEK_CUR:
    offset += ((UINT64)file->m_filePointer - (UINT64)file->m_data);
    break;

    // *** SEEK_END *** //
  case SEEK_END:
    offset += file->m_fileSize;
    break;

  default:
    _RPT0( _CRT_WARN, "Invalid whence parameter in osd_fseek\n" );
    PRINTMSG( T_ERROR, "Invalid whence parameter in osd_fseek" );
    return 1;
  }

    // Validate arg
  if( offset > file->m_fileSize || offset < 0)
  {
    _RPT0( _CRT_WARN, "Offset value too high or low in osd_fseek\n" );
    PRINTMSG( T_ERROR, "Offset value too high or low in osd_fseek" );
    return 1;
  }

  file->m_filePointer = &file->m_data[offset];
	return 0;
}

//---------------------------------------------------------------------
//	osd_ftell
//---------------------------------------------------------------------
UINT64 osd_ftell( osd_file *file )
{
	  /* Return current file position */
  return ( (UINT64)file->m_filePointer - (UINT64)file->m_data );
}

//---------------------------------------------------------------------
//	osd_feof
//---------------------------------------------------------------------
int osd_feof( osd_file *file )
{
	  /* Return 1 if we're at the end of file */
  return ( (UINT64)file->m_filePointer - (UINT64)file->m_data >= file->m_fileSize );
}

//---------------------------------------------------------------------
//	osd_fread
//---------------------------------------------------------------------
UINT32 osd_fread( osd_file *file, void *buffer, UINT32 length )
{
	  /* Read bytes from a file */
  if( !file || !file->m_data || file->m_fileSize < length )
    return 0;

    // Reduce the length param if the file is not large enough
  if( ((UINT64)file->m_filePointer - (UINT64)file->m_data) + length > file->m_fileSize )
    length = file->m_fileSize - ((UINT64)file->m_filePointer - (UINT64)file->m_data);

  memcpy( buffer, file->m_filePointer, length );
  file->m_filePointer += length;

  return length;
}

//---------------------------------------------------------------------
//	osd_fwrite
//---------------------------------------------------------------------
UINT32 osd_fwrite( osd_file *file, const void *buffer, UINT32 length )
{
  UINT64 filePointerOffset;

	  /* Write bytes to a file */
  if( !file )
    return 0;

    // Create the file data if it's not already created
  if( !file->m_data )
  {
    file->m_fileSize = length;

    if( !(file->m_data = (UINT8*)malloc( file->m_fileSize )) )
    {
      PRINTMSG( T_ERROR, "Failed to malloc %lu bytes for new file!", length );
      _RPT1( _CRT_WARN, "Failed to malloc %lu bytes for new file!", length );
      return 0;
    }

    file->m_filePointer = file->m_data;
  }

  filePointerOffset = ((UINT64)file->m_filePointer - (UINT64)file->m_data);

    // Grow the file if the current size is not enough
  if( filePointerOffset + length > file->m_fileSize )
  {
    file->m_fileSize = ((UINT64)file->m_filePointer - (UINT64)file->m_data) + length;

    if( !(file->m_data = realloc( file->m_data, file->m_fileSize )) )
    {
      PRINTMSG( T_ERROR, "Failed to realloc %lu bytes for new file!", length );
      _RPT1( _CRT_WARN, "Failed to realloc %lu bytes for new file!", length );
      return 0;
    }
    file->m_filePointer = file->m_data + filePointerOffset;   // Put the file pointer back into position
  }

  memcpy( file->m_filePointer, buffer, length );
  file->m_filePointer += length;
  file->m_dirtyFlag = TRUE;

  return length;
}

//---------------------------------------------------------------------
//	osd_fclose
//---------------------------------------------------------------------
void osd_fclose( osd_file *file )
{
	PRINTMSG( T_TRACE, "osd_fclose" );

		/* Close an open file */
  if( file->m_dirtyFlag )
  {
    DWORD bytesWritten;
    LARGE_INTEGER pos;
    pos.QuadPart = 0;

      // Write back data to the drive
    if( !SetFilePointerEx( file->m_fileHandle, pos, NULL, FILE_BEGIN ) )
    {
      _RPT0( _CRT_WARN, "Failed to reset file pointer to beginning of file. Data lost!\n" );
      PRINTMSG( T_ERROR, "Failed to reset file pointer to beginning of file. Data lost!" );
    }

    if( !WriteFile( file->m_fileHandle, file->m_data, file->m_fileSize, &bytesWritten, NULL ) || bytesWritten != file->m_fileSize )
    {
      _RPT0( _CRT_WARN, "Failed to write to file. Data lost!\n" );
      PRINTMSG( T_ERROR, "Failed to write to file. Data lost!" );
    }
  }

  if( file->m_data )
    free( file->m_data );
  CloseHandle( file->m_fileHandle );
	free( file );
}





//---------------------------------------------------------------------
//	Helper_CreateDirectoryPath
//---------------------------------------------------------------------
void Helper_CreateDirectoryPath( char *path, BOOL has_filename )
{
	DWORD attributes;
	char *sep;
	PRINTMSG( T_TRACE, "Helper_CreateDirectoryPath" );
	
	if( !path )
		return;

	sep = strrchr( path, '\\' );

		// if there's still a separator, and it's not the root, nuke it and recurse
	if( sep && sep > path && sep[-1] != '\\' )
	{
		*sep = 0;
		Helper_CreateDirectoryPath( path, FALSE );
		*sep = '\\';
	}

		// if we have a filename, we're done
	if( has_filename )
		return;

		// if the path already exists, we're done
	attributes = GetFileAttributes(path);
	if( attributes >= 0 )
		return;

		// create the path
	CreateDirectory( path, NULL );
}

//---------------------------------------------------------------------
//	ComposeFilePath
//---------------------------------------------------------------------
BOOL ComposeFilePath( char *buf, UINT32 maxLen, UINT32 pathtype, UINT32 pathindex, const char *filename )
{
  if( !buf || !filename )
    return FALSE;

  if( maxLen < strlen(g_pathNames[pathtype]) + strlen(filename) + 2 )
    return FALSE;

  sprintf( buf, "%s\\%s", g_pathNames[pathtype], filename );
	Helper_ConvertSlashToBackslash( buf );

  return TRUE;
}



//---------------------------------------------------------------------
//	Helper_ConvertSlashToBackslash
//---------------------------------------------------------------------
static void Helper_ConvertSlashToBackslash( char *path )
{
	char *slash;

	if( !path )
		return;

	slash = strrchr( path, '/' );
	while( slash )
	{
		*slash = '\\';
		slash = strrchr( slash, '/' );
	}
}


//---------------------------------------------------------------------
//	Helper_PreloadFile
//---------------------------------------------------------------------
static BOOL Helper_PreloadFile( _osd_file *dest, HANDLE openedFile )
{
	LARGE_INTEGER fileSize;
  DWORD bytesRead = 0;

  if( !dest || openedFile == INVALID_HANDLE_VALUE )
    return FALSE;

  dest->m_fileHandle = openedFile;
  dest->m_dirtyFlag = FALSE;
  dest->m_data = NULL;

    // Store the filesize
	if( !GetFileSizeEx( openedFile, &fileSize ) )
	{
		PRINTMSG( T_ERROR, "GetFileSizeEx failed!" );
		return FALSE;
	}

  dest->m_fileSize = fileSize.QuadPart;
  if( dest->m_fileSize )
  {
      // Read the file into memory
    dest->m_data = (UINT8*)malloc( dest->m_fileSize );
    if( !dest->m_data )
    {
      PRINTMSG( T_ERROR, "Out of memory while loading file!" );
      _RPT0( _CRT_WARN, "Out of memory while loading file!\n" );
      return FALSE;
    }

    if( !ReadFile(  dest->m_fileHandle,
                    dest->m_data,
                    dest->m_fileSize,
                    &bytesRead,
                    NULL ) || bytesRead != dest->m_fileSize )
    {
      _RPT1( _CRT_WARN, "Failed to read %lu bytes!\n", dest->m_fileSize );
      PRINTMSG( T_ERROR, "Failed to read %lu bytes!", dest->m_fileSize );
      free( dest->m_data );
      dest->m_data = NULL;
      return FALSE;
    }
  }
    // Set the read/write head position
  dest->m_filePointer = dest->m_data;

  return TRUE;
}

//---------------------------------------------------------------------
//  Helper_CreateOrOpenSystemPath
//---------------------------------------------------------------------
static BOOL Helper_CreateOrOpenSystemPath( char **path )
{
  DWORD attrib;
  if( !path || !(*path) )
    return FALSE;

  attrib = GetFileAttributes( *path );
	if( attrib == PATH_NOT_FOUND || attrib == 0xFFFFFFFF )
  {
    if( !CreateDirectory( *path, NULL ) )
    {
        // Attempt to create the directory on the Z: instead
      *path[0] = 'Z';
      attrib = GetFileAttributes( *path );
	    if( attrib == PATH_NOT_FOUND || attrib == 0xFFFFFFFF )
      {
        if( !CreateDirectory( *path, NULL ) )
          return FALSE;
      }
    }
  }

  return TRUE;
}
