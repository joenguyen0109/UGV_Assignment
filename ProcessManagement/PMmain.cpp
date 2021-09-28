#include <iostream>
#using <System.dll>
#include <conio.h>//_kbhit()
#include <UGV_module.h>

#include <smstructs.h>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;


int main() {
	std::cout << "PM \n";
	array<String^>^ moduleList = gcnew array<String^>{"GPS"};
	array<int>^ criticalList = gcnew array<int>(moduleList->Length) { 0 };
	array<Process^>^ processList = gcnew array<Process^>(moduleList->Length);


	SMObject MonitorDataSMObj(_TEXT("MonitorData"), sizeof(ProcessManagement));
	MonitorDataSMObj.SMCreate();
	MonitorDataSMObj.SMAccess();

	ProcessManagement* dataPointer = (ProcessManagement*)MonitorDataSMObj.pData;
	dataPointer->Heartbeat.Status = 0;
	dataPointer->Shutdown.Status = 0;

	for (int i = 0; i < moduleList->Length; i++) {
		if (Process::GetProcessesByName(moduleList[i])->Length == 0) {
			processList[i] = gcnew Process;
			processList[i]->StartInfo->FileName = moduleList[i];
			processList[i]->Start();
			Console::WriteLine("the process: " + moduleList[i] + ".exe started");
		}
	}

	while (!_kbhit()) {
		Thread::Sleep(1000);
	}

	dataPointer->Shutdown.Status = 0xFF;
	Console::ReadKey();
	return 0;
}