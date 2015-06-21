#include "stdafx.h"
#include "TelnetConsole.h"
#include "TelnetSession.h"


TelnetConsole::TelnetConsole(TelnetSession  *pSession)
{
	_session = pSession;
	Init();
	pSession->SetSizeChangedHandler([&](int pWidth, int pHeight) {
		SetSize(pWidth,pHeight);
	});
	
}

void TelnetConsole::Init()
{
	ZeroMemory(&_style, sizeof(ConsoleCharStyle));
	HideCursor();
	Clear();
	_col = 0;
	_row = 0;
	_buffer = new ConsoleBuffer(80, 25);
	SetSize(80, 25);
	SetCursorPosition(0, 0);
}

void TelnetConsole::SetSize(int pWidth, int pHeight)
{
	_width = pWidth;
	_height = pHeight;
	_buffer->Resize(pWidth, pHeight);
	OnSizeChanged();
}

void TelnetConsole::AddSizeChangedHandler(std::function<void()> *pHandler)
{
	_sizeChangedHandlers.push_back(pHandler);
}

void TelnetConsole::RemoveSizeChangedHandler(std::function<void()> *pHandler)
{
	for (auto i = 0; i < _sizeChangedHandlers.size(); i++)
		if (_sizeChangedHandlers.at(i) == pHandler){
			_sizeChangedHandlers.erase(_sizeChangedHandlers.begin() + i);
			return;
		}
}

void TelnetConsole::OnSizeChanged()
{
	for (auto i = 0; i < _sizeChangedHandlers.size(); i++)
		(*_sizeChangedHandlers.at(i))();
}

void TelnetConsole::SetCursorPosition(int pCol, int pRow)
{
	char buf[20];
	sprintf_s(buf, "\x1b[%d;%dH", pCol,pRow);
	_row = pRow;
	_col = pCol;
	_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
}

void TelnetConsole::MoveCursor(int pRelCol, int pRelRow)
{
	if (pRelCol>0 && _col + pRelCol>=_width)
	{
		auto remaining = pRelCol - _width + _col;
		pRelRow = ((remaining) / _width)+1;
		pRelCol = (remaining % _width)-_col;
	} else if (pRelCol < 0 && _col < -pRelCol)
	{
		auto remaining = pRelCol + _col;
		pRelRow = (remaining / _width) - 1;
		pRelCol = _width+ remaining % _width - _col;
	}


	char buf[20];
	if (pRelCol>0)
	{
		sprintf_s(buf, "\x1b[%dC", pRelCol);
		_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
	}
	else if (pRelCol < 0)
	{
		sprintf_s(buf, "\x1b[%dD", -pRelCol);
		_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
	}

	if (pRelRow>0)
	{
		sprintf_s(buf, "\x1b[%dB", pRelRow);
		_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
	}
	else if (pRelRow < 0)
	{
		sprintf_s(buf, "\x1b[%dA",-pRelRow);
		_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
	}
	_col += pRelCol;
	_row += pRelRow;
}
void TelnetConsole::Clear()
{
	auto clear = "\x1b[2J";
	_session->Write(strlen(clear), (BYTE*)clear);

	_row = 0;
	_col = 0;
}
void TelnetConsole::EraseToEndL()
{
	auto clear = "\x1b[K";
	_session->Write(strlen(clear), (BYTE*)clear);
}

void TelnetConsole::ShowCursor()
{
	auto clear = "\x1b[?25h";
	_session->Write(strlen(clear), (BYTE*)clear);
}
void TelnetConsole::HideCursor()
{
	auto clear = "\x1b[?25l";
	_session->Write(strlen(clear), (BYTE*)clear);
}

void TelnetConsole::SetMouseReporting(bool pOn)
{
	if (pOn) {
		auto clear = "\x1b[?1002h";
		_session->Write(strlen(clear), (BYTE*)clear);
	} else
	{
		auto clear = "\x1b[?1002l";
		_session->Write(strlen(clear), (BYTE*)clear);
	}
}

void TelnetConsole::SendRenditionParam(AnsiRenditionParameters pParameter)
{
	char buf[20];
	sprintf_s(buf, "\x1b[%dm", static_cast<int>(pParameter));
	_session->Write(strlen(buf), reinterpret_cast<BYTE*>(buf));
	switch (pParameter)
	{
	case Reset: 
		ZeroMemory(&_style, sizeof(ConsoleCharStyle));
		break;

	case Bold: 
		_style.Bold = true;
		break;
	case Italic: 
		break;
	case Underline: 
		_style.Underline = true;
		break;
	case Blink: 
		break;
	case Inverse: 
		_style.Inverse = true;
		break;
	case NonBold: 
		_style.Bold = false;
		break;
	case NonItalic: break;
	case BlinkOff: break;
	case NonInverse: 
		_style.Inverse = false;
		break;
	case Reveal: break;
	case ForegroundBlack:
	case ForegroundRed:
	case ForegroundGreen:
	case ForegroundYellow:
	case ForegroundBlue:
	case ForegroundMagenta: 
	case ForegroundCyan: 
	case ForegroundWhite: 
	case ForegroundExtended: 
	case ForegroundDefault: 
		_style.Foreground = static_cast<Color>(pParameter - ForegroundBlack);
		break;
	
	case BackgroundBlack:
	case BackgroundRed:
	case BackgroundGreen: 
	case BackgroundYellow:
	case BackgroundBlue: 
	case BackgroundMagenta: 
	case BackgroundCyan: 
	case BackgroundWhite: 
	case BackgroundExtended: 
	case BackgroundDefault: 
		_style.Background = static_cast<Color>(pParameter - BackgroundBlack);
		break;	
	default: break;
	}
}
void TelnetConsole::SetForeground(Color pColor)
{
	SendRenditionParam(static_cast<AnsiRenditionParameters>(pColor + ForegroundBlack));
}
void TelnetConsole::SetBackground(Color pColor)
{
	SendRenditionParam(static_cast<AnsiRenditionParameters>(pColor + BackgroundBlack));
}
void TelnetConsole::SetBold(bool pBold)
{
	if (pBold)
		SendRenditionParam(Bold);
	else
		SendRenditionParam(NonBold);
}

void TelnetConsole::ResetStyle()
{
	SendRenditionParam(Reset);
}

ConsoleKey TelnetConsole::ReadKey()
{
	ConsoleKey result;
	char key;
	auto processArrowKey = [&]() {
		switch (key)
		{
		case 'A':
			result.Type = Up;
			result.Ctrl = true;
			return true;
		case 'B':
			result.Type = Down;
			result.Ctrl = true;
			return true;
		case 'D':
			result.Type = Left;
			result.Ctrl = true;
			return true;
		case 'C':
			result.Type = Right;
			result.Ctrl = true;
			return true;
		}
		return false;
	};

	auto processKeyEscape = [&]() {
		switch (key)
		{
		case '[':
			_session->Read(1, reinterpret_cast<BYTE*>(&key));
			if (processArrowKey())
				return true;

			if (key >= '0' && key <= '9')
			{
				std::string val = "";
				while (key != '~')
				{
					val = val + key;
					_session->Read(1, reinterpret_cast<BYTE*>(&key));
				}
				result.Type = static_cast<ConsoleKeyType>(atoi(val.c_str()));
				return true;
			}
			break;
		case 'O':
			_session->Read(1, reinterpret_cast<BYTE*>(&key));
			return processArrowKey();
		}
		return false;
	};

	while (true) {
		result.Alt = false;
		result.Ctrl = false;
		result.Char = 0;
		
		_session->Read(1, reinterpret_cast<BYTE*>(&key));

		if (key == 27)
		{
			_session->Read(1, reinterpret_cast<BYTE*>(&key));
			bool complete = false;
			switch (key)
			{
			case 27:
				result.Alt = true;
				_session->Read(1, reinterpret_cast<BYTE*>(&key));
				complete = processKeyEscape();
				break;
			case '[':				
			case 'O':
				complete = processKeyEscape();
				break;
			}

			if (complete) break;
		}
		else
		{
			result.Type = key==127? Backspace : key==13 ? Enter : Char;
			result.Char = key;
			break;
		}
	}
	return result;
}

std::string TelnetConsole::ReadLine()
{
	auto insert = true;
	auto pos = 0;
	
	std::string str;
	auto sizeChanged = [&]() {

	};
	ShowCursor();
//	AddSizeChangedHandler(&sizeChanged);
	while (true)
	{
		auto key = ReadKey();
		switch (key.Type)
		{
		case Char: 
			if (key.Char < 32) continue;

			if (pos >= str.length()) {
				str.append(1, key.Char);
				
				IConsole::Write(key.Char);
			}else if (insert){
				str.insert(pos, 1, key.Char);
				auto wr = str.substr(pos);
				IConsole::Write(wr);
				MoveCursor(pos - str.length() +1+ (_col == _width), 0);
			}

			else {
				str.at(pos) = key.Char;
				IConsole::Write(key.Char);
			}
			pos++;
			break;
		case Home: 
			MoveCursor(-pos, 0);
			pos = 0;
			break;
		case End: 
			MoveCursor(str.length()-pos, 0);
			pos = str.length();
			break;
		case Insert: 
			insert = !insert;
			break;
		case Delete: 
			if (pos<str.length())
			{
				str.erase(pos, 1);
				IConsole::Write(str.substr(pos) + " ");
				MoveCursor((pos) - str.length()-1, 0);
			}
			break;
		case Left: 
			if (pos > 0){
				MoveCursor(-1, 0);
				pos--;
			}
			break;
		case Right: 
			if (pos<str.length())
			{
				MoveCursor(1, 0);
				pos++;
			}

			break;
		case Enter: 
		//	RemoveSizeChangedHandler(&sizeChanged);
			HideCursor();
			WriteLine();
			return str;
		case Backspace: 
			if (pos>0)
			{
				str.erase(pos-1, 1);
				MoveCursor(-1,0);
				if (pos <= str.length()) {
					IConsole::Write(str.substr(pos - 1) + " ");
					MoveCursor((pos - str.length()) - 2, 0);
				}
				else {
					IConsole::Write(' ');
					MoveCursor(-1, 0);
				}
				pos--;
			}
		
		default: break;
		}
	}
}

char TelnetConsole::ReadChar()
{
	while (true)
	{
		auto key = ReadKey();
		switch (key.Type)
		{
		case Char:
		case Enter:
			return key.Char;		
		default: break;
		}
	}
}

bool TelnetConsole::WaitForEnter()
{
	for (auto key = ReadKey(); key.Char == 13; key = ReadKey()) {}

	return true;
}


bool TelnetConsole::Write(int pBytes, BYTE *pData)
{
	auto required = MultiByteToWideChar(CP_UTF8, 0, (char*)pData, pBytes, NULL, 0);
	auto wideStr = static_cast<wchar_t*>(malloc(required*sizeof(wchar_t)));
	auto buf = static_cast<wchar_t*>(malloc(required*3+sizeof(wchar_t)));
	auto charLen = MultiByteToWideChar(CP_UTF8, 0, (char*)pData, pBytes, wideStr,required);
	auto offset = 0;
	
	for (auto i = 0; i < charLen;i++)
	{
		
		if (wideStr[i] >= 32 || wideStr[i] == '\n'|| wideStr[i]=='\r') 
		{
			buf[offset] = wideStr[i];

			offset++;
			if (buf[offset-1] == 13)
			{
				_row++;
				_col = 0;
				if (_row >= _height) {
					_buffer->ScrollUp();
					_row = _height - 1;
				}
			}
			else if (buf[offset-1] >= 32)
			{
				ConsoleChar chr;
				chr.Char = wideStr[i];
				chr.Style = _style;
				_buffer->Set(_col, _row, chr);
								
				_col++;
				if (_col == _width)
				{
					buf[offset++] = '\n';
					buf[offset++] = '\r';

					_row++;
					_col = 0;
					if (_row >= _height) {
						_buffer->ScrollUp();
						_row = _height - 1;
					}
				}
			}

			
		}		
	}
	free(wideStr);
	required = WideCharToMultiByte(CP_UTF8, 0, buf, offset, 0, 0, 0, NULL);
	auto bytes = (char*)malloc(required);
	auto byteLength = WideCharToMultiByte(CP_UTF8, 0, buf, offset, bytes, required, 0, NULL);
	auto result = _session->Write(byteLength, reinterpret_cast<BYTE*>(bytes));
	free(bytes);
	free(buf);
	return result;
}


void TelnetConsole::PauseRendering()
{
	_bufferStack.push_back(_buffer);
	_buffer = new ConsoleBuffer(_width, _height);
	_renderingPaused = true;
}
void TelnetConsole::ResumeRendering()
{
	auto oldBuffer = new ConsoleBuffer(_width, _height);
	_renderingPaused = false;
	oldBuffer->Resize(_width, _height);
	for (auto y = 0; y < _height; y++)
		for (auto x = 0; x < _width; x++)
		{
			//memcmp()
		}
}

