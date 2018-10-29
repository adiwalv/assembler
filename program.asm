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
	mov esp,43
	mov ebx,43
	mov esi,43
	mov edi,43
	mov ecx,eax
	mov ecx,ecx
	mov ecx,message
	mov eax,h
	mov ecx,h
	mov eax,a
	mov ecx,b
	add eax,10999
	add ecx,10
	add eax,eax
	add eax,ecx
	add ecx,edx
	add eax,message
	add ecx,message
	sub edx,1009
	sub eax,10999
	sub ecx,10
	sub eax,eax
	sub eax,ecx
	sub ecx,edx
	sub eax,message
	sub ecx,message
	sub edx,1009
	mul ecx
	mul edx
	mul ebx
	mul eax
	mul esp
	mul ebp
	mul esi
	mul edi
