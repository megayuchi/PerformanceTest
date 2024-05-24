#pragma once

inline DWORD64 GetClockCounter()
{
#if defined (ARM64)
	DWORD64 CurClock = _ReadStatusReg(ARM64_PMCCNTR_EL0);
#else
	DWORD64 CurClock = __rdtsc();
#endif
	return CurClock;
}

extern "C"
{
	void MemCpy_1Byte(char* pDest, char* pSrc, size_t Size);
	void MemCpy_4Bytes(char* pDest, char* pSrc, size_t Size);
	void MemCpy_32Bytes(char* pDest, char* pSrc, size_t Size);
	void MemCpy_32Bytes_Debug(char* pDest, char* pSrc, size_t Size);
	void MemCpy_16Bytes(char* pDest, char* pSrc, size_t Size);
	void MemCpy_16Bytes_Debug(char* pDest, char* pSrc, size_t Size);
	void MemCpy_8Bytes(char* pDest, char* pSrc, size_t Size);
	void MemCpy_8Bytes_Debug(char* pDest, char* pSrc, size_t Size);	
}