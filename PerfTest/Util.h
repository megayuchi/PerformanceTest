#pragma once

void MemCpyAVX(char* pDest, char* pSrc, size_t Size);


inline DWORD64 GetClockCounter()
{
#if defined (_M_ARM64) || defined(_M_ARM64EC)
	DWORD64 CurClock = _ReadStatusReg(ARM64_PMCCNTR_EL0);
#else
	DWORD64 CurClock = __rdtsc();
#endif
	return CurClock;
}