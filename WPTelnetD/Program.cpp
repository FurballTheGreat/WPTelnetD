// Main Entry Point for TelnetD
//

#include "pch.h"


#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include<string.h>
#include<vector>
#include<string>
#include "TelnetD.h"
#include "Networking.h"
#include "CommandProcessor.h"
#include "Commands.h"

#ifdef PHONE
#include<WPKernel.h>
#endif

int main(int args, char *argv[]){
	printf("WP8\n");
	int wsaError;
	if (!InitNetworking(&wsaError)) {
		printf("Failed to initialise networking (WSAError: %d)", wsaError);
		return 1;
	}

	SOCKET socket;

	if (ListenForOneConnection(27249, &socket, &wsaError)){
		printf("Failed to to accept connection (WSAError: %d)", wsaError);
		ShutDownNetworking();
		return 1;
	}

	string str = "Arguments:\n";
	for (int i = 0; i < args; i++){
		char buf[1024];
		sprintf_s(buf, "arg[%d]=%s", i, argv[i]);
		str = str + buf;
	}
	char *msg = strdup(str.c_str());
	ProcessConnection(socket,msg );
	free(msg);

	ShutDownNetworking();	
}