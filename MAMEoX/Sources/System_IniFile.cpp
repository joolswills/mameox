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

#ifdef WIN32
 #include <io.h>
#else
 #include <unistd.h>
 #include <sys/types.h>
#endif

//= D E F I N E S ========================================
#define strcasecmp		stricmp
#define strncasecmp		strnicmp

//= G L O B A L = V A R S ================================


//= F U N C T I O N S =======================================

//-----------------------------------
// Constructor
//-----------------------------------
CSystem_IniFile::CSystem_IniFile( const std::string &filename ) :
	m_dirtyFlag( FALSE )
{
	if( filename == "" )
		return;

  char fullpath[256];
  if( !ComposeFilePath( fullpath, 255, FILETYPE_INI, 0, filename.c_str() ) )
    return;

  m_fileName = fullpath;

	std::ifstream is( m_fileName.c_str() );
	if( is.fail() )
		return;

	std::vector< std::string > inputVector;

	while( 1 )
	{
		char buf[8192];

		is.getline( buf, 8191 );

		if( !is.eof() )
		{
			std::string inputStr = buf;
			inputVector.push_back( CSystem_StringModifier::KillLeadingWhitespaceStr( inputStr ) );
		}
		else
			break;
	}

	is.close();

	std::vector<std::string>::iterator i = inputVector.begin();
	for( ; i != inputVector.end(); ++i )
	{
			// Check to see if the current string is a new section
		if( (*i)[0] == '[' )
		{
			std::string sectionHeader = (*i++);
			std::vector<std::string> temp;

			for( ;(i+1) != inputVector.end() && (*(i+1))[0] != '['; ++i )
			{
				if( (*i).size() )	// Don't bother storing blank lines
					temp.push_back( (*i) );
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

	std::ofstream os( m_fileName.c_str() );
	if( os.fail() )
		return;

	std::map< std::string, std::vector<std::string> >::iterator i = m_data.begin();
	for( ; i != m_data.end(); ++i )
	{
		os << (*i).first << std::endl;

		std::vector<std::string>::iterator j = (*i).second.begin();
		for( ; j != (*i).second.end(); ++j )
			os << (*j) << std::endl;

		os << std::endl << std::endl;
	}

	os.close();
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

	std::vector<std::string>::const_iterator i = (*sectionData).second.begin();
	for( ; i != (*sectionData).second.end(); ++i )
	{
		if( !strncasecmp( (*i).c_str(), entryName.c_str(), entryName.size() ) )
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








