.include	"vect.inc"
.include	"syscall.h"
#include    "param.h"

.section .start, "ax"

.global  start

start:
    /* initialize the cache */
    mov.l CCR_REG, r0
    mov.l CCR_INIT, r1
    mov.l r1, @r0

    /* initialize status registers */
    mov.l SR_INIT, r1
    ldc r1, sr

    /* initialize bss */
    mov.l BSS_END, r1
    mov.l BSS_START, r2
    mov #0, r0
1:
    movca.l     r0, @r2
    mov.l       r0, @(4,r2)
    mov.l       r0, @(8,r2)
    mov.l       r0, @(12,r2)
    mov.l       r0, @(16,r2)
    mov.l       r0, @(20,r2)
    mov.l       r0, @(24,r2)
    mov.l       r0, @(28,r2)
    add         #32, r2
    cmp/hs      r1, r2
    bf          1b

    /* initialize stack point */
    mov.l       XV6_STACK, r0
    mov         r0, r15

    /* initialize vector base register */
    mov.l       VBR_INIT, r0
    ldc         r0, vbr

    mov.l       XV6_MAIN, r0
    jmp         @r0
    nop

.align 4
CCR_REG:
    .long 0xff00001c
CCR_INIT:
    .long 0x90b
SR_INIT:
    .long 0x50000000
VBR_INIT:
    .long start
BSS_START:
    .long __bss_start
BSS_END:
    .long __bss_end
XV6_MAIN:
    .long main
XV6_STACK:
    .long xv6_stack+STACK_SIZE

/****************************************************************
*                                                              *
*  general exception handler (VBR + 0x100)                     *
*                                                              *
****************************************************************/
.balign 256, 0, 256
general_exception:
    mov.l 4f, r3				
    !stc  ssr, r0
    !shll r0
    !shll r0
    !bt/s 1f
    mov   r15, r7
    //mov  r15, r0
    //shll r0
    //bt/s 1f
    //nop
    //nop
    //mov.l 9f, r0
    //mov.l @r0, r15

    1:	mov #-1, r4
    mov.l 2f, r1
    mov.l r7, @-r15
    mov.l r4, @-r15
    sts.l   macl, @-r15
    sts.l 	mach, @-r15
    stc.l	gbr, @-r15        
    stc.l   ssr,@-r15
    sts.l	pr, @-r15               ! save ssr
    stc.l   spc,@-r15               ! save spc
    lds	r3, pr        
    !mov.l	r0, @-r15
    mov.l	r14, @-r15
    mov.l	r13, @-r15
    mov.l	r12, @-r15
    mov.l	r11, @-r15
    mov.l	r10, @-r15
    mov.l	r9, @-r15
    mov.l	r8, @-r15

    stc 	sr, r8
    or	r1, r8
    mov.l	3f, r1
    and	r1, r8
    ldc	r8, sr

    mov.l	r7, @-r15
    mov.l	r6, @-r15
    mov.l	r5, @-r15
    mov.l	r4, @-r15
    mov.l	r3, @-r15
    mov.l	r2, @-r15
    mov.l	r1, @-r15
    mov.l	r0, @-r15
    mov.l   13f, r10
    mov.l	r15, @r10	


    mov.l  10f, r8
    mov.l  @r8, r8
    shlr2  r8
    shlr   r8
    mov.l  8f, r9
    add    r8, r9
    mov.l  @r9, r9
    ! set exception code
    jmp @r9
    nop
    nop

    .align	4
    .global __exp_term
    __exp_term:
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
    add	#4, r15
    mov.l	@r15+, r7
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
    mov   	r7, r15
    rte					! return
    nop
    .align 4

    2: .long 0x000000f0 !INTEVT
    3: .long 0xcfffffff !IMASKclr
    4: .long __exp_term
    5: .long 0x30000000 !MDRBBLset

    7:
__exp_IMASK:
.long 0xffffff0f!_INT_Vectors
8: .long vect_table
9: .long skstack
10: .long 0xff000024
11:  .long 0x80000000
12:  .long -0x80000000
13:  .long ktf	

/****************************************************************
 *                                                              *
 *  tlb exception handler (VBR + 0x400)                         *
 *                                                              *
 ****************************************************************/
.balign 1024, 0, 1024
tlb_exception:
mov.l 4f, r3				
!stc  ssr, r0
!shll r0
!shll r0
!bt/s 1f
mov   r15, r7
mov  r15, r0
shll r0
bt/s 1f
nop
nop
mov.l 9f, r0
mov.l @r0, r15

1:	mov #-1, r4
mov.l 2f, r1
mov.l r7, @-r15
mov.l r4, @-r15
sts.l   macl, @-r15
sts.l 	mach, @-r15
stc.l	gbr, @-r15        
stc.l   ssr,@-r15
sts.l	pr, @-r15               ! save ssr
stc.l   spc,@-r15               ! save spc
lds	r3, pr        
!mov.l	r0, @-r15
mov.l	r14, @-r15
mov.l	r13, @-r15
mov.l	r12, @-r15
mov.l	r11, @-r15
mov.l	r10, @-r15
mov.l	r9, @-r15
mov.l	r8, @-r15

stc 	sr, r8
or	r1, r8
mov.l	3f, r1
and	r1, r8
ldc	r8, sr

mov.l	r7, @-r15
mov.l	r6, @-r15
mov.l	r5, @-r15
mov.l	r4, @-r15
mov.l	r3, @-r15
mov.l	r2, @-r15
mov.l	r1, @-r15
mov.l	r0, @-r15
//mov.l   13f, r10
//mov.l	r15, @r10


mov.l  10f, r8
mov.l  @r8, r8
shlr2  r8
shlr   r8
mov.l  8f, r9
add    r8, r9
mov.l  @r9, r9
! set exception code
jmp @r9
nop
nop
.align	4
.global __tlb_term
__tlb_term:
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
add	#4, r15
mov.l	@r15+, r7
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
mov   	r7, r15
rte					! return
nop

.align 4

2: .long 0x000000f0 !INTEVT
3: .long 0xcfffffff !IMASKclr
4: .long __tlb_term
5: .long 0x30000000 !MDRBBLset

7: 
__tlb_IMASK:
.long 0xffffff0f!_INT_Vectors
8: .long vect_table
9: .long skstack
10: .long 0xff000024
11:  .long 0x80000000
12:  .long -0x80000000
13:  .long ktf




/****************************************************************
 *                                                              *
 *  interrupt handler (VBR + 0x600)                             *
 *                                                              *
 ****************************************************************/
.balign 512, 0, 512
intr_entry:
mov.l 4f, r3				
!stc  ssr, r0
!shll r0
!shll r0
!bt/s 1f
mov   r15, r7
//mov  r15, r0
//shll r0
//bt/s 1f
//nop
//nop
//mov.l 9f, r0
//mov.l @r0, r15

1:	mov #-1, r4
mov.l 2f, r1
mov.l r7, @-r15
mov.l r4, @-r15
sts.l   macl, @-r15
sts.l 	mach, @-r15
stc.l	gbr, @-r15        
stc.l   ssr,@-r15
sts.l	pr, @-r15               ! save ssr
stc.l   spc,@-r15               ! save spc
lds	r3, pr        
!mov.l	r0, @-r15
mov.l	r14, @-r15
mov.l	r13, @-r15
mov.l	r12, @-r15
mov.l	r11, @-r15
mov.l	r10, @-r15
mov.l	r9, @-r15
mov.l	r8, @-r15

stc 	sr, r8
or	r1, r8
mov.l	3f, r1
and	r1, r8
ldc	r8, sr

mov.l	r7, @-r15
mov.l	r6, @-r15
mov.l	r5, @-r15
mov.l	r4, @-r15
mov.l	r3, @-r15
mov.l	r2, @-r15
mov.l	r1, @-r15
mov.l	r0, @-r15
//mov.l   13f, r10
//mov.l	r15, @r10


mov.l  10f, r8
mov.l  @r8, r8
shlr2  r8
shlr   r8
mov.l  19f, r9
add    r8, r9
mov.l  @r9, r9
! set exception code
jmp @r9
nop
nop
.align	4

__int_term:
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
add	#4, r15
mov.l	@r15+, r7
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
mov   	r7, r15
rte					! return
nop

.align 4

2: .long 0x000000f0 !INTEVT
3: .long 0xcfffffff !IMASKclr
4: .long __int_term
5: .long 0x30000000 !MDRBBLset

7: 
__INV_IMASK:
.long 0xffffff0f!_INT_Vectors
8: .long vect_table
9: .long skstack
10: .long 0xff000028
13:  .long ktf
19: .long vect_table


.global system_call
system_call:
mov.l 1f, r9
mov.l @r9, r8

mov.l	__SYS_IMASK, r11
stc	sr, r10
and	r11, r10
ldc	r10, sr     

mov	r8, r0
cmp/eq	#0x24, r0
bf/s  good_system_call
nop
nop
nop
mov.l	@r15+, r0
mov.l	@r15+, r1
mov.l	@r15+, r2
mov.l	@r15+, r3
mov.l	@r15+, r4
mov.l	@r15+, r5
mov.l	@r15+, r6
mov.l	@r15+, r7

stc	sr, r8
mov.l 	14f, r9
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
add	#4, r15
mov.l	@r15+, r7

mov.l 4f, r3				
!stc  ssr, r0
!shll r0
!shll r0
!bt/s 1f
mov   r15, r7
mov  r15, r0
shll r0
bt/s 8f
nop
nop
mov.l 9f, r0
mov.l @r0, r15

8:	mov #-1, r4
mov.l 2f, r1
mov.l r7, @-r15
mov.l r4, @-r15
sts.l   macl, @-r15
sts.l 	mach, @-r15
stc.l	gbr, @-r15        
stc.l   ssr,@-r15
sts.l	pr, @-r15               ! save ssr
stc.l   spc,@-r15               ! save spc
lds	r3, pr        
!mov.l	r0, @-r15
mov.l	r14, @-r15
mov.l	r13, @-r15
mov.l	r12, @-r15
mov.l	r11, @-r15
mov.l	r10, @-r15
mov.l	r9, @-r15
mov.l	r8, @-r15

stc 	sr, r8
or	r1, r8
mov.l	15f, r1
and	r1, r8
ldc	r8, sr

mov.l	r7, @-r15
mov.l	r6, @-r15
mov.l	r5, @-r15
mov.l	r4, @-r15
mov.l	r3, @-r15
mov.l	r2, @-r15
mov.l	r1, @-r15
mov.l	r0, @-r15
mov.l   16f, r10
mov.l	r15, @r10

good_system_call:
mov.l 1f, r9
mov.l @r9, r9
!shll2 r9
mov.l 3f, r8
add   r8, r9
add   #-4, r9
mov.l @r9, r8
jsr   @r8
nop
nop

!mov	r15, r1
!mov.l	r0, @r1 


system_return:	
stc	sr, r1
mov.l	13f, r2
or	r2, r1
ldc	r1, sr


add	#4, r15
mov.l	@r15+, r1
mov.l	@r15+, r2
mov.l	@r15+, r3
mov.l	@r15+, r4
mov.l	@r15+, r5
mov.l	@r15+, r6
mov.l	@r15+, r7

stc	sr, r8
mov.l 	14f, r9
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
add	#4, r15
mov.l	@r15+, r7
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
mov   	r7, r15
rte					! return
nop


.align 4
1:	.long 	0xff000020
2: 	.long 0x000000f0 !INTEVT
3:	.long sys_call_table 
4: .long system_return
5: .long 0x20000000 !MDRBBLset

7: 
__SYS_IMASK:
.long 0xffffff0f!_INT_Vectors
9: .long skstack
11:  .long 0x80000000
12:  .long -0x80000000
13:  .long 0xf0
14:  .long 0x30000000
15: .long 0xcfffffff !IMASKclr
16:  .long ktf





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









