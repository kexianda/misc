; Intel format ASM
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda
; to be tested

section .data
align 16
test_data
DQ 0x0405060700010203
DQ 0x0c0d0e0f08090a0b

test_data2
DQ 0x0c0d0e0f08090a0b
DQ 0x0405060700010203

align 16
reduct_const
DQ 0x0000000000000000
DQ 0xc200000000000000

; ==============================================
section .text

; test driver
global test_mult
test_mult:
	; Linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0~7
	vmovdqa	xmm0,	[rdi] 
	vmovdqa xmm1,	[rsi]
	call _ghash
	vmovdqa	xmm0,	[rdi] 
	vmovdqa xmm1,	[rsi]
	call _ghash_2
	;call 	_cl_mult_textbook
	vmovdqa [rdx], 		xmm3
	vmovdqa [rdx+0x10],	xmm6

	ret	
	; exit
	; mov	rbx,	0	; arg1
	; mov	rax,	1	; sys_exit
	; int 0x80		; call kernel 
global test_mult_karatsuba
test_mult_karatsuba:
	vmovdqa xmm0, [rdi]
	vmovdqa xmm1, [rsi]
	call _cl_mult_karatsuba
	vmovdqa [rdx], 		xmm3
	vmovdqa [rdx+0x10], xmm6	
	ret
; ==========================================================

align 16
global _cl_mult_textbook  ; text book algorithm carry-less multiply
_cl_mult_textbook:
	vmovdqa		xmm3,	xmm0
	vpclmulqdq	xmm3,	xmm1, 0  ; xmm3 holds a0*b0
	vmovdqa		xmm4,	xmm0
	vpclmulqdq	xmm4,	xmm1, 16 ; xmm4 holds a0*b1
	vmovdqa		xmm5,	xmm0
	vpclmulqdq	xmm5,	xmm1, 1  ; xmm5 holds a1*b0
	vmovdqa		xmm6,	xmm0
	vpclmulqdq	xmm6,	xmm1, 17 ; xmm6 holds a1*b1
	vpxor		xmm4,	xmm5	 ; xmm4 holds a0*b1 + a1*b0
	vmovdqa		xmm5,	xmm4	
	vpsrldq		xmm4,	8
	vpslldq		xmm5,	8
	vpxor		xmm3,	xmm5
	vpxor		xmm6,	xmm4	; <xmm6:xmm3> holds the result of cl multiplication of xmm0 by xmm1
	ret

align 16
global _cl_mult_karatsuba
_cl_mult_karatsuba:
	vmovdqa		xmm3,	xmm0
	vpclmulqdq	xmm3,	xmm1,	0x00
	vmovdqa		xmm6,	xmm0	
	vpclmulqdq	xmm6,	xmm1,	0x11
	vpshufd		xmm4,	xmm0,	78
	vpshufd		xmm5,	xmm1,	78
	vpxor		xmm4,	xmm0
	vpxor		xmm5,	xmm1
	vpclmulqdq	xmm4,	xmm1,	0x00
	vpxor		xmm4,	xmm3
	vpxor		xmm4,	xmm6
	vmovdqa		xmm5,	xmm4	
	vpsrldq		xmm4,	8
	vpslldq		xmm5,	8
	vpxor		xmm3,	xmm5
	vpxor		xmm6,	xmm4	; <xmm6:xmm3> holds the result of cl multiplication of xmm0 by xmm1	
	ret

; ====================================================================	
align 16
global _ghash
_ghash:
	; multiply data with the hash key
	; xmm0 * xmm1  =>  <xmm6:xmm3>
	vmovdqa		xmm3,	xmm0
	vpclmulqdq	xmm3,	xmm1, 0  ; xmm3 holds a0*b0
	vmovdqa		xmm4,	xmm0
	vpclmulqdq	xmm4,	xmm1, 16 ; xmm4 holds a0*b1
	vmovdqa		xmm5,	xmm0
	vpclmulqdq	xmm5,	xmm1, 1  ; xmm5 holds a1*b0
	vmovdqa		xmm6,	xmm0
	vpclmulqdq	xmm6,	xmm1, 17 ; xmm6 holds a1*b1
	vpxor		xmm4,	xmm5	 ; xmm4 holds a0*b1 + a1*b0
	vmovdqa		xmm5,	xmm4	
	vpsrldq		xmm4,	8
	vpslldq		xmm5,	8
	vpxor		xmm3,	xmm5
	vpxor		xmm6,	xmm4	; <xmm6:xmm3> holds the result of cl multiplication of xmm0 by xmm1
	
	; shift the result of the multiplication by one bit to the left to
	; to cope with the fact that the bits are reversed	
	vmovdqa	xmm7,	xmm3
	vmovdqa	xmm8,	xmm6
	vpslld	xmm3,	1
	vpslld	xmm6,	1
	vpsrld	xmm7,	31
	vpsrld	xmm8,	31
	vmovdqa	xmm9,	xmm7
	vpslldq	xmm8,	4
	vpslldq	xmm7,	4
	vpsrldq	xmm9,	12
	vpor	xmm3,	xmm7
	vpor	xmm6,	xmm8
	vpor	xmm6,	xmm9	

	; first phase of the reduction
	vmovdqa	xmm7,	xmm3
	vmovdqa	xmm8,	xmm3
	vmovdqa	xmm9,	xmm3
	vpslld	xmm7,	31	;packed right shifting <<31
	vpslld	xmm8,	30
	vpslld	xmm9,	25
	vpxor	xmm7,	xmm8
	vpxor	xmm7,	xmm9
	
	vmovdqa xmm8,	xmm7
	vpslldq	xmm7,	12
	vpslldq	xmm8,	4
	vpxor	xmm3,	xmm7	; first phase of the reduction complete

	; second phase of the reduction
	vmovdqa	xmm2,	xmm3
	vmovdqa	xmm4,	xmm3
	vmovdqa	xmm5,	xmm3
	vpsrld	xmm2,	1
	vpsrld	xmm4,	2
	vpsrld	xmm5,	7
	vpxor	xmm2,	xmm4
	vpxor	xmm2,	xmm5
	vpxor	xmm2,	xmm8
	vpxor	xmm3,	xmm2
	vpxor	xmm6,	xmm3	; xmm6 is the result

	ret

; ====================================================================================
align 16
global _ghash_2
_ghash_2:
	; multiply data with the hash key
	; xmm0 * xmm1  =>  <xmm6:xmm3>
	vmovdqa		xmm3,	xmm0
	vpclmulqdq	xmm3,	xmm1, 0  ; xmm3 holds a0*b0
	vmovdqa		xmm4,	xmm0
	vpclmulqdq	xmm4,	xmm1, 16 ; xmm4 holds a0*b1
	vmovdqa		xmm5,	xmm0
	vpclmulqdq	xmm5,	xmm1, 1  ; xmm5 holds a1*b0
	vmovdqa		xmm6,	xmm0
	vpclmulqdq	xmm6,	xmm1, 17 ; xmm6 holds a1*b1
	vpxor		xmm4,	xmm5	 ; xmm4 holds a0*b1 + a1*b0
	vmovdqa		xmm5,	xmm4	
	vpsrldq		xmm4,	8
	vpslldq		xmm5,	8
	vpxor		xmm3,	xmm5
	vpxor		xmm6,	xmm4	; <xmm6:xmm3> holds the result of cl multiplication of xmm0 by xmm1

	; Carry-less Montgomery for the GHASH operations [Gueron 2012]
	vmovdqa		xmm1,	[reduct_const wrt rip] ; xmm1 T3
	vpclmulqdq	xmm2,	xmm1,	xmm3,	0x01
	vpshufd		xmm4,	xmm3,	78
	vpxor		xmm4,	xmm4,	xmm2
	vpclmulqdq	xmm2,	xmm1,	xmm4,	0x01
	vpshufd		xmm4,	xmm4,	78
	vpxor		xmm4,	xmm4,	xmm2
	vpxor		xmm6,	xmm4		; xmm6 holds the result 

	ret
