section .data
a db "Hello i am","gg",10,10,0,0
d dd 10,20
t dd 21
section .bss
b resb 10
section .text
global main
main: mov eax,10
mov ebx,eax
mov eax,pqr
;mov 10,eax
mov eax, a
mov ebx, t
