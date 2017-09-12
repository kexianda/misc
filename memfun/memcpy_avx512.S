; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda


; Notes
; 1.no vzeroupper 
;   VZEROUPPER on Skylake does not have the same penalties as earlier designs.
; 
;   Travis D. said that: "For what it's worth, the vzero family is still very 
;   much needed on Skylake. The big one-time state transition penalty has 
;   disappeared, but was replaced with the ongoing penalty of blending the 
;   high half of the register for all all the non-VEX encoded instructions."

;   so, use zmm16~zmm31 ?
;   see: https://software.intel.com/en-us/forums/intel-isa-extensions/topic/704023
; 
; 2. ...


section .data

; ==============================================
section .text

global _i_memcpy_avx512 ;
align 16

L_start:
    cmp     rdx,    0x400 ; 1024
    jae     L_ge_1024bytes
    
    ; TODO: <= 512 bytes

    ; TODO: copy backward?
L_ge_1024bytes:
    cmp     rdx,    0x80000  ; 512K
    jae     L_nt;            ; non-temporal stores 
L_ge_1024bytes_loop:
    vmovups     zmm16,       [rsi]
    vmovups     zmm17,       [rsi+0x40]
    vmovups     zmm18,       [rsi+0x80]
    vmovups     zmm19,       [rsi+0xC0]
    vmovups     zmm20,       [rsi+0x100]
    vmovups     zmm21,       [rsi+0x140]
    vmovups     zmm22,       [rsi+0x180]
    vmovups     zmm23,       [rsi+0x1C0]
    add         rsi,        0x200
    prefetchnta [rsi + 0x200]   ;  512bytes in advance?
    prefetchnta [rsi + 0x240]
    prefetchnta [rsi + 0x280]
    prefetchnta [rsi + 0x2C0]
    prefetchnta [rsi + 0x300]
    prefetchnta [rsi + 0x340]
    prefetchnta [rsi + 0x380]
    prefetchnta [rsi + 0x3C0]
    vmovups     [rdi],       zmm16       
    vmovups     [rdi+0x40],  zmm17
    vmovups     [rdi+0x80],  zmm18
    vmovups     [rdi+0xC0],  zmm19
    vmovups     [rdi+0x100], zmm20
    vmovups     [rdi+0x140], zmm21
    vmovups     [rdi+0x180], zmm22
    vmovups     [rdi+0x1C0], zmm23
    add         rdi,        0x200
    sub         rdx,        0x200
    cmp         rdx,        0x200
    ja          L_ge_1024bytes_loop:
    ; tailing bytes
    lea         r8,         [rsi+rdx]
    lea         r9,         [rdi+rdx]
    vmovups     zmm16,       [r8-0x200]
    vmovups     zmm17,       [r8-0x1C0]
    vmovups     zmm18,       [r8-0x180]
    vmovups     zmm19,       [r8-0x140]
    vmovups     zmm20,       [r8-0x100]
    vmovups     zmm21,       [r8-0xC0]
    vmovups     zmm22,       [r8-0x80]
    vmovups     zmm23,       [r8-0x40]
    vmovups     [r9-0x200], zmm16
    vmovups     [r9-0x1C0], zmm17
    vmovups     [r9-0x180], zmm18      
    vmovups     [r9-0x140], zmm19      
    vmovups     [r9-0x100], zmm20      
    vmovups     [r9-0xC0],  zmm21     
    vmovups     [r9-0x80],  zmm22     
    vmovups     [r9-0x40],  zmm23     
    ret

; --------------------------------------------
; non-temporal
;
; fetches into L1 while minimizing pollution to other levels of cache
; write to memeory directly, avoid cache pollution. CPU will write batch
; of data into memory.
; 
; Skylake has bigger out-of-order window
; --------------------------------------------
L_nt:           
    ; first partial block <64bytes
    xor         r8,		r8
    mov         r8d,            edi
    neg         r8d
    and         r8d,            3FH
    jz          L_nt_loop                ; destination already aligned
    vmovdqu     zmm16,		[rsi]	 ; save the first partial block
    add         rsi,		r8
    vmovdqu     [rdi],          zmm16
    add         rdi,		r8
    sub         rdx,		r8

L_nt_loop:    ; now, aligned  
    prefetchnta [rsi + 0x200]           ; 512 bytes in advance ; 
    prefetchnta [rsi + 0x240]           ; or prefetcht1 ??
    prefetchnta [rsi + 0x280]           ; 512 bytes in advance ; 
    prefetchnta [rsi + 0x2C0]           ; or prefetcht1 ??
    vmovdqu64   zmm16,           [rsi]         
    vmovdqu64   zmm17,           [rsi+0x40]       
    vmovdqu64   zmm18,           [rsi+0x80]
    vmovdqu64   zmm19,           [rsi+0xC0]
    vmovntdq    [rdi]           zmm16
    vmovntdq    [rdi+0x40]      zmm17
    vmovntdq    [rdi+0x80]      zmm18
    vmovntdq    [rdi+0xC0]      zmm19
    sub         rdx             0x200
    add         rsi             0x200   ;  256 bytes
    add         rdi             0x200
    cmp         rdx             0x200
    ja          L_nt_loop
    
    sfence
    lea 	r8,             [rsi+rdx]
    vmovdqu     zmm16,          [r8-0x100]
    vmovdqu     zmm17,          [r8-0xC0]
    vmovdqu     zmm18,          [r8-0x80]
    vmovdqu     zmm19,          [r8-0x40]
    lea 	r9,             [rdi+rdx]
    vmovdqu     [r9-0x100],     zmm16
    vmovdqu     [r9-0xC0],      zmm17
    vmovdqu     [r9-0x80],      zmm18
    vmovdqu     [r9-0x40],      zmm19
    ; vzeroupper         ; not needed on Xeon(Skylake) for zmm16~31
    ret