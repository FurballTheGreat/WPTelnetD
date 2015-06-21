#include "stdafx.h"
#include "StringConsole.h"


void StringConsole::SetCursorPosition(int pCol, int pRow){ }
void StringConsole::MoveCursor(int pRelCol, int pRelRow){ }
void StringConsole::Clear()
{
	_string = "";
}
void StringConsole::EraseToEndL(){ }
void StringConsole::SetForeground(Color pColor){ }
void StringConsole::SetBackground(Color pColor){ }
void StringConsole::SetBold(bool pBold){ }
void StringConsole::ResetStyle(){ }
void StringConsole::ShowCursor(){ }
void StringConsole::HideCursor(){ }

void StringConsole::PauseRendering()
{
}

void StringConsole::ResumeRendering()
{
}

ConsoleKey StringConsole::ReadKey()
{
	ConsoleKey result;
	memset(&result, 0, sizeof(result));
	return result;
}
std::string StringConsole::ReadLine()
{
	return "";
}
char StringConsole::ReadChar()
{
	return 0;
}

bool StringConsole::WaitForEnter()
{
	return false;
}

bool StringConsole::Write(int pBytes, BYTE *pData)
{
	for (int i = 0; i < pBytes; i++)
	{
		_string += static_cast<char>(pData[i]);
	}
	return true;
}
