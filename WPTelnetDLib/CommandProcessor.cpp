#include "pch.h"
#include "CommandProcessor.h"
#include "Networking.h"

#include<vector>
using namespace std;

ParsedCommandLine::ParsedCommandLine(string pCommandLine) {		
	int pos = 0;
	_arguments = vector<string>();
	_originalLine = pCommandLine;
	for (;;) {	
		bool skip = false;
		while(pCommandLine[pos]==' ')
			pos++;
		if(pCommandLine[pos] == '"') {
			if(pCommandLine[pos+1]=='"') 
			{
				pos++;
			} 
			else {
				int nextq =  pCommandLine.find('"', pos+1);
				if (nextq == string::npos) {
					if(pos < pCommandLine.length())
					{
						string expr(pCommandLine.substr(pos+1, pCommandLine.length()));
						_arguments.push_back(expr);
					}
					skip = true;
					break;
				} else {
					string expr(pCommandLine.substr(pos+1, nextq-pos-1));
					_arguments.push_back(expr);
					pos = nextq+1;
				}			
				skip = true;
			}				
		} 

		if (!skip){
			int next = pCommandLine.find(' ', pos);
			if (next == string::npos) {
				if(pos < pCommandLine.length())
				{
					string expr(pCommandLine.substr(pos, pCommandLine.length()));
					_arguments.push_back(expr);
				}
				break;
			} else {
				string expr(pCommandLine.substr(pos, next-pos));
				_arguments.push_back(expr);
				pos = next+1;
			}
		}
	}
	if(_arguments.size()>0) 
		_command = _arguments.front();
}

string ParsedCommandLine::GetName() {
	return _command;
}

string ParsedCommandLine::GetRaw() {
	return _originalLine;
}

ParsedCommandLine ParsedCommandLine::GetParametersAsLine(){
	if (_arguments.size() <= 1)
		return ParsedCommandLine(_originalLine);

	return ParsedCommandLine(_originalLine.substr(_command.length() + 1, string::npos));
}

vector<string> ParsedCommandLine::GetArgs() {
	return _arguments;
}		         

BaseCommand::BaseCommand() {
}

CommandProcessor::CommandProcessor(vector<BaseCommand *> pCommands, ICommandProcessorHost *pHost) {
	_commands = pCommands;
	_host = pHost;

}
void CommandProcessor::PrintPrompt(Connection *pConnection) {
	_host->PrintPrompt(pConnection);
}

bool CommandProcessor::ProcessData(Connection *pConnection, const char *pLine) {
	ParsedCommandLine cmdLine = ParsedCommandLine(string(pLine));
	return ProcessCommandLine(pConnection,&cmdLine);
}

bool CommandProcessor::ProcessCommandLine(Connection *pConnection, ParsedCommandLine *pLine) {
	string cmd = pLine->GetName();
	if (cmd == string("exit"))
		return true;
	bool done = false;
	for (std::vector<BaseCommand *>::iterator it = _commands.begin() ; it != _commands.end(); ++it)
		if((*it)->GetName() == cmd) {
			(*it)->ProcessCommand(pConnection,pLine);
			done = true;
			break;
		}				  
	if(!done) 
		_host->UnhandledLine(pConnection, pLine->GetRaw());
	
	PrintPrompt(pConnection);
	return false;
}

