#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"


class EnumWindowsCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class PostMessageCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

#ifdef PHONE
class ToastCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};
#endif
