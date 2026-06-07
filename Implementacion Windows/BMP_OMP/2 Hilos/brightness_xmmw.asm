;Rutina para modificar el brillo de una imagen BMP de 24 bits
;JJAP 23/Mayo/2024

;Prototipo en VC2022: extern "C" void brightness_xmmw(unsigned char u[16], BYTE * gimg, int size);
;Cuidado no se hace validación alguna sobre size
;Compilación: "C:\Program Files\NASM\nasm.exe" -f win64 -o brightness_xmmw.obj brightness_xmmw.asm
;Para vincular el archivo brightness_xmmw.obj al proyecto de VC2022 arrastrar el archivo a Archivos de origen en el Explorador de soluciones

segment .data

segment .bss

segment .text
	global brightness_xmmw

brightness_xmmw:

	push	rbp
	mov		rbp, rsp
	
	push	rax
	push	rbx
	push	rsi
		
	movdqu	xmm0, [rcx]
	mov		rsi, rdx
	mov		rax, r8

	mov		rcx, 16
	cdq
	div		rcx
	mov		rcx, rax
	xor		rax, rax
	xor		rbx, rbx
	xor		rdx, rdx
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
	
	pop		rsi
	pop		rbx
	pop		rax
	
	mov rsp, rbp
	pop rbp
	ret	
	
