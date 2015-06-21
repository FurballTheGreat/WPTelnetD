#include "pch.h"
#include "FileCommands.h"
#include "AclHelpers.h"
#include "TerminalHelper.h"

void TypeCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("ERROR: You must specify a file");
	else {

		HANDLE hFile = CreateFileA(pCmdLine->GetArgs().at(1).c_str(),  
			GENERIC_READ,          
			FILE_SHARE_READ,      
			NULL,                 
			OPEN_EXISTING,        
			FILE_ATTRIBUTE_NORMAL, 
			NULL);                 

		if (hFile == (HANDLE)-1) {
			pConsole->WriteLine(GetLastErrorAsString());
			return;
		}

		char   readBuffer[1024] = { 0 };
		DWORD read;
		BOOL ok = TRUE;
		while (ok){
			ok = ReadFile(hFile, readBuffer, sizeof(readBuffer) - 1, &read, NULL);
			if (!ok)
			{
				pConsole->WriteLine(GetLastErrorAsString());
			}
			else if (read > 0){
				pConsole->Write(read, (BYTE*)readBuffer);
			}
			else
				break;
		};
		if (!CloseHandle(hFile)) {
			pConsole->WriteLine(GetLastErrorAsString());
		}

	}
}

CommandInfo TypeCommand::GetInfo() {
	return CommandInfo("type", "<filename>", "Print file contents");
}

void CopyCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConsole->WriteLine("SYNTAX: copy src dst");
	else {
		if (!CopyFileA(pCmdLine->GetArgs().at(1).c_str(), pCmdLine->GetArgs().at(2).c_str(), TRUE))
			pConsole->WriteLine(GetLastErrorAsString());
	}
}

CommandInfo CopyCommand::GetInfo() {
	return CommandInfo("copy", "<src> <dst>", "Copy a file.");
}


void MoveCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConsole->WriteLine("SYNTAX: move src dst");
	else {
		if (!MoveFileA(pCmdLine->GetArgs().at(1).c_str(), pCmdLine->GetArgs().at(2).c_str()))
			pConsole->WriteLine(GetLastErrorAsString());
	}
}

CommandInfo MoveCommand::GetInfo() {
	return CommandInfo("move", "<src> <dst>", "Move a file.");
}


void AttribCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConsole->WriteLine("SYNTAX: attrib file [+|-]arsh");
	else {
		char src[1024];
		strcpy_s(src, pCmdLine->GetArgs().at(1).c_str());
		char attribss[100];
		strcpy_s(attribss, pCmdLine->GetArgs().at(2).c_str());
		DWORD attribs = GetFileAttributesA(src);
		bool set = true;
		char *attribstr = attribss;
		pConsole->WriteLine("%s current has %x attributes", src, attribs);
		while (*attribstr) {
			switch (*attribstr) {
			case '+':
				set = true;
				break;
			case '-':
				set = false;
				break;
			case 'a':
				if (set)
					attribs |= FILE_ATTRIBUTE_ARCHIVE;
				else
					attribs &= ~FILE_ATTRIBUTE_ARCHIVE;
				break;
			case 'r':
				if (set)
					attribs |= FILE_ATTRIBUTE_READONLY;
				else
					attribs &= ~FILE_ATTRIBUTE_READONLY;
				break;
			case 'h':
				if (set)
					attribs |= FILE_ATTRIBUTE_HIDDEN;
				else
					attribs &= ~FILE_ATTRIBUTE_HIDDEN;
				break;
			case 's':
				if (set)
					attribs |= FILE_ATTRIBUTE_SYSTEM;
				else
					attribs &= ~FILE_ATTRIBUTE_SYSTEM;
				break;
			}
			attribstr++;
		}
		pConsole->WriteLine("Setting attributes to %x", attribs);
		if (SetFileAttributesA(src, attribs))
			pConsole->WriteLine("Succeeded");
		else
			pConsole->WriteLine("Failed %x", GetLastError());
	}
}

CommandInfo AttribCommand::GetInfo() {
	return CommandInfo("attrib", "<path> <+/-attribs>", "Set file attribute(s).");
}


BaseFileCommand::BaseFileCommand() {

}

void BaseFileCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (!ProcessCommandLine(pConsole, pCmdLine))
		PrintSyntax(pConsole);

	if (pCmdLine->GetArgs().size() < 2){
		PrintSyntax(pConsole);
		return;
	}

	WIN32_FIND_DATAA data;
	string path = pCmdLine->GetArgs().at(1);
	HANDLE handle = FindFirstFileExA(path.c_str(), FindExInfoBasic, &data, FindExSearchNameMatch, NULL, 0);
	if (INVALID_HANDLE_VALUE == handle) {
		pConsole->WriteLine(GetLastErrorAsString());
		return;
	}
	if (path.find_last_of('\\') != string::npos) {
		path = path.substr(0, path.find_last_of('\\'));
		if (path.at(path.length() - 1) != '\\')
			path = path + "\\";
	}
	else
		path = "";
	
	do {
		string filePath = path + data.cFileName;

		if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0)
			continue;
		
		ProcessFile(pConsole, data, filePath);

	} while (FindNextFileA(handle, &data));

	FindClose(handle);
}


void DeleteFiles(IConsole *pConsole, char *pPath) {
	WIN32_FIND_DATAA data;
	HANDLE handle = FindFirstFileExA(pPath, FindExInfoBasic, &data, FindExSearchNameMatch, NULL, 0);


	pConsole->WriteLine("DELETING Files in: %s", pPath);
	if (INVALID_HANDLE_VALUE != handle) {
		do {
			pConsole->WriteLine("FOUND  %s", data.cFileName);
			char pathBuf[1024];
			strcpy_s(pathBuf, pPath);
			for (int i = strlen(pathBuf) - 1; i >= 0; i--)
			{
				bool brk = pathBuf[i] == '\\';
				pathBuf[i] = 0;
				if (brk)
					break;
			}
			strcat_s(pathBuf, "\\");
			strcat_s(pathBuf, data.cFileName);
			if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0)
				continue;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char *path = _strdup(pathBuf);

				strcat_s(pathBuf, "\\*.*");

				DeleteFiles(pConsole, pathBuf);
				RemoveDirectoryA(path);
				free(path);
			}
			else {
				pConsole->WriteLine("DELETING  %s", pathBuf);
				if (!DeleteFileA(pathBuf))
					pConsole->WriteLine(GetLastErrorAsString());

			}

		} while (FindNextFileA(handle, &data));
		FindClose(handle);
	}
	else{
		pConsole->WriteLine(GetLastErrorAsString());
	}
}

void DeleteCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: del path");
	else {
		DeleteFiles(pConsole, _strdup(pCmdLine->GetArgs().at(1).c_str()));
	}
}

CommandInfo DeleteCommand::GetInfo() {
	return CommandInfo("del", "<path>", "Delete file(s).");
}





void ListAclsCommand::ProcessFile(IConsole *pConsole, WIN32_FIND_DATAA pFileInfo, string pFileName) {
	//HMODULE lib = LoadLibraryA("ntmarta.dll");
	//if (lib == NULL){
	//	pConsole->WriteLine("Failed Load Lib");
	//	pConsole->WriteLine(GetLastErrorAsString());
	//	return;
	//}

	//setup security code
	PSECURITY_DESCRIPTOR securityDescriptor, ownerDescriptor;
	PACL dacl;
	wstring filename = wstring(pFileName.begin(), pFileName.end());
	HRESULT result = GetNamedSecurityInfoW(filename.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
		&dacl, NULL, &securityDescriptor);

	HRESULT ownerresult = GetNamedSecurityInfoW(filename.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, NULL, NULL,
		&dacl, NULL, &ownerDescriptor);

	if (ownerresult == ERROR_SUCCESS)
		PrintOwnerInformation(pConsole, ownerDescriptor);

	if (result == 2) {
		pConsole->WriteLine("Not found");
		return;
	}
	if (result != ERROR_SUCCESS){ 
		pConsole->WriteLine("Error occured %d", result);
		return; 
	}
	PrintAclInformation(pConsole, &securityDescriptor, dacl);
}

bool ListAclsCommand::ProcessCommandLine(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	return true;
}

bool ListAclsCommand::PrintSyntax(IConsole *pConsole){
	pConsole->WriteLine("lacl <wildcard>");
	return true;
}

CommandInfo ListAclsCommand::GetInfo(){
	return CommandInfo("lacl", "<path/wildcard>", "List acls on file(s).");
}
