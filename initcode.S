# Initial process execs /init.
#include <syscall.h>

.global start
start:
	mov #SYS_exec, r3
	mov.l 4f, r4
	mov.l 6f, r5
	TRAPA #SYS_exec
        nop
        nop

        //mov #SYS_getpid, r3
	//TRAPA #SYS_getpid
	//nop
	//nop

        //mov.l @r15, r14
         
	//mov #SYS_fork, r3
	//TRAPA #SYS_fork
        //nop
        //nop

	//mov #SYS_fork, r3
	//TRAPA #SYS_fork
        //nop
        //nop

	//mov #SYS_fork, r3
	//TRAPA #SYS_fork
        //nop
        //nop
  
	//mov #SYS_sleep, r3
	//mov #1, r4
	//TRAPA #SYS_sleep
        //nop
        //nop	
	//mov #SYS_wait, r3
	//TRAPA #SYS_wait
        //nop
        //nop

	//mov #SYS_exit, r3
	//TRAPA #SYS_exit
        //nop
        //nop

	//mov #SYS_kill, r3
	//mov #4, r4
	//TRAPA #SYS_kill
        //nop
        //nop

	//mov #SYS_sbrk, r3
	//mov.l arg1, r4
	//TRAPA #SYS_sbrk
        //nop
        //nop

	//mov #SYS_sleep, r3
	//mov #1, r4
	//TRAPA #SYS_sleep
        //nop
        //nop
    //mov.l 3f, r1
    //stc  sr, r2
    //and  r1, r2
    //ldc  r2, sr
         
    mov.l 1f, r7
100:
   bra 100b
   nop
.align 4
1: .long 0x12345678
2: arg1:
.long     0x100000
3: .long 0xf0







#  pushl $argv
#  pushl $init
#  pushl $0  // where caller pc would be
#  movl $SYS_exec, %eax
#  int $T_SYSCALL

# for(;;) exit();
#exit:
#  movl $SYS_exit, %eax
#  int $T_SYSCALL
#  jmp exit

# char init[] = "/init\0";
4: .long init
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.align 2
6: .long argv
argv:
  .long init
  .long 0

