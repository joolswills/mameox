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



#include "NameQueryInterface.h"



NameQueryInterface::NameQueryInterface()

{

}



NameQueryInterface::~NameQueryInterface()

{

}

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()

