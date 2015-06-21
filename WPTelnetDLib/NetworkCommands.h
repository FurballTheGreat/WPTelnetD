#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"

class NetstatCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};


class DownloadCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);

	CommandInfo GetInfo();
};


class RunFromCommand : Command {
private:
	IExecutionContext *_executionContext;
	CommandProcessor *_processor;
public:
	RunFromCommand(IExecutionContext *pExecutionContext, CommandProcessor *pProcessor);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);

	CommandInfo GetInfo();
};

