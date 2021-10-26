#include "GPS.h"

#include <conio.h>//_kbhit()

int main() {
	Console::WriteLine("Hello world");
	GPS^ gps = gcnew GPS();



	int PortNumber = 24000;
	gps->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(1000);
	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();
	gps->setupSharedMemory(MonitorDataSMObj);
	// timeStamp
	__int64 frequency, counter;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);


	while (!_kbhit())
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		long timestamp = (long)counter / (long)frequency * 1000;
		if (gps->checkHeartBeat(timestamp)) {
			break;
		}
		gps->getShutdownFlag();
		gps->setHeartbeat(TRUE);
		gps->getData();
		if (gps->checkData() == 1) {
			gps->sendDataToSharedMemory();
		}
		Threading::Thread::Sleep(500);
	}
	Console::WriteLine("Program end");
	Console::ReadKey();
	return 0;
}