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
    Copyright (C) 1999  Nicolas Brodu
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
#include "defines.h"
#ifndef USE_SAMBA

#include <errno.h>
#undef errno
#include <sys/types.h>
#ifdef _WIN32
#include <time.h>
#ifdef _XBOX
#include <xtl.h>
#undef errno
#else
#include <winsock2.h>
#endif
//#define strcasecmp xboxstricmp
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#ifdef DEBUG
#include <stdio.h>
#include <iostream.h>
#endif
#include "NMBIO.h"
#include "NameServicePacket.h"

static uint32 NBNS = 0; // IP of a NetBIOS name server


NMBIO::NMBIO(const char *name) // can specify our host name
{
	socknetaddr = new (struct sockaddr_in);
	struct hostent *us;
	cache = new NBHostCache();
//	NBNS=0;
	if (name) {
		ourName=new char[strlen(name)+1];
		strcpy(ourName,name);
		char *DNSName=new char[100];
    	::gethostname(DNSName,100); // and this is our DNS name
		us=::gethostbyname(DNSName); // store our net info here
		delete DNSName;
	} else {
#ifdef OURNAME
		ourName=new char[strlen(OURNAME)+1];
		strcpy(ourName,OURNAME);
		char *DNSName=new char[100];
    	::gethostname(DNSName,100); // and this is our DNS name
		us=::gethostbyname(DNSName); // store our net info here
		delete DNSName;
#else
		// Assume we have NetBIOS name = DNS name
		ourName=new char[100];
    	::gethostname(ourName,100);
		us=::gethostbyname(ourName); // store our net info here
		// search a "." so as to remove domain name
		char *p=strchr(ourName,'.');
		if (p) p[0]=0; // remove domain
#endif
	}

	netaddr=0xFFFFFFFF;
	uint32 ourIP=0;
	if ( (us) && (us->h_addr_list) && (us->h_addr_list[0]) ) {
		memcpy(&netaddr,us->h_addr_list[0],4);
		memcpy(&ourIP,us->h_addr_list[0],4);
		ourIP=BIGEND32(ourIP);
		uint8 net=(uint8)(us->h_addr_list[0][0]);
		uint32 mask=0xFFFFFFFF;

#ifndef _XBOX
		if (net<=127) {mask=BIGEND32(0x00FFFFFF);}
		else if (net<192) {mask=BIGEND32(0x0000FFFF);}
		else {mask=BIGEND32(0x000000FF);}

#else

		mask = 0xFFFFFFFF;

#endif
		netaddr|=mask;
	}
	// use it to create a broadcast address
	socknetaddr->sin_family = AF_INET;
	socknetaddr->sin_port = htons(137);	// NameService port
	memcpy(&(socknetaddr->sin_addr), &netaddr, 4);
	
	NameQueryPacket query(ourName);
	ourNBName=query.getQueryNBName();
#if DEBUG >= 6
	cout<<"+NMBIO : our name : "<<ourName<<", our NetBIOS Name : "<<ourNBName<<"\n";
#endif
	// set our name in cache, if any, with 'infinite' timeout
	if (ourIP) cache->add(ourNBName, ourName, ourIP, 0, false, 0x0FFFFFFF);
}

NMBIO::~NMBIO()
{
	if (cache) delete cache;
	if (ourName) delete ourName;
	if (ourNBName) delete ourNBName;
	if (socknetaddr) delete socknetaddr;
}

char *NMBIO::getOurName()
{
	if (!ourName) return 0;
	char *ret=new char[strlen(ourName)+1];
	strcpy(ret,ourName);
	return ret;
}

char *NMBIO::getOurNBName()
{
	if (!ourNBName) return 0;
	char *ret=new char[strlen(ourNBName)+1];
	strcpy(ret,ourNBName);
	return ret;
}

void NMBIO::setOurName(const char *name)
{
	if (!name) return;

	uint32 ourIP=0;
	if (ourName) {
		// remove old name from cache but keep IP
		NBHostEnt *cacheEntry = cache->find(ourName);
		if (cacheEntry) {
			ourIP=cacheEntry->ip;
			delete cacheEntry;
		}
		cache->remove(ourName);
		delete ourName;
	}
	if (!ourIP) { // then call system functions
		struct hostent *us;
		char DNSName[101];
		::gethostname(DNSName,100); // and this is our DNS name
		us=::gethostbyname(DNSName); // store our net info here
		if ( (us) && (us->h_addr_list) && (us->h_addr_list[0]) ) {
			memcpy(&ourIP,us->h_addr_list[0],4);
			ourIP=BIGEND32(ourIP);
		}
	}
	
	ourName=new char[strlen(name)+1];
	strcpy(ourName,name);
	if (ourNBName) delete ourNBName;
	NameQueryPacket query(ourName);
	ourNBName=query.getQueryNBName();
	
	// Add our name to cache with 'infinite' timeout
	cache->add(ourNBName, ourName, ourIP, 0, false, 0x0FFFFFFF);
}
	
// sets the broadcast address !
int NMBIO::setNetworkBroadcastAddress(uint32 addr)
{
	netaddr=addr;
	socknetaddr->sin_family = AF_INET;
	socknetaddr->sin_port = htons(137);	// NameService port
	memcpy(&(socknetaddr->sin_addr), &netaddr, 4);
	return 0;
}

int NMBIO::setNetworkBroadcastAddress(const char *addr)
{
/*	struct in_addr ip;
	if (!(inet_aton(addr, &ip))) return -1;
	memcpy(&netaddr, &ip, 4);*/
	netaddr=inet_addr(addr);
	socknetaddr->sin_family = AF_INET;
	socknetaddr->sin_port = htons(137);	// NameService port
	memcpy(&(socknetaddr->sin_addr), &netaddr, sizeof(netaddr));
	return 0;
}

int NMBIO::getError()
{
	return errno;
}

// sets the NBNS address
int NMBIO::setNBNSAddress(uint32 addr)
{
	NBNS=addr;
	return 0;
}

int NMBIO::setNBNSAddress(const char *addr)
{
/*	struct in_addr ip;
	if (!(inet_aton(addr, &ip))) return -1;*/
	uint32 ip=inet_addr(addr);
	if ((ip==0xFFFFFFFF) && strcmp(addr,"255.255.255.255")) return -1;
	memcpy(&NBNS, &ip, 4);
	return 0;
}

char *NMBIO::decodeNBName(const char* NBName, bool groupFlag)
{
	if (!NBName) return 0;
	int len=strlen(NBName);
	if (!len) {return 0;}
	char *ret=new char[len/2+1]; // should be close enough to reality
	uint8 NBLen=(uint8)NBName[0];
	if (len+1<NBLen) // invalid string, we do not recognize NetBIOS string pointers
		{delete ret; return 0;}
	for (int i=0; i<NBLen/2; i++)
		ret[i]=(((NBName[1+i*2]-0x41)&0xF)<<4) | ((NBName[1+i*2+1]-0x41)&0xF);
	ret[NBLen/2]=0;
	// Take care of removing group flag if specified
	if (groupFlag) {
		ret[NBLen/2-1]=' ';
		ret[NBLen/2-2]=' ';
	}
	// Now remove trailing spaces
#ifdef _WIN32
	for (i=NBLen/2-1; i>=0; i--)
#else
	for (int i=NBLen/2-1; i>=0; i--)
#endif
		if (ret[i]==' ') ret[i]=0; else break;
	// We do not care for domain name
	return ret;
}

#ifdef _WIN32
NBHostEnt *NMBIO::gethostbyname(const char *name, bool groupFlag)
#else
struct NBHostEnt *NMBIO::gethostbyname(const char *name, bool groupFlag)
#endif
{
//	struct in_addr ip;
//	struct sockaddr_in connectParam; // parameters of the connection
	NBHostEnt *ret=0, *current=0;

	if ((!name) || (name[0]==0)) return 0; // no need to check
	
// 1. Look in the cache

	NBHostEnt *cache_entry = cache->find(name, groupFlag);
	if (cache_entry != 0) 
	  if (cache_entry->ip) {
	    return cache_entry;
	  }
	  else
	    return 0; // The host doesn't exists.
	

// 2. Not in cache, have to lookup
	// If it is a dot form => hope NetBIOS and DNS name are the same...
	uint32 ipDot=inet_addr(name);
	if ((ipDot!=0xFFFFFFFF) || !strcmp(name,"255.255.255.255"))
	{ // yes, dot form, do a standard call since it cannot be
	  // a valid NetBIOS name
#ifdef DNSQUERY
		struct hostent *server=::gethostbyname(name);
	  	if (server) { // h_name field contains a dot notation ! => do by addr
			server=::gethostbyaddr(server->h_addr_list[0],server->h_length,AF_INET);
		}
	  	if ((!server) || (!(server->h_name)) || (inet_addr(server->h_name)!=0xFFFFFFFF))
			return 0; // still a dot notation
		// not a dotted form now, search a "." so as to remove domain name
		char *p=strchr(server->h_name,'.');
		if (p) p[0]=0; // remove domain
		return gethostbyname(server->h_name);
#else
		int sock=0; // needed for NetBIOS name query
		if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
		{
#if DEBUG >= 1
			cout<<"NMBIO::gethostbyname, cannot allocate socket.\n";
#endif
			errno=NO_RECOVERY;
			return 0;
		}
	
		struct sockaddr_in socknetaddr;
		socknetaddr.sin_family = AF_INET;
		socknetaddr.sin_port = htons(137);	// NameService port
		uint32 addr = inet_addr(name);
		memcpy(&(socknetaddr.sin_addr), &addr, 4);

		NameQueryPacket query("*",0,0,1); // only samba servers answer with * !

		uint8* p=query.packet();

#if DEBUG >= 6
		for (int i=0; i<query.getLength(); i++) printf("%X ",p[i]);
		cout<<"\n";
#endif
		
#ifdef _WIN32
		if (sendto(sock, (const char*)p, query.getLength()-2, 0, (sockaddr*)&socknetaddr, sizeof(struct sockaddr_in))==-1)
#else
		if (sendto(sock, p, query.getLength(), 0, (sockaddr*)&socknetaddr, sizeof(struct sockaddr_in))==-1)
#endif
		{
#if DEBUG >= 1
		  cout<< "NMBIO::gethostbyname, cannot send datagram.\n";
#endif
			errno=NO_RECOVERY;
#ifdef _WIN32
			if (sock) closesocket(sock);
#else
			if (sock) close(sock);
#endif
			return 0;
		}
        delete p;
		
		uint8 *rawdata=new uint8[1001]; // UDP doesn't use big packet
		int32 queue=0;

		struct timeval tv;
		fd_set rfds;	// from man select...
		// for all correct positive answers, stops when there is no answer left
		while (1)
		{
			FD_ZERO(&rfds);
			FD_SET(sock,&rfds);
#ifndef DNSQUERY
			tv.tv_sec = 0;	
			tv.tv_usec = 100000; // wait max .1 sec for UDP broadcast feedback
#else
			tv.tv_sec = 2;	//wait 2 sec
			tv.tv_usec = 0;
#endif
			// timeout=>exit loop
			if (!select(sock+1, &rfds, 0, 0, &tv)) break;
/*			FD_ZERO(&rfds);
			FD_SET(sock,&rfds);
			tv.tv_sec = 1;	// wait max 1 sec
			tv.tv_usec = 0;*/
//			queue=recvfrom(sock, rawdata, 1000, 0, 0, 0);
#ifdef _WIN32
			queue=recv(sock, (char*)rawdata, 1000, 0);
#else
			queue=read(sock, rawdata, 1000);
#endif
#if DEBUG >= 6
			cout<<"queue : "<<queue<<"\n";
			for (int i=0; i<queue; i++)
			{
				printf("%X ",rawdata[i]);
			}
			cout<<"\n";
#endif
			if (queue<=0) break;
			rawdata[queue]=0; // barrier, ok : rawdata size=1001
			if (queue<4) continue;
			if ((rawdata[0]!=0) || (rawdata[1]!=0) || // id=0
				((rawdata[2]&0xFD)!=0x84) || ((rawdata[3]&0x7F)!=0))
				continue;
			// Positive answer :-)
			if (queue<12) continue;
			// ignore fixed bytes
			int len=strlen((char*)(rawdata+12)); // barrier prevents cataclysm
			if (queue<12+len+1+8+2) continue;
			// skip more bytes
			uint16 dataLen=(((uint16)rawdata[12+len+1+8]) << 8)
						|(((uint16)rawdata[12+len+1+8+1]) &0xFF);
			if (queue<12+len+1+8+2+dataLen) continue;
			uint8 entries=(uint8)rawdata[12+len+1+8+2];
			uint8 *dat=rawdata+12+len+1+8+2+1;
			for (int i=0; i<entries; i++)
			{
				char* name = (char *)dat;
				unsigned char type = *(dat+15);
				unsigned char flags = *(dat+16);
				if(type == 0 && (flags & 0x04) && (flags & 0x80) == 0)
				{
					int len = strlen(name);
					while( name[len-1] == ' ' ) len--;
					char* realName = new char[len+1];
					memset(realName, 0, len+1);
					strncpy(realName, name, len);
#ifdef _WIN32
					if (sock) closesocket(sock);
#else
					if (sock) close(sock);
#endif
					return gethostbyname(realName, groupFlag);
				}
				dat += 18;
			}
			break;
		}
#ifdef _WIN32
		if (sock) closesocket(sock);
#else
		if (sock) close(sock);
#endif
		delete rawdata;
		return 0; // dot form not allowed if this option is not present
#endif
	}
	else
	{

// 3. Try NBNS, if any, before doing any UDP broadcast
		ret=askNBNS(name,groupFlag);
		if (ret) return ret;

// 4. broadcast a NetBIOS name query (hosts limited to local subnet)
		int sock=0; // needed for NetBIOS name query
				
		if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
		{
#if DEBUG >= 1
			cout<<"NMBIO::gethostbyname, cannot allocate socket.\n";
#endif
			errno=NO_RECOVERY;
			return 0;
		}
	
		int yes=1;
 		// David: the (void*) cast is required for Solaris
#ifdef _WIN32
	    if (setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(const char*)&yes,sizeof(int)) < 0)
#else
	    if (setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(void*)&yes,sizeof(int)) < 0)
#endif
//			|(fcntl(sock, F_SETFL, O_NONBLOCK)< 0))
		{
#if DEBUG >= 1
			cout<<"NMBIO::gethostbyname, set socket option impossible.\n";
#endif
#ifdef _WIN32
			if (sock) closesocket(sock);
#else
			if (sock) close(sock);
#endif
			errno=NO_RECOVERY;
			return 0;
		}
		
		// NB name, broadcast, id=0 (important, see below)
		NameQueryPacket query(name,1,0,0,groupFlag);
//		NameQueryPacket query("*",1,0); // only samba servers answer with * !

		uint8* p=query.packet();

#if DEBUG >= 6
		for (int i=0; i<query.getLength(); i++) printf("%X ",p[i]);
		cout<<"\n";
#endif
		
#ifdef _WIN32
		if (sendto(sock, (const char*)p, query.getLength(), 0, (sockaddr*)socknetaddr, sizeof(struct sockaddr_in))==-1)
#else
		if (sendto(sock, p, query.getLength(), 0, (sockaddr*)socknetaddr, sizeof(struct sockaddr_in))==-1)
#endif
		{
#if DEBUG >= 1
		  cout<< "NMBIO::gethostbyname, cannot send datagram.\n";
#endif
			errno=NO_RECOVERY;
#ifdef _WIN32
			if (sock) closesocket(sock);
#else
			if (sock) close(sock);
#endif
			return 0;
		}
        delete p;
		
		uint8 *rawdata=new uint8[1001]; // UDP doesn't use big packet
		int32 queue=0;

		struct timeval tv;
		fd_set rfds;	// from man select...
		// for all correct positive answers, stops when there is no answer left
		while (1)
		{
			FD_ZERO(&rfds);
			FD_SET(sock,&rfds);
#ifndef DNSQUERY
			tv.tv_sec = 2;	
			tv.tv_usec = 100000; // wait max .1 sec for UDP broadcast feedback
#else
			tv.tv_sec = 2;	//wait 2 sec
			tv.tv_usec = 0;
#endif
			// timeout=>exit loop
			if (!select(sock+1, &rfds, 0, 0, &tv)) break;
/*			FD_ZERO(&rfds);
			FD_SET(sock,&rfds);
			tv.tv_sec = 1;	// wait max 1 sec
			tv.tv_usec = 0;*/
//			queue=recvfrom(sock, rawdata, 1000, 0, 0, 0);
#ifdef _WIN32
			queue=recv(sock, (char*)rawdata, 1000, 0);
#else
			queue=read(sock, rawdata, 1000);
#endif
#if DEBUG >= 6
			cout<<"queue : "<<queue<<"\n";
			for (int i=0; i<queue; i++)
			{
				printf("%X ",rawdata[i]);
			}
			cout<<"\n";
#endif
			if (queue<=0) break;
			rawdata[queue]=0; // barrier, ok : rawdata size=1001
			if (queue<4) continue;
			if ((rawdata[0]!=0) || (rawdata[1]!=0) || // id=0
				((rawdata[2]&0xFD)!=0x85) || ((rawdata[3]&0x7F)!=0))
				continue;
			// Positive answer :-)
			if (queue<12) continue;
			// ignore fixed bytes
			int len=strlen((char*)(rawdata+12)); // barrier prevents cataclysm
			if (queue<12+len+1+8+2) continue;
			// skip more bytes
			uint16 addrLen=(((uint16)rawdata[12+len+1+8]) << 8)
						|(((uint16)rawdata[12+len+1+8+1]) &0xFF);
			if (queue<12+len+1+8+2+addrLen) continue;
			uint8 *dat=rawdata+12+len+1+8+2+2;
			addrLen/=6; // flags+ip
			for (int i=0; i<addrLen; i++)
			{
				// we should seek here the member names
				// instead of copying the group name if so
				if (!ret) {ret=new NBHostEnt; current=ret;}
				else {current->next=new NBHostEnt; current=current->next;}
				current->NBName=new char[len+1];
				strcpy(current->NBName,(char*)(rawdata+12));
				current->name=decodeNBName(current->NBName,groupFlag);
				current->ip=(((uint32)(*dat)&0xFF)<<24)
							|(((uint32)(*(dat+1))&0xFF)<<16)
							|(((uint32)(*(dat+2))&0xFF)<<8)
							|((uint32)(*(dat+3))&0xFF);
#if DEBUG>=5
cout<<"host query : name="<<current->name<<", ip=";
cout<<(int)((*dat)&0xFF)<<"."<<(int)((*(dat+1))&0xFF)<<"."<<((int)(*(dat+2))&0xFF)<<"."<<(int)((*(dat+3))&0xFF)<<"\n";
#endif
				dat+=6; // skip flags
			}
			break;
		}
#ifdef _WIN32
		if (sock) closesocket(sock);
#else
		if (sock) close(sock);
#endif
		delete rawdata;

#ifdef DNSQUERY
		if (!ret) { // Server not found locally
			struct hostent *server=::gethostbyname(name); // => try to pass a router and hope
		  	if ((server) && (server->h_addr_list) && (server->h_addr_list[0])) { // that DNS and NetBIOS names are the same !
				server=::gethostbyaddr(server->h_addr_list[0],server->h_length,AF_INET);
			} // this was done to remove dot notations
//		  	if ((!server) || (!(server->h_name)) || (!(server->h_addr_list)) || (!(server->h_addr_list[0])) || (inet_aton(server->h_name, &ip)))
		  	if ((!server) || (!(server->h_name)) || (!(server->h_addr_list)) || (!(server->h_addr_list[0])) || (inet_addr(server->h_name)!=0xFFFFFFFF))
				return 0; // still a dot notation, or no server/name
			// not a dotted form now, search a "." so as to remove domain name
			char *p=strchr(server->h_name,'.');
			if (p) p[0]=0; // remove domain
			ret=new NBHostEnt;
			ret->name=new char[strlen(server->h_name)+1];
			strcpy(ret->name,server->h_name);
			memcpy(&ret->ip,server->h_addr_list[0],4);
			ret->ip=BIGEND32(ret->ip); // htonl ...
			NameQueryPacket query(ret->name,0,0,0,groupFlag);
			ret->NBName=query.getQueryNBName();
		}
#endif // DNSQUERY
		if (ret)
		  cache->add(ret->NBName, ret->name, ret->ip, 0, groupFlag, 120);
		return ret;
	}
}


// This doesn't work ! server will not indicate its name whatever packet
// we send it !

#ifdef _WIN32
NBHostEnt *NMBIO::gethostbyaddr(uint32 IP, bool groupFlag)
#else
struct NBHostEnt *NMBIO::gethostbyaddr(uint32 IP, bool groupFlag)
#endif
{
        NBHostEnt *ret=new NBHostEnt; 
	ret = cache->find(IP, groupFlag); // Check the cache
	if (ret != 0) 
	  return ret;
	struct sockaddr_in connectParam; // parameters of the connection
	
	int sock; // needed for NetBIOS name query
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
	{
#if DEBUG >= 1
		cout<<"NMBIO::gethostbyaddr, cannot allocate socket.\n";
#endif
		errno=NO_RECOVERY;
		return 0;
	}
	
	connectParam.sin_family = AF_INET;
	connectParam.sin_port = htons(137);	// NameService port
	uint32 bigIP=BIGEND32(IP);
	memcpy(&connectParam.sin_addr, &bigIP, sizeof(IP));
	
	// any name, no broadcast, id=0 (important, see below)
	NameQueryPacket query("*",0,0,groupFlag);
//	NameConflictDemand query("*",0);

	uint8* p=query.packet();
#if DEBUG >= 6
	for (int i=0; i<query.getLength(); i++) printf("%X ",p[i]);
	cout<<"\n";
#endif
				
#ifdef _WIN32
	if (sendto(sock, (const char *)p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#else
	if (sendto(sock, p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#endif
	{
#if DEBUG >= 1
		cout<<"NMBIO::gethostbyaddr, cannot send datagram.\n";
#endif
		errno=NO_RECOVERY;
		return 0;
	}
	delete p;
		
	uint8 *rawdata=new uint8[1001]; // UDP doesn't use big packet
	int32 queue=0;

	struct timeval tv;
	fd_set rfds;	// from man select...
	
	// wait response
	FD_ZERO(&rfds);
	FD_SET(sock,&rfds);
	tv.tv_sec = 1;	// wait max 1 sec
	tv.tv_usec = 0;
	// timeout=>exit
	if (!select(sock+1, &rfds, 0, 0, &tv)) {errno=HOST_NOT_FOUND; delete rawdata;  return 0;}
//		queue=recvfrom(sock, rawdata, 1000, 0, 0, 0);
#ifdef _WIN32
	queue=recv(sock, (char*)rawdata, 1000, 0);
#else
	queue=read(sock, rawdata, 1000);
#endif
#if DEBUG >= 6
	cout<<"queue : "<<queue<<"\n";
	for (int i=0; i<queue; i++) {printf("%X ",rawdata[i]);}
	cout<<"\n";
#endif

	if (queue<=0) {errno=NO_RECOVERY; delete rawdata; return 0;}
	rawdata[queue]=0; // barrier, ok : rawdata size=1001
	if (queue<4) {errno=NO_RECOVERY; delete rawdata; return 0;}
	// we don't care here for the errcode
	if ((rawdata[0]!=0) || (rawdata[1]!=0) || // id=0
		((rawdata[2]&0xFD)!=0x85)) {errno=HOST_NOT_FOUND; delete rawdata; return 0;}
	// ignore fixed bytes
	if (queue<12) {errno=NO_RECOVERY; delete rawdata; return 0;}
	int len=strlen((char*)(rawdata+12)); // barrier prevents cataclysm
        if(!ret) // It's most probably null from the cache try.
	  ret = new NBHostEnt;
	ret->NBName=new char[len+1];
	strcpy(ret->NBName,(char*)(rawdata+12));
	ret->name=decodeNBName(ret->NBName,groupFlag);
#if DEBUG >= 6
cout<<"Name found : "<<ret->name<<"\n";
#endif
	ret->ip=IP;
	delete rawdata;
	return ret;
}

void NMBIO::addNameIpToCache(const char *hostname, uint32 ip, uint32 timeout, bool groupFlag)
{
  NameQueryPacket *nameQP = new NameQueryPacket(hostname, 0, 0, 0, groupFlag);
  cache->add(nameQP->getQueryNBName(), hostname, ip, 0, groupFlag, timeout);
}  


#ifdef _WIN32
NBHostEnt *NMBIO::askNBNS(const char *name, bool groupFlag)
#else
struct NBHostEnt *NMBIO::askNBNS(const char *name, bool groupFlag)
#endif
{
	if (!NBNS) return 0;
	
	int sock=0; // needed for NetBIOS name query

#if DEBUG >= 4
	cout<<"NMBIO::askNBNS, trying NBNS\n";
#endif
	
	struct sockaddr_in connectParam; // parameters of the connection
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
	{
#if DEBUG >= 1
		cout<<"NMBIO::askNBNS, cannot allocate socket.\n";
#endif
		errno=NO_RECOVERY;
		return 0;
	}

	connectParam.sin_family = AF_INET;
	connectParam.sin_port = htons(137);	// NameService port
	memcpy(&connectParam.sin_addr, &NBNS, sizeof(NBNS));

	// name, no broadcast, id=0 (important, see below)
	NameQueryPacket query(name,0,0,0,groupFlag);

	uint8* p=query.packet();
#if DEBUG >= 6
	for (int i=0; i<query.getLength(); i++) printf("%X ",p[i]);
	cout<<"\n";
#endif
#ifdef _WIN32
	if (sendto(sock, (const char *)p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#else
	if (sendto(sock, p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#endif
	{
#if DEBUG >= 1
		cout<<"NMBIO::askNBNS, cannot send datagram.\n";
#endif
		errno=NO_RECOVERY;
#ifdef _WIN32
		if (sock) closesocket(sock);
#else
		if (sock) close(sock);
#endif
		return 0;
	}
	delete p;

	uint8 *rawdata=new uint8[1001]; // UDP doesn't use big packet
	int32 queue=0;

	struct timeval tv;
	fd_set rfds;	// from man select...

	// wait response
	FD_ZERO(&rfds);
	FD_SET(sock,&rfds);
	tv.tv_sec = 1;	// wait max 1 sec
	tv.tv_usec = 0;
	// timeout=>exit
#ifdef _WIN32
	if (!select(sock+1, &rfds, 0, 0, &tv)) {errno=HOST_NOT_FOUND; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (!select(sock+1, &rfds, 0, 0, &tv)) {errno=HOST_NOT_FOUND; delete rawdata; if (sock) close(sock); return 0;}
#endif
//	queue=recvfrom(sock, rawdata, 1000, 0, 0, 0);
#ifdef _WIN32
	queue=recv(sock, (char *)rawdata, 1000, 0);
#else
	queue=read(sock, rawdata, 1000);
#endif
#if DEBUG >= 6
	cout<<"queue : "<<queue<<"\n";
	for (int i=0; i<queue; i++) {printf("%X ",rawdata[i]);}
	cout<<"\n";
#endif

#ifdef _WIN32
	if (queue<=0) {errno=NO_RECOVERY; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (queue<=0) {errno=NO_RECOVERY; delete rawdata; if (sock) close(sock); return 0;}
#endif
	rawdata[queue]=0; // barrier, ok : rawdata size=1001
#ifdef _WIN32
	if (queue<4) {errno=NO_RECOVERY; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (queue<4) {errno=NO_RECOVERY; delete rawdata; if (sock) close(sock); return 0;}
#endif
	// we don't care here for the errcode
	if ((rawdata[0]!=0) || (rawdata[1]!=0) || // id=0
#ifdef _WIN32
		((rawdata[2]&0xFD)!=0x85)) {errno=HOST_NOT_FOUND; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
		((rawdata[2]&0xFD)!=0x85)) {errno=HOST_NOT_FOUND; delete rawdata; if (sock) close(sock); return 0;}
#endif
	// ignore fixed bytes
#ifdef _WIN32
	if (queue<12) {errno=NO_RECOVERY; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (queue<12) {errno=NO_RECOVERY; delete rawdata; if (sock) close(sock); return 0;}
#endif
	int len=strlen((char*)(rawdata+12)); // barrier prevents cataclysm
	NBHostEnt *ret=new NBHostEnt;
	ret->NBName=new char[len+1];
	strcpy(ret->NBName,(char*)(rawdata+12));
	ret->name=decodeNBName(ret->NBName,groupFlag);
#ifdef _WIN32
	if (queue<12+len+1+8+2) {errno=NO_RECOVERY; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (queue<12+len+1+8+2) {errno=NO_RECOVERY; delete rawdata; if (sock) close(sock); return 0;}
#endif
	// skip more bytes
	uint16 addrLen=(((uint16)rawdata[12+len+1+8]) << 8)
				|(((uint16)rawdata[12+len+1+8+1]) &0xFF);
#ifdef _WIN32
	if (queue<12+len+1+8+2+addrLen) {errno=NO_RECOVERY; delete rawdata; if (sock) closesocket(sock); return 0;}
#else
	if (queue<12+len+1+8+2+addrLen) {errno=NO_RECOVERY; delete rawdata; if (sock) close(sock); return 0;}
#endif
	uint8 *dat=rawdata+12+len+1+8+2+2;
	addrLen/=6; // flags+ip
	// we should seek here the member names
	// instead of copying the group name if so
	ret->ip=(((uint32)(*dat)&0xFF)<<24)
				|(((uint32)(*(dat+1))&0xFF)<<16)
				|(((uint32)(*(dat+2))&0xFF)<<8)
				|((uint32)(*(dat+3))&0xFF);
#if DEBUG>=5
	cout<<"NBNS answer: name="<<ret->name<<", ip=";
	cout<<(int)((*dat)&0xFF)<<"."<<(int)((*(dat+1))&0xFF)<<"."<<((int)(*(dat+2))&0xFF)<<"."<<(int)((*(dat+3))&0xFF)<<"\n";
#endif

// Do we trust our beloved NBNS ?
// some servers like WINS can keep outdated info for days...
#ifdef OVERKILL // common sense mode on
	// copy IP directly from NBNS answer
	memcpy(&connectParam.sin_addr, dat, 4);
	// now we can free mem
	delete rawdata;

	// name, no broadcast, id=0, same packet than before
	p=query.packet();
#if DEBUG >= 6
	for (int i=0; i<query.getLength(); i++) printf("%X ",p[i]);
	cout<<"\n";
#endif
#ifdef _WIN32
	if (sendto(sock, (const char *)p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#else
	if (sendto(sock, p, query.getLength(), 0, (sockaddr*)&connectParam, sizeof(connectParam))==-1)
#endif
	{
#if DEBUG >= 1
			cout<<"NMBIO::askNBNS, verification, cannot send datagram.\n";
#endif
		errno=NO_RECOVERY;
		delete ret;
#ifdef _WIN32
		if (sock) closesocket(sock);
#else
		if (sock) close(sock);
#endif
		return 0;
	}
	delete p;

	rawdata=new uint8[1001]; // UDP doesn't use big packet
	queue=0;

	// wait response
	FD_ZERO(&rfds);
	FD_SET(sock,&rfds);
	tv.tv_sec = 1;	// wait max 1 sec
	tv.tv_usec = 0;
	// timeout=>exit
#ifdef _WIN32
	if (!select(sock+1, &rfds, 0, 0, &tv)) {errno=HOST_NOT_FOUND; delete rawdata; delete ret; if (sock) closesocket(sock); return 0;}
#else
	if (!select(sock+1, &rfds, 0, 0, &tv)) {errno=HOST_NOT_FOUND; delete rawdata; delete ret; if (sock) close(sock); return 0;}
#endif
//	queue=recvfrom(sock, rawdata, 1000, 0, 0, 0);
#ifdef _WIN32
	queue=recv(sock, (char *)rawdata, 1000, 0);
#else
	queue=read(sock, rawdata, 1000);
#endif
#if DEBUG >= 6
	cout<<"queue : "<<queue<<"\n";
	for (int i=0; i<queue; i++) {printf("%X ",rawdata[i]);}
	cout<<"\n";
#endif

#ifdef _WIN32
	if (queue<=0) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) closesocket(sock); return 0;}
#else
	if (queue<=0) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) close(sock); return 0;}
#endif
	rawdata[queue]=0; // barrier, ok : rawdata size=1001
#ifdef _WIN32
	if (queue<4) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) closesocket(sock); return 0;}
#else
	if (queue<4) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) close(sock); return 0;}
#endif
	// we don't care here for the errcode
	if ((rawdata[0]!=0) || (rawdata[1]!=0) || // id=0
#ifdef _WIN32
		((rawdata[2]&0xFD)!=0x85)) {errno=HOST_NOT_FOUND; delete rawdata; delete ret; if (sock) closesocket(sock); return 0;}
#else
		((rawdata[2]&0xFD)!=0x85)) {errno=HOST_NOT_FOUND; delete rawdata; delete ret; if (sock) close(sock); return 0;}
#endif
	// ignore fixed bytes
#ifdef _WIN32
	if (queue<12) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) closesocket(sock); return 0;}
#else
	if (queue<12) {errno=NO_RECOVERY; delete rawdata; delete ret; if (sock) close(sock); return 0;}
#endif
	len=strlen((char*)(rawdata+12)); // barrier prevents cataclysm
	if (strcasecmp(ret->NBName,(char*)(rawdata+12))) {
		// AARGL, NetBIOS Name Server sends invalid data !
#ifdef _WIN32
		if (sock) closesocket(sock);
#else
		if (sock) close(sock);
#endif
		delete rawdata; delete ret; return 0;
	}
#if DEBUG>=5
	cout<<"Check NBNS answer, host repy: name="<<ret->name<<", ip=";
	cout<<(int)((*dat)&0xFF)<<"."<<(int)((*(dat+1))&0xFF)<<"."<<((int)(*(dat+2))&0xFF)<<"."<<(int)((*(dat+3))&0xFF)<<"\n";
#endif
#endif // OVERKILL
#ifdef _WIN32
	if (sock) closesocket(sock);
#else
	if (sock) close(sock);
#endif
	delete rawdata;
	if (ret)
		cache->add(ret->NBName, ret->name, ret->ip, 0, groupFlag, 120);
	return ret;
}

#endif

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()

