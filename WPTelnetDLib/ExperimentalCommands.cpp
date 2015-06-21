#include "pch.h"
#include "ExperimentalCommands.h"
#include "TerminalHelper.h"



void ProvXmlCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {

	if (pCmdLine->GetArgs().size() < 2)
	{
		pConsole->WriteLine("SYNTAX: provxml CSP");
		return;
	}
#ifdef PHONE
	HMODULE lib = LoadLibraryA("DMProcessXMLFiltered.dll");
	if (lib == NULL){
		pConsole->WriteLine("Failed Load Lib");
		pConsole->WriteLine(GetLastErrorAsString());
		return;
	}

	PDMProcessConfigXMLFiltered DMProcessConfigXMLFiltered = (PDMProcessConfigXMLFiltered)GetProcAddress(lib, "DMProcessConfigXMLFiltered");

	string configA = pConsole->ReadLine();
	wstring configW = wstring(configA.begin(), configA.end());

	string csp = pCmdLine->GetArgs().at(1);
	wstring cspW = wstring(csp.begin(), csp.end());
	const WCHAR *config = configW.c_str();

	LPCWSTR rgszAllowedCspNodes[] =
	{
		cspW.c_str(),
	};

	BSTR bstr = NULL;

	HRESULT hr = DMProcessConfigXMLFiltered(
		config,
		rgszAllowedCspNodes,
		_countof(rgszAllowedCspNodes),
		&bstr
		);

	/* check error */
	pConsole->WriteLine("Result from DMProcessConfigXML %d", hr);

	if (bstr != NULL)
	{

		pConsole->WriteLine(wstring(bstr));
	}
#endif
}

CommandInfo ProvXmlCommand::GetInfo() {
	return CommandInfo("provxml", "<CSP>", "Execute a provxml block.");
}

void TestCommand::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {
	
	
}

CommandInfo TestCommand::GetInfo() {
	return CommandInfo("test", "", "Dummy placeholder command");
}


//Nokia::DeviceToolkit::Client::DeviceToolkitClient::

void TestCommand2::ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCmdLine) {


//	//delete client;
}

CommandInfo TestCommand2::GetInfo() {
	return CommandInfo("test2", "", "Dummy placeholder command");
}