#ifndef MTRN3500_HUD_H_
#define MTRN3500_HUD_H_

#ifdef __APPLE__
	#include <unistd.h>
#elif defined(WIN32)
	#include <Windows.h>
#else
	#include <unistd.h>
#endif
#include <smstructs.h>
#include <msclr/marshal.h>
using namespace msclr::interop;

class HUD
{
public:
	static void RenderString(const char * str, int x, int y, void * font);
	static void DrawGauge(double x, double y, double r, double min, double max, double val, const char * label, const char * minLabel = NULL, const char * maxLabel = NULL);
	static void Draw(SM_GPS *);
};

#endif
