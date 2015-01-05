#include "pch.h"
#include "RegistryCommands.h"
#include "AclHelpers.h"


RegCommand::RegCommand() {

}

HKEY ParseKey(const char *pStrKey) {
	if (!strcmpi(pStrKey, "HKLM"))				    return HKEY_LOCAL_MACHINE;
	if (!strcmpi(pStrKey, "HKEY_LOCAL_MACHINE"))    return HKEY_LOCAL_MACHINE;
	if (!strcmpi(pStrKey, "HKCR"))				    return HKEY_CLASSES_ROOT;
	if (!strcmpi(pStrKey, "HKEY_CLASSES_ROOT"))     return HKEY_CLASSES_ROOT;
	if (!strcmpi(pStrKey, "HKCU"))			        return HKEY_CURRENT_USER;
	if (!strcmpi(pStrKey, "HKEY_CURRENT_USER"))     return HKEY_CURRENT_USER;
	if (!strcmpi(pStrKey, "HKU"))				    return HKEY_USERS;
	if (!strcmpi(pStrKey, "HKEY_USERS"))		    return HKEY_USERS;
	if (!strcmpi(pStrKey, "HKCC"))				    return HKEY_CURRENT_CONFIG;
	if (!strcmpi(pStrKey, "HKEY_CURRENT_CONFIG"))   return HKEY_CURRENT_CONFIG;
	if (!strcmpi(pStrKey, "HKPD"))                  return HKEY_PERFORMANCE_DATA;
	if (!strcmpi(pStrKey, "HKEY_PERFORMANCE_DATA")) return HKEY_PERFORMANCE_DATA;
	return NULL;
}

void ProcessChangeRegPath(RegContext* pContext, Connection *pConnection,string pPath){
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
		
		if (error != ERROR_SUCCESS) {
			error = RegOpenKeyExA(pContext->rootKey, pPath.c_str(), 0, KEY_WRITE | KEY_READ, &key);
			if (error != ERROR_SUCCESS)
				error = RegOpenKeyExA(pContext->rootKey, pPath.c_str(), 0, KEY_READ, &key);
			newPath = pPath;
		}
	}


	if (error != ERROR_SUCCESS){
		pConnection->WriteLine("Failed to open registry key %d", error);
		return;
	}

	if (pContext->currentKey != pContext->rootKey)
		RegCloseKey(pContext->currentKey);
	pContext->regPath = newPath;
	pContext->currentKey = key;
}


class RegPathParam
{
private:
	RegContext *_context;
	HKEY _root;
	string _valueName;
	bool _closeKey;
public:
	RegPathParam(RegContext*pContext, string pValue, bool pIncludeValue) {
		_context = pContext;
		_closeKey = false;
		if (pValue.find_first_of('\\') == std::string::npos) {

			_root = _context->currentKey;
			_valueName = pValue;
			return;
		}

		string rootName = pValue.substr(0, pValue.find_first_of('\\'));
		HKEY root = ParseKey(rootName.c_str());
		pValue = pValue.substr(pValue.find_first_of('\\') + 1, string::npos);
		if (root == NULL) {
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

		DWORD error = RegOpenKeyExA(root, path.c_str(), NULL, KEY_WRITE | KEY_READ, &_root);
		if (error == ERROR_SUCCESS){
			_closeKey = true;
			return;
		}
		error = RegOpenKeyExA(root, path.c_str(), NULL,  KEY_READ, &_root);
		if (error != ERROR_SUCCESS) {
			throw invalid_argument(string("Error opening key ") + to_string(error));
		}
		_closeKey = true;
	}

	~RegPathParam() {
		if (_closeKey){
			RegCloseKey(_root);
		}
	}

	void SetValue(
		_In_ DWORD dwType,
		_In_reads_bytes_opt_(cbData) CONST BYTE * lpData,
		_In_ DWORD cbData
		){
		LSTATUS result = RegSetValueExA(_root, _valueName.c_str(), NULL, dwType, lpData, cbData);
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegSetValueExA " + to_string(result));
	}

	void DeleteValue() {
		LSTATUS result = RegDeleteValueA(_root, _valueName.c_str());
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteValueA " + to_string(result));
	}

	void CreateKey() {
		HKEY resultKey;
		DWORD result = RegCreateKeyExA(_root, _valueName.c_str(), 0, NULL, 0, NULL, NULL, &resultKey, NULL);
		RegCloseKey(resultKey);
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegCreateKeyExA " + to_string(result));
	}

	void DeleteKey() {
		string keyName = _valueName;
		wstring wideName = wstring(keyName.begin(), keyName.end());
		
		DWORD result= RegDeleteKeyExW(_root, wideName.c_str(), 0, NULL);
	
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteKeyExW " + to_string(result));
	}

	void DeleteTree() {
		string keyName = _valueName;
		wstring wideName = wstring(keyName.begin(), keyName.end());
		DWORD result = RegDeleteTreeW(_context->currentKey, wideName.c_str());
		if (result != ERROR_SUCCESS)
			throw invalid_argument("Error calling RegDeleteTreeW " + to_string(result));
	}

	void GetKeySecurity(
		_In_ SECURITY_INFORMATION SecurityInformation,
		_Out_writes_bytes_opt_(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
		_Inout_ LPDWORD lpcbSecurityDescriptor
		) {
		bool sizeCheck = *lpcbSecurityDescriptor;
		LSTATUS result = RegGetKeySecurity(_root, SecurityInformation, pSecurityDescriptor, lpcbSecurityDescriptor);
		if (result != ERROR_SUCCESS && sizeCheck)
			throw invalid_argument("Error calling RegGetKeySecurity " + to_string(result));
	}

	bool EnumKey(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
		_Inout_ LPDWORD lpcchName
		) {
		LSTATUS result = RegEnumKeyExA(_root, dwIndex, lpName, lpcchName, 0, NULL, NULL, NULL);
		if (result == ERROR_NO_MORE_ITEMS)
			return false;
		if (result == ERROR_SUCCESS)
			return true;
		throw invalid_argument("Error calling RegEnumKeyExA " + to_string(result));
	}

	bool EnumValue(
		_In_ DWORD dwIndex,
		_Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
		_Inout_ LPDWORD lpcchValueName,

		_Out_opt_ LPDWORD lpType,
		_Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
		_Inout_opt_ LPDWORD lpcbData
		) {
		LSTATUS result = RegEnumValueA(_root,dwIndex,lpValueName, lpcchValueName, 0,lpType,lpData, lpcbData);
		if (result == ERROR_NO_MORE_ITEMS)
			return false;
		if (result == ERROR_SUCCESS)
			return true;
		throw invalid_argument("Error calling RegEnumValueA " + to_string(result));
	}
};


class RegProcessHost : public ICommandProcessorHost
{
private:
	RegContext *_context;
	bool _noPrompt;
public:
	RegProcessHost(RegContext*pContext, bool pNoPrompt) {
		_context = pContext;
		_noPrompt = pNoPrompt;
	}
	virtual void PrintPrompt(Connection *pConnection) {
		if (_noPrompt)return;
		if (_context->rootKey == HKEY_LOCAL_MACHINE) pConnection->Write("HKLM\\");
		else if (_context->rootKey == HKEY_CLASSES_ROOT) pConnection->Write("HKCR\\");
		else if (_context->rootKey == HKEY_CURRENT_USER) pConnection->Write("HKCU\\");
		else if (_context->rootKey == HKEY_USERS) pConnection->Write("HKU\\");
		else if (_context->rootKey == HKEY_CURRENT_CONFIG) pConnection->Write("HKCC\\");
		else if (_context->rootKey == HKEY_PERFORMANCE_DATA) pConnection->Write("HKPD\\");

		pConnection->Write("%s>", _context->regPath.c_str());
	}
	virtual void UnhandledLine(Connection *pConnection, string pLine){
		ProcessChangeRegPath(_context, pConnection, pLine);
	}
};

void RegCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	RegContext *context = new RegContext();
	context->rootKey = HKEY_LOCAL_MACHINE;
	context->currentKey = HKEY_LOCAL_MACHINE;
	context->regPath = "";
	vector<BaseCommand *> commands = vector<BaseCommand *>();
	commands.push_back((BaseCommand*)new RegHelpCommand(context));
	commands.push_back((BaseCommand*)new RegListCommand(context));
	commands.push_back((BaseCommand*)new RegOpenCommand(context));
	commands.push_back((BaseCommand*)new RegRootCommand(context));
	commands.push_back((BaseCommand*)new FindRegCommand(context));
	commands.push_back((BaseCommand*)new FindRegWriteableCommand(context));
	commands.push_back((BaseCommand*)new RegSetCommand(context));
	commands.push_back((BaseCommand*)new RegDelValCommand(context));
	commands.push_back((BaseCommand*)new RegDeleteKeyCommand(context));
	commands.push_back((BaseCommand*)new RegCreateKeyCommand(context));
	commands.push_back((BaseCommand*)new RegDeleteTreeCommand(context));
	commands.push_back((BaseCommand*)new RegExportCommand(context));
	commands.push_back((BaseCommand*)new RegImportCommand(context));
	commands.push_back((BaseCommand*)new RegAclCommand(context));
	
	if (pCmdLine->GetArgs().size() == 1) {
		RegProcessHost host(context,false);
		CommandProcessor processor = CommandProcessor(commands, &host);
		pConnection->WriteLine("Registry Editor - Type HELP for assistance.");
		

		processor.PrintPrompt(pConnection);
		const char* line = pConnection->ReadLine();

		while (line != NULL) {
			if (processor.ProcessData(pConnection, line)) {

				break;
			}
			line = pConnection->ReadLine();
		}
	}
	else {
		RegProcessHost host(context,true);
		CommandProcessor processor = CommandProcessor(commands, &host);
		ParsedCommandLine line = pCmdLine->GetParametersAsLine();
		processor.ProcessCommandLine(pConnection, &line);
	}
}

string RegCommand::GetName() {
	return "reg";
}

void PrintRegValueLine(Connection* pConnection, string pValueName,DWORD pValType, BYTE *pValBuf, DWORD pValSize ) {

	pConnection->Write("  %s", pValueName.c_str());
	int i = 0;
	const char *valPtr = (const char*)pValBuf;
	switch (pValType){
	case REG_BINARY:
		pConnection->Write(" binary ");
		for (int i = 0; i < pValSize; i++)
			pConnection->Write("%02x", pValBuf[i]);
		break;
	case REG_DWORD:
		pConnection->Write(" dword %.8x", *(DWORD*)pValBuf);
		break;
	case REG_EXPAND_SZ:
		pConnection->Write(" expand '%s'", pValBuf);
		break;
	case REG_LINK:
		char Temp[1024];
		wcstombs(Temp, (wchar_t *)pValBuf, sizeof(Temp));
		pConnection->Write(" link '%s'", Temp);
		break;
	case REG_MULTI_SZ:
		pConnection->Write(" multi");


		while (strlen(valPtr) > 0) {
			pConnection->Write("  \"%s\"", valPtr);
			valPtr += strlen((const char*)valPtr);
			valPtr++;
		}
		
		break;
	case REG_QWORD:
		pConnection->Write(" qword %llx", *(long long*)pValBuf);
		break;
	case REG_SZ:
		pConnection->Write(" string '%s'", pValBuf);
		break;
	}
	pConnection->WriteLine("");
}

BYTE *valBuf = NULL;

RegListCommand::RegListCommand(RegContext *pContext) {
	_context = pContext;
	
}

void RegListCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	try {
		RegPathParam path(_context, pCmdLine->GetArgs().size() == 1 ? "" : pCmdLine->GetArgs().at(1), false);
		int index = 0;
		char Temp[1024];
		DWORD TMP;
		DWORD RC = 0;
		bool firstSubKey = true;
		TMP = sizeof(Temp);
		while (path.EnumKey(index, Temp, &TMP)) {	
			TMP = sizeof(Temp);
			if (firstSubKey){
				pConnection->WriteLine("Subkeys");
				firstSubKey = false;
			}
			pConnection->WriteLine("  %s", Temp);
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
				pConnection->WriteLine("");
				pConnection->WriteLine("Values");
				firstValue = false;
			}
			PrintRegValueLine(pConnection, Temp, valType, valBuf, valSize);
			valSize = valSizeP;
			index++;
		}
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

string RegListCommand::GetName() {
	return "list";
}


RegOpenCommand::RegOpenCommand(RegContext *pContext) {
	_context = pContext;
}




void RegOpenCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: open path");
		return;
	}
	
	
	ProcessChangeRegPath(_context, pConnection, pCmdLine->GetArgs().at(1));
}

string RegOpenCommand::GetName() {
	return "open";
}




RegRootCommand::RegRootCommand(RegContext *pContext) {
	_context = pContext;
}


void RegRootCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: root HKLM|HKCR|HKCU|HKU|HKCC|HKPD");
		return;
	}
	HKEY key = ParseKey(pCmdLine->GetArgs().at(1).c_str());

	if (key == NULL) {
		pConnection->WriteLine("Invalid Root");
		return;
	}

	if (_context->currentKey != _context->rootKey) 
		RegCloseKey(_context->currentKey);

	_context->currentKey = key;
	_context->rootKey = key;
	_context->regPath = "";
}

string RegRootCommand::GetName() {
	return "root";
}


FindRegCommand::FindRegCommand(RegContext *pContext) {
	_context = pContext;
}

void FindRegCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: findreg keyword ");
		return;
	}
	HKEY key = _context->currentKey;
	pConnection->WriteLine("Searching... ");
	ProcessLevel(pConnection, _context->regPath=="" ? NULL : _context->regPath.c_str(), pCmdLine->GetArgs().at(1).c_str());
	pConnection->WriteLine("Done");
}

bool strcontains(const char *s1, const char *s2)
{
	char    *s, *t, *u;

	if ((s1 == NULL) || (s2 == NULL))
		return false;

	s = strdup(s1);
	if (s == NULL)
		return false;
	t = strdup(s2);
	if (t == NULL)
	{
		free(s);
		return false;
	}
	strupr(s);
	strupr(t);

	bool result = strstr(s, t) != NULL;
	free(s);
	free(t);
	return result;
}

void FindRegCommand::ProcessLevel(Connection *pConnection,const char *pSubKey, const char *pKeyword) {
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
				PrintRegValueLine(pConnection, Temp, valType, valBuf, valSize);
		}
		index++;
	}
	RegCloseKey(key);
	
	if (isMatch)
		pConnection->WriteLine("Match: %s", pSubKey ? pSubKey : "");
	subKeys.unique();
	for each (string sub in subKeys)
	{

		ProcessLevel(pConnection, sub.c_str(), pKeyword);
	}
}

string FindRegCommand::GetName() {
	return "find";
}


FindRegWriteableCommand::FindRegWriteableCommand(RegContext *pContext) {
	_context = pContext;
}



void FindRegWriteableCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 1)
	{
		pConnection->WriteLine("SYNTAX: writeable");
		return;
	}

	HKEY key = _context->currentKey;
	pConnection->WriteLine("Searching... ");
	ProcessLevel(pConnection, _context->regPath == "" ? NULL : _context->regPath.c_str());
	pConnection->WriteLine("Done");

}


void FindRegWriteableCommand::ProcessLevel(Connection *pConnection, const char *pSubKey) {
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
		

		subKeys.push_back(basePath + Temp);
		index++;
	}
	RegCloseKey(key);
	error = RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_WRITE, &key);
	if (error == ERROR_SUCCESS){
		pConnection->WriteLine("Writable!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	else if(RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS){
		pConnection->WriteLine("Writable (value only)!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	else if (RegOpenKeyExA(_context->rootKey, pSubKey, 0, KEY_CREATE_SUB_KEY, &key) == ERROR_SUCCESS){
		pConnection->WriteLine("Writable (subkey only)!: %s", pSubKey ? pSubKey : "");
		RegCloseKey(key);
	}
	
	subKeys.unique();
	for each (string sub in subKeys)
	{

		ProcessLevel(pConnection, sub.c_str());
	}
}

string FindRegWriteableCommand::GetName() {
	return "writeable";
}

RegSetCommand::RegSetCommand(RegContext *pContext) {
	_context = pContext;
}




typedef struct {
	string name;
	DWORD type;
} RegValueType;


void RegSetCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size() < 4)
	{
		pConnection->WriteLine("SYNTAX: set <value name> <value type> <value>");
		pConnection->WriteLine("The following are the value types supported by this command.");
		pConnection->WriteLine("");
		pConnection->WriteLine("dword   - A hex or decimal dword value");
		pConnection->WriteLine("   Example: set testvalue dword  1");
		pConnection->WriteLine("			set testvalue dword 0xFFAA3311");
		pConnection->WriteLine("");
		pConnection->WriteLine("qword   - A hex or decimal qword value");
		pConnection->WriteLine("   Example: set testvalue qword  1");
		pConnection->WriteLine("			set testvalue qword 0x12345678FFAA3311");
		pConnection->WriteLine("");
		pConnection->WriteLine("string  - A string");
		pConnection->WriteLine("   Example: set teststr string  \"Hello\"");
		pConnection->WriteLine("");
		pConnection->WriteLine("expand  - A string");
		pConnection->WriteLine("   Example: set testexp expand  \"%%PATH%%;c:\\test\"");
		pConnection->WriteLine("");
		pConnection->WriteLine("multi   - One or more string values");
		pConnection->WriteLine("   Example: set testmulti multi  \"str1\" \"str2\" \"str3\" \"str4\"");
		pConnection->WriteLine("");
		pConnection->WriteLine("binary  - The value parameter is a hex or decimal dword value");
		pConnection->WriteLine("   Example: set testbin binary \"0F A4 5F 12 0A\"");
		pConnection->WriteLine("");		
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
		path = new RegPathParam(_context, valueName, true);
	}
	catch (invalid_argument pE){
		pConnection->WriteLine(string("ERROR: ") + pE.what());
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
		pConnection->WriteLine("Invalid Value Type");
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
			src = strdup(value.c_str());

			for (int i = 0; i < (DWORD)value.length(); i++) {
				switch (i % 3) {
				case 2:
					if (src[i] != ' '){
						pConnection->WriteLine("Incorrect value format");
						return;
					}
					src[i] = 0;
					break;
				case 1:
					valSize++;
				case 0:
					if (!((src[i] >= '0' && src[i] <= '9') || (src[i] >= 'A' && src[i] <= 'F') || (src[i] >= 'a' && src[i] <= 'f'))){
						pConnection->WriteLine("Incorrect value format");
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
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}

	delete path;
}

string RegSetCommand::GetName() {
	return "set";
}

RegDelValCommand::RegDelValCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDelValCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: delval name");
		return;
	}

	

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true);
		
		path.DeleteValue();
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ")+pE.what());
	}
}

string RegDelValCommand::GetName() {
	return "delval";
}


RegCreateKeyCommand::RegCreateKeyCommand(RegContext *pContext) {
	_context = pContext;
}

void RegCreateKeyCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: mkkey <subkey>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true);

		path.CreateKey();
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

string RegCreateKeyCommand::GetName() {
	return "mkkey";
}


RegDeleteKeyCommand::RegDeleteKeyCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDeleteKeyCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: delkey <subkey>");
		return;
	}
	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true);

		path.DeleteKey();
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

string RegDeleteKeyCommand::GetName() {
	return "delkey";
}




RegDeleteTreeCommand::RegDeleteTreeCommand(RegContext *pContext) {
	_context = pContext;
}

void RegDeleteTreeCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: deltree <subkey>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().at(1), true);

		path.DeleteTree();
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}

}

string RegDeleteTreeCommand::GetName() {
	return "deltree";
}

RegImportCommand::RegImportCommand(RegContext *pContext) {
	_context = pContext;
}

void RegImportCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: import <file.reg>");
		return;
	}
	string keyName = pCmdLine->GetArgs().at(1);
	wstring wideName = wstring(keyName.begin(), keyName.end());
	HKEY resultKey;
	DWORD error = RegDeleteKeyExW(_context->currentKey, wideName.c_str(), 0, NULL);
	RegCloseKey(resultKey);
	if (error != ERROR_SUCCESS){
		pConnection->WriteLine("Failed to delete key: %d", error);
		return;
	}
}

string RegImportCommand::GetName() {
	return "import";
}

RegExportCommand::RegExportCommand(RegContext *pContext) {
	_context = pContext;
}

void RegExportCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 2)
	{
		pConnection->WriteLine("SYNTAX: export <file.reg>");
		return;
	}
	string keyName = pCmdLine->GetArgs().at(1);
	wstring wideName = wstring(keyName.begin(), keyName.end());
	HKEY resultKey;
	DWORD error = RegSaveKeyW(_context->currentKey, wideName.c_str(), NULL);
	RegCloseKey(resultKey);
	if (error != ERROR_SUCCESS){
		pConnection->WriteLine("Failed to export key: %d", error);
		return;
	}
}

string RegExportCommand::GetName() {
	return "export";
}

RegAclCommand::RegAclCommand(RegContext *pContext) {
	_context = pContext;
}
void RegAclCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size() < 1)
	{
		pConnection->WriteLine("SYNTAX: acl <optional keypath>");
		return;
	}

	try {
		RegPathParam path(_context, pCmdLine->GetArgs().size() == 1 ? "":pCmdLine->GetArgs().at(1), false);
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
			pConnection->WriteLastError();
			return;
		}
		if (!daclPresent) {
			pConnection->WriteLine("No ACL present");
			return;
		}

		if (daclDefaulted)
			pConnection->WriteLine("ACL is a default");

		PrintOwnerInformation(pConnection, ownerSecurityDescriptor);
		PrintAclInformation(pConnection, securityDescriptor, acl);
		free(securityDescriptor);
		free(ownerSecurityDescriptor);
	}
	catch (invalid_argument pE) {
		pConnection->WriteLine(string("ERROR: ") + pE.what());
	}
	
}

string RegAclCommand::GetName() {
	return "acl";
}




RegHelpCommand::RegHelpCommand(RegContext *pContext) {
	_context = pContext;
}

void RegHelpCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	pConnection->WriteLine("help - Shows this screen");
	pConnection->WriteLine("exit - Exits the registry editor");
	pConnection->WriteLine("list - Lists values and subkeys of current path");
	pConnection->WriteLine("open [path] - Opens a registry key");
	pConnection->WriteLine("root HKLM|HKCU|HKU|HKCR|HKCC|HKPD - Sets the root key");
	pConnection->WriteLine("find [str] - Finds a string in the current key and subkeys");
	pConnection->WriteLine("writeable - Lists all writable entries in current key and subkeys");
	pConnection->WriteLine("set [valuename] [valueType] [value] - Set a value");	
	pConnection->WriteLine("delval [valuename] - Deletes a value");
	pConnection->WriteLine("delkey [subkey] - Deletes a subkey");
	pConnection->WriteLine("deltree [subkey] - Deletes a subkey and all its children");
	pConnection->WriteLine("mkkey [subkey] - Creates a new subkey");
	pConnection->WriteLine("acl [key] - Displays the ACL of a key.");
	
	pConnection->WriteLine("");
	pConnection->WriteLine("A path can also be opened by just typing it. '..' may be used to goto the parent key and a subkey can be opened by just specifying its name.");
	
}

string RegHelpCommand::GetName() {
	return "help";
}