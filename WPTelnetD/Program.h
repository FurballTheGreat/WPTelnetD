#pragma once
bool ProcessConnection(SOCKET pSocket, char *pWelcomeInfo);
int ListenForOneConnection(int pPort, SOCKET *pSocket, int *pWsaError);
int ConnectTo(char *pIpAddress, int pPort, SOCKET *pSocket, int *pWsaError);
bool InitNetworking(int *pWsaError);
bool ShutDownNetworking();