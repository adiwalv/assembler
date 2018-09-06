	section .bss
	z resb 10
	f resd 2
	f resb 3
	z resd 3
	f resd 2
section .data
	a db "hello",10,10,10,10,10,10,0	
	c db "jj",10,10,0		
	b dd 10,20
	b db "ad",10,0
	t dd 10,12,34,5
	a db "hel",10,0
	m dd 2
	section .text
	global main,printf,scanf,zzz
main:	mov eax,t
pqr:	mov ecx,xzy
	add edx,ebx
qwr:	mov to,o	
	add edi,esi
	mov a,ecx
	mov m,t
	mov ecx,tmp
	jmp z
ppp:	jmp pqr
	jmp asd
	jmp qwr	
