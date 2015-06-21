#include "pch.h"
#include "DirectoryCommands.h"
#include "TerminalHelper.h"

void RmdirCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: rmdir path");
	else {
		if (!RemoveDirectoryA(pCmdLine->GetArgs().at(1).c_str()))
			pConsole->WriteLine(GetLastErrorAsString());		
	}
}

CommandInfo RmdirCommand::GetInfo() {
	return CommandInfo("rmdir", "<path>", "Remove a directory.");
}

void MkdirCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: mkdir path");
	else {
		if (!CreateDirectoryA(pCmdLine->GetArgs().at(1).c_str(), NULL))
			pConsole->WriteLine(GetLastErrorAsString());
	}
}

CommandInfo MkdirCommand::GetInfo() {
	return CommandInfo("mkdir", "<path>", "Create a directory.");
}


BOOL DirCommand::GetLastWriteTime(WIN32_FIND_DATAA *data, char *string, DWORD dwSize)
{

	SYSTEMTIME stUTC, stLocal;



	// Convert the last-write time to local time.
	FileTimeToSystemTime(&data->ftLastWriteTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	// Build a string showing the date and time.
	_snprintf(string, dwSize,
		"%02d/%02d/%d  %02d:%02d",
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	return TRUE;

}

void DirCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	const char *search;

	if (pCmdLine->GetArgs().size()<2)
		search = "*.*";
	else {
		string s = pCmdLine->GetArgs().at(1);
		search = _strdup(s.c_str());
	}
	WIN32_FIND_DATAA data;
	HANDLE handle = FindFirstFileExA(search, FindExInfoBasic, &data, FindExSearchNameMatch, NULL, 0);
	int dirs = 0, files = 0, fileTotal = 0;;
	if (INVALID_HANDLE_VALUE != handle) {
		do {
			char timeStr[100];
			GetLastWriteTime(&data, timeStr, sizeof(timeStr));
			char attrStr[100];
			memset(attrStr, 0, sizeof(attrStr));
			int index = 0;

			if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				attrStr[index++] = 'r';
			if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				attrStr[index++] = 'h';
			if (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				attrStr[index++] = 's';
			if (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				attrStr[index++] = 'a';

			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				pConsole->SetForeground(Cyan);
				pConsole->WriteLine("%14s %-14s %-5s %s", timeStr, "    DIR  ", attrStr, data.cFileName);
				dirs++;
			}
			else {
				pConsole->SetForeground(Green);
				pConsole->WriteLine("%14s %14u %-5s %s", timeStr, data.nFileSizeLow, attrStr, data.cFileName);
				files++;
				fileTotal += data.nFileSizeLow;
			}

		} while (FindNextFileA(handle, &data));
		pConsole->SetForeground(Yellow);
		pConsole->WriteLine("%d Dir(s) and %d File(s) %u bytes", dirs, files, fileTotal);
		pConsole->WriteLine();
	}
	else{
		pConsole->WriteLine(GetLastErrorAsString());
	}

}

CommandInfo DirCommand::GetInfo() {
	return CommandInfo("dir", "[path/wildcard]", "List the contents of a directory.");
}


void CdCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("ERROR: You must specify a path");
	else {
		if (!SetCurrentDirectoryA(pCmdLine->GetArgs().at(1).c_str()))
		{
			pConsole->WriteLine(GetLastErrorAsString());
		}
	}
}

CommandInfo CdCommand::GetInfo() {
	return CommandInfo("cd", "<path>", "Change current directory.");
}
