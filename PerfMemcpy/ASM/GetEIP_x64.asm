.code

GetEIP	PROC

	mov		rdx,qword ptr[rsp]		; ���� ��巹��
	mov		qword ptr[rcx],rdx		; write
	ret
	
GetEIP	ENDP

end
