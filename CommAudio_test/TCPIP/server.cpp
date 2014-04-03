#include "server_menu.h"
/* handle to form */
HWND ghwnd;
LPSOCKET_INFORMATION server_SocketInfo;

u_long  lTTL               = TIMECAST_TTL;
u_short nInterval          = TIMECAST_INTRVL;
SYSTEMTIME stSysTime;


/* testing the music file?!?!*/
DWORD outdev[2];	// output devices
DWORD latency[2];	// latencies
HSTREAM chan[2];	// the streams
	int stream;
FILE *fp;

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

void play(char *filename)
{


	if (filename!=NULL)
	{
		stream = BASS_StreamCreateFile(FALSE,filename,0,0,0);
		/*{
			strc++;
			strs=(HSTREAM*)realloc((void*)strs,strc*sizeof(*strs));
			strs[strc-1]=str;
			//errMsg = "Can't open stream";
			//MessageBox::Show(errMsg);
		}*/


		BASS_ChannelSetDSP(stream,&DSP,0,0);

		BASS_ChannelPlay(stream, TRUE); // play the stream (continue from current position)
			//errMsg = "Can't open stream";
			//MessageBox::Show(errMsg);
		//}
	}
}

void stop(char *filename)
{
	if (filename!=NULL && stream != NULL)
	{
		BASS_ChannelStop(stream); // stop the music
	}
}

void pause(char *filename){
	if (filename!=NULL && stream != NULL)
	{
		BASS_ChannelPause(stream);	//pause the music
	}
}

void playPause(char *filename){
	BASS_ChannelPlay(stream, FALSE); // play the stream (continue from current position)
}


void CALLBACK DSP(HDSP handle, DWORD channel, void *buffer, DWORD length, void *user)
{
	short *s=(short *)buffer;
	fp = fopen("log.mp3", "ab");
    for (; length; length-=4, s+=2) {
        short temp=s[0];
        s[0]=s[1];
        s[1]=temp;
		//s[0]*=2;
		//fp = fopen("log.mp3", "ab");
		//fwrite(&s, 2, sizeof(s), fp);
		//fclose(fp);
    }
	fclose(fp);

	/*short *s = (short *)buffer;

	for (; length; length-=4, s+=2)
	{
		fp = fopen("log.mp3", "ab");
		fwrite(&s[0], 1, sizeof(s[0]), fp);
		fclose(fp);
	}*/
	//BASS_StreamPutData((HSTREAM)user,buffer,length);

}


/*
SERVER==================================================
BASS_RecordInit(-1); // initialize default recording device
record=BASS_RecordStart(freq, chans, 0, RecordProc, 0); // start recording
BASS_Encode_Start(record, "lame -", 0, EncodeProc, 0); // set an MP3 encoder (LAME) on it

...

// the recording callback
BOOL CALLBACK RecordProc(HRECORD handle, void *buffer, DWORD length, void *user)
{
    return TRUE; // just continue recording
}

// the encoding callback
void CALLBACK EncodeProc(HENCODE handle, DWORD channel, void *buffer, DWORD length, void *user)
{
	// send "length" bytes of data in "buffer" to the client
}


CLIENT====================================
void CALLBACK FileCloseProc(void *user)
{
	// close the connection
}

QWORD CALLBACK FileLenProc(void *user)
{
	return 0; // indeterminate length
}

DWORD CALLBACK FileReadProc(void *buffer, DWORD length, void *user)
{
	// read up to "length" bytes of data from the server
}

...

BASS_FILEPROCS fileprocs={FileCloseProc, FileLenProc, FileReadProc, NULL}; // callback table
stream=BASS_StreamCreateFileUser(STREAMFILE_BUFFER, 0, &fileprocs, 0); // create the stream
BASS_ChannelPlay(stream, 0); // start playing
*/