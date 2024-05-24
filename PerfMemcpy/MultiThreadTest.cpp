#include "stdafx.h"
#include <process.h>
#include "MultiThreadTest.h"
#include "ProcessorInfo.h"
#include "QueryPerfCounter.h"


enum WORKER_EVENT_TYPE
{
	WORKER_EVENT_TYPE_MEMCPY,
	WORKER_EVENT_TYPE_DESTROY
};
#define WORKER_EVENT_TYPE_NUM	2

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

HANDLE g_hCompleteEvent = nullptr;

const DWORD MAX_THREAD_NUM = 128;

// internal functions
UINT WINAPI WorkerThread(LPVOID lpVoid);
void InitWorkerThread(DWORD dwLogicalCoreCount);
void DestroyThread();
void MemcpyThread(DWORD dwThreadIndex);
void InitMemory(char* pSrc, char* pDest, size_t Size);

// Test functions
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

	InitWorkerThread(dwWorkerThreadNum);

	// memcpy() Test
	//const size_t CpyMemSize = 1024ull * 1024ull * 1024ull * 2;	// 2GB
	const size_t CpyMemSize = 1024ull * 1024ull * 1024ull * 1;		// 1GB
	//const size_t CpyMemSize = 1024ull * 1024ull * 512ull;			// 512MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 256ull;			// 256MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 128ull;			// 128MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 64ull;			// 64MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 32ull;			// 32MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 16ull;			// 16MB
	//const size_t CpyMemSize = 1024ull * 1024ull * 8ull;			// 8MB

	TestMemcpy(CpyMemSize);
		
	DestroyThread();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
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

	size_t	AlignSize = 32;

	SizePerThread = ((SizePerThread / AlignSize) + ((SizePerThread % AlignSize) != 0)) * AlignSize;
	MemSize = SizePerThread * g_dwWorkerThreadNum;

	g_pMemDescList = new MEMCPY_DESC[g_dwWorkerThreadNum];
	memset(g_pMemDescList, 0, sizeof(MEMCPY_DESC) * g_dwWorkerThreadNum);

	
	char*	pSrc = (char*)_aligned_malloc(MemSize, AlignSize);
	char*	pDest = (char*)_aligned_malloc(MemSize, AlignSize);

	InitMemory(pSrc, pDest, MemSize);

	wprintf_s(L"\tCompleted..\n\n");

	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();
	ULONGLONG PrvTick = GetTickCount64();

	memcpy(pDest, pSrc, MemSize);
	//MemCpy_1Byte(pDest, pSrc, MemSize);
	//MemCpy_4Bytes(pDest, pSrc, MemSize);
	//MemCpy_8Bytes(pDest, pSrc, MemSize);
	//MemCpy_16Bytes(pDest, pSrc, MemSize);
	//MemCpy_32Bytes(pDest, pSrc, MemSize);


	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	ULONGLONG ElapsedTick = GetTickCount64() - PrvTick;

#ifdef _DEBUG
	if (memcmp(pSrc, pDest, MemSize))
		__debugbreak();
#endif
	DWORD dwSizeInMB = (DWORD)(MemSize / (1024 * 1024));
	wprintf_s(L"\tSingle Thread - (%u) MiB Copied. %.4f ms elapsed. %I64u clocks.\n\n", dwSizeInMB, fElapsedTick, ElapsedClocks);

	DWORD	dwActiveWorkerThreadNum = g_dwWorkerThreadNum;
	while (dwActiveWorkerThreadNum)
	{
		SizePerThread = MemSize / dwActiveWorkerThreadNum;

		if ((SizePerThread % AlignSize) != 0)
			goto lb_next;

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

		// memcpy is successfull?
	#ifdef _DEBUG
		if (memcmp(pSrc, pDest, MemSize))
			__debugbreak();
	#endif
	lb_next:

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
		_aligned_free(pSrc);
		pSrc = nullptr;
	}
	if (pDest)
	{
		_aligned_free(pDest);
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

void MemcpyThread(DWORD dwThreadIndex)
{
	BOOL	bCompleted = FALSE;

	char* pSrc = g_pMemDescList[dwThreadIndex].pSrc;
	char* pDest = g_pMemDescList[dwThreadIndex].pDest;
	size_t Size = g_pMemDescList[dwThreadIndex].Size;

	// actually copy memory
	//memcpy(pDest, pSrc, Size);
	//MemCpy_8Bytes(pDest, pSrc, Size);
	//MemCpy_16Bytes(pDest, pSrc, Size);
	MemCpy_32Bytes(pDest, pSrc, Size);

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
