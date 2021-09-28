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
	Program^ GPS = gcnew Program("GPS", false, 20);
	Program^ Laser = gcnew Program("Laser", true, 10);
	Program^ VehicleControl = gcnew Program("VehicleControl", true, 10);
	Program^ Camera = gcnew Program("Camera", true, 10);
	Program^ Display = gcnew Program("Display", false, 20);
	array<Program^>^ moduleList = gcnew array<Program^>{Laser, VehicleControl, GPS, Display, Camera};
	//array<String^>^ moduleList = gcnew array<String^>{"Laser","VehicleControl","GPS","Display","Camera"};
	//array<int>^ criticalList = gcnew array<int>(moduleList->Length) {1,1,0,0,1 };
	//array<int>^ countLimit = gcnew array<int>(moduleList->Length) { 0, 0, 0, 0, 0 };
	//array<int>^ timeLimit = gcnew array<int>(moduleList->Length) { 10, 10, 20, 20, 10 };
	array<Process^>^ processList = gcnew array<Process^>(moduleList->Length);


	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();

	ProcessManagement* dataPointer = (ProcessManagement*)MonitorDataSMObj.pData;

	

	dataPointer->Heartbeat.Status = 0;
	dataPointer->Shutdown.Status = 0;



	for (int i = 0; i < moduleList->Length; i++) {
		if (Process::GetProcessesByName(moduleList[i]->getName())->Length == 0) {
			processList[i] = gcnew Process;
			processList[i]->StartInfo->FileName = moduleList[i]->getName();
			processList[i]->Start();
			Console::WriteLine("the process: " + moduleList[i]->getName() + ".exe started");
		}
	}
	dataPointer->Heartbeat.Flags.GPS = 1;
	while (!_kbhit()) {
		for (int i = 0; i < 5; i++) {
			if ((dataPointer->Heartbeat.Status >> i) & 1) {
				dataPointer->Heartbeat.Status &= ~(1UL << i);
			}else {
				
			}
		}
		Thread::Sleep(1000);
	}

	dataPointer->Shutdown.Status = 0xFF;
	Console::ReadKey();
	return 0;
}