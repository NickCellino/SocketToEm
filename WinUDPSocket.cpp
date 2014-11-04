#include "WinUDPSocket.h"
#include "stdafx.h"
#include "winsock.h"
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

bool WinUDPSocket::isFirstSocket = true;

WinUDPSocket::WinUDPSocket(int port) 
{
	if (this->isFirstSocket) {
		startWinsock();
		isFirstSocket = false;
		std::cout << "started winsock API... " << std::endl; 
	}
	socketFd = (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	configureSocketAddressInfo(port);
	bindSocket();
}


WinUDPSocket::~WinUDPSocket()
{
}

int WinUDPSocket::receiveData(char *buffer, int maxPacketSize) {
	int received = 0;

	sockaddr_in source;
	int source_size = sizeof(source);

	if ((received = recvfrom(socketFd, buffer, maxPacketSize, 0,
		(sockaddr*)&source, &source_size)) == SOCKET_ERROR){
		printf("ERROR #%d\n", WSAGetLastError());
		return SOCKET_ERROR;
	}

	//last_address = source;
	return received;
}

int WinUDPSocket::sendData(char *buffer, int len)
{
	// TODO
	return 1;
}
//
//void WinUDPSocket::listenForPackets(int maxPacketSize) 
//{
//	char *dataBuffer = new char[2 * maxPacketSize];
//	int dataBufferLength = 2 * maxPacketSize;
//	while (1) {
//		SOCKADDR_IN originAddr;
//		int connInfoSize = sizeof(originAddr);
//		recvfrom(sockFd, dataBuffer, )
//	}
//}
//
//void WinUDPSocket::putPacketOnQueue(DataPacket dataPacket) 
//{
//	dataPackets.push(dataPacket);
//}
//
//bool WinUDPSocket::hasNewPacket() {
//	if (dataPackets.empty()) 
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//}

void WinUDPSocket::configureSocketAddressInfo(int port) 
{
	sock_in_addr.sin_family = AF_INET;
	sock_in_addr.sin_port = htons(port);
	sock_in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

int WinUDPSocket::bindSocket()
{
	if (bind(socketFd, (LPSOCKADDR)&sock_in_addr, sizeof(sock_in_addr)) == SOCKET_ERROR) {
		std::cout << "Error binding socket." << std::endl;
		return -1;
	}
	else {
		return 0;
	}

}

int WinUDPSocket::startWinsock()
{
	int error = WSAStartup(0x0202, &wsaData);
	if (error) {
		std::cout << "Error starting winsock API." << std::endl;
		return -1;
	}
	if (wsaData.wVersion != 0x0202) {
		std::cout << "Wrong winsock version or something..." << std::endl;
		return -1;
	}
	return 0;
}
