#include "pch.h"
#include "SystemAPIs.h"

#include<string>
#include "winsock2.h"
#include <winuser.h>
#include "TelnetD.h"

using namespace Platform ;


namespace RegistryUtils
{
	int RegistryUtils::CreateProcess(String ^pCmdLine)
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



		int RegistryUtils::GetLastError()
		{
			return ::GetLastError();
		}

		String^ RegistryUtils::GetCommandLine() {
			return ref new String(::GetCommandLineW());
		}

		bool RegistryUtils::TelnetProcessConnection(int pSocket, String^ pWelcomeInfo){
			std::wstring fooW(pWelcomeInfo->Begin());
			char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
			bool res = ProcessConnection(pSocket, wel);
			free(wel);
			return res;

		}
		int RegistryUtils::TelnetListenForOneConnection(int pPort, int* pSocket, int *pWsaError){
			return ListenForOneConnection(pPort, (SOCKET *)pSocket, pWsaError);
		}
		int RegistryUtils::TelnetConnectTo(String^ pIpAddress, int pPort, int *pSocket, int *pWsaError){
			std::wstring fooW(pIpAddress->Begin());
			char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
			int res = ConnectTo(wel, pPort, (SOCKET *)pSocket, pWsaError);
			free(wel);
			return res;
		}
		bool RegistryUtils::TelnetInitNetworking(int *pWsaError){
			return InitNetworking(pWsaError);
		}
		bool RegistryUtils::TelnetShutDownNetworking(){
			return ShutDownNetworking();
		}

		ProcessInfo::ProcessInfo(PROCESS_INFORMATION pInfo) {
			_info = pInfo;
		}
		CETWLogging::CETWLogging() {
	
		}
}