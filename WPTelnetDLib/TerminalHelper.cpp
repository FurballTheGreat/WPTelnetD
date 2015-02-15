#include "pch.h"
#include "TerminalHelper.h"
#include<string>

TerminalHelper::TerminalHelper(Connection *pConnection)
{
	_connection = pConnection;
}


TerminalHelper::~TerminalHelper()
{
}

void TerminalHelper::WriteUnderlined(std::string pLine){
	std::string underline = "";
	for (int i = 0; i < pLine.length(); i++)
		underline += "-";
	_connection->WriteLine(pLine);
	_connection->WriteLine(underline);
	_connection->WriteLine("");
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
		NULL) == 0)
		return std::string(buffer) + " (" + std::to_string(pError) + ")";
	else
		return std::string("ERROR: Unknown Error - WinCode ") + std::to_string(pError);
}