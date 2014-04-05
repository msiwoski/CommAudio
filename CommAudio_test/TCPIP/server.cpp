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
HRECORD rec;
DWORD chan, rchan, reclen, p;
BOOL _recording;
char *recbuf=NULL;
short buf[1024];
int input;


/* testing the music file?!?!*/
DWORD outdev[2];	// output devices
DWORD latency[2];	// latencies
//HSTREAM chan;	// the streams
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
		chan = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_MONO);

		BASS_ChannelPlay(chan, FALSE);

		int nRet2;
		int q;
		DWORD Flags = 0;
		if (server_SocketInfo != NULL){
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
	}else
		BASS_ChannelPlay(chan, FALSE);
}

void stop(char *filename)
{
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelStop(chan); // stop the music
	}
}

void pause(char *filename){
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelPause(chan);	//pause the music
	}
}

void playPause(char *filename){
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelPlay(chan, FALSE); // play the stream (continue from current position)
	}
}

void record()
{
	if(!_recording)
	{
		InitDevice(-1);
		StartRecording();
	}else
		StopRecording();
}

BOOL InitDevice(int device)
{
	BASS_RecordFree(); // free current device (and recording channel) if there is one
	// initalize new device
	if (!BASS_RecordInit(device)) {
		//Error("Can't initialize recording device");
		return FALSE;
	}
	{ // get list of inputs
		int c;
		const char *i;
		for (c=0;i=BASS_RecordGetInputName(c);c++) {
			if (!(BASS_RecordGetInput(c,NULL)&BASS_INPUT_OFF)) { // this one is currently "on"
				input=c;
			}
		}
	}
	return TRUE;
}

// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	// increase buffer size if needed
	if ((reclen%BUFSTEP)+length>=BUFSTEP) {
		recbuf=(char*)realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
		if (!recbuf) {
			rchan=0;
			//Error("Out of memory!");
			//MESS(10,WM_SETTEXT,0,"Record");
			return FALSE; // stop recording
		}
	}
	// buffer the data
	memcpy(recbuf+reclen,buffer,length);
	reclen+=length;
	return TRUE; // continue recording
}

void StartRecording()
{
	WAVEFORMATEX *wf;

	_recording = true;

	if (recbuf) { // free old recording
		BASS_StreamFree(chan);
		chan=0;
		free(recbuf);
		recbuf=NULL;
		//EnableWindow(DLGITEM(11),FALSE);
		//EnableWindow(DLGITEM(12),FALSE);
		// close output device before recording incase of half-duplex device
		BASS_Free();
	}
	// allocate initial buffer and make space for WAVE header
	recbuf=(char*)malloc(BUFSTEP);
	reclen=44;
	// fill the WAVE header
	memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0",20);
	memcpy(recbuf+36,"data\0\0\0\0",8);
	wf=(WAVEFORMATEX*)(recbuf+20);
	wf->wFormatTag=1;
	wf->nChannels=CHANS;
	wf->wBitsPerSample=16;
	wf->nSamplesPerSec=FREQ;
	wf->nBlockAlign=wf->nChannels*wf->wBitsPerSample/8;
	wf->nAvgBytesPerSec=wf->nSamplesPerSec*wf->nBlockAlign;
	// start recording
	rchan=BASS_RecordStart(FREQ,CHANS,0,RecordingCallback,0);
	if (!rchan) {
		//Error("Couldn't start recording");
		free(recbuf);
		recbuf=0;
		return;
	}
	//MESS(10,WM_SETTEXT,0,"Stop");
}

void StopRecording()
{
	_recording = false;

	BASS_ChannelStop(rchan);
	rchan=0;
	//MESS(10,WM_SETTEXT,0,"Record");
	// complete the WAVE header
	*(DWORD*)(recbuf+4)=reclen-8;
	*(DWORD*)(recbuf+40)=reclen-44;
	// enable "save" button
	//EnableWindow(DLGITEM(12),TRUE);
	// setup output device (using default device)
	//if (!BASS_Init(-1,FREQ,0,win,NULL)) {
		//Error("Can't initialize output device");
		//return;
	//}
	// create a stream from the recording
	if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,0))
		//EnableWindow(DLGITEM(11),TRUE); // enable "play" button
		printf("sfd");
	else 
		BASS_Free();
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