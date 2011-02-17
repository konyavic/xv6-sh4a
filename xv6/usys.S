#include "syscall.h"
//#include "traps.h"


  .global mknod
  mknod: 
    mov #SYS_mknod, r3
    trapa #SYS_mknod
    nop 
    nop
    rts
    nop
    nop
 
  .global open
  open: 
    mov #SYS_open, r3
    trapa #SYS_open
    nop
    nop
    rts
    nop
    nop
 
  .global exit
  exit: 
    mov #SYS_exit, r3
    trapa #SYS_exit
    nop 
    nop
    rts
    nop
    nop           

  .global wait
  wait: 
    mov #SYS_wait, r3
    trapa #SYS_wait
    nop 
    nop
    rts
    nop
    nop

  .global pipe
  pipe: 
    mov #SYS_pipe, r3
    trapa #SYS_pipe
    nop 
    nop
    rts
    nop
    nop

  .global read
  read: 
    mov #SYS_read, r3
    trapa #SYS_read
    nop 
    nop
    rts
    nop
    nop

  .global write
  write: 
    mov #SYS_write, r3
    trapa #SYS_write
    nop 
    nop
    rts
    nop
    nop

  .global close
  close: 
    mov #SYS_close, r3
    trapa #SYS_close
    nop 
    nop
    rts
    nop
    nop

  .global kill
  kill: 
    mov #SYS_kill, r3
    trapa #SYS_kill
    nop 
    nop
    rts
    nop
    nop

  .global exec
  exec: 
    mov #SYS_exec, r3
    trapa #SYS_exec
    nop 
    nop
    rts
    nop
    nop


  .global fstat
  fstat: 
    mov #SYS_fstat, r3
    trapa #SYS_fstat
    nop 
    nop
    rts
    nop
    nop

  .global link
  link: 
    mov #SYS_link, r3
    trapa #SYS_link
    nop 
    nop
    rts
    nop
    nop

  .global mkdir
  mkdir: 
    mov #SYS_mkdir, r3
    trapa #SYS_mkdir
    nop 
    nop
    rts
    nop
    nop

  .global chdir
  chdir: 
    mov #SYS_chdir, r3
    trapa #SYS_chdir
    nop 
    nop
    rts
    nop
    nop

  .global dup
  dup: 
    mov #SYS_dup, r3
    trapa #SYS_dup
    nop 
    nop
    rts
    nop
    nop

  .global getpid
  getpid: 
    mov #SYS_getpid, r3
    trapa #SYS_getpid
    nop 
    nop
    rts
    nop
    nop

  .global sbrk
  sbrk: 
    mov #SYS_sbrk, r3
    trapa #SYS_sbrk
    nop 
    nop
    rts
    nop
    nop

  .global sleep
  sleep: 
    mov #SYS_sleep, r3
    trapa #SYS_sleep
    nop 
    nop
    rts
    nop
    nop

  .global uptime
  uptime: 
    mov #SYS_uptime, r3
    trapa #SYS_uptime
    nop 
    nop
    rts
    nop
    nop

  .global fork
  fork: 
    mov #SYS_fork, r3
    trapa #SYS_fork
    nop 
    nop
    rts
    nop
    nop

  .global test
  test: 
    mov #SYS_test, r3
    trapa #SYS_test
    nop 
    nop
    rts
    nop
    nop

  .global unlink
  unlink: 
    mov #SYS_unlink, r3
    trapa #SYS_unlink
    nop 
    nop
    rts
    nop
    nop

//SYSCALL(fork)
//SYSCALL(exit)
//SYSCALL(wait)
//SYSCALL(pipe)
//SYSCALL(read)
//SYSCALL(write)
//SYSCALL(close)
//SYSCALL(kill)
//SYSCALL(exec)
//SYSCALL(open)
//SYSCALL(mknod)
//SYSCALL(unlink)
//SYSCALL(fstat)
//SYSCALL(link)
//SYSCALL(mkdir)
//SYSCALL(chdir)
//SYSCALL(dup)
//SYSCALL(getpid)
//SYSCALL(sbrk)
//SYSCALL(sleep)
//SYSCALL(uptime)
