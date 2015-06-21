#include "pch.h"
#include "ProcessHost.h"
#include "Console.h"
#ifdef PHONE
#include<WPKernel.h>
#endif
#include<thread>
#include "stdlib.h"
#include "TerminalHelper.h"


bool ProcessHost::CreateChildProcess(IConsole *pConsole, const char *pCommandLine )
{ 
	PROCESS_INFORMATION processInfo; 
	STARTUPINFOA startInfo;
	BOOL isSuccess = FALSE; 

	ZeroMemory( &processInfo, sizeof(PROCESS_INFORMATION) );
	ZeroMemory( &startInfo, sizeof(STARTUPINFOA) );

	startInfo.cb = sizeof(STARTUPINFOA); 
	startInfo.hStdError = _childStdOutWritePipe;
	startInfo.hStdOutput = _childStdOutWritePipe;
	startInfo.hStdInput = _childStdInReadPipe;
	startInfo.dwFlags |= STARTF_USESTDHANDLES;
	
	if (strlen(pCommandLine) == 0) {
		pConsole->WriteLine("Not executing nothing");
		return false;
	}
	
	pConsole->WriteLine("CMD: %s",pCommandLine);
	_console = pConsole;
	isSuccess = CreateProcessA(
		NULL,
		(char*)pCommandLine, 		
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
		return false;
	}
	else 
	{
		std::thread writeToPipeThread(WriteToPipe, pConsole,_childStdInWritePipe );
		std::thread readFromPipeThread(ReadFromPipe, pConsole, _childStdOutReadPipe); 
		WaitForSingleObject(processInfo.hProcess,INFINITE);
		TerminateThread(writeToPipeThread.native_handle(),0);
		TerminateThread(readFromPipeThread.native_handle(),0);
		writeToPipeThread.join();
		readFromPipeThread.join();
		//Sleep(1000);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}

void ProcessHost::WriteToPipe(IConsole *pConsole, HANDLE pHandle) 
{ 
	DWORD  bytesWritten; 

	for (bool isSuccess = true;isSuccess;)
	{ 
		auto read = pConsole->ReadLine()+"\n";
		
		
		
		isSuccess = WriteFile(pHandle,read.c_str(), read.length(), &bytesWritten, NULL);
	} 

	if (!CloseHandle(pHandle)) 
		pConsole->WriteLine(GetLastErrorAsString());
} 

void ProcessHost::ReadFromPipe(IConsole *pConsole, HANDLE pHandle) 
{ 
	DWORD bytesRead, bytesWritten; 
	CHAR characterBuffer[4096]; 
	BOOL isSuccess = FALSE;

	for (bool isSuccess = true; isSuccess;)
	{ 
		isSuccess = ReadFile( pHandle, characterBuffer, sizeof(characterBuffer), &bytesRead, NULL);
		if(!isSuccess || !bytesRead) break; 

		isSuccess = pConsole->Write(bytesRead,(BYTE*)characterBuffer);
	} 
} 

ProcessHost::ProcessHost(IConsole *pConsole) {
	SECURITY_ATTRIBUTES saAttr; 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	if ( ! CreatePipe(&_childStdOutReadPipe, &_childStdOutWritePipe, &saAttr, 0) ) {
		pConsole->WriteLine("Unable to create a pipe for the child process's STDOUT"); 
		return;
	}

	if ( ! SetHandleInformation(_childStdOutReadPipe, HANDLE_FLAG_INHERIT, 0) ) {
		pConsole->WriteLine("Failed to call SetHandleInformation when ensuring read handle for STDOUT pipe is not inherited.");
		return;
	}

	if (! CreatePipe(&_childStdInReadPipe, &_childStdInWritePipe, &saAttr, 0)) {
		pConsole->WriteLine("Unable to create a pipe for the child process's STDIN");
		return;
	}

	if ( ! SetHandleInformation(_childStdInWritePipe, HANDLE_FLAG_INHERIT, 0) ){
		pConsole->WriteLine("Failed to call SetHandleInformation when ensuring write handle for STDIN pipe is not inherited.");
		return;
	}
}

void ProcessHost::ExecuteProcess(IConsole *pConsole, const char *pCmdLine) 
{
	CreateChildProcess(pConsole,pCmdLine);
	pConsole->WriteLine( "End of parent execution.");
}

