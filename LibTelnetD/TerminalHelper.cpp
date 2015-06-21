#include "stdafx.h"
#include "TerminalHelper.h"
#include<string>
#include "Console.h"

TerminalHelper::TerminalHelper(IConsole *pConsole)
{
	_console = pConsole;
}


TerminalHelper::~TerminalHelper()
{
}

void TerminalHelper::WriteUnderlined(std::string pLine){
	std::string underline = "";
	for (int i = 0; i < pLine.length(); i++)
		underline += "-";
	_console->WriteLine(pLine);
	_console->WriteLine(underline);
	_console->WriteLine("");
}


std::string GetErrorAsString(HRESULT pError) {
	char buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		pError,
		0,
		buffer + strlen(buffer),
		sizeof(buffer) - strlen(buffer),
		NULL) == 0) {
		for (auto i = 0; i < strlen(buffer); i++)
			if (buffer[i] == '\n' || buffer[i] == '\r')
				buffer[i] = 0;
		return std::string(buffer) + " (" + std::to_string(pError) + ")";
	}
	else
		return std::string("ERROR: Unknown Error - WinCode ") + std::to_string(pError);
}

std::string GetLastErrorAsString()
{
	return GetErrorAsString(GetLastError());
}