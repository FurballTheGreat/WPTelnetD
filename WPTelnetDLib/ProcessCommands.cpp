#include "pch.h"
#include "ProcessCommands.h"
#include "TerminalHelper.h"

void KillCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: kill PID [exit code]");
	else {
		int num = atoi(pCmdLine->GetArgs().at(1).c_str());
		int exitCode = pCmdLine->GetArgs().size() > 2 ? atoi(pCmdLine->GetArgs().at(2).c_str()) : 0;
		HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, num);


		if (process == (HANDLE)-1) {
			pConsole->WriteLine(GetLastErrorAsString());
			return;
		}

		if (!TerminateProcess(process, exitCode))
			pConsole->WriteLine(GetLastErrorAsString());
	}
}

CommandInfo KillCommand::GetInfo() {
	return CommandInfo("kill", "<pid>", "Kills a process.");

}

void PsCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	HANDLE _handle = ::CreateToolhelp32Snapshot(0x2, 0);


	if (_handle == 0 || _handle == INVALID_HANDLE_VALUE)
	{
		pConsole->WriteLine("ERROR: Failed to create snapshot");
	}
	else {
		PROCESSENTRY32W _process;
		_process.dwSize = sizeof(_process);
		DWORD old = _process.dwSize;
		bool result = Process32FirstW(_handle, &_process);
		
		MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
		
		if (GlobalMemoryStatusEx(&memStatus)){
			pConsole->WriteLine("Physical memory available %dmb out of %dmb (%d%% load).", (DWORD)(memStatus.ullAvailPhys / (1024 * 1024)), (DWORD)(memStatus.ullTotalPhys / (1024 * 1024)), memStatus.dwMemoryLoad);
			pConsole->WriteLine("Virtual memory available %dmb out of %dmb.", (DWORD)(memStatus.ullAvailVirtual / (1024 * 1024)), (DWORD)(memStatus.ullTotalVirtual / (1024 * 1024)));
			pConsole->WriteLine("Pagefile available %dmb out of %dmb.", (DWORD)(memStatus.ullAvailPageFile / (1024 * 1024)), (DWORD)(memStatus.ullTotalPageFile / (1024 * 1024)));
			pConsole->WriteLine("");
				
		}
		else
			pConsole->WriteLine(GetLastErrorAsString());

		if (!result) {
			pConsole->WriteLine("ERROR: Got false from Process32First: %d %d %d %d %d", GetLastError(), _handle, sizeof(_process), _process.dwSize, old);
		}
		else {


			pConsole->WriteLine("************");
			pConsole->WriteLine("Process List");
			pConsole->WriteLine("************");
			pConsole->WriteLine("");
			char exeFile[1024];
			size_t outnum;
			wcstombs_s(&outnum, exeFile, _process.szExeFile, sizeof(exeFile));
			pConsole->WriteLine("%s (%d,%d,%d)", exeFile, _process.th32ProcessID, _process.th32ParentProcessID, _process.th32ModuleID);
			
			bool result = true;
			while (result) {
				result = Process32NextW(_handle, &_process);
				if (result) {
					wcstombs_s(&outnum, exeFile, _process.szExeFile, sizeof(exeFile));
					pConsole->WriteLine("%s (%d,%d,%d)", exeFile, _process.th32ProcessID, _process.th32ParentProcessID, _process.th32ModuleID);
				}
			}
			pConsole->WriteLine("");
		}

	}
}

CommandInfo PsCommand::GetInfo() {
	return CommandInfo("ps", "", "List all running processes.");
}


void WhoAmICommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<1)
		pConsole->WriteLine("SYNTAX: whoami");
	else {
		

		char buf[1024];
		ULONG size = sizeof(buf);
		if (!GetUserNameExA(NameSamCompatible, buf, &size)) {
			pConsole->WriteLine(GetLastErrorAsString());
		}
		else
			pConsole->WriteLine("You are '%s'", buf);


	}
}

CommandInfo WhoAmICommand::GetInfo() {
	return CommandInfo("whoami", "", "Query which user this process is running as.");
}

void LookupChamberSidCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
#ifdef PHONE
	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: lookupchamber {id}");
	else {
		LPWSTR sid = nullptr;
		string idA = pCmdLine->GetArgs().at(1);
		wstring idW = wstring(idA.begin(), idA.end());
		HRESULT ret = ::GetChamberSidFromId((PWCHAR)idW.c_str(), &sid);
		if (ret!=0)
			pConsole->WriteLine("ERROR: HResult = %d", ret);

		if (sid) {
			pConsole->Write("SID: ");
			pConsole->WriteLine(sid);

			LocalFree(sid);
		}
	}
#endif
}

CommandInfo LookupChamberSidCommand::GetInfo() {
	return CommandInfo("lookchamber", "<chamberId>", "Query the SID associated with a chamber.");
}



void CreateProcessInChamberCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<4)
		pConsole->WriteLine("SYNTAX: execchamber sid|app {id} {exe} {params}");
	else {
		STARTUPINFOW si;
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(STARTUPINFOW);
		
		PROCESS_INFORMATION pi;

		string id = pCmdLine->GetArgs().at(2);
		wstring idW = wstring(id.begin(), id.end());
		PWSTR chamberSid = nullptr, appId = nullptr;
		if (pCmdLine->GetArgs().at(1) == "sid")
			chamberSid = (PWSTR)idW.c_str();
		else
			appId = (PWSTR)idW.c_str();

		string exe = pCmdLine->GetArgs().at(3);
		wstring exew = wstring(exe.begin(), exe.end());
		PWSTR exewstr = nullptr;
		if (exew.length() > 0)
			exewstr = (PWSTR)exew.c_str();

		wstring commandLine = L"";
		if (pCmdLine->GetArgs().size() > 4){
			string commandLineA = pCmdLine->GetArgs().at(4);
			commandLine = wstring(commandLineA.begin(), commandLineA.end());
		}
#ifdef PHONE
		HRESULT ret = CreateProcessInChamber(
			chamberSid,
			appId,
			exewstr,
			(PWSTR)commandLine.c_str(),
			TRUE,
			0x0,
			nullptr,
			&si,
			&pi);


		if (ret == 0)
			pConsole->WriteLine("ProcessID: %d, HProcess: %d, ThreadID: %d, HThread %d", pi.dwProcessId, pi.hProcess, pi.dwThreadId, pi.hThread);
		else
			pConsole->WriteLine(GetErrorAsString(ret));
#endif
	}
}

CommandInfo CreateProcessInChamberCommand::GetInfo() {
	return CommandInfo("execchamber", "<sid|app> <id> <exe> [params]", "Execute process in chamber.");
}

void BackgroundCommand::ProcessCommand(IConsole * pConsole, ParsedCommandLine * pCmdLine)
{

	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("Must at least specify a path to a binary.");
		return;
	}

	auto line = pCmdLine->GetParametersAsLine();
	auto lin = line.GetRaw();

	PROCESS_INFORMATION processInfo;
	STARTUPINFOA startInfo;
	BOOL isSuccess = FALSE;

	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&startInfo, sizeof(STARTUPINFOA));

	startInfo.cb = sizeof(STARTUPINFOA);

	
	isSuccess = CreateProcessA(
		NULL,
		(char*)lin.c_str(),
		NULL,
		NULL,
		TRUE,
		0x00000010,
		NULL,
		NULL,
		(LPSTARTUPINFOA)&startInfo,
		(LPPROCESS_INFORMATION)&processInfo);

	if (!isSuccess) {
		pConsole->WriteLine(GetLastErrorAsString());
		
	}
	else
	{
		pConsole->WriteLine("PID = %d", processInfo.dwProcessId);
	}
}

CommandInfo BackgroundCommand::GetInfo()
{
	return CommandInfo("bg", "<commandpath> [param1] [param2] ...", "Runs a process in the background.");
}
