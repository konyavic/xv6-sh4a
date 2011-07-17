# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save current register context in old
# and then load register context from new.

.macro debug  ! r5 = arg
  mov.l _debug_str, r4
  mov.l _cprintf, r1
  jsr @r1
  nop
.endm

.globl swtch
swtch:
  # swtch(&cpu->scheduler, proc->context);
  # r4 = &cpu->scheduler
  # r5 = proc->context

  /* callee-save */
  mov.l r14, @-r15
  sts.l pr, @-r15
  add #-4, r15
  mov r15, r14

  /* start of store */
  /* system registers */
  mov.l r14,  @-r15 ! pc
  stc.l sr,   @-r15
  mov.l r14,  @-r15 ! r15
  sts.l pr,   @-r15
  sts.l macl, @-r15
  sts.l mach, @-r15
  stc.l gbr,  @-r15
  /* general purpose registers (bank1) */
  stc.l r7_bank,  @-r15
  stc.l r6_bank,  @-r15
  stc.l r5_bank,  @-r15
  stc.l r4_bank,  @-r15
  stc.l r3_bank,  @-r15
  stc.l r2_bank,  @-r15
  stc.l r1_bank,  @-r15
  stc.l r0_bank,  @-r15
  /* control registers */
  stc.l dbr,  @-r15
  stc.l sgr,  @-r15
  stc.l spc,  @-r15
  stc.l ssr,  @-r15
  /* not-banked registers */
  mov.l r14,  @-r15
  mov.l r13,  @-r15
  mov.l r12,  @-r15
  mov.l r11,  @-r15
  mov.l r10,  @-r15
  mov.l r9,   @-r15
  mov.l r8,   @-r15
  /* general purpose registers (bank0) */
  mov.l r7,   @-r15
  mov.l r6,   @-r15
  mov.l r5,   @-r15
  mov.l r4,   @-r15
  mov.l r3,   @-r15
  mov.l r2,   @-r15
  mov.l r1,   @-r15
  mov.l r0,   @-r15
  /* return value */
  mov.l r15, @r4
  /* end of store */

#ifdef DEBUG
  /* debug */
  mov r15, r5
  debug

  mov r14, r5
  debug

  mov.l @r15, r5
  debug

  mov r15, r5
  add #-132, r5
  mov.l @r5, r5
  debug

  mov r15, r4
  mov.l _debug_context, r1
  jsr @r1
  nop
#endif

  /* start of load */
  /* general purpose registers (bank0) */
  mov.l @r5+, r0
  mov.l @r5+, r1
  mov.l @r5+, r2
  mov.l @r5+, r3
  mov.l @r5+, r4
  ! mov.l @r5+, r5
  add #4, r5
  mov.l @r5+, r6
  mov.l @r5+, r7
  /* not-banked registers */
  mov.l @r5+, r8
  mov.l @r5+, r9
  mov.l @r5+, r10
  mov.l @r5+, r11
  mov.l @r5+, r12
  mov.l @r5+, r13
  ! mov.l @r5+, r14
  add #4, r5
  /* control registers */
  ldc.l @r5+, ssr
  ldc.l @r5+, spc
  ! ldc.l @r5+, sgr ! forbidden in r2dplus
  add #4, r5
  ! ldc.l @r5+, dbr
  add #4, r5
  /* general purpose registers (bank1) */
  ldc.l @r5+, r0_bank
  ldc.l @r5+, r1_bank
  ldc.l @r5+, r2_bank
  ldc.l @r5+, r3_bank
  ldc.l @r5+, r4_bank
  ldc.l @r5+, r5_bank
  ldc.l @r5+, r6_bank
  ldc.l @r5+, r7_bank
  /* system registers */
  ldc.l @r5+, gbr
  lds.l @r5+, mach
  lds.l @r5+, macl
  lds.l @r5+, pr
  ! mov.l @r5+, r15 ! causing TLB miss
  mov.l _stack, r15 ! causing TLB miss
  ! add #4, r5
#if 0
loop:
  bra loop
  nop
#endif
  ! ldc.l @r5+, sr
  add #4, r5
  ! ignored pc
  add #4, r5
  /* end of load */

#ifdef DEBUG
  mov r15, r5
  debug
  stc spc, r5
  debug
  stc ssr, r5
  debug
#endif

  /* return */
  ! add #4, r14
  ! mov r14, r15
  ! ! lds.l @r15+, pr
  ! add #4 r15
  ! ! mov.l @r15+, r14
  ! add #4 r15
  ! rts
  ! nop
  rte
  nop

  /*-------------------------------------*/

  add #-36, r15
  stc.l dbr, @-r15
  sts.l macl, @-r15
  sts.l mach, @-r15
  add   #-4, r15
  sts.l pr, @-r15
  stc.l sr, @-r15
/* general purpose registers (bank0) */
  mov.l r14, @-r15
  mov.l r13, @-r15
  mov.l r12, @-r15
  mov.l r11, @-r15
  mov.l r10, @-r15
  mov.l r9, @-r15
  mov.l r8, @-r15
  mov.l r7, @-r15
  mov.l r6, @-r15
  mov.l r5, @-r15
  mov.l r4, @-r15
  mov.l r3, @-r15
  mov.l r2, @-r15
  mov.l r1, @-r15
  mov.l r0, @-r15 

6:
  mov   r4, r1
  mov.l r15, @r1


	/* general purpose registers (bank0) */
7:
  mov.l @r5+, r0
  mov.l @r5+, r1
  mov.l @r5+, r2
  mov.l @r5+, r3
  mov.l @r5+, r4
  add   #4, r5
  mov.l @r5+, r6
  mov.l @r5+, r7
  mov.l @r5+, r8
  mov.l @r5+, r9
  mov.l @r5+, r10
  mov.l @r5+, r11
  mov.l @r5+, r12
  mov.l @r5+, r13
  mov.l @r5+, r14
	/* control registers */
  ldc.l @r5+, ssr
  ldc.l @r5+, spc
  add   #4, r5
  lds.l @r5+, mach
  lds.l @r5+, macl
  ldc.l @r5+, dbr
  add #36, r5
  lds.l @r5+, pr
  add #4, r5
  mov   r5, r15
  mov   #0, r5
  rte
  nop
  nop

.align 4
1:	.long 0x80000000
3:	.long -(0x80000000)
4:	.long forkret
5:	.long cstack

_debug_context:
  .long debug_context
_cprintf:
  .long cprintf
_debug_str:
  .long debug_str

_stack:
  .long 0x1000


.global trapret
trapret:	
		//stc	sr, r1
	//mov	#0xf0, r2
        //or	r2, r1
	//ldc	r1, sr

				mov.l	13f, r1
				mov.l	@r1, r15 
				mov.l	@r15+, r0
				mov.l	@r15+, r1
				mov.l	@r15+, r2
				mov.l	@r15+, r3
				mov.l	@r15+, r4
				mov.l	@r15+, r5
				mov.l	@r15+, r6
				mov.l	@r15+, r7

				stc	sr, r8
				mov.l 	5f, r9
				or	r9, r8
				ldc	r8, sr
				
				mov.l	@r15+, r8
				mov.l	@r15+, r9
				mov.l	@r15+, r10
				mov.l	@r15+, r11
				mov.l	@r15+, r12
				mov.l	@r15+, r13
				mov.l	@r15+, r14
				!mov.l	@r15+, r4
				ldc.l	@r15+, spc
				lds.l	@r15+, pr
				mov.l	@r15+, r3
				ldc.l 	@r15+, gbr
				lds.l	@r15+, mach
				lds.l	@r15+, macl
				add	#8, r15
				!mov.l	@r15+, r7
				mov	r3, r2
				mov.l	7f, r1
				and	r1, r2

				//mov	r3, r1
				//shlr2	r1
				//mov	#0x3c, r3
				//and	r3, r1
				//cmp/eq	r3, r1
				//bt/s	6f
				//mov	r6, r1

			//6:	or	r1, r2
				ldc	r2, ssr
				!mov.l	@r15+, r2
				!mov	r4, r15
				!com/eq  r
				!mov.l   12f, r0
				!mov   	r7, r15
				rte					! return
				nop
	

.align 4
1:	.long 	0xff000020
2:	.long 23
3:	.long sys_call_table 
5: .long 0x30000000 !MDRBBLset

7: 
__SYS_IMASK:
	.long 0xffffff0f!_INT_Vectors
11:  .long 0x80000000
12:  .long -0x80000000
13:  .long etf
#  movl 4(%esp), %eax
#  movl 8(%esp), %edx

#  # Save old callee-save registers
#  pushl %ebp
#  pushl %ebx
#  pushl %esi
#  pushl %edi

  # Switch stacks
#  movl %esp, (%eax)
#  movl %edx, %esp

  # Load new callee-save registers
#  popl %edi
#  popl %esi
#  popl %ebx
#  popl %ebp
#  ret

.global sys_call_table
sys_call_table:
.long sys_fork	/* 0  -  old "setup()" system call*/
.long sys_exit
.long sys_wait
.long sys_pipe
.long sys_write
.long sys_read		/* 5 */
.long sys_close
.long sys_kill
.long sys_exec
.long sys_open
.long sys_mknod		/* 10 */
.long sys_unlink
.long sys_fstat
.long sys_link
.long sys_mkdir
.long sys_chdir		/* 15 */
.long sys_dup
.long sys_getpid	/* old break syscall holder */
.long sys_sbrk
.long sys_sleep
.long sys_uptime		/* 20 */
.long sys_test
