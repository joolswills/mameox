/**
	* \file			SkinChooser.h
	* \brief		Helper class which takes care of loading and maintaining
	*           the list of available skins
	*
	*/

#pragma once

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "BaseMenuView.h"

#include "SkinResource.h"
#include "SkinResource.h"
#include "MAMEoX.h"

#include <vector>

//= D E F I N E S ======================================================

//= G L O B A L = V A R S ==============================================


//= C L A S S E S ======================================================
class CSkinChooser
{
public:

		//------------------------------------------------------------
		// FindSkins
		//! \brief		Find any valid skins in the skin directory.
		//!
		//! \return		BOOL - Operation status
		//! \retval		TRUE - Operation successful
		//! \retval		FALSE - No skins could be found.
		//------------------------------------------------------------
	BOOL FindSkins( void );

		//------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------
  ~CSkinChooser( void ) {
		std::vector<CSkinResource*>::iterator i = m_skinResourceVector.begin();
		for( ; i != m_skinResourceVector.end(); ++i )
			delete (*i);
  }

	SkinOptions_t										m_options;
	std::vector<CSkinResource*>			m_skinResourceVector;
};





