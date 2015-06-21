#include "pch.h"
#include "ServiceCommands.h"
#include "ServiceControl.h"
#include "Commands.h"
#include "TerminalHelper.h"


void ScCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine)
{
	vector<Command *> commands = vector<Command *>();
	ExitCommand exit("Exit service control.");
	commands.push_back(static_cast<Command*>(new HelpCommand("",
	                                                         &commands)));
	commands.push_back(static_cast<Command*>(&exit));
	commands.push_back(static_cast<Command*>(new ScListCommand(false)));
	commands.push_back(static_cast<Command*>(new ScListCommand(true)));
	commands.push_back(static_cast<Command*>(new ScStartCommand()));
	commands.push_back(static_cast<Command*>(new ScStopCommand()));
	if (pCmdLine->GetArgs().size() == 1) {
		CommandProcessor processor = CommandProcessor(&commands);
		pConsole->WriteLine("Service Control - Type HELP for assistance.");


		while (!exit.IsExiting()) {
			pConsole->Write(">");
			pConsole->SetForeground(Yellow);
			pConsole->SetBold(true);
			std::string line = pConsole->ReadLine();
			pConsole->ResetStyle();
			ParsedCommandLine commandLine(line, pCmdLine->GetHost());

			if (!processor.ProcessCommandLine(pConsole, &commandLine)) {
				
			}
		}
	}
	else {
		CommandProcessor processor = CommandProcessor(&commands);
		ParsedCommandLine line = pCmdLine->GetParametersAsLine();
		processor.ProcessCommandLine(pConsole, &line);
	}
}
CommandInfo ScCommand::GetInfo()
{
	return CommandInfo("sc", "", "Service Control");
}

void ScStartCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine)
{
	if (pCmdLine->GetArgs().size() < 2) {
		pConsole->SetForeground(Cyan);
		pConsole->SetBold(true);
		pConsole->WriteLine("SYNTAX: start <servicename>");
		pConsole->ResetStyle();

		return;
	}
	
	try {
		ServiceControl services;
		services.StartService(pCmdLine->GetArgs().at(1));
	} catch (DWORD error)
	{
		pConsole->SetForeground(Red);
		pConsole->SetBold(true);
		std::string errorStr = GetErrorAsString(error);
		pConsole->WriteLine("ERROR(%d): %s", error, errorStr.c_str());
		pConsole->ResetStyle();
	}
}
CommandInfo ScStartCommand::GetInfo()
{
	return CommandInfo("start", "<servicename>", "Start service");
}


void ScStopCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine)
{
	if (pCmdLine->GetArgs().size() < 2) {
		pConsole->SetForeground(Cyan);
		pConsole->SetBold(true);
		pConsole->WriteLine("SYNTAX: stop <servicename>");
		pConsole->ResetStyle();

		return;
	}

	try {
		ServiceControl services;
		services.StopService(pCmdLine->GetArgs().at(1));
	}
	catch (DWORD error)
	{
		pConsole->SetForeground(Red);
		pConsole->SetBold(true);
		std::string errorStr = GetErrorAsString(error);
		pConsole->WriteLine("ERROR(%d): %s", error, errorStr.c_str());
		pConsole->ResetStyle();
	}
}
CommandInfo ScStopCommand::GetInfo()
{
	return CommandInfo("stop", "<servicename>", "Stop service");
}

ScListCommand::ScListCommand(bool pDrivers)
{
	_drivers = pDrivers;
}
void ScListCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine)
{
	DWORD serviceType;
	DWORD serviceState;

	if (_drivers)
		serviceType = SERVICE_DRIVER;
	else
		serviceType = SERVICE_WIN32 | SERVICE_INTERACTIVE_PROCESS;

	if (pCmdLine->GetArgs().size() < 2) {
		serviceState = SERVICE_ACTIVE;
	} else
	{
		auto stateStr = pCmdLine->GetArgs().at(1);
		if (stateStr == "all")
			serviceState = SERVICE_STATE_ALL;
		else if (stateStr == "inactive")
			serviceState = SERVICE_INACTIVE;
		else
			serviceState = SERVICE_ACTIVE;
	}

	try {
		ServiceControl services;
		auto result = services.EnumServices(serviceType, serviceState);
		pConsole->ResetStyle();
		pConsole->SetForeground(Yellow);
		pConsole->SetBold(true);
		pConsole->WriteLine(L"%-24s%-5s %-10s %-17s %s", L"Name", L"PID", L"Status", L"Type", L"Display Name");
		pConsole->ResetStyle();
		for (auto it = result.begin(); it != result.end(); ++it)
		{
			std::wstring status;
			std::wstring type;
			std::wstring name = it->GetName();
			wstring displayName = it->GetDisplayName();

			switch (it->GetState())
			{
			case Stopped:
				status = L"Stopped";
				break;
			case StartPending: 
				status = L"Starting";
				break;
			case StopPending: 
				status = L"Stopping";
				break;
			case Running: 
				status = L"Running";
				break;
			case ContinuePending: 
				status = L"Continueing";
				break;
			case PausePending: 
				status = L"Pausing";
				break;
			case Paused: 
				status = L"Paused";
				break;
			default: 
				status = L"Unknown";
				break;
			}

			switch (it->GetType())
			{
			case FileSystemDriver: 
				type = L"FileSystemDriver";
				break;
			case KernelDriver: 
				type = L"KernelDriver";
				break;
			case Win32OwnProcess: 
				type = L"OwnProcess";
				break;
			case Win32ShareProcess: 
				type = L"SharedProcess";
				break;
			default: break;
			}
			pConsole->SetForeground(Cyan);
			pConsole->Write(L"%-24s%-5d", name.c_str(), it->GetPID());
			pConsole->SetForeground(Green);
			pConsole->Write(L" %-10s", status.c_str());
			pConsole->SetForeground(Cyan);
			pConsole->Write(L" %-17s ", type.c_str());
			pConsole->SetForeground(White);
			pConsole->WriteLine(displayName);
		}
	}
	catch (DWORD error)
	{
		pConsole->SetForeground(Red);
		pConsole->SetBold(true);
		std::string errorStr = GetErrorAsString(error);
		pConsole->WriteLine("ERROR(%d): %s", error, errorStr.c_str());
		pConsole->ResetStyle();
	}
}
CommandInfo ScListCommand::GetInfo()
{
	if (_drivers)
		return CommandInfo("drivers", "[all|active|inactive]", "List driver services.");
	else
		return CommandInfo("services", "[all|active|inactive]", "List services.");
}
