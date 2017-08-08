single.s
; yasm -f elf64 aes.s -o aes.o

section .data
align 16
shuffle_mask:
DQ 0x0405060700010203
DQ 0x0c0d0e0f08090a0b

; **********************************************************
; **********************************************************
section .text

align 16
global aes_cbc_decrypt_noparallel
; RDI, RSI, RDX, RCX, R8, ;R9, XMM0-7
; aes_cbc_decrypt_noparallel(input, output, expandkey, iv, length);
aes_cbc_decrypt_noparallel:   
  ; linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0-7
  push   r8  ; length
  push   rbx
 
  vmovdqu xmm1,  [shuffle_mask wrt rip]  ; shuffle_mask
  vmovdqu xmm0,  [rcx] ; iv -> pre block
  mov rax, 0x0

  ;jmp     _single_loop  ; jmp to _single_loop for testing

  vmovdqu xmm5,  [rdx+0x10]
  vpshufb xmm5,xmm5,xmm1
  vmovdqu xmm6,  [rdx+0x20]
  vpshufb xmm6,xmm6,xmm1
  vmovdqu xmm7,  [rdx+0x30]
  vpshufb xmm7,xmm7,xmm1
  vmovdqu xmm8,  [rdx+0x40]
  vpshufb xmm8,xmm8,xmm1
  vmovdqu xmm9,  [rdx+0x50]
  vpshufb xmm9,xmm9,xmm1
  vmovdqu xmm10,  [rdx+0x60]
  vpshufb xmm10,xmm10,xmm1
  vmovdqu xmm11,  [rdx+0x70]
  vpshufb xmm11,xmm11,xmm1
  vmovdqu xmm12,  [rdx+0x80]
  vpshufb xmm12,xmm12,xmm1
  vmovdqu xmm13,  [rdx+0x90]
  vpshufb xmm13,xmm13,xmm1
  vmovdqu xmm14,  [rdx+0xa0]
  vpshufb xmm14,xmm14,xmm1
  vmovdqu xmm15,  [rdx]
  vpshufb xmm15,xmm15,xmm1

align 16
_single_loop:
  cmp    r8,0x0
  je     _exit
  vmovdqu xmm2,  [rdi+rax*1]
  vmovdqa xmm4,xmm2

  vpxor  xmm4,xmm4,xmm5
  
  vaesdec xmm4,xmm4,xmm6
  
  vaesdec xmm4,xmm4,xmm7
  
  vaesdec xmm4,xmm4,xmm8
  
  vaesdec xmm4,xmm4,xmm9
  
  vaesdec xmm4,xmm4,xmm10
  
  vaesdec xmm4,xmm4,xmm11
  
  vaesdec xmm4,xmm4,xmm12
  
  vaesdec xmm4,xmm4,xmm13

  vaesdec xmm4,xmm4,xmm14
  
  vaesdeclast xmm4,xmm4,xmm15
  vpxor  xmm4,xmm4,xmm0
  vmovdqu   [rsi+rax*1],xmm4
  vmovdqa xmm0,xmm2
  add    rax, 0x10
  sub    r8, 0x10
  jmp   _single_loop
  vmovdqu [rcx], xmm0

_exit:
  pop    rbx
  pop    rax
  leaveq
  retq
