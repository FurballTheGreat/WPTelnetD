#include "pch.h"
#include "DirectoryCommands.h"

void RmdirCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("SYNTAX: rmdir path");
	else {
		if (!RemoveDirectoryA(pCmdLine->GetArgs().at(1).c_str()))
			pConnection->WriteLastError();
	}
}

string RmdirCommand::GetName() {
	return "rmdir";
}

void MkdirCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("SYNTAX: mkdir path");
	else {
		if (!CreateDirectoryA(pCmdLine->GetArgs().at(1).c_str(), NULL))
			pConnection->WriteLastError();
	}
}

string MkdirCommand::GetName() {
	return "mkdir";
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

void DirCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
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
				pConnection->WriteLine("%14s %-14s %-5s %s", timeStr, "    DIR  ", attrStr, data.cFileName);
				dirs++;
			}
			else {

				pConnection->WriteLine("%14s %14u %-5s %s", timeStr, data.nFileSizeLow, attrStr, data.cFileName);
				files++;
				fileTotal += data.nFileSizeLow;
			}

		} while (FindNextFileA(handle, &data));
		pConnection->WriteLine("Finished listing %d dirs and %d files totalling %u bytes", dirs, files, fileTotal);
	}
	else{
		pConnection->WriteLastError();
	}

}

string DirCommand::GetName() {
	return "dir";
}


void CdCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("ERROR: You must specify a path");
	else {
		if (!SetCurrentDirectoryA(pCmdLine->GetArgs().at(1).c_str()))
		{
			pConnection->WriteLastError();
		}
	}
}

string CdCommand::GetName() {
	return "cd";
}
