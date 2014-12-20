#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"

class RegCommand : BaseCommand {
public:
	RegCommand();
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();

};

class RegContext {
public:
	HKEY rootKey;
	HKEY currentKey;
	string regPath;
};

class RegListCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegListCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RegOpenCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegOpenCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RegRootCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegRootCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RegDelValCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegDelValCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RegHelpCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegHelpCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class FindRegCommand : BaseCommand {
public:
	FindRegCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	void ProcessLevel(Connection *pConnection, const char *pSubKey, const char *keyword);
	RegContext *_context;
};

class FindRegWriteableCommand : BaseCommand {
public:
	FindRegWriteableCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	void ProcessLevel(Connection *pConnection, const char *pSubKey);
	RegContext *_context;
};



class RegDwordCommand : BaseCommand {
public:
	RegDwordCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};

class RegStrCommand : BaseCommand {
public:
	RegStrCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};

class RegCreateKeyCommand : BaseCommand {
public:
	RegCreateKeyCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};

class RegDeleteKeyCommand : BaseCommand {
public:
	RegDeleteKeyCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};

class RegDeleteTreeCommand : BaseCommand {
public:
	RegDeleteTreeCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};
