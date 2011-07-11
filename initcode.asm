
initcode.o:     file format elf32-sh-linux


Disassembly of section .text:

00000000 <start>:
   0:	09 e3       	mov	#9,r3
   2:	0a d4       	mov.l	2c <arg1+0x8>,r4	! 30 <init>
   4:	0c d5       	mov.l	38 <argv>,r5	! 30 <init>
   6:	16 c3       	trapa	#22
   8:	09 00       	nop	
   a:	09 00       	nop	
   c:	04 d7       	mov.l	20 <start+0x20>,r7	! 12345678
   e:	fe af       	bra	e <start+0xe>
  10:	09 00       	nop	
  12:	09 00       	nop	
  14:	09 00       	nop	
  16:	09 00       	nop	
  18:	09 00       	nop	
  1a:	09 00       	nop	
  1c:	09 00       	nop	
  1e:	09 00       	nop	
  20:	78 56       	mov.l	@(32,r7),r6
  22:	34 12       	mov.l	r3,@(16,r2)

00000024 <arg1>:
  24:	00 00       	.word 0x0000
  26:	10 00       	.word 0x0010
  28:	f0 00       	.word 0x00f0
  2a:	00 00       	.word 0x0000
  2c:	30 00       	.word 0x0030
	...

00000030 <init>:
  30:	2f 69       	exts.w	r2,r9
  32:	6e 69       	exts.b	r6,r9
  34:	74 00       	mov.b	r7,@(r0,r0)
	...

00000038 <argv>:
  38:	30 00       	.word 0x0030
  3a:	00 00       	.word 0x0000
  3c:	00 00       	.word 0x0000
	...
