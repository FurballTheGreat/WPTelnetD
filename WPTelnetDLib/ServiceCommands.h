#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"

class ScCommand : public Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class ScStartCommand : public Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class ScStopCommand : public Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class ScListCommand : public Command {
	bool _drivers;
public:
	ScListCommand(bool pDrivers);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};
