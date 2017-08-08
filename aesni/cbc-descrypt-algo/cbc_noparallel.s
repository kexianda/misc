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

  jmp     _single_loop  ; jmp to _single_loop for testing

align 16
_single_loop:
  cmp    r8,0x0
  je     _exit
  vmovdqu xmm10,  [rdi+rax*1]
  vmovdqa xmm4,xmm10

  vpxor  xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x20]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x30]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x40]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x50]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x60]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x70]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x80]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0x90]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx+0xa0]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm2
  vmovdqu xmm2,  [rdx]
  vpshufb xmm2,xmm2,xmm1
  vaesdeclast xmm4,xmm4,xmm2
  vpxor  xmm4,xmm4,xmm0
  vmovdqu   [rsi+rax*1],xmm4
  vmovdqa xmm0,xmm10
  add    rax, 0x10
  sub    r8, 0x10
  jmp   _single_loop
  vmovdqu [rcx], xmm0

_exit:
  pop    rbx
  pop    rax
  leaveq
  retq
