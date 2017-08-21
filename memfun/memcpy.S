; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda
; Email:	xianda.ke@intel.com

section .data

; ==============================================
section .text

align 16
global _i_memcpy_256_unaligned ;
_i_memcpy_256_unaligned:
	; Linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0~7
	mov	rax,	rdi	; save return address
	cmp	rdx,	0x100 ; 256
	jae 	L_ge_256	 ; if len >= 256 bytes

	; TODO
	ret

align 16
L_ge_256:
	; first partial block( <32bytes)
	xor			r8,		r8
	mov                     r8d,            edi
	neg                     r8d
	and                     r8d,            1FH
	jz			L_4ymm_loop
	vmovdqu                 ymm0,		[rsi]	 ; save the first partial block
	add			rsi,		r8
	sub			rdx,		r8
	vmovdqu                 [rdi],          ymm0
	add			rdi,		r8

	sub			rdx,		0x80  ; 32*4 = 128 bytes
L_4ymm_loop:
	vmovdqu                 ymm1,		[rsi]
	vmovdqu                 ymm2,		[rsi+0x20]
	vmovdqu                 ymm3,		[rsi+0x40]
	vmovdqu                 ymm4,		[rsi+0x60]
	add			rsi,		0x80
	vmovdqa                 [rdi],		ymm1
	vmovdqa                 [rdi+0x20],	ymm2
	vmovdqa                 [rdi+0x40],	ymm3
	vmovdqa                 [rdi+0x60],	ymm4
	add			rdi,		0x80
	sub			rdx,		0x80
	jae			L_4ymm_loop
	add			rdx,		0x80

L_single_ymm_loop:
	sub			rdx,		0x20
	jb			L_remaining_bytes
	vmovdqu                 ymm0,		[rsi]
	add			rsi,		0x20
	vmovdqa                 [rdi],		ymm0
	add			rdi,		0x20
	jmp			L_single_ymm_loop

L_remaining_bytes:  ; <= 32 bytes
	add			rdx,    	0x20
	add			rsi,		rdx
	add			rdi,		rdx
	vmovdqu                 ymm0,		[rsi-0x20]
        sub                     rdi,            0x20;
        vmovdqu                 [rdi],     ymm0
	vzeroupper         ; end of AVX mode

FINISH:
	ret
