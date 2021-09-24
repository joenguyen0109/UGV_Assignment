#include "Laser.h"
#include <conio.h>//_kbhit()
int main() {
	Console::WriteLine("Hello world");
	int PortNumber = 24000;
	SMObject LaserDataSMObj(_TEXT("LaserData"), sizeof(LaserData));
	LaserDataSMObj.SMCreate();
	LaserDataSMObj.SMAccess();
	Laser^ laser = gcnew Laser();
	laser->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(1000);
	laser->setupSharedMemory(LaserDataSMObj);
	Threading::Thread::Sleep(1000);


	while (!_kbhit())
	{


	/*	laser->getShutdownFlag();
		laser->setHeartbeat(TRUE);*/
		laser->sendData();
		laser->getData();
		//if (laser->checkData() == 1) {
		//	laser->sendDataToSharedMemory();
		//}

		Threading::Thread::Sleep(100);

	}

	Console::ReadKey();

	return 0;
}