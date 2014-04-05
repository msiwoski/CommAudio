#pragma once

#include "Variables.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Windows::Forms::DataVisualization::Charting;

/// <summary>
/// Summary for MyForm
/// </summary>
public ref class MyForm : public System::Windows::Forms::Form
{
public:
	int portNum;
	int protocolType;
	//int packetSize;
	String^ ipAddress;
	String^ nameDest;
	String^ errMsg;
private: System::Windows::Forms::Button^  playButton;
private: System::Windows::Forms::Button^  stopButton;
private: System::Windows::Forms::Button^  pauseButton;

public: 

public: 
	Stream^ sr;	

	MyForm(void) : portNum(80), protocolType(0), ipAddress("127.0.0.1"), nameDest("localhost"), errMsg("error"){
		InitializeComponent();
		//
		//TODO: Add the constructor code here
		//	
		
	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~MyForm()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::GroupBox^  groupBox1;
protected: 
private: System::Windows::Forms::RadioButton^  serverButton;
private: System::Windows::Forms::RadioButton^  clientButton;
private: System::Windows::Forms::GroupBox^  clientGroupBox;
private: System::Windows::Forms::GroupBox^  serverGroupBox;


private: System::Windows::Forms::Label^  Port;
private: System::Windows::Forms::Label^  label5;


private: System::Windows::Forms::Button^  sendButton;
private: System::Windows::Forms::Button^  openButton;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Button^  receiveButton;


private: System::Windows::Forms::TextBox^  ipAddressTextBox;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Button^  saveButton;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;


private: System::Windows::Forms::NumericUpDown^  portNumberChooser;

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
		this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
		this->ipAddressTextBox = (gcnew System::Windows::Forms::TextBox());
		this->portNumberChooser = (gcnew System::Windows::Forms::NumericUpDown());
		this->label5 = (gcnew System::Windows::Forms::Label());
		this->serverButton = (gcnew System::Windows::Forms::RadioButton());
		this->clientButton = (gcnew System::Windows::Forms::RadioButton());
		this->Port = (gcnew System::Windows::Forms::Label());
		this->clientGroupBox = (gcnew System::Windows::Forms::GroupBox());
		this->sendButton = (gcnew System::Windows::Forms::Button());
		this->pauseButton = (gcnew System::Windows::Forms::Button());
		this->openButton = (gcnew System::Windows::Forms::Button());
		this->stopButton = (gcnew System::Windows::Forms::Button());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->playButton = (gcnew System::Windows::Forms::Button());
		this->serverGroupBox = (gcnew System::Windows::Forms::GroupBox());
		this->label7 = (gcnew System::Windows::Forms::Label());
		this->saveButton = (gcnew System::Windows::Forms::Button());
		this->receiveButton = (gcnew System::Windows::Forms::Button());
		this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
		this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
		this->groupBox1->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->portNumberChooser))->BeginInit();
		this->clientGroupBox->SuspendLayout();
		this->serverGroupBox->SuspendLayout();
		this->SuspendLayout();
		// 
		// groupBox1
		// 
		this->groupBox1->Controls->Add(this->ipAddressTextBox);
		this->groupBox1->Controls->Add(this->portNumberChooser);
		this->groupBox1->Controls->Add(this->label5);
		this->groupBox1->Controls->Add(this->serverButton);
		this->groupBox1->Controls->Add(this->clientButton);
		this->groupBox1->Controls->Add(this->Port);
		this->groupBox1->Location = System::Drawing::Point(21, 18);
		this->groupBox1->Name = L"groupBox1";
		this->groupBox1->Size = System::Drawing::Size(574, 62);
		this->groupBox1->TabIndex = 0;
		this->groupBox1->TabStop = false;
		this->groupBox1->Text = L"Purpose";
		// 
		// ipAddressTextBox
		// 
		this->ipAddressTextBox->Location = System::Drawing::Point(449, 27);
		this->ipAddressTextBox->Name = L"ipAddressTextBox";
		this->ipAddressTextBox->Size = System::Drawing::Size(116, 20);
		this->ipAddressTextBox->TabIndex = 8;
		this->ipAddressTextBox->Text = L"127.0.0.1";
		// 
		// portNumberChooser
		// 
		this->portNumberChooser->Location = System::Drawing::Point(246, 27);
		this->portNumberChooser->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65535, 0, 0, 0});
		this->portNumberChooser->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
		this->portNumberChooser->Name = L"portNumberChooser";
		this->portNumberChooser->Size = System::Drawing::Size(120, 20);
		this->portNumberChooser->TabIndex = 4;
		this->portNumberChooser->UseWaitCursor = true;
		this->portNumberChooser->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5151, 0, 0, 0});
		// 
		// label5
		// 
		this->label5->AutoSize = true;
		this->label5->Location = System::Drawing::Point(382, 29);
		this->label5->Name = L"label5";
		this->label5->Size = System::Drawing::Size(61, 13);
		this->label5->TabIndex = 7;
		this->label5->Text = L"IP Address:";
		// 
		// serverButton
		// 
		this->serverButton->AutoSize = true;
		this->serverButton->Location = System::Drawing::Point(92, 27);
		this->serverButton->Name = L"serverButton";
		this->serverButton->Size = System::Drawing::Size(51, 17);
		this->serverButton->TabIndex = 1;
		this->serverButton->TabStop = true;
		this->serverButton->Text = L"Client";
		this->serverButton->UseVisualStyleBackColor = true;
		this->serverButton->Click += gcnew System::EventHandler(this, &MyForm::serverButton_Click);
		// 
		// clientButton
		// 
		this->clientButton->AutoSize = true;
		this->clientButton->Location = System::Drawing::Point(16, 27);
		this->clientButton->Name = L"clientButton";
		this->clientButton->Size = System::Drawing::Size(56, 17);
		this->clientButton->TabIndex = 0;
		this->clientButton->TabStop = true;
		this->clientButton->Text = L"Server";
		this->clientButton->UseVisualStyleBackColor = true;
		this->clientButton->Click += gcnew System::EventHandler(this, &MyForm::clientButton_Click);
		// 
		// Port
		// 
		this->Port->AutoSize = true;
		this->Port->Location = System::Drawing::Point(171, 29);
		this->Port->Name = L"Port";
		this->Port->Size = System::Drawing::Size(69, 13);
		this->Port->TabIndex = 0;
		this->Port->Text = L"Port Number:";
		// 
		// clientGroupBox
		// 
		this->clientGroupBox->Controls->Add(this->sendButton);
		this->clientGroupBox->Controls->Add(this->openButton);
		this->clientGroupBox->Controls->Add(this->label2);
		this->clientGroupBox->Location = System::Drawing::Point(23, 97);
		this->clientGroupBox->Name = L"clientGroupBox";
		this->clientGroupBox->Size = System::Drawing::Size(267, 263);
		this->clientGroupBox->TabIndex = 1;
		this->clientGroupBox->TabStop = false;
		this->clientGroupBox->Text = L"Server";
		// 
		// sendButton
		// 
		this->sendButton->Location = System::Drawing::Point(6, 193);
		this->sendButton->Name = L"sendButton";
		this->sendButton->Size = System::Drawing::Size(255, 64);
		this->sendButton->TabIndex = 2;
		this->sendButton->Text = L"Send";
		this->sendButton->UseVisualStyleBackColor = true;
		this->sendButton->Click += gcnew System::EventHandler(this, &MyForm::sendButton_Click);
		// 
		// pauseButton
		// 
		this->pauseButton->Location = System::Drawing::Point(188, 72);
		this->pauseButton->Name = L"pauseButton";
		this->pauseButton->Size = System::Drawing::Size(75, 23);
		this->pauseButton->TabIndex = 5;
		this->pauseButton->Text = L"Pause";
		this->pauseButton->UseVisualStyleBackColor = true;
		this->pauseButton->Click += gcnew System::EventHandler(this, &MyForm::pauseButton_Click);
		// 
		// openButton
		// 
		this->openButton->Location = System::Drawing::Point(73, 33);
		this->openButton->Name = L"openButton";
		this->openButton->Size = System::Drawing::Size(175, 23);
		this->openButton->TabIndex = 1;
		this->openButton->Text = L"Open";
		this->openButton->UseVisualStyleBackColor = true;
		this->openButton->Click += gcnew System::EventHandler(this, &MyForm::openButton_Click);
		// 
		// stopButton
		// 
		this->stopButton->Location = System::Drawing::Point(107, 72);
		this->stopButton->Name = L"stopButton";
		this->stopButton->Size = System::Drawing::Size(75, 23);
		this->stopButton->TabIndex = 4;
		this->stopButton->Text = L"Stop";
		this->stopButton->UseVisualStyleBackColor = true;
		this->stopButton->Click += gcnew System::EventHandler(this, &MyForm::stopButton_Click);
		// 
		// label2
		// 
		this->label2->AutoSize = true;
		this->label2->Location = System::Drawing::Point(18, 38);
		this->label2->Name = L"label2";
		this->label2->Size = System::Drawing::Size(52, 13);
		this->label2->TabIndex = 0;
		this->label2->Text = L"Open File";
		// 
		// playButton
		// 
		this->playButton->Location = System::Drawing::Point(26, 72);
		this->playButton->Name = L"playButton";
		this->playButton->Size = System::Drawing::Size(75, 23);
		this->playButton->TabIndex = 3;
		this->playButton->Text = L"Play";
		this->playButton->UseVisualStyleBackColor = true;
		this->playButton->Click += gcnew System::EventHandler(this, &MyForm::playButton_Click);
		// 
		// serverGroupBox
		// 
		this->serverGroupBox->Controls->Add(this->label7);
		this->serverGroupBox->Controls->Add(this->pauseButton);
		this->serverGroupBox->Controls->Add(this->saveButton);
		this->serverGroupBox->Controls->Add(this->receiveButton);
		this->serverGroupBox->Controls->Add(this->stopButton);
		this->serverGroupBox->Controls->Add(this->playButton);
		this->serverGroupBox->Location = System::Drawing::Point(308, 97);
		this->serverGroupBox->Name = L"serverGroupBox";
		this->serverGroupBox->Size = System::Drawing::Size(287, 264);
		this->serverGroupBox->TabIndex = 2;
		this->serverGroupBox->TabStop = false;
		this->serverGroupBox->Text = L"Client";
		// 
		// label7
		// 
		this->label7->AutoSize = true;
		this->label7->Location = System::Drawing::Point(12, 38);
		this->label7->Name = L"label7";
		this->label7->Size = System::Drawing::Size(51, 13);
		this->label7->TabIndex = 2;
		this->label7->Text = L"Save File";
		// 
		// saveButton
		// 
		this->saveButton->Location = System::Drawing::Point(69, 33);
		this->saveButton->Name = L"saveButton";
		this->saveButton->Size = System::Drawing::Size(209, 23);
		this->saveButton->TabIndex = 1;
		this->saveButton->Text = L"Save";
		this->saveButton->UseVisualStyleBackColor = true;
		this->saveButton->Click += gcnew System::EventHandler(this, &MyForm::saveButton_Click);
		// 
		// receiveButton
		// 
		this->receiveButton->Enabled = false;
		this->receiveButton->Location = System::Drawing::Point(7, 193);
		this->receiveButton->Name = L"receiveButton";
		this->receiveButton->Size = System::Drawing::Size(274, 64);
		this->receiveButton->TabIndex = 0;
		this->receiveButton->Text = L"Receive";
		this->receiveButton->UseVisualStyleBackColor = true;
		this->receiveButton->Click += gcnew System::EventHandler(this, &MyForm::receiveButton_Click);
		// 
		// openFileDialog1
		// 
		this->openFileDialog1->FileName = L"openFileDialog1";
		// 
		// MyForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(594, 371);
		this->Controls->Add(this->serverGroupBox);
		this->Controls->Add(this->clientGroupBox);
		this->Controls->Add(this->groupBox1);
		this->Name = L"MyForm";
		this->Text = L"Comm Audio";
		this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
		this->groupBox1->ResumeLayout(false);
		this->groupBox1->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->portNumberChooser))->EndInit();
		this->clientGroupBox->ResumeLayout(false);
		this->clientGroupBox->PerformLayout();
		this->serverGroupBox->ResumeLayout(false);
		this->serverGroupBox->PerformLayout();
		this->ResumeLayout(false);

	}
#pragma endregion

private: System::Void openButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void sendButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void receiveButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e);
private: System::Void clientButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void serverButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void playButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void pauseButton_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void openSong_Click(System::Object^  sender, System::EventArgs^  e);
};

