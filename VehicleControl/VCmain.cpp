//Compile in a C++ CLR empty project
#using <System.dll>

#include <conio.h>//_kbhit()
#include "VehicleControl.h"
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

int main()
{
	Console::WriteLine("Hello world");
	VehicleControl^ vc = gcnew VehicleControl();

	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();
	vc->setupSharedMemory(MonitorDataSMObj);

	int PortNumber = 25000;
	vc->connect("192.168.1.200", PortNumber);
	Threading::Thread::Sleep(500);


	if (vc->askAuth()) {
		// timeStamp
		__int64 frequency, counter;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

		while (!_kbhit())
		{

			QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			long timestamp = (long)counter / (long)frequency * 1000;
			if (vc->checkHeartBeat(timestamp)) {
				break;
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