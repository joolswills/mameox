#pragma code_seg("CSAMBA")
#pragma data_seg("DSAMBA")
#pragma bss_seg("BSAMBA")
#pragma const_seg("KSAMBA")
#pragma comment(linker, "/merge:DSAMBA=SAMBA")
#pragma comment(linker, "/merge:CSAMBA=SAMBA")
#pragma comment(linker, "/merge:BSAMBA=SAMBA")
#pragma comment(linker, "/merge:KSAMBA=SAMBA")

#include "defines.h"
#include "util.h"
#include <xtl.h>
#undef errno

struct hostent *gethostbyname(const char*);
struct hostent *gethostbyaddr(const char* addr, int len, int type);

#define XBOXNAME "myxbox"

int gethostname(char* buffer, int buffersize)
{
#pragma message("*** This function (gethostname) needs to be implemented by getting the machine name from the .xml file! ***")
	if( (unsigned int)buffersize < strlen(XBOXNAME) + 1 )
		return 0;
	strcpy(buffer, XBOXNAME);
	return 1;
}


struct hostent* gethostbyname(const char* _name)
{
	HostEnt* server = new HostEnt;

	if( gethostname(server->name, 128) && strcasecmp(server->name, _name) == 0 )
	{
		XNADDR xna;
		DWORD dwState;
		do
		{
			dwState = XNetGetTitleXnAddr(&xna);
		} while (dwState==XNET_GET_XNADDR_PENDING);

		server->addr_list[0] = server->addr;
		memcpy(server->addr, &(xna.ina.s_addr), 4);
		server->server.h_aliases = 0;
		server->server.h_addrtype = AF_INET;
		server->server.h_length = 4;
		server->server.h_addr_list = new char*[4];
		server->server.h_addr_list[0] = server->addr_list[0];
		server->server.h_addr_list[1] = 0;
		return (struct hostent*)server;
	}

	WSAEVENT hEvent = WSACreateEvent();
	XNDNS* pDns = NULL;
	INT err = XNetDnsLookup(_name, hEvent, &pDns);
	WaitForSingleObject(hEvent, INFINITE);
	if( pDns && pDns->iStatus == 0 )
	{
		unsigned long ulHostIp;
		memcpy(&ulHostIp, &(pDns->aina[0].s_addr), 4);

		strcpy(server->name, _name);
		server->addr_list[0] = server->addr;
		memcpy(server->addr, &(pDns->aina[0].s_addr), 4);
		server->server.h_name = server->name;
		server->server.h_aliases = 0;
		server->server.h_addrtype = AF_INET;
		server->server.h_length = 4;
		server->server.h_addr_list = new char*[4];

		server->server.h_addr_list[0] = server->addr_list[0];
		server->server.h_addr_list[1] = 0;
		XNetDnsRelease(pDns);
		return (struct hostent*)server;
	}
	if( pDns )
		XNetDnsRelease(pDns);
	delete server;
	return 0;
}



#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()


