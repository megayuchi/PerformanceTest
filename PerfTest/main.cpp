// PerfTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <conio.h>
#include "SIMD_MATH.h"
#include <intrin.h>
#include "QueryPerfCounter.h"
#include "ProcessorInfo.h"
#include"ArithmeticTest.h"
#include "MultiThreadTest.h"



// 멀티스레드 _InterlockedInc()
// 멀티스레드 AcquireSpinLock()
// 멀티스레드 SRW_Lock
// 멀티스레드 CriticalSection
// 멀티스레드 SetEvent() - WaitFor..()에서 깨어날때까지 걸리는 시간
// 싱글스레드 memcpy
// 멀티스레드 memcpy


#define QUALCOMM_8CX

int wmain(int argc, WCHAR* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	BOOL	bTestThreadScheduling = TRUE;
	if (argc >= 2)
	{
		if (!_wcsicmp(argv[1], L"/onlymath"))
		{
			bTestThreadScheduling = FALSE;
		}
	}
	/*
 #ifdef V8_HOST_ARCH_ARM64
 #define ReadCycleCounter() _ReadStatusReg(ARM64_PMCCNTR_EL0)
 #else
 #define ReadCycleCounter() __rdtsc()
 #endif
	*/
	/*
	ARM64_PMCCNTR_EL0;
	//PMCCNTR_EL0;
	int a = rand();
	int b = rand();
	int c = rand();
	unsigned __int64 PrvClock = _ReadStatusReg(ARM64_PMCCNTR_EL0);

	for (DWORD i = 0; i < 1000; i++)
	{
		a++;
		b++;
		c = a + b;
	}
	unsigned __int64 CurClock = _ReadStatusReg(ARM64_PMCCNTR_EL0);

	unsigned __int64 ElapsedClock = CurClock - PrvClock;

	wprintf_s(L"%d %d %d", a,b,c);

	*/
	QCInit();


	/*
	size_t MemSize = 1024ull * 1024ull * 1024ull * 2;
	char* pSrc = (char*)_aligned_malloc(MemSize, 32);
	char* pDest = (char*)_aligned_malloc(MemSize, 32);

	memset(pSrc, 0xff, MemSize);
	memset(pDest, 0, MemSize);

	LARGE_INTEGER PrvCounter = QCGetCounter();
	MemCpyAVX(pDest, pSrc, MemSize);

	float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	char*	pEntry = pDest;
	for (DWORD i = 0; i < MemSize / 4; i++)
	{
		if (*(DWORD*)pEntry != 0xffffffff)
			__debugbreak();

		pEntry += 4;
	}
	wprintf_s(L"AVX MemCpy : %.4f ms\n", fElapsedTick);

	PrvCounter = QCGetCounter();
	memcpy(pDest, pSrc, MemSize);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	wprintf_s(L"memcpy : %.4f ms\n", fElapsedTick);

	int a = 0;
	_aligned_free(pSrc);
	_aligned_free(pDest);

	*/
	srand(1);

	if (bTestThreadScheduling)
	{
		// multi thread test
		wprintf_s(L"<Multi-Threasd Test>---------------------------------------------------\n");

		// multi thread test
		TestMultiThread();
	}
	// multi thrad test for win32 vs PPL
	//TestMultiThread_win32_vs_ppl();

	wprintf_s(L"-----------------------------------------------------------------------\n\n\n");
	//
	// arithmetic test
	wprintf_s(L"<Arithmetic Test> -----------------------------------------------------\n");
	BOOL bIsHybridCpuSupported = IsHybridCpuSupported();

	HANDLE hProcess = GetCurrentProcess();

	DWORD	dwPhysicalCoreCount = 0;
	DWORD	dwLogicalCoreCount = 0;
	GetPhysicalCoreCount(&dwPhysicalCoreCount, &dwLogicalCoreCount);

	DWORD_PTR	PCoreMaskList[128] = {};
	ULONG PCoreCpuIdList[128] = {};
	DWORD_PTR	ECoreMaskList[128] = {};
	ULONG ECoreCpuIdList[128] = {};
	DWORD_PTR	AllCoreMaskList[128] = {};
	ULONG AllCoreCpuIdList[128] = {};


	ULONG ulPCoreCount = GetHybridCoreCpuIDSet(hProcess, PCoreCpuIdList, PCoreMaskList, (DWORD)_countof(PCoreCpuIdList), HYBRID_CORE_TYPE_PCORE, HT_PHYSICAL_CORE);
	ULONG ulECoreCount = GetHybridCoreCpuIDSet(hProcess, ECoreCpuIdList, ECoreMaskList, (DWORD)_countof(ECoreCpuIdList), HYBRID_CORE_TYPE_ECORE, HT_PHYSICAL_CORE);
	ULONG ulAllCoreCount = GetHybridCoreCpuIDSet(hProcess, AllCoreCpuIdList, AllCoreMaskList, (DWORD)_countof(AllCoreCpuIdList), HYBRID_CORE_TYPE_ALL, HT_PHYSICAL_CORE);

	if (0 == ulPCoreCount || 0 == ulECoreCount)
	{
		bIsHybridCpuSupported = FALSE;
	}
	if (bIsHybridCpuSupported)
	{
		wprintf_s(L"<<Hybrid CPU System>>\n");
		wprintf_s(L"---------------------------------------------------------------------------------\n");

		wprintf_s(L"[E-Core]\n");
		TestArithmetic(ECoreMaskList[1]);	// siver코어의 두 번째.인터럽트 방해를 덜 받기 위해

		wprintf_s(L"---------------------------------------------------------------------------------\n");

		wprintf_s(L"-P-Core:\n");
		TestArithmetic(PCoreMaskList[1]);	// gold코어의 두 번째.인터럽트 방해를 덜 받기 위해

		wprintf_s(L"---------------------------------------------------------------------------------\n");
	}
	else
	{
		wprintf_s(L"Non-Hybrid CPU System.\n");
		wprintf_s(L"---------------------------------------------------------------------------------\n");

		TestArithmetic(AllCoreMaskList[1]);	// 일반 코어의 두 번째. 인터럽트 방해를 덜 받기 위해
		
		wprintf_s(L"---------------------------------------------------------------------------------\n");
	}
	wprintf_s(L"-----------------------------------------------------------------------\n\n\n");
	//

	wprintf_s(L"Test completed.");
	//wprintf_s(L"Test completed. Press any key.\n");
	//_getch();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	
}