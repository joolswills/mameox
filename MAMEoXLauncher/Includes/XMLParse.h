/*!
 * \file			XMLParse.h
 * \brief			XML file parsing
 */

#ifndef _XMLPARSE_H__
#define _XMLPARSE_H__

//= I N C L U D E S ==========================================================================
#include <Xtl.h>
#include <string>

//= D E F I N E S ============================================================================


//= P R O T O T Y P E S ======================================================================
	//-------------------------------------------------------------------------------------------
	// XMLParseSkipUntilTag
	//!
	//! \brief			Throw away data until the given tag is found
	//!
	//! \param			fd - Descriptor to read from
	//! \param			tag - Delimiter tag 
	//!
	//! \return			HRESULT - operation status
	//! \retval			ERR_OK - Tag found
	//! \retval			ERR_NOENTRY - The tag was not found
	//-------------------------------------------------------------------------------------------
HRESULT XMLParseSkipUntilTag( int fd, const std::string &tag );

	//-------------------------------------------------------------------------------------------
	// XMLParseGetNextTag
	//!
	//! \brief			Throw away data until a tag is found and return that tag
	//!
	//! \param			ret - [OUT] Tag read
	//! \param			fd - Descriptor to read from
	//!
	//! \return			HRESULT - operation status
	//! \retval			ERR_OK - Tag found
	//! \retval			ERR_NOENTRY - No tag found
	//! \retval			ERR_INVALIDPOINTER - ret is NULL
	//-------------------------------------------------------------------------------------------
HRESULT XMLParseGetNextTag( std::string *ret, int fd );

	//-------------------------------------------------------------------------------------------
	// XMLParseReadUntilTag
	//!
	//! \brief			Read data until the given tag is found, return the data read.
	//!
	//! \param			ret - [OUT] Data read
	//! \param			fd - Descriptor to read from
	//! \param			tag - Delimiter tag 
	//!
	//! \return			HRESULT - operation status
	//! \retval			ERR_OK - Tag found
	//! \retval			ERR_NOENTRY - The tag was not found
	//! \retval			ERR_INVALIDPOINTER - ret is NULL
	//-------------------------------------------------------------------------------------------
HRESULT XMLParseReadUntilTag( std::string *ret, int fd, const std::string &tag );



#endif


