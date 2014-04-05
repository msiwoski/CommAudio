/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: MyForm.cpp
--
-- PROGRAM: WinAsgn2
--
-- FUNCTIONS:
-- DWORD WINAPI UDPWorkerThread(LPVOID)
-- VOID CALLBACK UDPWorkerRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD)
-- VOID UDPSend(int, char*, char*,  ControlPacket*)
-- DWORD WINAPI UDPSenderThread(LPVOID)
-- VOID CALLBACK UDPSenderRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD)
-- long delay(SYSTEMTIME, SYSTEMTIME)
--
-- DATE: February 03, 2014
--
-- REVISIONS:   
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- NOTES:
-- This Program acts as either a Server or a Client. 
-- When the program executes, the user is able to choose either client or server mode and select the port.
-- Receive Mode:
--		The Server can be placed into either TCP or UDP mode. When in Receive mode
-- Send Mode:
--		The Client can be placed into either TCP or UDP mode. The user is able to choose the packet size (multiples of 1 KB)
--		and the number of packets to send.  The user will be able to enter the ip address and the name of the server.
--
-- Remember to add "WS2_32.Lib" to the project source list.
--
----------------------------------------------------------------------------------------------------------------------*/

#include "MyForm.h"
#include "Variables.h"

InfoFile* fileInfo;
int openFlag	= 0;
SYSTEMTIME stStartTime, stEndTime;

/* ADDING THE BASS LIBRARY!!!! */
HSAMPLE *sams	= NULL;
HSAMPLE sam;
/* testing the music file?!?!*/
//HSTREAM *strs	=	NULL;
DWORD streamHandle;
int strc=0;
char streamDataBuffer[10000];

HWND hwnd;
#define MESS(id,m,w,l) SendDlgItemMessage(hwnd,id,m,(WPARAM)(w),(LPARAM)(l))
#define STLM(m,w,l) MESS(10,m,w,l)
#define MLM(m,w,l) MESS(20,m,w,l)
#define SLM(m,w,l) MESS(30,m,w,l)
#define GETSTR() STLM(LB_GETCURSEL,0,0)
#define GETMOD() MLM(LB_GETCURSEL,0,0)
#define GETSAM() SLM(LB_GETCURSEL,0,0)


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MyForm_Load
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::MyForm_Load(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function starts the WSAStartup when the program begins. 
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::MyForm_Load(System::Object^  sender, System::EventArgs^  e){
	InformationWSA* bInfo	= (InformationWSA*) malloc(sizeof(InformationWSA));
	String^ errMsg			= gcnew String("error");
	
	if ((bInfo->Ret = WSAStartup(0x0202,&(bInfo->wsaData))) != 0){
		errMsg = "WSAStartup failed. Error: " + bInfo->Ret;
		MessageBox::Show(errMsg);
		WSACleanup();
		return;
	}

	hwnd = static_cast<HWND>(Handle.ToPointer());
	if (!BASS_Init(-1,44100,0,hwnd,NULL)){
		errMsg = "Can't initialize device";
		MessageBox::Show(errMsg);
		return;
	}


	streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: clientButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::clientButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function handles the radio button being pressed and the Send/Receive Button from being enabled or not enabled.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::clientButton_Click(System::Object^  sender, System::EventArgs^  e){
	receiveButton->Enabled	= false;
	sendButton->Enabled		= true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: serverButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::serverButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function handles the radio button being pressed and the Send/Receive Button from being enabled or not enabled.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::serverButton_Click(System::Object^  sender, System::EventArgs^  e){
	sendButton->Enabled		= false;
	receiveButton->Enabled	= true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: openButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::openButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function opens the OpenFileDialog so that the user is able to select a file to send.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::openButton_Click(System::Object^  sender, System::EventArgs^  e) {
	String^ errMsg			= gcnew String("error");
	openFileDialog1->Filter = "Streamable files (wav/aif/mp3/mp2/mp1/ogg)|*.wav;*.aif;*.mp3;*.mp2;*.mp1;*.ogg|All files|*.*";
	ControlPacket* ctrlPkt	= (ControlPacket*) malloc(sizeof(ControlPacket));
	fileInfo = (InfoFile*) malloc(sizeof(InfoFile));
	openFlag = 1;
	if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK ){
		fileInfo->fileName = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(openFileDialog1->FileName + "\0");
		//fileInfo->pfile = fopen(fileInfo->fileName, "r");
		if (streamHandle = BASS_StreamCreateFile(FALSE,fileInfo->fileName,0,0,0)) {
			//strs=(HSTREAM*)realloc((void*)strs,strc*sizeof(*strs));
			//strs[strc-1]=str;
			//STLM(LB_ADDSTRING,0,strrchr(fileInfo->fileName,'\\')+1);
		} else{
			errMsg = "Can't open stream";
			MessageBox::Show(errMsg);
			return;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: saveButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::saveButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function opens the OpenFileDialog so that the user is able to select a file to send.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::saveButton_Click(System::Object^  sender, System::EventArgs^  e) {
	saveFileDialog1->Filter = "Text Files|*.txt|All Files|*.*";
	if(saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK){
		fileInfo->fileName = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(openFileDialog1->FileName + "\0");
		fileInfo->pfile = fopen(fileInfo->fileName, "w");
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::receiveButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function begins the server for the application. The function will set the values for the sockets, create a 
-- a socket for either TCP or UDP connections, bind the socket, listen on the socket, then create a thread for listening.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::receiveButton_Click(System::Object^  sender, System::EventArgs^  e){
	GoodInfo* gInfo			= (GoodInfo*) malloc(sizeof(GoodInfo));
	ControlPacket* ctrlPkt	= (ControlPacket*) malloc(sizeof(ControlPacket));
	PntlssInfo* pktPntlss	= (PntlssInfo*) malloc(sizeof(PntlssInfo));
	SOCKET_INFORMATION* SI	= (SOCKET_INFORMATION*) malloc(sizeof(SOCKET_INFORMATION)); 
	portNum					= (int) portNumberChooser->Value;
	SI->pktsRcvd			= 0;
	
	memset((char *)&(gInfo->InternetAddr), 0, sizeof(SOCKADDR_IN));
	memset((char *)&(gInfo->SenderAddr), 0, sizeof(SOCKADDR_IN));
	gInfo->InternetAddr.sin_family		= AF_INET;
	gInfo->InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	gInfo->InternetAddr.sin_port		= htons(portNum);
	gInfo->SenderAddr.sin_family		= AF_INET;
	gInfo->SenderAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	gInfo->SenderAddr.sin_port			= htons(portNum);


	if ((gInfo->ListenSocket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		errMsg = "Failed to get a socket for UDP. Error: " + WSAGetLastError();
		MessageBox::Show(errMsg);
		return;
	}
	

	if (bind(gInfo->ListenSocket, (PSOCKADDR) &(gInfo->InternetAddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR){
		errMsg = "bind() failed. Error: " + WSAGetLastError();
		MessageBox::Show(errMsg);
		closesocket(gInfo->ListenSocket);
		WSACleanup();
		return;
	}

	
	if ((gInfo->ThreadHandle = CreateThread(NULL, 0, UDPWorkerThread, (LPVOID)gInfo, 0, &(gInfo->ThreadID))) == NULL){
		errMsg = "CreateThread failed for UDP. Error: " + GetLastError();
		MessageBox::Show(errMsg);
		return;
	}
	
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPWorkerThread
--
-- DATE: February 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
--				LPVOID lpParameter: Void parameter passed by the thread. Information passed is the GoodInformation Struct
--
-- RETURNS: Return true upon success, False if there is an error.
--
-- NOTES:
-- This function is the thread for the UDP Receive portion of the server. It will set information on the socket, 
-- create an event and receive the control packet. The function will wait for multiple events and reset the event afterwards.
------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI UDPWorkerThread(LPVOID lpParameter){
	LPSOCKET_INFORMATION SocketInfo;
	DWORD Flags = 0, RecvBytes = 0;
	String^ errMsg = gcnew String("error");
	GoodInfo* gInfo = (GoodInfo*) lpParameter;
	memset((char *)&SocketInfo, 0, sizeof(LPSOCKET_INFORMATION));
	int SenderAddrSize = sizeof (gInfo->SenderAddr);

	if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR,sizeof(SOCKET_INFORMATION))) == NULL){
		errMsg = "GlobalAlloc() failed. Error: " + GetLastError();
		MessageBox::Show(errMsg);
		return FALSE;
	}

	SocketInfo->Socket		= gInfo->ListenSocket;
	ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));  
	//SocketInfo->Buffer = (char*) malloc(sizeof(ControlPacket));
	SocketInfo->Buffer = (char*) malloc(sizeof(2048));
	SocketInfo->BytesSEND	= 0;
	SocketInfo->BytesRECV	= 0;
	SocketInfo->DataBuf.len = sizeof(char) * 10000;
	SocketInfo->DataBuf.buf = SocketInfo->Buffer;
	SocketInfo->Overlapped.hEvent = WSACreateEvent();

	//if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(gInfo->SenderAddr), &SenderAddrSize,
	//	&(SocketInfo->Overlapped), UDPCtrlWorkerRoutine) == SOCKET_ERROR){
	if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(gInfo->SenderAddr), &SenderAddrSize,
		&(SocketInfo->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				errMsg = "WSARecvFrom() failed. Error: " + WSAGetLastError();
				MessageBox::Show(errMsg);
				return FALSE;
			}
	}
	if (WSAWaitForMultipleEvents(1, &(SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
		MessageBox::Show("WSA_TIMEOUT");
		return FALSE;
	}
	WSAResetEvent(SocketInfo->Overlapped.hEvent);

	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPCtrlWorkerRoutine
--
-- DATE: February 04, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: VOID CALLBACK UDPCtrlWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
--				DWORD Error: Error during the receive.
--				DWORD BytesTransferred: Number of bytes received
--				LPWSAOVERLAPPED Overlapped: The overlapped structure containing the socket information
-				DWORD InFlags: Flags for the worker routine
--
-- RETURNS: -
--
-- NOTES:
-- This function deals with receiving the control packet from the client. The control packet will contain the number
-- of packets being sent and the size of the packets. Upone receiving the control packet, WSARecvFrom is called again which
-- would receive the actual information the client would like to send.
------------------------------------------------------------------------------------------------------------------*/
VOID CALLBACK UDPCtrlWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags){
	/*DWORD RecvBytes = 0, Flags = 0;
	String^ errMsg = gcnew String("error");
	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;
	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);
	ControlPacket *pkt		= (ControlPacket*)SI->DataBuf.buf;
	SI->numSending = pkt->numTimesSending;
	SI->DataBuf.len = pkt->ctrlPktSize;
	SI->DataBuf.buf = (char*)malloc(pkt->ctrlPktSize);
	GetSystemTime(&stStartTime);
	if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR *) &SenderAddr, &SenderAddrSize, &(SI->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING ){
			errMsg = "WSARecvFrom() failed with error \n" + WSAGetLastError();
			MessageBox::Show(errMsg);
			return;
		}
	}
	if (WSAWaitForMultipleEvents(1, &(SI->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
		MessageBox::Show("WSA_TIMEOUT");
		return;
	}
	WSAResetEvent(SI->Overlapped.hEvent);*/
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPWorkerRoutine
--
-- DATE: February 04, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
--				DWORD Error: Error during the receive.
--				DWORD BytesTransferred: Number of bytes received
--				LPWSAOVERLAPPED Overlapped: The overlapped structure containing the socket information
-				DWORD InFlags: Flags for the worker routine
--
-- RETURNS: -
--
-- NOTES:
-- This function deals with receiving the packets from the client. When the amount of packets being sent has been reached
-- the program will stop receiving.
------------------------------------------------------------------------------------------------------------------*/
VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags){
	DWORD RecvBytes = 0, Flags = 0;
	String^ errMsg = gcnew String("error");
	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;
	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);
	
	if(BytesTransferred == 0 || Error != 0){
		MessageBox::Show("Connection has been closed.");
		return;
	}

	if (BytesTransferred > 0 ){
		BASS_StreamPutData(streamHandle, streamDataBuffer, RecvBytes);
	}
	SI->pktsRcvd++;
	
	if(SI->pktsRcvd >40){
		BASS_ChannelPlay(streamHandle, FALSE);
	}

	/*if (SI->pktsRcvd == SI->numSending){
		GetSystemTime(&stEndTime);
		errMsg = "Round-trip delay = " +  delay(stStartTime, stEndTime)  + "ms.";
		MessageBox::Show(errMsg);
		errMsg = "Total Number Received: " + SI->pktsRcvd + "Total Number Needed: " + SI->numSending;
		MessageBox::Show(errMsg);
		return;
	} else 
		*/
	if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR *) 
		&SenderAddr, &SenderAddrSize, &(SI->Overlapped), UDPWorkerRoutine) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING ){
			errMsg = "WSARecvFrom() failed with error \n" + WSAGetLastError();
			MessageBox::Show(errMsg);
			return;
		}
	}
	

	if (WSAWaitForMultipleEvents(1, &(SI->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
		errMsg = "Total Number Received: " + SI->pktsRcvd + "Total Number Needed: " + SI->numSending;
		MessageBox::Show(errMsg);
		return;
	}
	WSAResetEvent(SI->Overlapped.hEvent);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendButton_Click
--
-- DATE: February 01, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: System::Void MyForm::sendButton_Click(System::Object^  sender, System::EventArgs^  e)
--				Object^  sender: Object sender that raised the event.
--				EventArgs^  e: Event argument of the object.
--
-- RETURNS: -
--
-- NOTES:
-- This function begins the client for the application. The function will get the information about about the server (port &
-- ip address or name) set the values for the sockets, create an event on the socket, create a a socket for either 
-- TCP or UDP connections, connect on the socket, send out a control packet containing information on the number of packets
-- and the size of the packets and create threads to handle the communication.
------------------------------------------------------------------------------------------------------------------*/
System::Void MyForm::sendButton_Click(System::Object^  sender, System::EventArgs^  e){
	WSABUF DataBuf;
	char* result				= "result";
	char* host					= "host";
	DWORD Flags					= 0;
	GoodInfo* gInfo				= (GoodInfo*) malloc(sizeof(GoodInfo));
	portNum						= (int) portNumberChooser->Value; 
	//ControlPacket* ctrlPkt		= (ControlPacket*) malloc(sizeof(ControlPacket));
	//ctrlPkt->ctrlPktSize		= (int) pktSizeChoice->Value;
	//ctrlPkt->numTimesSending	= (int) sendTimesChoice->Value;
	//gInfo->protocolType			= (int) protocolTypeChooser->SelectedIndex;
	//ctrlPkt->ctrlPktSize		= 1024;
	//gInfo->ctrlPktSizeGI		= ctrlPkt->ctrlPktSize;
	//gInfo->numTimesSendingGI	= 1;
	//DataBuf.buf					= (char*)ctrlPkt;
	//DataBuf.len					= sizeof(ControlPacket);
	//gInfo->fileData				= (char*) malloc(ctrlPkt->ctrlPktSize);
	/*for(int i = 0; i < FILENAMELENGTH; i++){
		ctrlPkt->fileName[i] = 0;
	}
	if (fileInfo->fileName != NULL){
		strcpy(ctrlPkt->fileName, fileInfo->fileName);
	}*/

	/*if(openFlag == 1){
		for(int i = 0; i < gInfo->ctrlPktSizeGI; i++){
			fscanf(fileInfo->pfile, "%c", &(gInfo->fileData[i]));
		}
	} else {
		for(int i = 0; i < gInfo->ctrlPktSizeGI; i++){
			gInfo->fileData[i] = 'a';
		}
	}*/
	
	if ((ipAddress = (String^) ipAddressTextBox->Text) != ""){
		result = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ipAddress);
	} else {
		errMsg = "No IP or Host";
		MessageBox::Show(errMsg);
		return;
	}

	gInfo->SenderAddr.sin_family = AF_INET;
	gInfo->SenderAddr.sin_port = htons(portNum);
	gInfo->SenderAddr.sin_addr.s_addr  = htonl(INADDR_ANY);
	gInfo->InternetAddr.sin_port = htons(portNum);
	gInfo->InternetAddr.sin_family = AF_INET;

	if ((gInfo->InternetAddr.sin_addr.s_addr = inet_addr(result)) == INADDR_NONE){
		errMsg =  "Unable to get IP Address. Error: " + GetLastError();
		MessageBox::Show(errMsg);
		return;
	}

	if((gInfo->SendOverlapped.hEvent = WSACreateEvent()) == WSA_INVALID_EVENT) {
		errMsg = "WSACreateEvent failed. Error: " + WSAGetLastError();
		MessageBox::Show(errMsg);
		WSACleanup();
		return;
	}
	if ((gInfo->SenderSocket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		errMsg = "WSASocket failed. Cannot create socket. Error: " + WSAGetLastError();
		MessageBox::Show(errMsg);
		WSACloseEvent(gInfo->SendOverlapped.hEvent);
		WSACleanup();
		return;
	}
	/*if(sendto(gInfo->SenderSocket, DataBuf.buf, DataBuf.len, Flags, (SOCKADDR*) &(gInfo->InternetAddr), sizeof(gInfo->InternetAddr)) == SOCKET_ERROR){
		errMsg = "Sending Control Packet failed. Error: " + GetLastError();
		MessageBox::Show(errMsg);
		return ;
	}*/
	if ((gInfo->ThreadHandle = CreateThread(NULL, 0, UDPSenderThread, (LPVOID)gInfo, 0, &(gInfo->ThreadID))) == NULL){
		errMsg = "CreateThread failed. Error: " + GetLastError();
		MessageBox::Show(errMsg);
		return;
	}
	
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPSenderThread
--
-- DATE: February 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: DWORD WINAPI UDPSenderThread(LPVOID lpParameter)
--				LPVOID lpParameter: Void parameter passed by the thread. Information passed is the GoodInformation Struct
--
-- RETURNS: Return true upon success, False if there is an error.
--
-- NOTES:
-- This function is the thread for the UDP Send portion of the client. It will send the number of packets and the size of the packets
-- as selected by the user. The function will wait for multiple events and reset the event afterwards.
------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI UDPSenderThread(LPVOID lpParameter){
	String^ errMsg		= gcnew String("error");
	GoodInfo* gInfo		= (GoodInfo*) lpParameter;
	DWORD Flags			= 0;
	WSABUF DataBuf;
	DWORD sendBytes = 0;
	DWORD bytesSent = 0;
	//DataBuf.buf = gInfo->fileData;
	//DataBuf.len = gInfo->ctrlPktSizeGI;


	char streamDataBuffer[2048];
	HSTREAM streamBuffer = BASS_StreamCreateFile(FALSE, fileInfo->fileName, 0, 0, BASS_STREAM_DECODE);

	while(BASS_ChannelIsActive(streamBuffer) == BASS_ACTIVE_PLAYING ) {
		DWORD readLength =	BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);
		DataBuf.buf = (CHAR*)streamDataBuffer;
		DataBuf.len = readLength;

		if (WSASendTo(gInfo->SenderSocket, &DataBuf, 1, &sendBytes, Flags, (SOCKADDR*) &(gInfo->InternetAddr), sizeof(SOCKADDR), &(gInfo->SendOverlapped), NULL) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				errMsg = "WSASendTo() failed with error \n" + WSAGetLastError();
				MessageBox::Show(errMsg);
				return -1;
			}
		}
		WSAWaitForMultipleEvents(1, &(gInfo->SendOverlapped.hEvent), TRUE, WSA_INFINITE, TRUE);
		WSAGetOverlappedResult(gInfo->SenderSocket, &(gInfo->SendOverlapped), &bytesSent, FALSE, &Flags);
		WSAResetEvent(gInfo->SendOverlapped.hEvent);

		bytesSent += sendBytes;
	}
	return 1;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: delay
--
-- DATE: February 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Mat Siwoski
--
-- INTERFACE: long delay (SYSTEMTIME t1, SYSTEMTIME t2)
--				SYSTEMTIME t1, t2: Systemtime of the cpu
--
-- RETURNS: Return the difference in times as a long
--
-- NOTES:
-- This function is used to check the delay between start time and send time of receiving a packet.
------------------------------------------------------------------------------------------------------------------*/
long delay (SYSTEMTIME t1, SYSTEMTIME t2)
{
	long d;

	d = (t2.wSecond - t1.wSecond) * 1000;
	d += (t2.wMilliseconds - t1.wMilliseconds);
	return(d);
}

System::Void MyForm::openSong_Click(System::Object^  sender, System::EventArgs^  e){
	/*String^ errMsg		= gcnew String("error");
	int strc = 0;
	//HSTREAM str;
	if (streamHandle=BASS_StreamCreateFile(FALSE,fileInfo->fileName,0,0,0)) {
		strc++;
		//strs=(HSTREAM*)realloc((void*)strs,strc*sizeof(*strs));
		//strs[strc-1]=str;
		STLM(LB_ADDSTRING,0,strrchr(fileInfo->fileName,'\\')+1);
	} else {
		errMsg = "Can't open stream";
		MessageBox::Show(errMsg);
	}*/
}

System::Void MyForm::playButton_Click(System::Object^  sender, System::EventArgs^  e){
	if (!BASS_ChannelPlay(streamHandle,FALSE)){ // play the stream (continue from current position)
		errMsg = "Can't play song!";
		MessageBox::Show(errMsg);
	}		

}

System::Void MyForm::stopButton_Click(System::Object^  sender, System::EventArgs^  e){
	String^ errMsg		= gcnew String("error");
	errMsg = "Can't Create Stream File";
	
	BASS_ChannelStop(streamHandle); // stop the music
	
}

System::Void MyForm::pauseButton_Click(System::Object^  sender, System::EventArgs^  e){
	String^ errMsg		= gcnew String("error");
	errMsg	= "Can't Create Stream File";
	BASS_ChannelPause(streamHandle);	//pause the music
}