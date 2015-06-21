#pragma once

#include "stdafx.h"
#include<string>
#include "TelnetConsole.h"



class Connection {
	SOCKET _socket;
public:
	Connection(SOCKET pSocket);
	~Connection();
	int Read(int pBufferLen, BYTE *pBuffer);
	bool Write(int pBytesCount, BYTE *pData);
};


