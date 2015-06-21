 #pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"


class BaseFileCommand : Command {
protected:
	virtual void ProcessFile(IConsole *pConsole, WIN32_FIND_DATAA pFileInfo, string pFileName) = 0;
	virtual bool ProcessCommandLine(IConsole *pConsole, ParsedCommandLine *pCmdLine) = 0;
	virtual bool PrintSyntax(IConsole *pConsole) = 0;
public:
	BaseFileCommand();
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	virtual CommandInfo GetInfo() = 0;
};

class ListAclsCommand : BaseFileCommand {
protected:
	void ProcessFile(IConsole *pConsole, WIN32_FIND_DATAA pFileInfo, string pFileName);
	bool ProcessCommandLine(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	bool PrintSyntax(IConsole *pConsole);
public:
	CommandInfo GetInfo();
};

class TypeCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};



class CopyCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};


class DeleteCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};



class MoveCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};


class AttribCommand : Command {
public:
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};
