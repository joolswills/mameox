/**
	* \file			SkinElement.cpp
	* \brief		CSkinElement class implementation
	*
	*/


//= I N C L U D E S ====================================================
#include "SkinElement.h"
#include "DebugLogger.h"


//= D E F I N E S =====================================================

//= G L O B A L = V A R S =============================================
const char		*g_skinDelimiters = ";:., ";

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================

//---------------------------------------------------------------------
//	StringToColor
//---------------------------------------------------------------------
D3DCOLOR CSkinElement::StringToColor( const CStdString &argbString )
{
	int color[4] = { 255, 0, 0, 0 };
	char *str = strdup( argbString.c_str() );

	char *value = str;
	char *delim = strpbrk( value, g_skinDelimiters );
	for( int i = 0; i < 4 && value; ++i )
	{
		char *next = NULL;
		if( delim )
		{
			*delim = 0;
					
				// Skip to the next number
			next = strpbrk( delim + 1, "0123456789abcdef" );
			delim = strpbrk( next, g_skinDelimiters );
		}

		sscanf( value, "%d", &color[i] );
		value = next;
	}

	free( str );

	return D3DCOLOR_ARGB( color[0], color[1], color[2], color[3] );
}



//---------------------------------------------------------------------
//	StringToRect
//---------------------------------------------------------------------
RECT CSkinElement::StringToRect( const CStdString &rectString )
{
	INT32 values[4] = { -1, -1, -1, -1 };
	char *str = strdup( rectString.c_str() );

	char *value = str;
	char *delim = strpbrk( value, g_skinDelimiters );
	for( int i = 0; i < 4 && value; ++i )
	{
		char *next = NULL;
		if( delim )
		{
			*delim = 0;
					
				// Skip to the next entry
			next = delim + 1;
			while( *next && strchr( g_skinDelimiters, *next ) )
				++next;

			delim = strpbrk( next, g_skinDelimiters );
		}

		if( !stricmp( value, "unbounded" ) )
			values[i] = VALUE_UNBOUNDED;
		else if( !stricmp( value, "auto" ) )
			values[i] = VALUE_AUTO;
		else
			sscanf( value, "%d", &values[i] );
		value = next;
	}

	free( str );

	RECT ret;
	ret.left		= values[0];
	ret.top			= values[1];
	ret.right		= values[2];
	ret.bottom	= values[3];

	return ret;
}

//---------------------------------------------------------------------
//	StringToPoint
//---------------------------------------------------------------------
POINT CSkinElement::StringToPoint( const CStdString &pointString )
{
	INT32 values[2] = { -1, -1 };
	char *str = strdup( pointString.c_str() );

	char *value = str;
	char *delim = strpbrk( value, g_skinDelimiters );
	for( int i = 0; i < 2 && value; ++i )
	{
		char *next = NULL;
		if( delim )
		{
			*delim = 0;
					
				// Skip to the next entry
			next = delim + 1;
			while( *next && strchr( g_skinDelimiters, *next ) )
				++next;

			delim = strpbrk( next, g_skinDelimiters );
		}

		if( !stricmp( value, "auto" ) )
			values[i] = VALUE_AUTO;
		else
			sscanf( value, "%d", &values[i] );
		value = next;
	}

	free( str );

	POINT ret;
	ret.x		= values[0];
	ret.y		= values[1];

	return ret;
}



