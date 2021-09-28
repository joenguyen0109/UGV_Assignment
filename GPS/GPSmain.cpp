#include "GPS.h"

#include <conio.h>//_kbhit()

int main() {
	Console::WriteLine("Hello world");
	GPS^ gps = gcnew GPS();

	// setup share memory
	SMObject GPSDataSMObj(_TEXT("GPSData"), sizeof(SM_GPS));
	GPSDataSMObj.SMCreate();
	GPSDataSMObj.SMAccess();

	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();

	gps->setupSharedMemory(GPSDataSMObj, MonitorDataSMObj);
	Threading::Thread::Sleep(100);


	int PortNumber = 24000;
	//gps->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(1000);



	while (!_kbhit())
	{
		if (gps->checkHeartBeat()) {
			break;
		}
		//gps->getShutdownFlag();
		//gps->setHeartbeat(TRUE);
		//gps->getData();
		//if (gps->checkData() == 1) {
		//	gps->sendDataToSharedMemory();
		//}
		Threading::Thread::Sleep(500);
	}
	Console::WriteLine("Program end");
	Console::ReadKey();
	return 0;
}