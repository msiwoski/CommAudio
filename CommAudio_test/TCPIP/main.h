#ifndef MAIN_H
#define MAIN_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <cstdio>
//#include <ws2tcpip.h>

#define DATA_BUFSIZE		1024
#define PROTOCOL_TCP		0
#define PROCOTOL_UDP		1
#define FILENAMELENGTH 		32
#define TIMEOUT_TIME		3000
#define TIMEOUT_TIME_UDP	3000

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