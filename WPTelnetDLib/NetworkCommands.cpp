#include "pch.h"
#include "NetworkCommands.h"
#include "TcpConnectionTable.h"
#include "UdpConnectionTable.h"

void NetstatCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	try {
		TcpConnectionTable table = TcpConnectionTable();
		pConnection->WriteLine("%-14s%14s   %-24s %6s %-15s %15s %s", "Protocol", "Local", "Remote", "PID", "State", "Owner", "Owner Path");
		for (std::vector<TcpConnectionEntry, std::allocator<TcpConnectionEntry>>::iterator it = table.begin(); it != table.end(); ++it)
		{
			char remote[100];
			char local[100];
			sprintf_s(local, "%s:%d", (*it).GetLocalHost().data(), (*it).GetLocalPort());
			sprintf_s(remote, "%s:%d", (*it).GetRemoteHost().data(), (*it).GetRemotePort());

			pConnection->WriteLine("%-4s%24s<->%-24s %6u %-15s %15s %s", "TCP",
				local,
				remote,
				(*it).GetOwnerPID(),
				(*it).GetStateString().data(),
				(*it).GetOwningModule().data(),
				(*it).GetOwningModulePath().data());
		}
		UdpConnectionTable table2 = UdpConnectionTable();
		for (std::vector<UdpConnectionEntry, std::allocator<UdpConnectionEntry>>::iterator it = table2.begin(); it != table2.end(); ++it)
		{


			char local[100];
			sprintf_s(local, "%s:%d", (*it).GetLocalHost().data(), (*it).GetLocalPort());
			pConnection->WriteLine("%-4s%24s   %-24s %6u %-15s %15s %s", "UDP",
				local,
				"",
				(*it).GetOwnerPID(),
				"",
				(*it).GetOwningModule().data(),
				(*it).GetOwningModulePath().data());
		}
	}
	catch (...) {
		pConnection->WriteLine("An Error occured");
	}

}

string NetstatCommand::GetName() {
	return "netstat";
}


void DownloadCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	if (pCmdLine->GetArgs().size()<3)
		pConnection->WriteLine("ERROR: You must specify a URL and save name");
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

		if (!(connectHanlde = InternetOpenUrlA(handle, strdup(url.c_str()),
			headerStr,
			strlen(headerStr), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
		{
			pConnection->WriteLine("Error Failed to download");
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
		pConnection->Write("Downloading.. ");
		while (InternetReadFile(connectHanlde, tempStr, 16384, &size) && size != 0) {
			pConnection->Write(" %d ", size);
			fwrite(tempStr, sizeof(char), size, fileHandle);
		}
		pConnection->WriteLine(". Downloaded!");

		fclose(fileHandle);

	}
}

string DownloadCommand::GetName() {
	return "down";
}
