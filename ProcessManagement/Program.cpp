#include "PM.h"


Program::Program(String^ Name, bool Crit, int CountLimit) {
	programName = Name;
	crit = Crit;
	count = 0;
	countLimit = CountLimit;
}

Program::~Program() {

}


String^ Program::getName() {
	return programName;
}