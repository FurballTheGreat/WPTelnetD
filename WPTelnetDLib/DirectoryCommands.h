#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"

class CdCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};


class MkdirCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RmdirCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class DirCommand : Command {
private:
	BOOL GetLastWriteTime(WIN32_FIND_DATAA *data, char *string, DWORD dwSize);
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};
