#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"


class BaseFileCommand : BaseCommand {
protected:
	virtual void ProcessFile(Connection *pConnection, WIN32_FIND_DATAA pFileInfo, string pFileName) = 0;
	virtual bool ProcessCommandLine(Connection *pConnection, ParsedCommandLine *pCmdLine) = 0;
	virtual bool PrintSyntax(Connection *pConnection) = 0;
public:
	BaseFileCommand();
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	virtual string GetName() = 0;
};

class ListAclsCommand : BaseFileCommand {
protected:
	void ProcessFile(Connection *pConnection, WIN32_FIND_DATAA pFileInfo, string pFileName);
	bool ProcessCommandLine(Connection *pConnection, ParsedCommandLine *pCmdLine);
	bool PrintSyntax(Connection *pConnection);
public:
	string GetName();
};

class TypeCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};



class CopyCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


class DeleteCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


class MoveCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};


class AttribCommand : BaseCommand {
public:
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};
