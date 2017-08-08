
section .data
align 16
shuffle_mask:
DD 0FFFFFFFFh
DD 03020100h
DD 07060504h
DD 0B0A0908h

section .text
global _foo
_foo:
	mov rax, 7
	ret


align 16
global aes_cbc_decrypt
aes_cbc_decrypt:   
  ; linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0-7
  push   rax  ; length
  push   rbx
 
  vmovdqu xmm1,  [shuffle_mask wrt rip]  ; shuffle_mask
  vmovdqu xmm0,  [rcx] ; iv -> pre block
  mov rax, 0x0

align 16
_multiblocks_loop:
  cmp    r8, 0x60
  jl     _single_loop
  vmovdqu xmm10,  [rdi+rax*1]
  vmovdqu xmm11,  [rdi+rax*1+0x10]
  vmovdqu xmm12,  [rdi+rax*1+0x20]
  vmovdqu xmm13,  [rdi+rax*1+0x30]
  vmovdqu xmm14,  [rdi+rax*1+0x40]
  vmovdqu xmm15,  [rdi+rax*1+0x50]
  vmovdqa xmm4,xmm10
  vmovdqa xmm5,xmm11
  vmovdqa xmm6,xmm12
  vmovdqa xmm7,xmm13
  vmovdqa xmm8,xmm14
  vmovdqa xmm9,xmm15
  vmovdqu xmm2,  [rdx+0x10]  ; tmp key 0
  vpshufb xmm2,xmm2,xmm1
  vpxor  xmm4,xmm4,xmm2
  vpxor  xmm5,xmm5,xmm2
  vpxor  xmm6,xmm6,xmm2
  vpxor  xmm7,xmm7,xmm2
  vpxor  xmm8,xmm8,xmm2
  vpxor  xmm9,xmm9,xmm2
  vmovdqu xmm3,  [rdx+0x20]
  vpshufb xmm3,xmm3,xmm1
  vmovdqu xmm2,  [rdx+0x30]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm3
  vaesdec xmm5,xmm5,xmm3
  vaesdec xmm6,xmm6,xmm3
  vaesdec xmm7,xmm7,xmm3
  vaesdec xmm8,xmm8,xmm3
  vaesdec xmm9,xmm9,xmm3
  vaesdec xmm4,xmm4,xmm2
  vaesdec xmm5,xmm5,xmm2
  vaesdec xmm6,xmm6,xmm2
  vaesdec xmm7,xmm7,xmm2
  vaesdec xmm8,xmm8,xmm2
  vaesdec xmm9,xmm9,xmm2
  vmovdqu xmm3,  [rdx+0x40]
  vpshufb xmm3,xmm3,xmm1
  vmovdqu xmm2,  [rdx+0x50]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm3
  vaesdec xmm5,xmm5,xmm3
  vaesdec xmm6,xmm6,xmm3
  vaesdec xmm7,xmm7,xmm3
  vaesdec xmm8,xmm8,xmm3
  vaesdec xmm9,xmm9,xmm3
  vaesdec xmm4,xmm4,xmm2
  vaesdec xmm5,xmm5,xmm2
  vaesdec xmm6,xmm6,xmm2
  vaesdec xmm7,xmm7,xmm2
  vaesdec xmm8,xmm8,xmm2
  vaesdec xmm9,xmm9,xmm2
  vmovdqu xmm3,  [rdx+0x60]
  vpshufb xmm3,xmm3,xmm1
  vmovdqu xmm2,  [rdx+0x70]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm3
  vaesdec xmm5,xmm5,xmm3
  vaesdec xmm6,xmm6,xmm3
  vaesdec xmm7,xmm7,xmm3
  vaesdec xmm8,xmm8,xmm3
  vaesdec xmm9,xmm9,xmm3
  vaesdec xmm4,xmm4,xmm2
  vaesdec xmm5,xmm5,xmm2
  vaesdec xmm6,xmm6,xmm2
  vaesdec xmm7,xmm7,xmm2
  vaesdec xmm8,xmm8,xmm2
  vaesdec xmm9,xmm9,xmm2
  vmovdqu xmm3,  [rdx+0x80]
  vpshufb xmm3,xmm3,xmm1
  vmovdqu xmm2,  [rdx+0x90]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm3
  vaesdec xmm5,xmm5,xmm3
  vaesdec xmm6,xmm6,xmm3
  vaesdec xmm7,xmm7,xmm3
  vaesdec xmm8,xmm8,xmm3
  vaesdec xmm9,xmm9,xmm3
  vaesdec xmm4,xmm4,xmm2
  vaesdec xmm5,xmm5,xmm2
  vaesdec xmm6,xmm6,xmm2
  vaesdec xmm7,xmm7,xmm2
  vaesdec xmm8,xmm8,xmm2
  vaesdec xmm9,xmm9,xmm2
  vmovdqu xmm3,  [rdx+0xa0]
  vpshufb xmm3,xmm3,xmm1
  vmovdqu xmm2,  [rdx]
  vpshufb xmm2,xmm2,xmm1
  vaesdec xmm4,xmm4,xmm3
  vaesdec xmm5,xmm5,xmm3
  vaesdec xmm6,xmm6,xmm3
  vaesdec xmm7,xmm7,xmm3
  vaesdec xmm8,xmm8,xmm3
  vaesdec xmm9,xmm9,xmm3
  vaesdeclast xmm4,xmm4,xmm2
  vaesdeclast xmm5,xmm5,xmm2
  vaesdeclast xmm6,xmm6,xmm2
  vaesdeclast xmm7,xmm7,xmm2
  vaesdeclast xmm8,xmm8,xmm2
  vaesdeclast xmm9,xmm9,xmm2
  vpxor  xmm4,xmm4,xmm0
  vpxor  xmm5,xmm5,xmm10
  vpxor  xmm6,xmm6,xmm11
  vpxor  xmm7,xmm7,xmm12
  vpxor  xmm8,xmm8,xmm13
  vpxor  xmm9,xmm9,xmm14
  vmovdqa xmm0,xmm15
  vmovdqu   [rsi+rax*1],xmm4
  vmovdqu   [rsi+rax*1+0x10],xmm5
  vmovdqu   [rsi+rax*1+0x20],xmm6
  vmovdqu   [rsi+rax*1+0x30],xmm7
  vmovdqu   [rsi+rax*1+0x40],xmm8
  vmovdqu   [rsi+rax*1+0x50],xmm9
  add    rax,0x60
  sub    r8,0x60
  jmp    _multiblocks_loop
  

align 16
_single_loop:
  cmp    r8,0x0
  je     _exit
  vmovdqu xmm10,  [rdi+rax*1]
  vmovdqa xmm4,xmm10
  vmovdqu xmm2,  [rdx+0x10]
  vpshufb xmm2,xmm2,xmm1
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
