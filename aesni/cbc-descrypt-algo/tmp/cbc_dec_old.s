   0x7fffe91231e0:   push   rbp
   0x7fffe91231e1:   mov    rbp,rsp
   0x7fffe91231e4:   push   r8
   0x7fffe91231e6:   vmovdqu xmm1,XMMWORD PTR [rip+0xfffffffffffffb62]        # 0x7fffe9122d50
   0x7fffe91231ee:   vmovdqu xmm5,XMMWORD PTR [rdx+0x10]
   0x7fffe91231f3:   vpshufb xmm5,xmm5,xmm1
   0x7fffe91231f8:   vmovdqu xmm6,XMMWORD PTR [rdx+0x20]
   0x7fffe91231fd:   vpshufb xmm6,xmm6,xmm1
   0x7fffe9123202:   vmovdqu xmm7,XMMWORD PTR [rdx+0x30]
   0x7fffe9123207:   vpshufb xmm7,xmm7,xmm1
   0x7fffe912320c:   vmovdqu xmm8,XMMWORD PTR [rdx+0x40]
   0x7fffe9123211:   vpshufb xmm8,xmm8,xmm1
   0x7fffe9123216:   vmovdqu xmm9,XMMWORD PTR [rdx+0x50]
   0x7fffe912321b:   vpshufb xmm9,xmm9,xmm1
   0x7fffe9123220:   vmovdqu xmm10,XMMWORD PTR [rdx+0x60]
   0x7fffe9123225:   vpshufb xmm10,xmm10,xmm1
   0x7fffe912322a:   vmovdqu xmm11,XMMWORD PTR [rdx+0x70]
   0x7fffe912322f:   vpshufb xmm11,xmm11,xmm1
   0x7fffe9123234:   vmovdqu xmm12,XMMWORD PTR [rdx+0x80]
   0x7fffe912323c:   vpshufb xmm12,xmm12,xmm1
   0x7fffe9123241:   vmovdqu xmm13,XMMWORD PTR [rdx+0x90]
   0x7fffe9123249:   vpshufb xmm13,xmm13,xmm1
   0x7fffe912324e:   vmovdqu xmm14,XMMWORD PTR [rdx+0xa0]
   0x7fffe9123256:   vpshufb xmm14,xmm14,xmm1
   0x7fffe912325b:   vmovdqu xmm15,XMMWORD PTR [rdx]
   0x7fffe912325f:   vpshufb xmm15,xmm15,xmm1
   0x7fffe9123264:   vmovdqu xmm1,XMMWORD PTR [rcx]
   0x7fffe9123268:   mov    eax,DWORD PTR [rdx-0x4]
   0x7fffe912326b:   cmp    eax,0x2c
   0x7fffe912326e:   jne    0x7fffe9123437
   0x7fffe9123274:   movabs rax,0x0
   0x7fffe912327e:   nop
   0x7fffe912327f:   nop
   0x7fffe9123280:   cmp    r8,0x40
   0x7fffe9123284:   jl     0x7fffe91233d0
   0x7fffe912328a:   vmovdqu xmm0,XMMWORD PTR [rdi+rax*1]
   0x7fffe912328f:   vmovdqu xmm2,XMMWORD PTR [rdi+rax*1+0x10]
   0x7fffe9123295:   vmovdqu xmm3,XMMWORD PTR [rdi+rax*1+0x20]
   0x7fffe912329b:   vmovdqu xmm4,XMMWORD PTR [rdi+rax*1+0x30]
   0x7fffe91232a1:   vpxor  xmm0,xmm0,xmm5
   0x7fffe91232a5:   vpxor  xmm2,xmm2,xmm5
   0x7fffe91232a9:   vpxor  xmm3,xmm3,xmm5
   0x7fffe91232ad:   vpxor  xmm4,xmm4,xmm5
   0x7fffe91232b1:   vaesdec xmm0,xmm0,xmm6
   0x7fffe91232b6:   vaesdec xmm2,xmm2,xmm6
   0x7fffe91232bb:   vaesdec xmm3,xmm3,xmm6
   0x7fffe91232c0:   vaesdec xmm4,xmm4,xmm6
   0x7fffe91232c5:   vaesdec xmm0,xmm0,xmm7
   0x7fffe91232ca:   vaesdec xmm2,xmm2,xmm7
   0x7fffe91232cf:   vaesdec xmm3,xmm3,xmm7
   0x7fffe91232d4:   vaesdec xmm4,xmm4,xmm7
   0x7fffe91232d9:   vaesdec xmm0,xmm0,xmm8
   0x7fffe91232de:   vaesdec xmm2,xmm2,xmm8
   0x7fffe91232e3:   vaesdec xmm3,xmm3,xmm8
   0x7fffe91232e8:   vaesdec xmm4,xmm4,xmm8
   0x7fffe91232ed:   vaesdec xmm0,xmm0,xmm9
   0x7fffe91232f2:   vaesdec xmm2,xmm2,xmm9
   0x7fffe91232f7:   vaesdec xmm3,xmm3,xmm9
   0x7fffe91232fc:   vaesdec xmm4,xmm4,xmm9
   0x7fffe9123301:   vaesdec xmm0,xmm0,xmm10
   0x7fffe9123306:   vaesdec xmm2,xmm2,xmm10
   0x7fffe912330b:   vaesdec xmm3,xmm3,xmm10
   0x7fffe9123310:   vaesdec xmm4,xmm4,xmm10
   0x7fffe9123315:   vaesdec xmm0,xmm0,xmm11
   0x7fffe912331a:   vaesdec xmm2,xmm2,xmm11
   0x7fffe912331f:   vaesdec xmm3,xmm3,xmm11
   0x7fffe9123324:   vaesdec xmm4,xmm4,xmm11
   0x7fffe9123329:   vaesdec xmm0,xmm0,xmm12
   0x7fffe912332e:   vaesdec xmm2,xmm2,xmm12
   0x7fffe9123333:   vaesdec xmm3,xmm3,xmm12
   0x7fffe9123338:   vaesdec xmm4,xmm4,xmm12
   0x7fffe912333d:   vaesdec xmm0,xmm0,xmm13
   0x7fffe9123342:   vaesdec xmm2,xmm2,xmm13
   0x7fffe9123347:   vaesdec xmm3,xmm3,xmm13
   0x7fffe912334c:   vaesdec xmm4,xmm4,xmm13
   0x7fffe9123351:   vaesdec xmm0,xmm0,xmm14
   0x7fffe9123356:   vaesdec xmm2,xmm2,xmm14
   0x7fffe912335b:   vaesdec xmm3,xmm3,xmm14
   0x7fffe9123360:   vaesdec xmm4,xmm4,xmm14
   0x7fffe9123365:   vaesdeclast xmm0,xmm0,xmm15
   0x7fffe912336a:   vaesdeclast xmm2,xmm2,xmm15
   0x7fffe912336f:   vaesdeclast xmm3,xmm3,xmm15
   0x7fffe9123374:   vaesdeclast xmm4,xmm4,xmm15
   0x7fffe9123379:   vpxor  xmm0,xmm0,xmm1
   0x7fffe912337d:   vmovdqu xmm1,XMMWORD PTR [rdi+rax*1]
   0x7fffe9123382:   vpxor  xmm2,xmm2,xmm1
   0x7fffe9123386:   vmovdqu xmm1,XMMWORD PTR [rdi+rax*1+0x10]
   0x7fffe912338c:   vpxor  xmm3,xmm3,xmm1
   0x7fffe9123390:   vmovdqu xmm1,XMMWORD PTR [rdi+rax*1+0x20]
   0x7fffe9123396:   vpxor  xmm4,xmm4,xmm1
   0x7fffe912339a:   vmovdqu xmm1,XMMWORD PTR [rdi+rax*1+0x30]
   0x7fffe91233a0:   vmovdqu XMMWORD PTR [rsi+rax*1],xmm0
   0x7fffe91233a5:   vmovdqu XMMWORD PTR [rsi+rax*1+0x10],xmm2
   0x7fffe91233ab:   vmovdqu XMMWORD PTR [rsi+rax*1+0x20],xmm3
   0x7fffe91233b1:   vmovdqu XMMWORD PTR [rsi+rax*1+0x30],xmm4
   0x7fffe91233b7:   add    rax,0x40
   0x7fffe91233bb:   sub    r8,0x40
   0x7fffe91233bf:   jmp    0x7fffe9123280
   0x7fffe91233c4:   nop
   0x7fffe91233c5:   nop
   0x7fffe91233c6:   nop
   0x7fffe91233c7:   nop
   0x7fffe91233c8:   nop
   0x7fffe91233c9:   nop
   0x7fffe91233ca:   nop
   0x7fffe91233cb:   nop
   0x7fffe91233cc:   nop
   0x7fffe91233cd:   nop
   0x7fffe91233ce:   nop
   0x7fffe91233cf:   nop
   0x7fffe91233d0:   cmp    r8,0x0
   0x7fffe91233d4:   je     0x7fffe9123430
   0x7fffe91233da:   vmovdqu xmm0,XMMWORD PTR [rdi+rax*1]
   0x7fffe91233df:   vmovdqa xmm2,xmm0
   0x7fffe91233e3:   vpxor  xmm0,xmm0,xmm5
   0x7fffe91233e7:   vaesdec xmm0,xmm0,xmm6
   0x7fffe91233ec:   vaesdec xmm0,xmm0,xmm7
   0x7fffe91233f1:   vaesdec xmm0,xmm0,xmm8
   0x7fffe91233f6:   vaesdec xmm0,xmm0,xmm9
   0x7fffe91233fb:   vaesdec xmm0,xmm0,xmm10
   0x7fffe9123400:   vaesdec xmm0,xmm0,xmm11
   0x7fffe9123405:   vaesdec xmm0,xmm0,xmm12
   0x7fffe912340a:   vaesdec xmm0,xmm0,xmm13
   0x7fffe912340f:   vaesdec xmm0,xmm0,xmm14
   0x7fffe9123414:   vaesdeclast xmm0,xmm0,xmm15
   0x7fffe9123419:   vpxor  xmm0,xmm0,xmm1
   0x7fffe912341d:   vmovdqu XMMWORD PTR [rsi+rax*1],xmm0
   0x7fffe9123422:   vmovdqa xmm1,xmm2
   0x7fffe9123426:   add    rax,0x10
   0x7fffe912342a:   sub    r8,0x10
   0x7fffe912342e:   jmp    0x7fffe91233d0
   0x7fffe9123430:   vmovdqu XMMWORD PTR [rcx],xmm1
   0x7fffe9123434:   pop    rax
   0x7fffe9123435:   leave  
   0x7fffe9123436:   ret    
   0x7fffe9123437:   vmovdqu xmm3,XMMWORD PTR [rdx+0xb0]
   0x7fffe912343f:   vpshufb xmm3,xmm3,XMMWORD PTR [rip+0xfffffffffffff908]        # 0x7fffe9122d50
   0x7fffe9123448:   cmp    eax,0x34
   0x7fffe912344b:   jne    0x7fffe91234d5
   0x7fffe9123451:   vmovdqu xmm4,XMMWORD PTR [rdx+0xc0]
   0x7fffe9123459:   vpshufb xmm4,xmm4,XMMWORD PTR [rip+0xfffffffffffff8ee]        # 0x7fffe9122d50
   0x7fffe9123462:   movabs rax,0x0
   0x7fffe912346c:   nop
   0x7fffe912346d:   nop
   0x7fffe912346e:   nop
   0x7fffe912346f:   nop
   0x7fffe9123470:   vmovdqu xmm0,XMMWORD PTR [rdi+rax*1]
   0x7fffe9123475:   vmovdqa xmm2,xmm0
   0x7fffe9123479:   vpxor  xmm0,xmm0,xmm5
   0x7fffe912347d:   vaesdec xmm0,xmm0,xmm6
   0x7fffe9123482:   vaesdec xmm0,xmm0,xmm7
   0x7fffe9123487:   vaesdec xmm0,xmm0,xmm8
   0x7fffe912348c:   vaesdec xmm0,xmm0,xmm9
   0x7fffe9123491:   vaesdec xmm0,xmm0,xmm10
   0x7fffe9123496:   vaesdec xmm0,xmm0,xmm11
   0x7fffe912349b:   vaesdec xmm0,xmm0,xmm12
   0x7fffe91234a0:   vaesdec xmm0,xmm0,xmm13
   0x7fffe91234a5:   vaesdec xmm0,xmm0,xmm14
   0x7fffe91234aa:   vaesdec xmm0,xmm0,xmm3
   0x7fffe91234af:   vaesdec xmm0,xmm0,xmm4
   0x7fffe91234b4:   vaesdeclast xmm0,xmm0,xmm15
   0x7fffe91234b9:   vpxor  xmm0,xmm0,xmm1
   0x7fffe91234bd:   vmovdqu XMMWORD PTR [rsi+rax*1],xmm0
   0x7fffe91234c2:   vmovdqa xmm1,xmm2
   0x7fffe91234c6:   add    rax,0x10
   0x7fffe91234ca:   sub    r8,0x10
   0x7fffe91234ce:   jne    0x7fffe9123470
   0x7fffe91234d0:   jmp    0x7fffe9123430
   0x7fffe91234d5:   movabs rax,0x0
   0x7fffe91234df:   nop
   0x7fffe91234e0:   vmovdqu xmm0,XMMWORD PTR [rdi+rax*1]
   0x7fffe91234e5:   vmovdqa xmm2,xmm0
   0x7fffe91234e9:   vpxor  xmm0,xmm0,xmm5
   0x7fffe91234ed:   vaesdec xmm0,xmm0,xmm6
   0x7fffe91234f2:   vaesdec xmm0,xmm0,xmm7
   0x7fffe91234f7:   vaesdec xmm0,xmm0,xmm8
   0x7fffe91234fc:   vaesdec xmm0,xmm0,xmm9
   0x7fffe9123501:   vaesdec xmm0,xmm0,xmm10
   0x7fffe9123506:   vaesdec xmm0,xmm0,xmm11
   0x7fffe912350b:   vaesdec xmm0,xmm0,xmm12
   0x7fffe9123510:   vaesdec xmm0,xmm0,xmm13
   0x7fffe9123515:   vaesdec xmm0,xmm0,xmm14
   0x7fffe912351a:   vaesdec xmm0,xmm0,xmm3
   0x7fffe912351f:   vmovdqu xmm4,XMMWORD PTR [rdx+0xc0]
   0x7fffe9123527:   vpshufb xmm4,xmm4,XMMWORD PTR [rip+0xfffffffffffff820]        # 0x7fffe9122d50
   0x7fffe9123530:   vaesdec xmm0,xmm0,xmm4
   0x7fffe9123535:   vmovdqu xmm4,XMMWORD PTR [rdx+0xd0]
   0x7fffe912353d:   vpshufb xmm4,xmm4,XMMWORD PTR [rip+0xfffffffffffff80a]        # 0x7fffe9122d50
   0x7fffe9123546:   vaesdec xmm0,xmm0,xmm4
   0x7fffe912354b:   vmovdqu xmm4,XMMWORD PTR [rdx+0xe0]
   0x7fffe9123553:   vpshufb xmm4,xmm4,XMMWORD PTR [rip+0xfffffffffffff7f4]        # 0x7fffe9122d50
   0x7fffe912355c:   vaesdec xmm0,xmm0,xmm4
   0x7fffe9123561:   vaesdeclast xmm0,xmm0,xmm15
   0x7fffe9123566:   vpxor  xmm0,xmm0,xmm1
   0x7fffe912356a:   vmovdqu XMMWORD PTR [rsi+rax*1],xmm0
   0x7fffe912356f:   vmovdqa xmm1,xmm2
   0x7fffe9123573:   add    rax,0x10
   0x7fffe9123577:   sub    r8,0x10
   0x7fffe912357b:   jne    0x7fffe91234e0
   0x7fffe9123581:   jmp    0x7fffe9123430
   0x7fffe9123586:   nop
   