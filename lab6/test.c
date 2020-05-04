#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 2
#define DEFAULT_PORT "27123"

//#include <Strsafe.h>

LRESULT CALLBACK NaszaProcedura (HWND hwnd, UINT message, WPARAM wpar, LPARAM lpar);
HWND hOkno;

char wyslij(char znak[2])
{
	char ret = 0;
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	int i;
		for (i=0; i < DEFAULT_BUFLEN; i++)
			recvbuf[i] = 0;
		for (i=0; i < DEFAULT_BUFLEN; i++)
			sendbuf[i] = 0;
		// Validate the parameters
		

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			system("PAUSE");
			return 1;
		}

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
		if ( iResult != 0 ) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			system("PAUSE");
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				system("PAUSE");
				WSACleanup();
				return 1;
			}

			// Connect to server.
			iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			system("PAUSE");
			WSACleanup();
			return 1;
		}
		for (i=0; i < 2; i++)
			sendbuf[i] = znak[i];

		// Send an initial buffer
		iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			system("PAUSE");
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);

		// shutdown the connection since no more data will be sent
		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			system("PAUSE");
			return 1;
		}

		// Receive until the peer closes the connection
		do {

			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if ( iResult > 0 )
			{
				ret = recvbuf[0];
				//printf("Bytes received: %d\n", iResult);
			}
			else if ( iResult == 0 )
			{
				printf("Connection closed\n");
				//system("PAUSE");
			}
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				system("PAUSE");
			}

		} while( iResult > 0 );

		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();	
		return ret;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpsCmdLine, int nMode)
{
	MSG message;
	WNDCLASS okno;
     
	okno.hInstance = hInstance;
	okno.lpszClassName = "klasa g³ówna";
	okno.lpfnWndProc = NaszaProcedura; //tutaj pojawia siê nowoœæ!!
	okno.lpszMenuName = NULL;
	okno.style = 0;
	okno.hIcon = LoadIcon (NULL, IDI_WINLOGO);
	okno.hCursor = LoadCursor (NULL, IDC_ARROW);
	okno.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	okno.cbClsExtra = 0;
	okno.cbWndExtra = 0;
	if(!RegisterClass (&okno)) return 0;
	hOkno = CreateWindow ("klasa g³ówna", "",
		WS_OVERLAPPEDWINDOW, 100,100,900,100,
		NULL, NULL, hInstance, NULL);
	ShowWindow (hOkno, SW_SHOW);
	while (GetMessage (&message, NULL, 0, 0))
	{
		TranslateMessage (&message);
		DispatchMessage (&message);
	}
}
