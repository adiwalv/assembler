section .data
	a db "Hello vikas",10,0
	section .bss
	t resb 2		
	section .text
	global main
main:	mov eax,4
	mov ebx,1
	mov ecx,a
	mov edx,2
	int 0x80
	mov eax,1
	mov ebx,1
	int 0x80
