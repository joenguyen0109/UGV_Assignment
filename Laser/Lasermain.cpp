#include "Laser.h"
#include <conio.h>//_kbhit()
int main() {
	Console::WriteLine("Hello world");
	Laser^ laser = gcnew Laser();


	int PortNumber = 23000;
	laser->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(500);

	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();
	laser->setupSharedMemory(MonitorDataSMObj);


	if (laser->askAuth()) {
		// timeStamp
		__int64 frequency, counter;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

		while (!_kbhit())
		{

			QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			long timestamp = (long)counter / (long)frequency * 1000;
			if (laser->checkHeartBeat(timestamp)) {
				break;
			}
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