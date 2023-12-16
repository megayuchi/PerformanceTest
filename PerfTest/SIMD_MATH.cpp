#include "stdafx.h"
#include "SIMD_MATH.h"
#include <intrin.h>
//#if defined(WIN64)
//#include <mmintrin.h>
//#elif defined(ARM64)
//
//#endif




void FillVectorStream(VECTOR4_SIMD* pDest, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		float f[4];
		for (DWORD i = 0; i < 4; i++)
		{
			f[i] = (float)(rand() % 10) / 10.0f;
		}
		pDest->Set(f);
		pDest++;
	}
}

void AddVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		*pDest = *pSrc0 + *pSrc1;
		pSrc0++;
		pSrc1++;
		pDest++;

	}
}
void AddVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		*pDest = *pSrc0 + *pSrc1;
		pSrc0++;
		pSrc1++;
		pDest++;

	}
}

void MulVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		(*pDest) = (*pSrc0) * (*pSrc1);
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}
void MulVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		(*pDest) = (*pSrc0) * (*pSrc1);
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}

void DivVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		(*pDest) = (*pSrc0) / (*pSrc1);
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}
void DivVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		(*pDest) = (*pSrc0) / (*pSrc1);
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}

void AddOrSubOrMulOrDivVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		int iCase = (i % 16);
		if (0 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (1 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (2 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (3 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (4 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (5 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (6 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (7 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (8 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (9 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (10 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (11 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (12 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (13 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (14 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (15 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}

void AddOrSubOrMulOrDivVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		int iCase = (i % 16);
		if (0 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (1 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (2 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (3 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (4 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (5 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (6 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (7 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (8 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (9 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (10 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (11 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		else if (12 == iCase)
		{
			(*pDest) = (*pSrc0) + (*pSrc1);
		}
		else if (13 == iCase)
		{
			(*pDest) = (*pSrc0) - (*pSrc1);
		}
		else if (14 == iCase)
		{
			(*pDest) = (*pSrc0) * (*pSrc1);
		}
		else if (15 == iCase)
		{
			(*pDest) = (*pSrc0) / (*pSrc1);
		}
		pSrc0++;
		pSrc1++;
		pDest++;
	}
}

void AddOrSubOrMulOrDivVectorStream_Random_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		DWORD dwIndex = rand() % dwCount;
		int iCase = (dwIndex % 16);
		const VECTOR4_SIMD* pSrcA = pSrc0 + dwIndex;
		const VECTOR4_SIMD* pSrcB = pSrc1 + dwIndex;
		VECTOR4_SIMD* pDestC = pDest + dwIndex;

		if (0 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (1 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (2 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (3 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (4 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (5 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (6 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (7 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (8 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (9 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (10 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (11 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (12 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (13 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (14 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (15 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
	}
}
void AddOrSubOrMulOrDivVectorStream_Random(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount)
{
	for (DWORD i = 0; i < dwCount; i++)
	{
		DWORD dwIndex = rand() % dwCount;
		int iCase = (dwIndex % 16);
		const VECTOR4_SIMD* pSrcA = pSrc0 + dwIndex;
		const VECTOR4_SIMD* pSrcB = pSrc1 + dwIndex;
		VECTOR4_SIMD* pDestC = pDest + dwIndex;

		if (0 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (1 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (2 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (3 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (4 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (5 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (6 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (7 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (8 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (9 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (10 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (11 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
		else if (12 == iCase)
		{
			(*pDestC) = (*pSrcA) + (*pSrcB);
		}
		else if (13 == iCase)
		{
			(*pDestC) = (*pSrcA) - (*pSrcB);
		}
		else if (14 == iCase)
		{
			(*pDestC) = (*pSrcA) * (*pSrcB);
		}
		else if (15 == iCase)
		{
			(*pDestC) = (*pSrcA) / (*pSrcB);
		}
	}
}