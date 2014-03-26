#ifndef MAIN_H
#define MAIN_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <cstdio>

#define DATA_BUFSIZE		1024
#define PROTOCOL_TCP		0
#define PROCOTOL_UDP		1
#define FILENAMELENGTH 		32
#define TIMEOUT_TIME		3000
#define TIMEOUT_TIME_UDP	3000
#define TIMECAST_TTL		2
#define TIMECAST_INTRVL		7

typedef struct{
	char fileName[FILENAMELENGTH];
} ControlPacket;

typedef struct {
	WSAEVENT AcceptEvent;
} PntlssInfo;

typedef struct {
	SOCKADDR_IN InternetAddr;
	SOCKADDR_IN SenderAddr;
	SOCKET AcceptSocket;
	SOCKET SenderSocket;
	SOCKET ListenSocket;
	HANDLE ThreadHandle;
	DWORD ThreadID;
	WSAEVENT AcceptEvent;
	WSAOVERLAPPED SendOverlapped;
	char* fileData;
} GoodInfo;

typedef struct _SOCKET_INFORMATION
{
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	WSAOVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
	SOCKADDR_IN SenderAddr;
	SOCKADDR_IN DestAddr;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

typedef struct {
	WSADATA wsaData;	
	int Ret;
} InformationWSA;

typedef struct {
	char* fileName;
	FILE* pfile;
} InfoFile;

/* 
*	FUNCTION PROTOTYPES
*/


#endif