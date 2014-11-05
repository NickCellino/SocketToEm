#pragma once
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

class WinUDPSocket
{
public:
	WinUDPSocket(int port);
	~WinUDPSocket();
	void configureSocketAddressInfo(int port);
	int bindSocket();
	int startWinsock();
	int receiveData(char *buffer, int maxPacketSize);
	int sendData(char* buffer, int len, char* host, int port);
private:
	SOCKET socketFd;
	SOCKADDR_IN sock_in_addr;
	SOCKADDR_IN last_address;
	static bool isFirstSocket;
	
};

