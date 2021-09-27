#include "GPS.h"

#include <conio.h>//_kbhit()

int main() {
	Console::WriteLine("Hello world");
	int PortNumber = 24000;
	SMObject GPSDataSMObj(_TEXT("GPSData"), sizeof(SM_GPS));
	GPSDataSMObj.SMCreate();
	GPSDataSMObj.SMAccess();
	GPS^ gps = gcnew GPS();
	gps->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(1000);
	gps->setupSharedMemory(GPSDataSMObj);
	Threading::Thread::Sleep(1000);
	while (!_kbhit())
	{
		gps->getShutdownFlag();
		gps->setHeartbeat(TRUE);
		gps->getData();
		if (gps->checkData() == 1) {
			gps->sendDataToSharedMemory();
		}
		Threading::Thread::Sleep(5000);
	}
	
	Console::ReadKey();
	return 0;
}