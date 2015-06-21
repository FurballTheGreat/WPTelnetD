#include "pch.h"
#include "RegistryCommands.h"
#include "AclHelpers.h"
#include "TerminalHelper.h"
#include "Commands.h"


RegCommand::RegCommand() {
	
}

HKEY ParseKey(const char *pStrKey) {
	if (!_strcmpi(pStrKey, "HKLM"))				     return HKEY_LOCAL_MACHINE;
	if (!_strcmpi(pStrKey, "HKEY_LOCAL_MACHINE"))    return HKEY_LOCAL_MACHINE;
	if (!_strcmpi(pStrKey, "HKCR"))				     return HKEY_CLASSES_ROOT;
	if (!_strcmpi(pStrKey, "HKEY_CLASSES_ROOT"))     return HKEY_CLASSES_ROOT;
	if (!_strcmpi(pStrKey, "HKCU"))			        return HKEY_CURRENT_USER;
	if (!_strcmpi(pStrKey, "HKEY_CURRENT_USER"))     return HKEY_CURRENT_USER;
	if (!_strcmpi(pStrKey, "HKU"))				    return HKEY_USERS;
	if (!_strcmpi(pStrKey, "HKEY_USERS"))		    return HKEY_USERS;
	if (!_strcmpi(pStrKey, "HKCC"))				    return HKEY_CURRENT_CONFIG;
	if (!_strcmpi(pStrKey, "HKEY_CURRENT_CONFIG"))   return HKEY_CURRENT_CONFIG;
	if (!_strcmpi(pStrKey, "HKPD"))                  return HKEY_PERFORMANCE_DATA;
	if (!_strcmpi(pStrKey, "HKEY_PERFORMANCE_DATA")) return HKEY_PERFORMANCE_DATA;
	return NULL;
}

void ProcessChangeRegPath(RegContext* pContext, IConsole *pConsole,string pPath){
	HKEY key = NULL;
	DWORD error = ERROR_SUCCESS;
	string newPath = "";
	if (pPath == "")
		return;
	if (pPath == "\\") {
		key = pContext->rootKey;
	}else
	if (pPath == ".."){
		std::size_t found = pContext->regPath.find_last_of("\\");
		if (found == std::string::npos || found == 0)
			key = pContext->rootKey;
		else {
			newPath = pContext->regPath.substr(0,found );
			error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_WRITE | KEY_READ, &key);
			if (error != ERROR_SUCCESS)
				error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_READ, &key);
			if (error != ERROR_SUCCESS)
				error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_WRITE, &key);
		}
	}
	else {
		if (pContext->regPath != "")
			newPath = pContext->regPath + "\\" + pPath;
		else
			newPath = pPath;
		error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_WRITE | KEY_READ, &key);
		if (error != ERROR_SUCCESS)
			error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_READ, &key);
		
		if (error != ERROR_SUCCESS)
			error = RegOpenKeyExA(pContext->rootKey, newPath.c_str(), 0, KEY_WRITE, &key);

		if (error != ERROR_SUCCESS) {
			error = RegOpenKeyExA(pContext->rootKey, pPath.c_str(), 0, KEY_WRITE | KEY_READ, &key);
			if (error != ERROR_SUCCESS)
				error = RegOpenKeyExA(pContext->rootKey, pPath.c_str(), 0, KEY_READ, &key);

			if (error != ERROR_SUCCESS)
				error = RegOpenKeyExA(pContext->rootKey, pPath.c_str(), 0, KEY_WRITE, &key);
			newPath = pPath;
		}
	}


	if (error != ERROR_SUCCESS){
		pConsole->WriteLine("Failed to open registry key %d", error);
		return;
	}

	if (pContext->currentKey != pContext->rootKey)
		RegCloseKey(pContext->currentKey);
	pContext->regPath = newPath;
	pContext->currentKey = key;
}



	RegPathParam::RegPathParam(RegContext*pContext, string pValue, bool pIncludeValue, bool pOpen) {
		_context = pContext;
		_closeKey = false;
		if (pValue.find_first_of('\\') == std::string::npos) {
			_root = _context->rootKey;
			_path = wstring(_context->regPath.begin(), _context->regPath.end());
			_key = _context->currentKey;
			_valueName = pValue;
			return;
		}

		if (pValue.find_first_of('###') == 0) {
			_root = _context->rootKey;
			_path = wstring(_context->regPath.begin(), _context->regPath.end());
			_key = _context->currentKey;
			_valueName = pValue.substr(3, string::npos);;
			return;
		}

		string rootName = pValue.substr(0, pValue.find_first_of('\\'));
		_root = ParseKey(rootName.c_str());
		pValue = pValue.substr(pValue.find_first_of('\\') + 1, string::npos);
		if (_root == NULL) {
			throw invalid_argument(rootName + " is not a valid root");
		}
		string path;
		if (pIncludeValue){
			if (pValue.find_last_of('\\') == std::string::npos) {
				throw invalid_argument("A full value path is in the form HKLM\PATH\ValueName");
			}
			path = pValue.substr(0, pValue.find_last_of('\\'));
			_valueName = pValue.substr(pValue.find_last_of('\\') + 1, string::npos);
		}
		else
			path = pValue;

		_path = wstring(path.begin(), path.end());
		if (pOpen) {
			DWORD error = RegOpenKeyExA(_root, path.c_str(), NULL, KEY_WRITE | KEY_READ, &_key);
			if (error == ERROR_SUCCESS){
				_closeKey = true;
				return;
			}
			error = RegOpenKeyExA(_root, path.c_str(), NULL, KEY_READ, &_key);
			if (error != ERROR_SUCCESS) {
				throw invalid_argument(string("Error opening key ") + to_string(error));
			}
			_closeKey = true;
		}
	}

	RegPathParam::~RegPathParam() {
		if (_closeKey){
			RegCloseKey(_key);
		}
	}

	HKEY RegPathParam::GetKey() {
		return _key;
	}

	HKEY RegPathParam::GetRoot() {
		return _root;
	}

	wstring RegPathParam::GetPath() {
		return _path;
	}
	wstring RegPathParam::GetValName() {
		return wstring(_valueName.begin(), _valueName.end());
	}
	void RegPathParam::SetValue(
		_In_ DWORD dwType,
		_In_reads_bytes_opt_(cbData) CONST BYTE * lpData,
		_In_ DWORD cbData
		){
		LSTATUS result = RegSetValueExA(_key, _valueName.c_str(), NULL, dwType, lpData, cbData);
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegSetValueExA " + to_string(result));
	}

	void RegPathParam::DeleteValue() {
		LSTATUS result = RegDeleteValueA(_key, _valueName.c_str());
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteValueA " + to_string(result));
	}

	void RegPathParam::CreateKey() {
		HKEY resultKey;
		DWORD result = RegCreateKeyExA(_key, _valueName.c_str(), 0, NULL, 0, NULL, NULL, &resultKey, NULL);
		RegCloseKey(resultKey);
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegCreateKeyExA " + to_string(result));
	}

	void RegPathParam::DeleteKey() {
		string keyName = _valueName;
		wstring wideName = wstring(keyName.begin(), keyName.end());
		
		DWORD result= RegDeleteKeyExW(_key, wideName.c_str(), 0, NULL);
	
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteKeyExW " + to_string(result));
	}

	void RegPathParam::DeleteTree() {
		string keyName = _valueName;
		wstring wideName = wstring(keyName.begin(), keyName.end());
		DWORD result = RegDeleteTreeW(_context->currentKey, wideName.c_str());
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteTreeW " + to_string(result));
	}

	void RegPathParam::GetKeySecurity(
		_In_ SECURITY_INFORMATION SecurityInformation,
		_Out_writes_bytes_opt_(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
		_Inout_ LPDWORD lpcbSecurityDescriptor
		) {
		bool sizeCheck = *lpcbSecurityDescriptor;
		LSTATUS result = RegGetKeySecurity(_key, SecurityInformation, pSecurityDescriptor, lpcbSecurityDescriptor);
		if (result != ERROR_SUCCESS && sizeCheck)
			throw invalid_argument("Error calling RegGetKeySecurity " + to_string(result));
	}

	bool RegPathParam::EnumKey(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
		_Inout_ LPDWORD lpcchName
		) {
		LSTATUS result = RegEnumKeyExA(_key, dwIndex, lpName, lpcchName, 0, NULL, NULL, NULL);
		if (result == ERROR_NO_MORE_ITEMS)
			return false;
		if (result == ERROR_SUCCESS)
			return true;
		throw invalid_argument("Error calling RegEnumKeyExA " + to_string(result));
	}

	bool RegPathParam::EnumValue(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
		_Inout_ LPDWORD lpcchValueName,

		_Out_opt_ LPDWORD lpType,
		_Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
		_Inout_opt_ LPDWORD lpcbData
		) {
		LSTATUS result = RegEnumValueA(_key,dwIndex,lpValueName, lpcchValueName, 0,lpType,lpData, lpcbData);
		if (result == ERROR_NO_MORE_ITEMS)
			return false;
		if (result == ERROR_SUCCESS)
			return true;
		throw invalid_argument("Error calling RegEnumValueA " + to_string(result));
	}



class RecursiveRegOperation

{
	
private:
	RegPathParam *_path;

	void ProcessLevel(string pSubKey) {
		bool isMatch = false;
		HKEY key;
	
		DWORD error = RegOpenKeyExA(_path->GetKey(), pSubKey.length() ? pSubKey.c_str() : NULL, 0, KEY_READ, &key);

		if (error != ERROR_SUCCESS){
			return;
		}

		int index = 0;
		char keyName[1024];
		DWORD keyNameLength;
		DWORD RC = 0;
		std::list<string> subKeys;
		string basePath = !pSubKey.length() ? "" : string(pSubKey) + "\\";
		while (RC != ERROR_NO_MORE_ITEMS) {
			keyNameLength = sizeof(keyName);
			RC = RegEnumKeyExA(key, index, keyName, &keyNameLength, 0, NULL, NULL, NULL);
			
			subKeys.push_back(basePath + keyName);
			index++;
		}
		RegCloseKey(key);
		
		subKeys.unique();
		for each (string sub in subKeys)
		{

			ProcessLevel(sub);
		}
	}
public:
	RecursiveRegOperation(RegPathParam *pPath) {
		
	}

	~RecursiveRegOperation() {
	
	}
};


void PrintPrompt(IConsole *pConsole, RegContext *pContext) {

	if (pContext->rootKey == HKEY_LOCAL_MACHINE) pConsole->Write("HKLM\\");
	else if (pContext->rootKey == HKEY_CLASSES_ROOT) pConsole->Write("HKCR\\");
	else if (pContext->rootKey == HKEY_CURRENT_USER) pConsole->Write("HKCU\\");
	else if (pContext->rootKey == HKEY_USERS) pConsole->Write("HKU\\");
	else if (pContext->rootKey == HKEY_CURRENT_CONFIG) pConsole->Write("HKCC\\");
	else if (pContext->rootKey == HKEY_PERFORMANCE_DATA) pConsole->Write("HKPD\\");

	pConsole->Write("%s>", pContext->regPath.c_str());
}

void RegCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	RegContext *context = new RegContext();
	context->rootKey = HKEY_LOCAL_MACHINE;
	context->currentKey = HKEY_LOCAL_MACHINE;
	context->regPath = "";
	vector<Command *> commands = vector<Command *>();
	ExitCommand exit("Exit the registry editor.");
	commands.push_back((Command*)new HelpCommand("A path can also be opened by just typing it. '..' may be used to goto the parent key and a subkey can be opened by just specifying its name.",
												 &commands));
	commands.push_back((Command*)&exit);
	commands.push_back((Command*)new RegListCommand(context));
	commands.push_back((Command*)new RegOpenCommand(context));
	commands.push_back((Command*)new RegRootCommand(context));
	commands.push_back((Command*)new FindRegCommand(context));
	commands.push_back((Command*)new FindRegWriteableCommand(context));
	commands.push_back((Command*)new RegSetCommand(context));
	commands.push_back((Command*)new RegDelValCommand(context));
	commands.push_back((Command*)new RegDeleteKeyCommand(context));
	commands.push_back((Command*)new RegCreateKeyCommand(context));
	commands.push_back((Command*)new RegDeleteTreeCommand(context));
	commands.push_back((Command*)new RegExportCommand(context));
	commands.push_back((Command*)new RegImportCommand(context));
	commands.push_back((Command*)new RegAclCommand(context));
	if (pCmdLine->GetArgs().size() == 1) {
		CommandProcessor processor = CommandProcessor(&commands);
		pConsole->WriteLine("Registry Editor - Type HELP for assistance.");
		

		while (!exit.IsExiting()) {
			PrintPrompt(pConsole, context);
			pConsole->SetForeground(Yellow);
			pConsole->SetBold(true);
			std::string line = pConsole->ReadLine();
			pConsole->ResetStyle();
			ParsedCommandLine commandLine(line,pCmdLine->GetHost());
			
			if (!processor.ProcessCommandLine(pConsole, &commandLine)) {
				ProcessChangeRegPath(context, pConsole, line);
			}
		}
	}
	else {
		CommandProcessor processor = CommandProcessor(&commands);
		ParsedCommandLine line = pCmdLine->GetParametersAsLine();
		processor.ProcessCommandLine(pConsole, &line);
	}
}

CommandInfo RegCommand::GetInfo() {
	return CommandInfo("reg", "", "Open the registry editor.");
}

void PrintRegValueLine(IConsole* pConsole, string pValueName,DWORD pValType, BYTE *pValBuf, DWORD pValSize ) {

	pConsole->Write("  %s", pValueName.c_str());
	int i = 0;
	wstring linkW;
	string linkA;
	const char *valPtr = (const char*)pValBuf;
	switch (pValType){
	case REG_BINARY:
		pConsole->Write(" binary ");
		for (int i = 0; i < pValSize; i++)
			pConsole->Write("%02x", pValBuf[i]);
		break;
	case REG_DWORD:
		pConsole->Write(" dword %.8x", *(DWORD*)pValBuf);
		break;
	case REG_EXPAND_SZ:
		pConsole->Write(" expand '%s'", pValBuf);
		break;
	case REG_LINK:
		linkW = wstring((wchar_t*)pValBuf);
		linkA = string(linkW.begin(), linkW.end());
	
		pConsole->Write(" link '%s'", linkA.c_str());
		break;
	case REG_MULTI_SZ:
		pConsole->Write(" multi");


		while (strlen(valPtr) > 0) {
			pConsole->Write("  \"%s\"", valPtr);
			valPtr += strlen((const char*)valPtr);
			valPtr++;
		}
		
		break;
	case REG_QWORD:
		pConsole->Write(" qword %llx", *(long long*)pValBuf);
		break;
	case REG_SZ:
		pConsole->Write(" string '%s'", pValBuf);
		break;
	}
	pConsole->WriteLine("");
}

void PrintRegValueLineW(IConsole* pConsole, wstring pValueName, DWORD pValType, BYTE *pValBuf, DWORD pValSize) {
	string valname = string(pValueName.begin(), pValueName.end());
	pConsole->Write(string("  ")+valname);
	int i = 0;
	wstring linkW;
	string linkA;
	const wchar_t *valPtr = (const wchar_t*)pValBuf;
	switch (pValType){
	case REG_BINARY:
		pConsole->Write(" binary ");
		for (int i = 0; i < pValSize; i++)
			pConsole->Write("%02x", pValBuf[i]);
		break;
	case REG_DWORD:
		pConsole->Write(" dword %.8x", *(DWORD*)pValBuf);
		break;
	case REG_EXPAND_SZ:
		linkW = wstring((wchar_t*)pValBuf);
		linkA = string(linkW.begin(), linkW.end());
		pConsole->Write(" expand '%s'", linkA.c_str());
		break;
	case REG_LINK:
		linkW = wstring((wchar_t*)pValBuf);
		linkA = string(linkW.begin(), linkW.end());

		pConsole->Write(" link '%s'", linkA.c_str());
		break;
	case REG_MULTI_SZ:
		pConsole->Write(" multi");


		while (wcslen(valPtr) > 0) {
			linkW = wstring(const_cast<wchar_t*>(valPtr));
			linkA = string(linkW.begin(), linkW.end());
			pConsole->Write("  \"%s\"", linkA.c_str());
			valPtr += wcslen(static_cast<const wchar_t*>(valPtr));
			valPtr++;
		}

		break;
	case REG_QWORD:
		pConsole->Write(" qword %llx", *(long long*)pValBuf);
		break;
	case REG_SZ:
		linkW = wstring((wchar_t*)pValBuf);
		linkA = string(linkW.begin(), linkW.end());
		pConsole->Write(" string '%s'", linkA.c_str());
		break;
	}
	pConsole->WriteLine("");
}

BYTE *valBuf = NULL;

RegListCommand::RegListCommand(RegContext *pContext) {
	_context = pContext;
	
}

void RegListCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	try {
		RegPathParam path(_context, pCmdLine->GetArgs().size() == 1 ? "" : pCmdLine->GetArgs().at(1), false,true);
		int index = 0;
		char Temp[1024];
		DWORD TMP;
		DWORD RC = 0;
		bool firstSubKey = true;
		TMP = sizeof(Temp);
		while (path.EnumKey(index, Temp, &TMP)) {	
			TMP = sizeof(Temp);
			if (firstSubKey){
				pConsole->WriteLine("Subkeys");
				firstSubKey = false;
			}
			pConsole->WriteLine("  %s", Temp);
			index++;
		}

		bool firstValue = true;
		DWORD valType;
		DWORD valSize;
		DWORD valSizeP = 1024 * 1024 * 3;
		if (valBuf == NULL)
			valBuf = (BYTE*)malloc(valSizeP);
		RC = 0;
		index = 0;
		valSize = valSizeP;
		while (path.EnumValue(index, Temp, &TMP,  &valType, valBuf, &valSize)) {
				
			TMP = sizeof(Temp);
			
			if (firstValue) {
				pConsole->WriteLine("");
				pConsole->WriteLine("Values");
				firstValue = false;
			}
			PrintRegValueLine(pConsole, Temp, valType, valBuf, valSize);
			valSize = valSizeP;
			index++;
		}
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

CommandInfo RegListCommand::GetInfo() {
	return CommandInfo("list", "[pathtokey]", "Lists values and subkeys of the current or specified path");
}


RegOpenCommand::RegOpenCommand(RegContext *pContext) {
	_context = pContext;
}




void RegOpenCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: open path");
		return;
	}
	
	
	ProcessChangeRegPath(_context, pConsole, pCmdLine->GetArgs().at(1));
}

CommandInfo RegOpenCommand::GetInfo() {
	return CommandInfo("open", "<path>", "Open the specified path");
}




RegRootCommand::RegRootCommand(RegContext *pContext) {
	_context = pContext;
}


void RegRootCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: root HKLM|HKCR|HKCU|HKU|HKCC|HKPD");
		return;
	}
	HKEY key = ParseKey(pCmdLine->GetArgs().at(1).c_str());

	if (key == NULL) {
		pConsole->WriteLine("Invalid Root");
		return;
	}

	if (_context->currentKey != _context->rootKey) 
		RegCloseKey(_context->currentKey);

	_context->currentKey = key;
	_context->rootKey = key;
	_context->regPath = "";
}

CommandInfo RegRootCommand::GetInfo() {
	return CommandInfo("root", "<rootkey>", "Sets the root key to HKLM, HKCU, HKU, HKCR, HKCC or HKPD.");
}


FindRegCommand::FindRegCommand(RegContext *pContext) {
	_context = pContext;
}

void FindRegCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: findreg keyword ");
		return;
	}
	HKEY key = _context->currentKey;
	pConsole->WriteLine("Searching... ");
	ProcessLevel(pConsole, _context->regPath=="" ? NULL : _context->regPath.c_str(), pCmdLine->GetArgs().at(1).c_str());
	pConsole->WriteLine("Done");
}

bool strcontains(const char *s1, const char *s2)
{
	char    *s, *t, *u;

	if ((s1 == NULL) || (s2 == NULL))
		return false;

	s = _strdup(s1);
	if (s == NULL)
		return false;
	t = _strdup(s2);
	if (t == NULL)
	{
		free(s);
		return false;
	}
	_strupr(s);
	_strupr(t);

	bool result = strstr(s, t) != NULL;
	free(s);
	free(t);
	return result;
}

void FindRegCommand::ProcessLevel(IConsole *pConsole,const char *pSubKey, const char *pKeyword) {
	bool isMatch = false;
	HKEY key;

	DWORD error = RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_READ, &key);

	if (error != ERROR_SUCCESS){
		return;
	}

	int index = 0;
	char Temp[1024];
	DWORD TMP;
	DWORD RC = 0;

	std::list<string> subKeys;
	string basePath = !pSubKey ? "" : string(pSubKey) + "\\";
	while (RC != ERROR_NO_MORE_ITEMS) {
		RC = RegEnumKeyExA(key, index, Temp, &TMP, 0, NULL, NULL, NULL);
		TMP = sizeof(Temp);
		if (RC == ERROR_SUCCESS && strcontains(Temp, pKeyword)) {
			isMatch = true;
		}
		if (strlen(Temp)>0)
			subKeys.push_back(basePath + Temp);
		index++;
	}


	DWORD valType;
	DWORD valSize;
	DWORD valSizeP = 1024 * 1024 * 3;
	if (valBuf == NULL)
		valBuf = (BYTE*)malloc(valSizeP);
	RC = 0;
	index = 0;

	while (RC != ERROR_NO_MORE_ITEMS) {
		valSize = valSizeP;
		RC = RegEnumValueA(key, index, Temp, &TMP, 0, &valType, valBuf, &valSize);
		TMP = sizeof(Temp);
		if (RC == ERROR_SUCCESS) {
			bool thisIsMatch = false;
			if (strcontains(Temp, pKeyword)) {
				isMatch = true;
				thisIsMatch = true;
			}

			int i = 0;
			const char *valPtr = (const char*)valBuf;
			switch (valType){
			case REG_BINARY:

				break;
			case REG_DWORD:

				break;
			case REG_EXPAND_SZ:
				if (strcontains((const char*)valBuf, pKeyword)){
					isMatch = true;
					thisIsMatch = true;
				}

				break;
			case REG_LINK:
				char Temp2[1024];
				wcstombs(Temp2, (wchar_t *)valBuf, sizeof(Temp2));
				if (strcontains((const char*)Temp2, pKeyword)){
					isMatch = true;
					thisIsMatch = true;
				}

				break;
			case REG_MULTI_SZ:
				while (strlen(valPtr) > 0) {
					if (strcontains((const char*)valPtr, pKeyword)){
						isMatch = true;
						thisIsMatch = true;
					}
					valPtr += strlen((const char*)valPtr)+1;
				}
				break;
			case REG_QWORD:

				break;
			case REG_SZ:
				if (strcontains((const char*)valBuf, pKeyword)){
					isMatch = true;
					thisIsMatch = true;
				}

				break;
			}
			if (thisIsMatch)
				PrintRegValueLine(pConsole, Temp, valType, valBuf, valSize);
		}
		index++;
	}
	RegCloseKey(key);
	
	if (isMatch)
		pConsole->WriteLine("Match: %s", pSubKey ? pSubKey : "");
	subKeys.unique();
	for each (string sub in subKeys)
	{

		ProcessLevel(pConsole, sub.c_str(), pKeyword);
	}
}

CommandInfo FindRegCommand::GetInfo() {
	return CommandInfo("find", "<keyword>", "Searches for a string in the current key and subkeys");
}


FindRegWriteableCommand::FindRegWriteableCommand(RegContext *pContext) {
	_context = pContext;
}



void FindRegWriteableCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 1)
	{
		pConsole->WriteLine("SYNTAX: writeable");
		return;
	}

	HKEY key = _context->currentKey;
	pConsole->WriteLine("Searching... ");
	ProcessLevel(pConsole, _context->regPath == "" ? NULL : _context->regPath.c_str());
	pConsole->WriteLine("Done");

}


void FindRegWriteableCommand::ProcessLevel(IConsole *pConsole, const char *pSubKey) {
	bool isMatch = false;
	HKEY key;

	DWORD error = RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_READ, &key);

	if (error != ERROR_SUCCESS){
		return;
	}

	int index = 0;
	char Temp[1024];
	DWORD TMP;
	DWORD RC = 0;
	std::list<string> subKeys;
	string basePath = !pSubKey ? "" : string(pSubKey) + "\\";
	while (RC != ERROR_NO_MORE_ITEMS) {
		RC = RegEnumKeyExA(key, index, Temp, &TMP, 0, NULL, NULL, NULL);
		TMP = sizeof(Temp);
		
		if (strlen(Temp)>0)
			subKeys.push_back(basePath + Temp);
		index++;
	}
	RegCloseKey(key);
	error = RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_WRITE, &key);
	if (error == ERROR_SUCCESS){
		pConsole->WriteLine("Writable!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	else if(RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS){
		pConsole->WriteLine("Writable (value only)!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	else if (RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_CREATE_SUB_KEY, &key) == ERROR_SUCCESS){
		pConsole->WriteLine("Writable (subkey only)!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	
	subKeys.unique();
	for each (string sub in subKeys)
	{
		if (sub.length()>0)
			ProcessLevel(pConsole, sub.c_str());
	}
}

CommandInfo FindRegWriteableCommand::GetInfo() {
	return CommandInfo("writeable", "", "Lists all writable entries in current key and subkeys");
}

RegSetCommand::RegSetCommand(RegContext *pContext) {
	_context = pContext;
}




typedef struct {
	string name;
	DWORD type;
} RegValueType;


void RegSetCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size() < 4)
	{
		pConsole->WriteLine("SYNTAX: set <value name> <value type> <value>");
		pConsole->WriteLine("The following are the value types supported by this command.");
		pConsole->WriteLine("");
		pConsole->WriteLine("dword   - A hex or decimal dword value");
		pConsole->WriteLine("   Example: set testvalue dword  1");
		pConsole->WriteLine("			set testvalue dword 0xFFAA3311");
		pConsole->WriteLine("");
		pConsole->WriteLine("qword   - A hex or decimal qword value");
		pConsole->WriteLine("   Example: set testvalue qword  1");
		pConsole->WriteLine("			set testvalue qword 0x12345678FFAA3311");
		pConsole->WriteLine("");
		pConsole->WriteLine("string  - A string");
		pConsole->WriteLine("   Example: set teststr string  \"Hello\"");
		pConsole->WriteLine("");
		pConsole->WriteLine("expand  - A string");
		pConsole->WriteLine("   Example: set testexp expand  \"%%PATH%%;c:\\test\"");
		pConsole->WriteLine("");
		pConsole->WriteLine("multi   - One or more string values");
		pConsole->WriteLine("   Example: set testmulti multi  \"str1\" \"str2\" \"str3\" \"str4\"");
		pConsole->WriteLine("");
		pConsole->WriteLine("binary  - The value parameter is a hex or decimal dword value");
		pConsole->WriteLine("   Example: set testbin binary \"0F A4 5F 12 0A\"");
		pConsole->WriteLine("");		
		return;
	}

	RegValueType type[] = { 
			{ "dword", REG_DWORD }, 
			{ "qword", REG_QWORD }, 
			{ "string", REG_SZ }, 
			{ "expand", REG_EXPAND_SZ }, 
			{ "multi", REG_MULTI_SZ },
			{ "binary", REG_BINARY } };
	

	string valueName = pCmdLine->GetArgs().at(1);
	RegPathParam *path;
	try {
		path = new RegPathParam(_context, valueName, true,true);
	}
	catch (invalid_argument pE){
		pConsole->WriteLine(string("ERROR: ") + pE.what());
		return;
	}

	string valueType = pCmdLine->GetArgs().at(2);
	string value = pCmdLine->GetArgs().at(3);
	DWORD valueTypeCode = -1;
	for (int i = 0; i < 6;i++)
		if (type[i].name == valueType)
		{
			valueTypeCode = type[i].type;
			break;
		}
	if (valueTypeCode == -1)
	{
		pConsole->WriteLine("Invalid Value Type");
		return;
	}
	
	DWORD valSize = 0;
	DWORD dwordValue;
	LONGLONG qwordValue;
	char *src;
	char *mbuffer;
	char *curPtr;
	BYTE *buffer;

	try {
		switch (valueTypeCode) {
		case REG_DWORD:
			dwordValue = strtoul(value.c_str(), NULL, 0);
			valSize = sizeof(DWORD);
			path->SetValue(REG_DWORD, (BYTE*)&dwordValue, valSize);
			break;
		case REG_QWORD:
			qwordValue = _strtoui64(value.c_str(), NULL, 0);
			valSize = sizeof(LONGLONG);
			path->SetValue(REG_QWORD, (BYTE*)&qwordValue, valSize);
			break;
		case REG_SZ:
			path->SetValue(REG_SZ, (const BYTE*)value.c_str(), (DWORD)value.length() + 1);
			break;
		case REG_EXPAND_SZ:
			path->SetValue(REG_EXPAND_SZ, (const BYTE*)value.c_str(), (DWORD)value.length() + 1);
			break;
		case REG_MULTI_SZ:
			for (int i = 3; i < pCmdLine->GetArgs().size(); i++)
				valSize += (DWORD)pCmdLine->GetArgs().at(i).length() + 1;

			valSize++;
			mbuffer = (char*)malloc(valSize);
			curPtr = mbuffer;
			for (int i = 3; i < pCmdLine->GetArgs().size(); i++)
			{
				string arg = pCmdLine->GetArgs().at(i);
				memcpy(curPtr, arg.c_str(), arg.length());
				curPtr += (DWORD)arg.length();
				*curPtr = 0;
				curPtr++;
			}
			*curPtr = 0;
			curPtr++;
			path->SetValue(REG_MULTI_SZ, (const BYTE*)mbuffer, valSize);
			free(mbuffer);
			break;
		case REG_BINARY:
			src = _strdup(value.c_str());

			for (int i = 0; i < (DWORD)value.length(); i++) {
				switch (i % 3) {
				case 2:
					if (src[i] != ' '){
						pConsole->WriteLine("Incorrect value format");
						return;
					}
					src[i] = 0;
					break;
				case 1:
					valSize++;
				case 0:
					if (!((src[i] >= '0' && src[i] <= '9') || (src[i] >= 'A' && src[i] <= 'F') || (src[i] >= 'a' && src[i] <= 'f'))){
						pConsole->WriteLine("Incorrect value format");
						return;
					}
					break;
				}
			}
			buffer = (BYTE*)malloc(valSize);
			for (int i = 0; i < (DWORD)value.length(); i += 3){
				DWORD val = strtol(&src[i], NULL, 16);
				buffer[i / 3] = (BYTE)val;
			}
			path->SetValue(REG_BINARY, (const BYTE*)buffer, valSize);
			free(buffer);
			free(src);
		}
	}
	catch (invalid_argument pE){
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}

	delete path;
}

CommandInfo RegSetCommand::GetInfo() {
	return CommandInfo("set", "<name|path\\name> <type> <value>", "Sets a value.");
}

RegDelValCommand::RegDelValCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDelValCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: delval name");
		return;
	}

	

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true,true);
		
		path.DeleteValue();
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ")+pE.what());
	}
}

CommandInfo RegDelValCommand::GetInfo() {
	return CommandInfo("delval", "<valuename|valuepath>", "Deletes a value.");
}


RegCreateKeyCommand::RegCreateKeyCommand(RegContext *pContext) {
	_context = pContext;
}

void RegCreateKeyCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: mkkey <subkey>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true,true);

		path.CreateKey();
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

CommandInfo RegCreateKeyCommand::GetInfo() {
	return CommandInfo("mkkey", "<keyname|keypath>", "Create a new key.");
}


RegDeleteKeyCommand::RegDeleteKeyCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDeleteKeyCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: delkey <subkey>");
		return;
	}
	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true,true);

		path.DeleteKey();
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

CommandInfo RegDeleteKeyCommand::GetInfo() {
	return CommandInfo("delkey", "<keyname|keypath>", "Delete a new key.");
}




RegDeleteTreeCommand::RegDeleteTreeCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDeleteTreeCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: deltree <subkey>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true,true);

		path.DeleteTree();
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}

}

CommandInfo RegDeleteTreeCommand::GetInfo() {
	return CommandInfo("deltree", "<vkeyname|keypath>", "Delete a registry tree.");
}

RegImportCommand::RegImportCommand(RegContext *pContext) {
	_context = pContext;
}

void RegImportCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: import <file.reg>");
		return;
	}
	string keyName = pCmdLine->GetArgs().at(1);
	wstring wideName = wstring(keyName.begin(), keyName.end());

	DWORD error = RegDeleteKeyExW(_context->currentKey, wideName.c_str(), 0, NULL);

	if (error != ERROR_SUCCESS){
		pConsole->WriteLine("Failed to delete key: %d", error);
		return;
	}
}

CommandInfo RegImportCommand::GetInfo() {
	return CommandInfo("import", "<file>", "Import registry values from file.");
}

RegExportCommand::RegExportCommand(RegContext *pContext) {
	_context = pContext;
}

void RegExportCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: export <file.reg>");
		return;
	}
	string keyName = pCmdLine->GetArgs().at(1);
	wstring wideName = wstring(keyName.begin(), keyName.end());

	DWORD error = RegSaveKeyW(_context->currentKey, wideName.c_str(), NULL);

	if (error != ERROR_SUCCESS){
		pConsole->WriteLine("Failed to export key: %d", error);
		return;
	}
}

CommandInfo RegExportCommand::GetInfo() {
	return CommandInfo("export", "<file>", "Export registry values to a file.");
}

RegAclCommand::RegAclCommand(RegContext *pContext) {
	_context = pContext;
}
void RegAclCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 1)
	{
		pConsole->WriteLine("SYNTAX: acl <optional keypath>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().size() == 1 ? "":pCmdLine->GetArgs().at(1), false,true);
		PSECURITY_DESCRIPTOR securityDescriptor = NULL;
		PSECURITY_DESCRIPTOR ownerSecurityDescriptor = NULL;
		DWORD descriptorSize = 0;
		path.GetKeySecurity(DACL_SECURITY_INFORMATION, securityDescriptor, &descriptorSize);
		securityDescriptor = malloc(descriptorSize);
		path.GetKeySecurity(DACL_SECURITY_INFORMATION, securityDescriptor, &descriptorSize);
		
		descriptorSize = 0;
		path.GetKeySecurity(OWNER_SECURITY_INFORMATION, ownerSecurityDescriptor, &descriptorSize);
		ownerSecurityDescriptor = malloc(descriptorSize);
		path.GetKeySecurity(OWNER_SECURITY_INFORMATION, ownerSecurityDescriptor, &descriptorSize);
		
		BOOL daclPresent;
		BOOL daclDefaulted;
		PACL acl;
		if (!GetSecurityDescriptorDacl(securityDescriptor, &daclPresent, &acl, &daclDefaulted)) {
			pConsole->WriteLine(GetLastErrorAsString());
			return;
		}
		if (!daclPresent) {
			pConsole->WriteLine("No ACL present");
			return;
		}

		if (daclDefaulted)
			pConsole->WriteLine("ACL is a default");

		PrintOwnerInformation(pConsole, ownerSecurityDescriptor);
		PrintAclInformation(pConsole, securityDescriptor, acl);
		free(securityDescriptor);
		free(ownerSecurityDescriptor);
	}
	catch (invalid_argument pE) {
		pConsole->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

CommandInfo RegAclCommand::GetInfo() {
	return CommandInfo("acl", "[keypath]", "Display ACL for a key.");
}




RegHelpCommand::RegHelpCommand(RegContext *pContext) {
	_context = pContext;
}

void RegHelpCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	pConsole->WriteLine("help - Shows this screen");
	pConsole->WriteLine("exit - Exits the registry editor");
	pConsole->WriteLine("list - Lists values and subkeys of current path");
	pConsole->WriteLine("open [path] - Opens a registry key");
	pConsole->WriteLine("root HKLM|HKCU|HKU|HKCR|HKCC|HKPD - Sets the root key");
	pConsole->WriteLine("find [str] - Finds a string in the current key and subkeys");
	pConsole->WriteLine("writeable - Lists all writable entries in current key and subkeys");
	pConsole->WriteLine("set [valuename] [valueType] [value] - Set a value");	
	pConsole->WriteLine("delval [valuename] - Deletes a value");
	pConsole->WriteLine("delkey [subkey] - Deletes a subkey");
	pConsole->WriteLine("deltree [subkey] - Deletes a subkey and all its children");
	pConsole->WriteLine("mkkey [subkey] - Creates a new subkey");
	pConsole->WriteLine("acl [key] - Displays the ACL of a key.");
	
	pConsole->WriteLine("");
	pConsole->WriteLine("A path can also be opened by just typing it. '..' may be used to goto the parent key and a subkey can be opened by just specifying its name.");
	
}

CommandInfo RegHelpCommand::GetInfo() {
	return CommandInfo("help", "", "Display this screen");
}