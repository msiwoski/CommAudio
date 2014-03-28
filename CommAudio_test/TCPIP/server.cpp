#include "server_menu.h"
/* handle to form */
HWND ghwnd;
LPSOCKET_INFORMATION server_SocketInfo;

u_long  lTTL               = TIMECAST_TTL;
u_short nInterval          = TIMECAST_INTRVL;
SYSTEMTIME stSysTime;


int init_server(int port)
{
	int nRet, i;
	BOOL  fFlag;
	WSADATA stWSAData;

	SOCKADDR_IN stLclAddr;
	struct ip_mreq stMreq;        /* Multicast interface structure */


	WSAStartup(0x0202, &stWSAData);

	if ((server_SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		// error message
		return 0;
	} 

	fFlag = FALSE;

	server_SocketInfo->Socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (server_SocketInfo->Socket == INVALID_SOCKET) {
		printf ("socket() failed, Err: %d\n", WSAGetLastError());
		exit(1);
	}

	stLclAddr.sin_family      = AF_INET; 
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */
	stLclAddr.sin_port        = 0;                 /* any port */
	nRet = bind(server_SocketInfo->Socket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf ("bind() port: %d failed, Err: %d\n", port, 
			WSAGetLastError());
	}

	/* Join the multicast group */
	stMreq.imr_multiaddr.s_addr = inet_addr("234.5.6.7");
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf (
			"setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",
			"234.5.6.7", WSAGetLastError());
	} 

	/* Set IP TTL to traverse up to multiple routers */
	nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&lTTL, sizeof(lTTL));
	if (nRet == SOCKET_ERROR) {
		printf ("setsockopt() IP_MULTICAST_TTL failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Disable loopback */
	fFlag = FALSE;
	nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR) {
		printf ("setsockopt() IP_MULTICAST_LOOP failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Assign our destination address */
	server_SocketInfo->DestAddr.sin_family =      AF_INET;
	server_SocketInfo->DestAddr.sin_addr.s_addr = inet_addr("234.5.6.7");
	server_SocketInfo->DestAddr.sin_port =        htons(port);

	return 0;

}

void run_server()
{
	int nRet2;
	DWORD Flags = 0;

	if((server_SocketInfo->Overlapped.hEvent = WSACreateEvent()) == WSA_INVALID_EVENT) {
		WSACleanup();
		return;
	}

	while(1){
		if (WSASendTo(server_SocketInfo->Socket, (LPWSABUF)(char *)&stSysTime, sizeof(stSysTime), NULL, Flags, (struct sockaddr*)&(server_SocketInfo->DestAddr), sizeof(server_SocketInfo->DestAddr) , &(server_SocketInfo->Overlapped), NULL) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				//error message
			}
		}
		WSAWaitForMultipleEvents(1, &(server_SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE);
		WSAResetEvent(server_SocketInfo->Overlapped.hEvent);

		Sleep(nInterval*1000);
	}
}
