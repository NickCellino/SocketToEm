#pragma once
#include <queue>

struct DataPacket {
	char *beginning;
	int length;
};


class WinUDPSocket
{
public:
	WinUDPSocket(int port);
	~WinUDPSocket();
	void configureSocketAddressInfo(int port);
	int bindSocket();
	int startWinsock();
	int receiveData(char *buffer, int maxPacketSize);
	int sendData(char *buffer, int len);
	//void listenForPackets(int maxPacketSize);
	//bool hasNewPacket();
	//DataPacket popPacketFromQueue();
private:
	//void putPacketOnQueue(DataPacket dataPacket);
	//std::queue<DataPacket> dataPackets;
	SOCKET socketFd;
	SOCKADDR_IN sock_in_addr;
	static WSAData wsaData;
	static bool isFirstSocket;
	
};

