#include "pch.h"
#include "GuiCommands.h"

#ifdef PHONE
#define STRSIZE 1024
void ToastCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("SYNTAX: Toast url [title] [content] [guid]");
	else {

		CLSID cguid;
		ZeroMemory(&cguid, sizeof(CLSID));
		wstring url;
		wstring title;
		wstring content;


		size_t convertedChars = 0;
		string urla = pCmdLine->GetArgs().at(1);
		url = wstring(urla.begin(), urla.end());
		
		if (pCmdLine->GetArgs().size() > 2)
		{
			string titlea = pCmdLine->GetArgs().at(2);
			title = wstring(titlea.begin(), titlea.end());
		}
		else
			title = L"Click Me";

		if (pCmdLine->GetArgs().size() > 3)
		{
			string contenta = pCmdLine->GetArgs().at(3);
			content = wstring(contenta.begin(),contenta.end());
		}
		else
			content = L"WPTD Toast";

		if (pCmdLine->GetArgs().size() > 4)
		{
			string guida = pCmdLine->GetArgs().at(4);
			wstring guidw = wstring(guida.begin(), guida.end());
			IIDFromString(guidw.c_str(), &cguid);
		}

		TOAST_MESSAGE tm;
		tm.guid = cguid;
		tm.lpTitle = title.c_str();
		tm.lpContent = content.c_str();
		tm.lpUri = url.c_str();
		tm.lpType = L"Default";

		HRESULT result = Shell_PostMessageToast(&tm);
		pConnection->WriteLine("Result from Shell_PostMessageToast: %d", result);
	}
}

string ToastCommand::GetName() {
	return "toast";
}
#endif


Connection *_cmdCon;

void printWindow(char*prefix, HWND hwnd) {
	wchar_t class_name[80];
	wchar_t title[80];
	if (!GetClassNameW(hwnd, class_name, sizeof(class_name)))
		wcscpy(class_name, L"*Unknown Class*");
	if (!GetWindowTextW(hwnd, title, sizeof(title)))
		wcscpy(title, L"*Untitled*");
	DWORD processId = 0;
	DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);
	char classA[1024], titleA[1024];

	wcstombs(classA, class_name, 1024);
	wcstombs(titleA, title, 1024);

	_cmdCon->WriteLine("%s[%d] - %s %s (%d, %d)\n", prefix, hwnd, classA, titleA, processId, threadId);

}

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
	printWindow("  ", hwnd);
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	printWindow("", hwnd);
	EnumChildWindows(hwnd, EnumChildWindowsProc, 0);
	return TRUE;
}


void EnumWindowsCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	pConnection->WriteLine("************");
	_cmdCon = pConnection;
	EnumWindows(EnumWindowsProc, NULL);

}

string EnumWindowsCommand::GetName() {
	return "ewin";
}

void PostMessageCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	pConnection->WriteLine("************");
	if (pCmdLine->GetArgs().size() < 5)
	{
		pConnection->WriteLine("SYNTAX: POST hwnd msgnum wparam lparam");
		return;
	}

	HWND hwnd = (HWND)atoi(pCmdLine->GetArgs().at(1).c_str());
	DWORD msg = (DWORD)atoi(pCmdLine->GetArgs().at(2).c_str());
	DWORD wparam = (DWORD)atoi(pCmdLine->GetArgs().at(3).c_str());
	DWORD lparam = (DWORD)atoi(pCmdLine->GetArgs().at(4).c_str());


	if (!PostMessageW(hwnd, msg, wparam, lparam)) {
		pConnection->WriteLine("Failed to post message %d", GetLastError());
	}
	else
		pConnection->WriteLine("Successfully posted message");

}

string PostMessageCommand::GetName() {
	return "post";
}

