// PerfTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>
#include "QueryPerfCounter.h"
#include "ProcessorInfo.h"
#include <conio.h>
#include "ArithmeticTest.h"
#include "MathFunc.h"

#ifdef _DEBUG
//	#define USE_DEBUG_COMMON_DLL
#endif

#define QUALCOMM_8CX

int main()
{
#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif
	size_t size = sizeof(MATRIX4);
	QCInit();

	srand(3);

	const DWORD VECTOR_NUM = 10000;
	const DWORD REPEAT_COUNT = 100;

	/*-		PCoreMaskList	0x000000f53c19df20 {1, 4, 16, 64, 256, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...}	unsigned __int64[128]
		[0]	1	unsigned __int64
		[1]	4	unsigned __int64
		[2]	16	unsigned __int64
		[3]	64	unsigned __int64
		[4]	256	unsigned __int64
		[5]	1024	unsigned __int64*/


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
		TestArithmetic(ECoreMaskList[1], VECTOR_NUM);	// siver코어의 두 번째.인터럽트 방해를 덜 받기 위해

		wprintf_s(L"---------------------------------------------------------------------------------\n");

		wprintf_s(L"-P-Core:\n");
		TestArithmetic(PCoreMaskList[1], VECTOR_NUM);	// gold코어의 두 번째.인터럽트 방해를 덜 받기 위해

		wprintf_s(L"---------------------------------------------------------------------------------\n");
	}
	else
	{
		wprintf_s(L"Non-Hybrid CPU System.\n");
		wprintf_s(L"---------------------------------------------------------------------------------\n");

		TestArithmetic(AllCoreMaskList[1], VECTOR_NUM);	// 일반 코어의 두 번째. 인터럽트 방해를 덜 받기 위해
		
		wprintf_s(L"---------------------------------------------------------------------------------\n");
	}
	wprintf_s(L"All testing completed.\n");
	//wprintf_s(L"All testing completed.\n");
	//_getch();
#ifdef _DEBUG	
	_ASSERT(_CrtCheckMemory());
#endif
	

}
