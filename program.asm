	section .data
	j dd 22
	h dd 12
	section .bss
	a resd 33
	b resb 24
	section .text
	global main
main:	mov eax,j
	add eax,h
	mov eax,12
