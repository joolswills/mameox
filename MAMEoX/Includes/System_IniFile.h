/*
System_IniFile.h
Author(s):
Erik B Abair (abaire@rpi.edu)
Purpose:
"Ini" file manipulation class header

*/

#ifndef _SYSTEM_INIFILE_H__
#define _SYSTEM_INIFILE_H__

// I N C L U D E S /////////////////////////////////////////////////////////
#include "MAMEoX.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>

// D E F I N E S ///////////////////////////////////////////////////////////

// C L A S S E S ///////////////////////////////////////////////////////////
class CSystem_IniFile
{
public:
	CSystem_IniFile( const std::string &filename );
	~CSystem_IniFile( void );

	// Write an int to the file
	BOOL WriteProfileInt( const std::string &section,
							const std::string &entry,
							long value );

	// Write an int to the file
	BOOL WriteProfileFloat( const std::string &section,
							const std::string &entry,
							float value );

	// Write a string to the file
	BOOL WriteProfileString( const std::string &section,
							const std::string &entry,
							const std::string &value );

	// Get an unsigned long from the file
	long GetProfileInt( const std::string &section,
						const std::string &entry,
						long idefault = 0 ) const;

	// Get an unsigned long from the file
	float GetProfileFloat( const std::string &section,
							const std::string &entry,
							float idefault = 0 ) const;

	// Get a string from the file
	std::string GetProfileString( const std::string &section,
									const std::string &entry,
									const std::string &sdefault ) const;

protected:

	BOOL WriteValueString( const std::string &section, const std::string &entry, const std::string &fullEntryString );
	BOOL GetValueString( const std::string &section, const std::string &entryName, std::string &valueRet ) const;

	std::string											m_fileName; // The ini's filename (and path)
	std::map< std::string, std::vector<std::string> >	m_data;     // The storage array for the data
	BOOL												m_dirtyFlag;// If false, don't write on close
};

#endif




