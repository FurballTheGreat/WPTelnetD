#include "pch.h"
#include<string>
#include "Networking.h"
#include "CommandProcessor.h"
#include<vector>

class CertsCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};