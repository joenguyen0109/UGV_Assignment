//Compile in a C++ CLR empty project
#using <System.dll>

#include <conio.h>//_kbhit()

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

int main()
{
	// LMS151 port number must be 2111
	int PortNumber = 23000;
	// Pointer to TcpClent type object on managed heap
	TcpClient^ Client;
	// arrays of unsigned chars to send and receive data
	array<unsigned char>^ SendData;
	array<unsigned char>^ ReadData;
	// String command to ask for Channel 1 analogue voltage from the PLC
	// These command are available on Galil RIO47122 command reference manual
	// available online
	String^ AskScan = gcnew String("5253838\n");
	// String to store received data for display
	String^ ResponseData;

	// Creat TcpClient object and connect to it
	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 5000;//ms
	Client->SendTimeout = 5000;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	// unsigned char arrays of 16 bytes each are created on managed heap
	SendData = gcnew array<unsigned char>(16);
	ReadData = gcnew array<unsigned char>(2500);
	// Convert string command to an array of unsigned char
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);


	// Get the network streab object associated with clien so we 
	// can use it to read and write
	NetworkStream^ Stream = Client->GetStream();

	Stream->Write(SendData, 0, SendData->Length);
	System::Threading::Thread::Sleep(10);
	Stream->Read(ReadData, 0, ReadData->Length);
	ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);

	String^ AskScan2 = gcnew String("sRN LMDscandata");
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan2);
	//Loop

	array<System::String^>^ data;
	while (!_kbhit())
	{

	}

	Stream->Close();
	Client->Close();

	Console::ReadKey();

	return 0;
}