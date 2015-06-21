#include "pch.h"
#include "CertCommands.h"
#include "Console.h"
#include "ProcessHost.h"
#include<vector>
#include "TerminalHelper.h"
using namespace std;

void CertsCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConsole->WriteLine("SYNTAX: certs storename");
	else {


		// Convert char* string to a wchar_t* string.
	std:string str = pCmdLine->GetArgs().at(1);
		std::wstring wsTmp(str.begin(), str.end());

		HANDLE          hStoreHandle = NULL;
		PCCERT_CONTEXT  pCertContext = NULL;
		char * pszStoreName = "CA";
		pConsole->WriteLine("Listing certs");
		//--------------------------------------------------------------------
		// Open a system certificate store.
		if (hStoreHandle = (*CertOpenStore)(CERT_STORE_PROV_SYSTEM_W,          // The store provider type
			0,                               // The encoding type is
			// not needed
			NULL,                            // Use the default HCRYPTPROV
			CERT_SYSTEM_STORE_CURRENT_USER,  // Set the store location in a
			// registry location
			wsTmp.c_str()                            // The store name as a Unicode 
			// string
			))
		{
			pConsole->WriteLine("Opened certificate store");

		}
		else
		{
			pConsole->WriteLine(GetLastErrorAsString());


			return;
		}


		while (pCertContext = CertEnumCertificatesInStore(
			hStoreHandle,
			pCertContext))
		{
			char buf[1000];
			(*CertNameToStrA)(1, &pCertContext->pCertInfo->Subject, 2, (char *)buf, sizeof(buf));
			pConsole->WriteLine("Found %s", buf);
		}

		//--------------------------------------------------------------------
		//   Clean up.
		if (!(*CertCloseStore)(
			hStoreHandle,
			0))
		{
			pConsole->WriteLine("Failed CertCloseStore");

		}

	}
}

CommandInfo CertsCommand::GetInfo() {
	return CommandInfo("certs", "", "Certificate Manager");
}
