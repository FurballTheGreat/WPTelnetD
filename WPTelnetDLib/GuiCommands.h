#pragma once

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"


class EnumWindowsCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class PostMessageCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

#ifdef PHONE
class ToastCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};
#endif
