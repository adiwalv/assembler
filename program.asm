	section .bss
	z resb 10
	f resd 2
section .data
	a db "hello",10,10,10,10,10,10,0	
	c db "jj",10,10,0		
	b dd 10,20
	t dd 10,12,34,5
	section .text
	global main,printf,scanf,zzz
main:	mov eax,t
pqr:	add edx,ebx
qwr:	mov t,eax
	add edi,esi
	mov a,ecx
	jmp z
ppp:	jmp pqr
	jmp asd
	jmp qwr	
