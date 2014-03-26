#pragma once

#ifndef _SERVERMENU_
#define _SERVERMENU_

#include "main.h"
#include "atlstr.h"
#include <string>

#define FILE_SIZE 255

#define PACKETSRECV_LISTBOX WM_APP+1
#define BYTESRECV_LISTBOX   WM_APP+2
#define STRING_LISTBOX      WM_APP+3

void run_server();
int init_server(int port);

namespace TCPIP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Security::Permissions;

	/// <summary>
	/// Summary for server_menu
	/// </summary>
	public ref class server_menu : public System::Windows::Forms::Form
	{
		HWND hwnd;
	public:
		server_menu(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		virtual void WndProc(Message% m) override
		{
			if(!(WindowProc((HWND)m.HWnd.ToPointer(), m.Msg, (WPARAM)m.WParam.ToPointer(), (LPARAM)m.LParam.ToPointer())))
				Form::WndProc(m);
		}
		BOOL WindowProc(HWND hwnd, int uMsg, WPARAM wParam, LPARAM lParam)
		{
			if(uMsg == PACKETSRECV_LISTBOX)
			{
				if(wParam != NULL)
				{
					int buf = (int)wParam;
					Object ^Obj = buf; 
					outputListBox->Items->Add("Packets received: " + Obj);
				}
				return 1;
			}
			if(uMsg == BYTESRECV_LISTBOX)
			{
				if(wParam != NULL)
				{
					int buf = (int)wParam;
					Object ^Obj = buf; 
					outputListBox->Items->Add("Bytes received: " + Obj);
				}
				return 1;
			}
			if(uMsg == STRING_LISTBOX)
			{
				if(wParam != NULL)
				{
					char *buf = (char*)wParam;
					String^ s = gcnew String(buf);
					Object ^Obj = s; 
					outputListBox->Items->Add(s);
				}
				return 1;
			}
			return 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~server_menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ListenButton;
	protected: 

	protected: 

	private: System::Windows::Forms::TextBox^  PortTextBox;
	private: System::Windows::Forms::Label^  PortLabel;
	private: Boolean appActive;
	private: System::Windows::Forms::ListBox^  outputListBox;





	private: System::Windows::Forms::Button^  CloseButton;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;


	private: System::ComponentModel::BackgroundWorker^  backgroundWorker;
	private: System::ComponentModel::BackgroundWorker^  UDPbackgroundWorker;
	private: System::Windows::Forms::Label^  FileLabel;
	private: System::Windows::Forms::TextBox^  FileTextBox;
	private: System::Windows::Forms::Button^  SendButton;




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
			this->ListenButton = (gcnew System::Windows::Forms::Button());
			this->PortTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->outputListBox = (gcnew System::Windows::Forms::ListBox());
			this->CloseButton = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->UDPbackgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->FileLabel = (gcnew System::Windows::Forms::Label());
			this->FileTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SendButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// ListenButton
			// 
			this->ListenButton->Location = System::Drawing::Point(54, 56);
			this->ListenButton->Name = L"ListenButton";
			this->ListenButton->Size = System::Drawing::Size(75, 23);
			this->ListenButton->TabIndex = 0;
			this->ListenButton->Text = L"Stream";
			this->ListenButton->UseVisualStyleBackColor = true;
			this->ListenButton->Click += gcnew System::EventHandler(this, &server_menu::ListenButton_Click);
			// 
			// PortTextBox
			// 
			this->PortTextBox->Location = System::Drawing::Point(54, 29);
			this->PortTextBox->Name = L"PortTextBox";
			this->PortTextBox->Size = System::Drawing::Size(237, 20);
			this->PortTextBox->TabIndex = 1;
			// 
			// PortLabel
			// 
			this->PortLabel->AutoSize = true;
			this->PortLabel->Location = System::Drawing::Point(19, 32);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(29, 13);
			this->PortLabel->TabIndex = 2;
			this->PortLabel->Text = L"Port:";
			// 
			// outputListBox
			// 
			this->outputListBox->FormattingEnabled = true;
			this->outputListBox->Location = System::Drawing::Point(54, 117);
			this->outputListBox->Name = L"outputListBox";
			this->outputListBox->Size = System::Drawing::Size(237, 186);
			this->outputListBox->TabIndex = 3;
			// 
			// CloseButton
			// 
			this->CloseButton->Enabled = false;
			this->CloseButton->Location = System::Drawing::Point(216, 56);
			this->CloseButton->Name = L"CloseButton";
			this->CloseButton->Size = System::Drawing::Size(75, 23);
			this->CloseButton->TabIndex = 8;
			this->CloseButton->Text = L"Disconnect";
			this->CloseButton->UseVisualStyleBackColor = true;
			this->CloseButton->Click += gcnew System::EventHandler(this, &server_menu::CloseButton_Click);
			// 
			// backgroundWorker
			// 
			this->backgroundWorker->WorkerSupportsCancellation = true;
			this->backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &server_menu::backgroundWorker_DoWork);
			// 
			// 
			// FileLabel
			// 
			this->FileLabel->AutoSize = true;
			this->FileLabel->Location = System::Drawing::Point(22, 321);
			this->FileLabel->Name = L"FileLabel";
			this->FileLabel->Size = System::Drawing::Size(26, 13);
			this->FileLabel->TabIndex = 13;
			this->FileLabel->Text = L"File:";
			// 
			// FileTextBox
			// 
			this->FileTextBox->Location = System::Drawing::Point(54, 318);
			this->FileTextBox->Name = L"FileTextBox";
			this->FileTextBox->Size = System::Drawing::Size(237, 20);
			this->FileTextBox->TabIndex = 12;
			// 
			// SendButton
			// 
			this->SendButton->Location = System::Drawing::Point(54, 344);
			this->SendButton->Name = L"SendButton";
			this->SendButton->Size = System::Drawing::Size(75, 23);
			this->SendButton->TabIndex = 11;
			this->SendButton->Text = L"Send";
			this->SendButton->UseVisualStyleBackColor = true;
			// 
			// server_menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(319, 382);
			this->Controls->Add(this->FileLabel);
			this->Controls->Add(this->FileTextBox);
			this->Controls->Add(this->SendButton);
			this->Controls->Add(this->CloseButton);
			this->Controls->Add(this->outputListBox);
			this->Controls->Add(this->PortLabel);
			this->Controls->Add(this->PortTextBox);
			this->Controls->Add(this->ListenButton);
			this->Name = L"server_menu";
			this->Text = L"server_menu";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &server_menu::server_menu_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FileDirButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 openFileDialog1->ShowDialog();
				 FileTextBox->Text = openFileDialog1->FileName;
			 }
	private: System::Void ListenButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 char fileName[128] = "";

				 this->PortTextBox->Enabled = false;
				 this->ListenButton->Enabled = false;
				 this->CloseButton->Enabled = true;

				 hwnd = static_cast<HWND>(Handle.ToPointer());

				 backgroundWorker->RunWorkerAsync();
			 }
	private: System::Void ClearButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }
	private: System::Void backgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 int port = 0;
				 char fileName[128] = "";

				 pin_ptr<const wchar_t> tempFileName;


				 if(PortTextBox->Text != "")
					 port = System::Int32::Parse(PortTextBox->Text);

				 init_server(port);
				 run_server();
			 }
	private: System::Void CloseButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->backgroundWorker->CancelAsync();

				 // Disable the Cancel button.
				 this->PortTextBox->Enabled = true;
				 this->ListenButton->Enabled = true;
				 this->CloseButton->Enabled = false;

				 //close_socket();
			 }
	private: System::Void server_menu_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {
				 exit(1);
			 }
	private: System::Void clearButton_Click_1(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }
};
}

#endif
