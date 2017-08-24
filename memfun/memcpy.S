; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda

section .data

; ==============================================
section .text

align 16
global _i_memcpy_256_unaligned ;
_i_memcpy_256_unaligned:
        ; Linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0~7
    mov         rax,            rdi	; save return address
    cmp         rdx,            0x100 ; >=256
    jae         L_ge_256	 

    cmp         dl,             0x10 ; <=16
    jbe         L_le_16bytes

    cmp         dl,             0x80 ; >=128
    jbe         L_le_128bytes

L_lt_256bytes:    ;(128, 256) bytes
    vmovdqu     ymm0,		[rsi]  ; 32 bytes per time
    vmovdqu     ymm1,		[rsi + 0x20]
    vmovdqu     ymm2,		[rsi + 0x40]
    vmovdqu     ymm3,		[rsi + 0x60]
    vmovdqu     [rdi],          ymm0
    vmovdqu     [rdi+ 0x20],    ymm1
    vmovdqu     [rdi+ 0x40],    ymm2
    vmovdqu     [rdi+ 0x60],    ymm3
    vmovdqu     ymm0,		[rsi + rdx - 0x80]
    vmovdqu     ymm1,		[rsi + rdx - 0x60] 
    vmovdqu     ymm2,		[rsi + rdx - 0x40]
    vmovdqu     ymm3,		[rsi + rdx - 0x20]
    vmovdqu     [rdi+rdx-0x80], ymm0
    vmovdqu     [rdi+rdx-0x60], ymm1
    vmovdqu     [rdi+rdx-0x40], ymm2
    vmovdqu     [rdi+rdx-0x20], ymm3
    ret

align 16
L_le_128bytes:                       ; (64,128] bytes
    cmp         dl,             0x40 
    jbe          L_le_64bytes
    vmovdqu     ymm0,		[rsi]  ; 32 bytes per time
    vmovdqu     ymm1,		[rsi + 0x20]
    vmovdqu     ymm2,		[rsi + rdx - 0x40]
    vmovdqu     ymm3,		[rsi + rdx - 0x20]
    vmovdqu     [rdi],          ymm0
    vmovdqu     [rdi+ 0x20],    ymm1
    vmovdqu     [rdi+rdx-0x40], ymm2
    vmovdqu     [rdi+rdx-0x20], ymm3
    ret         ; TODO: compare ymm & zeroupper...

align 16
L_le_64bytes:                           ; (32,64]
    cmp         dl,             0x20    ;
    jbe          L_le_32bytes
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi+0x10]
    vmovdqu     xmm2,		[rsi+rdx-0x20]
    vmovdqu     xmm3,		[rsi+rdx-0x10]  ; 
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+ 0x10],    xmm1
    vmovdqu     [rdi+rdx-0x20], xmm2
    vmovdqu     [rdi+rdx-0x10], xmm3
    ret

align 16
L_le_32bytes:                          ; (16,32]
    cmp         dl,             0x10   ;
    jbe         L_le_16bytes
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi + rdx - 0x10]  ;
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+rdx-0x10], xmm1
    ret

align 16
L_le_16bytes:                           ; (8,16]
    cmp         dl,             0x08
    jbe          L_le_8bytes
    mov         r8,             [rsi]
    mov         r9,             [rsi+rdx-0x08]
    mov         [rdi],          r8
    mov         [rdi+rdx-0x08], r9
    ret

align 16
L_le_8bytes:                            ; (4,8]
    cmp         dl,             0x04
    jbe         L_le_4bytes
    mov         r8d,            [rsi]
    mov         r9d,            [rsi+rdx-0x04]
    mov         [rdi],          r8d
    mov         [rdi+rdx-0x04], r9d
    ret

align 16
L_le_4bytes:                            ; [2,4]
    cmp         dl,             0x01
    jbe         L_le_1byte
    mov         r8w,            [rsi]
    mov         r9w,            [rsi+rdx-0x02]
    mov         [rdi],          r8w
    mov         [rdi+rdx-0x02], r9w
    ret

L_le_1byte:                             ; [0,1]           
    jb          L_finish_small
    mov         r8b,            [rsi]
    mov         [rdi],          r8b                                    
L_finish_small:
    ret         
; TODO:  to be refined for tiny memcpy

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
