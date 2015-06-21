#pragma once

bool ProcessConnection(SOCKET pSocket,  char *pWelcomeInfo);
int ListenForOneConnection(int pPort, SOCKET *pSocket, int *pWsaError, timeval *pTimeout);
int ConnectTo(char *pIpAddress, int pPort, SOCKET *pSocket, int *pWsaError);
bool InitNetworking(int *pWsaError);
bool ShutDownNetworking();
bool ExecuteCommand(char *pCommandLine);
