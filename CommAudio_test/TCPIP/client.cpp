#include "client_menu.h"

LPSOCKET_INFORMATION SocketInfo;

VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
	DWORD RecvBytes = 0, Flags = 0;
	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;
	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);


	if(BytesTransferred == 0 || Error != 0){
		//MessageBox::Show("Connection has been closed.");
		return;
	}

	if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR *) &SenderAddr, &SenderAddrSize, &(SI->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING ){
			// error message
			return;
		}
	}
	if (WSAWaitForMultipleEvents(1, &(SI->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
		//MessageBox::Show(errMsg);
		return;
	}
	WSAResetEvent(SI->Overlapped.hEvent);
}

void run_client(System::Windows::Forms::ListBox ^lb)
{
	DWORD Flags = 0, RecvBytes = 0;
	int SenderAddrSize = sizeof (SocketInfo->SenderAddr);

	ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));  
	//SocketInfo->Buffer = (char*) malloc(sizeof(ControlPacket));
	SocketInfo->BytesSEND = 0;
	SocketInfo->BytesRECV = 0;
	SocketInfo->DataBuf.len = sizeof(SocketInfo->Buffer);
	SocketInfo->DataBuf.buf = SocketInfo->Buffer;
	SocketInfo->Overlapped.hEvent = WSACreateEvent();

	while (1) {

		SocketInfo->Overlapped.hEvent = WSACreateEvent();

		if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(SocketInfo->SenderAddr), &SenderAddrSize,
			&(SocketInfo->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
				if (WSAGetLastError() != WSA_IO_PENDING){
					exit(1);
					// error message
				}
		}
		if (WSAWaitForMultipleEvents(1, &(SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
			exit(1);
			// error message
		}
		WSAResetEvent(SocketInfo->Overlapped.hEvent);
	}
}

int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port)
{
	int nRet;
	BOOL  fFlag;
	struct ip_mreq stMreq;
	WSADATA stWSAData;

	WSAStartup(0x0202, &stWSAData);

	if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		// error message
		return 0;
	} 

	/* Get a datagram socket */
	SocketInfo->Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (SocketInfo->Socket == INVALID_SOCKET) 
	{
		WSACleanup();
		exit(1);
	}

	fFlag = TRUE;
	nRet = setsockopt(SocketInfo->Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));

	if (nRet == SOCKET_ERROR)
	{
		printf ("setsockopt() SO_REUSEADDR failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	SocketInfo->SenderAddr.sin_family      = AF_INET;
	SocketInfo->SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SocketInfo->SenderAddr.sin_port        = htons(port);

	nRet = bind(SocketInfo->Socket, (struct sockaddr*) &(SocketInfo->SenderAddr), sizeof(SocketInfo->SenderAddr));

	if (nRet == SOCKET_ERROR)
	{
		exit(0);
		// error message
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(ip);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));

	if (nRet == SOCKET_ERROR) 
	{
		exit(0);
		// error message
	}
	return 0;
}

/*
int multicast_disconnect()
{

stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
stMreq.imr_interface.s_addr = INADDR_ANY;
nRet = setsockopt(hSocket, 
IPPROTO_IP, 
IP_DROP_MEMBERSHIP, 
(char *)&stMreq, 
sizeof(stMreq));
if (nRet == SOCKET_ERROR) {
printf (
"setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
achMCAddr, WSAGetLastError());
} 

closesocket(hSocket);
WSACleanup();
}*/