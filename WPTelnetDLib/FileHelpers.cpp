#include "pch.h"
#include "FileHelpers.h"
#include<string>

bool FileExists(std::string pFileName)
{
	WIN32_FIND_DATA findFileData;
	HANDLE handle = FindFirstFileA(pFileName.c_str(), &findFileData);
	bool found = handle != INVALID_HANDLE_VALUE;
	if (found)
		FindClose(handle);
	return found;
}

std::string GetCurrentDirectory() {
	char buf[1024];
	GetCurrentDirectoryA(sizeof(buf), buf);
	return std::string(buf);
}



TextFileReader::TextFileReader(std::string pFileName){
	_file = CreateFileA(pFileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (_file == (HANDLE)-1) {
		throw GetLastError();
	}

	

}
TextFileReader::~TextFileReader(){
	if (!CloseHandle(_file)) {
		throw GetLastError();
	}
}


int TextFileReader::Read(char *pBuf, int pLength){
	DWORD read;
	ReadFile(_file, pBuf, sizeof(pBuf) - 1, &read, NULL);
	return read;
}



DownloadTextFileReader::DownloadTextFileReader(std::string pUrl){
	_handle = InternetOpenA("Microsoft Internet Explorer",
		PRE_CONFIG_INTERNET_ACCESS,
		NULL,
		INTERNET_INVALID_PORT_NUMBER,
		0);

	DWORD size;
	CHAR headerStr[256] = "Accept: */*\r\n\r\n";




	if (!(_connectHandle = InternetOpenUrlA(_handle, strdup(pUrl.c_str()),
		headerStr,
		strlen(headerStr), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
	{
		throw std::string("Error downloading file");
	}
	DWORD dwByteToRead = 0;
	DWORD sizeOfRq = 4;
	DWORD dwBytes = 0;

	if (!HttpQueryInfoA(_connectHandle, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
		(LPVOID)&dwByteToRead, &sizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}

}
DownloadTextFileReader::~DownloadTextFileReader(){
	/*if (!CloseHandle(_handle)) {
		
		throw GetLastError();
	}*/
}


int DownloadTextFileReader::Read(char *pBuf, int pLength){
	DWORD read;
	if (InternetReadFile(_connectHandle, pBuf, pLength, &read))
		return read;
	return 0;
}

bool TextReader::ReadLine(std::string &pLine){
	
	DWORD read;
	BOOL ok = TRUE;
	
	while (true){
		if (_used > 0){
			char *ptr = _buf;

			while ((*ptr != '\n' && *ptr != '\r') && (ptr - _buf) < _used)
				ptr++;


			if ((ptr - _buf)<_used) {
				*ptr = 0;
				strcpy_s(_lineBuf, _buf);
				ptr++;
				while ((*ptr == '\n' || *ptr == '\r') && (ptr - _buf) < _used)
					ptr++;

				memcpy(_buf, ptr, sizeof(_buf) - (ptr - _buf));
				_used -= (ptr - _buf);
				pLine = std::string(_lineBuf);
				return true;
			}
			if ((ptr - _buf) == sizeof(_buf))
				_used = 0;
		}
		read = Read(_buf, sizeof(_buf)); 
		
		if (!read)
			break;

		_used += read;

	}
	if (_used > 0)
	{
		_buf[_used] = 0;
		_used = 0;
		pLine = std::string(_buf);
		return true;
	}
	return false;
}