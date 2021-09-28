#include "PM.h"


Program::Program(String^ Name, bool Crit, int CountLimit) {
	programName = Name;
	crit = Crit;
	count = 1;
	countLimit = CountLimit;
}

int Program::handelHeartBeat()
{
	if (count >= countLimit) {
		if (crit) {
			// shut down
			return 1;
		}
		// reopen
		count = 1;
		return 2;
	}
	else {
		count++;
		return 0;
	}
}

void Program::setCount() {
	count = 1;
}

Program::~Program() {

}


String^ Program::getName() {
	return programName;
}