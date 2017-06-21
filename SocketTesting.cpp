// SocketTesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinUDPSocket.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Starting up..." << std::endl;

	int port = 9876;
	char* clientIp = "192.168.1.5";
	int clientPort = port;
	WinUDPSocket socket(port);
	char* message = "Hello fellow udp socket program!!!!";
	int len = strlen(message);
	socket.sendData(message, len, clientIp, clientPort);

	return 0;
}

