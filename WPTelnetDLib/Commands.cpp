#pragma once
#include "pch.h"
#include "Commands.h"
#include "CommandProcessor.h"
#include "Console.h"


using namespace std;

HelpCommand::HelpCommand(string pIntro, vector<Command*> *pCommands)
{
	_intro = pIntro;
	_commands = pCommands;
}

void HelpCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	pConsole->ResetStyle();
	pConsole->SetForeground(Yellow);
	pConsole->SetBold(true);
	pConsole->WriteLine("%-12s %-32s %s", "Command", "Parameters", "Description");
	pConsole->ResetStyle();
	for (auto it = _commands->begin(); it != _commands->end(); ++it)
	{
		auto info = (*it)->GetInfo();
		auto name = info.GetName();
		auto params = info.GetParams();
		pConsole->SetForeground(Cyan);
		pConsole->Write("%-12s ",name.c_str());
		pConsole->SetForeground(Green);
		pConsole->Write("%-32s ", params.c_str());
		pConsole->SetForeground(White);
		pConsole->WriteLine(info.GetDescription());
	}
}

CommandInfo HelpCommand::GetInfo() {
	return CommandInfo("help", "", "Shows this screen"); 
}

#undef GetEnvironmentStrings
extern "C" WINBASEAPI
_NullNull_terminated_
LPCH
WINAPI
GetEnvironmentStrings(
VOID
);

void EnvCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	LPCH env;	
	env = GetEnvironmentStrings();
	pConsole->WriteLine("Environment Strings");
	while(*env) {
		pConsole->WriteLine("%s", env);
		env = env+strlen(env)+1;			
	}

}

CommandInfo EnvCommand::GetInfo() {
	return CommandInfo("env", "", "List all environment variables.");
}

void EchoCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	string text = pCmdLine->GetParametersAsLine().GetRaw();
	pConsole->WriteLine(text);
}

CommandInfo EchoCommand::GetInfo() {
	return CommandInfo("echo", "<string>", "Echo a string to console.");
}


void ListPrivsCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, GetCurrentProcessId());
	HANDLE hToken;
	if (OpenProcessToken(
		hProc,
		TOKEN_QUERY,
		&hToken) == 0)
	{
		pConsole->WriteLine("I was unable to execute OpenProcessToken\r\nErrorCode: %d", GetLastError());
		return ;
	}


	DWORD SizeReturned = 0;

	// Pass NULL pointer to return the size needed
	if (!GetTokenInformation(hToken, TokenPrivileges, NULL, SizeReturned, &SizeReturned))
	{
		pConsole->WriteLine("1 GetTokenInformation failed. We needed a buffer this big: %d.", SizeReturned);
		pConsole->WriteLine("Error Code: %d\r\n", GetLastError());
	
	}

	TOKEN_PRIVILEGES *TokenPrivilegesType = (PTOKEN_PRIVILEGES)malloc(SizeReturned);

	if (!GetTokenInformation(hToken, TokenPrivileges, TokenPrivilegesType, SizeReturned, &SizeReturned))
	{
		pConsole->WriteLine("2 GetTokenInformation failed. We needed a buffer this big: %d. But ours was this big: %d", SizeReturned, sizeof(TokenPrivilegesType));
		pConsole->WriteLine("Error Code: %d", GetLastError());
		return;
	}
	pConsole->WriteLine("Privilages (%d)",  TokenPrivilegesType->PrivilegeCount);
	for (int i = 0; i < TokenPrivilegesType->PrivilegeCount; i++) {
		wchar_t privname[1024];
		
		DWORD privlen = 1024;
	
		if (!LookupPrivilegeNameW(NULL, &TokenPrivilegesType->Privileges[i].Luid, privname, &privlen)) {
			pConsole->WriteLine(" +unknown %X:%X (Error: %d)", TokenPrivilegesType->Privileges[i].Luid.HighPart, TokenPrivilegesType->Privileges[i].Luid.LowPart, GetLastError());
		}
		else {
			wstring privNameW(privname);
			string privNameA(privNameW.begin(), privNameW.end());
		
			pConsole->WriteLine(" +%s", privNameA.c_str());
		}
	}
	free(TokenPrivilegesType);

	// Pass NULL pointer to return the size needed
	SizeReturned = 0;
	if (!GetTokenInformation(hToken, TokenGroups, NULL, SizeReturned, &SizeReturned))
	{
		pConsole->WriteLine("1 GetTokenInformation failed. We needed a buffer this big: %d.", SizeReturned);
		pConsole->WriteLine("Error Code: %d\r\n", GetLastError());

	}
	TOKEN_GROUPS* groups = (TOKEN_GROUPS *)malloc(SizeReturned);
	if (!GetTokenInformation(hToken, TokenGroups, groups, SizeReturned, &SizeReturned))
	{
		pConsole->WriteLine("2 GetTokenInformation failed. We needed a buffer this big: %d. But ours was this big: %d", SizeReturned, sizeof(TokenPrivilegesType));
		pConsole->WriteLine("Error Code: %d", GetLastError());
		return;
	}

	pConsole->WriteLine("Capabilities");
	for (int i = 0; i < groups->GroupCount; i++) {
		wchar_t privname[1024];
		wchar_t donname[1024];
		char privnameA[1024];
		DWORD privlen = 1024;
		DWORD domlen = 1024;
		SID_NAME_USE use;
		if (!LookupAccountSidW(NULL, groups->Groups[i].Sid, privname, &privlen,donname,&domlen,&use )) {
			pConsole->WriteLine(" +unknown %X (Error: %d)", groups->Groups[i].Sid, GetLastError());
		}
		else {
			wcstombs(privnameA, privname, sizeof(privnameA));
			pConsole->WriteLine(" +%s", privnameA);
		}
	}
	free(groups);


}

CommandInfo ListPrivsCommand::GetInfo() {
	return CommandInfo("listprivs", "", "List all privileges assigned to current security token.");
}

ExitCommand::ExitCommand(string pDescription)
{
	_description = pDescription;
	_exiting = false;
}

void ExitCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine)
{
	_exiting = true;
}

CommandInfo  ExitCommand::GetInfo()
{
	return CommandInfo("exit", "", _description);
}

bool ExitCommand::IsExiting()
{
	return _exiting;
}





