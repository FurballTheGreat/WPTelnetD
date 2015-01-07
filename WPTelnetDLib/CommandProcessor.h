#pragma once

#include "pch.h"
#include<string>
#include "Networking.h"
#include<vector>

using namespace std;

typedef void (*PPRINT_PROMPT)(Connection*, LPVOID);


class IExecutionContext
{
public:
	virtual string GetVariable(string pName) = 0;
	virtual void SetVariable(string pName, string pValue) = 0;
};

class ParsedCommandLine
{
private:
	vector<string> _arguments ;
	string _command;
	string _originalLine;
	IExecutionContext *_host;
public:
	ParsedCommandLine(string pCommandLine, IExecutionContext *pHost) ;

	string GetName();
	string GetRaw();
	vector<string> GetArgs();
	IExecutionContext *GetHost();
	ParsedCommandLine GetParametersAsLine();
};



class BaseCommand 
{
public:
	BaseCommand();
	virtual void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCommandLine) = 0;
	virtual string GetName() = 0;
};

class CommandProcessor 
{
private:
	vector<BaseCommand *> *_commands;
	Connection *_connection;
public:
	CommandProcessor(vector<BaseCommand *> *pCommands,Connection *pConnection);
	bool ProcessCommandLine(ParsedCommandLine *pLine);
};
