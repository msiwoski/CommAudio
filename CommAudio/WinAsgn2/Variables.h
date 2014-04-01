#ifndef VARIABLES_H
#define VARIABLES_H

#include <winsock2.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <cstdio>
#include <bass.h>

#define DATA_BUFSIZE		1024
#define PROTOCOL_TCP		0
#define PROCOTOL_UDP		1
#define FILENAMELENGTH 		32
#define TIMEOUT_TIME		3000
#define TIMEOUT_TIME_UDP	3000

typedef struct _SOCKET_INFORMATION {
	OVERLAPPED Overlapped;
	SOCKET Socket;
	CHAR* Buffer;
	WSABUF DataBuf;
	DWORD BytesSEND;
	DWORD BytesRECV;
	int pktsRcvd;
	int numSending;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

typedef struct{
	int ctrlPktSize;
	int numTimesSending;
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
	int ctrlPktSizeGI;
	int numTimesSendingGI;
	char* fileData;
	int protocolType;
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
DWORD WINAPI UDPWorkerThread(LPVOID);
VOID CALLBACK UDPCtrlWorkerRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
VOID CALLBACK UDPWorkerRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
DWORD WINAPI UDPSenderThread(LPVOID);
long delay (SYSTEMTIME, SYSTEMTIME);

#endif