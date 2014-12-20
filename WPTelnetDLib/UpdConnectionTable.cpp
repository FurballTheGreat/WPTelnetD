#include "pch.h"
#include "UdpConnectionTable.h"

#include <iostream>
using namespace std;

#ifdef PHONE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <WPIPHldApi.h>

#endif
#include <stdio.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

UdpConnectionEntry::UdpConnectionEntry(PMIB_UDPROW_OWNER_MODULE pUdpRow) {
	char szLocalAddr[128];
	char szRemoteAddr[128];

	struct in_addr IpAddr;
	IpAddr.S_un.S_addr = (u_long) pUdpRow->dwLocalAddr;
	strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
	_localPort = ntohs((u_short)pUdpRow->dwLocalPort);
	_localHost = szLocalAddr;
	_ownerPID = pUdpRow->dwOwningPid;
	PTCPIP_OWNER_MODULE_BASIC_INFO  info = (PTCPIP_OWNER_MODULE_BASIC_INFO)malloc(1000);
	 DWORD size = 1000;
	 memset(info,0, 1000);
	DWORD err= GetOwnerModuleFromUdpEntry(pUdpRow,  TCPIP_OWNER_MODULE_INFO_BASIC,info, &size);
	if(err==NO_ERROR && info->pModuleName!=NULL) {
	char buf[1024];
	wcstombs ( buf, info->pModuleName, sizeof(buf) );
	_owner = string(buf);
	wcstombs ( buf, info->pModulePath,sizeof(buf) );
	_ownerPath  = string(buf);
	} else {
		_owner = "Unknown";
		_ownerPath="";
	}
}

std::string UdpConnectionEntry::GetLocalHost(){
	return _localHost;
}

int UdpConnectionEntry::GetLocalPort(){
	return _localPort;
}

int UdpConnectionEntry::GetOwnerPID(){
	return _ownerPID;
}
std::string UdpConnectionEntry::GetOwningModule(){
	return _owner;
}

std::string UdpConnectionEntry::GetOwningModulePath(){
	return _ownerPath;
}


UdpConnectionTable::UdpConnectionTable() {
	PMIB_UDPTABLE_OWNER_MODULE udpTable;
	DWORD size = 0;
	DWORD retVal = 0;

	int i;

	udpTable = ( MIB_UDPTABLE_OWNER_MODULE *) MALLOC(sizeof ( MIB_UDPTABLE_OWNER_MODULE));
	if (udpTable == NULL) 
		throw new runtime_error("Error allocating memory");
	

	size = sizeof ( MIB_UDPTABLE_OWNER_MODULE);

	do {
		if ((retVal =  GetExtendedUdpTable(udpTable, &size, TRUE,AF_INET,UDP_TABLE_OWNER_MODULE,NULL)) ==
			ERROR_INSUFFICIENT_BUFFER) {
				FREE(udpTable);
				udpTable = ( MIB_UDPTABLE_OWNER_MODULE *) MALLOC(size);
				if(udpTable==NULL) 
					throw new runtime_error("Error allocating table memory");
				
		}
	} while(retVal==ERROR_INSUFFICIENT_BUFFER);

	if ((retVal =  GetExtendedUdpTable(udpTable, &size, TRUE,AF_INET,UDP_TABLE_OWNER_MODULE,NULL)) == NO_ERROR) {
		for (i = 0; i < (int) udpTable->dwNumEntries; i++) {
			push_back(UdpConnectionEntry(&udpTable->table[i]));
		}
	} else {
		string str = "GetUdpTable failed with " + retVal;

		FREE(udpTable);
		throw new runtime_error(str.c_str());		
	}
}

