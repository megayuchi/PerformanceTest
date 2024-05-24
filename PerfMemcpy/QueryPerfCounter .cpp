#include "stdafx.h"
#include "QueryPerfCounter.h"
#include <Windows.h>

LARGE_INTEGER	g_Frequency = {};

void QCInit()
{
	QueryPerformanceFrequency(&g_Frequency);
}
LARGE_INTEGER QCGetCounter()
{
	LARGE_INTEGER CurCounter;
	QueryPerformanceCounter(&CurCounter);
	return CurCounter;
}
float QCMeasureElapsedTick(LARGE_INTEGER CurCounter, LARGE_INTEGER PrvCounter)
{
#ifdef _DEBUG
	if (!g_Frequency.QuadPart)
		__debugbreak();
#endif
	
	UINT64 ElapsedCounter = CurCounter.QuadPart - PrvCounter.QuadPart;
	float ElapsedSec = ((float)ElapsedCounter / (float)g_Frequency.QuadPart);
	float ElapsedMilSec = ElapsedSec * 1000.0f;
	
	return ElapsedMilSec;
}