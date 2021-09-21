#using <System.dll>
#include <conio.h>//_kbhit()
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;
int main() {
	Console::WriteLine("Hello world");
	int PortNumber = 24000;
	TcpClient^ Client;
	array<unsigned char>^ ReadData;
	String^ ResponseData;

	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;
	//Loop
	NetworkStream^ Stream = Client->GetStream();
	Console::WriteLine("Hello world");
	ReadData = gcnew array<unsigned char>(84);
	while (!_kbhit())
	{

		Threading::Thread::Sleep(1000);
		Console::WriteLine("---------");

		Stream->Read(ReadData, 0, ReadData->Length);
		Console::WriteLine(System::BitConverter::ToString(ReadData));
		Console::WriteLine(System::BitConverter::ToSingle(ReadData, 48));
		Console::WriteLine(System::BitConverter::ToSingle(ReadData, 52));
		Console::WriteLine(System::BitConverter::ToSingle(ReadData, 56));

	}
	Stream->Close();
	Client->Close();
	Console::ReadKey();
	return 0;
}