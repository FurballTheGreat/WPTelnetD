#pragma once

#include "pch.h"
#include<string>

#include "pch.h"
#include "ProcessHost.h"
#include "Console.h"
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
	IConsole *_console;
	char _buf[4096];
	bool CreateChildProcess(IConsole *pConsole,const char *pCommandLine );
	static void WriteToPipe(IConsole *pConsole, HANDLE pHandle);
	static void ReadFromPipe(IConsole *pConsole, HANDLE pHandle);
public:
	ProcessHost(IConsole *pConsole);
	void ExecuteProcess(IConsole *pConsole,const char *pCmdLine) ;
};
