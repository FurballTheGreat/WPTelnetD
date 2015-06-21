#pragma once

#include "stdafx.h"
#include<string>
#include "Console.h"
#include<vector>


using namespace std;


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

class CommandInfo
{
private:
	string _name;
	string _params;
	string _description;
public:
	CommandInfo(string pName, string pParams, string pDescription);

	string GetName();
	string GetParams();
	string GetDescription();
};


class Command 
{
public:
	Command();
	virtual void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCommandLine) = 0;
	virtual CommandInfo GetInfo() = 0;
};

class CommandProcessor 
{
private:
	vector<Command *> *_commands;
	
public:
	CommandProcessor(vector<Command *> *pCommands);
	bool ProcessCommandLine(IConsole *_console, ParsedCommandLine *pLine);
};
