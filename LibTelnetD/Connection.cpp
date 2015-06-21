#include "stdafx.h"
#include "Connection.h"

Connection::Connection(SOCKET pSocket) 
{
	_socket = pSocket;
}

Connection::~Connection() {
	closesocket(_socket);
}

int Connection::Read(int pBufferLen, BYTE *pBuffer) {
	auto read = recv(_socket, reinterpret_cast<char *>(pBuffer), pBufferLen, 0);
	if (read == SOCKET_ERROR)
		throw WSAGetLastError();
	return read;
}

bool Connection::Write(int pBytes, BYTE *data) 
{
	return send(_socket,reinterpret_cast<const char*>(data),pBytes,0) != SOCKET_ERROR;
}






