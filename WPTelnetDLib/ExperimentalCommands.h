#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"

class ProvXmlCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();

};


class TestCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();

};

class TestCommand2 : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();

};