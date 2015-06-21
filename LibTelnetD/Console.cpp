#include "stdafx.h"
#include "Console.h"

bool IConsole::Write(std::string pString)
{
	return Write(pString.length(), (BYTE*)pString.c_str());
}


bool IConsole::WriteLine(std::wstring pString)
{
	
	return Write(pString+L"\n\r");
}

bool IConsole::Write(std::wstring pString)
{
	auto required = WideCharToMultiByte(CP_UTF8, 0, pString.c_str(), pString.length(), NULL, 0, NULL, NULL);
	char *buf = (char *)malloc(required);
	auto length = WideCharToMultiByte(CP_UTF8, 0, pString.c_str(), pString.length(), buf, required, NULL, NULL);
	auto result = Write(length, (BYTE *)buf);
	free(buf);
	return result;
}
bool IConsole::WriteLine(const char* pMsg, ...)
{
	char buffer[4096];
	va_list arg;
	va_start(arg, pMsg);
	int n = vsnprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	buffer[n++] = '\n';
	buffer[n++] = '\r';
	buffer[n] = 0;
	return Write(n, reinterpret_cast<BYTE*>(buffer));

}

bool IConsole::WriteLine(const wchar_t* pMsg, ...)
{
	wchar_t buffer[4096];
	char *out = (char*)malloc(8096);
	va_list arg;
	va_start(arg, pMsg);
	int n = _vsnwprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	
	buffer[n] = 0;
	int size  = WideCharToMultiByte(CP_UTF8, 0, buffer,n, out, 8094, NULL, NULL);
	out[size++] = '\n';
	out[size++] = '\r';
	if (size > 0) {
		auto result = Write(size, reinterpret_cast<BYTE*>(out));
		free(out);
		return result;
	} else
	{
		free(out);
		throw GetLastError();
	}
}

bool IConsole::WriteLine()
{
	return WriteLine("");
}
bool IConsole::Write(const char* pMsg, ...)
{
	char buffer[4096];
	va_list arg;
	va_start(arg, pMsg);
	int n = vsnprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	return Write(n, reinterpret_cast<BYTE*>(buffer));
}

bool IConsole::Write(const wchar_t* pMsg, ...)
{
	wchar_t buffer[4096];
	char *out = (char*)malloc(8096);
	va_list arg;
	va_start(arg, pMsg);
	int n = _vsnwprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	buffer[n] = 0;
	int size = WideCharToMultiByte(CP_UTF8, 0, buffer, n, out, 8096, NULL, NULL);
	if (size > 0) {
		auto result = Write(size, reinterpret_cast<BYTE*>(out));
		free(out);
		return result;
	}
	else
	{
		free(out);
		throw GetLastError();
	}
}

bool IConsole::Write(char pChar)
{
	return Write(1, reinterpret_cast<BYTE*>(&pChar));
}

bool IConsole::WriteLine(std::string pString)
{
	auto line = pString + "\n\r";
	Write(line.length(), (BYTE*)line.c_str());
	return true;
}

ConsoleBuffer::ConsoleBuffer(int pWidth, int pHeight)
{
	_width = pWidth;
	_height = pHeight;
	_buffer = static_cast<ConsoleChar *>(malloc(pWidth* pHeight*sizeof(ConsoleChar)));
}

ConsoleBuffer::~ConsoleBuffer()
{
	free(_buffer);	
}
int ConsoleBuffer::GetWidth()
{
	return _width;	
}
int ConsoleBuffer::GetHeight()
{
	return _height;
}

ConsoleBuffer ConsoleBuffer::Clone()
{
	ConsoleBuffer result(_width, _height);
	memcpy(result._buffer, _buffer, sizeof(ConsoleChar)*_width*_height);
	return result;
}

void ConsoleBuffer::ScrollUp()
{
	for (auto y = 0; y < _height-1; y++)
		memcpy(&_buffer[y*_width], &_buffer[(y+1)*_width], _width*sizeof(ConsoleChar));
}

void ConsoleBuffer::Resize(int pWidth, int pHeight)
{
	if (pWidth == _width && pHeight == _height)
		return;
	auto newBuffer = static_cast<ConsoleChar *>(malloc(pWidth* pHeight*sizeof(ConsoleChar)));

	if (_buffer != nullptr) {
		auto copyWidth = pWidth > _width ? _width : pWidth;
		auto copyHeight = pHeight > _height ? _height : pHeight;
		for (auto y = 0; y < copyHeight; y++)
			memcpy(&newBuffer[y*pWidth], &_buffer[y*_width], copyWidth*sizeof(ConsoleChar));

		free(_buffer);
	}

	_buffer = newBuffer;
	_width = pWidth;
	_height = pHeight;
}
ConsoleChar ConsoleBuffer::Get(int pX, int pY)
{
	if (pX >= _width || pY >= _height || pX < 0 || pY < 0)
		throw 0;

	return _buffer[pY*_width + pX];
}
void ConsoleBuffer::Set(int pX, int pY, ConsoleChar pChar)
{
	if (pX >= _width || pY >= _height || pX < 0 || pY < 0)
		throw 0;

	memcpy(&_buffer[pY*_width + pX], &pChar, sizeof(ConsoleChar));
}
