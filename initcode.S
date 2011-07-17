# Initial process execs /init.

#include "syscall.h"

.global start
start:
	mov #SYS_exec, r3
	mov.l 4f, r4
	mov.l 6f, r5
	TRAPA #SYS_exec
  nop
  nop
         
  mov.l 1f, r7
100:
   bra 100b
   nop
.align 4
1: .long 0x12345678
2: arg1:
.long     0x100000
3: .long 0xf0
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
6: 
  .long argv
argv:
  .long init
  .long 0

