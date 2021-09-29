#include <iostream>
#using <System.dll>
#include <conio.h>//_kbhit()
#include <UGV_module.h>
#include "PM.h"
#include <smstructs.h>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;


int main() {
	std::cout << "PM \n";
	Program^ GPS = gcnew Program("GPS", false, 5);
	Program^ Laser = gcnew Program("Laser", true, 10);
	Program^ VehicleControl = gcnew Program("VehicleControl", true, 1000);
	Program^ Camera = gcnew Program("Camera", true, 10000);
	Program^ Display = gcnew Program("Display", false, 10);
	array<Program^>^ moduleList = gcnew array<Program^>{Laser, VehicleControl, GPS, Display, Camera};
	array<Process^>^ processList = gcnew array<Process^>(moduleList->Length);

	// setup SM
	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();
	ProcessManagement* dataPointer = (ProcessManagement*)MonitorDataSMObj.pData;
	dataPointer->Heartbeat.Status = 0;
	dataPointer->Shutdown.Status = 0;
	dataPointer->LifeCounter = 0;


	double timeStamp;
	__int64 frequency, counter;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	for (int i = 0; i < moduleList->Length; i++) {
		if (Process::GetProcessesByName(moduleList[i]->getName())->Length == 0) {
			processList[i] = gcnew Process;
			processList[i]->StartInfo->FileName = moduleList[i]->getName();
			processList[i]->Start();
			Console::WriteLine("the process: " + moduleList[i]->getName() + ".exe started");
		}
	}

	bool shutdown = false;

	while (!_kbhit()) {
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		dataPointer->LifeCounter = (long)counter / (long)frequency * 1000;
		for (int i = 0; i < 5; i++) {
			if ((dataPointer->Heartbeat.Status >> i) & 1) {
				dataPointer->Heartbeat.Status &= ~(1UL << i);
				moduleList[i]->setCount();
			}else {
				int value = moduleList[i]->handelHeartBeat();
				if (value == 1) {
					Console::WriteLine("Missing " + moduleList[i]->getName() + ".exe leads to shutdown");
					shutdown = true;
					break;
				}
				else if (value == 2) {
					Console::WriteLine("Reopen: " + moduleList[i]->getName() + ".exe");
					if (processList[i]->HasExited) {
						processList[i]->Start();
					}
					else {
						processList[i]->Kill();
						processList[i]->Start();
					}
				}
			}
		}
		if (shutdown) {
			break;
		}
		Thread::Sleep(500);
	}

	dataPointer->Shutdown.Status = 0xFF;
	Console::WriteLine("Program end");
	Console::ReadKey();
	return 0;
}