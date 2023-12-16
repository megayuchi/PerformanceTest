#pragma once

#if defined(_M_ARM64) || defined(_M_ARM64EC)
#undef __VECTORCALL
#define __VECTORCALL 
#else
#define __VECTORCALL __vectorcall
#endif

#if defined(_M_ARM64) || defined(_M_ARM64EC)
struct __declspec(align(16)) VECTOR4_SIMD
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			int32x4_t m;
		};
	};
	inline void Set(float x, float y, float z, float w)
	{
		float __declspec(align(16)) f[4] = { x, y, z, w };
		this->m = vld1q_f32(f);
	}
	inline void Set(const float* A)
	{
		this->m = vld1q_f32(A);
	}
	inline VECTOR4_SIMD __VECTORCALL operator+(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = vaddq_f32(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator-(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = vsubq_f32(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator*(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = vmulq_f32(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator/(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = vdivq_f32(this->m, v.m);
		return r;
	}

};
#else
struct __declspec(align(16)) VECTOR4_SIMD
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		__m128	m;
	};
	inline void Set(float x, float y, float z, float w)
	{
		this->m = _mm_setr_ps(x, y, z, w);

	}
	inline void Set(const float* A)
	{
		this->m = _mm_load_ps(A);
	}
	inline VECTOR4_SIMD __VECTORCALL operator+(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = _mm_add_ps(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator-(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = _mm_sub_ps(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator*(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = _mm_mul_ps(this->m, v.m);
		return r;
	}
	inline VECTOR4_SIMD __VECTORCALL operator/(const VECTOR4_SIMD& v) const
	{
		VECTOR4_SIMD r;
		r.m = _mm_div_ps(this->m, v.m);
		return r;
	}

};
#endif

struct VECTOR4 : VECTOR4_SIMD
{
	inline void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	inline void Set(const float* A)
	{
		this->x = A[0];
		this->y = A[1];
		this->z = A[2];
		this->w = A[3];
	}
	inline VECTOR4 operator+(const VECTOR4& v) const
	{
		VECTOR4 r;
		r.x = this->x + v.x;
		r.y = this->y + v.y;
		r.z = this->z + v.z;
		r.w = this->w + v.w;
		return r;
	}
	inline VECTOR4 operator-(const VECTOR4& v) const
	{
		VECTOR4 r;
		r.x = this->x - v.x;
		r.y = this->y - v.y;
		r.z = this->z - v.z;
		r.w = this->w - v.w;
		return r;
	}
	inline VECTOR4 operator*(const VECTOR4& v) const
	{
		VECTOR4 r;
		r.x = this->x * v.x;
		r.y = this->y * v.y;
		r.z = this->z * v.z;
		r.w = this->w * v.w;
		return r;
	}
	inline VECTOR4 operator/(const VECTOR4& v) const
	{
		VECTOR4 r;
		r.x = this->x / v.x;
		r.y = this->y / v.y;
		r.z = this->z / v.z;
		r.w = this->w / v.w;
		return r;
	}
};

void FillVectorStream(VECTOR4_SIMD* pDest, DWORD dwCount);
void AddVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount);
void AddVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount);
void MulVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount);
void MulVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount);
void DivVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount);
void DivVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount);

void AddOrSubOrMulOrDivVectorStream_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount);
void AddOrSubOrMulOrDivVectorStream(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount);

void AddOrSubOrMulOrDivVectorStream_Random_SIMD(VECTOR4_SIMD* pDest, const VECTOR4_SIMD* pSrc0, const VECTOR4_SIMD* pSrc1, DWORD dwCount);
void AddOrSubOrMulOrDivVectorStream_Random(VECTOR4* pDest, const VECTOR4* pSrc0, const VECTOR4* pSrc1, DWORD dwCount);
