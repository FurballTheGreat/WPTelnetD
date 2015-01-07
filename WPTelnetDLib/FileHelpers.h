#pragma once
#include "pch.h"
#include<string>
bool FileExists(std::string pFileName);
std::string GetCurrentDirectory();

class TextReader{
private:
	char _buf[4096];
	char _lineBuf[0xFFFF];
	int _used;
public:
	TextReader() {
		_used = 0;
	}
	bool ReadLine(std::string &pLine);
	virtual int Read(char *pBuf, int pBufLength) = 0;
};

class TextFileReader : public TextReader {
private:
	HANDLE _file;
public:
	TextFileReader(std::string pFileName);
	~TextFileReader();
	int Read(char *pBuf, int pBufLength);
};


class DownloadTextFileReader : public TextReader {
private:
	HINTERNET _handle;
	HINTERNET _connectHandle;
public:
	DownloadTextFileReader(std::string pFileName);
	~DownloadTextFileReader();
	int Read(char *pBuf, int pBufLength);
};