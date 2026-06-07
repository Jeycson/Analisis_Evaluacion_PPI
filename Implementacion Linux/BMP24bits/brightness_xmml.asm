;Rutina para modificar el brillo de una imagen BMP de 24 bits
;JJAP 23/Mayo/2024

;Prototipo en C: extern "C" void brightness_xmml(unsigned char [16], BYTE *, DWORD );
;Cuidado no se hace validación alguna sobre size
;Compilación: nasm -f elf64 -o brightness_xmml.o brightness_xmml.asm 

segment .data

segment .bss

segment .text
	global brightness_xmml

brightness_xmml:

	push	rbp
	mov		rbp, rsp
	
	push	rax
	push	rbx
	push	rcx
	
	mov		rax, rdx

	mov		rcx, 16
	cdq
	div		rcx
	mov		rcx, rax
	xor		rax, rax
	xor		rbx, rbx
	movdqu	xmm0, [rdi]
bri_loop:
	movdqu	xmm1, [rsi+rax]
	paddusb	xmm1, xmm0
	movdqu	[rsi+rax], xmm1
	inc		rbx
	mov		rax, rbx
	mov		rdx, 16
	mul		rdx
	loop	bri_loop
	
	;emms	
	
	pop		rcx
	pop		rbx
	pop		rax
	
	mov rsp, rbp
	pop rbp
	ret	
	
