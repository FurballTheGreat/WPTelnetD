#include "pch.h"
#include "ProcessHost.h"
#include "Networking.h"
#ifdef PHONE
#include<WPKernel.h>
#endif
#include<thread>
#include "stdlib.h"


bool ProcessHost::CreateChildProcess(Connection *pConnection, const char *pCommandLine )
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
		pConnection->WriteLine("Not executing nothing");
		return false;
	}
	
	pConnection->WriteLine("CMD: %s",pCommandLine);
	_connection = pConnection;
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
		pConnection->WriteLastError();
		return false;
	}
	else 
	{
		std::thread writeToPipeThread(WriteToPipe, pConnection,_childStdInWritePipe );
		std::thread readFromPipeThread(ReadFromPipe, pConnection, _childStdOutReadPipe); 
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

void ProcessHost::WriteToPipe(Connection *pConnection, HANDLE pHandle) 
{ 
	DWORD  bytesWritten; 

	for (bool isSuccess = true;isSuccess;)
	{ 
		const char* read = pConnection->ReadLine();
		char buf[4096];
		
		if (!read) 
			break; 
		
		sprintf_s(buf, "%s\n", read);
		isSuccess = WriteFile(pHandle,buf, strlen(buf), &bytesWritten, NULL);
	} 

	if (!CloseHandle(pHandle)) 
		pConnection->WriteLastError();
} 

void ProcessHost::ReadFromPipe(Connection *pConnection, HANDLE pHandle) 
{ 
	DWORD bytesRead, bytesWritten; 
	CHAR characterBuffer[4096]; 
	BOOL isSuccess = FALSE;

	for (bool isSuccess = true; isSuccess;)
	{ 
		isSuccess = ReadFile( pHandle, characterBuffer, sizeof(characterBuffer), &bytesRead, NULL);
		if(!isSuccess || !bytesRead) break; 

		isSuccess = pConnection->Write(bytesRead,characterBuffer);
	} 
} 

ProcessHost::ProcessHost(Connection *pConnection) {
	SECURITY_ATTRIBUTES saAttr; 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	if ( ! CreatePipe(&_childStdOutReadPipe, &_childStdOutWritePipe, &saAttr, 0) ) {
		pConnection->WriteLine("Unable to create a pipe for the child process's STDOUT"); 
		return;
	}

	if ( ! SetHandleInformation(_childStdOutReadPipe, HANDLE_FLAG_INHERIT, 0) ) {
		pConnection->WriteLine("Failed to call SetHandleInformation when ensuring read handle for STDOUT pipe is not inherited.");
		return;
	}

	if (! CreatePipe(&_childStdInReadPipe, &_childStdInWritePipe, &saAttr, 0)) {
		pConnection->WriteLine("Unable to create a pipe for the child process's STDIN");
		return;
	}

	if ( ! SetHandleInformation(_childStdInWritePipe, HANDLE_FLAG_INHERIT, 0) ){
		pConnection->WriteLine("Failed to call SetHandleInformation when ensuring write handle for STDIN pipe is not inherited.");
		return;
	}
}

void ProcessHost::ExecuteProcess(Connection *pConnection, const char *pCmdLine) 
{
	CreateChildProcess(pConnection,pCmdLine);
	pConnection->WriteLine( "End of parent execution.");
}

