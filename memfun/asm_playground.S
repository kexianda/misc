; Intel format
; yasm -f elf64 xxx.s -o xxx.o
; Author: Ke, Xianda

section .data

; ==============================================
section .text

align 16
global asm_playground ;
asm_playground:
	; Linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0~7

	; Calculate size of first block up to first regular boundary of dest
	mov     edx, edi
	neg     edx
	and     edx, 1FH
	; Skip if dest aligned by 32


	ret
