#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"

class ProvXmlCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();

};


class TestCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();

};

class TestCommand2 : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();

};