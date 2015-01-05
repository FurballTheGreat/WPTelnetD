#include "pch.h"
#include "Networking.h"

Connection::Connection(SOCKET pSocket) 
{
	_socket = pSocket;
	_used = 0;
	memset(_buf, 0, sizeof(_buf));
}

Connection::~Connection() {
	closesocket(_socket);
}

const char *Connection::ReadLine() {
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
				return _lineBuf;
			}
			if ((ptr - _buf) == sizeof(_buf))
				_used = 0;
		}
		int read = recv(_socket, _buf + _used, sizeof(_buf) - _used, 0);
		if (read == 0)
			return NULL;

		_used += read;
		
	}
}

bool Connection::WaitForEnter() {
	char buf[10];
	buf[0] = 0;
	while(buf[0]!=13 && buf[0]!=10) {
		if(recv(_socket, buf, 1,0)<1)
			return false;
	}
}


bool Connection::WriteLine(std::wstring pString)
{
	std::string strA = std::string(pString.begin(), pString.end())+"\n\r";
	const char *str = strA.c_str();
	int length = strlen(str);
	return send(_socket, str, length, 0) != SOCKET_ERROR;
}

bool Connection::WriteLine(std::string pString) 
{
	std::string str = pString + "\n\r";
	
	return send(_socket,str.c_str(),str.length(),0) != SOCKET_ERROR;
}

bool Connection::Write(char pChar)
{
	
	return send(_socket, &pChar, 1, 0) != SOCKET_ERROR;
}

bool Connection::Write(int pBytes, char *data) 
{

	return send(_socket,data,pBytes,0) != SOCKET_ERROR;
}


bool Connection::Write(std::string pString) 
{
	const char *str  = (pString).c_str();
	int length = strlen(str);
	return send(_socket,str,length,0) != SOCKET_ERROR;
}

bool Connection::WriteLine( const char* pMsg, ...)
{
	char buffer[4096];
	va_list arg;
	va_start(arg, pMsg);
	int n = vsnprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	buffer[n++] = '\n';
	buffer[n++] = '\r';
	buffer[n] = 0;
	return send(_socket,buffer,n,0) != SOCKET_ERROR;
};

bool Connection::Write( const char* pMsg, ...)
{
	char buffer[4096];
	va_list arg;
	va_start(arg, pMsg);
	int n = vsnprintf_s(buffer, 4096, pMsg, arg);
	va_end(arg);
	return send(_socket,buffer,n,0) != SOCKET_ERROR;
};

bool Connection::WriteLastError() {
	char buffer[4096];
	sprintf_s(buffer, "ERROR: ");
	int err = GetLastError();
	if(!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		err,
		0,
		buffer+strlen(buffer),
		sizeof(buffer)-strlen(buffer),
		NULL) == 0)
		return WriteLine("%s (%d)", buffer,err);
	else
		WriteLine("ERROR: Unknown Error - WinCode %d", err);
}

ListenSocket::ListenSocket(u_short pPort) {
	_port = pPort;
	_socket = INVALID_SOCKET;
}

ListenSocket::~ListenSocket() {
	if(_socket!=INVALID_SOCKET)
		closesocket(_socket);
}

bool ListenSocket::Start() {
	sockaddr_in local;

	local.sin_family=AF_INET; 
	local.sin_addr.s_addr=INADDR_ANY; 
	local.sin_port=htons(_port); 

	SOCKET server =socket(AF_INET,SOCK_STREAM,0);
	if(server==INVALID_SOCKET)
		return false;

	
	if(::bind(server,(sockaddr*)&local,sizeof(local))!=0)
	{
		closesocket(server);
		return false;
	}

	if(listen(server,10)!=0)
	{
		closesocket(server);
		return false;
	}
	_socket = server;
}

SOCKET ListenSocket::Accept() {
	SOCKET client;
	sockaddr_in from;
	int fromlen=sizeof(from);
	client = accept(_socket,
		(struct sockaddr*)&from,&fromlen);
	return client;
	
}

void ListenSocket::Stop() {
	if(_socket!=INVALID_SOCKET)
		closesocket(_socket);
	_socket = INVALID_SOCKET;
}


