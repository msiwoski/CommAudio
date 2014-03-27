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
private: System::Windows::Forms::Button^  openSongButton;
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
private: System::Windows::Forms::ComboBox^  protocolTypeChooser;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  Port;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Button^  sendButton;
private: System::Windows::Forms::Button^  openButton;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Button^  receiveButton;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::TextBox^  nameTextBox;
private: System::Windows::Forms::TextBox^  ipAddressTextBox;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Button^  saveButton;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
private: System::Windows::Forms::NumericUpDown^  pktSizeChoice;
private: System::Windows::Forms::NumericUpDown^  sendTimesChoice;
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
		this->portNumberChooser = (gcnew System::Windows::Forms::NumericUpDown());
		this->protocolTypeChooser = (gcnew System::Windows::Forms::ComboBox());
		this->serverButton = (gcnew System::Windows::Forms::RadioButton());
		this->label1 = (gcnew System::Windows::Forms::Label());
		this->clientButton = (gcnew System::Windows::Forms::RadioButton());
		this->Port = (gcnew System::Windows::Forms::Label());
		this->clientGroupBox = (gcnew System::Windows::Forms::GroupBox());
		this->pktSizeChoice = (gcnew System::Windows::Forms::NumericUpDown());
		this->sendTimesChoice = (gcnew System::Windows::Forms::NumericUpDown());
		this->label6 = (gcnew System::Windows::Forms::Label());
		this->nameTextBox = (gcnew System::Windows::Forms::TextBox());
		this->ipAddressTextBox = (gcnew System::Windows::Forms::TextBox());
		this->label5 = (gcnew System::Windows::Forms::Label());
		this->label4 = (gcnew System::Windows::Forms::Label());
		this->label3 = (gcnew System::Windows::Forms::Label());
		this->sendButton = (gcnew System::Windows::Forms::Button());
		this->openButton = (gcnew System::Windows::Forms::Button());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->serverGroupBox = (gcnew System::Windows::Forms::GroupBox());
		this->label7 = (gcnew System::Windows::Forms::Label());
		this->saveButton = (gcnew System::Windows::Forms::Button());
		this->receiveButton = (gcnew System::Windows::Forms::Button());
		this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
		this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
		this->playButton = (gcnew System::Windows::Forms::Button());
		this->stopButton = (gcnew System::Windows::Forms::Button());
		this->pauseButton = (gcnew System::Windows::Forms::Button());
		this->openSongButton = (gcnew System::Windows::Forms::Button());
		this->groupBox1->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->portNumberChooser))->BeginInit();
		this->clientGroupBox->SuspendLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pktSizeChoice))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->sendTimesChoice))->BeginInit();
		this->serverGroupBox->SuspendLayout();
		this->SuspendLayout();
		// 
		// groupBox1
		// 
		this->groupBox1->Controls->Add(this->portNumberChooser);
		this->groupBox1->Controls->Add(this->protocolTypeChooser);
		this->groupBox1->Controls->Add(this->serverButton);
		this->groupBox1->Controls->Add(this->label1);
		this->groupBox1->Controls->Add(this->clientButton);
		this->groupBox1->Controls->Add(this->Port);
		this->groupBox1->Location = System::Drawing::Point(21, 18);
		this->groupBox1->Name = L"groupBox1";
		this->groupBox1->Size = System::Drawing::Size(574, 62);
		this->groupBox1->TabIndex = 0;
		this->groupBox1->TabStop = false;
		this->groupBox1->Text = L"Purpose";
		// 
		// portNumberChooser
		// 
		this->portNumberChooser->Location = System::Drawing::Point(246, 27);
		this->portNumberChooser->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65535, 0, 0, 0});
		this->portNumberChooser->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
		this->portNumberChooser->Name = L"portNumberChooser";
		this->portNumberChooser->Size = System::Drawing::Size(120, 20);
		this->portNumberChooser->TabIndex = 4;
		this->portNumberChooser->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5150, 0, 0, 0});
		// 
		// protocolTypeChooser
		// 
		this->protocolTypeChooser->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->protocolTypeChooser->FormattingEnabled = true;
		this->protocolTypeChooser->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"TCP", L"UDP"});
		this->protocolTypeChooser->Location = System::Drawing::Point(451, 26);
		this->protocolTypeChooser->MaxDropDownItems = 2;
		this->protocolTypeChooser->Name = L"protocolTypeChooser";
		this->protocolTypeChooser->Size = System::Drawing::Size(114, 21);
		this->protocolTypeChooser->TabIndex = 3;
		// 
		// serverButton
		// 
		this->serverButton->AutoSize = true;
		this->serverButton->Location = System::Drawing::Point(92, 27);
		this->serverButton->Name = L"serverButton";
		this->serverButton->Size = System::Drawing::Size(56, 17);
		this->serverButton->TabIndex = 1;
		this->serverButton->TabStop = true;
		this->serverButton->Text = L"Server";
		this->serverButton->UseVisualStyleBackColor = true;
		this->serverButton->Click += gcnew System::EventHandler(this, &MyForm::serverButton_Click);
		// 
		// label1
		// 
		this->label1->AutoSize = true;
		this->label1->Location = System::Drawing::Point(372, 29);
		this->label1->Name = L"label1";
		this->label1->Size = System::Drawing::Size(76, 13);
		this->label1->TabIndex = 2;
		this->label1->Text = L"Protocol Type:";
		// 
		// clientButton
		// 
		this->clientButton->AutoSize = true;
		this->clientButton->Location = System::Drawing::Point(16, 27);
		this->clientButton->Name = L"clientButton";
		this->clientButton->Size = System::Drawing::Size(51, 17);
		this->clientButton->TabIndex = 0;
		this->clientButton->TabStop = true;
		this->clientButton->Text = L"Client";
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
		this->clientGroupBox->Controls->Add(this->pktSizeChoice);
		this->clientGroupBox->Controls->Add(this->sendTimesChoice);
		this->clientGroupBox->Controls->Add(this->label6);
		this->clientGroupBox->Controls->Add(this->nameTextBox);
		this->clientGroupBox->Controls->Add(this->ipAddressTextBox);
		this->clientGroupBox->Controls->Add(this->label5);
		this->clientGroupBox->Controls->Add(this->label4);
		this->clientGroupBox->Controls->Add(this->label3);
		this->clientGroupBox->Controls->Add(this->sendButton);
		this->clientGroupBox->Controls->Add(this->openButton);
		this->clientGroupBox->Controls->Add(this->label2);
		this->clientGroupBox->Location = System::Drawing::Point(23, 97);
		this->clientGroupBox->Name = L"clientGroupBox";
		this->clientGroupBox->Size = System::Drawing::Size(267, 263);
		this->clientGroupBox->TabIndex = 1;
		this->clientGroupBox->TabStop = false;
		this->clientGroupBox->Text = L"Client";
		// 
		// pktSizeChoice
		// 
		this->pktSizeChoice->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
		this->pktSizeChoice->Location = System::Drawing::Point(90, 76);
		this->pktSizeChoice->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {262144, 0, 0, 0});
		this->pktSizeChoice->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
		this->pktSizeChoice->Name = L"pktSizeChoice";
		this->pktSizeChoice->Size = System::Drawing::Size(158, 20);
		this->pktSizeChoice->TabIndex = 12;
		this->pktSizeChoice->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
		// 
		// sendTimesChoice
		// 
		this->sendTimesChoice->Location = System::Drawing::Point(90, 103);
		this->sendTimesChoice->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
		this->sendTimesChoice->Name = L"sendTimesChoice";
		this->sendTimesChoice->Size = System::Drawing::Size(158, 20);
		this->sendTimesChoice->TabIndex = 11;
		this->sendTimesChoice->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
		// 
		// label6
		// 
		this->label6->AutoSize = true;
		this->label6->Location = System::Drawing::Point(46, 159);
		this->label6->Name = L"label6";
		this->label6->Size = System::Drawing::Size(38, 13);
		this->label6->TabIndex = 10;
		this->label6->Text = L"Name:";
		// 
		// nameTextBox
		// 
		this->nameTextBox->Location = System::Drawing::Point(90, 156);
		this->nameTextBox->Name = L"nameTextBox";
		this->nameTextBox->Size = System::Drawing::Size(158, 20);
		this->nameTextBox->TabIndex = 9;
		this->nameTextBox->Text = L"localhost";
		// 
		// ipAddressTextBox
		// 
		this->ipAddressTextBox->Location = System::Drawing::Point(90, 129);
		this->ipAddressTextBox->Name = L"ipAddressTextBox";
		this->ipAddressTextBox->Size = System::Drawing::Size(158, 20);
		this->ipAddressTextBox->TabIndex = 8;
		this->ipAddressTextBox->Text = L"127.0.0.1";
		// 
		// label5
		// 
		this->label5->AutoSize = true;
		this->label5->Location = System::Drawing::Point(21, 132);
		this->label5->Name = L"label5";
		this->label5->Size = System::Drawing::Size(61, 13);
		this->label5->TabIndex = 7;
		this->label5->Text = L"IP Address:";
		// 
		// label4
		// 
		this->label4->AutoSize = true;
		this->label4->Location = System::Drawing::Point(22, 105);
		this->label4->Name = L"label4";
		this->label4->Size = System::Drawing::Size(60, 13);
		this->label4->TabIndex = 6;
		this->label4->Text = L"# of Times:";
		// 
		// label3
		// 
		this->label3->AutoSize = true;
		this->label3->Location = System::Drawing::Point(15, 78);
		this->label3->Name = L"label3";
		this->label3->Size = System::Drawing::Size(67, 13);
		this->label3->TabIndex = 3;
		this->label3->Text = L"Packet Size:";
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
		// label2
		// 
		this->label2->AutoSize = true;
		this->label2->Location = System::Drawing::Point(18, 38);
		this->label2->Name = L"label2";
		this->label2->Size = System::Drawing::Size(52, 13);
		this->label2->TabIndex = 0;
		this->label2->Text = L"Open File";
		// 
		// serverGroupBox
		// 
		this->serverGroupBox->Controls->Add(this->label7);
		this->serverGroupBox->Controls->Add(this->saveButton);
		this->serverGroupBox->Controls->Add(this->receiveButton);
		this->serverGroupBox->Location = System::Drawing::Point(308, 97);
		this->serverGroupBox->Name = L"serverGroupBox";
		this->serverGroupBox->Size = System::Drawing::Size(287, 264);
		this->serverGroupBox->TabIndex = 2;
		this->serverGroupBox->TabStop = false;
		this->serverGroupBox->Text = L"Server";
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
		// playButton
		// 
		this->playButton->Location = System::Drawing::Point(620, 18);
		this->playButton->Name = L"playButton";
		this->playButton->Size = System::Drawing::Size(75, 23);
		this->playButton->TabIndex = 3;
		this->playButton->Text = L"Play";
		this->playButton->UseVisualStyleBackColor = true;
		this->playButton->Click += gcnew System::EventHandler(this, &MyForm::playButton_Click);
		// 
		// stopButton
		// 
		this->stopButton->Location = System::Drawing::Point(620, 47);
		this->stopButton->Name = L"stopButton";
		this->stopButton->Size = System::Drawing::Size(75, 23);
		this->stopButton->TabIndex = 4;
		this->stopButton->Text = L"Stop";
		this->stopButton->UseVisualStyleBackColor = true;
		this->stopButton->Click += gcnew System::EventHandler(this, &MyForm::stopButton_Click);
		// 
		// pauseButton
		// 
		this->pauseButton->Location = System::Drawing::Point(620, 77);
		this->pauseButton->Name = L"pauseButton";
		this->pauseButton->Size = System::Drawing::Size(75, 23);
		this->pauseButton->TabIndex = 5;
		this->pauseButton->Text = L"Pause";
		this->pauseButton->UseVisualStyleBackColor = true;
		this->pauseButton->Click += gcnew System::EventHandler(this, &MyForm::pauseButton_Click);
		// 
		// openSongButton
		// 
		this->openSongButton->Location = System::Drawing::Point(620, 124);
		this->openSongButton->Name = L"openSongButton";
		this->openSongButton->Size = System::Drawing::Size(75, 23);
		this->openSongButton->TabIndex = 6;
		this->openSongButton->Text = L"Open Song";
		this->openSongButton->UseVisualStyleBackColor = true;
		this->openSongButton->Click += gcnew System::EventHandler(this, &MyForm::openSong_Click);
		// 
		// MyForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(750, 371);
		this->Controls->Add(this->openSongButton);
		this->Controls->Add(this->pauseButton);
		this->Controls->Add(this->stopButton);
		this->Controls->Add(this->playButton);
		this->Controls->Add(this->serverGroupBox);
		this->Controls->Add(this->clientGroupBox);
		this->Controls->Add(this->groupBox1);
		this->Name = L"MyForm";
		this->Text = L"Client/Server";
		this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
		this->groupBox1->ResumeLayout(false);
		this->groupBox1->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->portNumberChooser))->EndInit();
		this->clientGroupBox->ResumeLayout(false);
		this->clientGroupBox->PerformLayout();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pktSizeChoice))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->sendTimesChoice))->EndInit();
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

