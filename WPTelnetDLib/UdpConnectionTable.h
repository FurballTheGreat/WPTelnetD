#pragma once
#include "pch.h"
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include <string>
#include <iostream>
#include <stdexcept>
#ifdef PHONE
#include <WPIPHldApi.h>
#endif
class UdpConnectionEntry {
private:
	int _localPort;
	std::string _localHost;
	std::string _owner;
	std::string _ownerPath;	
	int _ownerPID;
public:
	UdpConnectionEntry(PMIB_UDPROW_OWNER_MODULE pUdpRow);

	int GetLocalPort();
	std::string GetLocalHost();
	int GetOwnerPID();
	std::string GetOwningModule();
	std::string GetOwningModulePath();
};

class UdpConnectionTable : public std::vector<UdpConnectionEntry> {
public:
	UdpConnectionTable();
};