	section .data
	message db "value is %d",10,0
	h dd 12
	section .bss
	a resd 33
	b resb 24
	section .text
	global main
main:	mov eax,34
	mov ecx,23
	add eax,ecx
	add eax,4412412
