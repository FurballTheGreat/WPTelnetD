#pragma once
#include "string"
#include "Console.h"

class StringConsole : public IConsole
{
	std::string _string;
public:
	void SetCursorPosition(int pCol, int pRow) override;
	void MoveCursor(int pRelCol, int pRelRow) override;
	void Clear() override;
	void EraseToEndL() override;
	void SetForeground(Color pColor) override;
	void SetBackground(Color pColor) override;
	void SetBold(bool pBold) override;
	void ResetStyle() override;
	void ShowCursor() override;
	void HideCursor() override;
	void PauseRendering() override;
	void ResumeRendering() override;
	ConsoleKey ReadKey() override;
	std::string ReadLine() override;
	char ReadChar() override;

	bool WaitForEnter() override;
	bool Write(int pBytes, BYTE *pData) override;
};
