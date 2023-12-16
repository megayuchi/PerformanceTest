#include "stdafx.h"
#include "ArithmeticTest.h"
#include "QueryPerfCounter.h"
#include "MathFunc.h"
#include <conio.h>

//#define ENFORCE_ALIGNED_MEMORY

void FillMatrix(MATRIX4* pOutMat);
void PrintMatrix(MATRIX4* pMat);
void FillVector3(VECTOR3* pv3Out);
void FillVector4(VECTOR4* pv3Out);

void Test_Transpose();
void Test_CalcPlaneEquation();
void Test_CalcDistance(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_DotProduct(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_CrossProduct(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_MatrixMultiply();
void Test_NoSIMDize(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_TransformVector3_VPTR2(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_TransformVector3_VPTR1(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_TransformVector4_VPTR2(VECTOR4* pv4ListDest, VECTOR4* pv4ListSrc, DWORD dwVectorNum);
void Test_VECTOR3Length(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum);
void Test_ClipTriWithPlaneList();

#ifndef _DEBUG
#pragma optimize("",off)
#endif
void TestArithmetic(DWORD_PTR ThreadAffinityMask, DWORD dwVectorNum)
{
	wprintf_s(L"\n<Begin with Core mask: 0x%llx>\n\n", ThreadAffinityMask);
	
	HANDLE hThread = GetCurrentThread();
	DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hThread, ThreadAffinityMask);


	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	VECTOR3* pv3First = nullptr;
	VECTOR3* pv3Last = nullptr;
	VECTOR4* pv4First = nullptr;
	VECTOR4* pv4Last = nullptr;

	VECTOR3*	pv3ListSrc = new VECTOR3[dwVectorNum];
	VECTOR3*	pv3ListDest = new VECTOR3[dwVectorNum];

	memset(pv3ListSrc, 0, sizeof(VECTOR3) * dwVectorNum);
	memset(pv3ListDest, 0, sizeof(VECTOR3) * dwVectorNum);

	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		FillVector3(pv3ListSrc + i);
	}

#if defined(ENFORCE_ALIGNED_MEMORY)
	VECTOR4*	pv4ListSrc = (VECTOR4*)_aligned_malloc(sizeof(VECTOR4) * dwVectorNum, sizeof(VECTOR4));
	VECTOR4*	pv4ListDest = (VECTOR4*)_aligned_malloc(sizeof(VECTOR4) * dwVectorNum, sizeof(VECTOR4));
	if (0 != (DWORD_PTR)pv3ListSrc % sizeof(VECTOR4))
		__debugbreak();
	if (0 != (DWORD_PTR)pv4ListDest % sizeof(VECTOR4))
		__debugbreak();
#else
	VECTOR4*	pv4ListSrc = (VECTOR4*)malloc(sizeof(VECTOR4) * dwVectorNum);
	VECTOR4*	pv4ListDest = (VECTOR4*)malloc(sizeof(VECTOR4) * dwVectorNum);
#endif




	memset(pv4ListSrc, 0, sizeof(VECTOR4) * dwVectorNum);
	memset(pv4ListDest, 0, sizeof(VECTOR4) * dwVectorNum);

	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		FillVector4(pv4ListSrc + i);
	}



	MATRIX4	mat;
	FillMatrix(&mat);



	VECTOR3	v0, v1;
	TransformVector3_VPTR2_NoSIMD(&v0, pv3ListSrc, &mat, 1);
	
#if defined(ARM64)
	TransformVector3_VPTR2_NEON(&v1, pv3ListSrc, &mat, 1);
#else
	TransformVector3_VPTR2_SSE(&v1, pv3ListSrc, &mat, 1);
#endif

	VECTOR4 v4SrcList_0[4] = {};
	VECTOR4 v4SrcList_1[4] = {};
	VECTOR4 v4DestList_0[4] = {};
	VECTOR4 v4DestList_1[4] = {};
	for (DWORD i = 0; i < 4; i++)
	{
		FillVector4(v4SrcList_0 + i);
		FillVector4(v4SrcList_1 + i);
	}
	TransformVector4_VPTR2_NoSIMD(v4DestList_0, v4SrcList_0, &mat, 4);
#if defined(ARM64)
	TransformVector4_VPTR2_NEON(v4DestList_1, v4SrcList_0, &mat, 4);
#else
	TransformVector4_VPTR2_SSE(v4DestList_1, v4SrcList_0, &mat, 4);
#endif

	TransformVector3_VPTR1_NoSIMD(&v0, &mat, 1);
#if defined(ARM64)
	TransformVector3_VPTR1_NEON(&v1, &mat, 1);
#else
	TransformVector3_VPTR1_SSE(&v1, &mat, 1);
#endif


	float len0 = VECTOR3Length_NoSIMD(&v0);
#if defined(ARM64)
	float len1 = VECTOR3Length_NEON(&v0);
#else
	float len1 = VECTOR3Length_SSE(&v0);
#endif


	VECTOR3 r0 = {};
	VECTOR3 r1 = {};
	VECTOR3 r2 = {};

	TRIANGLE tri;
	tri.v3Point[0].Set(-10.0f, 2.0f, 3.0f);
	tri.v3Point[1].Set(1.0f, 10.0f, 2.0f);
	tri.v3Point[2].Set(0.5f, -2.0f, 10.0f);

	PLANE	plane0, plane1;
	//CalcPlaneEquation_NoSIMD(&plane0, (VECTOR3*)&tri);
	CalcPlaneEquationWithPtr_NoSIMD(&plane0, tri.v3Point + 0, tri.v3Point + 1, tri.v3Point + 2);

	//CalcPlaneEquation(&plane1, (VECTOR3*)&tri);
#if defined(ARM64)
	CalcPlaneEquationWithPtr_NEON(&plane1, tri.v3Point + 0, tri.v3Point + 1, tri.v3Point + 2);
#else
	CalcPlaneEquationWithPtr_SSE(&plane1, tri.v3Point + 0, tri.v3Point + 1, tri.v3Point + 2);
#endif

	// Transpose Matrix
	Test_Transpose();

	// CalcPlaneEquation
	Test_CalcPlaneEquation();
	
	// CalcDistance
	Test_CalcDistance(pv3ListDest, pv3ListSrc, dwVectorNum);
	
	// DotProduct
	Test_DotProduct(pv3ListDest, pv3ListSrc, dwVectorNum);

	// Cross Product
	Test_CrossProduct(pv3ListDest, pv3ListSrc, dwVectorNum);
	
	// MatrixMultiply
	Test_MatrixMultiply();

	// Normalize
	Test_NoSIMDize(pv3ListDest, pv3ListSrc, dwVectorNum);

	// TransformVector3_VPTR2
	Test_TransformVector3_VPTR2(pv3ListDest, pv3ListSrc, dwVectorNum);

	// TransformVector3_VPTR1
	Test_TransformVector3_VPTR1(pv3ListDest, pv3ListSrc, dwVectorNum);

	// TransformVector4_VPTR2
	Test_TransformVector4_VPTR2(pv4ListDest, pv4ListSrc, dwVectorNum);

	// test VECTOR3Length
	Test_VECTOR3Length(pv3ListDest, pv3ListSrc, dwVectorNum);

	// ClipTriWithPlaneList
	Test_ClipTriWithPlaneList();


	OldThreadAffinityMask = SetThreadAffinityMask(hThread, OldThreadAffinityMask);

	if (pv3ListSrc)
	{
		delete[] pv3ListSrc;
		pv3ListSrc = nullptr;
	}
	if (pv3ListDest)
	{
		delete[] pv3ListDest;
		pv3ListDest = nullptr;
	}
	if (pv4ListSrc)
	{

	#if defined(ENFORCE_ALIGNED_MEMORY)
		_aligned_free(pv4ListSrc);
	#else
		free(pv4ListSrc);
	#endif
		pv4ListSrc = nullptr;
	}
	if (pv4ListDest)
	{
	#if defined(ENFORCE_ALIGNED_MEMORY)
		_aligned_free(pv4ListDest);
	#else
		free(pv4ListDest);
	#endif
		pv4ListDest = nullptr;
	}


#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}

void Test_Transpose()
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// Transpose Matrix
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	MATRIX4	mat =
	{
		0.0f, 0.11f, 0.21f, 0.31f,
		1.0f, 1.11f, 1.21f, 1.31f,
		2.0f, 2.11f, 2.21f, 2.31f,
		3.0f, 3.11f, 3.21f, 3.31f
	};
	wprintf_s(L"Transpose\n");

	const DWORD TRANSPOSE_REPEAT_COUNT = 10000;

	MATRIX4_A matT_NoSIMD = {};
	MATRIX4_A matT_SSE = {};
	for (DWORD i = 0; i < TRANSPOSE_REPEAT_COUNT; i++)
	{
		Transpose_NoSIMD(&matT_NoSIMD, &mat);
		Transpose_SSE(&matT_SSE, &mat);
	};
	
	// NoSIMD
	for (DWORD i = 0; i < TRANSPOSE_REPEAT_COUNT; i++)
	{
		Transpose_NoSIMD(&matT_NoSIMD, &mat);
	};
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < TRANSPOSE_REPEAT_COUNT; i++)
	{
		Transpose_NoSIMD(&matT_NoSIMD, &mat);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"NoSIMD (%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n , %.2f clocks.\n",
			  matT_SSE._11.m128_f32[0], matT_SSE._11.m128_f32[1], matT_SSE._11.m128_f32[2], matT_SSE._11.m128_f32[3],
			  matT_SSE._21.m128_f32[0], matT_SSE._21.m128_f32[1], matT_SSE._21.m128_f32[2], matT_SSE._21.m128_f32[3],
			  matT_SSE._31.m128_f32[0], matT_SSE._31.m128_f32[1], matT_SSE._31.m128_f32[2], matT_SSE._31.m128_f32[3],
			  matT_SSE._41.m128_f32[0], matT_SSE._41.m128_f32[1], matT_SSE._41.m128_f32[2], matT_SSE._41.m128_f32[3],
			  (float)ElapsedClock / (float)TRANSPOSE_REPEAT_COUNT);
	Sleep(1);

	// SSE
	for (DWORD i = 0; i < TRANSPOSE_REPEAT_COUNT; i++)
	{
		Transpose_SSE(&matT_SSE, &mat);
	};
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < TRANSPOSE_REPEAT_COUNT; i++)
	{
	#if defined(ARM64)
	#else
		Transpose_SSE(&matT_SSE, &mat);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"SIMD (%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n(%.2f, %.2f, %.2f %.2f)\n , %.2f clocks.\n",
			  matT_SSE._11.m128_f32[0], matT_SSE._11.m128_f32[1], matT_SSE._11.m128_f32[2], matT_SSE._11.m128_f32[3],
			  matT_SSE._21.m128_f32[0], matT_SSE._21.m128_f32[1], matT_SSE._21.m128_f32[2], matT_SSE._21.m128_f32[3],
			  matT_SSE._31.m128_f32[0], matT_SSE._31.m128_f32[1], matT_SSE._31.m128_f32[2], matT_SSE._31.m128_f32[3],
			  matT_SSE._41.m128_f32[0], matT_SSE._41.m128_f32[1], matT_SSE._41.m128_f32[2], matT_SSE._41.m128_f32[3],
			  (float)ElapsedClock / (float)TRANSPOSE_REPEAT_COUNT);

	wprintf_s(L"\n");
	Sleep(1);
}
	
void Test_CalcPlaneEquation()
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// CalcPlaneEquation
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	VECTOR3* pv3First = nullptr;
	VECTOR3* pv3Last = nullptr;
	VECTOR4* pv4First = nullptr;
	VECTOR4* pv4Last = nullptr;

	TRIANGLE tri;
	tri.v3Point[0].Set(-10.0f, 2.0f, 3.0f);
	tri.v3Point[1].Set(1.0f, 10.0f, 2.0f);
	tri.v3Point[2].Set(0.5f, -2.0f, 10.0f);

	PLANE	plane0, plane1;

	wprintf_s(L"CalcPlaneEquation\n");

	const DWORD CALC_PLANE_REPEAT_COUNT = 100;

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < CALC_PLANE_REPEAT_COUNT; i++)
	{
		CalcPlaneEquation_NoSIMD(&plane0, (VECTOR3*)&tri);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD (%.2f, %.2f, %.2f - %.2f) , %.2f clocks.\n",
			  plane0.v3Up.x, plane0.v3Up.y, plane0.v3Up.z, plane0.D,
			  (float)ElapsedClock / (float)CALC_PLANE_REPEAT_COUNT);
	Sleep(1);

	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < CALC_PLANE_REPEAT_COUNT; i++)
	{
		// ARM64에선 NoSIMD를 사용하도록 한다.
	#if defined(ARM64)
		CalcPlaneEquation_NEON(&plane1, (VECTOR3*)&tri);
	#else
		CalcPlaneEquation_SSE(&plane1, (VECTOR3*)&tri);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD (%.2f, %.2f, %.2f - %.2f) , %.2f clocks.\n",
			  plane1.v3Up.x, plane1.v3Up.y, plane1.v3Up.z, plane1.D,
			  (float)ElapsedClock / (float)CALC_PLANE_REPEAT_COUNT);

	wprintf_s(L"\n");
	Sleep(1);
}

void Test_CalcDistance(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// CalcDistance
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"CalcDistance\n");

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		pv3ListDest[i].x = CalcDistance_NoSIMD(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD Dist[%u] = (%.2f), Dist[%u] = (%.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x,
			  (float)ElapsedClock / (float)dwVectorNum);

	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		// NEON의 경우 근소하게 SIMD가 느리다. 차이가 크지 않으므로 MathLib에는 NEON코드를 적용했다.
	#if defined(ARM64)
		pv3ListDest[i].x = CalcDistance_NEON(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#else
		pv3ListDest[i].x = CalcDistance_SSE(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD Dist[%u] = (%.2f), Dist[%u] = (%.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x,
			  (float)ElapsedClock / (float)dwVectorNum);

	wprintf_s(L"\n");
	Sleep(1);
}
void Test_DotProduct(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// DotProduct
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"DotProduct\n");

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		pv3ListDest[i].x = DotProduct(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f), Vector[%u] = (%.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x,
			  (float)ElapsedClock / (float)dwVectorNum);

	
	// SSE, NEON모두 NoSIMD보다 느리므로 비교하지 않음.
	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
	#if defined(ARM64)
		pv3ListDest[i].x = DotProduct_NEON(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#else
		pv3ListDest[i].x = DotProduct_SSE(pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD Vector[%u] = (%.2f), Vector[%u] = (%.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x,
			  (float)ElapsedClock / (float)dwVectorNum);
	
	wprintf_s(L"\n");
	Sleep(1);
}
void Test_CrossProduct(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// Cross Product
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"CrossProduct\n");

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		CrossProduct_NoSIMD(pv3ListDest + i, pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x, pv3ListDest->y, pv3ListDest->z,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x, pv3ListDest[dwVectorNum - 1].y, pv3ListDest[dwVectorNum - 1].z,
			  (float)ElapsedClock / (float)dwVectorNum);

	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		// NEON의 경우 No SIMD보다 느림. 따라서 SIMD적용 안함.
	#if defined(ARM64)
		CrossProduct_NEON(pv3ListDest + i, pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#else
		CrossProduct_SSE(pv3ListDest + i, pv3ListSrc + i, pv3ListSrc + dwVectorNum - i - 1);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x, pv3ListDest->y, pv3ListDest->z,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x, pv3ListDest[dwVectorNum - 1].y, pv3ListDest[dwVectorNum - 1].z,
			  (float)ElapsedClock / (float)dwVectorNum);

	wprintf_s(L"\n");
	Sleep(1);
}

void Test_MatrixMultiply()
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// MatrixMultiply
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"MatrixMultiply\n");
	const DWORD MATRIX_MUL_COUNT = 100;

	MATRIX4 m0, m1;
	FillMatrix(&m0);
	FillMatrix(&m1);

	MATRIX4		mr0, mr1;

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < MATRIX_MUL_COUNT; i++)
	{
		MatrixMultiply2_NoSIMD(&mr0, &m0, &m1);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	PrintMatrix(&mr0);
	wprintf_s(L"\tNoSIMD %.2f clocks\n", (float)ElapsedClock / (float)MATRIX_MUL_COUNT);

	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < MATRIX_MUL_COUNT; i++)
	{
	#if defined(ARM64)
		MatrixMultiply2_NEON(&mr1, &m0, &m1);
	#else
		MatrixMultiply2_SSE(&mr1, &m0, &m1);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	PrintMatrix(&mr1);
	wprintf_s(L"\tSIMD, %.2f clocks\n", (float)ElapsedClock / (float)MATRIX_MUL_COUNT);

	wprintf_s(L"\n");
	Sleep(1);
}

void Test_NoSIMDize(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// Normalize
	//
	///////////////////////////////////////////////////////////////////////////////

	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"Normalize\n");

	// NoSIMD
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		Normalize_NoSIMD(pv3ListDest + i, pv3ListSrc + i);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x, pv3ListDest->y, pv3ListDest->z,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x, pv3ListDest[dwVectorNum - 1].y, pv3ListDest[dwVectorNum - 1].z,
			  (float)ElapsedClock / (float)dwVectorNum);

	// SSE
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
	#if defined(ARM64)
		Normalize_NEON(pv3ListDest + i, pv3ListSrc + i);
	#else
		Normalize_SSE(pv3ListDest + i, pv3ListSrc + i);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f) , %.2f clocks.\n",
			  0, pv3ListDest->x, pv3ListDest->y, pv3ListDest->z,
			  dwVectorNum - 1, pv3ListDest[dwVectorNum - 1].x, pv3ListDest[dwVectorNum - 1].y, pv3ListDest[dwVectorNum - 1].z,
			  (float)ElapsedClock / (float)dwVectorNum);

	wprintf_s(L"\n");
	Sleep(1);
}

void Test_TransformVector3_VPTR2(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// TransformVector3_VPTR2
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;
	
	VECTOR3* pv3First = nullptr;
	VECTOR3* pv3Last = nullptr;
	VECTOR4* pv4First = nullptr;
	VECTOR4* pv4Last = nullptr;

	MATRIX4	mat;
	FillMatrix(&mat);

	wprintf_s(L"TransformVector3_VPTR2\n");
	
	// NoSIMD
	TransformVector3_VPTR2_NoSIMD(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

	TransformVector3_VPTR2_NoSIMD(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pv3First->x, pv3First->y, pv3First->z,
			  dwVectorNum - 1, pv3Last->x, pv3Last->y, pv3Last->z);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);

	memset(pv3ListDest, 0, sizeof(VECTOR3) * dwVectorNum);

	// SSE
#if defined(ARM64)
	TransformVector3_VPTR2_NEON(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
#else
	TransformVector3_VPTR2_SSE(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
	//TransformVector3_VPTR2_SSE_1(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
#endif

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

#if defined(ARM64)
	TransformVector3_VPTR2_NEON(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
#else
	TransformVector3_VPTR2_SSE(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
	//TransformVector3_VPTR2_SSE_1(pv3ListDest, pv3ListSrc, &mat, dwVectorNum);
#endif

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pv3First->x, pv3First->y, pv3First->z,
			  dwVectorNum - 1, pv3Last->x, pv3Last->y, pv3Last->z);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);

	wprintf_s(L"\n");
	Sleep(1);
}

void Test_TransformVector3_VPTR1(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// TransformVector3_VPTR1
	//
	///////////////////////////////////////////////////////////////////////////////
	
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;
	
	VECTOR3* pv3First = nullptr;
	VECTOR3* pv3Last = nullptr;

	MATRIX4	mat;
	FillMatrix(&mat);

	wprintf_s(L"TransformVector3_VPTR1\n");

	// NoSIMD
	memcpy(pv3ListDest, pv3ListSrc, sizeof(VECTOR3) * dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

#if defined(ARM64)
	TransformVector3_VPTR1_NEON(pv3ListDest, &mat, dwVectorNum);
#else
	TransformVector3_VPTR1_SSE(pv3ListDest, &mat, dwVectorNum);
#endif

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pv3First->x, pv3First->y, pv3First->z,
			  dwVectorNum - 1, pv3Last->x, pv3Last->y, pv3Last->z);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);

	// SSE
	memcpy(pv3ListDest, pv3ListSrc, sizeof(VECTOR3) * dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

#if defined(ARM64)
	TransformVector3_VPTR1_NEON(pv3ListDest, &mat, dwVectorNum);
#else
	TransformVector3_VPTR1_SSE(pv3ListDest, &mat, dwVectorNum);
#endif

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD Vector[%u] = (%.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f)\n",
			  0, pv3First->x, pv3First->y, pv3First->z,
			  dwVectorNum - 1, pv3Last->x, pv3Last->y, pv3Last->z);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
	wprintf_s(L"\n");
	Sleep(1);
}

void Test_TransformVector4_VPTR2(VECTOR4* pv4ListDest, VECTOR4* pv4ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// TransformVector4_VPTR2
	//
	///////////////////////////////////////////////////////////////////////////////

	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	VECTOR4* pv4First = nullptr;
	VECTOR4* pv4Last = nullptr;

	MATRIX4	mat;
	FillMatrix(&mat);

	wprintf_s(L"TransformVector4_VPTR2\n");

	// NoSIMD
	TransformVector4_VPTR2_NoSIMD(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

	TransformVector4_VPTR2_NoSIMD(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv4First = pv4ListDest + 0;
	pv4Last = pv4ListDest + dwVectorNum - 1;

	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f, %.2f)\n",
			  0, pv4First->x, pv4First->y, pv4First->z, pv4First->w,
			  dwVectorNum - 1, pv4Last->x, pv4Last->y, pv4Last->z, pv4Last->w);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);

	memset(pv4ListDest, 0, sizeof(VECTOR4) * dwVectorNum);
	
	// SSE
#if defined(ARM64)
	TransformVector4_VPTR2_NEON(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);
#else
	TransformVector4_VPTR2_SSE(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);
#endif

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

#if defined(ARM64)
	TransformVector4_VPTR2_NEON(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);
#else
	TransformVector4_VPTR2_SSE(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);
	//TransformVector4_VPTR2_SSE_1(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);
#endif

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv4First = pv4ListDest + 0;
	pv4Last = pv4ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD Vector[%u] = (%.2f, %.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f, %.2f)\n",
			  0, pv4First->x, pv4First->y, pv4First->z, pv4First->w,
			  dwVectorNum - 1, pv4Last->x, pv4Last->y, pv4Last->z, pv4Last->w);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
	wprintf_s(L"\n");
	Sleep(1);
}
void Test_VECTOR3Length(VECTOR3* pv3ListDest, VECTOR3* pv3ListSrc, DWORD dwVectorNum)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	// test VECTOR3Length
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;
	
	VECTOR3* pv3First = nullptr;
	VECTOR3* pv3Last = nullptr;

	wprintf_s(L"VECTOR3Length\n");

	// NoSIMD
	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		pv3ListDest[i].x = VECTOR3Length_NoSIMD(pv3ListSrc + i);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f), Vector[%u] = (%.2f)\n",
			  0, pv3First->x,
			  dwVectorNum - 1, pv3Last->x);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);

	// SSE
	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		// NEON의 경우 NoSIMD보다 느림. 따라서 NEON 적용안함.
	#if defined(ARM64)
		pv3ListDest[i].x = VECTOR3Length_NEON(pv3ListSrc + i);
	#else
		pv3ListDest[i].x = VECTOR3Length_SSE(pv3ListSrc + i);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv3First = pv3ListDest + 0;
	pv3Last = pv3ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD Vector[%u] = (%.2f), Vector[%u] = (%.2f)\n",
			  0, pv3First->x,
			  dwVectorNum - 1, pv3Last->x);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
	wprintf_s(L"\n");
	Sleep(1);
}
void Test_ClipTriWithPlaneList()
{

	///////////////////////////////////////////////////////////////////////////////
	//
	// ClipTriWithPlaneList
	//
	///////////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	wprintf_s(L"ClipTriWithPlaneList\n");

	PLANE	planeList[6] =
	{
		0.994650185f, -0.0948872417f, -0.0408352949f, -834.679993f,
		-0.994650185f, 0.0948872417f, 0.0408352949f, 764.964722f,
		-0.0408352949f, 0.000000000f, -0.994650185f, 202.556473f,
		0.0408352949f, -0.000000000f, 0.994650185f, -256.556458f,
		0.0943796113f, 0.990996480f, -0.00387474848f, -102.147346f,
		-0.0943796113f, -0.990996480f, 0.00387474848f, 48.1473465f
	};
	TRIANGLE tri0 =
	{
		-51200.0000f, 9375.32129f, 51200.0000f,
		51200.0000f, -154.090591f, 51200.0000f,
		-51200.0000f, -154.090591f, 51200.0000f
	};
	const DWORD CLIP_TRI_WITH_PLANE_REPEAT_COUNT = 100;

	// NoSIMD
	DWORD dwClipResult0 = 0;
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < CLIP_TRI_WITH_PLANE_REPEAT_COUNT; i++)
	{
		dwClipResult0 += ClipTriWithPlaneList_NoSIMD(planeList, 6, &tri0);
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tNoSIMD ClipResult : %u , %.2f clocks.\n", dwClipResult0, (float)ElapsedClock / (float)CLIP_TRI_WITH_PLANE_REPEAT_COUNT);

	// SSE
	DWORD dwClipResult1 = 0;
	PrvClock = __rdtsc();
	for (DWORD i = 0; i < CLIP_TRI_WITH_PLANE_REPEAT_COUNT; i++)
	{
	#if defined(ARM64)
		dwClipResult1 += ClipTriWithPlaneList_NEON(planeList, 6, &tri0);
	#else
		dwClipResult1 += ClipTriWithPlaneList_SSE(planeList, 6, &tri0);
	#endif
	}
	ElapsedClock = __rdtsc() - PrvClock;
	wprintf_s(L"\tSIMD ClipResult : %u , %.2f clocks.\n", dwClipResult1, (float)ElapsedClock / (float)CLIP_TRI_WITH_PLANE_REPEAT_COUNT);
	wprintf_s(L"\n");
}
#ifndef _DEBUG
#pragma optimize("",on)
#endif

void FillMatrix(MATRIX4* pOutMat)
{
	SetIdentityMatrix(pOutMat);

	for (DWORD y = 0; y < 4; y++)
	{
		for (DWORD x = 0; x < 4; x++)
		{
			pOutMat->f[y][x] = (float)((rand() % 10) + 1) / 10.0f;
		}
	}
}
void FillVector3(VECTOR3* pv3Out)
{
	float* pf = &pv3Out->x;
	for (DWORD i = 0; i < 3; i++)
	{
		pf[i] = (float)((rand() % 10) + 1) / 10.0f;
	}
}
void FillVector4(VECTOR4* pv3Out)
{
	float* pf = &pv3Out->x;
	for (DWORD i = 0; i < 4; i++)
	{
		pf[i] = (float)((rand() % 10) + 1) / 10.0f;
	}
}

void PrintMatrix(MATRIX4* pMat)
{
	for (DWORD y = 0; y < 4; y++)
	{
		wprintf_s(L"%.2f %.2f %.2f %.2f\n", pMat->f[y][0], pMat->f[y][1], pMat->f[y][2], pMat->f[y][3]);
	}
}