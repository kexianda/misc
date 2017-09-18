; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda

section .data

; ==============================================
section .text

global _i_memcpy_256_unaligned ;
align 16
_i_memcpy_256_unaligned:
        ; Linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0~7
    mov         rax,            rdi	; save return address
    cmp         rdx,            0x100 ; >=256
    jae         L_ge_256 

    cmp         dl,             0x10 ; <=16
    ;jbe         L_le_16bytes
    jbe         L_cpySmall

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
    vzeroupper
    ret         ; TODO: compare ymm & zeroupper...

align 16
L_le_64bytes:                           ; (32,64]
    cmp         dl,             0x20    ;
    jbe          L_le_32bytes
    vmovdqu     ymm0,		[rsi]  ; 16 bytes per time
    vmovdqu     ymm1,		[rsi+rdx-0x20]
    vmovdqu     [rdi],          ymm0
    vmovdqu     [rdi+rdx-0x20], ymm1
    vzeroupper
    ret  ; 

align 16
L_le_32bytes:                          ; (16,32]
    cmp         dl,             0x10   ;
    jbe         L_cpySmall
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[rsi + rdx - 0x10]  ;
    vmovdqu     [rdi],          xmm0
    vmovdqu     [rdi+rdx-0x10], xmm1
    ret


; ------------------------
; jump table

align 16
L_cpySmallTab:
    dq  L_cpySmallTab
    dq  L_1B
    dq  L_2B
    dq  L_3B
    dq  L_4B
    dq  L_5B
    dq  L_6B
    dq  L_7B
    dq  L_8B
    dq  L_9B
    dq  L_10B
    dq  L_11B
    dq  L_12B
    dq  L_13B
    dq  L_14B
    dq  L_15B
    dq  L_16B

align   16
L_cpySmall:
    mov r10,   [L_cpySmallTab + rdx*8]      ;L_cpySmallTab[rdx*8]
    jmp r10

L_1B:
    mov         r10b,           [rsi]   ; load 1 byte
    mov         [rdi],          r10b    ; store 1 byte
    ret

L_2B:
    mov         r10w,           [rsi]   ; load 2 bytes
    mov         [rdi],          r10w    ; store 2 bytes
    ret

align   16
L_3B:
    mov        r10w,           [rsi]    ; load 2 bytes
    mov        r11b,           [rsi+2]  ; load 1 byte
    mov        [rdi],          r10w     ; store 2 bytes
    mov        [rdi+2],        r11b     ; store 1 byte
    ret

L_4B:
    mov         r10d,           [rsi]   ; load 4 bytes
    mov         [rdi],          r10d    ; store 4 bytes
    ret
align   16
L_5B:
    mov         r10d,           [rsi]   ; load 4 bytes
    mov         r11b,           [rsi+4] ; load 1 byte
    mov         [rdi],          r10d    ; store 4 bytes
    mov         [rdi+4],        r11b    ; store 1 byte
    ret

L_6B:
    mov         r10d,           [rsi]   ; load 4 bytes
    mov         r11w,           [rsi+4] ; load 2 bytes
    mov         [rdi],          r10d    ; store 4 bytes
    mov         [rdi+4],        r11w    ; store 2 bytes
    ret

align   16
L_7B:
    mov         r10d,           [rsi]   ; load 4 bytes
    mov         r11w,           [rsi+4] ; load 2 bytes
    mov         [rdi],          r10d    ; store 4 bytes
    mov         [rdi+4],        r11w    ; store 2 bytes
    mov         r10b,           [rsi+6] ; load 1 byte
    mov         [rdi+6],        r10b    ; store 1 byte
    ret

L_8B:
    mov         r10,           [rsi]    ; load 8 bytes
    mov         [rdi],         r10      ; store 8 bytes
    ret 

align   16
L_9B:
    mov         r10,           [rsi]    ; load 8 bytes
    mov         r11b,          [rsi+8]  ; load 1 byte 
    mov         [rdi],         r10      ; store 8 bytes
    mov         [rdi+8],       r11b     ; store 1 byte
    ret
L_10B:
    mov         r10,           [rsi]    ; load 8 bytes
    mov         r11w,          [rsi+8]  ; load 2 bytes
    mov         [rdi],         r10      ; store 8 bytes
    mov         [rdi+8],       r11w     ; store 2 bytes
    ret

align   16
L_11B:
    mov         r10,           [rsi]    ; load 8 bytes
    mov         r11w,          [rsi+8]  ; load 2 bytes
    mov         [rdi],         r10      ; store 8 bytes
    mov         [rdi+8],       r11w     ; store 2 bytes
    mov         r10b,          [rsi+10] ; load 1 byte
    mov         [rdi+10],      r10b     ; store 1 byte
    ret

L_12B:
    mov         r10,            [rsi]   ; load 8 bytes
    mov         r11d,           [rsi+8] ; load 4 bytes
    mov         [rdi],          r10     ; store 8 bytes
    mov         [rdi+8],        r11d    ; store 4 bytes
    ret

align   16
L_13B:
    mov         r10,            [rsi]   ; load 8 bytes
    mov         r11d,           [rsi+8] ; load 4 bytes
    mov         [rdi],          r10     ; store 8 bytes
    mov         [rdi+8],        r11d    ; store 4 bytes
    mov         r10b,           [rsi+12]; load 1 byte
    mov         [rdi+12],       r10b    ; store 1 byte
    ret

L_14B:
    mov         r10,            [rsi]   ; load 8 bytes
    mov         [rdi],          r10     ; store 8 bytes
    mov         r11d,           [rsi+8] ; load 4 bytes
    mov         [rdi+8],        r11d    ; store 4 bytes
    mov         r10w,           [rsi+12]; load 2 bytes
    mov         [rdi+12],       r10w    ; store 2 bytes
    ret

align   16
L_15B:
    mov         r10,            [rsi]   ; load 8 bytes
    mov         r11d,           [rsi+8] ; load 4 bytes
    mov         [rdi],          r10     ; store 8 bytes    
    mov         [rdi+8],        r11d    ; store 4 bytes
    mov         r10w,           [rsi+12]; load 2 bytes
    mov         r11b,           [rsi+14]; load 1 byte
    mov         [rdi+12],       r10w    ; store 2 bytes    
    mov         [rdi+14],       r11b    ; store 1 byte
    ret

L_16B:
    movdqu      xmm0,          [rsi]
    movdqu      [rdi],         xmm0
    ret
; ------------------------------------------------

align 16
L_ge_256:
    cmp         rdx,            0x800 ; 2048
    jae         L_fast_str
    ; first partial block( <32bytes)
    xor         r8,		r8
    mov         r8d,            edi
    neg         r8d
    and         r8d,            1FH
    jz          L_4ymm_loop
    vmovdqu     ymm0,		[rsi]	 ; save the first partial block
    add         rsi,		r8
    vmovdqu     [rdi],          ymm0
    add         rdi,		r8
    sub         rdx,		r8

align 16
L_4ymm_loop:
    sub         rdx,		0x80     ; 128 bytes per loop
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
    cmp         rdx,            0x80
    jae         L_4ymm_loop

align 16
L_single_ymm_loop:                      ;[0,128) bytes
    sub         rdx,		0x20
    jb          L_remaining_bytes
    vmovdqu     ymm0,		[rsi]
    add         rsi,		0x20
    vmovdqa     [rdi],		ymm0
    add 	rdi,		0x20
    jmp		L_single_ymm_loop

L_remaining_bytes:  ; <= 32 bytes
    add         rdx,    	0x20
    jz          FINISH
    vmovdqu     ymm0,		[rsi+rdx-0x20]
    vmovdqu     [rdi+rdx-0x20], ymm0
    vzeroupper         ; end of AVX mode

FINISH:
    ret

; Enhanced Fast Strings:  [2048, 1024*1024*6)  bytes
; Since there is overhead to set up REP MOVSB operation,
; REP MOVSB isn't faster on short data
; more recent processors support Fast Strings
; see intel sdm 7.3.9.3 Fast-String Operation
align 16
L_fast_str:  ;
    cmp         rdx,            0x400000  ; 4 * 1024*1024
    jae         L_non_temporal
    mov         rcx,            rdx     ; load copy len to rcx
    rep movsb
    ret

; non_temporal  avoid write cache pollution
; 1024 * 1024 * 6
; minimizing cache pollution
align 16
L_non_temporal:
    ; first partial block( <32bytes)
    xor         r8,		r8
    mov         r8d,            edi
    neg         r8d
    and         r8d,            1FH
    jz          L_non_temporal_loop      ; destination already aligned
    vmovdqu     ymm0,		[rsi]	 ; save the first partial block
    add         rsi,		r8
    vmovdqu     [rdi],          ymm0
    add         rdi,		r8
    sub         rdx,		r8
L_non_temporal_loop:
    ; prefetchnta
    ; movups  xmm0, [rcx + rdx]       ; load
    ; movntps (-128)[rcx], xmm0       ; store
    prefetchnta [rsi + 512]           ; in advance ; 
    prefetchnta [rsi + 576]
    vmovdqu     ymm0,           [rsi]         
    vmovdqu     ymm1,           [rsi+0x20]
    vmovdqu     ymm2,           [rsi+0x40]
    vmovdqu     ymm3,           [rsi+0x60]    
    add         rsi,            0x80          ; 32byte * 4
    sub         rdx,            0x80
    vmovntdq    [rdi],          ymm0
    vmovntdq    [rdi+0x20],     ymm1
    vmovntdq    [rdi+0x40],     ymm2
    vmovntdq    [rdi+0x60],     ymm3
    add         rdi,            0x80
    cmp         rdx,            0x80   
    ja          L_non_temporal_loop
    sfence                              ; it is weak type, need sfence.
                                        ; now, last block
    lea 	r8,             [rsi+rdx]
    vmovdqu     ymm0,           [r8-0x80]
    vmovdqu     ymm1,           [r8-0x60]
    vmovdqu     ymm2,           [r8-0x40]
    vmovdqu     ymm3,           [r8-0x20]
    lea 	r9,             [rdi+rdx]
    vmovdqu     [r9-0x80],      ymm0
    vmovdqu     [r9-0x60],      ymm1
    vmovdqu     [r9-0x40],      ymm2
    vmovdqu     [r9-0x20],      ymm3
    vzeroupper         ; end of AVX mode
    ret