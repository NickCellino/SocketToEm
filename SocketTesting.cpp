// SocketTesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinUDPSocket.h"
#include <iostream>
#include "global.h"

double GetRandomNormalized()
{
	return (double)(rand() - (RAND_MAX >> 1)) / (double)(RAND_MAX >> 1);
}

// Sample camera capture class
class CLEyeCameraCapture
{
	CHAR _windowName[256];
	GUID _cameraGUID;
	CLEyeCameraInstance _cam;
	CLEyeCameraColorMode _mode;
	CLEyeCameraResolution _resolution;
	float _fps;
	HANDLE _hThread;
	bool _running;
public:
	CLEyeCameraCapture(LPSTR windowName, GUID cameraGUID, CLEyeCameraColorMode mode, CLEyeCameraResolution resolution, float fps) :
		_cameraGUID(cameraGUID), _cam(NULL), _mode(mode), _resolution(resolution), _fps(fps), _running(false)
	{
		strcpy(_windowName, windowName);
	}
	bool StartCapture()
	{
		_running = true;
		cvNamedWindow(_windowName, CV_WINDOW_AUTOSIZE);
		// Start CLEye image capture thread
		_hThread = CreateThread(NULL, 0, &CLEyeCameraCapture::CaptureThread, this, 0, 0);
		if (_hThread == NULL)
		{
			MessageBox(NULL, _T("Could not create capture thread"), _T("CLEyeMulticamTest"), MB_ICONEXCLAMATION);
			return false;
		}
		return true;
	}
	void StopCapture()
	{
		if (!_running)	return;
		_running = false;
		WaitForSingleObject(_hThread, 1000);
		cvDestroyWindow(_windowName);
	}
	void IncrementCameraParameter(int param)
	{
		if (!_cam)	return;
		printf("CLEyeGetCameraParameter %d\n", CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param));
		CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param) + 10);
	}
	void DecrementCameraParameter(int param)
	{
		if (!_cam)	return;
		printf("CLEyeGetCameraParameter %d\n", CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param));
		CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param) - 10);
	}
	void Run()
	{
		int w, h;
		IplImage *pCapImage;
		PBYTE pCapBuffer = NULL;
		// Create camera instance
		_cam = CLEyeCreateCamera(_cameraGUID, _mode, _resolution, _fps);
		if (_cam == NULL)		return;
		// Get camera frame dimensions
		CLEyeCameraGetFrameDimensions(_cam, w, h);
		// Depending on color mode chosen, create the appropriate OpenCV image
		if (_mode == CLEYE_COLOR_PROCESSED || _mode == CLEYE_COLOR_RAW)
			pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
		else
			pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

		// Set some camera parameters
		CLEyeSetCameraParameter(_cam, CLEYE_GAIN, 0);
		CLEyeSetCameraParameter(_cam, CLEYE_EXPOSURE, 511);
		CLEyeSetCameraParameter(_cam, CLEYE_ZOOM, (int)(GetRandomNormalized()*100.0));
		CLEyeSetCameraParameter(_cam, CLEYE_ROTATION, (int)(GetRandomNormalized()*300.0));

		// Start capturing
		CLEyeCameraStart(_cam);
		cvGetImageRawData(pCapImage, &pCapBuffer);
		// image capturing loop
		while (_running)
		{
			CLEyeCameraGetFrame(_cam, pCapBuffer);

			cvShowImage(_windowName, pCapImage);
		}
		// Stop camera capture
		CLEyeCameraStop(_cam);
		// Destroy camera object
		CLEyeDestroyCamera(_cam);
		// Destroy the allocated OpenCV image
		cvReleaseImage(&pCapImage);
		_cam = NULL;
	}
	static DWORD WINAPI CaptureThread(LPVOID instance)
	{
		// seed the rng with current tick count and thread id
		srand(GetTickCount() + GetCurrentThreadId());
		// forward thread to Capture function
		CLEyeCameraCapture *pThis = (CLEyeCameraCapture *)instance;
		pThis->Run();
		return 0;
	}
};

int _tmain(int argc, _TCHAR* argv[]) {

	CLEyeCameraCapture *cam[2] = { NULL };
	srand(GetTickCount());
	// Query for number of connected cameras
	int numCams = CLEyeGetCameraCount();
	if (numCams == 0)
	{
		printf("No PS3Eye cameras detected\n");
		return -1;
	}
	printf("Found %d cameras\n", numCams);
	for (int i = 0; i < numCams; i++)
	{
		char windowName[64];
		// Query unique camera uuid
		GUID guid = CLEyeGetCameraUUID(i);
		printf("Camera %d GUID: [%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x]\n",
			i + 1, guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2],
			guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		sprintf(windowName, "Camera Window %d", i + 1);
		// Create camera capture object
		// Randomize resolution and color mode
		cam[i] = new CLEyeCameraCapture(windowName, guid, rand()<(RAND_MAX >> 1) ? CLEYE_COLOR_PROCESSED : CLEYE_MONO_PROCESSED,
			rand()<(RAND_MAX >> 1) ? CLEYE_VGA : CLEYE_QVGA, 30);
		printf("Starting capture on camera %d\n", i + 1);
		cam[i]->StartCapture();
	}
	printf("Use the following keys to change camera parameters:\n"
		"\t'1' - select camera 1\n"
		"\t'2' - select camera 2\n"
		"\t'g' - select gain parameter\n"
		"\t'e' - select exposure parameter\n"
		"\t'z' - select zoom parameter\n"
		"\t'r' - select rotation parameter\n"
		"\t'+' - increment selected parameter\n"
		"\t'-' - decrement selected parameter\n");
	// The <ESC> key will exit the program
	CLEyeCameraCapture *pCam = NULL;
	int param = -1, key;
	while ((key = cvWaitKey(0)) != 0x1b)
	{
		switch (key)
		{
		case 'g':	case 'G':	printf("Parameter Gain\n");		param = CLEYE_GAIN;		break;
		case 'e':	case 'E':	printf("Parameter Exposure\n");	param = CLEYE_EXPOSURE;	break;
		case 'z':	case 'Z':	printf("Parameter Zoom\n");		param = CLEYE_ZOOM;		break;
		case 'r':	case 'R':	printf("Parameter Rotation\n");	param = CLEYE_ROTATION;	break;
		case '1':				printf("Selected camera 1\n");	pCam = cam[0];			break;
		case '2':				printf("Selected camera 2\n");	pCam = cam[1];			break;
		case '+':	if (pCam)	pCam->IncrementCameraParameter(param);		break;
		case '-':	if (pCam)	pCam->DecrementCameraParameter(param);		break;
		}
	}

	for (int i = 0; i < numCams; i++)
	{
		printf("Stopping capture on camera %d\n", i + 1);
		cam[i]->StopCapture();
		delete cam[i];
	}
	return 0;
}
//const u_short PORT = 9876;
//const int MAX_CONNECTIONS = 1D:\C++_Libraries\CL-Eye Platform SDK\Include\opencv2;
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
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	std::cout << "Starting up..." << std::endl;
//
//	int port = 9876;
//	char* clientIp = "192.168.1.5";
//	int clientPort = port;
//	WinUDPSocket socket(port);
//	char* message = "Hello fellow udp socket program!!!!";
//	int len = strlen(message);
//	socket.sendData(message, len, clientIp, clientPort);
//
//	//WSADATA wsaData;
//	//if (start_winsock(&wsaData) == -1) {
//	//	return -1;
//	//}
//
//	//SOCKET mySocket;
//	//SOCKADDR_IN addr;
//	//configureSocketAddressInfo(&addr);
//
//	////mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	//mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//
//	//if (mySocket == INVALID_SOCKET) {
//	//	std::cout << "Error creating socket." << std::endl;
//	//	return -1;
//	//}
//	//
//	//if (bind(mySocket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
//	//	std::cout << "Error binding socket." << std::endl;
//	//	return -1;
//	//}
//
//	//listen(mySocket, MAX_CONNECTIONS);
//	//std::cout << "Listening on port " << PORT << std::endl;
//	//	
//	//SOCKADDR_IN incomingConnectionInfo;
//	//int connInfoSize = sizeof(incomingConnectionInfo);
//	//SOCKET connectedSocket;
//	//char *messageToClients = "Wassup client bitch!";
//	//int len;
//	//len = strlen(messageToClients);
//
//	//while (true) {
//	//	connectedSocket = accept(mySocket, (LPSOCKADDR)&incomingConnectionInfo, &connInfoSize);
//	//	if (connectedSocket == -1) {
//	//		continue;
//	//	}
//	//	if (sendall(connectedSocket, messageToClients, &len) == -1) {
//	//		std::cout << "Error sending requested data. " << std::endl;
//	//	}
//	//	else {
//	//		std::cout << "Just sent " << len << " bytes to client. I hope he or she enjoyed them" << std::endl;
//	//	}
//	//}
//
//	return 0;
//}
//
