; RCX, RDX, R8, R9
; varg_stub_x64(FILE* fp,char* szFormat,void** ppVArgList,QWORD qwVArgNum)
; volatile		RAX, RCX, RDX, R8, R9, R10, R11 
; non volatile	RBX, RBP, RDI, RSI, R12, R13, R14, and R15 


;The registers RAX, RCX, RDX, R8, R9, R10, R11, XMM0-5, 
;and the upper portions of YMM0-15 and ZMM0-15 are considered volatile and must be considered destroyed on function calls 
;(unless otherwise safety-provable by analysis such as whole program optimization). On AVX512VL, the ZMM, YMM, and XMM registers 16-31 are volatile.

;The registers RBX, RBP, RDI, RSI, RSP, R12, R13, R14, R15, and XMM6-15 are considered nonvolatile and must be saved and restored by a function that uses them.


.code

;call_stub_x64 PROC pFunc:QWORD , pRetAddr:QWORD ,pArgList:QWORD , dwArgNum:QWORD
	; rcx = pFunc
	; rdx = pRetAddr
	; r8 = pArgList
	; r9 = dwArgNum
	
	
	
	;local		t:QWORD
	
	
	
	; rbp�������͸� push�����Ƿ� 8����Ʈ �ɼ��� ���� ��ġ���� arg home�����̴�
	;mov			qword ptr[rbp+16],rcx
	;mov			qword ptr[rbp+24],rdx
	;mov			qword ptr[rbp+32],r8
	;mov			qword ptr[rbp+40],r9
	



; void MemCpy_8Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_8Bytes_Debug	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize

	local t:QWORD

	; rbp�������͸� push�����Ƿ� 8����Ʈ �ɼ��� ���� ��ġ���� arg home�����̴�
	mov			qword ptr[rbp+16],rcx
	mov			qword ptr[rbp+24],rdx
	mov			qword ptr[rbp+32],r8
	
	;mov			qword ptr[rsp+8],rcx
	;mov			qword ptr[rsp+16],rdx
	;mov			qword ptr[rsp+24],r8

	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,3

	rep movsq

	pop rdi
	pop rsi
	
	ret
	
MemCpy_8Bytes_Debug	ENDP

; void MemCpy_1Byte(char* pDest, const char* pSrc, size_t Size)
MemCpy_1Byte	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size

	rep movsb

	pop rdi
	pop rsi
	
	ret
	
MemCpy_1Byte	ENDP

; void MemCpy_4Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_4Bytes	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,2		; size /= 4

	rep movsd

	pop rdi
	pop rsi
	
	ret
	
MemCpy_4Bytes	ENDP

; void MemCpy_8Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_8Bytes	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,3		; size /= 8

	rep movsq

	pop rdi
	pop rsi
	
	ret
	
MemCpy_8Bytes	ENDP

; void MemCpy_16Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_16Bytes	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,4
lb_loop:

	MOVNTDQA xmm0,xmmword ptr[rsi]
	MOVNTDQ xmmword ptr[rdi],xmm0
	add	rdi,16
	add rsi,16
	loop lb_loop;

	pop rdi
	pop rsi
	
	ret
	
MemCpy_16Bytes	ENDP

; void MemCpy_16Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_16Bytes_Debug	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	local t:QWORD

	; rbp�������͸� push�����Ƿ� 8����Ʈ �ɼ��� ���� ��ġ���� arg home�����̴�
	mov			qword ptr[rbp+16],rcx
	mov			qword ptr[rbp+24],rdx
	mov			qword ptr[rbp+32],r8

	push rsi
	push rdi

	
	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,4
lb_loop:

	MOVNTDQA xmm0,xmmword ptr[rsi]
	MOVNTDQ xmmword ptr[rdi],xmm0
	add	rdi,16
	add rsi,16
	loop lb_loop;

	pop rdi
	pop rsi
	
	ret
	
MemCpy_16Bytes_Debug	ENDP


; void MemCpy_32Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_32Bytes	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	push rsi
	push rdi

	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,5
lb_loop:

	VMOVNTDQA ymm0,ymmword ptr[rsi]
	VMOVNTDQ ymmword ptr[rdi],ymm0
	add	rdi,32
	add rsi,32
	loop lb_loop;

	pop rdi
	pop rsi
	
	ret
	
MemCpy_32Bytes	ENDP

; void MemCpy_32Bytes(char* pDest, const char* pSrc, size_t Size)
MemCpy_32Bytes_Debug	PROC pDest:QWORD ,pSrc:QWORD , MemSize:QWORD

	; rcx = pDest
	; rdx = pSrc
	; r8 = MemSize
	
	local t:QWORD

	; rbp�������͸� push�����Ƿ� 8����Ʈ �ɼ��� ���� ��ġ���� arg home�����̴�
	mov			qword ptr[rbp+16],rcx
	mov			qword ptr[rbp+24],rdx
	mov			qword ptr[rbp+32],r8

	push rsi
	push rdi

	
	mov rdi,rcx		; dest ptr
	mov rsi,rdx		; src ptr
	mov rcx,r8		; Size
	shr rcx,5
lb_loop:

	VMOVNTDQA ymm0,ymmword ptr[rsi]
	VMOVNTDQ ymmword ptr[rdi],ymm0
	add	rdi,32
	add rsi,32
	loop lb_loop;

lb_exit:

	pop rdi
	pop rsi
	
	ret
	
MemCpy_32Bytes_Debug	ENDP
end