/*
System_IniFile.cpp
Author(s):
Erik B Abair (abaire@rpi.edu)
Purpose:
"Ini" file manipulation class
*/

//= I N C L U D E S =======================================
#include "System_IniFile.h"
#include "System_StringModifier.h"
#include "xbox_FileIO.h"

#include <stdio.h>
#include <fcntl.h>
#include <algorithm>
#include <string>

#include <io.h>

//= D E F I N E S ========================================
#define strcasecmp		stricmp
#define strncasecmp		strnicmp

//= G L O B A L = V A R S ================================


//= F U N C T I O N S =======================================

//-----------------------------------
// Constructor
//-----------------------------------
CSystem_IniFile::CSystem_IniFile( const CStdString &strFullPath ) :
	m_dirtyFlag( FALSE )
{
	if( strFullPath == "" )
		return;
  
  m_fileName = strFullPath;


	std::vector< std::string > inputVector;

    // Read the entire file into RAM, as reads/writes are unbuffered
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, m_fileName.c_str(), "r" );
  if( !file )
    return;

  osd_fseek( file, 0, SEEK_END );
  UINT32 fileSize = osd_ftell( file );
  osd_fseek( file, 0, SEEK_SET );

  char *buffer = new char[fileSize];
  if( !buffer )
    return;

  osd_fread( file, buffer, fileSize );

  osd_fclose( file );


    // Break the file up into a vector of lines
  UINT32 i = 0;
	while( i < fileSize )
	{
		char buf[8192];
    UINT32 j = 0;
    for( ; buffer[i] != '\n' && buffer[i] != '\r' && j < 8192; ++i, ++j )
      buf[j] = buffer[i];

      // Skip paired \r\n's
    while( buffer[i+1] == '\n' || buffer[i+1] == '\r' )
      ++i;

    if( j == 8192 )
    {
      PRINTMSG( T_ERROR, "Corrupt INI file, read 8k chars before finding a \\n!" );
      return;
    }
    else
      buf[j] = 0;

      // Catch no newline at end of file
		if( buffer[i] == '\n' || buffer[i] == '\r' )
		{
      ++i;
			std::string inputStr = buf;
			inputVector.push_back( CSystem_StringModifier::KillLeadingWhitespaceStr( inputStr ) );
		}
		else
			break;
	}

    // The file data is no longer needed
  delete[] buffer;


    // Break each line up
	std::vector<std::string>::iterator it = inputVector.begin();
	for( ; it != inputVector.end(); ++it )
	{
			// Check to see if the current string is a new section
		if( (*it)[0] == '[' )
		{
			std::string sectionHeader = (*it);
			std::vector<std::string> temp;

			for( ;(it+1) != inputVector.end() && (*(it+1))[0] != '['; ++it )
			{
				if( (*(it+1)).size() )	// Don't bother storing blank lines
          temp.push_back( (*(it+1)) );
			}

			m_data[sectionHeader] = temp;
		}
	}
}

//-----------------------------------
// Destructor
//-----------------------------------
CSystem_IniFile::~CSystem_IniFile( void )
{
	if( !m_dirtyFlag || m_fileName == "" )
		return;

    // Write via osd_fwrite, as it's buffered
  osd_file *file = osd_fopen( FILETYPE_MAMEOX_FULLPATH, 0, m_fileName.c_str(), "w" );
  if( !file )
    return;

	std::map< std::string, std::vector<std::string> >::iterator i = m_data.begin();
	for( ; i != m_data.end(); ++i )
	{
    osd_fwrite( file, (*i).first.c_str(), (*i).first.length() );
    osd_fwrite( file, "\n", 1 );

		std::vector<std::string>::iterator j = (*i).second.begin();
		for( ; j != (*i).second.end(); ++j )
    {
      osd_fwrite( file, (*j).c_str(), (*j).length() );
      osd_fwrite( file, "\n", 1 );
    }

    osd_fwrite( file, "\n\n", 2 );
	}

  osd_fclose( file );
}


//-----------------------------------
// WriteValueString
//-----------------------------------
BOOL CSystem_IniFile::WriteValueString( const std::string &section, const std::string &entry, const std::string &fullEntryString )
{

	std::vector<std::string> &sectionVector = m_data[ section ];
	std::vector<std::string>::iterator i = sectionVector.begin();

	for( ; i != sectionVector.end(); ++i )
	{
		if( !strncasecmp( (*i).c_str(), entry.c_str(), entry.size() ) )
		{
			(*i) = fullEntryString;
			return TRUE;
		}
	}

	sectionVector.push_back( fullEntryString );
	return TRUE;
}


//-----------------------------------
// WriteProfileInt
//-----------------------------------
BOOL CSystem_IniFile::WriteProfileInt( const std::string &section, 
									  const std::string &entry, 
									  long value )
{
	m_dirtyFlag = TRUE;

	// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";

	// Create the string that we're gonna use ('entry' = 'value') 
	char buf[64];
	sprintf( buf, "%lu", value );
	std::string entryString = entry;
	entryString += " = ";
	entryString += buf;

	return WriteValueString( sect, entry, entryString );
}

//-----------------------------------
// WriteProfileFloat
//-----------------------------------
BOOL CSystem_IniFile::WriteProfileFloat(const std::string &section, 
										const std::string &entry, 
										float value )
{
	m_dirtyFlag = TRUE;

	// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";

	// Create the string that we're gonna use ('entry' = 'value') 
	char buf[64];
	sprintf( buf, "%f", value );
	std::string entryString = entry;
	entryString += " = ";
	entryString += buf;

	return WriteValueString( sect, entry, entryString );
}

//-----------------------------------
// WriteProfileString
//-----------------------------------
BOOL CSystem_IniFile::WriteProfileString( const std::string &section,
										 const std::string &entry,
										 const std::string &value )
{
	m_dirtyFlag = TRUE;

	// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";

	// Create the string that we're gonna use ('entry' = 'value') 
	std::string entryString = entry;
	entryString += " = ";
	entryString += value;

	return WriteValueString( sect, entry, entryString );
}




//-----------------------------------
// GetValueString
//-----------------------------------
BOOL CSystem_IniFile::GetValueString( const std::string &section, 
									 const std::string &entryName, 
									 std::string &valueRet ) const
{
	std::map< std::string, std::vector<std::string> >::const_iterator sectionData = m_data.find( section );
  if( sectionData == m_data.end() )
    return FALSE;

	std::vector<std::string>::const_iterator i = (*sectionData).second.begin();
	for( ; i != (*sectionData).second.end(); ++i )
	{
    PRINTMSG( T_INFO, "%s =? %s", (*i).c_str(), entryName.c_str() );
		if( !strncasecmp( (*i).c_str(), entryName.c_str(), entryName.length() ) )
		{
			char *front = strstr( (*i).c_str(), "=" );
			if( !front )
				return FALSE;

			valueRet = ++front;
			CSystem_StringModifier::KillLeadingWhitespaceStr( valueRet );
			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------
// GetProfileInt
//-----------------------------------
long CSystem_IniFile::GetProfileInt( const std::string &section,
									const std::string &entry,
									long idefault ) const
{
		// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";


	std::string retStr;
	if( GetValueString( sect, entry, retStr ) )	
	{
		long ret;
		sscanf( retStr.c_str(), "%lu", &ret );
		return ret;
	}

	return idefault;
}

//-----------------------------------
// GetProfileFloat
//-----------------------------------
float CSystem_IniFile::GetProfileFloat(	const std::string &section,
									   const std::string &entry,
									   float idefault ) const
{
	// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";

	std::string retStr;
	if( GetValueString( sect, entry, retStr ) )	
	{
		float ret;
		sscanf( retStr.c_str(), "%f", &ret );
		return ret;
	}

	return idefault;
}

//-----------------------------------
// GetProfileString
//-----------------------------------
std::string CSystem_IniFile::GetProfileString( const std::string &section,
											  const std::string &entry,
											  const std::string &sdefault ) const
{
	// Format the section header
	std::string sect = "[";
	sect += section;
	sect += "]";


	std::string retStr;
	if( GetValueString( sect, entry, retStr ) )	
		return retStr;

	return sdefault;
}








