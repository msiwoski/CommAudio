#pragma once

#ifndef _MAINMENU_
#define _MAINMENU_

#include "client_menu.h"
#include "server_menu.h"

namespace TCPIP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for main_menu
	/// </summary>
	public ref class main_menu : public System::Windows::Forms::Form
	{
	public:
		main_menu(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~main_menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  clientButton;
	private: System::Windows::Forms::Button^  serverButton;
	protected: 


	protected: 


	protected: 

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
			this->clientButton = (gcnew System::Windows::Forms::Button());
			this->serverButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// clientButton
			// 
			this->clientButton->Location = System::Drawing::Point(44, 41);
			this->clientButton->Name = L"clientButton";
			this->clientButton->Size = System::Drawing::Size(142, 67);
			this->clientButton->TabIndex = 0;
			this->clientButton->Text = L"Client";
			this->clientButton->UseVisualStyleBackColor = true;
			this->clientButton->Click += gcnew System::EventHandler(this, &main_menu::clientButton_Click);
			// 
			// serverButton
			// 
			this->serverButton->Location = System::Drawing::Point(44, 123);
			this->serverButton->Name = L"serverButton";
			this->serverButton->Size = System::Drawing::Size(142, 67);
			this->serverButton->TabIndex = 1;
			this->serverButton->Text = L"Server";
			this->serverButton->UseVisualStyleBackColor = true;
			this->serverButton->Click += gcnew System::EventHandler(this, &main_menu::serverButton_Click);
			// 
			// main_menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(231, 234);
			this->Controls->Add(this->serverButton);
			this->Controls->Add(this->clientButton);
			this->Name = L"main_menu";
			this->Text = L"main_menu";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void clientButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 client_menu ^client = gcnew client_menu();
				 client->Show();
				 this->Hide();
			 }
	private: System::Void serverButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 server_menu ^server = gcnew server_menu();
				 server->Show();
				 this->Hide();
			 }
	};
}

#endif