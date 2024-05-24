.code

GetEIP	PROC

	mov		rdx,qword ptr[rsp]		; 리턴 어드레스
	mov		qword ptr[rcx],rdx		; write
	ret
	
GetEIP	ENDP

end
