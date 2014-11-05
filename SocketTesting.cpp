// SocketTesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinUDPSocket.h"
#include <iostream>

//const u_short PORT = 9876;
//const int MAX_CONNECTIONS = 1;
//const char LEFT_CAM_IMAGE = 0x00;
//const char RIGHT_CAM_IMAGE = 0x00;
//
//int sendall(int s, char *buf, int *len) {
//	int total = 0;
//	int bytesleft = *len;
//	int n;
//
//	while (total < *len) {
//		n = send(s, buf + total, bytesleft, 0);
//		if (n == -1) {
//			break;
//		}
//		total += n;
//		bytesleft -= n;
//	}
//
//	*len = total;
//	return n == -1 ? -1 : 0;
//}
//
//int start_winsock(WSADATA *wsaData) {
//	int error = WSAStartup(0x0202, wsaData);
//	if (error) {
//		std::cout << "Error starting winsock API." << std::endl;
//		return -1;
//	}
//	if (wsaData->wVersion != 0x0202) {
//		std::cout << "Wrong winsock version or something..." << std::endl;
//		return -1;
//	}
//	return 0;
//}
//
//void configureSocketAddressInfo(SOCKADDR_IN *sock_in_addr) {
//	sock_in_addr->sin_family = AF_INET;
//	sock_in_addr->sin_port = htons(PORT);
//	sock_in_addr->sin_addr.s_addr = htonl(INADDR_ANY);
//	
//}

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

	//WSADATA wsaData;
	//if (start_winsock(&wsaData) == -1) {
	//	return -1;
	//}

	//SOCKET mySocket;
	//SOCKADDR_IN addr;
	//configureSocketAddressInfo(&addr);

	////mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//if (mySocket == INVALID_SOCKET) {
	//	std::cout << "Error creating socket." << std::endl;
	//	return -1;
	//}
	//
	//if (bind(mySocket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
	//	std::cout << "Error binding socket." << std::endl;
	//	return -1;
	//}

	//listen(mySocket, MAX_CONNECTIONS);
	//std::cout << "Listening on port " << PORT << std::endl;
	//	
	//SOCKADDR_IN incomingConnectionInfo;
	//int connInfoSize = sizeof(incomingConnectionInfo);
	//SOCKET connectedSocket;
	//char *messageToClients = "Wassup client bitch!";
	//int len;
	//len = strlen(messageToClients);

	//while (true) {
	//	connectedSocket = accept(mySocket, (LPSOCKADDR)&incomingConnectionInfo, &connInfoSize);
	//	if (connectedSocket == -1) {
	//		continue;
	//	}
	//	if (sendall(connectedSocket, messageToClients, &len) == -1) {
	//		std::cout << "Error sending requested data. " << std::endl;
	//	}
	//	else {
	//		std::cout << "Just sent " << len << " bytes to client. I hope he or she enjoyed them" << std::endl;
	//	}
	//}

	return 0;
}

