#include "pch.h"
#include<string>
#include "Connection.h"
#include "CommandProcessor.h"
#include<vector>

class CertsCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};