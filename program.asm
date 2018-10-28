	section .data
	message db "value is %d",10,0
	h dd 12
	s dd 12
	d dd 12
	f dd 12
	hh dd 12
	hg dd 12
	sah dd 12
	ah dd 12
	sdh dd 12
	dh dd 12
	xxh dd 12
	wsh dd 12
	qqqh dd 12
	wh dd 12
	aah dd 12
	ssssh dd 12
	ha dd 12
	hs dd 12
	zzha dd 12
	hx dd 12
	xh dd 12
	hv dd 12
	zh dd 12
	section .bss
	a resd 33
	b resb 24
	section .text
	global main
main:	mov eax,34
	mov eax,ecx
	mov eax,message
	mov ecx,43
	mov ecx,eax
	mov ecx,message
