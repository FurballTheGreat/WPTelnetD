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