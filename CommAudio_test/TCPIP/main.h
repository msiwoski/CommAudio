#ifndef _MAIN_
#define _MAIN_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <cstdio>
#include <string>
#include <fstream> 
#include <bass.h>
#include "atlstr.h"

#define DATA_BUFSIZE		1024
#define FREQ				44100
#define CHANS				2
#define BUFSTEP				200000	// memory allocation unit
#define FILENAMELENGTH 		32
#define TIMEOUT_TIME		3000
#define TIMEOUT_TIME_UDP	3000
#define TIMECAST_TTL		2
#define TIMECAST_INTRVL		7


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

/*typedef struct {
	char* fileName;
	FILE* pfile;
} InfoFile;*/


/* 
*	FUNCTION PROTOTYPES
*/
void run_server();
int init_server(int port);
void record();
void play(char *filename);
void stop(char *filename);
void pause(char *filename);
void playPause(char *filename);
BOOL InitDevice(int device);
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user);
void StartRecording();
void StopRecording();


DWORD WINAPI UDPWorkerThread(LPVOID lpParameter);
VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port);
void run_client(System::Windows::Forms::ListBox ^lb);

#endif