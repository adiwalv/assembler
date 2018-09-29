section .data
a db "Hello i am",10,0
d dd 10,20
t dd 21
section .rss
a resb 10
section .text
global main
main: mov eax,10
mov ebx,eax
mov 10,eax
mov a,t
add a
ma: mov a,tx
mov eax, a
mov ebx, t
