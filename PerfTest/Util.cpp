#include "stdafx.h"
#include <Windows.h>
#include <intrin.h>
#include "Util.h"

#if defined(_M_ARM64) || defined(_M_ARM64EC)
void MemCpyAVX(char* pDest, char* pSrc, size_t Size)
{

}
#else
void MemCpyAVX(char* pDest, char* pSrc, size_t Size)
{
	DWORD	Count = Size / 32;
	for (DWORD i = 0; i < Count; i++)
	{
		__m256 t = _mm256_load_ps((float*)pSrc);
		_mm256_store_ps((float*)pDest, t);
		pSrc += 32;
		pDest += 32;
	}
}
#endif