; yasm -f elf64 cbc_old.s -o cbc_old.o

section .data
align 16
shuffle_mask:
DQ 0x0405060700010203
DQ 0x0c0d0e0f08090a0b

; **********************************************************
; **********************************************************
section .text

align 16
global aes_cbc_decrypt_old
; aes_cbc_decrypt_old(input, output, expandkey, iv, length, keylen);
aes_cbc_decrypt_old:   
  ; linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0-7

      push   r8  ; save length

      vmovdqu xmm1, [shuffle_mask wrt rip]  ; shuffle_mask
      vmovdqu xmm5, [rdx+0x10]  ; rnd key 1
      vpshufb xmm5,xmm5,xmm1
      vmovdqu xmm6, [rdx+0x20]
      vpshufb xmm6,xmm6,xmm1
      vmovdqu xmm7, [rdx+0x30]
      vpshufb xmm7,xmm7,xmm1
      vmovdqu xmm8, [rdx+0x40]
      vpshufb xmm8,xmm8,xmm1
      vmovdqu xmm9, [rdx+0x50]
      vpshufb xmm9,xmm9,xmm1
      vmovdqu xmm10, [rdx+0x60]
      vpshufb xmm10,xmm10,xmm1
      vmovdqu xmm11, [rdx+0x70]
      vpshufb xmm11,xmm11,xmm1
      vmovdqu xmm12, [rdx+0x80]
      vpshufb xmm12,xmm12,xmm1
      vmovdqu xmm13, [rdx+0x90]
      vpshufb xmm13,xmm13,xmm1
      vmovdqu xmm14, [rdx+0xa0]
      vpshufb xmm14,xmm14,xmm1
      vmovdqu xmm15, [rdx]
      vpshufb xmm15,xmm15,xmm1

      vmovdqu xmm1, [rcx]   ; pre block cipher

      mov    rax, r9        ; key length
      cmp    rax,128
      jne    _key_192_256

      mov rax,0x0
 
align 16
_multiblocks_loop_128:
      cmp    r8,0x40
      jl     _single_block_128
      vmovdqu xmm0, [rdi+rax*1]
      vmovdqu xmm2, [rdi+rax*1+0x10]
      vmovdqu xmm3, [rdi+rax*1+0x20]
      vmovdqu xmm4, [rdi+rax*1+0x30]
      vpxor  xmm0,xmm0,xmm5
      vpxor  xmm2,xmm2,xmm5
      vpxor  xmm3,xmm3,xmm5
      vpxor  xmm4,xmm4,xmm5
      vaesdec xmm0,xmm0,xmm6
      vaesdec xmm2,xmm2,xmm6
      vaesdec xmm3,xmm3,xmm6
      vaesdec xmm4,xmm4,xmm6
      vaesdec xmm0,xmm0,xmm7
      vaesdec xmm2,xmm2,xmm7
      vaesdec xmm3,xmm3,xmm7
      vaesdec xmm4,xmm4,xmm7
      vaesdec xmm0,xmm0,xmm8
      vaesdec xmm2,xmm2,xmm8
      vaesdec xmm3,xmm3,xmm8
      vaesdec xmm4,xmm4,xmm8
      vaesdec xmm0,xmm0,xmm9
      vaesdec xmm2,xmm2,xmm9
      vaesdec xmm3,xmm3,xmm9
      vaesdec xmm4,xmm4,xmm9
      vaesdec xmm0,xmm0,xmm10
      vaesdec xmm2,xmm2,xmm10
      vaesdec xmm3,xmm3,xmm10
      vaesdec xmm4,xmm4,xmm10
      vaesdec xmm0,xmm0,xmm11
      vaesdec xmm2,xmm2,xmm11
      vaesdec xmm3,xmm3,xmm11
      vaesdec xmm4,xmm4,xmm11
      vaesdec xmm0,xmm0,xmm12
      vaesdec xmm2,xmm2,xmm12
      vaesdec xmm3,xmm3,xmm12
      vaesdec xmm4,xmm4,xmm12
      vaesdec xmm0,xmm0,xmm13
      vaesdec xmm2,xmm2,xmm13
      vaesdec xmm3,xmm3,xmm13
      vaesdec xmm4,xmm4,xmm13
      vaesdec xmm0,xmm0,xmm14
      vaesdec xmm2,xmm2,xmm14
      vaesdec xmm3,xmm3,xmm14
      vaesdec xmm4,xmm4,xmm14
      vaesdeclast xmm0,xmm0,xmm15
      vaesdeclast xmm2,xmm2,xmm15
      vaesdeclast xmm3,xmm3,xmm15
      vaesdeclast xmm4,xmm4,xmm15
      vpxor  xmm0,xmm0,xmm1
      vmovdqu xmm1, [rdi+rax*1]
      vpxor  xmm2,xmm2,xmm1
      vmovdqu xmm1, [rdi+rax*1+0x10]
      vpxor  xmm3,xmm3,xmm1
      vmovdqu xmm1, [rdi+rax*1+0x20]
      vpxor  xmm4,xmm4,xmm1
      vmovdqu xmm1, [rdi+rax*1+0x30]
      vmovdqu  [rsi+rax*1],xmm0
      vmovdqu  [rsi+rax*1+0x10],xmm2
      vmovdqu  [rsi+rax*1+0x20],xmm3
      vmovdqu  [rsi+rax*1+0x30],xmm4
      add    rax,0x40
      sub    r8,0x40
      jmp    _multiblocks_loop_128

align 16
_single_block_128:
      cmp    r8,0x0
      je     _exit
      vmovdqu xmm0, [rdi+rax*1]
      vmovdqa xmm2,xmm0
      vpxor  xmm0,xmm0,xmm5
      vaesdec xmm0,xmm0,xmm6
      vaesdec xmm0,xmm0,xmm7
      vaesdec xmm0,xmm0,xmm8
      vaesdec xmm0,xmm0,xmm9
      vaesdec xmm0,xmm0,xmm10
      vaesdec xmm0,xmm0,xmm11
      vaesdec xmm0,xmm0,xmm12
      vaesdec xmm0,xmm0,xmm13
      vaesdec xmm0,xmm0,xmm14
      vaesdeclast xmm0,xmm0,xmm15
      vpxor  xmm0,xmm0,xmm1
      vmovdqu  [rsi+rax*1],xmm0
      vmovdqa xmm1,xmm2
      add    rax,0x10
      sub    r8,0x10
      jmp    _single_block_128


_key_192_256:  
      vmovdqu xmm3, [rdx+0xb0]
      vpshufb xmm3,xmm3, [shuffle_mask wrt rip]       
      cmp    eax,192
      jne    _key_256

      vmovdqu xmm4, [rdx+0xc0]
      vpshufb xmm4,xmm4, [shuffle_mask wrt rip]   
      mov rax,0x0

align 16
_singleBlock_loopTop_192:
      vmovdqu xmm0, [rdi+rax*1]
      vmovdqa xmm2,xmm0
      vpxor  xmm0,xmm0,xmm5
      vaesdec xmm0,xmm0,xmm6
      vaesdec xmm0,xmm0,xmm7
      vaesdec xmm0,xmm0,xmm8
      vaesdec xmm0,xmm0,xmm9
      vaesdec xmm0,xmm0,xmm10
      vaesdec xmm0,xmm0,xmm11
      vaesdec xmm0,xmm0,xmm12
      vaesdec xmm0,xmm0,xmm13
      vaesdec xmm0,xmm0,xmm14
      vaesdec xmm0,xmm0,xmm3
      vaesdec xmm0,xmm0,xmm4
      vaesdeclast xmm0,xmm0,xmm15
      vpxor  xmm0,xmm0,xmm1
      vmovdqu  [rsi+rax*1],xmm0
      vmovdqa xmm1,xmm2
      add    rax,0x10
      sub    r8,0x10
      jne    _singleBlock_loopTop_192
      jmp    _exit
      

_key_256:
      mov rax,0x0

align 16
_key_256_loop:
      vmovdqu xmm0, [rdi+rax*1]
      vmovdqa xmm2,xmm0
      vpxor  xmm0,xmm0,xmm5
      vaesdec xmm0,xmm0,xmm6
      vaesdec xmm0,xmm0,xmm7
      vaesdec xmm0,xmm0,xmm8
      vaesdec xmm0,xmm0,xmm9
      vaesdec xmm0,xmm0,xmm10
      vaesdec xmm0,xmm0,xmm11
      vaesdec xmm0,xmm0,xmm12
      vaesdec xmm0,xmm0,xmm13
      vaesdec xmm0,xmm0,xmm14
      vaesdec xmm0,xmm0,xmm3
      vmovdqu xmm4, [rdx+0xc0]
      vpshufb xmm4,xmm4,[shuffle_mask wrt rip]          
      vaesdec xmm0,xmm0,xmm4
      vmovdqu xmm4, [rdx+0xd0]
      vpshufb xmm4,xmm4,[shuffle_mask wrt rip]  
      vaesdec xmm0,xmm0,xmm4
      vmovdqu xmm4, [rdx+0xe0]
      vpshufb xmm4,xmm4,[shuffle_mask wrt rip]  
      vaesdec xmm0,xmm0,xmm4
      vaesdeclast xmm0,xmm0,xmm15
      vpxor  xmm0,xmm0,xmm1
      vmovdqu  [rsi+rax*1],xmm0
      vmovdqa xmm1,xmm2
      add    rax,0x10
      sub    r8,0x10
      jne    _key_256_loop
      jmp    _exit

_exit:      
      vmovdqu  [rcx],xmm1   ; store back to iv
      pop rax
      ;leave  
      ret