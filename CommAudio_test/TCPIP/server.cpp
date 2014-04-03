#include "server_menu.h"

#include <conio.h>



#ifdef _BIG_ENDIAN
inline DWORD le_32(DWORD v)
{
	return (v>>24)|((v>>8)&0xff00)|((v&0xff00)<<8)|(v<<24);
}
inline WORD le_16(WORD v)
{
	return (v>>8)|(v<<8);
}
#else
#define le_32(v) (v)
#define le_16(v) (v)
#endif




/* handle to form */
HWND ghwnd;
LPSOCKET_INFORMATION server_SocketInfo;

u_long  lTTL               = TIMECAST_TTL;
u_short nInterval          = TIMECAST_INTRVL;
SYSTEMTIME stSysTime;


BASS_CHANNELINFO info;
WAVEFORMATEX wf;
DWORD chan,p;
short buf[1024];

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
	//BASS_CHANNELINFO info;
	//WAVEFORMATEX wf;
	//DWORD chan,p;
	//short buf[1000];

	if (filename!=NULL)
	{
		chan = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_MONO);

		//BASS_ChannelSetDSP(chan,&DSP,0,0);
		BASS_ChannelPlay(chan, FALSE);

		/*fp = fopen("log.WAV", "wb");

		BASS_ChannelGetInfo(chan,&info);
		wf.wFormatTag=1;
		wf.nChannels=info.chans;
		wf.wBitsPerSample=(info.flags&BASS_SAMPLE_8BITS?8:16);
		wf.nBlockAlign=wf.nChannels*wf.wBitsPerSample/8;
		wf.nSamplesPerSec=info.freq;
		wf.nAvgBytesPerSec=wf.nSamplesPerSec*wf.nBlockAlign;

		fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0",20,1,fp);
		fwrite(&wf,16,1,fp);
		fwrite("data\0\0\0\0",8,1,fp);

		while(1)
		{
			while (!_kbhit() && (BASS_ChannelIsActive(chan) == BASS_ACTIVE_PLAYING)) 
			{
			int c=BASS_ChannelGetData(chan,buf,1024);
			fwrite(buf,1,c,fp);
			}
		}

		fflush(fp);
		p=ftell(fp);
		fseek(fp,4,SEEK_SET);
		putw(le_32(p-8),fp);
		fflush(fp);
		fseek(fp,40,SEEK_SET);
		putw(le_32(p-44),fp);
		fflush(fp);
		fclose(fp);*/


		
		int nRet2;
		int q;
		DWORD Flags = 0;

		if((server_SocketInfo->Overlapped.hEvent = WSACreateEvent()) == WSA_INVALID_EVENT) {
			WSACleanup();
			return;
		}

		BASS_ChannelGetInfo(chan,&info);
		wf.wFormatTag=1;
		wf.nChannels=info.chans;
		wf.wBitsPerSample=(info.flags&BASS_SAMPLE_8BITS?8:16);
		wf.nBlockAlign=wf.nChannels*wf.wBitsPerSample/8;
		wf.nSamplesPerSec=info.freq;
		wf.nAvgBytesPerSec=wf.nSamplesPerSec*wf.nBlockAlign;

		memset(server_SocketInfo->Buffer, 0, sizeof(server_SocketInfo->Buffer));

		strcpy(server_SocketInfo->Buffer, "RIFF\0\0\0\0WAVEfmt \20\0\0\0");
		strcat(server_SocketInfo->Buffer, (char *)&wf);
		strcat(server_SocketInfo->Buffer, "data\0\0\0\0");

		server_SocketInfo->DataBuf.buf = server_SocketInfo->Buffer;
		server_SocketInfo->DataBuf.len = sizeof(server_SocketInfo->Buffer);

		if (WSASendTo(server_SocketInfo->Socket, (LPWSABUF)&server_SocketInfo->DataBuf, 1, NULL, Flags, (struct sockaddr*)&(server_SocketInfo->DestAddr), sizeof(server_SocketInfo->DestAddr) , &(server_SocketInfo->Overlapped), NULL) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				//error message
				q = WSAGetLastError();
				q = 0;
			}
		}
		WSAWaitForMultipleEvents(1, &(server_SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE);
		WSAResetEvent(server_SocketInfo->Overlapped.hEvent);

		while(1)
		{
			while (!_kbhit() && (BASS_ChannelIsActive(chan) == BASS_ACTIVE_PLAYING)) 
			{
				memset(server_SocketInfo->Buffer, 0, sizeof(server_SocketInfo->Buffer));
				int c = BASS_ChannelGetData(chan,buf,1024);
				strcpy(server_SocketInfo->Buffer, (char *)&buf);

				server_SocketInfo->DataBuf.buf = server_SocketInfo->Buffer;
				server_SocketInfo->DataBuf.len = c;

				if (WSASendTo(server_SocketInfo->Socket, (LPWSABUF)&server_SocketInfo->DataBuf, 1, NULL, Flags, (struct sockaddr*)&(server_SocketInfo->DestAddr), sizeof(server_SocketInfo->DestAddr) , &(server_SocketInfo->Overlapped), NULL) == SOCKET_ERROR){
					if (WSAGetLastError() != WSA_IO_PENDING){
						//error message
						q = WSAGetLastError();
						q = 0;
					}
				}
				WSAWaitForMultipleEvents(1, &(server_SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE);
				WSAResetEvent(server_SocketInfo->Overlapped.hEvent);
			}
		}
	}
}

void CALLBACK DSP(HDSP handle, DWORD channel, void *buffer, DWORD length, void *user)
{
	

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