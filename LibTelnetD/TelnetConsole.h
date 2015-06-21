#pragma once
#include <string>
#include "Console.h"
#include <functional>
#include <vector>

class TelnetSession;


enum AnsiRenditionParameters
{
	Reset = 0,
	Bold = 1,
	Italic = 3,
	Underline = 4,
	Blink = 5,
	Inverse= 7,
	PrimaryFont = 10,
	SecondaryFont1 = 11,
	SecondaryFont2 = 12,
	SecondaryFont3 = 13,
	SecondaryFont4 = 14,
	SecondaryFont5 = 15,
	SecondaryFont6 = 16,
	SecondaryFont7 = 17,
	SecondaryFont8 = 18,
	SecondaryFont9 = 19,
	NonBold = 22,
	NonItalic = 23,
	BlinkOff = 25,
	NonInverse = 27,
	Reveal = 28,
	ForegroundBlack = 30,
	ForegroundRed = 31,
	ForegroundGreen = 32,
	ForegroundYellow = 33,
	ForegroundBlue = 34,
	ForegroundMagenta = 35,
	ForegroundCyan = 36,
	ForegroundWhite = 37,
	ForegroundExtended = 38,
	ForegroundDefault = 39,
	BackgroundBlack = 40,
	BackgroundRed = 41,
	BackgroundGreen = 42,
	BackgroundYellow = 43,
	BackgroundBlue = 44,
	BackgroundMagenta = 45,
	BackgroundCyan = 46,
	BackgroundWhite = 47,
	BackgroundExtended = 48,
	BackgroundDefault = 49,
	Framed = 50,
	Encircled = 52,
	Overlined = 53,
	NonFramed = 54,
	NonOverlined = 55


};



class TelnetConsole : public IConsole
{
	int _row;
	int _col;
	int _width;
	int _height;
	bool _renderingPaused;
	ConsoleCharStyle _style;
	std::vector<std::function<void()>*> _sizeChangedHandlers;
	TelnetSession *_session;
	std::vector<ConsoleBuffer*> _bufferStack;
	ConsoleBuffer* _buffer;
	void SetSize(int pWidth, int pHeight);
	
	void Init();
	void OnSizeChanged();
public:
	TelnetConsole(TelnetSession  *pSession);
	void SendRenditionParam(AnsiRenditionParameters pParameter);
	void AddSizeChangedHandler(std::function<void()> *pHandler);
	void RemoveSizeChangedHandler(std::function<void()> *pHandler);
	void SetCursorPosition(int pCol, int pRow) override;
	void MoveCursor(int pRelCol, int pRelRow) override;
	void Clear() override;
	void EraseToEndL() override;
	void SetMouseReporting(bool pOn);

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
	bool Write(int pBytes, BYTE *data) override;
};

