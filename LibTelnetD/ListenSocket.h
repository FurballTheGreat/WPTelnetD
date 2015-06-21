#pragma once

class Connection;

class ListenSocket
{
private:
	u_short _port;
	SOCKET _socket;
public:
	ListenSocket(u_short pPort);
	~ListenSocket();
	bool Start();
	SOCKET Accept(timeval *pTimeout);
	void Stop();
};

