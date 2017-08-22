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
    mov         rax,            rdi	; save return address
    cmp         rdx,            0x100 ; 256
    jae         L_ge_256	 ; if len >= 256 bytes
    cmp         dl,            0x80 ; 128
    jb          L_less_128bytes
    ;TODO  [128, 256)

align 16
L_less_128bytes:                       ; [64,128) bytes
    cmp         dl,             0x40 
    jb          L_less_64bytes
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi + 0x10]
    vmovdqu     xmm2,		[rsi + 0x20]
    vmovdqu     xmm3,		[rsi + 0x30]
    vmovdqu     xmm4,		[rsi + 0x40]
    vmovdqu     xmm5,		[rsi + 0x50]
    vmovdqu     xmm6,		[rsi + 0x60]
    vmovdqu     xmm7,		[rsi + rdx - 0x10]  ; 
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+ 0x10],    xmm1
    vmovdqu     [rdi+ 0x20],    xmm2
    vmovdqu     [rdi+ 0x30],    xmm3
    vmovdqu     [rdi+ 0x40],    xmm4
    vmovdqu     [rdi+ 0x50],    xmm5
    vmovdqu     [rdi+ 0x60],    xmm6
    vmovdqu     [rdi+rdx-0x10], xmm7
    ret         ; TODO: compare ymm & zeroupper...

align 16
L_less_64bytes:
    cmp         dl,             0x20    ;
    jb          L_less_32bytes
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi + 0x10]
    vmovdqu     xmm2,		[rsi + 0x20]
    vmovdqu     xmm3,		[rsi + rdx - 0x10]  ; 
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+ 0x10],    xmm1
    vmovdqu     [rdi+ 0x20],    xmm2
    vmovdqu     [rdi+rdx-0x10], xmm3
    ret

align 16
L_less_32bytes:
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi + rdx - 0x10]  ;
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+rdx-0x10], xmm1
    ret

align 16
L_less_16bytes:
    cmp         dl,             0x08
    jb          L_less_8bytes
    movq        r8,             [rsi]
    movq        r9,             [rsi+rdx-0x08]
    movq        [rdi],          r8
    movq        [rdi+rdx-0x08], r9
    ret

align 16
L_less_8bytes:   ; [5,8)
    cmp         dl,             0x04
    jb          L_less_4bytes
    mov         r8d,            [rsi]
    mov         r9d,            [rsi+rdx-0x04]
    mov         [rdi],          r8d
    mov         [rdi+rdx-0x04], r9d
    ret

L_le_4bytes:  ; [3,4]
    cmp         dl,             0x02
    jbe         L_le_2bytes
    mov         r8d,            [rsi]
    mov         r9d,            [rsi+rdx-0x02]
    mov         [rdi],          r8d
    mov         [rdi+rdx-0x02], r9d
    ret

L_le_2bytes:

    jb          L_less_2bytes
    mov         r8d,            [rsi]
    mov         r9d,            [rsi+rdx-0x02]
    mov         [rdi],          r8d
    mov         [rdi+rdx-0x02], r9d
    ret                                     ; TODO:  write conflict? cache pollution?

align 16
L_ge_256:
    ; first partial block( <32bytes)
    xor         r8,		r8
    mov         r8d,            edi
    neg         r8d
    and         r8d,            1FH
    jz          L_4ymm_loop
    vmovdqu     ymm0,		[rsi]	 ; save the first partial block
    add         rsi,		r8
    sub         rdx,		r8
    vmovdqu     [rdi],          ymm0
    add         rdi,		r8

    sub		rdx,		0x80  ; 32*4 = 128 bytes
L_4ymm_loop:
    vmovdqu     ymm1,		[rsi]
    vmovdqu     ymm2,		[rsi+0x20]
    vmovdqu     ymm3,		[rsi+0x40]
    vmovdqu     ymm4,		[rsi+0x60]
    add 	rsi,		0x80
    vmovdqa     [rdi],		ymm1
    vmovdqa     [rdi+0x20],	ymm2
    vmovdqa     [rdi+0x40],	ymm3
    vmovdqa     [rdi+0x60],	ymm4
    add         rdi,		0x80
    sub         rdx,		0x80
    jae         L_4ymm_loop
    add         rdx,		0x80

L_single_ymm_loop:
    sub         rdx,		0x20
    jb          L_remaining_bytes
    vmovdqu     ymm0,		[rsi]
    add         rsi,		0x20
    vmovdqa     [rdi],		ymm0
    add 	rdi,		0x20
    jmp		L_single_ymm_loop

L_remaining_bytes:  ; <= 32 bytes
    add         rdx,    	0x20
    add         rsi,		rdx
    add         rdi,		rdx
    vmovdqu     ymm0,		[rsi-0x20]
    sub         rdi,            0x20;
    vmovdqu     [rdi],          ymm0
    vzeroupper         ; end of AVX mode

FINISH:
    ret
