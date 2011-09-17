/*
System_StringModifier.h
Author(s):
Erik B Abair
Purpose:
Container for static string manipulation functions, created to
keep program as OO as possible
*/

#ifndef _SYSTEM_STRING_MODIFIER_H__
#define _SYSTEM_STRING_MODIFIER_H__

// I N C L U D E S /////////////////////////////////////////////////////
#include <string>
#include <stdio.h>

//#include "System_Strings.h"



#define TAB_REPLACE_STR   "  "

//= G L O B A L = V A R S ==============================================

// C L A S S E S ///////////////////////////////////////////////////////
class CSystem_StringModifier
{
public:
	/**************************************************************
	Removes all occurences of todie in string s, and returns
	string s
	***************************************************************/
	static std::string &KillCharacterStr( std::string &s, 
		const char todie, 
		int num = -1) 
	{
		if ( !num )
			return s;

		size_t headIdx = 0;

		if( num > 0 )
		{
			for( int i = num; i >= 0; i-- )
			{
				headIdx = s.find(todie, headIdx);
				if( headIdx == std::string::npos )
					break;

				s.erase( headIdx, 1 ); //remove the character
			}
		}
		else
		{
			while(1)
			{
				headIdx = s.find(todie, headIdx);
				if( headIdx == std::string::npos )
					break;

				s.erase( headIdx, 1 ); //remove the character
			}
		}

		return s;
	}

	/***************************************************************
	Removes all occurences of any character in todie that appears 
	in string s, and returns string s
	***************************************************************/
	static std::string &KillCharactersStr( std::string &s, const char *todie ) {
		size_t headIdx = 0;

		while(1)
		{
			headIdx = s.find_first_of(todie, headIdx);
			if( headIdx == std::string::npos )
				break;

			s.erase( headIdx, 1 );
		}

		return s;
	}

	/***************************************************************
	Remove whitespace from the front of a string
	***************************************************************/
	static std::string &KillLeadingWhitespaceStr( std::string &s ) {
		const char codes[] = { ' ', '\t', '\b', '\r', '\n', 10, 1, 0 };

		size_t headIdx = s.find_first_not_of( codes );
		if( headIdx != std::string::npos )
		{
			std::string tmp = s.substr( headIdx );
			s = tmp;
		}
		else
			s = "";

		return s;
	}

	/***************************************************************
	Removes any newline characters in the string
	***************************************************************/
	static std::string &KillNewlineStr( std::string &s ) {
		const char badchars[] = {'\n', '\r', 10, 0};
		size_t headIdx = 0;

		while(1)
		{
			headIdx = s.find_first_of(badchars, headIdx);
			if( headIdx == std::string::npos )
				break;

			s.erase( headIdx, 1 );
		}

		return s;
	}

	/***************************************************************
	Changes any uppercase letters in s to lowercase
	***************************************************************/
	static inline std::string &LowerStr( std::string &s ) {
		for( unsigned int i = 0; i < s.length(); i++ )
			s[i] = tolower(s[i]);

		return s;
	}

	/***************************************************************
	Changes any lowercase letters in s to uppercase
	***************************************************************/
	static inline std::string &UpperStr( std::string &s ) {
		for( unsigned int i = 0; i < s.length(); i++ )
			s[i] = toupper(s[i]);

		return s;
	}


	/***************************************************************
	Breaks a single token off string s. S is modified to remove that token
	and any whitespace/delimiter up until the next token.
	***************************************************************/
	static std::string TokenStr( std::string &s, const char delim = ' ' ) {
		std::string ret = "";

		KillLeadingWhitespaceStr(s);
		size_t headIdx = s.find( delim );
		if( headIdx == std::string::npos )
		{
			ret = s;
			s = "";
		}
		else
		{
			const char codes[] = { ' ', '\t', '\b', '\r', '\n', 10, 1, 0 };

			ret = s.substr( 0, headIdx );
			KillCharactersStr( ret, codes ) ;
			std::string tmp = s.substr( headIdx + 1 );
			s = KillLeadingWhitespaceStr( tmp );
		}

		return ret;
	}

	/***************************************************************
	Gets a single token off string s. S is not modified in any way
	***************************************************************/
	static std::string PeekTokenStr( const std::string &s, const char delim = ' ' ) {
		std::string ret = "";
		std::string tmp = s;

		KillLeadingWhitespaceStr(tmp);

		size_t headIdx = tmp.find( delim );
		if( headIdx == std::string::npos )
			ret = tmp;
		else
			ret = tmp.substr( headIdx );

		return s;
	}    

	/***************************************************************
	Converts all instances of the character conv in string s
	to the character to
	***************************************************************/
	static std::string &ConvertCharStr( std::string &s, const char conv, const char to ) {
		if( to != conv )
		{
			size_t headIdx = 0;
			while(1)
			{
				headIdx = s.find( conv, headIdx );
				if( headIdx == std::string::npos )
					break;

				s[headIdx++] = to;
			}
		}

		return s;    
	}

	/***************************************************************
	Converts '\b', '\t', etc...
	***************************************************************/
	static std::string &InterpretFormatCharsStr( std::string &s ) {
		std::string tmp = "";

		//skip any leading backspaces 
		size_t headIdx = s.find_first_not_of( '\b' );
		if( headIdx != std::string::npos )
		{
			tmp = s.substr( headIdx );
			s = tmp;
		}

		//Step 1: Interpret any backspaces from
		// back to front as they might remove other formatting chars
		while(1)
		{
			size_t badIdx = s.find_last_of( '\b' );
			if( badIdx == std::string::npos )
				break;

			//copy in string up to (not including) char before backspace
			tmp = s.substr( 0, badIdx - 1 );

			//copy in string after backspace
			tmp += s.substr( badIdx + 1 );
			s = tmp;
		}


		headIdx = 0;
		size_t numspaces = strlen(TAB_REPLACE_STR);
		while(1)
		{
			size_t badIdx = s.find( '\t', headIdx );
			if( badIdx == std::string::npos )
				break;

			tmp = s.substr( 0, badIdx );
			tmp += TAB_REPLACE_STR;
			tmp += s.substr( badIdx + 1 );
			s = tmp;
			headIdx = badIdx + numspaces;
		}

		return s;    
	}

	/***************************************************************
	Returns a filename for a given name
	***************************************************************/
	static std::string FilenameFromName( const std::string &n ) {
		std::string ret = n;

		//kill color codes
//		RemoveFormatStr( ret );

		//Lowercase only
		LowerStr( ret );

		return ret;
	}

	/**************************************************************
	Removes any data after the last newline
	**************************************************************/
	static std::string &RemoveLastLine( std::string &str ) {
		if( str == "" )
			return str;

		size_t tail = str.rfind('\n');
		if( tail == std::string::npos )
		{
			str = "";
			return str;
		}

		size_t newTail = str.rfind('\n', tail-1);
		if( newTail == std::string::npos )
		{
			str = "";
			return str;
		}

		if( str[newTail+1] == '\r' )
			newTail++;

		std::string tmp = str.substr( 0, newTail+1 );
		str = tmp;
		return str;
	}

protected:
};



#endif

