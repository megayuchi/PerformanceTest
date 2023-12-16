#pragma once

#include "typedef.h"
void __stdcall Transpose_SSE(MATRIX4_A* pOutMat, const MATRIX4* pMat);
void __stdcall Transpose_NoSIMD(MATRIX4_A* pOutMat, const MATRIX4* pMat);
void __stdcall TransposeMatrix(MATRIX4* pOutMat, MATRIX4* pMat);

float __stdcall VECTOR3Length_NoSIMD(const VECTOR3* pv3);
float __stdcall VECTOR3Length_SSE(const VECTOR3* pv3);
float __stdcall VECTOR3Length_NEON(const VECTOR3* pv3);

void __stdcall TransformVector3_VPTR2_NoSIMD(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector3_VPTR2_SSE(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector3_VPTR2_SSE_1(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector3_VPTR2_NEON(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum);

void __stdcall TransformVector3_VPTR1_NEON(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector3_VPTR1_NoSIMD(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector3_VPTR1_SSE(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector4_VPTR2_NoSIMD(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector4_VPTR2_SSE(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector4_VPTR2_SSE_1(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum);
void __stdcall TransformVector4_VPTR2_NEON(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum);

float __stdcall DotProduct_NoSIMD(const VECTOR3* pv3_0, const VECTOR3* pv3_1);
float __stdcall DotProduct_SSE(const VECTOR3* pv3_0, const VECTOR3* pv3_1);
float __stdcall DotProduct_NEON(const VECTOR3* pv3_0, const VECTOR3* pv3_1);

void __stdcall Normalize_NoSIMD(VECTOR3* vn, const VECTOR3* v);
void __stdcall Normalize_SSE(VECTOR3* pv3OutN, const VECTOR3* pv3Src);
void __stdcall Normalize_NEON(VECTOR3* pv3OutN, const VECTOR3* pv3Src);

void __stdcall MatrixMultiply2_NoSIMD(MATRIX4* pResult, const MATRIX4* mat1, const MATRIX4* mat2);
void __stdcall MatrixMultiply2_SSE(MATRIX4* pResult, const MATRIX4* mat1, const MATRIX4* mat2);
void __stdcall MatrixMultiply2_NEON(MATRIX4* pResult, const MATRIX4* pMat0, const MATRIX4* pMat1);
void __stdcall MatrixMultiply2_NEON_1(MATRIX4* pResult, const MATRIX4* pMatA, const MATRIX4* pMatB);

void __stdcall CrossProduct_NoSIMD(VECTOR3* r, const VECTOR3* u, const VECTOR3* v);
void __stdcall CrossProduct_SSE(VECTOR3* r, const VECTOR3* u, const VECTOR3* v);
void __stdcall CrossProduct_NEON(VECTOR3* r, const VECTOR3* u, const VECTOR3* v);
void __stdcall CrossProduct_NEON_1(VECTOR3* r, const VECTOR3* u, const VECTOR3* v);

float __stdcall CalcDistance_NoSIMD(const VECTOR3* pv3Start, const VECTOR3* pv3End);
float __stdcall CalcDistance_SSE(const VECTOR3* pv3Start, const VECTOR3* pv3End);
float __stdcall CalcDistance_NEON(const VECTOR3* pv3Start, const VECTOR3* pv3End);

BOOL __stdcall CalcPlaneEquation_NoSIMD(PLANE* pPlane, const VECTOR3* pv3Tri);
BOOL __stdcall CalcPlaneEquation_SSE(PLANE* pPlane, const VECTOR3* pv3Tri);
BOOL __stdcall CalcPlaneEquation_NEON(PLANE* pPlane, const VECTOR3* pv3Tri);

BOOL __stdcall CalcPlaneEquationWithPtr_NoSIMD(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2);
BOOL __stdcall CalcPlaneEquationWithPtr_SSE(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2);
BOOL __stdcall CalcPlaneEquationWithPtr_NEON(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2);

BOOL __stdcall ClipTriWithPlaneList_NoSIMD(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri);
BOOL __stdcall ClipTriWithPlaneList_SSE(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri);
BOOL __stdcall ClipTriWithPlaneList_NEON(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri);

BOOL __stdcall SetIdentityMatrix(MATRIX4* pOutMat);
//void matrix_multiply_4x4_neon(float32_t *A, float32_t *B, float32_t *C);
void TestShuffle();