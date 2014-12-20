#include "pch.h"
#include "SystemAPIs.h"

#include<string>
#include "winsock2.h"
#include <winuser.h>
#include "TelnetD.h"
#include <list>
using namespace Platform ;

using namespace std;

namespace DiagDebugLogWinPRT
{	
	int CDiagDebugLogWinPRT::CreateProcess(String ^pCmdLine)
		{
			STARTUPINFOW si;
			PROCESS_INFORMATION pi;

			wchar_t buf[1024];

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );
			if (!::CreateProcessW(pCmdLine->Data(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE & CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi))
				return 0;
			else 				
				return pi.dwProcessId;			
		}
	


	int CDiagDebugLogWinPRT::GetLastError()
		{
			return ::GetLastError();
		}
#undef GetEnvironmentStrings
	extern "C" WINBASEAPI
		_NullNull_terminated_
		LPCH
		WINAPI
		GetEnvironmentStrings(
		VOID
		);
	String^ CDiagDebugLogWinPRT::GetCommandLine() {
		return ref new String(GetCommandLineW());

		
		}

		ProcessInfo::ProcessInfo(PROCESS_INFORMATION pInfo) {
			_info = pInfo;
		}
		

	

		bool  CDiagDebugLogWinPRT::TelnetProcessConnection(int pSocket, String^ pWelcomeInfo){
			std::wstring fooW(pWelcomeInfo->Begin());
			char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
			bool res = ProcessConnection(pSocket, wel);
			free(wel);
			return res;

		}
		int CDiagDebugLogWinPRT::TelnetListenForOneConnection(int pPort, int* pSocket, int *pWsaError){
			return ListenForOneConnection(pPort, (SOCKET *)pSocket, pWsaError);
		}
		int CDiagDebugLogWinPRT::TelnetConnectTo(String^ pIpAddress, int pPort, int *pSocket, int *pWsaError){
			std::wstring fooW(pIpAddress->Begin());
			char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
			int res = ConnectTo(wel, pPort, (SOCKET *)pSocket, pWsaError);
			free(wel);
			return res;
		}
		bool CDiagDebugLogWinPRT::TelnetInitNetworking(int *pWsaError){
			return InitNetworking(pWsaError);
		}
		bool CDiagDebugLogWinPRT::TelnetShutDownNetworking(){
			return ShutDownNetworking();
		}
	/*	CETWLogging::CETWLogging() {
	
		}*/
	//}
}