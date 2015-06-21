#include "pch.h"
#include "SystemAPIs.h"

#include<string>
#include<vector>
#include "winsock2.h"
#include <winuser.h>
#include "TelnetD.h"
using namespace Platform ;


namespace TelnetDS81
{
	int DevProgramReg::CreateProcess(String ^pCmdLine)
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



	int DevProgramReg::GetLastError()
		{
			return ::GetLastError();
		}

	String^ DevProgramReg::GetCommandLine() {
			return ref new String(::GetCommandLineW());
		}

	bool DevProgramReg::TelnetProcessConnection(int pSocket, String^ pWelcomeInfo){
		std::wstring fooW(pWelcomeInfo->Begin());
		char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
		bool res = ProcessConnection(pSocket, wel);
		free(wel);
		return res;

	}
	int DevProgramReg::TelnetListenForOneConnection(int pPort, int* pSocket, int *pWsaError){
		return ListenForOneConnection(pPort, (SOCKET *)pSocket, pWsaError,NULL);
	}
	int DevProgramReg::TelnetConnectTo(String^ pIpAddress, int pPort, int *pSocket, int *pWsaError){
		std::wstring fooW(pIpAddress->Begin());
		char *wel = _strdup(std::string(fooW.begin(), fooW.end()).c_str());
		int res = ConnectTo(wel, pPort, (SOCKET *)pSocket, pWsaError);
		free(wel);
		return res;
	}
	bool DevProgramReg::TelnetInitNetworking(int *pWsaError){
		return InitNetworking(pWsaError);
	}
	bool DevProgramReg::TelnetShutDownNetworking(){
		return ShutDownNetworking();
	}


	bool DevProgramReg::TelnetExecuteCommand(String^ pCommandLine) {
		std::wstring cmdW(pCommandLine->Begin());
		std::string cmd(cmdW.begin(), cmdW.end());
		return ExecuteCommand((char *)cmd.c_str());
	}
		ProcessInfo::ProcessInfo(PROCESS_INFORMATION pInfo) {
			_info = pInfo;
		}


		Array<String^>^ DevProgramReg::GetCps(bool pIncludeFirst){
		HKEY key;
		HRESULT error = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\SecurityManager\\CapabilityClasses", NULL, KEY_READ, &key);
		if (error!=ERROR_SUCCESS)
			return ref new Array<String^>(0);
		DWORD index = 0;
		char nameBuf[1024];
		std::vector<std::string> cls;
		if (pIncludeFirst)
			cls.push_back("CAPABILITY_CLASS_FIRST_PARTY_APPLICATIONS");
		cls.push_back("CAPABILITY_CLASS_SECOND_PARTY_APPLICATIONS");
		cls.push_back("CAPABILITY_CLASS_THIRD_PARTY_APPLICATIONS");
		LSTATUS result = ERROR_SUCCESS;
		std::vector<String^> caps;

		DWORD valType;
		DWORD valSize;
		DWORD valSizeP = 1024 * 1024 * 3;
		BYTE* valBuf = (BYTE*)malloc(valSizeP);
		char*valPtr;
		while (result == ERROR_SUCCESS) {
			valSize = valSizeP;
			DWORD nameLen = sizeof(nameBuf);
			result = RegEnumValueA(key, index, nameBuf, &nameLen, 0, &valType, valBuf, &valSize);
			index++;
			std::string nameA(nameBuf);
			std::wstring nameW(nameA.begin(), nameA.end());
			String^ capName = ref new String(nameW.c_str());
			if (nameA.substr(0, 6) == "ID_CAP" && valType == REG_MULTI_SZ && (result == ERROR_SUCCESS || result == ERROR_NO_MORE_ITEMS)){
				valPtr = (char*)valBuf;

				bool done = false;
				while (strlen(valPtr) > 0 && !done) {
					for (int i = 0; i < cls.size(); i++){
						std::string valA(valPtr);
					
						if (valA== cls.at(i)){
							caps.push_back(capName);
							done = true;
							break;
						}
					}
					valPtr += strlen((const char*)valPtr) + 1;

				}
			}
		}
		free(valBuf);
		Array<String^>^ returnVal = ref new Array<String^>(caps.size());
		for (int i = 0; i < caps.size(); i++)
			returnVal[i] = caps.at(i);


		
		//if (result == ERROR_NO_MORE_ITEMS)
		//	return false;
		//if (result == ERROR_SUCCESS)
		//	return true;

		RegCloseKey(key);
		return returnVal;
	}
}