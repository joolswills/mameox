/**
* \file			GFilterManager.cpp
* \brief		Graphics Filter Manger subsystem
*
*/

#pragma once

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include "StdString.h"
#include "scale2x.h"
#include <Map>
#include <Vector>

//= T Y P E S ==========================================================

// Enum of all possible filter types
enum EFilterType
{
	eftNone,
	eftScale2X,
	eft2xSai,
	eftSuper2xSai,
	eftSuperEagle
};

struct SFilterInfo
{
	EFilterType m_FilterType;
	WCHAR       m_strName[25];
	DWORD       m_dwMagnificatonLvl;
	void        (*m_filterFunction16)(u8*,u32,u8*,u8*,u32,int,int);
	void        (*m_filterFunction32)(u8*,u32,u8*,u8*,u32,int,int);

	// Constructor
	SFilterInfo() 
	{
		m_dwMagnificatonLvl = 0;
		m_FilterType = eftNone;
		m_filterFunction16 = NULL;
		m_filterFunction32 = NULL;
	}

	// Copy constructor
	SFilterInfo(const SFilterInfo& src)
	{
		lstrcpyW(m_strName, src.m_strName);
		m_dwMagnificatonLvl = src.m_dwMagnificatonLvl;
		m_FilterType        = src.m_FilterType;
		m_filterFunction16  = src.m_filterFunction16;
		m_filterFunction32  = src.m_filterFunction32;
	}
};

//= T Y P E  D E F I N I T I O N S =====================================
typedef std::vector<SFilterInfo> VecFilters;
typedef VecFilters::iterator ItVecFilters;

typedef std::map<EFilterType, SFilterInfo> MapFilters;
typedef MapFilters::iterator ItMapFilters;

//= C L A S S E S ======================================================
class CGFilterManager
{
public:
	CGFilterManager(void);
	virtual ~CGFilterManager(void);

	void SetBitmapDepth(u32 depth);
	void FilterBitmap(u8* dest, u8* src, u32 origRowBytes, int width, int height);

	SFilterInfo GetActiveFilter() { return m_mapFilters[m_CurrentFilter]; }
	void SetActiveFilter(EFilterType filterType);
	MapFilters GetAvailableFilters() { return m_mapFilters; }

protected:
	BOOL m_bIs16Bit;
	void InitFilterMap();
	EFilterType m_CurrentFilter;
	MapFilters m_mapFilters;
};
