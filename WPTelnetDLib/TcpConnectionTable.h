#pragma once

#ifdef PHONE
#include <WPIPHldApi.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include <string>
#include <iostream>
#include <stdexcept>

class TcpConnectionEntry {
private:
	std::string _localHost;
	int _localPort;
	std::string _remoteHost;
	int _remotePort;
	int _ownerPID;
	std::string _state;
	std::string _owner;
	std::string _ownerPath;
public:
	TcpConnectionEntry(PMIB_TCPROW_OWNER_MODULE pTcpRow);
	std::string GetLocalHost();
	std::string GetRemoteHost();
	int GetLocalPort();
	int GetRemotePort();
	int GetOwnerPID();
	std::string GetStateString();
	std::string GetOwningModule();
	std::string GetOwningModulePath();
};

class TcpConnectionTable : public std::vector<TcpConnectionEntry> {
public:
	TcpConnectionTable();
};