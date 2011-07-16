# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save current register context in old
# and then load register context from new.

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

  /* return */
  add #4, r14
  mov r14, r15
  lds.l @r15+, pr
  mov.l @r15+, r14
  rts
  nop

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
