#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"

class NetstatCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


class DownloadCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);

	string GetName();
};

