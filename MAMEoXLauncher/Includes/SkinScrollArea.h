/**
	* \file			SkinScrollArea.h
	*/
#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "System_IniFile.h"

#include "SkinElement.h"
#include "SkinHighlightBar.h"

#include <XBUtil.h>

#ifdef __cplusplus

//= D E F I N E S =====================================================


//= C L A S S E S ====================================================


/**
 * \class		CSkinScrollArea
 * \brief   Contains layout information for a scroll area
 */
class CSkinScrollArea : public CSkinElement
{
public:
		//------------------------------------------------------
		//	Constructor
		//------------------------------------------------------
	CSkinScrollArea( void ) : CSkinElement( SKINELEMENT_SCROLLAREA ) {
		m_singleRowHeight = VALUE_INVALID;
		m_highlightBar = NULL;
	}

		//------------------------------------------------------
		//	Constructor (CSkinScrollArea)
		//------------------------------------------------------
	CSkinScrollArea( const CSkinScrollArea &a ) : CSkinElement( a ) {
		m_singleRowHeight = a.m_singleRowHeight;
		if( a.m_highlightBar )
			m_highlightBar = new CSkinHighlightBar( *a.m_highlightBar );
		else
			m_highlightBar = NULL;
	}

		//------------------------------------------------------
		// Destructor
		//------------------------------------------------------
	~CSkinScrollArea( void ) {
		if( m_highlightBar )
			delete m_highlightBar;
	}

		//------------------------------------------------------
		//------------------------------------------------------
	BOOL ParseINI(	CSystem_IniFile &iniFile, 
									const CStdString &sectionName, 
									const CStdString &entryName, 
									const CSkinElement *parentObject );


	FLOAT									m_singleRowHeight;
	CSkinHighlightBar			*m_highlightBar;
};







//= G L O B A L = V A R S =============================================

//= P R O T O T Y P E S ===============================================

//= F U N C T I O N S =================================================


#endif		// __cplusplus

