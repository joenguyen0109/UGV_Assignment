#include <conio.h>//_kbhit()
#using <System.dll>
int main() {
	while (!_kbhit()) {

		System::Threading::Thread::Sleep(1000);
	}
	return 0;
}