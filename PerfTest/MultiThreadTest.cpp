#include "stdafx.h"
#include "MultiThreadTest.h"
#include <process.h>
#include "ProcessorInfo.h"
#include "QueryPerfCounter.h"
#include "SpinLock.h"

// 멀티스레드 _InterlockedInc()
// 멀티스레드 AcquireSpinLock()
// 멀티스레드 SRW_Lock
// 멀티스레드 CriticalSection
// 멀티스레드 SetEvent() - WaitFor..()에서 깨어날때까지 걸리는 시간
// 멀티스레드 memcpy

enum WORKER_EVENT_TYPE
{
	WORKER_EVENT_TYPE_INC_ATOMIC,
	WORKER_EVENT_TYPE_INC_SPINLOCK,
	WORKER_EVENT_TYPE_INC_SRWLOCK,
	WORKER_EVENT_TYPE_INC_CRITICAL_SECTION,
	WORKER_EVENT_TYPE_AWAKE_NEXT_THREAD,
	WORKER_EVENT_TYPE_MEMCPY,
	WORKER_EVENT_TYPE_DESTROY
};
#define WORKER_EVENT_TYPE_NUM	7

struct WORKER_THREAD_DESC
{
	HANDLE	hThread;
	HANDLE	hEventList[WORKER_EVENT_TYPE_NUM];
	DWORD	dwThreadIndex;
};
struct MEMCPY_DESC
{
	char* pDest;
	char* pSrc;
	size_t	Size;
};
WORKER_THREAD_DESC*	g_pThreadDescList = nullptr;
MEMCPY_DESC*	g_pMemDescList = nullptr;

DWORD	g_dwWorkerThreadNum = 0;
DWORD	g_dwActiveWorkerThreadNum = 0;
LONG volatile g_lCurCount = 0;
LONG volatile g_lGoalCount = 0;
LONG volatile g_lCompleted = 0;

CRITICAL_SECTION g_CS = {};
SRWLOCK	g_srwLock = {};
LONG volatile g_lSpinLock = 0;

HANDLE g_hCompleteEvent = nullptr;

const DWORD MAX_THREAD_NUM = 128;

// internal functions
UINT WINAPI WorkerThread(LPVOID lpVoid);
void InitWorkerThread(DWORD dwLogicalCoreCount);
void DestroyThread();
void IncAtomiByThread();
void IncCriticalSectionByThread();
void IncSRWLockByThread();
void IncSpinLockByThread();
void IncAwakeThreadByThread(DWORD dwThreadIndex);
void MemcpyThread(DWORD dwThreadIndex);
void InitMemory(char* pSrc, char* pDest, size_t Size);

// Test functions
void TestInc_Atomic(LONG lGloalCount);
void TestInc_CriticalSection(LONG lGloalCount);
void TestInc_SRWLock(LONG lGloalCount);
void TestInc_SpinLock(LONG lGloalCount);
void TestInc_AwakeNextThread(LONG lGloalCount);
void TestInc_AwakeNextThread_PPL(LONG lGloalCount);
void TestMemcpy(size_t MemSize);

void TestMultiThread()
{
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	DWORD	dwPhysicalCoreCount = 0;
	DWORD	dwLogicalCoreCount = 0;
	GetPhysicalCoreCount(&dwPhysicalCoreCount, &dwLogicalCoreCount);

	DWORD	dwWorkerThreadNum = dwLogicalCoreCount;
	if (dwWorkerThreadNum > MAX_THREAD_NUM)
	{
		dwWorkerThreadNum = MAX_THREAD_NUM;
	}
	//dwWorkerThreadNum = ((dwWorkerThreadNum / 6) + ((dwWorkerThreadNum % 6) != 0)) * 6;

	InitWorkerThread(dwWorkerThreadNum);
	//InitWorkerThread(1);

	// MemCpy Test
	//const size_t CpyMemSize = 1024ull * 1024ull * 1024ull * 2;	// 2GB
	const size_t CpyMemSize = 1024ull * 1024ull * 1024ull * 1;	// 1GB
	//const size_t CpyMemSize = 1024ull * 1024ull * 512ull;			// 512MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 256ull;			// 256MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 128ull;			// 128MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 64ull;			// 64MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 32ull;			// 32MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 16ull;			// 16MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 8ull;			// 8MB

	TestMemcpy(CpyMemSize);
	
	LONG lGloalValue = INT_MAX / 8;
	
	// Test Increment - Atomic
	TestInc_Atomic(lGloalValue);

	// Test Increment - CriticalSection
	TestInc_CriticalSection(lGloalValue);

	// Test Increment - SRWLock
	TestInc_SRWLock(lGloalValue);

	// Test Increment - SpinLock
	TestInc_SpinLock(lGloalValue);

	// Test Increment - Awak Next Thread
	lGloalValue = INT_MAX / 1000;
	TestInc_AwakeNextThread(lGloalValue);
	
	DestroyThread();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}
void TestMultiThread_win32_vs_ppl()
{
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	DWORD	dwPhysicalCoreCount = 0;
	DWORD	dwLogicalCoreCount = 0;
	GetPhysicalCoreCount(&dwPhysicalCoreCount, &dwLogicalCoreCount);

	DWORD	dwWorkerThreadNum = dwLogicalCoreCount;
	if (dwWorkerThreadNum > MAX_THREAD_NUM)
	{
		dwWorkerThreadNum = MAX_THREAD_NUM;
	}
	//dwWorkerThreadNum = ((dwWorkerThreadNum / 6) + ((dwWorkerThreadNum % 6) != 0)) * 6;

	InitWorkerThread(dwWorkerThreadNum);
	//InitWorkerThread(1);
	
	LONG lGloalValue = INT_MAX / 8;
	
	// Test Increment - Awak Next Thread
	lGloalValue = INT_MAX / 1000;
	TestInc_AwakeNextThread(lGloalValue);

	// Test Increment - Awak Next Thread with PPL
	TestInc_AwakeNextThread_PPL(lGloalValue);
	
	DestroyThread();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}
void TestInc_Atomic(LONG lGloalCount)
{
	wprintf_s(L"Multi-Threads Increament(Atomic)\n");

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = lGloalCount;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		LARGE_INTEGER PrvCounter = QCGetCounter();

		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_INC_ATOMIC]);
		}
		while (1)
		{
			LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
			if (lCurCount >= g_lGoalCount)
			{
				break;
			}
			wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
			Sleep(5000);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

		dwActiveWorkerThreadNum /= 2;
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");
}

void TestInc_CriticalSection(LONG lGloalCount)
{
	InitializeCriticalSection(&g_CS);

	wprintf_s(L"Multi-Threads Increament(CriticalSection)\n");

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = lGloalCount;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		LARGE_INTEGER PrvCounter = QCGetCounter();

		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_INC_CRITICAL_SECTION]);
		}
		while (1)
		{
			LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
			if (lCurCount >= g_lGoalCount)
			{
				break;
			}
			wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
			Sleep(5000);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

		dwActiveWorkerThreadNum /= 2;
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");

	DeleteCriticalSection(&g_CS);
	g_CS = {};
}

void TestInc_SRWLock(LONG lGloalCount)
{
	InitializeSRWLock(&g_srwLock);

	wprintf_s(L"Multi-Threads Increament(SRWLock)\n");

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = lGloalCount;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		LARGE_INTEGER PrvCounter = QCGetCounter();

		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_INC_SRWLOCK]);
		}
		while (1)
		{
			LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
			if (lCurCount >= g_lGoalCount)
			{
				break;
			}
			wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
			Sleep(5000);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

		dwActiveWorkerThreadNum /= 2;
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");
}

void TestInc_SpinLock(LONG lGloalCount)
{
	wprintf_s(L"Multi-Threads Increament(SpinLock)\n");

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = lGloalCount;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		LARGE_INTEGER PrvCounter = QCGetCounter();

		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_INC_SPINLOCK]);
		}
		while (1)
		{
			LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
			if (lCurCount >= g_lGoalCount)
			{
				break;
			}
			wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
			Sleep(5000);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

		dwActiveWorkerThreadNum /= 2;
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");
}
void TestInc_AwakeNextThread(LONG lGloalCount)
{
	wprintf_s(L"Multi-Threads Increament(Awake Next Thread)\n");

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = lGloalCount;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		LARGE_INTEGER PrvCounter = QCGetCounter();

		SetEvent(g_pThreadDescList[0].hEventList[WORKER_EVENT_TYPE_AWAKE_NEXT_THREAD]);

		while (1)
		{
			LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
			if (lCurCount >= g_lGoalCount)
			{
				break;
			}
			wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
			Sleep(5000);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

		dwActiveWorkerThreadNum /= 2;
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");
}
#include <ppl.h>
#include <ppltasks.h>

using namespace concurrency;

BOOL IncAwakeThreadByThread_PPL()
{
	BOOL	bCompleted = FALSE;

	LONG lCount = _InterlockedIncrement(&g_lCurCount);
	if (lCount > g_lGoalCount)
	{
		_InterlockedDecrement(&g_lCurCount);
		bCompleted = TRUE;
	}

	if (bCompleted)
	{
		LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
		if (!lOldValue)
		{
			SetEvent(g_hCompleteEvent);
		}
	}
	else
	{
		create_task([]() 
		{
			IncAwakeThreadByThread_PPL();
		});
	}
	return bCompleted;
}
void TestInc_AwakeNextThread_PPL(LONG lGloalCount)
{
	wprintf_s(L"Multi-Threads Increament(Awake Next Thread - PPL)\n");

	g_lCurCount = 0;
	g_lGoalCount = lGloalCount;
	g_lCompleted = 0;

	LARGE_INTEGER PrvCounter = QCGetCounter();

	create_task([]() 
	{
		IncAwakeThreadByThread_PPL();
	});
	while (1)
	{
		LONG lCurCount = _InterlockedOr(&g_lCurCount, 0);
		if (lCurCount >= g_lGoalCount)
		{
			break;
		}
		wprintf_s(L"\t%d / %d\n", g_lCurCount, g_lGoalCount);
		Sleep(5000);
	}
	WaitForSingleObject(g_hCompleteEvent, INFINITE);

	float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"\t(%d) / (%d) : %.4f ms elapsed.\n", g_lCurCount, g_lGoalCount, fElapsedTick);

	wprintf_s(L"\n");
}
void InitMemory(char* pSrc, char* pDest, size_t Size)
{
	memset(pDest, 0, Size);

	DWORD* pSrcEntry = (DWORD*)pSrc;
	for (DWORD i = 0; i < Size / 4; i++)
	{
		*pSrcEntry = 0xff00ff00;
		pSrcEntry++;
	}
}
void TestMemcpy(size_t MemSize)
{
	LARGE_INTEGER PrvCounter = {};
	float fElapsedTick = 0.0f;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	wprintf_s(L"Multi-Threads Memcpy\n");

	wprintf_s(L"\tInitializeing Memory...\n");

	size_t SizePerThread = MemSize / g_dwWorkerThreadNum;

	SizePerThread = ((SizePerThread / 4) + ((SizePerThread % 4) != 0)) * 4;
	MemSize = SizePerThread * g_dwWorkerThreadNum;

	g_pMemDescList = new MEMCPY_DESC[g_dwWorkerThreadNum];
	memset(g_pMemDescList, 0, sizeof(MEMCPY_DESC) * g_dwWorkerThreadNum);

	char*	pSrc = (char*)malloc(MemSize);
	char*	pDest = (char*)malloc(MemSize);

	InitMemory(pSrc, pDest, MemSize);

	wprintf_s(L"\tCompleted..\n\n");

	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	memcpy(pDest, pSrc, MemSize);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	DWORD dwSizeInMB = (DWORD)(MemSize / (1024 * 1024));
	wprintf_s(L"\tSingled Thread - (%u) MiB Copied. %.4f ms elapsed. %I64u clocks.\n\n", dwSizeInMB, fElapsedTick, ElapsedClocks);

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		SizePerThread = MemSize / dwActiveWorkerThreadNum;
		char*	pSrcEntry = pSrc;
		char*	pDestEntry = pDest;
		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			g_pMemDescList[i].pSrc = pSrcEntry;
			g_pMemDescList[i].pDest = pDestEntry;
			g_pMemDescList[i].Size = SizePerThread;

			pSrcEntry += SizePerThread;
			pDestEntry += SizePerThread;
		}
		g_dwActiveWorkerThreadNum = dwActiveWorkerThreadNum;
		g_lCurCount = 0;
		g_lGoalCount = dwActiveWorkerThreadNum;
		g_lCompleted = 0;

		wprintf_s(L"\t[%u threads]\n", dwActiveWorkerThreadNum);

		PrvCounter = QCGetCounter();

		for (DWORD i = 0; i < dwActiveWorkerThreadNum; i++)
		{
			SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_MEMCPY]);
		}
		WaitForSingleObject(g_hCompleteEvent, INFINITE);

		fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
		wprintf_s(L"\t(%u) MiB Copied. %.4f ms elapsed.\n", dwSizeInMB, fElapsedTick);

		dwActiveWorkerThreadNum--;
		wprintf_s(L"\n");
	}

	if (g_pMemDescList)
	{
		delete g_pMemDescList;
		g_pMemDescList = nullptr;
	}
	if (pSrc)
	{
		free(pSrc);
		pSrc = nullptr;
	}
	if (pDest)
	{
		free(pDest);
		pDest = nullptr;
	}
	wprintf_s(L"\n");
}
void InitWorkerThread(DWORD dwWorkerThreadNum)
{
	g_dwWorkerThreadNum = dwWorkerThreadNum;
	g_pThreadDescList = new WORKER_THREAD_DESC[g_dwWorkerThreadNum];
	memset(g_pThreadDescList, 0, sizeof(WORKER_THREAD_DESC) * g_dwWorkerThreadNum);

	g_hCompleteEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	for (DWORD i = 0; i < g_dwWorkerThreadNum; i++)
	{
		g_pThreadDescList[i].dwThreadIndex = i;
		for (DWORD j = 0; j < WORKER_EVENT_TYPE_NUM; j++)
		{
			g_pThreadDescList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);;
		}
		UINT uiThreadID = 0;
		g_pThreadDescList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, g_pThreadDescList + i, 0, &uiThreadID);
	}
}

void IncAtomiByThread()
{
	while (1)
	{
		LONG lCount = _InterlockedIncrement(&g_lCurCount);
		if (lCount > g_lGoalCount)
		{
			_InterlockedDecrement(&g_lCurCount);
			break;
		}
	}

	LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
	if (!lOldValue)
	{
		SetEvent(g_hCompleteEvent);
	}
}
void IncCriticalSectionByThread()
{
	BOOL	bCompleted = FALSE;
	while (!bCompleted)
	{
		EnterCriticalSection(&g_CS);
		if (g_lCurCount < g_lGoalCount)
		{
			g_lCurCount++;
		}
		else
		{
			bCompleted = TRUE;
		}
		LeaveCriticalSection(&g_CS);
	}

	LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
	if (!lOldValue)
	{
		SetEvent(g_hCompleteEvent);
	}
}

void IncSRWLockByThread()
{
	BOOL	bCompleted = FALSE;
	while (!bCompleted)
	{
		AcquireSRWLockExclusive(&g_srwLock);
		if (g_lCurCount < g_lGoalCount)
		{
			g_lCurCount++;
		}
		else
		{
			bCompleted = TRUE;
		}
		ReleaseSRWLockExclusive(&g_srwLock);
	}

	LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
	if (!lOldValue)
	{
		SetEvent(g_hCompleteEvent);
	}
}
void IncSpinLockByThread()
{
	BOOL	bCompleted = FALSE;
	while (!bCompleted)
	{
		AcquireSpinLock(&g_lSpinLock);
		if (g_lCurCount < g_lGoalCount)
		{
			g_lCurCount++;
		}
		else
		{
			bCompleted = TRUE;
		}
		ReleaseSpinLock(&g_lSpinLock);
	}

	LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
	if (!lOldValue)
	{
		SetEvent(g_hCompleteEvent);
	}
}
void IncAwakeThreadByThread(DWORD dwThreadIndex)
{
	BOOL	bCompleted = FALSE;

	LONG lCount = _InterlockedIncrement(&g_lCurCount);
	if (lCount > g_lGoalCount)
	{
		_InterlockedDecrement(&g_lCurCount);
		bCompleted = TRUE;
	}

	if (bCompleted)
	{
		LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
		if (!lOldValue)
		{
			SetEvent(g_hCompleteEvent);
		}
	}
	else
	{
		DWORD dwNextThreadIndex = (dwThreadIndex + 1) % g_dwActiveWorkerThreadNum;
		SetEvent(g_pThreadDescList[dwNextThreadIndex].hEventList[WORKER_EVENT_TYPE_AWAKE_NEXT_THREAD]);
	}
}
void MemcpyThread(DWORD dwThreadIndex)
{
	BOOL	bCompleted = FALSE;

	char* pSrc = g_pMemDescList[dwThreadIndex].pSrc;
	char* pDest = g_pMemDescList[dwThreadIndex].pDest;
	size_t Size = g_pMemDescList[dwThreadIndex].Size;

	memcpy(pDest, pSrc, Size);

	LONG lCount = _InterlockedIncrement(&g_lCurCount);
	if (lCount >= g_lGoalCount)
	{
		bCompleted = TRUE;
	}

	if (bCompleted)
	{
		LONG lOldValue = _InterlockedCompareExchange(&g_lCompleted, 1, 0);
		if (!lOldValue)
		{
			SetEvent(g_hCompleteEvent);
		}
	}
}
void DestroyThread()
{
	for (DWORD i = 0; i < g_dwWorkerThreadNum; i++)
	{
		SetEvent(g_pThreadDescList[i].hEventList[WORKER_EVENT_TYPE_DESTROY]);
		WaitForSingleObject(g_pThreadDescList[i].hThread, INFINITE);
		CloseHandle(g_pThreadDescList[i].hThread);
		g_pThreadDescList[i].hThread = nullptr;

		for (DWORD j = 0; j < WORKER_EVENT_TYPE_NUM; j++)
		{
			CloseHandle(g_pThreadDescList[i].hEventList[j]);
			g_pThreadDescList[i].hEventList[j] = nullptr;
		}
	}
	CloseHandle(g_hCompleteEvent);
	g_hCompleteEvent = nullptr;
}

UINT WINAPI WorkerThread(LPVOID lpVoid)
{
	WORKER_THREAD_DESC*	pDesc = (WORKER_THREAD_DESC*)lpVoid;
	DWORD	dwThreadIndex = pDesc->dwThreadIndex;

	while (1)
	{
		DWORD	dwEventIndex = WaitForMultipleObjects(WORKER_EVENT_TYPE_NUM, pDesc->hEventList, FALSE, INFINITE);

		switch (dwEventIndex)
		{
			case WORKER_EVENT_TYPE_INC_ATOMIC:
				IncAtomiByThread();
				break;
			case WORKER_EVENT_TYPE_INC_SPINLOCK:
				IncSpinLockByThread();
				break;
			case WORKER_EVENT_TYPE_INC_SRWLOCK:
				IncSRWLockByThread();
				break;
			case WORKER_EVENT_TYPE_INC_CRITICAL_SECTION:
				IncCriticalSectionByThread();
				break;
			case WORKER_EVENT_TYPE_AWAKE_NEXT_THREAD:
				IncAwakeThreadByThread(dwThreadIndex);
				break;
			case WORKER_EVENT_TYPE_MEMCPY:
				MemcpyThread(dwThreadIndex);
				break;
			case WORKER_EVENT_TYPE_DESTROY:
				goto lb_return;

		}
	}
lb_return:

	_endthreadex(0);
	return 0;
}
