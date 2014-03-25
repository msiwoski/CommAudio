#include "client_menu.h"

LPSOCKET_INFORMATION SocketInfo;

DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
{
	/*LPSOCKET_INFORMATION SocketInfo;
	DWORD Flags = 0, RecvBytes = 0;
	//String^ errMsg = gcnew String("error");
	GoodInfo* gInfo = (GoodInfo*) lpParameter;
	memset((char *)&SocketInfo, 0, sizeof(LPSOCKET_INFORMATION));
	int SenderAddrSize = sizeof (gInfo->SenderAddr);

	if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR,sizeof(SOCKET_INFORMATION))) == NULL){
	//errMsg = "GlobalAlloc() failed. Error: " + GetLastError();
	//MessageBox::Show(errMsg);
	return FALSE;
	}

	SocketInfo->Socket = gInfo->ListenSocket;
	ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));  
	SocketInfo->Buffer = (char*) malloc(sizeof(ControlPacket));
	SocketInfo->BytesSEND = 0;
	SocketInfo->BytesRECV = 0;
	SocketInfo->DataBuf.len = sizeof(ControlPacket);
	SocketInfo->DataBuf.buf = SocketInfo->Buffer;
	SocketInfo->Overlapped.hEvent = WSACreateEvent();

	if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(gInfo->SenderAddr), &SenderAddrSize,
	&(SocketInfo->Overlapped), UDPCtrlWorkerRoutine) == SOCKET_ERROR){
	if (WSAGetLastError() != WSA_IO_PENDING){
	//errMsg = "WSARecvFrom() failed. Error: " + WSAGetLastError();
	//MessageBox::Show(errMsg);
	return FALSE;
	}
	}
	if (WSAWaitForMultipleEvents(1, &(SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
	//MessageBox::Show("WSA_TIMEOUT");
	return FALSE;
	}
	WSAResetEvent(SocketInfo->Overlapped.hEvent);

	*/return TRUE;
}

VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
	/*DWORD RecvBytes = 0, Flags = 0;
	//String^ errMsg = gcnew String("error");
	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;
	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);


	if(BytesTransferred == 0 || Error != 0){
	//MessageBox::Show("Connection has been closed.");
	return;
	}

	if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR *) &SenderAddr, &SenderAddrSize, &(SI->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
	if (WSAGetLastError() != WSA_IO_PENDING ){
	//errMsg = "WSARecvFrom() failed with error \n" + WSAGetLastError();
	//MessageBox::Show(errMsg);
	return;
	}
	}
	if (WSAWaitForMultipleEvents(1, &(SI->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
	//MessageBox::Show(errMsg);
	return;
	}
	WSAResetEvent(SI->Overlapped.hEvent);*/
}

void run_client()
{
	int nRet;
	char achInBuf [DATA_BUFSIZE];
	SOCKADDR_IN stSrcAddr;

	while (true) {
		int addr_size = sizeof(struct sockaddr_in);

		nRet = recvfrom(SocketInfo->Socket, achInBuf, DATA_BUFSIZE, 0, (struct sockaddr*)&stSrcAddr, &addr_size);

		if (nRet < 0) {
			printf ("recvfrom() failed, Error: %d\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

	}
}

int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port)
{
	int nRet;
	BOOL  fFlag;
	SOCKADDR_IN stLclAddr;
	struct ip_mreq stMreq;
	WSADATA stWSAData;

	WSAStartup(0x0202, &stWSAData);

	if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		lb->Items->Add("GlobalAlloc() failed with error " + GetLastError());
		return 0;
	} 

	/* Get a datagram socket */
	//SocketInfo->Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SocketInfo->Socket = socket(AF_INET, SOCK_DGRAM, 0);

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
	stLclAddr.sin_family      = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port        = htons(port);

	nRet = bind(SocketInfo->Socket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));

	if (nRet == SOCKET_ERROR)
	{
		printf ("bind() port: %d failed, Err: %d\n", port, 
			WSAGetLastError());
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(ip);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));

	if (nRet == SOCKET_ERROR) 
	{
		printf("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",
			ip, WSAGetLastError());
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