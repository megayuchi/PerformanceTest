#include "stdafx.h"
#include "typedef.h"
#include "MathFunc.h"
#if defined(ARM64)
#include <arm64_neon.h>
#endif

float __stdcall VECTOR3Length_NoSIMD(const VECTOR3* pv3)
{
	float	r = (float)sqrtf((pv3->x * pv3->x) + (pv3->y * pv3->y) + (pv3->z * pv3->z));
	return r;
}
float __stdcall VECTOR3Length_SSE(const VECTOR3* pv3)
{
	__m128 zero = {};
	__m128 xy00 = _mm_loadl_pi(zero, (__m64*) & pv3->x);
	__m128 z000 = _mm_load_ss(&pv3->z);
	__m128 xyz0 = _mm_movelh_ps(xy00, z000);
	xyz0 = _mm_mul_ps(xyz0, xyz0);

	__m128 sum = _mm_hadd_ps(xyz0, xyz0);
	sum = _mm_hadd_ps(sum, sum);
	__m128 r = _mm_sqrt_ss(sum);

	return r.m128_f32[0];
}
void __stdcall TransformVector3_VPTR2_NoSIMD(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum)
{
	for (DWORD i = 0; i < dwNum; i++)
	{
		pv3Dest->x = pv3Src->x * pMat->_11 + pv3Src->y * pMat->_21 + pv3Src->z * pMat->_31 + pMat->_41;
		pv3Dest->y = pv3Src->x * pMat->_12 + pv3Src->y * pMat->_22 + pv3Src->z * pMat->_32 + pMat->_42;
		pv3Dest->z = pv3Src->x * pMat->_13 + pv3Src->y * pMat->_23 + pv3Src->z * pMat->_33 + pMat->_43;

		pv3Src++;
		pv3Dest++;
	}
}

void __stdcall Transpose_SSE(MATRIX4_A* pOutMat, const MATRIX4* pMat)
{
	// original
	// _m00 _m01 _m02 _m03
	// _m10 _m11 _m12 _m13
	// _m20 _m21 _m22 _m23
	// _m30 _m31 _m32 _m33

	// trasnposed
	// _m00 _m10 _m20 _m30
	// _m01 _m11 _m21 _m31
	// _m02 _m12 _m22 _m32
	// _m03 _m13 _m23 _m33

	// origianl - registers
	// m0 = _m03 _m02 _m01 _m00
	// m1 = _m13 _m12 _m11 _m10
	// m2 = _m23 _m22 _m21 _m20
	// m3 = _m33 _m32 _m31 _m30

	// transposed - registers
	// n0 = _m30 _m20 _m10 _m00 - xmm0
	// n1 = _m31 _m21 _m11 _m01 - xmm1
	// n2 = _m32 _m22 _m12 _m02 - xmm2
	// n3 = _m33 _m23 _m13 _m03 - xmm3
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);

	// begin
	__m128 m01_T;
	__m128 m23_T;
	m01_T = _mm_shuffle_ps(m0, m1, 0b00000000);	// _m10 _m10 _m00 _m00
	m23_T = _mm_shuffle_ps(m2, m3, 0b00000000);	// _m30 _m30 _m20 _m20
	__m128 m0_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m30 _m20 _m10 _m00

	m01_T = _mm_shuffle_ps(m0, m1, 0b01010101);	// _m11 _m11 _m01 _m01
	m23_T = _mm_shuffle_ps(m2, m3, 0b01010101);	// _m31 _m31 _m21 _m21
	__m128 m1_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m31 _m21 _m11 _m01

	m01_T = _mm_shuffle_ps(m0, m1, 0b10101010);	// _m12 _m12 _m02 _m02
	m23_T = _mm_shuffle_ps(m2, m3, 0b10101010);	// _m32 _m32 _m22 _m22
	__m128 m2_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m32 _m22 _m12 _m02

	m01_T = _mm_shuffle_ps(m0, m1, 0b11111111);	// _m13 _m13 _m03 _m03
	m23_T = _mm_shuffle_ps(m2, m3, 0b11111111);	// _m33 _m33 _m23 _m23
	__m128 m3_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m33 _m23 _m13 _m03
	
	pOutMat->_11 = m0_T;
	pOutMat->_21 = m1_T;
	pOutMat->_31 = m2_T;
	pOutMat->_41 = m3_T;
}
void __stdcall Transpose_NoSIMD(MATRIX4_A* pOutMat, const MATRIX4* pMat)
{
	__m128 m0_T = { pMat->f[0][0], pMat->f[1][0], pMat->f[2][0], pMat->f[3][0] };
	__m128 m1_T = { pMat->f[0][1], pMat->f[1][1], pMat->f[2][1], pMat->f[3][1] };
	__m128 m2_T = { pMat->f[0][2], pMat->f[1][2], pMat->f[2][2], pMat->f[3][2] };
	__m128 m3_T = { pMat->f[0][3], pMat->f[1][3], pMat->f[2][3], pMat->f[3][3] };

	pOutMat->_11 = m0_T;
	pOutMat->_21 = m1_T;
	pOutMat->_31 = m2_T;
	pOutMat->_41 = m3_T;
}
void __stdcall TransposeMatrix(MATRIX4* pOutMat, MATRIX4* pMat)
{
	float t_12, t_13, t_14, t_23, t_24, t_34;

	t_12 = pMat->_12;
	t_13 = pMat->_13;
	t_14 = pMat->_14;
	t_23 = pMat->_23;
	t_24 = pMat->_24;
	t_34 = pMat->_34;

	pOutMat->_12 = pMat->_21;
	pOutMat->_13 = pMat->_31;
	pOutMat->_14 = pMat->_41;
	pOutMat->_23 = pMat->_32;
	pOutMat->_24 = pMat->_42;
	pOutMat->_34 = pMat->_43;

	pOutMat->_11 = pMat->_11;
	pOutMat->_22 = pMat->_22;
	pOutMat->_33 = pMat->_33;
	pOutMat->_44 = pMat->_44;

	pOutMat->_21 = t_12;
	pOutMat->_31 = t_13;
	pOutMat->_41 = t_14;
	pOutMat->_32 = t_23;
	pOutMat->_42 = t_24;
	pOutMat->_43 = t_34;
}
void __stdcall TransformVector3_VPTR2_SSE(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum)
{
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);
		
	__m128 zero = {};
	float one = 1.0f;

	for (DWORD i = 0; i < dwNum; i++)
	{
		__m128 src_xy = _mm_loadl_pi(zero, (__m64*)pv3Src);
		__m128 src_z = _mm_load_ss(&pv3Src->z);
		__m128 src_xyz0 = _mm_movelh_ps(src_xy, src_z);

		__m128 xxxx = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b00000000);
		__m128 yyyy = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b01010101);
		__m128 zzzz = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b10101010);

		__m128 r0 = _mm_mul_ps(xxxx, m0);
		__m128 r1 = _mm_mul_ps(yyyy, m1);
		__m128 r2 = _mm_mul_ps(zzzz, m2);

		r0 = _mm_add_ps(r0, r1);
		r2 = _mm_add_ps(r2, m3);
		r0 = _mm_add_ps(r0, r2);

		_mm_storel_pi((__m64*)pv3Dest, r0);

		__m128 rz = _mm_movehl_ps(zero, r0);
		_mm_store_ss(&pv3Dest->z, rz);


		pv3Src++;
		pv3Dest++;
	}
}
void __stdcall TransformVector3_VPTR2_SSE_1(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum)
{
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);
		
	__m128 m01_T;
	__m128 m23_T;
	m01_T = _mm_shuffle_ps(m0, m1, 0b00000000);	// _m10 _m10 _m00 _m00
	m23_T = _mm_shuffle_ps(m2, m3, 0b00000000);	// _m30 _m30 _m20 _m20
	__m128 m0_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m30 _m20 _m10 _m00

	m01_T = _mm_shuffle_ps(m0, m1, 0b01010101);	// _m11 _m11 _m01 _m01
	m23_T = _mm_shuffle_ps(m2, m3, 0b01010101);	// _m31 _m31 _m21 _m21
	__m128 m1_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m31 _m21 _m11 _m01

	m01_T = _mm_shuffle_ps(m0, m1, 0b10101010);	// _m12 _m12 _m02 _m02
	m23_T = _mm_shuffle_ps(m2, m3, 0b10101010);	// _m32 _m32 _m22 _m22
	__m128 m2_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m32 _m22 _m12 _m02

	m01_T = _mm_shuffle_ps(m0, m1, 0b11111111);	// _m13 _m13 _m03 _m03
	m23_T = _mm_shuffle_ps(m2, m3, 0b11111111);	// _m33 _m33 _m23 _m23
	__m128 m3_T = _mm_shuffle_ps(m01_T, m23_T, 0b10001000);	// _m33 _m23 _m13 _m03

	__m128 zero = _mm_set1_ps(0.0f);
	__m128 w = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

	for (DWORD i = 0; i < dwNum; i++)
	{
		__m128 src_xy = _mm_loadl_pi(zero, (__m64*)pv3Src);
		__m128 src_z = _mm_load_ss(&pv3Src->z);
		__m128 src_xyzw = _mm_movelh_ps(src_xy, src_z);
		src_xyzw = _mm_add_ps(src_xyzw, w);

		__m128 r0 = _mm_mul_ps(src_xyzw, m0_T);
		__m128 r1 = _mm_mul_ps(src_xyzw, m1_T);
		__m128 r2 = _mm_mul_ps(src_xyzw, m2_T);
		__m128 r3 = _mm_mul_ps(src_xyzw, m3_T);

		r0 = _mm_hadd_ps(r0, r0);
		r0 = _mm_hadd_ps(r0, r0);

		r1 = _mm_hadd_ps(r1, r1);
		r1 = _mm_hadd_ps(r1, r1);

		r2 = _mm_hadd_ps(r2, r2);
		r2 = _mm_hadd_ps(r2, r2);

		r3 = _mm_hadd_ps(r3, r3);
		r3 = _mm_hadd_ps(r3, r3);

		r0 = _mm_shuffle_ps(r0, r1, 0b00000000);	// r1.x | r1.x | r0.x | r0.x
		r2 = _mm_shuffle_ps(r2, r3, 0b00000000);	// r3.x | r3.x | r2.x | r2.x

		__m128 r = _mm_shuffle_ps(r0, r2, 0b10001000);

		_mm_storel_pi((__m64*)pv3Dest, r);

		__m128 rz = _mm_movehl_ps(zero, r);
		_mm_store_ss(&pv3Dest->z, rz);


		pv3Src++;
		pv3Dest++;
	}
}
#if defined(ARM64)

void matrix_multiply_4x4_neon(float32_t *A, float32_t *B, float32_t *C)
{
	// these are the columns A
	float32x4_t A0;
	float32x4_t A1;
	float32x4_t A2;
	float32x4_t A3;

	// these are the columns B
	float32x4_t B0;
	float32x4_t B1;
	float32x4_t B2;
	float32x4_t B3;

	// these are the columns C
	float32x4_t C0;
	float32x4_t C1;
	float32x4_t C2;
	float32x4_t C3;

	A0 = vld1q_f32(A);
	A1 = vld1q_f32(A + 4);
	A2 = vld1q_f32(A + 8);
	A3 = vld1q_f32(A + 12);

	// Zero accumulators for C values
	C0 = vmovq_n_f32(0);
	C1 = vmovq_n_f32(0);
	C2 = vmovq_n_f32(0);
	C3 = vmovq_n_f32(0);

	// Multiply accumulate in 4x1 blocks, i.e. each column in C
	B0 = vld1q_f32(B);
	// vfmaq_laneq_f32 A0의 각 성분들을 B0[마지막 인자로 전달한 인덱스의]로 곱해서 C0와 더한다.
	C0 = vfmaq_laneq_f32(C0, A0, B0, 0);		// C0[0] = A0[0] * B0[0] , C0[1] = A0[1] * B0[0] , C0[2] = A0[2] * B0[0] , C0[3] = A0[3] * B0[0]
	C0 = vfmaq_laneq_f32(C0, A1, B0, 1);
	C0 = vfmaq_laneq_f32(C0, A2, B0, 2);
	C0 = vfmaq_laneq_f32(C0, A3, B0, 3);
	vst1q_f32(C, C0);

	B1 = vld1q_f32(B + 4);
	C1 = vfmaq_laneq_f32(C1, A0, B1, 0);
	C1 = vfmaq_laneq_f32(C1, A1, B1, 1);
	C1 = vfmaq_laneq_f32(C1, A2, B1, 2);
	C1 = vfmaq_laneq_f32(C1, A3, B1, 3);
	vst1q_f32(C + 4, C1);

	B2 = vld1q_f32(B + 8);
	C2 = vfmaq_laneq_f32(C2, A0, B2, 0);
	C2 = vfmaq_laneq_f32(C2, A1, B2, 1);
	C2 = vfmaq_laneq_f32(C2, A2, B2, 2);
	C2 = vfmaq_laneq_f32(C2, A3, B2, 3);
	vst1q_f32(C + 8, C2);

	B3 = vld1q_f32(B + 12);
	C3 = vfmaq_laneq_f32(C3, A0, B3, 0);
	C3 = vfmaq_laneq_f32(C3, A1, B3, 1);
	C3 = vfmaq_laneq_f32(C3, A2, B3, 2);
	C3 = vfmaq_laneq_f32(C3, A3, B3, 3);
	vst1q_f32(C + 12, C3);
}

void __stdcall MatrixMultiply2_NEON(MATRIX4* pResult, const MATRIX4* pMatA, const MATRIX4* pMatB)
{
	// 4 x4 matrix multiply
	// A00 , A01, A02, A03				B00 , B01, B02, B03
	// A10 , A11, A12, A13		x		B10 , B11, B12, B13		
	// A20 , A21, A22, A23				B20 , B21, B22, B23
	// A30 , A31, A32, A33				B30 , B31, B32, B33

	// = 
	// A00xB00 + A01xB10 + A02xB20 + A03xB30, A00xB01 + A01xB11 + A02xB21 + A03xB31, A00xB02 + A01xB12 + A02xB22 + A03xB32, A00xB03 + A01xB13 + A02xB23 + A03xB33

	// for Neon
	// registers
	// [ B03 | B02 | B01 | B00 ] x [A00 | A00 | A00 | A00 ] = [ A00xB03 | A00xB02 | A00xB01 | A00xB00 ]
	//																		     +				
	// [ B13 | B12 | B11 | B10 ] x [A01 | A01 | A01 | A01 ] = [ A01xB13 | A01xB12 | A01xB11 | A01xB10 ]
	//																		     +				
	// [ B23 | B22 | B21 | B20 ] x [A02 | A02 | A02 | A02 ] = [ A02xB23 | A02xB22 | A02xB21 | A02xB20 ]
	//																		     +				
	// [ B33 | B32 | B31 | B30 ] x [A03 | A03 | A03 | A03 ] = [ A03xB33 | A03xB32 | A03xB31 | A03xB30 ]
	//																		     =				
	// [ A00xB03 + A01xB13 + A02xB23 + A03xB33 | A00xB02 + A01xB12 + A02xB22 + A03xB32 | A00xB01 + A01xB11 + A02xB21 + A03xB31 | A00xB00 + A01xB10 + A02xB20 + A03xB30]

	//vfmaq_laneq_f32(C , B, A, index)	<- B[0,1,2,3] x A[index] + C[0,1,2,3]

	float32x4_t R0 = vmovq_n_f32(0);
	float32x4_t R1 = vmovq_n_f32(0);
	float32x4_t R2 = vmovq_n_f32(0);
	float32x4_t R3 = vmovq_n_f32(0);

	float32x4_t B0 = vld1q_f32((float*)pMatB);
	float32x4_t B1 = vld1q_f32((float*)pMatB + 4);
	float32x4_t B2 = vld1q_f32((float*)pMatB + 8);
	float32x4_t B3 = vld1q_f32((float*)pMatB + 12);

	float32x4_t A0 = vld1q_f32((float*)pMatA + 0);
	float32x4_t A1 = vld1q_f32((float*)pMatA + 4);
	float32x4_t A2 = vld1q_f32((float*)pMatA + 8);
	float32x4_t A3 = vld1q_f32((float*)pMatA + 12);

	R0 = vfmaq_laneq_f32(R0, B0, A0, 0);
	R0 = vfmaq_laneq_f32(R0, B1, A0, 1);
	R0 = vfmaq_laneq_f32(R0, B2, A0, 2);
	R0 = vfmaq_laneq_f32(R0, B3, A0, 3);

	vst1q_f32((float*)pResult, R0);

	R1 = vfmaq_laneq_f32(R1, B0, A1, 0);
	R1 = vfmaq_laneq_f32(R1, B1, A1, 1);
	R1 = vfmaq_laneq_f32(R1, B2, A1, 2);
	R1 = vfmaq_laneq_f32(R1, B3, A1, 3);
	vst1q_f32((float*)pResult + 4, R1);


	R2 = vfmaq_laneq_f32(R2, B0, A2, 0);
	R2 = vfmaq_laneq_f32(R2, B1, A2, 1);
	R2 = vfmaq_laneq_f32(R2, B2, A2, 2);
	R2 = vfmaq_laneq_f32(R2, B3, A2, 3);
	vst1q_f32((float*)pResult + 8, R2);


	R3 = vfmaq_laneq_f32(R3, B0, A3, 0);
	R3 = vfmaq_laneq_f32(R3, B1, A3, 1);
	R3 = vfmaq_laneq_f32(R3, B2, A3, 2);
	R3 = vfmaq_laneq_f32(R3, B3, A3, 3);
	vst1q_f32((float*)pResult + 12, R3);
}
void __stdcall MatrixMultiply2_NEON_1(MATRIX4* pResult, const MATRIX4* pMatA, const MATRIX4* pMatB)
{
	//
	// 4회 loop사용하도록 수정. 그러나 더 느려짐. unrolling안해주나봄.
	//

	// 4 x4 matrix multiply
	// A00 , A01, A02, A03				B00 , B01, B02, B03
	// A10 , A11, A12, A13		x		B10 , B11, B12, B13		
	// A20 , A21, A22, A23				B20 , B21, B22, B23
	// A30 , A31, A32, A33				B30 , B31, B32, B33

	// = 
	// A00xB00 + A01xB10 + A02xB20 + A03xB30, A00xB01 + A01xB11 + A02xB21 + A03xB31, A00xB02 + A01xB12 + A02xB22 + A03xB32, A00xB03 + A01xB13 + A02xB23 + A03xB33

	// for Neon
	// registers
	// [ B03 | B02 | B01 | B00 ] x [A00 | A00 | A00 | A00 ] = [ A00xB03 | A00xB02 | A00xB01 | A00xB00 ]
	//																		     +				
	// [ B13 | B12 | B11 | B10 ] x [A01 | A01 | A01 | A01 ] = [ A01xB13 | A01xB12 | A01xB11 | A01xB10 ]
	//																		     +				
	// [ B23 | B22 | B21 | B20 ] x [A02 | A02 | A02 | A02 ] = [ A02xB23 | A02xB22 | A02xB21 | A02xB20 ]
	//																		     +				
	// [ B33 | B32 | B31 | B30 ] x [A03 | A03 | A03 | A03 ] = [ A03xB33 | A03xB32 | A03xB31 | A03xB30 ]
	//																		     =				
	// [ A00xB03 + A01xB13 + A02xB23 + A03xB33 | A00xB02 + A01xB12 + A02xB22 + A03xB32 | A00xB01 + A01xB11 + A02xB21 + A03xB31 | A00xB00 + A01xB10 + A02xB20 + A03xB30]

	//vfmaq_laneq_f32(C , B, A, index)	<- B[0,1,2,3] x A[index] + C[0,1,2,3]

	float*	pA = (float*)pMatA;
	float*	pDest = (float*)pResult;

	float32x4_t B0 = vld1q_f32((float*)pMatB);
	float32x4_t B1 = vld1q_f32((float*)pMatB + 4);
	float32x4_t B2 = vld1q_f32((float*)pMatB + 8);
	float32x4_t B3 = vld1q_f32((float*)pMatB + 12);

	for (int i = 0; i < 4; i++)
	{
		float32x4_t R = vmovq_n_f32(0);
		float32x4_t A = vld1q_f32((float*)pA);

		R = vfmaq_laneq_f32(R, B0, A, 0);
		R = vfmaq_laneq_f32(R, B1, A, 1);
		R = vfmaq_laneq_f32(R, B2, A, 2);
		R = vfmaq_laneq_f32(R, B3, A, 3);

		vst1q_f32(pDest, R);

		pA += 4;
		pDest += 4;
	}
}
void __stdcall Normalize_NEON(VECTOR3* pv3OutN, const VECTOR3* pv3Src)
{
	float32x2_t xy = vld1_f32(pv3Src);
	float32x2_t z = vld1_lane_f32(&pv3Src->z, vdup_n_f32(0), 0);
	float32x4_t xyz0 = vcombine_f32(xy, z);

	float32x4_t mag = vmulq_f32(xyz0, xyz0);
	mag = vpaddq_f32(mag, mag);
	mag = vpaddq_f32(mag, mag);

	if (0.0f != mag.n128_f32[0])
	{
		mag = vrsqrteq_f32(mag);	// 1 / sqrt(r)
	}
	float32x4_t r = vmulq_f32(xyz0, mag);

	vst1_f32(pv3OutN, vget_low_f32(r));	// write xy
	vst1q_lane_f32(&pv3OutN->z, r, 2);	// write z
}
float __stdcall CalcDistance_NEON(const VECTOR3* pv3Start, const VECTOR3* pv3End)
{
	float32x2_t u_xy = vld1_f32(pv3Start);
	float32x2_t u_z = vld1_lane_f32(&pv3Start->z, vdup_n_f32(0), 0);
	float32x4_t u_xyz0 = vcombine_f32(u_xy, u_z);

	float32x2_t v_xy = vld1_f32(pv3End);
	float32x2_t v_z = vld1_lane_f32(&pv3End->z, vdup_n_f32(0), 0);
	float32x4_t v_xyz0 = vcombine_f32(v_xy, v_z);

	float32x4_t	dir = vsubq_f32(u_xyz0, v_xyz0);
	float32x4_t	mag = vmulq_f32(dir, dir);

	mag = vpaddq_f32(mag, mag);
	float32x4_t r = vpaddq_f32(mag, mag);

	float32x4_t recipsq = vrsqrteq_f32(r);	// 1 / sqrt(r)
	r = vrecpeq_f32(recipsq);	// sqrt(r);

	return r.n128_f32[0];
}
/*
void __stdcall CrossProduct_NoSIMD(VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
{
	r->x = u->y * v->z - u->z * v->y;
	r->y = u->z * v->x - u->x * v->z;
	r->z = u->x * v->y - u->y * v->x;
}
__m128 zero = {};

	__m128 u_xy00 = _mm_loadl_pi(zero, (__m64*) & u->x);
	__m128 u_z000 = _mm_load_ss(&u->z);
	__m128 u_xyz0 = _mm_movelh_ps(u_xy00, u_z000);

	__m128 v_xy00 = _mm_loadl_pi(zero, (__m64*) & v->x);
	__m128 v_z000 = _mm_load_ss(&v->z);
	__m128 v_xyz0 = _mm_movelh_ps(v_xy00, v_z000);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	__m128 u_yzzx = _mm_shuffle_ps(u_xyz0, u_xyz0, 41);
	__m128 v_zxyz = _mm_shuffle_ps(v_xyz0, v_xyz0, 146);

	__m128 t0 = _mm_mul_ps(u_yzzx, v_zxyz);	// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z
	__m128 t1 = _mm_movehl_ps(zero, t0);	// |			 |			  |	u->x*v->z |	u->z*v->y
	__m128 r_xy = _mm_sub_ps(t0, t1);		// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	__m128 ux__uy = _mm_shuffle_ps(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x
	__m128 vy__vx = _mm_shuffle_ps(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y
	t0 = _mm_mul_ps(ux__uy, vy__vx);					//	|			  |	u->y*v->x |			  | u->x*v->y
	t1 = _mm_movehl_ps(zero, t0);						//	|			  |			  | 		  | u->y*v->x
	__m128 r_z = _mm_sub_ss(t0, t1);					//  |			  |           |           | u->x*v->y - u->y*v->x

	_mm_storel_pi((__m64*)r, r_xy);
	_mm_store_ss(&r->z, r_z);
}
*/
/*
inline float32x4_t __vectorcall _ARM64_mm_shuffle_ps(float32x4_t a, float32x4_t b, int i)
{
	float32x4_t r;

	unsigned dest_index_0 = i & 0b00000011;
	unsigned dest_index_1 = (i & 0b00001100) >> 2;
	unsigned src_index_0 = (i & 0b000110000) >> 4;
	unsigned src_index_1 = (i & 0b11000000) >> 6;

	r.n128_f32[0] = a.n128_f32[dest_index_0];
	r.n128_f32[1] = a.n128_f32[dest_index_1];
	r.n128_f32[2] = b.n128_f32[src_index_0];
	r.n128_f32[3] = b.n128_f32[src_index_1];

	return r;
}
*/
constexpr unsigned int ShuffleIndex_0(unsigned int imm)
{
	return (imm & 0x3);
}
constexpr unsigned int ShuffleIndex_1(unsigned int imm)
{
	return ((imm >> 2) & 0x3);
}
constexpr unsigned int ShuffleIndex_2(unsigned int imm)
{
	return ((imm >> 4) & 0x3);
}
constexpr unsigned int ShuffleIndex_3(unsigned int imm)
{
	return ((imm >> 6) & 0x3);
}


void __stdcall CrossProduct_NEON(VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
{
	//unsigned int imm = 5;
	//unsigned int index_0 = ShuffleIndex_0(imm);
	//unsigned int index_1 = ShuffleIndex_1(imm);
	//unsigned int index_2 = ShuffleIndex_2(imm);
	//unsigned int index_3 = ShuffleIndex_3(imm);

	float32x4_t zero = vdupq_n_f32(0);
	float32x2_t u_xy00 = vld1_f32(u);
	float32x2_t u_z000 = vld1_lane_f32(&u->z, vdup_n_f32(0), 0);
	float32x4_t u_xyz0 = vcombine_f32(u_xy00, u_z000);

	float32x2_t v_xy00 = vld1_f32(v);
	float32x2_t v_z000 = vld1_lane_f32(&v->z, vdup_n_f32(0), 0);
	float32x4_t v_xyz0 = vcombine_f32(v_xy00, v_z000);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	float32x4_t u_yzzx = _ARM64_mm_shuffle_00_10_10_01(u_xyz0, u_xyz0, 41);		// | u->x	 |	u->z	|	u->z	|	u->y
	float32x4_t v_zxyz = _ARM64_mm_shuffle_10_01_00_10(v_xyz0, v_xyz0, 146);	// | v->z	 |	v->y	|	v->x	|	v->z	



	float32x4_t t0 = vmulq_f32(u_yzzx, v_zxyz);								// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z	
	float32x4_t t1 = vcombine_f32(vget_high_f32(t0), vget_high_f32(zero));	// |			 |			  |	u->x*v->z |	u->z*v->y
	float32x4_t r_xy = vsubq_f32(t0, t1);									// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	float32x4_t ux__uy = _ARM64_mm_shuffle_00_01_00_00(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x	
	float32x4_t vy__vx = _ARM64_mm_shuffle_00_00_01_01(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y	


	t0 = vmulq_f32(ux__uy, vy__vx);									//	|			  |	u->y*v->x |			  | u->x*v->y 
	t1 = vcombine_f32(vget_high_f32(t0), vget_high_f32(zero));		//	|			  |			  | 		  | u->y*v->x
	//t1 = _mm_movehl_ps(zero, t0);						

	//__m128 r_z = _mm_sub_ss(t0, t1);
	float32x4_t t1_x000 = vsetq_lane_f32(vgetq_lane_f32(t1, 0), vdupq_n_f32(0), 0);
	float32x4_t r_z = vsubq_f32(t0, t1_x000);						//  |			  |           |           | u->x*v->y - u->y*v->x				

	vst1_f32(r, vget_low_f32(r_xy));	// write xy
	vst1q_lane_f32(&r->z, r_z, 0);		// write z

}

void __stdcall CrossProduct_NEON_1(VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
{
	//unsigned int imm = 5;
	//unsigned int index_0 = ShuffleIndex_0(imm);
	//unsigned int index_1 = ShuffleIndex_1(imm);
	//unsigned int index_2 = ShuffleIndex_2(imm);
	//unsigned int index_3 = ShuffleIndex_3(imm);

	float32x4_t zero = vdupq_n_f32(0);
	float32x2_t u_xy00 = vld1_f32(u);
	float32x2_t u_z000 = vld1_lane_f32(&u->z, vdup_n_f32(0), 0);
	float32x4_t u_xyz0 = vcombine_f32(u_xy00, u_z000);

	float32x2_t v_xy00 = vld1_f32(v);
	float32x2_t v_z000 = vld1_lane_f32(&v->z, vdup_n_f32(0), 0);
	float32x4_t v_xyz0 = vcombine_f32(v_xy00, v_z000);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	float32x4_t u_yzzx = _ARM64_mm_shuffle_ps(u_xyz0, u_xyz0, 41);		// | u->x	 |	u->z	|	u->z	|	u->y
	float32x4_t v_zxyz = _ARM64_mm_shuffle_ps(v_xyz0, v_xyz0, 146);	// | v->z	 |	v->y	|	v->x	|	v->z	



	float32x4_t t0 = vmulq_f32(u_yzzx, v_zxyz);								// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z	
	float32x4_t t1 = vcombine_f32(vget_high_f32(t0), vget_high_f32(zero));	// |			 |			  |	u->x*v->z |	u->z*v->y
	float32x4_t r_xy = vsubq_f32(t0, t1);									// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	float32x4_t ux__uy = _ARM64_mm_shuffle_ps(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x	
	float32x4_t vy__vx = _ARM64_mm_shuffle_ps(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y	


	t0 = vmulq_f32(ux__uy, vy__vx);									//	|			  |	u->y*v->x |			  | u->x*v->y 
	t1 = vcombine_f32(vget_high_f32(t0), vget_high_f32(zero));		//	|			  |			  | 		  | u->y*v->x
	//t1 = _mm_movehl_ps(zero, t0);						

	//__m128 r_z = _mm_sub_ss(t0, t1);
	float32x4_t t1_x000 = vsetq_lane_f32(vgetq_lane_f32(t1, 0), vdupq_n_f32(0), 0);
	float32x4_t r_z = vsubq_f32(t0, t1_x000);						//  |			  |           |           | u->x*v->y - u->y*v->x				

	vst1_f32(r, vget_low_f32(r_xy));	// write xy
	vst1q_lane_f32(&r->z, r_z, 0);		// write z

}
void TestShuffle()
{
	float32x4_t m0123;
	m0123.n128_f32[0] = 0.0f;
	m0123.n128_f32[1] = 1.0f;
	m0123.n128_f32[2] = 2.0f;
	m0123.n128_f32[3] = 3.0f;

	float32_t m0 = vgetq_lane_f32(m0123, 0);	// 0번째 32비트 샘플
	float32_t m1 = vgetq_lane_f32(m0123, 1);	// 1번째 32비트 샘플
	float32_t m2 = vgetq_lane_f32(m0123, 2);	// 2번째 32비트 샘플
	float32_t m3 = vgetq_lane_f32(m0123, 3);	// 3번째 32비트 샘플

	float32x2_t m01 = vget_low_f32(m0123);	// 하위 64비트
	float32x2_t m23 = vget_high_f32(m0123);	// 상위 64비트
	float32x2_t m10 = vrev64_f32(m01);	// 두 32비트 샘플의 위치를 바꾼다.
	float32x2_t m32 = vrev64_f32(m23);	// 두 32비트 샘플의 위치를 바꾼다.

	float32x2_t m00 = vdup_lane_f32(m01, 0);	// 0번째 샘플로 32비트 두 샘플을 다 채운다.
	float32x2_t m11 = vdup_lane_f32(m01, 1);	// 0번째 샘플로 32비트 두 샘플을 다 채운다.

	float32x2_t m22 = vdup_lane_f32(m23, 0);	// 0번째 샘플로 32비트 두 샘플을 다 채운다.
	float32x2_t m33 = vdup_lane_f32(m23, 1);	// 0번째 샘플로 32비트 두 샘플을 다 채운다.
	float32x4_t m3210 = vcombine_f32(m32, m10);	// 64비트 두 샘플을 조합. 앞이 하위 64비트, 뒤가 상위 64비트

	//https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a/optimizing-c-code-with-neon-intrinsics/optimizing-matrix-multiplication
	//https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/coding-for-neon---part-5-rearranging-vectors
//	float32x2_t x = vld1_f32( reinterpret_cast<const float*>(pSource) );
//float32x2_t zero = vdup_n_f32(0);
//float32x2_t y = vld1_lane_f32( reinterpret_cast<const float*>(pSource)+2, zero, 0 );
//return vcombine_f32( x, y );

	//float32x4_t m0 = vld1q_f32((float*)&pMat->_11);
	float32x2_t zero = {};
	//	vld1_lane_f32(m01, zero, 0);
		//vsetq_lane_f32(


	int a = 0;

	// 전치행렬도 이걸로 구현하자.

	// shuffle(a,b,0);

}
float __stdcall VECTOR3Length_NEON(const VECTOR3* pv3)
{
	float32x2_t src_xy = vld1_f32(pv3);
	float32x2_t src_z = vld1_lane_f32(&pv3->z, vdup_n_f32(0), 0);
	float32x4_t src_xyz0 = vcombine_f32(src_xy, src_z);

	float32x4_t	r = vmulq_f32(src_xyz0, src_xyz0);
	r = vpaddq_f32(r, r);
	r = vpaddq_f32(r, r);

	float32x4_t recipsq = vrsqrteq_f32(r);	// 1 / sqrt(r)
	r = vrecpeq_f32(recipsq);	// sqrt(r);

	return r.n128_f32[0];
}
float __stdcall DotProduct_NEON(const VECTOR3* pv3_0, const VECTOR3* pv3_1)
{
	float32x2_t src0_xy = vld1_f32(pv3_0);
	float32x2_t src0_z = vld1_lane_f32(&pv3_0->z, vdup_n_f32(0), 0);
	float32x4_t src0_xyz0 = vcombine_f32(src0_xy, src0_z);

	float32x2_t src1_xy = vld1_f32(pv3_1);
	float32x2_t src1_z = vld1_lane_f32(&pv3_1->z, vdup_n_f32(0), 0);
	float32x4_t src1_xyz0 = vcombine_f32(src1_xy, src1_z);


	float32x4_t	r = vmulq_f32(src0_xyz0, src1_xyz0);
	r = vpaddq_f32(r, r);
	r = vpaddq_f32(r, r);


	return r.n128_f32[0];
}
void __stdcall TransformVector3_VPTR2_NEON(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum)
{
	// 1x4 matrix multiply
	//									B00 , B01, B02, B03
	// A00 , A01, A02, 1		x		B10 , B11, B12, B13		
	//									B20 , B21, B22, B23
	//									B30 , B31, B32, B33

	// = 
	// A00xB00 + A01xB10 + A02xB20 + 1xB30, A00xB01 + A01xB11 + A02xB21 + 1xB31, A00xB02 + A01xB12 + A02xB22 + 1xB32, A00xB03 + A01xB13 + A02xB23 + 1xB33

	// for Neon
	// registers
	// [ B03 | B02 | B01 | B00 ] x [A00 | A00 | A00 | A00 ] = [ A00xB03 | A00xB02 | A00xB01 | A00xB00 ]
	//																		     +				
	// [ B13 | B12 | B11 | B10 ] x [A01 | A01 | A01 | A01 ] = [ A01xB13 | A01xB12 | A01xB11 | A01xB10 ]
	//																		     +				
	// [ B23 | B22 | B21 | B20 ] x [A02 | A02 | A02 | A02 ] = [ A02xB23 | A02xB22 | A02xB21 | A02xB20 ]
	//																		     +				
	// [ B33 | B32 | B31 | B30 ] x [  1 |   1 |   1 |   1 ]	= [   B33   |   B32   |   B31   |   B30   ]
	//																		     =				
	// [ A00xB03 + A01xB13 + A02xB23 + 1xB33 | A00xB02 + A01xB12 + A02xB22 + 1xB32 | A00xB01 + A01xB11 + A02xB21 + 1xB31 | A00xB00 + A01xB10 + A02xB20 + 1xB30]

	//vfmaq_laneq_f32(C , B, A, index)	<- B[0,1,2,3] x A[index] + C[0,1,2,3]

	float32x4_t B0 = vld1q_f32((float*)pMat);
	float32x4_t B1 = vld1q_f32((float*)pMat + 4);
	float32x4_t B2 = vld1q_f32((float*)pMat + 8);
	float32x4_t B3 = vld1q_f32((float*)pMat + 12);

	for (DWORD i = 0; i < dwNum; i++)
	{
		float32x2_t xy = vld1_f32(pv3Src);
		float32x2_t z = vld1_lane_f32(&pv3Src->z, vdup_n_f32(1), 0);
		float32x4_t src_xyz1 = vcombine_f32(xy, z);

		float32x4_t R = vmovq_n_f32(0);

		R = vfmaq_laneq_f32(R, B0, src_xyz1, 0);
		R = vfmaq_laneq_f32(R, B1, src_xyz1, 1);
		R = vfmaq_laneq_f32(R, B2, src_xyz1, 2);
		R = vfmaq_laneq_f32(R, B3, src_xyz1, 3);	//R = vaddq_f32(R, B3);로 바꿔도 되지만 성능향상이 제로

		vst1_f32(pv3Dest, vget_low_f32(R));	// write xy
		vst1q_lane_f32(&pv3Dest->z, R, 2);	// write z

		pv3Src++;
		pv3Dest++;
	}
}

void __stdcall TransformVector3_VPTR1_NEON(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum)
{
	// 1x4 matrix multiply
	//									B00 , B01, B02, B03
	// A00 , A01, A02, 1		x		B10 , B11, B12, B13		
	//									B20 , B21, B22, B23
	//									B30 , B31, B32, B33

	// = 
	// A00xB00 + A01xB10 + A02xB20 + 1xB30, A00xB01 + A01xB11 + A02xB21 + 1xB31, A00xB02 + A01xB12 + A02xB22 + 1xB32, A00xB03 + A01xB13 + A02xB23 + 1xB33

	// for Neon
	// registers
	// [ B03 | B02 | B01 | B00 ] x [A00 | A00 | A00 | A00 ] = [ A00xB03 | A00xB02 | A00xB01 | A00xB00 ]
	//																		     +				
	// [ B13 | B12 | B11 | B10 ] x [A01 | A01 | A01 | A01 ] = [ A01xB13 | A01xB12 | A01xB11 | A01xB10 ]
	//																		     +				
	// [ B23 | B22 | B21 | B20 ] x [A02 | A02 | A02 | A02 ] = [ A02xB23 | A02xB22 | A02xB21 | A02xB20 ]
	//																		     +				
	// [ B33 | B32 | B31 | B30 ] x [  1 |   1 |   1 |   1 ]	= [   B33   |   B32   |   B31   |   B30   ]
	//																		     =				
	// [ A00xB03 + A01xB13 + A02xB23 + 1xB33 | A00xB02 + A01xB12 + A02xB22 + 1xB32 | A00xB01 + A01xB11 + A02xB21 + 1xB31 | A00xB00 + A01xB10 + A02xB20 + 1xB30]

	//vfmaq_laneq_f32(C , B, A, index)	<- B[0,1,2,3] x A[index] + C[0,1,2,3]

	float32x4_t B0 = vld1q_f32((float*)pMat);
	float32x4_t B1 = vld1q_f32((float*)pMat + 4);
	float32x4_t B2 = vld1q_f32((float*)pMat + 8);
	float32x4_t B3 = vld1q_f32((float*)pMat + 12);

	for (DWORD i = 0; i < dwNum; i++)
	{
		float32x2_t xy = vld1_f32(pv3SrcDest);
		float32x2_t z = vld1_lane_f32(&pv3SrcDest->z, vdup_n_f32(1), 0);
		float32x4_t src_xyz1 = vcombine_f32(xy, z);

		//float32x4_t src_z = vsetq_lane_f32(pv3SrcDest->z, vdupq_n_f32(1), 0);
		//float32x4_t src_xyz1 = vcombine_f32(vld1_f32((const float32_t *)&pv3SrcDest->x), vget_low_f32(src_z));

		float32x4_t R = vmovq_n_f32(0);

		R = vfmaq_laneq_f32(R, B0, src_xyz1, 0);
		R = vfmaq_laneq_f32(R, B1, src_xyz1, 1);
		R = vfmaq_laneq_f32(R, B2, src_xyz1, 2);
		R = vfmaq_laneq_f32(R, B3, src_xyz1, 3);

		vst1_f32(pv3SrcDest, vget_low_f32(R));	// write xy
		vst1q_lane_f32(&pv3SrcDest->z, R, 2);	// write z
		//vst1q_lane_f32(&pv3SrcDest->x, R, 0);	// write x
		//vst1q_lane_f32(&pv3SrcDest->y, R, 1);	// write y
		//vst1q_lane_f32(&pv3SrcDest->z, R, 2);	// write z
		pv3SrcDest++;
	}
}
#endif
void __stdcall TransformVector4_VPTR2_NoSIMD(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum)
{
	for (DWORD i = 0; i < dwNum; i++)
	{
		pv4Dest->x = pv4Src->x * pMat->_11 + pv4Src->y * pMat->_21 + pv4Src->z * pMat->_31 + pv4Src->w * pMat->_41;
		pv4Dest->y = pv4Src->x * pMat->_12 + pv4Src->y * pMat->_22 + pv4Src->z * pMat->_32 + pv4Src->w * pMat->_42;
		pv4Dest->z = pv4Src->x * pMat->_13 + pv4Src->y * pMat->_23 + pv4Src->z * pMat->_33 + pv4Src->w * pMat->_43;
		pv4Dest->w = pv4Src->x * pMat->_14 + pv4Src->y * pMat->_24 + pv4Src->z * pMat->_34 + pv4Src->w * pMat->_44;

		pv4Src++;
		pv4Dest++;
	}
}
void __stdcall TransformVector4_VPTR2_SSE(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum)
{
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);

	for (DWORD i = 0; i < dwNum; i++)
	{
		__m128 src = _mm_loadu_ps((float*)pv4Src);

		__m128 xxxx = _mm_shuffle_ps(src, src, 0b00000000);
		__m128 yyyy = _mm_shuffle_ps(src, src, 0b01010101);
		__m128 zzzz = _mm_shuffle_ps(src, src, 0b10101010);
		__m128 wwww = _mm_shuffle_ps(src, src, 0b11111111);

		__m128 r0 = _mm_mul_ps(xxxx, m0);
		__m128 r1 = _mm_mul_ps(yyyy, m1);
		__m128 r2 = _mm_mul_ps(zzzz, m2);
		__m128 r3 = _mm_mul_ps(wwww, m3);

		r0 = _mm_add_ps(r0, r1);
		r2 = _mm_add_ps(r2, r3);
		r0 = _mm_add_ps(r0, r2);

		_mm_storeu_ps((float*)pv4Dest, r0);

		pv4Src++;
		pv4Dest++;
	}
}

#if defined(ARM64)

void __stdcall TransformVector4_VPTR2_NEON(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum)
{
	// 4 x4 matrix multiply
	// A00 , A01, A02, A03				B00 , B01, B02, B03
	// A10 , A11, A12, A13		x		B10 , B11, B12, B13		
	// A20 , A21, A22, A23				B20 , B21, B22, B23
	// A30 , A31, A32, A33				B30 , B31, B32, B33

	// = 
	// A00xB00 + A01xB10 + A02xB20 + A03xB30, A00xB01 + A01xB11 + A02xB21 + A03xB31, A00xB02 + A01xB12 + A02xB22 + A03xB32, A00xB03 + A01xB13 + A02xB23 + A03xB33

	// for Neon
	// registers
	// [ B03 | B02 | B01 | B00 ] x [A00 | A00 | A00 | A00 ] = [ A00xB03 | A00xB02 | A00xB01 | A00xB00 ]
	//																		     +				
	// [ B13 | B12 | B11 | B10 ] x [A01 | A01 | A01 | A01 ] = [ A01xB13 | A01xB12 | A01xB11 | A01xB10 ]
	//																		     +				
	// [ B23 | B22 | B21 | B20 ] x [A02 | A02 | A02 | A02 ] = [ A02xB23 | A02xB22 | A02xB21 | A02xB20 ]
	//																		     +				
	// [ B33 | B32 | B31 | B30 ] x [A03 | A03 | A03 | A03 ] = [ A03xB33 | A03xB32 | A03xB31 | A03xB30 ]
	//																		     =				
	// [ A00xB03 + A01xB13 + A02xB23 + A03xB33 | A00xB02 + A01xB12 + A02xB22 + A03xB32 | A00xB01 + A01xB11 + A02xB21 + A03xB31 | A00xB00 + A01xB10 + A02xB20 + A03xB30]

	//vfmaq_laneq_f32(C , B, A, index)	<- B[0,1,2,3] x A[index] + C[0,1,2,3]

	float32x4_t B0 = vld1q_f32((float*)pMat);
	float32x4_t B1 = vld1q_f32((float*)pMat + 4);
	float32x4_t B2 = vld1q_f32((float*)pMat + 8);
	float32x4_t B3 = vld1q_f32((float*)pMat + 12);

	for (DWORD i = 0; i < dwNum; i++)
	{
		float32x4_t A = vld1q_f32((float*)pv4Src);
		float32x4_t R = vmovq_n_f32(0);

		R = vfmaq_laneq_f32(R, B0, A, 0);
		R = vfmaq_laneq_f32(R, B1, A, 1);
		R = vfmaq_laneq_f32(R, B2, A, 2);
		R = vfmaq_laneq_f32(R, B3, A, 3);

		vst1q_f32((float*)pv4Dest, R);
		pv4Src++;
		pv4Dest++;
	}
}
#else
void __stdcall TransformVector4_VPTR2_SSE_1(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, DWORD dwNum)
{
	//
	// x86에선 전치하는 방식이 더 느리다.
	//
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);

	__m128 m0_T = Transpose(m0, m1, m2, m3, 0);
	__m128 m1_T = Transpose(m0, m1, m2, m3, 1);
	__m128 m2_T = Transpose(m0, m1, m2, m3, 2);
	__m128 m3_T = Transpose(m0, m1, m2, m3, 3);

	for (DWORD i = 0; i < dwNum; i++)
	{
		__m128 src = _mm_loadu_ps((float*)pv4Src);

		__m128 r0 = _mm_mul_ps(src, m0_T);
		__m128 r1 = _mm_mul_ps(src, m1_T);
		__m128 r2 = _mm_mul_ps(src, m2_T);
		__m128 r3 = _mm_mul_ps(src, m3_T);

		r0 = _mm_hadd_ps(r0, r0);
		r0 = _mm_hadd_ps(r0, r0);

		r1 = _mm_hadd_ps(r1, r1);
		r1 = _mm_hadd_ps(r1, r1);

		r2 = _mm_hadd_ps(r2, r2);
		r2 = _mm_hadd_ps(r2, r2);

		r3 = _mm_hadd_ps(r3, r3);
		r3 = _mm_hadd_ps(r3, r3);

		r0 = _mm_shuffle_ps(r0, r1, 0b00000000);	// r1.x | r1.x | r0.x | r0.x
		r2 = _mm_shuffle_ps(r2, r3, 0b00000000);	// r3.x | r3.x | r2.x | r2.x

		__m128 r = _mm_shuffle_ps(r0, r2, 0b10001000);
		_mm_storeu_ps((float*)pv4Dest, r);

		pv4Src++;
		pv4Dest++;
	}
}
#endif


void __stdcall TransformVector3_VPTR1_NoSIMD(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum)
{
	VECTOR3	v3Result;

	for (DWORD i = 0; i < dwNum; i++)
	{
		v3Result.x = pv3SrcDest->x * pMat->_11 + pv3SrcDest->y * pMat->_21 + pv3SrcDest->z * pMat->_31 + pMat->_41;
		v3Result.y = pv3SrcDest->x * pMat->_12 + pv3SrcDest->y * pMat->_22 + pv3SrcDest->z * pMat->_32 + pMat->_42;
		v3Result.z = pv3SrcDest->x * pMat->_13 + pv3SrcDest->y * pMat->_23 + pv3SrcDest->z * pMat->_33 + pMat->_43;

		*pv3SrcDest = v3Result;
		pv3SrcDest++;
	}
}
void __stdcall TransformVector3_VPTR1_SSE(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum)
{
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);

	//MOVLPS __m128 _mm_loadl_pi ( __m128 a, __m64 *p)
	//MOVLPS void _mm_storel_pi (__m64 *p, __m128 a)
	//MOVLHPS __m128 _mm_movelh_ps(__m128 a, __m128 b)
	__m128 zero = {};
	float one = 1.0f;

	for (DWORD i = 0; i < dwNum; i++)
	{
		__m128 src_xy = _mm_loadl_pi(zero, (__m64*)pv3SrcDest);
		__m128 src_z = _mm_load_ss(&pv3SrcDest->z);
		__m128 src_xyz0 = _mm_movelh_ps(src_xy, src_z);

		__m128 xxxx = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b00000000);
		__m128 yyyy = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b01010101);
		__m128 zzzz = _mm_shuffle_ps(src_xyz0, src_xyz0, 0b10101010);


		__m128 r0 = _mm_mul_ps(xxxx, m0);
		__m128 r1 = _mm_mul_ps(yyyy, m1);
		__m128 r2 = _mm_mul_ps(zzzz, m2);

		r0 = _mm_add_ps(r0, r1);
		r2 = _mm_add_ps(r2, m3);
		r0 = _mm_add_ps(r0, r2);

		_mm_storel_pi((__m64*)pv3SrcDest, r0);

		__m128 rz = _mm_movehl_ps(zero, r0);
		_mm_store_ss(&pv3SrcDest->z, rz);


		pv3SrcDest++;
	}
}

float __stdcall DotProduct_NoSIMD(const VECTOR3* pv3_0, const VECTOR3* pv3_1)
{
	float result;

	result = pv3_0->x * pv3_1->x + pv3_0->y * pv3_1->y + pv3_0->z * pv3_1->z;


	return result;
}
float __stdcall DotProduct_SSE(const VECTOR3* pv3_0, const VECTOR3* pv3_1)
{
	/*
	__m128 zero = {};
	__m128 xy00_0 = _mm_loadl_pi(zero, (__m64*) & pv3_0->x);
	__m128 xy00_1 = _mm_loadl_pi(zero, (__m64*) & pv3_1->x);
	xy00_0 = _mm_mul_ps(xy00_0, xy00_1);

	__m128 z000_0 = _mm_load_ss(&pv3_0->z);
	__m128 z000_1 = _mm_load_ss(&pv3_1->z);
	z000_0 = _mm_mul_ss(z000_0, z000_1);

	__m128 xyz0_0 = _mm_movelh_ps(xy00_0, z000_0);

	__m128 sum = _mm_hadd_ps(xyz0_0, xyz0_0);
	sum = _mm_hadd_ps(sum, sum);
	*/

	__m128 zero = _mm_set1_ps(0.0f);
	__m128 xy00_0 = _mm_loadl_pi(zero, (__m64*) & pv3_0->x);
	__m128 xy00_1 = _mm_loadl_pi(zero, (__m64*) & pv3_1->x);
	__m128 z000_1 = _mm_load_ss(&pv3_1->z);
	__m128 z000_0 = _mm_load_ss(&pv3_0->z);

	__m128 xyz0_0 = _mm_movelh_ps(xy00_0, z000_0);
	__m128 xyz0_1 = _mm_movelh_ps(xy00_1, z000_1);

	xyz0_0 = _mm_mul_ps(xyz0_0, xyz0_1);

	__m128 sum = _mm_hadd_ps(xyz0_0, xyz0_0);
	sum = _mm_hadd_ps(sum, sum);

	return sum.m128_f32[0];
}

void __stdcall Normalize_NoSIMD(VECTOR3* vn, const VECTOR3* v)
{
	float s = (float)sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z));

	if (0.0f != s)
	{
		s = 1.0f / s;
	}
	*vn = *v * s;
}
void __stdcall Normalize_SSE(VECTOR3* pv3OutN, const VECTOR3* pv3Src)
{
	__m128 zero = {};

	__m128 xy00 = _mm_loadl_pi(zero, (__m64*) & pv3Src->x);
	__m128 z000 = _mm_load_ss(&pv3Src->z);
	__m128 xyz0 = _mm_movelh_ps(xy00, z000);

	__m128 mag = _mm_mul_ps(xyz0, xyz0);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_sqrt_ps(mag);

	if (0.0f != mag.m128_f32[0])
	{
		mag = _mm_rcp_ps(mag);
	}

	xyz0 = _mm_mul_ps(xyz0, mag);

	_mm_storel_pi((__m64*)pv3OutN, xyz0);
	__m128 rz = _mm_movehl_ps(zero, xyz0);
	_mm_store_ss(&pv3OutN->z, rz);
}


void __stdcall MatrixMultiply2_NoSIMD(MATRIX4* pResult, const MATRIX4* pMat0, const MATRIX4* pMat1)
{
	MATRIX4	matResult;
	matResult._11 = (pMat0->_11 * pMat1->_11) + (pMat0->_12 * pMat1->_21) + (pMat0->_13 * pMat1->_31) + (pMat0->_14 * pMat1->_41);
	matResult._12 = (pMat0->_11 * pMat1->_12) + (pMat0->_12 * pMat1->_22) + (pMat0->_13 * pMat1->_32) + (pMat0->_14 * pMat1->_42);
	matResult._13 = (pMat0->_11 * pMat1->_13) + (pMat0->_12 * pMat1->_23) + (pMat0->_13 * pMat1->_33) + (pMat0->_14 * pMat1->_43);
	matResult._14 = (pMat0->_11 * pMat1->_14) + (pMat0->_12 * pMat1->_24) + (pMat0->_13 * pMat1->_34) + (pMat0->_14 * pMat1->_44);

	matResult._21 = (pMat0->_21 * pMat1->_11) + (pMat0->_22 * pMat1->_21) + (pMat0->_23 * pMat1->_31) + (pMat0->_24 * pMat1->_41);
	matResult._22 = (pMat0->_21 * pMat1->_12) + (pMat0->_22 * pMat1->_22) + (pMat0->_23 * pMat1->_32) + (pMat0->_24 * pMat1->_42);
	matResult._23 = (pMat0->_21 * pMat1->_13) + (pMat0->_22 * pMat1->_23) + (pMat0->_23 * pMat1->_33) + (pMat0->_24 * pMat1->_43);
	matResult._24 = (pMat0->_21 * pMat1->_14) + (pMat0->_22 * pMat1->_24) + (pMat0->_23 * pMat1->_34) + (pMat0->_24 * pMat1->_44);

	matResult._31 = (pMat0->_31 * pMat1->_11) + (pMat0->_32 * pMat1->_21) + (pMat0->_33 * pMat1->_31) + (pMat0->_34 * pMat1->_41);
	matResult._32 = (pMat0->_31 * pMat1->_12) + (pMat0->_32 * pMat1->_22) + (pMat0->_33 * pMat1->_32) + (pMat0->_34 * pMat1->_42);
	matResult._33 = (pMat0->_31 * pMat1->_13) + (pMat0->_32 * pMat1->_23) + (pMat0->_33 * pMat1->_33) + (pMat0->_34 * pMat1->_43);
	matResult._34 = (pMat0->_31 * pMat1->_14) + (pMat0->_32 * pMat1->_24) + (pMat0->_33 * pMat1->_34) + (pMat0->_34 * pMat1->_44);

	matResult._41 = (pMat0->_41 * pMat1->_11) + (pMat0->_42 * pMat1->_21) + (pMat0->_43 * pMat1->_31) + (pMat0->_44 * pMat1->_41);
	matResult._42 = (pMat0->_41 * pMat1->_12) + (pMat0->_42 * pMat1->_22) + (pMat0->_43 * pMat1->_32) + (pMat0->_44 * pMat1->_42);
	matResult._43 = (pMat0->_41 * pMat1->_13) + (pMat0->_42 * pMat1->_23) + (pMat0->_43 * pMat1->_33) + (pMat0->_44 * pMat1->_43);
	matResult._44 = (pMat0->_41 * pMat1->_14) + (pMat0->_42 * pMat1->_24) + (pMat0->_43 * pMat1->_34) + (pMat0->_44 * pMat1->_44);

	*pResult = matResult;

}
void __stdcall MatrixMultiply2_SSE(MATRIX4* pResult, const MATRIX4* pMat0, const MATRIX4* pMat1)
{
	__m128 m1_0_0123 = _mm_loadu_ps(&pMat1->_11);	// m1_0 =  pMat1->m[0][3] | pMat1->m[0][2] | pMat1->m[0][1] | pMat1->m[0][0]
	__m128 m1_1_0123 = _mm_loadu_ps(&pMat1->_21);	// m1_1 =  pMat1->m[1][3] | pMat1->m[1][2] | pMat1->m[1][1] | pMat1->m[1][0]
	__m128 m1_2_0123 = _mm_loadu_ps(&pMat1->_31);	// m1_2 =  pMat1->m[2][3] | pMat1->m[2][2] | pMat1->m[2][1] | pMat1->m[2][0]
	__m128 m1_3_0123 = _mm_loadu_ps(&pMat1->_41);	// m1_3 =  pMat1->m[3][3] | pMat1->m[3][2] | pMat1->m[3][1] | pMat1->m[3][0]	

	float*	pMat0_n = (float*)pMat0;
	float*	pDest = (float*)pResult;
	for (DWORD n = 0; n < 4; n++)
	{
		__m128 m0_n_0123 = _mm_loadu_ps(pMat0_n);				// m0_n =  pMat0_n->f[][3] | pMat0_n->f[][2] | pMat0_n->[][1] | pMat0_n->[][0]

		// pMat0의 N번째 라인
		__m128 m0_n_0000 = _mm_shuffle_ps(m0_n_0123, m0_n_0123, 0);		// m0_n_0000 = pMat0_n->m[n][0] | pMat0_n->m[n][0] | pMat0_n->m[n][0] | pMat0_n->m[n][0]
		__m128 m0_n_1111 = _mm_shuffle_ps(m0_n_0123, m0_n_0123, 85);	// m0_n_1111 = pMat0_n->m[n][1] | pMat0_n->m[n][1] | pMat0_n->m[n][1] | pMat0_n->m[n][1]
		__m128 m0_n_2222 = _mm_shuffle_ps(m0_n_0123, m0_n_0123, 170);	// m0_n_2222 = pMat0_n->m[n][2] | pMat0_n->m[n][2] | pMat0_n->m[n][2] | pMat0_n->m[n][2]
		__m128 m0_n_3333 = _mm_shuffle_ps(m0_n_0123, m0_n_0123, 255);	// m0_n_3333 = pMat0_n->m[n][3] | pMat0_n->m[n][3] | pMat0_n->m[n][3] | pMat0_n->m[n][3]

		__m128 r_0 = _mm_mul_ps(m0_n_0000, m1_0_0123); // pMat0_n->m[n][0] * pMat1->m[0][3] | pMat0_n->m[n][0] * pMat1->m[0][2] | pMat0_n->m[n][0] * pMat1->m[0][1] | pMat0_n->m[n][0] * pMat1->m[0][0]
		__m128 r_1 = _mm_mul_ps(m0_n_1111, m1_1_0123); // pMat0_n->m[n][1] * pMat1->m[1][3] | pMat0_n->m[n][1] * pMat1->m[1][2] | pMat0_n->m[n][1] * pMat1->m[1][1] | pMat0_n->m[n][1] * pMat1->m[1][0]
		__m128 r_2 = _mm_mul_ps(m0_n_2222, m1_2_0123); // pMat0_n->m[n][2] * pMat1->m[2][3] | pMat0_n->m[n][2] * pMat1->m[2][2] | pMat0_n->m[n][2] * pMat1->m[2][1] | pMat0_n->m[n][2] * pMat1->m[2][0]
		__m128 r_3 = _mm_mul_ps(m0_n_3333, m1_3_0123); // pMat0_n->m[n][3] * pMat1->m[3][3] | pMat0_n->m[n][3] * pMat1->m[3][2] | pMat0_n->m[n][3] * pMat1->m[3][1] | pMat0_n->m[n][3] * pMat1->m[3][0]

		r_0 = _mm_add_ps(r_0, r_1);
		r_2 = _mm_add_ps(r_2, r_3);
		r_0 = _mm_add_ps(r_0, r_2);

		_mm_storeu_ps(pDest, r_0);

		pMat0_n += 4;
		pDest += 4;
	}
}
void __stdcall CrossProduct_NoSIMD(VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
{
	r->x = u->y * v->z - u->z * v->y;
	r->y = u->z * v->x - u->x * v->z;
	r->z = u->x * v->y - u->y * v->x;
}
void __stdcall CrossProduct_SSE(VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
{
	__m128 zero = _mm_setzero_ps();

	__m128 u_xy00 = _mm_loadl_pi(zero, (__m64*) & u->x);
	__m128 u_z000 = _mm_load_ss(&u->z);
	__m128 u_xyz0 = _mm_movelh_ps(u_xy00, u_z000);

	__m128 v_xy00 = _mm_loadl_pi(zero, (__m64*) & v->x);
	__m128 v_z000 = _mm_load_ss(&v->z);
	__m128 v_xyz0 = _mm_movelh_ps(v_xy00, v_z000);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	__m128 u_yzzx = _mm_shuffle_ps(u_xyz0, u_xyz0, 41);
	__m128 v_zxyz = _mm_shuffle_ps(v_xyz0, v_xyz0, 146);

	__m128 t0 = _mm_mul_ps(u_yzzx, v_zxyz);	// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z	
	__m128 t1 = _mm_movehl_ps(zero, t0);	// |			 |			  |	u->x*v->z |	u->z*v->y
	__m128 r_xy = _mm_sub_ps(t0, t1);		// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	__m128 ux__uy = _mm_shuffle_ps(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x	
	__m128 vy__vx = _mm_shuffle_ps(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y	
	t0 = _mm_mul_ps(ux__uy, vy__vx);					//	|			  |	u->y*v->x |			  | u->x*v->y 
	t1 = _mm_movehl_ps(zero, t0);						//	|			  |			  | 		  | u->y*v->x
	__m128 r_z = _mm_sub_ss(t0, t1);					//  |			  |           |           | u->x*v->y - u->y*v->x				

	_mm_storel_pi((__m64*)r, r_xy);
	_mm_store_ss(&r->z, r_z);
}



float __stdcall CalcDistance_NoSIMD(const VECTOR3* pv3Start, const VECTOR3* pv3End)
{
	float dx = (pv3End->x - pv3Start->x);
	float dy = (pv3End->y - pv3Start->y);
	float dz = (pv3End->z - pv3Start->z);

	float	dist = (float)sqrtf((dx * dx + dy * dy + dz * dz));
	return dist;
}
float __stdcall CalcDistance_SSE(const VECTOR3* pv3Start, const VECTOR3* pv3End)
{
	__m128 zero = {};

	__m128 u_xy00 = _mm_loadl_pi(zero, (__m64*) & pv3Start->x);
	__m128 u_z000 = _mm_load_ss(&pv3Start->z);
	__m128 u_xyz0 = _mm_movelh_ps(u_xy00, u_z000);

	__m128 v_xy00 = _mm_loadl_pi(zero, (__m64*) & pv3End->x);
	__m128 v_z000 = _mm_load_ss(&pv3End->z);
	__m128 v_xyz0 = _mm_movelh_ps(v_xy00, v_z000);

	__m128 dir = _mm_sub_ps(u_xyz0, v_xyz0);
	__m128 mag = _mm_mul_ps(dir, dir);

	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_hadd_ps(mag, mag);
	__m128 r = _mm_sqrt_ss(mag);

	return r.m128_f32[0];
}

BOOL __stdcall CalcPlaneEquation_NoSIMD(PLANE* pPlane, const VECTOR3* pv3Tri)
{
	BOOL		bResult = FALSE;

	VECTOR3	u = pv3Tri[1] - pv3Tri[0];
	VECTOR3	v = pv3Tri[2] - pv3Tri[0];

	VECTOR3	r;
	CrossProduct_NoSIMD(&r, &u, &v);

	if (r.x == 0.0f && r.y == 0.0f && r.z == 0.0f)
		goto lb_return;

	Normalize_NoSIMD(&r, &r);
	pPlane->v3Up = r;
	pPlane->D = -(pv3Tri->x * r.x + pv3Tri->y * r.y + pv3Tri->z * r.z);
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL __stdcall CalcPlaneEquation_SSE(PLANE* pPlane, const VECTOR3* pv3Tri)
{
	BOOL bResult = FALSE;

	__m128 zero = {};
	__m128 w_mask = _mm_setr_ps(1.0f, 1.0f, 1.0f, 0.0f);
	__m128 neg = _mm_setr_ps(-1.0f, 0.0f, 0.0f, 0.0f);

	TRIANGLE*	pTri = (TRIANGLE*)pv3Tri;
	__m128 v0_xy = _mm_loadl_pi(zero, (__m64*)(pTri->v3Point + 0));
	__m128 v1_xy = _mm_loadl_pi(zero, (__m64*)(pTri->v3Point + 1));
	__m128 v2_xy = _mm_loadl_pi(zero, (__m64*)(pTri->v3Point + 2));

	__m128 v0_z = _mm_load_ss(&(pTri->v3Point + 0)->z);
	__m128 v1_z = _mm_load_ss(&(pTri->v3Point + 1)->z);
	__m128 v2_z = _mm_load_ss(&(pTri->v3Point + 2)->z);

	__m128 v0 = _mm_movelh_ps(v0_xy, v0_z);
	__m128 v1 = _mm_movelh_ps(v1_xy, v1_z);
	__m128 v2 = _mm_movelh_ps(v2_xy, v2_z);

	// 삼각형의 세점 v0, v1, v2

	//VECTOR3	u = v1 - v0
	//VECTOR3	v = v2 - v0
	__m128 u_xyz0 = _mm_sub_ps(v1, v0);
	__m128 v_xyz0 = _mm_sub_ps(v2, v0);

	//VECTOR3	r;
	//CrossProduct(&r, &u, &v);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	__m128 u_yzzx = _mm_shuffle_ps(u_xyz0, u_xyz0, 41);
	__m128 v_zxyz = _mm_shuffle_ps(v_xyz0, v_xyz0, 146);

	__m128 t0 = _mm_mul_ps(u_yzzx, v_zxyz);	// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z	
	__m128 t1 = _mm_movehl_ps(zero, t0);	// |			 |			  |	u->x*v->z |	u->z*v->y
	__m128 r_xy = _mm_sub_ps(t0, t1);		// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	__m128 ux__uy = _mm_shuffle_ps(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x	
	__m128 vy__vx = _mm_shuffle_ps(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y	
	t0 = _mm_mul_ps(ux__uy, vy__vx);					//	|			  |	u->y*v->x |			  | u->x*v->y 
	t1 = _mm_movehl_ps(zero, t0);						//	|			  |			  | 		  | u->y*v->x
	__m128 r_z = _mm_sub_ss(t0, t1);					//  |			  |           |           | u->x*v->y - u->y*v->x				

	// CrossProduct(u,v)결과 r
	__m128 r = _mm_movelh_ps(r_xy, r_z);

	if (r.m128_f32[0] == 0.0f && r.m128_f32[1] == 0.0f && r.m128_f32[2] == 0.0f)
		goto lb_return;

	// r.w 가 0인지 확인할것
	r = _mm_mul_ps(r, w_mask);

	// Normalize(r)
	__m128 mag = _mm_mul_ps(r, r);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_sqrt_ps(mag);

	if (0.0f != mag.m128_f32[0])
	{
		mag = _mm_rcp_ps(mag);
	}
	__m128 N = _mm_mul_ps(r, mag);


	// float D = DotProduct(N,v0);
	__m128 D = _mm_mul_ps(N, v0);
	D = _mm_hadd_ps(D, D);
	D = _mm_hadd_ps(D, D);
	D = _mm_mul_ss(D, neg);

	_mm_storeu_ps((float*)pPlane, N);
	_mm_store_ss(&pPlane->D, D);

	bResult = FALSE;

lb_return:
	return bResult;
}
#if defined(ARM64)
BOOL __stdcall CalcPlaneEquation_NEON(PLANE* pPlane, const VECTOR3* pv3Tri)
{
	BOOL		bResult = FALSE;

	VECTOR3	u = pv3Tri[1] - pv3Tri[0];
	VECTOR3	v = pv3Tri[2] - pv3Tri[0];

	VECTOR3	r;
	CrossProduct_NEON(&r, &u, &v);

	if (r.x == 0.0f && r.y == 0.0f && r.z == 0.0f)
		goto lb_return;

	Normalize_NEON(&r, &r);
	pPlane->v3Up = r;
	pPlane->D = -(pv3Tri->x * r.x + pv3Tri->y * r.y + pv3Tri->z * r.z);
	bResult = TRUE;

lb_return:
	return bResult;

}
#endif

BOOL __stdcall CalcPlaneEquationWithPtr_NoSIMD(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2)
{
	BOOL	bResult = TRUE;
	VECTOR3 u, v, r;

	u.x = pv3Point1->x - pv3Point0->x;
	u.y = pv3Point1->y - pv3Point0->y;
	u.z = pv3Point1->z - pv3Point0->z;

	v.x = pv3Point2->x - pv3Point0->x;
	v.y = pv3Point2->y - pv3Point0->y;
	v.z = pv3Point2->z - pv3Point0->z;

	CrossProduct_NoSIMD(&r, &u, &v);
	if (r.x == 0.0f && r.y == 0.0f && r.z == 0.0f)
	{
		pPlane->v3Up.x = 0.0f;
		pPlane->v3Up.y = 0.0f;
		pPlane->v3Up.z = 0.0f;
		pPlane->D = 0.0f;

		bResult = FALSE;
		goto lb_return;
	}

	Normalize_NoSIMD(&r, &r);
	pPlane->v3Up = r;
	pPlane->D = -(pv3Point0->x * r.x + pv3Point0->y * r.y + pv3Point0->z * r.z);

lb_return:
	return bResult;
}
BOOL __stdcall CalcPlaneEquationWithPtr_SSE(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2)
{
	BOOL bResult = FALSE;

	__m128 zero = {};
	__m128 w_mask = _mm_setr_ps(1.0f, 1.0f, 1.0f, 0.0f);
	__m128 neg = _mm_setr_ps(-1.0f, 0.0f, 0.0f, 0.0f);

	__m128 v0_xy = _mm_loadl_pi(zero, (__m64*)(pv3Point0));
	__m128 v1_xy = _mm_loadl_pi(zero, (__m64*)(pv3Point1));
	__m128 v2_xy = _mm_loadl_pi(zero, (__m64*)(pv3Point2));

	__m128 v0_z = _mm_load_ss(&(pv3Point0)->z);
	__m128 v1_z = _mm_load_ss(&(pv3Point1)->z);
	__m128 v2_z = _mm_load_ss(&(pv3Point2)->z);

	__m128 v0 = _mm_movelh_ps(v0_xy, v0_z);
	__m128 v1 = _mm_movelh_ps(v1_xy, v1_z);
	__m128 v2 = _mm_movelh_ps(v2_xy, v2_z);

	// 삼각형의 세점 v0, v1, v2

	//VECTOR3	u = v1 - v0
	//VECTOR3	v = v2 - v0
	__m128 u_xyz0 = _mm_sub_ps(v1, v0);
	__m128 v_xyz0 = _mm_sub_ps(v2, v0);

	//VECTOR3	r;
	//CrossProduct(&r, &u, &v);

	// x,y성분
	// r->x = u->y*v->z - u->z*v->y;
	// r->y = u->z*v->x - u->x*v->z;
	__m128 u_yzzx = _mm_shuffle_ps(u_xyz0, u_xyz0, 41);
	__m128 v_zxyz = _mm_shuffle_ps(v_xyz0, v_xyz0, 146);

	__m128 t0 = _mm_mul_ps(u_yzzx, v_zxyz);	// | u->x*v->z	 |	u->z*v->y | u->z*v->x |	u->y*v->z	
	__m128 t1 = _mm_movehl_ps(zero, t0);	// |			 |			  |	u->x*v->z |	u->z*v->y
	__m128 r_xy = _mm_sub_ps(t0, t1);		// |    ?		 |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	// z성분
	// r->z = u->x*v->y - u->y*v->x;
	__m128 ux__uy = _mm_shuffle_ps(u_xyz0, u_xyz0, 16);	//	|			  |	u->y	  |			  |	u->x	
	__m128 vy__vx = _mm_shuffle_ps(v_xyz0, v_xyz0, 5);	//	|			  |	v->x	  |			  |	v->y	
	t0 = _mm_mul_ps(ux__uy, vy__vx);					//	|			  |	u->y*v->x |			  | u->x*v->y 
	t1 = _mm_movehl_ps(zero, t0);						//	|			  |			  | 		  | u->y*v->x
	__m128 r_z = _mm_sub_ss(t0, t1);					//  |			  |           |           | u->x*v->y - u->y*v->x				

	// CrossProduct(u,v)결과 r
	__m128 r = _mm_movelh_ps(r_xy, r_z);

	if (r.m128_f32[0] == 0.0f && r.m128_f32[1] == 0.0f && r.m128_f32[2] == 0.0f)
		goto lb_return;

	// r.w 가 0인지 확인할것
	r = _mm_mul_ps(r, w_mask);

	// Normalize(r)
	__m128 mag = _mm_mul_ps(r, r);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_hadd_ps(mag, mag);
	mag = _mm_sqrt_ps(mag);

	if (0.0f != mag.m128_f32[0])
	{
		mag = _mm_rcp_ps(mag);
	}
	__m128 N = _mm_mul_ps(r, mag);


	// float D = DotProduct(N,v0);
	__m128 D = _mm_mul_ps(N, v0);
	D = _mm_hadd_ps(D, D);
	D = _mm_hadd_ps(D, D);
	D = _mm_mul_ss(D, neg);

	_mm_storeu_ps((float*)pPlane, N);
	_mm_store_ss(&pPlane->D, D);

	bResult = FALSE;

lb_return:
	return bResult;
}
#if defined(ARM64)
BOOL __stdcall CalcPlaneEquationWithPtr_NEON(PLANE* pPlane, const VECTOR3* pv3Point0, const VECTOR3* pv3Point1, const VECTOR3* pv3Point2)
{
	BOOL	bResult = TRUE;
	VECTOR3 u, v, r;

	u.x = pv3Point1->x - pv3Point0->x;
	u.y = pv3Point1->y - pv3Point0->y;
	u.z = pv3Point1->z - pv3Point0->z;

	v.x = pv3Point2->x - pv3Point0->x;
	v.y = pv3Point2->y - pv3Point0->y;
	v.z = pv3Point2->z - pv3Point0->z;

	CrossProduct_NEON(&r, &u, &v);
	if (r.x == 0.0f && r.y == 0.0f && r.z == 0.0f)
	{
		pPlane->v3Up.x = 0.0f;
		pPlane->v3Up.y = 0.0f;
		pPlane->v3Up.z = 0.0f;
		pPlane->D = 0.0f;

		bResult = FALSE;
		goto lb_return;
	}

	Normalize_NEON(&r, &r);
	pPlane->v3Up = r;
	pPlane->D = -(pv3Point0->x * r.x + pv3Point0->y * r.y + pv3Point0->z * r.z);

lb_return:
	return bResult;
}
#endif
BOOL __stdcall ClipTriWithPlaneList_NoSIMD(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri)
{
	BOOL		bResult = FALSE;

	float		D;
	// 뷰볼륨 클리핑.
	for (DWORD i = 0; i < dwPlaneNum; i++)
	{

		D = DotProduct(&pPlaneList[i].v3Up, &pTri->v3Point[0]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;

		D = DotProduct(&pPlaneList[i].v3Up, &pTri->v3Point[1]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;

		D = DotProduct(&pPlaneList[i].v3Up, &pTri->v3Point[2]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;


		goto lb_return;
	lb_next_plane:
		__noop();
	}


	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall ClipTriWithPlaneList_SSE(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri)
{
	BOOL		bResult = FALSE;

	__m128 zero = {};
	__m128 w_one = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

	// Load Tri
	__m128 v0_xy = _mm_loadl_pi(zero, (__m64*) & (pTri->v3Point + 0)->x);
	__m128 v0_z = _mm_load_ss(&(pTri->v3Point + 0)->z);
	__m128 v0_xyz1 = _mm_movelh_ps(v0_xy, v0_z);
	v0_xyz1 = _mm_add_ps(v0_xyz1, w_one);	// (pTri->v3Point[0].x, pTri->v3Point[0].y, pTri->v3Point[0].z, 1)

	__m128 v1_xy = _mm_loadl_pi(zero, (__m64*) & (pTri->v3Point + 1)->x);
	__m128 v1_z = _mm_load_ss(&(pTri->v3Point + 1)->z);
	__m128 v1_xyz1 = _mm_movelh_ps(v1_xy, v1_z);
	v1_xyz1 = _mm_add_ps(v1_xyz1, w_one);	// (pTri->v3Point[1].x, pTri->v3Point[1].y, pTri->v3Point[1].z, 1)

	__m128 v2_xy = _mm_loadl_pi(zero, (__m64*) & (pTri->v3Point + 2)->x);
	__m128 v2_z = _mm_load_ss(&(pTri->v3Point + 2)->z);
	__m128 v2_xyz1 = _mm_movelh_ps(v2_xy, v2_z);
	v2_xyz1 = _mm_add_ps(v2_xyz1, w_one);	// (pTri->v3Point[2].x, pTri->v3Point[2].y, pTri->v3Point[2].z, 1)

	for (DWORD i = 0; i < dwPlaneNum; i++)
	{
		// Load pPlaneList[i]
		__m128 plane = _mm_loadu_ps((float*)(pPlaneList + i));

		__m128 D = _mm_mul_ps(v0_xyz1, plane);
		D = _mm_hadd_ps(D, D);
		D = _mm_hadd_ps(D, D);
		if (D.m128_f32[0] <= 0.0f)
			goto lb_next_plane;

		D = _mm_mul_ps(v1_xyz1, plane);
		D = _mm_hadd_ps(D, D);
		D = _mm_hadd_ps(D, D);
		if (D.m128_f32[0] <= 0.0f)
			goto lb_next_plane;

		D = _mm_mul_ps(v2_xyz1, plane);
		D = _mm_hadd_ps(D, D);
		D = _mm_hadd_ps(D, D);
		if (D.m128_f32[0] <= 0.0f)
			goto lb_next_plane;

		goto lb_return;
	lb_next_plane:
		__noop();
	}
	bResult = TRUE;

lb_return:
	return bResult;
}
#if defined(ARM64)
BOOL __stdcall ClipTriWithPlaneList_NEON(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri)
{
	BOOL		bResult = FALSE;

	float		D;
	// 뷰볼륨 클리핑.
	for (DWORD i = 0; i < dwPlaneNum; i++)
	{

		D = DotProduct_NEON(&pPlaneList[i].v3Up, &pTri->v3Point[0]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;

		D = DotProduct_NEON(&pPlaneList[i].v3Up, &pTri->v3Point[1]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;

		D = DotProduct_NEON(&pPlaneList[i].v3Up, &pTri->v3Point[2]) + pPlaneList[i].D;
		if (D <= 0)
			goto lb_next_plane;


		goto lb_return;
	lb_next_plane:
		__noop();
	}


	bResult = TRUE;

lb_return:
	return bResult;
}
#endif

BOOL __stdcall SetIdentityMatrix(MATRIX4* pOutMat)
{
	pOutMat->_12 = pOutMat->_13 = pOutMat->_14 = pOutMat->_21 = pOutMat->_23 = pOutMat->_24 = 0.0f;
	pOutMat->_31 = pOutMat->_32 = pOutMat->_34 = pOutMat->_41 = pOutMat->_42 = pOutMat->_43 = 0.0f;
	pOutMat->_11 = pOutMat->_22 = pOutMat->_33 = pOutMat->_44 = 1.0f;
	return TRUE;
}
