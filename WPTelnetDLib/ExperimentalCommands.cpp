#include "pch.h"
#include "ExperimentalCommands.h"



void TestCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	HMODULE lib = LoadLibraryA("samcli.dll");
	if (lib == NULL){
		pConnection->WriteLine("Failed Load Lib");
		pConnection->WriteLastError();
		return;
	}

	PNetUserEnum NetUserEnum = (PNetUserEnum)GetProcAddress(lib, "NetUserEnum");
	
	do // begin do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		pConnection->WriteLine("Status: %d", nStatus);
		//
		// If the call succeeds,
		//
		if ((nStatus ==ERROR_SUCCESS) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				//
				// Loop through the entries.
				//
				for (i = 0; (i < dwEntriesRead); i++)
				{
					
					if (pTmpBuf == NULL)
					{
						pConnection->WriteLine("TmpBuf is null");
						break;
					}
					wstring userNameW = pTmpBuf->usri0_name;
					string userNameA = string(userNameW.begin(), userNameW.end());
					pConnection->WriteLine(userNameA);

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		//
		// Otherwise, print the system error.
		//
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		//
		// Free the allocated buffer.
		//
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	// Continue to call NetUserEnum while 
	//  there are more entries. 
	// 
	while (nStatus == ERROR_MORE_DATA); // end do
	//
	// Check again for allocated memory.
	
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	
	// Print the final count of users enumerated.
	//
	pConnection->WriteLine("Total of %d entries enumerated", dwTotalCount);

	
	
}

string TestCommand::GetName() {
	return "test";
}


void TestCommand2::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	
	
}

string TestCommand2::GetName() {
	return "test2";
}