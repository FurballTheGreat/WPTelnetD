// Main Entry Point for TelnetD
//

#include "pch.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include<string.h>
#include<vector>

#include "TelnetD.h"
#include "Connection.h"
#include "ListenSocket.h"
#include "CommandProcessor.h"

#include "Commands.h"
#include "CertCommands.h"
#include "RegistryCommands.h"
#include "ProcessCommands.h"
#include "NetworkCommands.h"
#include "GuiCommands.h"
#include "FileCommands.h"
#include "ExperimentalCommands.h"
#include "DirectoryCommands.h"
#include "Commands.h"
#include "ProcessHost.h"
#include "FileHelpers.h"
#include "ServiceCommands.h"

#ifdef PHONE
#include<WPKernel.h>
#endif
#include <TelnetSession.h>
#include <StringConsole.h>


class ExecutionContext : public IExecutionContext
{

public:
	ExecutionContext() {

	}
	string GetVariable(string pName) {
		return "";
	};
	void SetVariable(string pName, string pValue) {
	};
};

void PrintPrompt(IConsole *pConsole) {
	char buf[1024];
	GetCurrentDirectoryA(sizeof(buf), buf);
	pConsole->ResetStyle();
	
	pConsole->Write("%s>", buf);
	
}

CommandProcessor* CreateProcessor(IExecutionContext *pExecutionContext, ExitCommand *pExit) {
	vector<Command *> *commands = new vector<Command *>();
	
	commands->push_back((Command*)new HelpCommand("",commands));
	commands->push_back((Command*)pExit);
#ifdef PHONE
	commands->push_back((Command*)new ToastCommand());
#endif
	commands->push_back((Command*)new CdCommand());
	commands->push_back((Command*)new TypeCommand());
	commands->push_back((Command*)new DirCommand());
	commands->push_back((Command*)new MkdirCommand());
	commands->push_back((Command*)new RmdirCommand());
	commands->push_back((Command*)new CopyCommand());
	commands->push_back((Command*)new DeleteCommand());
	commands->push_back((Command*)new MoveCommand());
	commands->push_back((Command*)new EnvCommand());
	commands->push_back((Command*)new DownloadCommand());
	commands->push_back((Command*)new PsCommand());
	commands->push_back((Command*)new KillCommand());
	commands->push_back((Command*)new BackgroundCommand());
	commands->push_back((Command*)new NetstatCommand());
	commands->push_back((Command*)new PostMessageCommand());
	commands->push_back((Command*)new EnumWindowsCommand());
	commands->push_back((Command*)new ListPrivsCommand());
	commands->push_back((Command*)new RegCommand());
	commands->push_back((Command*)new ScCommand());
	commands->push_back((Command*)new EchoCommand());
	commands->push_back((Command*)new ProvXmlCommand());


	commands->push_back((Command*)new AttribCommand());
	commands->push_back((Command*)new CertsCommand());
	commands->push_back((Command*)new ListAclsCommand());
	commands->push_back((Command*)new WhoAmICommand());
	commands->push_back((Command*)new LookupChamberSidCommand());
	commands->push_back((Command*)new CreateProcessInChamberCommand());
	commands->push_back((Command*)new TestCommand());
	commands->push_back((Command*)new TestCommand2());
	
	CommandProcessor *processor = new CommandProcessor(commands);
	commands->push_back((Command*)new RunFromCommand(pExecutionContext, processor));
	return processor;
	
}

bool ProcessLine(IConsole *pConsole, IExecutionContext *pExecutionContext, CommandProcessor *pProcessor,  string pLine) {
	ParsedCommandLine cmdLine = ParsedCommandLine(pLine, pExecutionContext);
	
	bool result = false;
	if (!pProcessor->ProcessCommandLine(pConsole,&cmdLine)) {
		string batName = GetCurrentDirectory() + "\\" + cmdLine.GetName() + ".bat";
		if (FileExists(batName)){
			TextFileReader *reader = new TextFileReader(batName);
			string batLine;
			while (reader->ReadLine(batLine)) {
				ParsedCommandLine batCmdLine = ParsedCommandLine(batLine, pExecutionContext);
				result = ProcessLine(pConsole, pExecutionContext, pProcessor, batLine);
				if (result)
					return result;
			}
		}
		else {
			ProcessHost host(pConsole);
			host.ExecuteProcess(pConsole, pLine.c_str());
		}
	}
	return result;
}

bool ExecuteCommand(char *pCommandLine){
	StringConsole console;
	ExecutionContext context;
	ExitCommand exit("Exit this instance of WPTelnetD");
	CommandProcessor*processor = CreateProcessor(&context, &exit);
	ProcessLine(&console, &context, processor, pCommandLine);
	return true;
}

bool ProcessConnection(SOCKET pSocket, char *pWelcomeInfo) {
	Connection *connection = new Connection(pSocket);
	TelnetSession session(connection);
	TelnetConsole console(&session);
	console.SendRenditionParam(SecondaryFont1);
	console.SetForeground(White);
	console.SetBackground(Blue);
	IConsole *con = &console;
	   con->WriteLine(L"┌────────────────────────────────────────┐");
	con->Write(L"│    ");
	con->Write(L"Welcome to the WP8 Telnet Server");
	con->WriteLine(L"    │");
	console.WriteLine(L"└────────────────────────────────────────┘");
	con->ResetStyle();
	console.WriteLine(pWelcomeInfo);
	console.WriteLine("");


	
	ExecutionContext context;
	ExitCommand exit("Exit this instance of WPTelnetD");
	CommandProcessor*processor = CreateProcessor(&context, &exit);
	PrintPrompt(&console);

	try {
		console.SetBold(true);
		console.SetForeground(Yellow);
		auto line = console.ReadLine();
		console.ResetStyle();
		while (!exit.IsExiting()) {
			if (ProcessLine(&console, &context, processor, line))
			{
				
			}
			PrintPrompt(&console);
			console.SetBold(true);
			console.SetForeground(Yellow);
			line = console.ReadLine();
			console.ResetStyle();
		}
	} catch (int error)
	{
		
	}	
	
	delete processor;
	return true;
}

int ListenForOneConnection(int pPort, SOCKET *pSocket, int *pWsaError, timeval *pTimeout) {
	ListenSocket *listener = new ListenSocket(pPort);

	if (!listener->Start()){
		*pWsaError = WSAGetLastError();
		return 1;
	}

	auto socket = listener->Accept(pTimeout);
	listener->Stop();
	delete listener;

	if (socket == INVALID_SOCKET){
		*pWsaError = WSAGetLastError();
		return 2;
	}

	*pSocket = socket;
	return ERROR_SUCCESS;
}

int ConnectTo(char *pIpAddress, int pPort, SOCKET *pSocket, int *pWsaError) {
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		*pWsaError = WSAGetLastError();
		return 1;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(pIpAddress);
	addr.sin_port = htons(pPort);

	HRESULT result = connect(sock, (SOCKADDR *)& addr, sizeof(addr));
	if (result == SOCKET_ERROR) {
		*pWsaError = WSAGetLastError();
		result = closesocket(sock);		
		return 2;
	}

	*pSocket = sock;
	return ERROR_SUCCESS;
}

bool InitNetworking(int *pWsaError) {
	WSADATA wsaData;
	int wsaret = WSAStartup(0x101, &wsaData);
	if (wsaret != 0){
		*pWsaError = WSAGetLastError();
		return false;
	}

	return true;
}


bool ShutDownNetworking() {
	WSACleanup();
	return true;
}

