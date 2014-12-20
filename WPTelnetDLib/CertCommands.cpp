#include "pch.h"
#include "CertCommands.h"
#include "Networking.h"
#include "ProcessHost.h"
#include<vector>
using namespace std;

void CertsCommand::ProcessCommand(Connection *pConnection, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size()<2)
		pConnection->WriteLine("SYNTAX: certs storename");
	else {
		HMODULE lib = LoadLibraryA("crypt32.dll");
		if (lib == NULL){
			pConnection->WriteLine("Failed Load Lib");
			pConnection->WriteLastError();
			return;
		}

		PCertOpenStore CertOpenStore = (PCertOpenStore)GetProcAddress(lib, "CertOpenStore");
		PCertEnumCertificatesInStore CertEnumCertificatesInStore = (PCertEnumCertificatesInStore)GetProcAddress(lib, "CertEnumCertificatesInStore");
		PCertGetNameStringA CertGetNameStringA = (PCertGetNameStringA)GetProcAddress(lib, "CertGetNameStringA");
		PCertCloseStore CertCloseStore = (PCertCloseStore)GetProcAddress(lib, "CertCloseStore");
		if (CertOpenStore == NULL){
			pConnection->WriteLine("Failed to call GetProcAddress on CertOpenStore");
			pConnection->WriteLastError();
			return;
		}
		if (CertEnumCertificatesInStore == NULL){
			pConnection->WriteLine("Failed to call GetProcAddress on CertEnumCertificatesInStore");
			pConnection->WriteLastError();
			return;
		}

		if (CertGetNameStringA == NULL){
			pConnection->WriteLine("Failed to call GetProcAddress on CertGetNameStringA");
			pConnection->WriteLastError();
			return;
		}

		if (CertCloseStore == NULL){
			pConnection->WriteLine("Failed to call GetProcAddress on CertCloseStore");
			pConnection->WriteLastError();
			return;
		}




		// Convert char* string to a wchar_t* string.
	std:string str = pCmdLine->GetArgs().at(1);
		std::wstring wsTmp(str.begin(), str.end());

		HANDLE          hStoreHandle = NULL;
		PCCERT_CONTEXT  pCertContext = NULL;
		char * pszStoreName = "CA";
		pConnection->WriteLine("Listing certs");
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
			pConnection->WriteLine("Opened certificate store");

		}
		else
		{
			pConnection->WriteLastError();


			return;
		}


		while (pCertContext = (*CertEnumCertificatesInStore)(
			hStoreHandle,
			pCertContext))
		{
			char buf[1000];
			(*CertGetNameStringA)(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, buf, sizeof(buf));
			pConnection->WriteLine("Found %s", buf);
		}

		//--------------------------------------------------------------------
		//   Clean up.
		if (!(*CertCloseStore)(
			hStoreHandle,
			0))
		{
			pConnection->WriteLine("Failed CertCloseStore");

		}

	}
}

string CertsCommand::GetName() {
	return "certs";
}
