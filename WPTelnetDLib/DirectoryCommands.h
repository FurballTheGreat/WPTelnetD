#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"

class CdCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


class MkdirCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RmdirCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class DirCommand : BaseCommand {
private:
	BOOL GetLastWriteTime(WIN32_FIND_DATAA *data, char *string, DWORD dwSize);
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};
