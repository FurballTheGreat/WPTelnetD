#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Console.h"
class RegContext {
public:
	HKEY rootKey;
	HKEY currentKey;
	string regPath;
};

class RegPathParam
{
private:
	RegContext *_context;
	HKEY _key,_root;
	string _valueName;
	bool _closeKey;
	wstring _path;

public:
	RegPathParam(RegContext*pContext, string pValue, bool pIncludeValue, bool pOpen);

	~RegPathParam();

	HKEY GetKey();
	HKEY GetRoot();

	void SetValue(
		_In_ DWORD dwType,
		_In_reads_bytes_opt_(cbData) CONST BYTE * lpData,
		_In_ DWORD cbData
		);
	void DeleteValue();

	wstring GetPath();
	wstring GetValName();

	void CreateKey();

	void DeleteKey();

	void DeleteTree();

	void GetKeySecurity(
		_In_ SECURITY_INFORMATION SecurityInformation,
		_Out_writes_bytes_opt_(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
		_Inout_ LPDWORD lpcbSecurityDescriptor
		);

	bool EnumKey(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
		_Inout_ LPDWORD lpcchName
		);

	bool EnumValue(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
		_Inout_ LPDWORD lpcchValueName,

		_Out_opt_ LPDWORD lpType,
		_Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
		_Inout_opt_ LPDWORD lpcbData
		);
};

class RegCommand : Command {
public:
	RegCommand();
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();

};

void PrintRegValueLine(IConsole *pConsole, string pValueName, DWORD pValType, BYTE *pValBuf, DWORD pValSize);
void PrintRegValueLineW(IConsole *pConsole, wstring pValueName, DWORD pValType, BYTE *pValBuf, DWORD pValSize);


class RegListCommand : Command {
private:
	RegContext *_context;
public:
	RegListCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RegAclCommand : Command {
private:
	RegContext *_context;
public:
	RegAclCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RegOpenCommand : Command {
private:
	RegContext *_context;
public:
	RegOpenCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RegRootCommand : Command {
private:
	RegContext *_context;
public:
	RegRootCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RegDelValCommand : Command {
private:
	RegContext *_context;
public:
	RegDelValCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class RegHelpCommand : Command {
private:
	RegContext *_context;
public:
	RegHelpCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
};

class FindRegCommand : Command {
public:
	FindRegCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	void ProcessLevel(IConsole *pConsole, const char *pSubKey, const char *keyword);
	RegContext *_context;
};

class FindRegWriteableCommand : Command {
public:
	FindRegWriteableCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	void ProcessLevel(IConsole *pConsole, const char *pSubKey);
	RegContext *_context;
};




class RegSetCommand : Command {
public:
	RegSetCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};

class RegCreateKeyCommand : Command {
public:
	RegCreateKeyCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};

class RegDeleteKeyCommand : Command {
public:
	RegDeleteKeyCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};

class RegDeleteTreeCommand : Command {
public:
	RegDeleteTreeCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};

class RegImportCommand : Command {
public:
	RegImportCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};

class RegExportCommand : Command {
public:
	RegExportCommand(RegContext *pContext);
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine);
	CommandInfo GetInfo();
private:
	RegContext *_context;
};
