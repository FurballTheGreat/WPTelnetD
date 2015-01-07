#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"


using namespace std;
class HelpCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class EnvCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class EchoCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class ListPrivsCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


