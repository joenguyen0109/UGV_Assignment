#include "GPS.h"

#include <conio.h>//_kbhit()

int main() {
	Console::WriteLine("Hello world");
	int PortNumber = 24000;

	GPS^ gps = gcnew GPS();
	gps->connect("192.168.1.200", PortNumber);
	//Client = gcnew TcpClient("192.168.1.200", PortNumber);
	gps->setupSharedMemory();
	while (!_kbhit())
	{

		Threading::Thread::Sleep(3000);
		gps->getShutdownFlag();
		gps->setHeartbeat(TRUE);
		gps->getData();
		gps->checkData();
		gps->sendDataToSharedMemory();



	}
	
	Console::ReadKey();
	return 0;
}