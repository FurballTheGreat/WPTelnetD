#pragma once
#include "Console.h"
#include<string>

class TerminalHelper
{
private:
	IConsole *_console;
public:
	TerminalHelper(IConsole *pConsole);

	void WriteUnderlined(std::string pLine);

	~TerminalHelper();
};

std::string GetErrorAsString(HRESULT pError);
std::string GetLastErrorAsString();