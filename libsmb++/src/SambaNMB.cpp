#pragma code_seg("CSAMBA")
#pragma data_seg("DSAMBA")
#pragma bss_seg("BSAMBA")
#pragma const_seg("KSAMBA")
#pragma comment(linker, "/merge:DSAMBA=SAMBA")
#pragma comment(linker, "/merge:CSAMBA=SAMBA")
#pragma comment(linker, "/merge:BSAMBA=SAMBA")
#pragma comment(linker, "/merge:KSAMBA=SAMBA")
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

	Implements NameQueryInterface using the Samba code

*/

#include "defines.h"

#ifdef USE_SAMBA



#include <sys/types.h> // needed on FreeBSD

#ifdef _WIN32

#include <winsock2.h>

#else

#include <netdb.h>

#include <sys/socket.h>  // for AF_INET

#endif

#include "SambaNMB.h"

#include "SambaLink.h"

#include "strtool.h"



extern "C" {

BOOL resolve_name(const char *name, struct in_addr *return_ip, int name_type);

}



#ifdef _WIN32

hostent *SambaNMB::gethostbyname(const char *name, bool groupquery)

#else

hostent *SambaNMB::gethostbyname(const char *name, bool groupquery=false)

#endif

{

	struct in_addr ip;

	BOOL ret = True;

	if (!groupquery) ret = resolve_name(name, &ip, 0x20);

	else {

		// try master browser

		ret = resolve_name(name, &ip, 0x1D);

		// no => try for domain

		if (ret != True) ret = resolve_name(name, &ip, 0x1B);

	}

	if (ret != True) return 0;

	newstrcpy(returnValue.h_name, name);

	memcpy(returnValue.h_addr_list[0], &ip, 4);

	return &returnValue;

}

/*

hostent *SambaNMB::gethostbyaddr(const char *addr, int len = 4, int type = AF_INET)

{

	if (type != AF_INET) return 0; // according to man, the only one supported

	return gethostbyname(addr, false); // use Samba resolve_name facility

}

*/



SambaNMB::SambaNMB()

{

	returnValue.h_name = 0;

	returnValue.h_aliases = 0;

	returnValue.h_addrtype = AF_INET;

	returnValue.h_length = 4;

#ifdef _WIN32

	returnValue.h_addr_list = new char*[2];

#else

	returnValue.h_addr_list = new (char*)[2];

#endif

	returnValue.h_addr_list[0] = new char[4];

	returnValue.h_addr_list[1] = 0;

	// load Samba code if not already done

	SambaLink::loadSamba();

}



SambaNMB::~SambaNMB()

{

	if (returnValue.h_name) delete returnValue.h_name;

	if (returnValue.h_addr_list) {

		if (returnValue.h_addr_list[0]) delete returnValue.h_addr_list[0];

		delete returnValue.h_addr_list;

	}

}



int SambaNMB::setNBNSAddress(uint32 addr)

{

	return 1;

}



int SambaNMB::setNBNSAddress(const char *addr)

{

	return 1;

}







#endif



#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()

