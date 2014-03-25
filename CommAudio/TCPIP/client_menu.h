#pragma once

#ifndef _CLIENTMENU_
#define _CLIENTMENU_

#include "main.h"
#include <stdio.h>
#include <fstream> 
#include <string.h>
#include "atlstr.h"

#define DATA_BUFSIZE 70000

typedef struct _SOCKET_INFORMATION
{
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	WSAOVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

DWORD WINAPI UDPWorkerThread(LPVOID lpParameter);
VOID CALLBACK UDPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port);
void run_client();

namespace TCPIP 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for client_menu
	/// </summary>
	public ref class client_menu : public System::Windows::Forms::Form
	{
	public:
		client_menu(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~client_menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ConnectButton;


	private: System::Windows::Forms::Label^  IPLabel;
	private: System::Windows::Forms::Label^  PortLabel;

	private: System::Windows::Forms::TextBox^  IPTextBox;
	private: System::Windows::Forms::TextBox^  PortTextBox;

	





	private: System::Windows::Forms::ListBox^  outputListBox;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;


	private: System::Windows::Forms::Button^  DisconnectButton;
	private: System::Windows::Forms::Button^  ClearButton;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  SaveAsLabel;
	private: System::Windows::Forms::TextBox^  SaveAsTextBox;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ConnectButton = (gcnew System::Windows::Forms::Button());
			this->IPLabel = (gcnew System::Windows::Forms::Label());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->IPTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PortTextBox = (gcnew System::Windows::Forms::TextBox());
			this->outputListBox = (gcnew System::Windows::Forms::ListBox());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->DisconnectButton = (gcnew System::Windows::Forms::Button());
			this->ClearButton = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SaveAsLabel = (gcnew System::Windows::Forms::Label());
			this->SaveAsTextBox = (gcnew System::Windows::Forms::TextBox());
			this->backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// ConnectButton
			// 
			this->ConnectButton->Location = System::Drawing::Point(64, 71);
			this->ConnectButton->Name = L"ConnectButton";
			this->ConnectButton->Size = System::Drawing::Size(75, 23);
			this->ConnectButton->TabIndex = 0;
			this->ConnectButton->Text = L"Connect";
			this->ConnectButton->UseVisualStyleBackColor = true;
			this->ConnectButton->Click += gcnew System::EventHandler(this, &client_menu::ConnectButton_Click);
			// 
			// IPLabel
			// 
			this->IPLabel->AutoSize = true;
			this->IPLabel->Location = System::Drawing::Point(38, 22);
			this->IPLabel->Name = L"IPLabel";
			this->IPLabel->Size = System::Drawing::Size(20, 13);
			this->IPLabel->TabIndex = 2;
			this->IPLabel->Text = L"IP:";
			// 
			// PortLabel
			// 
			this->PortLabel->AutoSize = true;
			this->PortLabel->Location = System::Drawing::Point(32, 48);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(29, 13);
			this->PortLabel->TabIndex = 3;
			this->PortLabel->Text = L"Port:";
			// 
			// IPTextBox
			// 
			this->IPTextBox->Location = System::Drawing::Point(64, 19);
			this->IPTextBox->Name = L"IPTextBox";
			this->IPTextBox->Size = System::Drawing::Size(100, 20);
			this->IPTextBox->TabIndex = 4;
			// 
			// PortTextBox
			// 
			this->PortTextBox->Location = System::Drawing::Point(64, 45);
			this->PortTextBox->Name = L"PortTextBox";
			this->PortTextBox->Size = System::Drawing::Size(100, 20);
			this->PortTextBox->TabIndex = 5;
			// 
			// outputListBox
			// 
			this->outputListBox->FormattingEnabled = true;
			this->outputListBox->Location = System::Drawing::Point(170, 19);
			this->outputListBox->Name = L"outputListBox";
			this->outputListBox->Size = System::Drawing::Size(211, 43);
			this->outputListBox->TabIndex = 11;
			// 
			// DisconnectButton
			// 
			this->DisconnectButton->Location = System::Drawing::Point(306, 147);
			this->DisconnectButton->Name = L"DisconnectButton";
			this->DisconnectButton->Size = System::Drawing::Size(75, 23);
			this->DisconnectButton->TabIndex = 14;
			this->DisconnectButton->Text = L"Disconnect";
			this->DisconnectButton->UseVisualStyleBackColor = true;
			this->DisconnectButton->Click += gcnew System::EventHandler(this, &client_menu::DisconnectButton_Click);
			// 
			// ClearButton
			// 
			this->ClearButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->ClearButton->Location = System::Drawing::Point(170, 74);
			this->ClearButton->Name = L"ClearButton";
			this->ClearButton->Size = System::Drawing::Size(211, 20);
			this->ClearButton->TabIndex = 15;
			this->ClearButton->Text = L"clear";
			this->ClearButton->UseVisualStyleBackColor = true;
			this->ClearButton->Click += gcnew System::EventHandler(this, &client_menu::ClearButton_Click);
			// 
			// button1
			// 
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button1->Location = System::Drawing::Point(360, 121);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(21, 20);
			this->button1->TabIndex = 18;
			this->button1->Text = L"..";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// SaveAsLabel
			// 
			this->SaveAsLabel->AutoSize = true;
			this->SaveAsLabel->Location = System::Drawing::Point(9, 124);
			this->SaveAsLabel->Name = L"SaveAsLabel";
			this->SaveAsLabel->Size = System::Drawing::Size(49, 13);
			this->SaveAsLabel->TabIndex = 17;
			this->SaveAsLabel->Text = L"Save as:";
			// 
			// SaveAsTextBox
			// 
			this->SaveAsTextBox->Location = System::Drawing::Point(64, 121);
			this->SaveAsTextBox->Name = L"SaveAsTextBox";
			this->SaveAsTextBox->Size = System::Drawing::Size(317, 20);
			this->SaveAsTextBox->TabIndex = 16;
			// 
			// backgroundWorker
			// 
			this->backgroundWorker->WorkerSupportsCancellation = true;
			this->backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &client_menu::backgroundWorker_DoWork);
			// 
			// client_menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(398, 185);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->SaveAsLabel);
			this->Controls->Add(this->SaveAsTextBox);
			this->Controls->Add(this->ClearButton);
			this->Controls->Add(this->DisconnectButton);
			this->Controls->Add(this->outputListBox);
			this->Controls->Add(this->PortTextBox);
			this->Controls->Add(this->IPTextBox);
			this->Controls->Add(this->PortLabel);
			this->Controls->Add(this->IPLabel);
			this->Controls->Add(this->ConnectButton);
			this->Name = L"client_menu";
			this->Text = L"client_menu";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &client_menu::client_menu_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FileDirButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";

				 saveFileDialog1->FilterIndex = 2;
				 saveFileDialog1->RestoreDirectory = true;
				 saveFileDialog1->ShowDialog();

				 SaveAsTextBox->Text = saveFileDialog1->FileName;
			 }
	private: System::Void ConnectButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 backgroundWorker->RunWorkerAsync();
			 }

	private: System::Void DisconnectButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->backgroundWorker->CancelAsync();
				 //multicast_disconnect();
			 }
	private: System::Void ClearButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }


	private: System::Void client_menu_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {
				 exit(1);
			 }
	private: System::Void backgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 int port = 0;
				 char ip[32]= "";

				 pin_ptr<const wchar_t> tempIP;

				 tempIP = PtrToStringChars(this->IPTextBox->Text);
				 if(PortTextBox->Text != "")
					 port = System::Int32::Parse(PortTextBox->Text);

				 wcstombs_s(0, ip, wcslen(tempIP) + 1, tempIP, _TRUNCATE);

				 multicast_connect(outputListBox, ip, port);
				 run_client();
			 }
};
}

#endif
