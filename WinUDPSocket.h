#pragma once

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
	static WSAData wsaData;
	static bool isFirstSocket;
	
};

