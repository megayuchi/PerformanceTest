INCLUDE math_type_x64.inc


.code


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_ADD_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, const VECTOR3* pv3Arg2)
VECTOR3_ADD_VECTOR3	PROC

	; LOAD VECTOR 1 , LOAD VECTOR 2
	movlps		xmm0,qword ptr[rdx]
	movlps		xmm1,qword ptr[r8]
	
	movss		xmm2,dword ptr[rdx+8]
	movss		xmm3,dword ptr[r8+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	
	addps		xmm0,xmm1
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z
	
	ret
VECTOR3_ADD_VECTOR3	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_SUB_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, const VECTOR3* pv3Arg2)
VECTOR3_SUB_VECTOR3	PROC

	; LOAD VECTOR 1 , LOAD VECTOR 2
	movlps		xmm0,qword ptr[rdx]
	movlps		xmm1,qword ptr[r8]
	
	movss		xmm2,dword ptr[rdx+8]
	movss		xmm3,dword ptr[r8+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	
	subps		xmm0,xmm1
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z
	
	ret
	
VECTOR3_SUB_VECTOR3	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_MUL_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, const VECTOR3* pv3Arg2)
VECTOR3_MUL_VECTOR3	PROC

		; LOAD VECTOR 1 , LOAD VECTOR 2
	movlps		xmm0,qword ptr[rdx]
	movlps		xmm1,qword ptr[r8]
	
	movss		xmm2,dword ptr[rdx+8]
	movss		xmm3,dword ptr[r8+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	
	mulps		xmm0,xmm1
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z

	ret

VECTOR3_MUL_VECTOR3	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_DIV_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, const VECTOR3* pv3Arg2)
VECTOR3_DIV_VECTOR3	PROC

		; LOAD VECTOR 1 , LOAD VECTOR 2
	movlps		xmm0,qword ptr[rdx]
	movlps		xmm1,qword ptr[r8]
	
	movss		xmm2,dword ptr[rdx+8]
	movss		xmm3,dword ptr[r8+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	
	divps		xmm0,xmm1
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z

	ret
	
VECTOR3_DIV_VECTOR3	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_ADDEQU_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1)
VECTOR3_ADDEQU_VECTOR3	PROC

	; rcx = pv3Result
	; rdx = pv3Arg1
	
	; LOAD VECTOR 1, LOAD VECTOR 2
	
	movlps		xmm0,qword ptr[rcx]
	movlps		xmm1,qword ptr[rdx]
	
	movss		xmm2,dword ptr[rcx+8]
	movss		xmm3,dword ptr[rdx+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	addps		xmm0,xmm1
	
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z
	
	ret

VECTOR3_ADDEQU_VECTOR3	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_SUBEQU_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1)
VECTOR3_SUBEQU_VECTOR3	PROC

	; rcx = pv3Result
	; rdx = pv3Arg1
	
	; LOAD VECTOR 1, LOAD VECTOR 2
	
	movlps		xmm0,qword ptr[rcx]
	movlps		xmm1,qword ptr[rdx]
	
	movss		xmm2,dword ptr[rcx+8]
	movss		xmm3,dword ptr[rdx+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	subps		xmm0,xmm1
	
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z
	
	ret

VECTOR3_SUBEQU_VECTOR3	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_MULEQU_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1)
VECTOR3_MULEQU_VECTOR3	PROC

	; rcx = pv3Result
	; rdx = pv3Arg1
	
	; LOAD VECTOR 1, LOAD VECTOR 2
	
	movlps		xmm0,qword ptr[rcx]
	movlps		xmm1,qword ptr[rdx]
	
	movss		xmm2,dword ptr[rcx+8]
	movss		xmm3,dword ptr[rdx+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	mulps		xmm0,xmm1
	
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z

	ret

VECTOR3_MULEQU_VECTOR3	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_DIVEQU_VECTOR3(VECTOR3* pv3Result, const VECTOR3* pv3Arg1)
VECTOR3_DIVEQU_VECTOR3	PROC

	; rcx = pv3Result
	; rdx = pv3Arg1
	
	; LOAD VECTOR 1, LOAD VECTOR 2
	
	movlps		xmm0,qword ptr[rcx]
	movlps		xmm1,qword ptr[rdx]
	
	movss		xmm2,dword ptr[rcx+8]
	movss		xmm3,dword ptr[rdx+8]
	
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	divps		xmm0,xmm1
	
	movhlps		xmm1,xmm0					; z
	movlps		qword ptr[rcx],xmm0					; write xy
	movss		dword ptr[rcx+8],xmm1				; write z

	ret

VECTOR3_DIVEQU_VECTOR3	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_ADDEQU_FLOAT(VECTOR3* pv3Result, float fVal)
VECTOR3_ADDEQU_FLOAT	PROC	

	; rcx = pv3Result
	; xmm1 = fVal
	
	movlps		xmm0,qword ptr[rcx]
	movss		xmm2,dword ptr[rcx+8]
	movlhps		xmm0,xmm2
	
	shufps		xmm1,xmm1,0
	
	addps		xmm0,xmm1
	
	movhlps		xmm2,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm2
	
	ret
VECTOR3_ADDEQU_FLOAT	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_SUBEQU_FLOAT(VECTOR3* pv3Result, float fVal)
VECTOR3_SUBEQU_FLOAT	PROC	

	; rcx = pv3Result
	; xmm1 = fVal
	
	movlps		xmm0,qword ptr[rcx]
	movss		xmm2,dword ptr[rcx+8]
	movlhps		xmm0,xmm2
	
	shufps		xmm1,xmm1,0
	
	subps		xmm0,xmm1
	
	movhlps		xmm2,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm2
	
	ret
VECTOR3_SUBEQU_FLOAT	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_MULEQU_FLOAT(VECTOR3* pv3Result, float fVal)
VECTOR3_MULEQU_FLOAT	PROC	

	; rcx = pv3Result
	; xmm1 = fVal
	
	movlps		xmm0,qword ptr[rcx]
	movss		xmm2,dword ptr[rcx+8]
	movlhps		xmm0,xmm2
	
	shufps		xmm1,xmm1,0
	
	mulps		xmm0,xmm1
	
	movhlps		xmm2,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm2
	
	ret
VECTOR3_MULEQU_FLOAT	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_DIVEQU_FLOAT(VECTOR3* pv3Result, float fVal)
VECTOR3_DIVEQU_FLOAT	PROC	

	; rcx = pv3Result
	; xmm1 = fVal
	
	movlps		xmm0,qword ptr[rcx]
	movss		xmm2,dword ptr[rcx+8]
	movlhps		xmm0,xmm2
	
	shufps		xmm1,xmm1,0
	
	divps		xmm0,xmm1
	
	movhlps		xmm2,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm2
	
	ret
VECTOR3_DIVEQU_FLOAT	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_ADD_FLOAT(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, float fVal)
VECTOR3_ADD_FLOAT	PROC

	movlps		xmm0,qword ptr[rdx]
	movss		xmm1,dword ptr[rdx+8]
	
	shufps		xmm2,xmm2,0
	movlhps		xmm0,xmm1
		
	addps		xmm0,xmm2
	
	movhlps		xmm1,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm1
	
	ret
	
VECTOR3_ADD_FLOAT	ENDP


;GLOBAL_FUNC_DLL void __stdcall VECTOR3_SUB_FLOAT(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, float fVal)
VECTOR3_SUB_FLOAT	PROC

	movlps		xmm0,qword ptr[rdx]
	movss		xmm1,dword ptr[rdx+8]
	
	shufps		xmm2,xmm2,0
	movlhps		xmm0,xmm1
		
	subps		xmm0,xmm2
	
	movhlps		xmm1,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm1
	
	ret
	
VECTOR3_SUB_FLOAT	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_MUL_FLOAT(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, float fVal)
VECTOR3_MUL_FLOAT	PROC

	movlps		xmm0,qword ptr[rdx]
	movss		xmm1,dword ptr[rdx+8]
	
	shufps		xmm2,xmm2,0
	movlhps		xmm0,xmm1
		
	mulps		xmm0,xmm2
	
	movhlps		xmm1,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm1
	
	ret
	
VECTOR3_MUL_FLOAT	ENDP

;GLOBAL_FUNC_DLL void __stdcall VECTOR3_DIV_FLOAT(VECTOR3* pv3Result, const VECTOR3* pv3Arg1, float fVal)
VECTOR3_DIV_FLOAT	PROC

	movlps		xmm0,qword ptr[rdx]
	movss		xmm1,dword ptr[rdx+8]
	
	shufps		xmm2,xmm2,0
	movlhps		xmm0,xmm1
		
	divps		xmm0,xmm2
	
	movhlps		xmm1,xmm0
	movlps		qword ptr[rcx],xmm0
	movss		dword ptr[rcx+8],xmm1
	
	ret
	
VECTOR3_DIV_FLOAT	ENDP

;GLOBAL_FUNC_DLL float __stdcall DotProduct(const VECTOR3* pv3_0, const VECTOR3* pv3_1)
DotProduct		PROC

	; LOAD VECTOR1 , LOAD VECTOR2
	movlps	xmm0,qword ptr[rcx]			
	movlps	xmm1,qword ptr[rdx]			
	
	movss	xmm2,dword ptr[rcx+8]
	movss	xmm3,dword ptr[rdx+8]
	
	movlhps	xmm0,xmm2
	movlhps	xmm1,xmm3

	mulps	xmm0,xmm1
	
	movhlps xmm3,xmm0	
	addss	xmm3,xmm0				; xmm[0-31] = x+z

	movaps	xmm2,xmm0
	shufps	xmm2,xmm2,1				; xmm [0-31] = y
	
	
	addss	xmm3,xmm2

	movss	xmm0,xmm3				; return value
	
	ret

DotProduct	ENDP



;GLOBAL_FUNC_DLL void __stdcall TransformVector3_VPTR1(VECTOR3* pv3SrcDest, const MATRIX4* pMat, DWORD dwNum)
TransformVector3_VPTR1	PROC	

	; rcx = pv3SrcDest
	; rdx = pMat
	; r8 = dwNum
	
	or		r8,r8
	jz		lb_exit
	
	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7
	
	movups	xmm4,xmmword ptr[rdx]		; load matrix 1 line
	movups	xmm5,xmmword ptr[rdx+16]	; load matrix 2 line
	movups	xmm6,xmmword ptr[rdx+32]	; load matrix 3 line
	movups	xmm7,xmmword ptr[rdx+48]	; load matrix 4 line

lb_loop:

	; LOAD VECTOR
	movlps	xmm0,qword ptr[rcx]			
	movss	xmm1,dword ptr[rcx+8]
	movlhps	xmm0,xmm1
	
	movaps	xmm1,xmm0		; src vector
	shufps	xmm1,xmm1,0		; x x x x
	mulps	xmm1,xmm4		; 1 line ok


	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,85	; y y y y
	mulps	xmm2,xmm5		; 2 line ok

	addps	xmm1,xmm2		

	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,170	; z z z z 
	mulps	xmm2,xmm6		; 3 line ok
	
	addps	xmm1,xmm2
	addps	xmm1,xmm7

	; xmm = ?	|	z	|	y	|	x
	movhlps		xmm0,xmm1		; z

	movlps	qword ptr[rcx],xmm1			; write x,y
	movss	dword ptr[rcx+8],xmm0		; write z

	add		rcx,VECTOR3_SIZE
	dec		r8
	jnz		lb_loop
	
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32

lb_exit:
	ret
	
TransformVector3_VPTR1	ENDP



;GLOBAL_FUNC_DLL void __stdcall TransformVector3_VPTR2(VECTOR3* pv3Dest, const VECTOR3* pv3Src, const MATRIX4* pMat, DWORD dwNum)
TransformVector3_VPTR2	PROC
	; rcx = pv3SrcDest
	; rdx = pv3Src
	; r8 = pMat
	; r9 = dwNum
	
	or			r9,r9
	jz			lb_exit
	
	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7
	
	
	movups	xmm4,xmmword ptr[r8]	; load matrix 1 line
	movups	xmm5,xmmword ptr[r8+16]	; load matrix 2 line
	movups	xmm6,xmmword ptr[r8+32]	; load matrix 3 line
	movups	xmm7,xmmword ptr[r8+48]	; load matrix 4 line

lb_loop:
	; LOAD VECTOR
	movlps	xmm0,qword ptr[rdx]
	movss	xmm1,dword ptr[rdx+8]
	movlhps	xmm0,xmm1
	
	movaps	xmm1,xmm0		; src vector
	shufps	xmm1,xmm1,0		; x x x x
	mulps	xmm1,xmm4		; 1 line ok


	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,85	; y y y y
	mulps	xmm2,xmm5		; 2 line ok

	addps	xmm1,xmm2		

	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,170	; z z z z 
	mulps	xmm2,xmm6		; 3 line ok
	
	addps	xmm1,xmm2
	addps	xmm1,xmm7

	; xmm = ?	|	z	|	y	|	x
	movhlps		xmm0,xmm1		; z

	movlps	qword ptr[rcx],xmm1			; write x,y
	movss	dword ptr[rcx+8],xmm0		; write z

	add		rcx,VECTOR3_SIZE
	add		rdx,VECTOR3_SIZE

	dec		r9
	jnz		lb_loop
		
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32

lb_exit:
	ret

TransformVector3_VPTR2	ENDP




;GLOBAL_FUNC_DLL void __stdcall CrossProduct(const VECTOR3* r, const VECTOR3* u, const VECTOR3* v)
CrossProduct	PROC	
	; rcx = r
	; rdx = u
	; r8 = v
	
	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7

	

	
	; LOAD VECTOR u, LOAD VECTOR v
	movlps	xmm0,qword ptr[rdx]			
	movlps	xmm1,qword ptr[r8]			

	movss	xmm2,dword ptr[rdx+8]
	movss	xmm3,dword ptr[r8+8]

	movlhps	xmm0,xmm2
	movlhps	xmm1,xmm3

	

	
	xorps		xmm2,xmm2		; 0으로 초기화
	movaps		xmm6,xmm0		; backup	xmm0

	; x,y성분
	; 	r->x = u->y*v->z - u->z*v->y;
	;	r->y = u->z*v->x - u->x*v->z;

	shufps		xmm0,xmm0,41	;	u->x	|	u->z	|	u->z	|	u->y
	shufps		xmm1,xmm1,146	;	v->z	|	v->y	|	v->x	|	v->z

	
	mulps		xmm0,xmm1		;	u->x*v->z |	u->z*v->y | u->z*v->x |	u->y*v->z	
	movhlps		xmm2,xmm0		;			  |			  |	u->x*v->z |	u->z*v->y
	subps		xmm0,xmm2		;		?	  |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	; z성분
	;	r->z = u->x*v->y - u->y*v->x;
								
	shufps		xmm6,xmm6,16	;			  |	u->y	  |			  |	u->x	
	shufps		xmm1,xmm1,18	;			  |	v->x	  |			  |	v->y	
	mulps		xmm6,xmm1		;			  |	u->y*v->x |			  | u->x*v->y 
	movhlps		xmm2,xmm6		;			  |			  | 		  | u->y*v->x
	subss		xmm6,xmm2		;			  |           |           | u->x*v->y - u->y*v->x				

	
	movlps		qword ptr[rcx],xmm0		; write x,y
	movss		dword ptr[rcx+8],xmm6
	
	
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32
	
	ret
	
CrossProduct	ENDP
	
	
	
	
;GLOBAL_FUNC_DLL void __stdcall Normalize(VECTOR3* OUT vn, const VECTOR3* IN v)
Normalize	PROC

	; rcx = vn
	; rdx = v

	xorps			xmm1,xmm1
	xorps			xmm3,xmm3

	; LOAD VECTOR
	movlps			xmm0,qword ptr[rdx]
	movss			xmm2,dword ptr[rdx+8]
	
	movlhps			xmm0,xmm2

	movaps			xmm2,xmm0			; backup 

	mulps			xmm0,xmm0			;	?	|	zz	|	yy	|	xx
	movhlps			xmm1,xmm0			;	?	|	?	|	?	|	zz
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+zz
	shufps			xmm0,xmm0,1			;	?	|	?	|	?	|	yy
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+yy+zz

	sqrtss			xmm0,xmm1
	
	shufps			xmm0,xmm0,0			; distance

	; prevent divide by zero
	cmpneqps		xmm3,xmm0			; if (distance == 0) then xmm3 = 0 else xmm3 = 0xffffffff

	divps			xmm2,xmm0			; (x , y, z) / distance

	andps			xmm2,xmm3			; if (distance == 0) (x,y,z) = (0,0,0)

	movhlps			xmm0,xmm2			; z
	
	movlps			qword ptr[rcx],xmm2
	movss			dword ptr[rcx+8],xmm0
	
	ret

Normalize	ENDP


;GLOBAL_FUNC_DLL float __stdcall CalcDistance(const VECTOR3* pv3Start, const VECTOR3* pv3End)
CalcDistance	PROC

	; rcx = pv3Start
	; rdx = pv3End	
	
	; LOAD VECTOR
	movlps		xmm0,qword ptr[rcx]		; start
	movlps		xmm1,qword ptr[rdx]		; end
	
	movss		xmm2,dword ptr[rcx+8]
	movss		xmm3,dword ptr[rdx+8]
	movlhps		xmm0,xmm2
	movlhps		xmm1,xmm3

	subps			xmm0,xmm1
	mulps			xmm0,xmm0			;	?	|	zz	|	yy	|	xx
	movhlps			xmm1,xmm0			;	?	|	?	|	?	|	zz
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+zz
	shufps			xmm0,xmm0,1			;	?	|	?	|	?	|	yy
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+yy+zz
	sqrtss			xmm0,xmm1
	
	ret
	
CalcDistance	ENDP


;GLOBAL_FUNC_DLL void __stdcall MatrixMultiply2(MATRIX4* pResult, const MATRIX4* mat1, const MATRIX4* mat2)
MatrixMultiply2	PROC
	
	; rcx = pResult
	; rdx = mat1
	; r8 = mat2
	
	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7

	
	
	movups	xmm4,xmmword ptr[r8]		; matrix line 1
	movups	xmm5,xmmword ptr[r8+16]	; matrix line 2
	movups	xmm6,xmmword ptr[r8+32]	; matrix line 3
	movups	xmm7,xmmword ptr[r8+48]	; matrix line 4


	mov		eax,4
lp:
	movups	xmm0,xmmword ptr[rdx]		; load matrix (n) line

	
	movaps	xmm1,xmm0		; src vector
	shufps	xmm1,xmm1,0		; x x x x
	mulps	xmm1,xmm4		; 1 line ok


	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,85	; y y y y
	mulps	xmm2,xmm5		; 2 line ok


	addps	xmm1,xmm2		


	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,170	; z z z z 
	mulps	xmm2,xmm6		; 3 line ok
	
	addps	xmm1,xmm2

	movaps	xmm2,xmm0		; src vector
	shufps	xmm2,xmm2,255	; w w w w 
	mulps	xmm2,xmm7		; 4 line ok

	addps	xmm1,xmm2

	movups	xmmword ptr[rcx],xmm1
	
	add		rdx,16
	add		rcx,16

	dec		rax
	jnz		lp
	
	
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32
	
	ret

MatrixMultiply2	ENDP






;void __stdcall PhysiqueTransformPosAndNormalAndUV(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry)
PhysiqueTransformPosAndNormalAndUV	PROC	pVertexResult:QWORD , pPhyVertex:QWORD , dwVertexNum:QWORD , dwSize:QWORD , pMatrixEntry:QWORD


	local			v3Normal	: VECTOR4	
	local			v3Tangent	: VECTOR4
	
	
	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7
	
	mov				qword ptr[rbp+16],rcx
	mov				qword ptr[rbp+24],rdx
	mov				qword ptr[rbp+32],r8
	mov				qword ptr[rbp+40],r9
	

	mov			rcx,dwVertexNum
	or			rcx,rcx
	jz			lb_return

	mov			r10,pPhyVertex
	mov			r8,pVertexResult; pVertexResult
	

lb_loop_physique_num:
	xorps		xmm3,xmm3					; v3Result
	movups		v3Normal,xmm3				; v3Normal
	movups		v3Tangent,xmm3				; v3Tangent
	mov			rax,qword ptr[r10+BONE_ENTRY_OFFSE_IN_PHYVERTEX]		; bone entry
	movzx		rcx,byte ptr[r10+BONE_NUM_OFFSE_IN_PHYVERTEX]			; pPhyVertex->bBonesNum
	

	; 아무곳에도 어싸인되지 않은 버텍스 처리
	xor			rdx,rdx
	mov			edx,dword ptr[rax]			; pBone->dwBoneIndex
	cmp			edx,-1
	jz			lb_write
	

lb_loop_bones_num:

	xor			rdx,rdx								; rdx = pBone->dwBoneIndex(matrix index)
	mov			edx,dword ptr[rax]					; 
	
	movups		xmm0,[r10+POS_OFFSET_IN_PHYVERTEX]	; pPhyVertex->v3Offset
	shl			rdx,6								; matrix size = 64bytes
	add			rdx,pMatrixEntry					; matrix entry
	

	movups		xmm4,[rdx]		; load matrix 1 line
	movups		xmm5,[rdx+16]	; load matrix 2 line
	movups		xmm6,[rdx+32]	; load matrix 3 line
	movups		xmm7,[rdx+48]	; load matrix 4 line

	movaps		xmm1,xmm0		; src vector
	shufps		xmm1,xmm1,0		; x x x x
	mulps		xmm1,xmm4		; 1 line ok


	movaps		xmm2,xmm0		; src vector
	shufps		xmm2,xmm2,85	; y y y y
	mulps		xmm2,xmm5		; 2 line ok

	addps		xmm1,xmm2		

	movaps		xmm2,xmm0		; src vector
	shufps		xmm2,xmm2,170	; z z z z 
	mulps		xmm2,xmm6		; 3 line ok
	
	addps		xmm1,xmm2
	addps		xmm1,xmm7


	; 여기서부터 탄젠트 벡터 계산
	; LOAD VECTOR
	movlps		xmm0,qword ptr[r10+TANGENT_OFFSET_IN_PHYVERTEX]			; pPhyVertex->v3Tangent
	movss		xmm2,dword ptr[r10+TANGENT_OFFSET_IN_PHYVERTEX+8]
	movlhps		xmm0,xmm2

	movaps		xmm2,xmm0		; src normal
	shufps		xmm2,xmm2,0		; x x x x
	mulps		xmm2,xmm4		; 1 line ok

	movaps		xmm7,xmm0		; src normal
	shufps		xmm7,xmm7,85	; y y y y
	mulps		xmm7,xmm5		; 2 line ok

	addps		xmm2,xmm7

	movaps		xmm7,xmm0		; src normal
	shufps		xmm7,xmm7,170	; z z z z
	mulps		xmm7,xmm6		; 3 line ok

	addps		xmm2,xmm7

	movss		xmm0,dword ptr[rax+4]	; bone->fWeight
	shufps		xmm0,xmm0,0		; xmm0 = fWeight | fWeight | fWeight | fWeight
	
	; xmm2 = ?  |	tz	|	ty	|	tx
	movups		xmm7,v3Tangent
	mulps		xmm2,xmm0		; v3Tangent = (v3Offeset[0] * fWeight) + (v3Offeset[1] * fWeight) + (v3Offeset[2] * fWeight) ...
	addps		xmm7,xmm2

	movups		v3Tangent,xmm7



	; 여기서부터 노멀계산..
	; LOAD VECTOR
	movlps	xmm0,qword ptr[r10+NORMAL_OFFSET_IN_PHYVERTEX]			; pPhyVertex->v3Normal
	movss	xmm2,dword ptr[r10+NORMAL_OFFSET_IN_PHYVERTEX+8]
	movlhps	xmm0,xmm2

	movaps		xmm2,xmm0		; src normal
	shufps		xmm2,xmm2,0		; x x x x
	mulps		xmm2,xmm4		; 1 line ok

	movaps		xmm7,xmm0		; src normal
	shufps		xmm7,xmm7,85	; y y y y
	mulps		xmm7,xmm5		; 2 line ok

	addps		xmm2,xmm7

	movaps		xmm7,xmm0		; src normal
	shufps		xmm7,xmm7,170	; z z z z
	mulps		xmm7,xmm6		; 3 line ok

	addps		xmm2,xmm7


	; xmm1 = ?	|	z	|	y	|	x
	
	movss		xmm0,dword ptr[rax+4]	; bone->fWeight
	shufps		xmm0,xmm0,0		; xmm0 = fWeight | fWeight | fWeight | fWeight
	
	mulps		xmm1,xmm0		; v3Result = (v3Offeset[0] * fWeight) + (v3Offeset[1] * fWeight) + (v3Offeset[2] * fWeight) ...
	addps		xmm3,xmm1

	; xmm2 = ?  |	nz	|	ny	|	nx
	movups		xmm7,v3Normal
	mulps		xmm2,xmm0		; v3Normal = (v3Offeset[0] * fWeight) + (v3Offeset[1] * fWeight) + (v3Offeset[2] * fWeight) ...
	addps		xmm7,xmm2
	movups		v3Normal,xmm7



	add			rax,BONE_LITE_SIZE
	dec			rcx
	jnz			lb_loop_bones_num

lb_write:
	movhlps		xmm0,xmm3			; z
	movlps		qword ptr[r8],xmm3			; write x,y
	movss		dword ptr[r8+8],xmm0		; write z

	; 노멀 써넣기
	movups		xmm1,v3Normal		; v3Normal.xy
	movhlps		xmm2,xmm1			; v3Normal.z
	
	movlps		qword ptr[r8+12],xmm1
	movss		dword ptr[r8+12+8],xmm2
	

	; uv 써넣기
	mov			rax,qword ptr[r10+TEXCOORD_OFFSET_IN_PHYVERTEX]
	mov			qword ptr[r8+12+8+4],rax			; u,v
	

	; tangent 써넣기
	movups		xmm1,v3Tangent		; v3Tangent.xy
	movhlps		xmm2,xmm1			; v3Tangent.z
	
	movlps		qword ptr[r8+12+8+8+4],xmm1
	movss		dword ptr[r8+12+8+8+12],xmm2
	
	dec			dwVertexNum
	jz			lb_return

	
	add			r10,PHYSIQUE_VERTEX_SIZE_X64
	add			r8,dwSize
	jmp			lb_loop_physique_num

lb_return:

	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32
	
	ret
PhysiqueTransformPosAndNormalAndUV	ENDP







;GLOBAL_FUNC_DLL float __stdcall VECTOR3Length(const VECTOR3* pv3)
VECTOR3Length	PROC
	;rcx = pv3
			
	; LOAD VECTOR
	movlps		xmm0,qword ptr[rcx]		; start
	movss		xmm2,dword ptr[rcx+8]
	movlhps		xmm0,xmm2

	movaps	xmm1,xmm0
	
	mulps	xmm0,xmm1
	
	movhlps xmm3,xmm0	
	addps	xmm3,xmm0		; xmm[0-31] = x+z

	movaps	xmm2,xmm0
	shufps	xmm2,xmm2,1		; xmm [0-31] = y
	
	
	addps	xmm3,xmm2
	sqrtss	xmm3,xmm3
	
	movaps	xmm0,xmm3		; result
	ret
	
VECTOR3Length	ENDP



;GLOBAL_FUNC_DLL BOOL __stdcall CalcPlaneEquation(PLANE* pPlane, const VECTOR3* pv3Tri)
CalcPlaneEquation	PROC	
	; rcx = pPlane
	; rdx = pv3Tri

	;float	zero = 0.0f;
	;BOOL	bResult = TRUE;


	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7
	
				

	xor			rax,rax		; bResult = FALSE;
	xorps		xmm7,xmm7

	movups		xmm5,[rdx]							; 0
	movups		xmm0,[rdx+VECTOR3_SIZE]				; 1

	; LOAD VECTOR
	movlps		xmm1,qword ptr[rdx+VECTOR3_SIZE+VECTOR3_SIZE]		; 2
	movss		xmm2,dword ptr[rdx+VECTOR3_SIZE+VECTOR3_SIZE+8]
	movlhps		xmm1,xmm2

	; cross product			r = u*v
	subps		xmm0,xmm5		; u
	subps		xmm1,xmm5		; v


	xorps		xmm2,xmm2		; 0으로 초기화
	movaps		xmm6,xmm0		; backup	xmm0

	; x,y성분
	; 	r->x = u->y*v->z - u->z*v->y;
	;	r->y = u->z*v->x - u->x*v->z;

	shufps		xmm0,xmm0,41	;	u->x	|	u->z	|	u->z	|	u->y
	shufps		xmm1,xmm1,146	;	v->z	|	v->y	|	v->x	|	v->z

	
	mulps		xmm0,xmm1		;	u->x*v->z |	u->z*v->y | u->z*v->x |	u->y*v->z	
	movhlps		xmm2,xmm0		;			  |			  |	u->x*v->z |	u->z*v->y
	subps		xmm0,xmm2		;		?	  |		?	  |	u->z*v->x - u->x*v->z |	u->y*v->z - u->z*v->y

	; z성분
	;	r->z = u->x*v->y - u->y*v->x;
								
	shufps		xmm6,xmm6,16	;			  |	u->y	  |			  |	u->x	
	shufps		xmm1,xmm1,18	;			  |	v->x	  |			  |	v->y	
	mulps		xmm6,xmm1		;			  |	u->y*v->x |			  | u->x*v->y 
	movhlps		xmm2,xmm6		;			  |			  | 		  | u->y*v->x
	subss		xmm6,xmm2		;			  |           |           | u->x*v->y - u->y*v->x	
	movlhps		xmm0,xmm6		; r


	; normalize r 
	xorps			xmm1,xmm1
	movaps			xmm2,xmm0			; backup 

	mulps			xmm0,xmm0			;	?	|	zz	|	yy	|	xx
	movhlps			xmm1,xmm0			;	?	|	?	|	?	|	zz
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+zz
	shufps			xmm0,xmm0,1			;	?	|	?	|	?	|	yy
	addss			xmm1,xmm0			;	?	|	?	|	?	| xx+yy+zz

	sqrtss			xmm0,xmm1
	comiss			xmm0,xmm7			; xmm0 != 0
	jnz				lb_distance

	
	xorps			xmm0,xmm0
	movups			[rcx],xmm0
	
	jmp				lb_return
	
lb_distance:
	
	shufps			xmm0,xmm0,0			; distance
	divps			xmm2,xmm0

	; 	pPlane->v3Up = r;
	;	VECTOR3		v3Up;
	;	float		D;
	;	float		fAreaSize;	

	movups			[rcx],xmm2			; pPlane->v3Up


	;	pPlane->D = -(pv3Tri->x * r.x + pv3Tri->y * r.y + pv3Tri->z * r.z);
	mulps			xmm5,xmm2
	movhlps			xmm3,xmm5	
	addps			xmm3,xmm5		; xmm[0-31] = x+z

	shufps			xmm5,xmm5,1		; xmm [0-31] = y
	addss			xmm3,xmm5

	xorps			xmm0,xmm0
	subss			xmm0,xmm3
	mov				rax,1
lb_return:
	movss			dword ptr[rcx+PLANE_D_OFFSET],xmm0
	
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	
	add			rsp,32
	
	ret	
	
CalcPlaneEquation	ENDP

; GLOBAL_FUNC_DLL BOOL __stdcall ClipTriWithPlaneList(const PLANE* pPlaneList, DWORD dwPlaneNum, const TRIANGLE* pTri)

ClipTriWithPlaneList PROC pPlaneList:QWORD , dwPlaneNum:QWORD , pTri:QWORD


	; backup xmm6
	sub			rsp,32
	movups		[rsp+0],xmm6
	movups		[rsp+16],xmm7
	
	
	xor			rax,rax		; return value
	
	mov			qword ptr[rbp+16],rcx
	mov			qword ptr[rbp+24],rdx
	mov			qword ptr[rbp+32],r8
	

	mov				r10,pPlaneList
	mov				r11,pTri
	mov				rcx,dwPlaneNum
	
	
	movlps			xmm0,qword ptr[r11]
	movlps			xmm1,qword ptr[r11+12]
	movlps			xmm2,qword ptr[r11+12+12]

	movss			xmm7,dword ptr[r11+8]
	movss			xmm6,dword ptr[r11+12+8]
	movss			xmm5,dword ptr[r11+12+12+8]

	movlhps			xmm0,xmm7		; pTri->v3Point[0]
	movlhps			xmm1,xmm6		; pTri->v3Point[1]
	movlhps			xmm2,xmm5		; pTri->v3Point[2]

	; xmm0 = pTri->v3Point[0]
	; xmm1 = pTri->v3Point[1]
	; xmm2 = pTri->v3Point[2]


lb_loop_plane:
	movups			xmm3,[r10]		; up
	xorps			xmm7,xmm7		; zero

	movaps			xmm4,xmm3
	shufps			xmm4,xmm4,255	; D 

	; pTri->v3Point[0] * Up + D

	movaps			xmm5,xmm0		; pTri->v3Point[0]

	mulps			xmm5,xmm3		; w0*w1 | z0*z1 | y0*y1 | x0*x1  
    movhlps			xmm6,xmm5		;  ?    |   ?   | w0*w1 | z0*z1
    addss			xmm6,xmm5		;  ?    |   ?   |   ?   | x0*x1 + z0*z1
    shufps			xmm5,xmm5,1		; y0*y1 | y0*y1 | y0*y1 | y0*y1
    addss			xmm5,xmm6		;  ?    |   ?   |   ?   | x0*x1 + y0*y1 + z0*z1

	addss			xmm5,xmm4		; +D

	comiss			xmm5,xmm7
	jbe				lb_next_plane

	; pTri->v3Point[1] * Up + D

	movaps			xmm5,xmm1		; pTri->v3Point[0]

	mulps			xmm5,xmm3		; w0*w1 | z0*z1 | y0*y1 | x0*x1  
    movhlps			xmm6,xmm5		;  ?    |   ?   | w0*w1 | z0*z1
    addss			xmm6,xmm5		;  ?    |   ?   |   ?   | x0*x1 + z0*z1
    shufps			xmm5,xmm5,1		; y0*y1 | y0*y1 | y0*y1 | y0*y1
    addss			xmm5,xmm6		;  ?    |   ?   |   ?   | x0*x1 + y0*y1 + z0*z1

	addss			xmm5,xmm4		; +D

	comiss			xmm5,xmm7
	jbe				lb_next_plane


	; pTri->v3Point[2] * Up + D

	movaps			xmm5,xmm2		; pTri->v3Point[0]

	mulps			xmm5,xmm3		; w0*w1 | z0*z1 | y0*y1 | x0*x1  
    movhlps			xmm6,xmm5		;  ?    |   ?   | w0*w1 | z0*z1
    addss			xmm6,xmm5		;  ?    |   ?   |   ?   | x0*x1 + z0*z1
    shufps			xmm5,xmm5,1		; y0*y1 | y0*y1 | y0*y1 | y0*y1
    addss			xmm5,xmm6		;  ?    |   ?   |   ?   | x0*x1 + y0*y1 + z0*z1

	addss			xmm5,xmm4		; +D

	comiss			xmm5,xmm7
	jbe				lb_next_plane
	
	jmp				lb_return		; false

lb_next_plane:
	add				r10,16			; pPlaneList += sizeof(PLANE)
	dec				rcx
	jnz				lb_loop_plane
	
	inc				rax

lb_return:
	
	; restore xmm6
	movups		xmm6,[rsp+0]
	movups		xmm7,[rsp+16]
	add			rsp,32
	
	ret

;	return bResult;

ClipTriWithPlaneList ENDP

end