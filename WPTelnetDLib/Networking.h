#pragma once

#include "pch.h"
#include<string>

class Connection {
private:
	SOCKET _socket;
	char _buf[4096];
	char _lineBuf[4096];
	int _used;
public:
	Connection(SOCKET pSocket);
	~Connection();
	const char * ReadLine();
	bool WaitForEnter();
	bool WriteLine(std::string pString);
	bool Write(int pBytes, char *data);
	bool Write(std::string pString);
	bool WriteLine(std::wstring pString);
	bool WriteLine( const char* pMsg, ...);
	bool Write( const char* pMsg, ...);
	bool Write(char pChar);
	bool WriteLastError();
};

class ListenSocket 
{
private:
	u_short _port;
	SOCKET _socket;
public:
	ListenSocket(u_short pPort);
	~ListenSocket() ;
	bool Start();
	SOCKET Accept();
	void Stop();
};

