	section .data
	message db "value is %d",10,0
	h dd 12
	section .bss
	a resd 33
	b resb 24
	section .text
	global main
main:	mov eax,34
	mov eax,ecx
	mov eax,eax
	mov eax,message
	mov ecx,43
	mov edx,43
	mov ebx,43
	mov ecx,eax
	mov ecx,ecx
	mov ecx,message
	mov eax,h
	mov ecx,h
	mov eax,a
	mov ecx,b
