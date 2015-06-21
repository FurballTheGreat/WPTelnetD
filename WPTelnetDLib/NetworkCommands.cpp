#include "pch.h"
#include "NetworkCommands.h"
#include "TcpConnectionTable.h"
#include "UdpConnectionTable.h"
#include "FileHelpers.h"
#include "TelnetD.h"
#include<string>
#include<vector>

void NetstatCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	try {
		TcpConnectionTable table = TcpConnectionTable();
		pConsole->WriteLine("%-14s%14s   %-24s %6s %-15s %15s %s", "Protocol", "Local", "Remote", "PID", "State", "Owner", "Owner Path");
		for (std::vector<TcpConnectionEntry, std::allocator<TcpConnectionEntry>>::iterator it = table.begin(); it != table.end(); ++it)
		{
			char remote[100];
			char local[100];
			sprintf_s(local, "%s:%d", (*it).GetLocalHost().data(), (*it).GetLocalPort());
			sprintf_s(remote, "%s:%d", (*it).GetRemoteHost().data(), (*it).GetRemotePort());

			pConsole->WriteLine("%-4s%24s<->%-24s %6u %-15s %15s %s", "TCP",
				local,
				remote,
				(*it).GetOwnerPID(),
				(*it).GetStateString().data(),
				(*it).GetOwningModule().data(),
				(*it).GetOwningModulePath().data());
		}
	}
	catch (runtime_error *re) {

		pConsole->WriteLine(string("ERROR: ") + string(re->what()));
	}
	try {
		UdpConnectionTable table2 = UdpConnectionTable();
		for (std::vector<UdpConnectionEntry, std::allocator<UdpConnectionEntry>>::iterator it = table2.begin(); it != table2.end(); ++it)
		{


			char local[100];
			sprintf_s(local, "%s:%d", (*it).GetLocalHost().data(), (*it).GetLocalPort());
			pConsole->WriteLine("%-4s%24s   %-24s %6u %-15s %15s %s", "UDP",
				local,
				"",
				(*it).GetOwnerPID(),
				"",
				(*it).GetOwningModule().data(),
				(*it).GetOwningModulePath().data());
		}
	}
	catch (runtime_error *re) {
		
		pConsole->WriteLine(string("ERROR: ")+string(re->what()));
	}

}

CommandInfo NetstatCommand::GetInfo() {
	return CommandInfo("netstat", "", "List all active IP connections.");
}


void DownloadCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size()<3)
		pConsole->WriteLine("ERROR: You must specify a URL and save name");
	else {
		HINTERNET handle = InternetOpenA("Microsoft Internet Explorer",
			PRE_CONFIG_INTERNET_ACCESS,
			NULL,
			INTERNET_INVALID_PORT_NUMBER,
			0);

		DWORD size;
		CHAR headerStr[256] = "Accept: */*\r\n\r\n";
		char tempStr[66000];
		HINTERNET connectHanlde;
		FILE * fileHandle;
		std::string url = pCmdLine->GetArgs().at(1);

		if (!(connectHanlde = InternetOpenUrlA(handle, _strdup(url.c_str()),
			headerStr,
			strlen(headerStr), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
		{
			pConsole->WriteLine("Error Failed to download");
			return;
		}
		DWORD dwByteToRead = 0;
		DWORD sizeOfRq = 4;
		DWORD dwBytes = 0;

		if (!HttpQueryInfoA(connectHanlde, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
			(LPVOID)&dwByteToRead, &sizeOfRq, NULL))
		{
			dwByteToRead = 0;
		}
		std::string outFile = pCmdLine->GetArgs().at(2);
		if (!(fileHandle = fopen(outFile.c_str(), "wb")))
		{
			return;
		}
		size = 1;
		pConsole->Write("Downloading.. ");
		while (InternetReadFile(connectHanlde, tempStr, 16384, &size) && size != 0) {
			pConsole->Write(" %d ", size);
			fwrite(tempStr, sizeof(char), size, fileHandle);
		}
		pConsole->WriteLine(". Downloaded!");

		fclose(fileHandle);

	}
}

CommandInfo DownloadCommand::GetInfo() {
	return CommandInfo("down", "<url> <file>", "Download a file and save a file from a URL.");
}

RunFromCommand::RunFromCommand(IExecutionContext *pExecutionContext, CommandProcessor *pProcessor) {
	_executionContext = pExecutionContext;
	_processor = pProcessor;
}

bool ProcessLine(IConsole *pConsole, IExecutionContext *pExecutionContext, CommandProcessor *pProcessor, string pLine);

void RunFromCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("ERROR: You must specify a URL and save name");
	else {
		std::vector<std::string> lines;
		try {
			DownloadTextFileReader reader(pCmdLine->GetArgs().at(1));
			string line;
			while (reader.ReadLine(line)) {
				lines.push_back(line);
			}
		}
		catch (std::string msg) {
			pConsole->WriteLine(string("ERROR: ") + msg);
		}
		for (int i = 0; i < lines.size(); i++)
			ProcessLine(pConsole, _executionContext, _processor, lines.at(i));


	}
}

CommandInfo RunFromCommand::GetInfo() {
	return CommandInfo("runfrom", "<url>", "Download a script file from URL and run its contents.");
}