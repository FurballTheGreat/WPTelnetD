#pragma once 

#include "pch.h"
#include<string>
#include "CommandProcessor.h"
#include "Networking.h"
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

class RegCommand : BaseCommand {
public:
	RegCommand();
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();

};

void PrintRegValueLine(Connection* pConnection, string pValueName, DWORD pValType, BYTE *pValBuf, DWORD pValSize);
void PrintRegValueLineW(Connection* pConnection, wstring pValueName, DWORD pValType, BYTE *pValBuf, DWORD pValSize);


class RegListCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegListCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
};

class RegAclCommand : BaseCommand {
private:
	RegContext *_context;
public:
	RegAclCommand(RegContext *pContext);
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




class RegSetCommand : BaseCommand {
public:
	RegSetCommand(RegContext *pContext);
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

class RegImportCommand : BaseCommand {
public:
	RegImportCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};

class RegExportCommand : BaseCommand {
public:
	RegExportCommand(RegContext *pContext);
	void ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine);
	string GetName();
private:
	RegContext *_context;
};
