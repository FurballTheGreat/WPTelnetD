#pragma once

#include "pch.h"
#include<string>
#include "Networking.h"
#include<vector>

using namespace std;

typedef void (*PPRINT_PROMPT)(Connection*, LPVOID);

class ParsedCommandLine
{
private:
	vector<string> _arguments ;
	string _command;
	string _originalLine;
public:
	ParsedCommandLine(string pCommandLine) ;

	string GetName();
	string GetRaw();
	vector<string> GetArgs();

	ParsedCommandLine GetParametersAsLine();
};

class ICommandProcessorHost 
{

	public:
		virtual void PrintPrompt(Connection *pConnection) = 0;
		virtual void UnhandledLine(Connection *pConnection, string pLine) = 0;
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
	vector<BaseCommand *> _commands;
	ICommandProcessorHost *_host;
public:
	CommandProcessor(vector<BaseCommand *> pCommands, ICommandProcessorHost *pHost);
	void PrintPrompt(Connection *pConnection);


	bool ProcessData(Connection *pConnection,const char *pLine);
	bool ProcessCommandLine(Connection *pConnection, ParsedCommandLine *pLine);
};
