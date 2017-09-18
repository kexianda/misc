; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda


; Notes
; no vzeroupper 
;   VZEROUPPER on Skylake does not have the same penalties as earlier designs.
;   Travis D. said that: "the vzero family is still very much needed on Skylake. 
;   The big one-time state transition penalty has disappeared, but was replaced 
;   with the ongoing penalty of blending the  high half of the register for all 
;   the non-VEX encoded instructions."
;
;   On Xeon Phi platform, no need to use VZEROUPPER. on Skylake platform, to avoid 
;   use VZEROUPPER, we use zmm16~zmm31.
;
;   see: https://software.intel.com/en-us/forums/intel-isa-extensions/topic/704023
; 

section .data

; ==============================================
section .text

global _i_memcpy_avx512 ;
align 16
_i_memcpy_avx512:
    lea 	r8,             [rsi+rdx]
    lea 	r9,             [rdi+rdx]
    cmp         rdx,            0x200 ;  512
    ja          L_gt_512bytes
    cmp         rdx,             0x10    ; 16
    jbe         L_cpySmall              ; jmp to  [0, 64] bytes block 

L_le_512bytes:                         ; Less Equal 512bytes: (256, 512]
    cmp         dx,             0x100  ; 256
    jbe         L_le_256bytes          ; jmp to  (128, 256] bytes block
    vmovups     zmm16,		[rsi]  ; load 64 bytes
    vmovups     zmm17,		[rsi+0x40]  
    vmovups     zmm18,		[rsi+0x80]  
    vmovups     zmm19,		[rsi+0xC0]  
    vmovups     zmm20,		[r8-0x100]  
    vmovups     zmm21,		[r8-0xC0]   
    vmovups     zmm22,		[r8-0x80]   
    vmovups     zmm23,		[r8-0x40]  
    vmovups     [rdi],          zmm16       
    vmovups     [rdi+0x40],     zmm17    
    vmovups     [rdi+0x80],     zmm18       
    vmovups     [rdi+0xC0],     zmm19
    vmovups     [r9-0x100],     zmm20
    vmovups     [r9-0xC0],      zmm21
    vmovups     [r9-0x80],      zmm22
    vmovups     [r9-0x40],      zmm23
    ret 
 
align 16
L_le_256bytes:                       ; (128, 256] bytes
    cmp         dx,             0x80 ; 128
    jbe         L_le_128bytes        
    vmovups     zmm16,		[rsi]  
    vmovups     zmm17,		[rsi+0x40] 
    vmovups     zmm18,		[r8-0x80]
    vmovups     zmm19,		[r8-0x40]
    vmovups     [rdi],          zmm16
    vmovups     [rdi+0x40],     zmm17
    vmovups     [r9-0x80],      zmm18
    vmovups     [r9-0x40],      zmm19
    ret    

    ; TODO: copy backward?

;-----------------------------------------------------------
;-----------------------------------------------------------
align 16
L_le_128bytes:                          ; (64, 128] 
    cmp         dl,             0x40    ;
    jbe         L_le_64bytes
    vmovups     zmm16,		[rsi]
    vmovups     zmm17,		[r8-0x40]
    vmovups     [rdi],          zmm16
    vmovups     [r9-0x40],      zmm17
    ret

align 16
L_le_64bytes:                           ; (32,64]
    cmp         dl,             0x20    ;
    jbe          L_le_32bytes
    vmovdqu     ymm0,		[rsi]  ; 16 bytes per time
    vmovdqu     ymm1,		[r8-0x20]
    vmovdqu     [rdi],          ymm0
    vmovdqu     [r9-0x20], ymm1
    ret  ; 

align 16
L_le_32bytes:                          ; (16,32]
    cmp         dl,             0x10   ;
    jbe         L_cpySmall
    vmovdqu     xmm0,		[rsi]  ; 16 bytes per time
    vmovdqu     xmm1,		[r8 - 0x10]  ;
    vmovdqu     [rdi],          xmm0
    vmovdqu     [r9-0x10],      xmm1
    ret

;-------------------------------------------------------
; this sub routine handle small block: [0,16) bytes 
;-------------------------------------------------------
align 16
L_cpySmallTab:
    dq  L_0B
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
    mov         r10,    [L_cpySmallTab + rdx*8]     
    jmp         r10 

L_0B:
    ret
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
    movdqu      [rdi],           xmm0
    ret
; ------------------------------------------------



;-------------------------------------------------------
; this sub routine handle (512, 1024] bytes block
;-------------------------------------------------------
align 16
L_gt_512bytes:
    cmp     rdx,    0x400
    ja      L_gt_1024bytes
    prefetcht1      [rsi]           ; prefetcht1: -> L1, L2. try prefetchnta?
    prefetcht1      [rsi+0x40]
    prefetcht1      [rsi+0x80]
    prefetcht1      [rsi+0xC0]
    prefetcht1      [rsi+0x100]
    prefetcht1      [rsi+0x140]
    prefetcht1      [rsi+0x180]
    prefetcht1      [rsi+0x1C0]
    prefetcht1      [r8-0x200]
    prefetcht1      [r8-0x1C0]
    prefetcht1      [r8-0x180]
    prefetcht1      [r8-0x140]
    prefetcht1      [r8-0x100]
    prefetcht1      [r8-0xC0]
    prefetcht1      [r8-0x80]
    prefetcht1      [r8-0x40]

    vmovups     zmm16,      [rsi]      ; Load 
    vmovups     zmm17,      [rsi+0x40]
    vmovups     zmm18,      [rsi+0x80]
    vmovups     zmm19,      [rsi+0xC0]
    vmovups     zmm20,      [rsi+0x100]
    vmovups     zmm21,      [rsi+0x140]
    vmovups     zmm22,      [rsi+0x180]
    vmovups     zmm23,      [rsi+0x1C0]

    vmovups     zmm24,      [r8-0x200]
    vmovups     zmm25,      [r8-0x1C0]
    vmovups     zmm26,      [r8-0x180]
    vmovups     zmm27,      [r8-0x140]
    vmovups     zmm28,      [r8-0x100]
    vmovups     zmm29,      [r8-0xC0]
    vmovups     zmm30,      [r8-0x80]
    vmovups     zmm31,      [r8-0x40]

    vmovups     [rdi],          zmm16  ; Store
    vmovups     [rdi+0x40],     zmm17
    vmovups     [rdi+0x80],     zmm18
    vmovups     [rdi+0xC0],     zmm19
    vmovups     [rdi+0x100],    zmm20
    vmovups     [rdi+0x140],    zmm21
    vmovups     [rdi+0x180],    zmm22
    vmovups     [rdi+0x1C0],    zmm23
    
    vmovups     [r9-0x200],     zmm24
    vmovups     [r9-0x1C0],     zmm25
    vmovups     [r9-0x180],     zmm26
    vmovups     [r9-0x140],     zmm27
    vmovups     [r9-0x100],     zmm28
    vmovups     [r9-0xC0],      zmm29
    vmovups     [r9-0x80],      zmm30
    vmovups     [r9-0x40],      zmm31
    ret


;-------------------------------------------------------
; this sub routine handle (1024, 512K)  bytes block
;-------------------------------------------------------   
align 16
L_gt_1024bytes:
    cmp     rdx,             0x80000  ; 512K threshold ?
    jae     L_nt;            
L_8ZMM_loop:
    prefetcht1  [rsi + 0x200]   ; prefetch data.  512bytes in advance?
    prefetcht1  [rsi + 0x240]   ; prefetchnta, prefetcht1/prefetcht2 ?
    prefetcht1  [rsi + 0x280]
    prefetcht1  [rsi + 0x2C0]
    prefetcht1  [rsi + 0x300]
    prefetcht1  [rsi + 0x340]
    prefetcht1  [rsi + 0x380]
    prefetcht1  [rsi + 0x3C0]
    vmovups     zmm16,       [rsi]
    vmovups     zmm17,       [rsi+0x40]
    vmovups     zmm18,       [rsi+0x80]
    vmovups     zmm19,       [rsi+0xC0]
    vmovups     zmm20,       [rsi+0x100]
    vmovups     zmm21,       [rsi+0x140]
    vmovups     zmm22,       [rsi+0x180]
    vmovups     zmm23,       [rsi+0x1C0]
    add         rsi,         0x200
    vmovups     [rdi],       zmm16       
    vmovups     [rdi+0x40],  zmm17
    vmovups     [rdi+0x80],  zmm18
    vmovups     [rdi+0xC0],  zmm19
    vmovups     [rdi+0x100], zmm20
    vmovups     [rdi+0x140], zmm21
    vmovups     [rdi+0x180], zmm22
    vmovups     [rdi+0x1C0], zmm23
    add         rdi,        0x200    ; 8*64=512 bytes per loop
    sub         rdx,        0x200   
    cmp         rdx,        0x200
    ja          L_8ZMM_loop
    
    ; tailing bytes
    vmovups     zmm16,       [r8-0x200]
    vmovups     zmm17,       [r8-0x1C0]
    vmovups     zmm18,       [r8-0x180]
    vmovups     zmm19,       [r8-0x140]
    vmovups     zmm20,       [r8-0x100]
    vmovups     zmm21,       [r8-0xC0]
    vmovups     zmm22,       [r8-0x80]
    vmovups     zmm23,       [r8-0x40]
    vmovups     [r9-0x200],  zmm16
    vmovups     [r9-0x1C0],  zmm17
    vmovups     [r9-0x180],  zmm18      
    vmovups     [r9-0x140],  zmm19      
    vmovups     [r9-0x100],  zmm20      
    vmovups     [r9-0xC0],   zmm21     
    vmovups     [r9-0x80],   zmm22     
    vmovups     [r9-0x40],   zmm23     
    ret

;-------------------------------------------------------
; this sub routine handle [512K, +oo) bytes block
; 
; non-temporal/ Write Combining Buffer(64B)/ ILP
;
; 'prefetchnta' fetches data into L1 while minimizing pollution to other levels of cache
; 'vmovntdq' writes data to memeory directly, avoid cache pollution. CPU will write batch
; of data into memory.
; 
;-------------------------------------------------------
align 16
L_nt:           
    ; first partial block <64bytes
    mov         ecx,            edi
    neg         ecx
    and         ecx,            3FH      ; ecx is the number of bytes in partial block
    jz          L_nt_loop                ; if destination is already aligned
    vmovups     zmm16,		[rsi]	 ; save the first partial block
    add         rsi,		rcx
    vmovups     [rdi],          zmm16
    add         rdi,		rcx
    sub         rdx,		rcx

L_nt_loop:    ; now, aligned  
    prefetchnta [rsi + 0x200]           ; 512 bytes in advance ; 
    prefetchnta [rsi + 0x240]           ; 
    prefetchnta [rsi + 0x280]           ; 
    prefetchnta [rsi + 0x2C0]           ; 
    vmovdqu64   zmm16,           [rsi]         
    vmovdqu64   zmm17,           [rsi+0x40]       
    vmovdqu64   zmm18,           [rsi+0x80]
    vmovdqu64   zmm19,           [rsi+0xC0]
    vmovntdq    [rdi],           zmm16
    vmovntdq    [rdi+0x40],      zmm17
    vmovntdq    [rdi+0x80],      zmm18
    vmovntdq    [rdi+0xC0],      zmm19
    sub         rdx,             0x100   ; 4*64 = 256 bytes per loop
    add         rsi,             0x100   
    add         rdi,             0x100
    cmp         rdx,             0x100
    ja          L_nt_loop
    
    ; we need store fence here
    sfence 

    ; last block
    vmovups     zmm16,          [r8-0x100]
    vmovups     zmm17,          [r8-0xC0]
    vmovups     zmm18,          [r8-0x80]
    vmovups     zmm19,          [r8-0x40]
    vmovups     [r9-0x100],     zmm16
    vmovups     [r9-0xC0],      zmm17
    vmovups     [r9-0x80],      zmm18
    vmovups     [r9-0x40],      zmm19
    ; vzeroupper         ; not needed on Xeon(Skylake) for zmm16~31
    ret