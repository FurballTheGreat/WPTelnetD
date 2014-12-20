#include "pch.h"
#include "TcpConnectionTable.h"
#ifdef PHONE
#include <WPIPHldApi.h>
#endif
#include <iostream>
using namespace std;

#ifdef PHONE
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <stdio.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

TcpConnectionEntry::TcpConnectionEntry(PMIB_TCPROW_OWNER_MODULE pTcpRow) {
	char szLocalAddr[128];
	char szRemoteAddr[128];

	struct in_addr IpAddr;
	IpAddr.S_un.S_addr = (u_long) pTcpRow->dwLocalAddr;
	strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
	IpAddr.S_un.S_addr = (u_long) pTcpRow->dwRemoteAddr;
	strcpy_s(szRemoteAddr, sizeof (szRemoteAddr), inet_ntoa(IpAddr));
	_localHost = std::string(szLocalAddr);
	_remoteHost = std::string(szRemoteAddr);
	_localPort = ntohs((u_short)pTcpRow->dwLocalPort);
	_remotePort = ntohs((u_short)pTcpRow->dwRemotePort);
	_ownerPID = pTcpRow->dwOwningPid;

	switch (pTcpRow->dwState) {
	case MIB_TCP_STATE_CLOSED:
		_state = "CLOSED";
		break;
	case MIB_TCP_STATE_LISTEN:
		_state = "LISTEN";
		break;
	case MIB_TCP_STATE_SYN_SENT:
		_state = "SYN-SENT";
		break;
	case MIB_TCP_STATE_SYN_RCVD:
		_state = "SYN-RECEIVED";;
		break;
	case MIB_TCP_STATE_ESTAB:
		_state = "ESTABLISHED";
		break;
	case MIB_TCP_STATE_FIN_WAIT1:
		_state = "FIN-WAIT-1";
		break;
	case MIB_TCP_STATE_FIN_WAIT2:
		_state = "FIN-WAIT-2";
		break;
	case MIB_TCP_STATE_CLOSE_WAIT:
		_state = "CLOSE-WAIT";
		break;
	case MIB_TCP_STATE_CLOSING:
		_state = "CLOSING";
		break;
	case MIB_TCP_STATE_LAST_ACK:
		_state = "LAST-ACK";
		break;
	case MIB_TCP_STATE_TIME_WAIT:
		_state = "TIME-WAIT";
		break;
	case MIB_TCP_STATE_DELETE_TCB:
		_state = "DELETE-TCB";
		break;
	default:
		char buf[100];
		sprintf_s(buf,"UNKNOWN (%d)",pTcpRow->dwState);
		break;
	}

	 PTCPIP_OWNER_MODULE_BASIC_INFO  info = (PTCPIP_OWNER_MODULE_BASIC_INFO)malloc(1000);
	 DWORD size = 1000;
	 memset(info,0, 1000);
	DWORD err= GetOwnerModuleFromTcpEntry(pTcpRow,  TCPIP_OWNER_MODULE_INFO_BASIC,info, &size);
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
std::string TcpConnectionEntry::GetRemoteHost(){
	return _remoteHost;
}
std::string TcpConnectionEntry::GetLocalHost(){
	return _localHost;
}
int TcpConnectionEntry::GetRemotePort(){
	return _remotePort;
}
int TcpConnectionEntry::GetLocalPort(){
	return _localPort;
}

int TcpConnectionEntry::GetOwnerPID(){
	return _ownerPID;
}
std::string TcpConnectionEntry::GetStateString(){
	return _state;
}

std::string TcpConnectionEntry::GetOwningModule(){
	return _owner;
}

std::string TcpConnectionEntry::GetOwningModulePath(){
	return _ownerPath;
}

TcpConnectionTable::TcpConnectionTable() {
	PMIB_TCPTABLE_OWNER_MODULE tcpTable;
	DWORD size = 0;
	DWORD returnValue = 0;

	int i;

	tcpTable = (MIB_TCPTABLE_OWNER_MODULE *) MALLOC(sizeof (MIB_TCPTABLE_OWNER_MODULE));
	if (tcpTable == NULL) 	
		throw new runtime_error("Error allocating memory");
	

	size = sizeof (MIB_TCPTABLE_OWNER_MODULE);

	do {
		if ((returnValue = GetExtendedTcpTable(tcpTable, &size,true,AF_INET,TCP_TABLE_OWNER_MODULE_ALL,NULL)) ==
			ERROR_INSUFFICIENT_BUFFER) {
				FREE(tcpTable);
				tcpTable = (MIB_TCPTABLE_OWNER_MODULE *) MALLOC(size);
				if(tcpTable==NULL) 					
					throw new runtime_error("Error allocating memory");				
		}
	} while(returnValue==ERROR_INSUFFICIENT_BUFFER);

	if ((returnValue = GetExtendedTcpTable(tcpTable, &size,true,AF_INET,TCP_TABLE_OWNER_MODULE_ALL,NULL)) == NO_ERROR) {
		for (i = 0; i < (int) tcpTable->dwNumEntries; i++) {
			push_back(TcpConnectionEntry(&tcpTable->table[i]));
		}
	} else {
		string str = "GetTcpTable failed with " + returnValue;
		
		FREE(tcpTable);
		throw new runtime_error(str.c_str());
	}
}

