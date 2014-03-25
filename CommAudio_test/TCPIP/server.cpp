#include "server_menu.h"
/* handle to form */
HWND ghwnd;
DWORD EventTotal = 0;
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
//LPSOCKET_INFORMATION SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION CriticalSection;   

DWORD WINAPI UDPSenderThread(LPVOID lpParameter)
{
	//String^ errMsg		= gcnew String("error");
	GoodInfo* gInfo		= (GoodInfo*) lpParameter;
	DWORD Flags			= 0;
	WSABUF DataBuf;
	DataBuf.buf = gInfo->fileData;


	/*for (int i = 0; i < gInfo->numTimesSendingGI; i++){
		if (WSASendTo(gInfo->SenderSocket, &DataBuf, 1, NULL, Flags, (SOCKADDR*) &(gInfo->InternetAddr), sizeof(SOCKADDR) , &(gInfo->SendOverlapped), NULL) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				errMsg = "WSASendTo() failed with error \n" + WSAGetLastError();
				MessageBox::Show(errMsg);
				return -1;
			}
		}
		WSAWaitForMultipleEvents(1, &(gInfo->SendOverlapped.hEvent), TRUE, WSA_INFINITE, TRUE);
		WSAResetEvent(gInfo->SendOverlapped.hEvent);
	}*/

	return 1;
}


void run_udp_serv(HWND hwnd, char* fileName, int port)
{
	FILE *fp;

	WSADATA wsaData;
	WSABUF DataBuf;
	WSAOVERLAPPED Overlapped;

	SOCKET RecvSocket = INVALID_SOCKET;
	struct sockaddr_in RecvAddr;
	struct sockaddr_in SenderAddr;

	int SenderAddrSize = sizeof (SenderAddr);
	u_short Port = port;

	char RecvBuf[DATA_BUFSIZE];
	char stats[1024];

	DWORD BytesRecv, TotalBytesRecv;
	DWORD Flags = 0;

	int err = 0;
	int rc;
	int retval = 0;
	int packets_received = 0;

	if(!strcmp(fileName, ""))
		strcpy(fileName, "UDPoutput");

	/* Initialize Winsock */
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	/* Make sure the Overlapped struct is zeroed out */
	SecureZeroMemory((PVOID) &Overlapped, sizeof(WSAOVERLAPPED) );

	/* Create an event handle and setup the overlapped structure */
	Overlapped.hEvent = WSACreateEvent();

	/* Create a receiver socket to receive datagrams */
	RecvSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (RecvSocket == INVALID_SOCKET) 
	{
		/* Could not open a socket */
		//lb->Items->Add("WSASocket failed with error: " + WSAGetLastError());
		WSACloseEvent(Overlapped.hEvent);
		WSACleanup();
	}

	/* Bind the socket to any address and the specified port */
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(RecvSocket, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));

	while(1)
	{
		DataBuf.len = DATA_BUFSIZE;
		DataBuf.buf = RecvBuf;
		rc = WSARecvFrom(RecvSocket,&DataBuf, 1, &BytesRecv,&Flags,(SOCKADDR *) &SenderAddr, &SenderAddrSize, &Overlapped, NULL);
		
		if (rc != 0) 
		{
			err = WSAGetLastError();
			if (err != WSA_IO_PENDING) 
			{
				//lb->Items->Add("WSARecvFrom failed with error: " + err);
				WSACloseEvent(Overlapped.hEvent);
				closesocket(RecvSocket);
				WSACleanup();
			}
			else 
			{
				rc = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
				if (rc == WSA_WAIT_FAILED)
				{
					//lb->Items->Add("WSAWaitForMultipleEvents failed with error: " + WSAGetLastError());
				}

				rc = WSAGetOverlappedResult(RecvSocket, &Overlapped, &BytesRecv, FALSE, &Flags);
				if (rc == FALSE) 
				{
					//lb->Items->Add("WSArecvFrom failed with error:" + WSAGetLastError());
				}
			}
		}

		fp = fopen(fileName, "a+b");
		fwrite(RecvBuf, 1, BytesRecv, fp);
		fclose(fp);

		packets_received++;
		TotalBytesRecv += BytesRecv;

		PostMessage(hwnd, BYTESRECV_LISTBOX, (WPARAM)TotalBytesRecv, (LPARAM)0);
		PostMessage(hwnd, PACKETSRECV_LISTBOX, (WPARAM)packets_received, (LPARAM)0);
	}

	WSACloseEvent(Overlapped.hEvent);
	closesocket(RecvSocket);

	WSACleanup();
}
