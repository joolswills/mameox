/*!
 * \file			XMLParse.cpp
 * \brief			XML file parsing
 */

//= I N C L U D E S =======================================
#include "XMLParse.h"
#include "System_StringModifier.h"

#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <io.h>

//= D E F I N E S ===========================================================================================
#define strcasecmp		stricmp
#define strncasecmp		strnicmp

#define READCHAR( f, b )			if( read( (f), (b), 1 ) != 1 ) return E_FAIL
//= G L O B A L = V A R S ====================================================================================


//= F U N C T I O N S ========================================================================================

//-------------------------------------------------------------------------------------------
// XMLParseSkipUntilTag
//-------------------------------------------------------------------------------------------
HRESULT XMLParseSkipUntilTag( int fd, const std::string &tag )
{
	while( 1 )
	{
		std::string dataBuffer = "";
		char data[2] = {0,0};

		READCHAR( fd, data );
		if( data[0] == '<' )
		{
			dataBuffer += data;

				// Tag opening, read the whole tag
			while( data[0] != '>' )
			{
				READCHAR( fd, data );
				tolower( data[0] );
				dataBuffer += data;
			}

			if( !stricmp( dataBuffer.c_str(), tag.c_str() ) )
				return S_OK;
			dataBuffer = "";
		}

	}

	return E_FAIL;
}

//-------------------------------------------------------------------------------------------
// XMLParseGetNextTag
//-------------------------------------------------------------------------------------------
HRESULT XMLParseGetNextTag( std::string *ret, int fd )
{
	if( !ret )
		return E_POINTER;

	char data[2] = {0,0};

	while( 1 )
	{
		READCHAR( fd, data );
		if( data[0] == '<' )
		{
			*ret += data;

				// Tag opening, read the whole tag
			while( data[0] != '>' )
			{
				READCHAR( fd, data );
				tolower( data[0] );
				*ret += data;
			}
			return S_OK;
		}
	}

	return E_FAIL;
}

//-------------------------------------------------------------------------------------------
// XMLParseReadUntilTag
//-------------------------------------------------------------------------------------------
HRESULT XMLParseReadUntilTag( std::string *ret, int fd, const std::string &tag )
{
	if( !ret )
		return E_POINTER;

	while( 1 )
	{
		std::string dataBuffer = "";
		char data[2] = {0,0};

		READCHAR( fd, data );
		if( data[0] == '<' )
		{
			dataBuffer += data;

				// Tag opening, read the whole tag
			while( data[0] != '>' )
			{
				READCHAR( fd, data );
				tolower( data[0] );
				dataBuffer += data;
			}

			if( !stricmp( dataBuffer.c_str(), tag.c_str() ) )
				return S_OK;

				// This is not the tag we're looking for, just throw it on *ret
			*ret += dataBuffer;
		}
		else
			*ret += data;
	}

	return E_FAIL;
}







