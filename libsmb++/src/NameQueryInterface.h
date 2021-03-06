/*
    This file is part of the smb++ library
    Copyright (C) 2000  Nicolas Brodu
    nicolas.brodu@free.fr

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program, see the file COPYING; if not, write
    to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
    MA 02139, USA.
*/
/*
    This class provides a common interface for the underlying NMB core functions
    It's an abstract one. Use the NMB class to instanciate (it will select
    between the different possible NMB core systems)
*/


#ifndef NAME_QUERY_INTERFACE_H
#define NAME_QUERY_INTERFACE_H
#include "defines.h"

struct hostent;

class NameQueryInterface
{
public:
	NameQueryInterface();
	virtual ~NameQueryInterface();
	// Do a name query and can find NetBIOS names as well.
	// groupquery must be specified when looking for a workgroup
	virtual hostent *gethostbyname(const char *name, bool groupquery=false) = 0;
	virtual int setNBNSAddress(uint32 addr) = 0;
	virtual int setNBNSAddress(const char *addr) = 0;
};

#endif // NAME_QUERY_INTERFACE_H
