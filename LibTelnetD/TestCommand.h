#pragma once

#include "CommandProcessor.h"

class TestCommand : public Command
{
public:
	TestCommand();
	void ProcessCommand(IConsole *pConsole, ParsedCommandLine *pCommandLine) override;
	CommandInfo GetInfo() override;
};

