#include "pch.h"
#include "FileCommands.h"
#include "AclHelpers.h"

void TypeCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("ERROR: You must specify a file");
	else {

		HANDLE hFile = CreateFileA(pCmdLine->GetArgs().at(1).c_str(),  
			GENERIC_READ,          
			FILE_SHARE_READ,      
			NULL,                 
			OPEN_EXISTING,        
			FILE_ATTRIBUTE_NORMAL, 
			NULL);                 

		if (hFile == (HANDLE)-1) {
			pConnection->WriteLastError();
			return;
		}

		char   readBuffer[1024] = { 0 };
		DWORD read;
		BOOL ok = TRUE;
		while (ok){
			ok = ReadFile(hFile, readBuffer, sizeof(readBuffer) - 1, &read, NULL);
			if (!ok)
			{
				pConnection->WriteLastError();
			}
			else if (read > 0){
				pConnection->Write(read, readBuffer);
			}
			else
				break;
		};
		if (!CloseHandle(hFile)) {
			pConnection->WriteLastError();
		}

	}
}

string TypeCommand::GetName() {
	return "type";
}

void CopyCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConnection->WriteLine("SYNTAX: copy src dst");
	else {
		if (!CopyFileA(pCmdLine->GetArgs().at(1).c_str(), pCmdLine->GetArgs().at(2).c_str(), TRUE))
			pConnection->WriteLastError();
	}
}

string CopyCommand::GetName() {
	return "copy";
}


void MoveCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConnection->WriteLine("SYNTAX: move src dst");
	else {
		if (!MoveFileA(pCmdLine->GetArgs().at(1).c_str(), pCmdLine->GetArgs().at(2).c_str()))
			pConnection->WriteLastError();
	}
}

string MoveCommand::GetName() {
	return "move";
}


void AttribCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<3)
		pConnection->WriteLine("SYNTAX: attrib file [+|-]arsh");
	else {
		char src[1024];
		strcpy_s(src, pCmdLine->GetArgs().at(1).c_str());
		char attribss[100];
		strcpy_s(attribss, pCmdLine->GetArgs().at(2).c_str());
		DWORD attribs = GetFileAttributesA(src);
		bool set = true;
		char *attribstr = attribss;
		pConnection->WriteLine("%s current has %x attributes", src, attribs);
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
		pConnection->WriteLine("Setting attributes to %x", attribs);
		if (SetFileAttributesA(src, attribs))
			pConnection->WriteLine("Succeeded");
		else
			pConnection->WriteLine("Failed %x", GetLastError());
	}
}

string AttribCommand::GetName() {
	return "attrib";
}


BaseFileCommand::BaseFileCommand() {

}

void BaseFileCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (!ProcessCommandLine(pConnection, pCmdLine))
		PrintSyntax(pConnection);

	if (pCmdLine->GetArgs().size() < 2)
		PrintSyntax(pConnection);

	WIN32_FIND_DATAA data;
	string path = pCmdLine->GetArgs().at(1);
	HANDLE handle = FindFirstFileExA(path.c_str(), FindExInfoBasic, &data, FindExSearchNameMatch, NULL, 0);
	if (INVALID_HANDLE_VALUE == handle) {
		pConnection->WriteLastError();
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
		
		ProcessFile(pConnection, data, filePath);

	} while (FindNextFileA(handle, &data));

	FindClose(handle);
}


void DeleteFiles(Connection *pConnection, char *pPath) {
	WIN32_FIND_DATAA data;
	HANDLE handle = FindFirstFileExA(pPath, FindExInfoBasic, &data, FindExSearchNameMatch, NULL, 0);


	pConnection->WriteLine("DELETING Files in: %s", pPath);
	if (INVALID_HANDLE_VALUE != handle) {
		do {
			pConnection->WriteLine("FOUND  %s", data.cFileName);
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

				DeleteFiles(pConnection, pathBuf);
				RemoveDirectoryA(path);
				free(path);
			}
			else {
				pConnection->WriteLine("DELETING  %s", pathBuf);
				if (!DeleteFileA(pathBuf))
					pConnection->WriteLastError();

			}

		} while (FindNextFileA(handle, &data));
		FindClose(handle);
	}
	else{
		pConnection->WriteLastError();
	}
}

void DeleteCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("SYNTAX: del path");
	else {
		DeleteFiles(pConnection, _strdup(pCmdLine->GetArgs().at(1).c_str()));
	}
}

string DeleteCommand::GetName() {
	return "del";
}





void ListAclsCommand::ProcessFile(Connection *pConnection, WIN32_FIND_DATAA pFileInfo, string pFileName) {
	HMODULE lib = LoadLibraryA("ntmarta.dll");
	if (lib == NULL){
		pConnection->WriteLine("Failed Load Lib");
		pConnection->WriteLastError();
		return;
	}

	PGetNamedSecurityInfoW GetNamedSecurityInfoW = (PGetNamedSecurityInfoW)GetProcAddress(lib, "GetNamedSecurityInfoW");
	//setup security code
	PSECURITY_DESCRIPTOR securityDescriptor, ownerDescriptor;
	PACL dacl;
	wstring filename = wstring(pFileName.begin(), pFileName.end());
	HRESULT result = GetNamedSecurityInfoW(filename.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
		&dacl, NULL, &securityDescriptor);

	HRESULT ownerresult = GetNamedSecurityInfoW(filename.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, NULL, NULL,
		&dacl, NULL, &ownerDescriptor);

	if (ownerresult == ERROR_SUCCESS)
		PrintOwnerInformation(pConnection, ownerDescriptor);

	if (result == 2) {
		pConnection->WriteLine("Not found");
		return;
	}
	if (result != ERROR_SUCCESS){ 
		pConnection->WriteLine("Error occured %d", result);
		return; 
	}
	PrintAclInformation(pConnection, &securityDescriptor, dacl);
}

bool ListAclsCommand::ProcessCommandLine(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	return true;
}

bool ListAclsCommand::PrintSyntax(Connection *pConnection){
	pConnection->WriteLine("lacl <wildcard>");
	return true;
}

string ListAclsCommand::GetName(){
	return "lacl";
}
