#pragma once
#include "Networking.h"
#include<string>

class TerminalHelper
{
private:
	Connection *_connection;
public:
	TerminalHelper(Connection *pConnection);

	void WriteUnderlined(std::string pLine);

	~TerminalHelper();
};

