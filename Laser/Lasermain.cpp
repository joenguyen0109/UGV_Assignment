#include "Laser.h"
#include <conio.h>//_kbhit()
int main() {
	Console::WriteLine("Hello world");
	Laser^ laser = gcnew Laser();
	
	SMObject LaserDataSMObj(_TEXT("LaserData"), sizeof(SM_Laser));
	LaserDataSMObj.SMCreate();
	LaserDataSMObj.SMAccess();
	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();
	laser->setupSharedMemory(LaserDataSMObj, MonitorDataSMObj);
	Threading::Thread::Sleep(500);


	int PortNumber = 23000;
	laser->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(500);




	if (laser->askAuth()) {
		// timeStamp
		__int64 frequency, counter;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

		while (!_kbhit())
		{

			QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			long timestamp = (long)counter / (long)frequency * 1000;
			//if (laser->checkHeartBeat(timestamp)) {
			//	break;
			//}
			/*	laser->getShutdownFlag();
				laser->setHeartbeat(TRUE);*/
			laser->sendData();
			laser->getData();
			if (laser->checkData() == 1) {
				laser->sendDataToSharedMemory();
			}

			Threading::Thread::Sleep(100);

		}
	}
	else {
		Console::WriteLine("NO AUTH");
	}


	Console::WriteLine("Program end");
	Console::ReadKey();

	return 0;
}