#include <iostream>
#using <System.dll>
#include <conio.h>//_kbhit()
using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;


int main() {
	std::cout << "PM \n";
	array<String^>^ moduleList = gcnew array<String^>{"GPS"};
	array<int>^ criticalList = gcnew array<int>(moduleList->Length) { 0 };
	array<Process^>^ processList = gcnew array<Process^>(moduleList->Length);

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
	Console::ReadKey();
	return 0;
}