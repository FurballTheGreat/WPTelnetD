// Main Entry Point for TelnetD
//

#include "pch.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include<string.h>
#include<vector>

#include "TelnetD.h"
#include "Networking.h"
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
#ifdef PHONE
#include<WPKernel.h>
#endif


class CmdProcessHost : public ICommandProcessorHost
{

public:
	CmdProcessHost() {

	}
	virtual void PrintPrompt(Connection *pConnection) {
		char buf[1024];
		GetCurrentDirectoryA(sizeof(buf), buf);
		pConnection->Write("%s>", buf);
	}
	virtual void UnhandledLine(Connection *pConnection, string pLine){
		ProcessHost host(pConnection);
		host.ExecuteProcess(pConnection, pLine.c_str());
	}
};

bool ProcessConnection(SOCKET pSocket, char *pWelcomeInfo) {
	Connection *connection = new Connection(pSocket);
	connection->WriteLine("******************************************");
	connection->WriteLine("*    Welcome to the WP8 Telnet Server -  *");
	connection->WriteLine("******************************************");
	connection->WriteLine(pWelcomeInfo);
	connection->WriteLine("");
	
	Sleep(100);
	//LaunchAnotherInstance(connection);
	vector<BaseCommand *> commands = vector<BaseCommand *>();
	commands.push_back((BaseCommand*)new HelpCommand());
#ifdef PHONE
	commands.push_back((BaseCommand*)new ToastCommand());
#endif
	commands.push_back((BaseCommand*)new CdCommand());
	commands.push_back((BaseCommand*)new TypeCommand());
	commands.push_back((BaseCommand*)new DirCommand());
	commands.push_back((BaseCommand*)new MkdirCommand());
	commands.push_back((BaseCommand*)new RmdirCommand());
	commands.push_back((BaseCommand*)new CopyCommand());
	commands.push_back((BaseCommand*)new DeleteCommand());
	commands.push_back((BaseCommand*)new MoveCommand());
	commands.push_back((BaseCommand*)new EnvCommand());
	commands.push_back((BaseCommand*)new DownloadCommand());
	commands.push_back((BaseCommand*)new PsCommand());
	commands.push_back((BaseCommand*)new KillCommand());
	commands.push_back((BaseCommand*)new NetstatCommand());
	commands.push_back((BaseCommand*)new PostMessageCommand());
	commands.push_back((BaseCommand*)new EnumWindowsCommand());
	commands.push_back((BaseCommand*)new ListPrivsCommand());
	commands.push_back((BaseCommand*)new RegCommand());


	commands.push_back((BaseCommand*)new AttribCommand());
	commands.push_back((BaseCommand*)new CertsCommand());
	commands.push_back((BaseCommand*)new TestCommand());
	commands.push_back((BaseCommand*)new TestCommand2());
	
	CmdProcessHost host;

	CommandProcessor processor = CommandProcessor(commands, (ICommandProcessorHost*)&host);
	processor.PrintPrompt(connection);
	const char* line = connection->ReadLine();

	while (line!=NULL) {
		if (processor.ProcessData(connection, line)) {
			connection->WriteLine("Shutting Down");
			break;
		}
		line = connection->ReadLine();
	}

	return true;
}

int ListenForOneConnection(int pPort, SOCKET *pSocket, int *pWsaError) {
	ListenSocket *listener = new ListenSocket(pPort);

	if (!listener->Start()){
		*pWsaError = WSAGetLastError();
		return 1;
	}

	SOCKET socket = listener->Accept();
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

