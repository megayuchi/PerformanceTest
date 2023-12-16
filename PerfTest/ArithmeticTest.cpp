#include "stdafx.h"
#include "SIMD_MATH.h"
#include "ArithmeticTest.h"
#include <stdio.h>
#include "Util.h"

void TestAdd(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum);
void TestMul(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum);
void TestDiv(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum);
void TestAddOrSubOrMulOrDiv(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum);
void TestAddOrSubOrMulOrDiv_Random(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum);


#if !defined(_DEBUG)
#pragma optimize("",off)
#endif

void TestArithmetic(DWORD_PTR ThreadAffinityMask)
{
	wprintf_s(L"\n<Begin with Core mask: 0x%llx>\n\n", ThreadAffinityMask);

	HANDLE hThread = GetCurrentThread();
	DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hThread, ThreadAffinityMask);

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	size_t size = sizeof(VECTOR4_SIMD);

	VECTOR4_SIMD v0,v1;
	v0.Set(1.0f, 3.0f, 5.0f, 7.0f);
	v1.Set(2.0f, 4.0f, 6.0f, 8.0f);

	VECTOR4_SIMD r = v0 + v1;
	r = v0 * v1;
	r = v0 - v1;
	r = v0 / v1;
	int a = 0;

	//const DWORD VECTOR_NUM = 1000000;
	const DWORD VECTOR_NUM = 10000;
	VECTOR4_SIMD* pSrc0 = (VECTOR4_SIMD*)_aligned_malloc(sizeof(VECTOR4_SIMD)*VECTOR_NUM, 16);
	VECTOR4_SIMD* pSrc1 = (VECTOR4_SIMD*)_aligned_malloc(sizeof(VECTOR4_SIMD)*VECTOR_NUM, 16);
	VECTOR4_SIMD* pDest = (VECTOR4_SIMD*)_aligned_malloc(sizeof(VECTOR4_SIMD)*VECTOR_NUM, 16);
	memset(pDest, 0, sizeof(VECTOR4_SIMD) * VECTOR_NUM);

	FillVectorStream(pSrc0, VECTOR_NUM);
	FillVectorStream(pSrc1, VECTOR_NUM);

	TestAdd(pDest, pSrc0, pSrc1, VECTOR_NUM);
	TestMul(pDest, pSrc0, pSrc1, VECTOR_NUM);
	TestDiv(pDest, pSrc0, pSrc1, VECTOR_NUM);
	TestAddOrSubOrMulOrDiv(pDest, pSrc0, pSrc1, VECTOR_NUM);
	TestAddOrSubOrMulOrDiv_Random(pDest, pSrc0, pSrc1, VECTOR_NUM);

	OldThreadAffinityMask = SetThreadAffinityMask(hThread, OldThreadAffinityMask);

	if (pSrc0)
	{
		_aligned_free(pSrc0);
		pSrc0 = nullptr;
	}
	if (pSrc1)
	{
		_aligned_free(pSrc1);
		pSrc1 = nullptr;
	}
	if (pDest)
	{
		_aligned_free(pDest);
		pDest = nullptr;
	}

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}
void TestAdd(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum)
{
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0.0f;
	
	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	//
	// Add
	//
	wprintf_s(L"[Add]\n");

	// Test Normal - AddVectorStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddVectorStream((VECTOR4*)pDest, (VECTOR4*)pSrc0, (VECTOR4*)pSrc1, dwVectorNum);
	
	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	
	wprintf_s(L"\tAddVectroStream_Normal(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n\n");

	// Test SIMD - AddVectorStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddVectorStream_SIMD(pDest, pSrc0, pSrc1, dwVectorNum);
	
	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tAddVectroStream_SIMD(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n");
}
void TestMul(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum)
{
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0.0f;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	//
	// Mul
	//
	wprintf_s(L"[Mul]\n");

	// Test Normal - MulVectorStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	MulVectorStream((VECTOR4*)pDest, (VECTOR4*)pSrc0, (VECTOR4*)pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tMulVectroStream_Normal(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n\n");

	// Test SIMD - MulStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	MulVectorStream_SIMD(pDest, pSrc0, pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tMulVectroStream_SIMD(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n");
}

void TestDiv(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum)
{
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0.0f;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	//
	// Div
	//
	wprintf_s(L"[Div]\n");

	// Test Normal - DivVectorStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	DivVectorStream((VECTOR4*)pDest, (VECTOR4*)pSrc0, (VECTOR4*)pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tDivVectroStream_Normal(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n\n");

	// Test SIMD - DivStream(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	DivVectorStream_SIMD(pDest, pSrc0, pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tDivVectroStream_SIMD(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n");
}

void TestAddOrSubOrMulOrDiv(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum)
{
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0.0f;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	//
	// Add or Sub or Mul or Div;
	//
	wprintf_s(L"[Add or Sub or Mul or Div]\n");

	// Test Normal - AddOrSubOrMulOrDiv(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddOrSubOrMulOrDivVectorStream((VECTOR4*)pDest, (VECTOR4*)pSrc0, (VECTOR4*)pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tAddOrSubOrMulOrDivVectorStream_Normal(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n\n");

	// Test SIMD - AddOrSubOrMulOrDiv_SIMD(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddOrSubOrMulOrDivVectorStream_SIMD(pDest, pSrc0, pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tAddOrSubOrMulOrDivVectorStream_SIMD(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n");
}

void TestAddOrSubOrMulOrDiv_Random(VECTOR4_SIMD* pDest, VECTOR4_SIMD* pSrc0, VECTOR4_SIMD* pSrc1, DWORD dwVectorNum)
{
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0.0f;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClocks = 0;

	//
	// Add or Sub or Mul or Div with random;
	//
	wprintf_s(L"[Add or Sub or Mul or Div with Random, broken cache locality]\n");

	// Test Normal - AddOrSubOrMulOrDivVectorStream_Random(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddOrSubOrMulOrDivVectorStream_Random((VECTOR4*)pDest, (VECTOR4*)pSrc0, (VECTOR4*)pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tAddOrSubOrMulOrDivVectorStream_Normal(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n\n");

	// Test SIMD - AddOrSubOrMulOrDivVectorStream_Random_SIMD(); 
	PrvCounter = QCGetCounter();
	PrvClock = GetClockCounter();

	AddOrSubOrMulOrDivVectorStream_Random_SIMD(pDest, pSrc0, pSrc1, dwVectorNum);

	ElapsedClocks = GetClockCounter() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);


	wprintf_s(L"\tAddOrSubOrMulOrDivVectorStream_SIMD(%u) , Elapsed tick : %.4f ms. Avg Clocks Per Operation : %.2f clocks.\n", dwVectorNum, fElapsedTick, (float)ElapsedClocks / (float)dwVectorNum);
	wprintf_s(L"\tpDest[%u] = (%.2f, %.2f, %.2f), pDest[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pDest[0].x, pDest[0].y, pDest[0].z,
			  dwVectorNum - 1, pDest[dwVectorNum - 1].x, pDest[dwVectorNum - 1].y, pDest[dwVectorNum - 1].z);

	wprintf_s(L"\n");
}


#if !defined(_DEBUG)
#pragma optimize("",on)
#endif
