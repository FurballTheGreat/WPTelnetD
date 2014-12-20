#pragma once

#include "pch.h"
#include<string>

#include "pch.h"
#include "ProcessHost.h"
#include "Networking.h"
#ifdef PHONE
#include<WPKernel.h>
#endif
#include<thread>


class ProcessHost {
private:
	HANDLE _childStdInReadPipe;
	HANDLE _childStdOutReadPipe;
	HANDLE _childStdOutWritePipe;
	HANDLE _childStdInWritePipe;
	Connection *_connection;
	char _buf[4096];
	bool CreateChildProcess(Connection *pConnection,const char *pCommandLine );
	static void WriteToPipe(Connection *pConnection, HANDLE pHandle);
	static void ReadFromPipe(Connection *pConnection, HANDLE pHandle);
public:
	ProcessHost(Connection *pConnection);
	void ExecuteProcess(Connection *pConnection,const char *pCmdLine) ;
};
