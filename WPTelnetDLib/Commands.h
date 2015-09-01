#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"


using namespace std;
class HelpCommand : public Command {
	vector<Command*> *_commands;
	string _intro;
public:
	HelpCommand(string pIntro, vector<Command*> *pCommands);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class EnvCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class ExitCommand : public Command {
	bool _exiting;
	string _description;
public:
	ExitCommand(string pDescription);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
	bool IsExiting();
};

class EchoCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class ListPrivsCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};




