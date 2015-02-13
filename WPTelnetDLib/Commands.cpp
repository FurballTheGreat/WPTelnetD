#pragma once
#include "pch.h"
#include "Commands.h"
#include "CommandProcessor.h"
#include "Networking.h"


using namespace std;

void HelpCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	pConnection->WriteLine("help - Shows this screen.");
	pConnection->WriteLine("ps - Shows a list of processes.");
	pConnection->WriteLine("kill - Kills a process.");
	pConnection->WriteLine("exit - Exits THIS process");
	pConnection->WriteLine("whoami - Displays who this process is running as.");
	pConnection->WriteLine("netstat - Shows a list ip connections.");
	pConnection->WriteLine("env - Shows a list of environment variables.");
	pConnection->WriteLine("down <url> <saveasfile> - Downloads a file.");
	pConnection->WriteLine("cd <path> - Change directory/drive.");
	pConnection->WriteLine("attrib <path> <attributes> - Change directory/file attributes.");
	pConnection->WriteLine("type <path> - Prints contents of file to console.");
	pConnection->WriteLine("lacl <file> - Prints the access control list for a path/file.");
	pConnection->WriteLine("del <path> - Deletes a file.");
	pConnection->WriteLine("move <oldname> <newname> - Move/rename a file.");
	pConnection->WriteLine("copy <oldname> <newname> - Rename a file.");
	pConnection->WriteLine("mkdir <path> - Create a directory.");
	pConnection->WriteLine("rmdir <path> - Remove a directory.");
	pConnection->WriteLine("dir [filter] - List Directory.");
	pConnection->WriteLine("ewin - Enumerates windows visible to this process.");
	pConnection->WriteLine("postmessage - Posts a message using PostMessageW.");
	pConnection->WriteLine("listprivs - Posts a message using PostMessageW");
	pConnection->WriteLine("reg - Open the WPTD registry editor.");
	pConnection->WriteLine("toast - Open the WPTD registry editor.");
	pConnection->WriteLine("echo - Open the WPTD registry editor.");
	pConnection->WriteLine("provxml - Open the WPTD registry editor.");
}

string HelpCommand::GetName() {
	return "help";
}

#undef GetEnvironmentStrings
extern "C" WINBASEAPI
_NullNull_terminated_
LPCH
WINAPI
GetEnvironmentStrings(
VOID
);

void EnvCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	LPCH env;	
	env = GetEnvironmentStrings();
	pConnection->WriteLine("Environment Strings");
	while(*env) {
		pConnection->WriteLine("%s", env);
		env = env+strlen(env)+1;			
	}

}

string EnvCommand::GetName() {
	return "env";
}

void EchoCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	string text = pCmdLine->GetParametersAsLine().GetRaw();
	pConnection->WriteLine(text);
}

string EchoCommand::GetName() {
	return "echo";
}


void ListPrivsCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, GetCurrentProcessId());
	HANDLE hToken;
	if (OpenProcessToken(
		hProc,
		TOKEN_QUERY,
		&hToken) == 0)
	{
		pConnection->WriteLine("I was unable to execute OpenProcessToken\r\nErrorCode: %d", GetLastError());
		return ;
	}


	DWORD SizeReturned = 0;

	// Pass NULL pointer to return the size needed
	if (!GetTokenInformation(hToken, TokenPrivileges, NULL, SizeReturned, &SizeReturned))
	{
		pConnection->WriteLine("1 GetTokenInformation failed. We needed a buffer this big: %d.", SizeReturned);
		pConnection->WriteLine("Error Code: %d\r\n", GetLastError());
	
	}

	TOKEN_PRIVILEGES *TokenPrivilegesType = (PTOKEN_PRIVILEGES)malloc(SizeReturned);

	if (!GetTokenInformation(hToken, TokenPrivileges, TokenPrivilegesType, SizeReturned, &SizeReturned))
	{
		pConnection->WriteLine("2 GetTokenInformation failed. We needed a buffer this big: %d. But ours was this big: %d", SizeReturned, sizeof(TokenPrivilegesType));
		pConnection->WriteLine("Error Code: %d", GetLastError());
		return;
	}
	pConnection->WriteLine("Privilages (%d)",  TokenPrivilegesType->PrivilegeCount);
	for (int i = 0; i < TokenPrivilegesType->PrivilegeCount; i++) {
		wchar_t privname[1024];
		
		DWORD privlen = 1024;
	
		if (!LookupPrivilegeNameW(NULL, &TokenPrivilegesType->Privileges[i].Luid, privname, &privlen)) {
			pConnection->WriteLine(" +unknown %X:%X (Error: %d)", TokenPrivilegesType->Privileges[i].Luid.HighPart, TokenPrivilegesType->Privileges[i].Luid.LowPart, GetLastError());
		}
		else {
			wstring privNameW(privname);
			string privNameA(privNameW.begin(), privNameW.end());
		
			pConnection->WriteLine(" +%s", privNameA.c_str());
		}
	}
	free(TokenPrivilegesType);

	// Pass NULL pointer to return the size needed
	SizeReturned = 0;
	if (!GetTokenInformation(hToken, TokenGroups, NULL, SizeReturned, &SizeReturned))
	{
		pConnection->WriteLine("1 GetTokenInformation failed. We needed a buffer this big: %d.", SizeReturned);
		pConnection->WriteLine("Error Code: %d\r\n", GetLastError());

	}
	TOKEN_GROUPS* groups = (TOKEN_GROUPS *)malloc(SizeReturned);
	if (!GetTokenInformation(hToken, TokenGroups, groups, SizeReturned, &SizeReturned))
	{
		pConnection->WriteLine("2 GetTokenInformation failed. We needed a buffer this big: %d. But ours was this big: %d", SizeReturned, sizeof(TokenPrivilegesType));
		pConnection->WriteLine("Error Code: %d", GetLastError());
		return;
	}

	pConnection->WriteLine("Capabilities");
	for (int i = 0; i < groups->GroupCount; i++) {
		wchar_t privname[1024];
		wchar_t donname[1024];
		char privnameA[1024];
		DWORD privlen = 1024;
		DWORD domlen = 1024;
		SID_NAME_USE use;
		if (!LookupAccountSidW(NULL, groups->Groups[i].Sid, privname, &privlen,donname,&domlen,&use )) {
			pConnection->WriteLine(" +unknown %X (Error: %d)", groups->Groups[i].Sid, GetLastError());
		}
		else {
			wcstombs(privnameA, privname, sizeof(privnameA));
			pConnection->WriteLine(" +%s", privnameA);
		}
	}
	free(groups);


}

string ListPrivsCommand::GetName() {
	return "listprivs";
}






