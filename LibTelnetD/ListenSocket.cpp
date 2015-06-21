#include "stdafx.h"
#include "ListenSocket.h"
#include "Connection.h"

ListenSocket::ListenSocket(u_short pPort) {
	_port = pPort;
	_socket = INVALID_SOCKET;
}

ListenSocket::~ListenSocket() {
	if (_socket != INVALID_SOCKET)
		closesocket(_socket);
}

bool ListenSocket::Start() {
	sockaddr_in local;

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(_port);

	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
		return false;


	if (::bind(server, (sockaddr*)&local, sizeof(local)) != 0)
	{
		closesocket(server);
		return false;
	}

	if (listen(server, 10) != 0)
	{
		closesocket(server);
		return false;
	}
	_socket = server;
}

SOCKET ListenSocket::Accept(timeval *pTimeout) {
	SOCKET client;
	sockaddr_in from;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(_socket, &readfds);
	auto activity = select(0, &readfds, NULL, NULL, pTimeout);

	if (activity == SOCKET_ERROR)
	{
		printf("select call failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	if (FD_ISSET(_socket, &readfds))
	{
		int fromlen = sizeof(from);
		client = accept(_socket,
			(struct sockaddr*)&from, &fromlen);
		return client;
	}
	return INVALID_SOCKET;
}

void ListenSocket::Stop() {
	if (_socket != INVALID_SOCKET)
		closesocket(_socket);
	_socket = INVALID_SOCKET;
}


