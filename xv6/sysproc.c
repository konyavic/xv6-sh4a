#include "types.h"
#include "xv6.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"


int
sys_test(void)
{
  //switchkvm();
  cprintf("syscall_test");
  //while(1){;}
  //switchuvm(proc);
  return;
}

int
sys_fork(void)
{
  int pid;
 // switchkvm();
  cprintf("ktf->spc%x,ktf->pr%x\n", ktf->spc,ktf->pr);
  pid = fork();
  //switchuvm(proc);
  return pid;
}


int
sys_exit(void)
{
  //switchkvm();
  exit();
  //switchuvm(proc);
  return 0;  // not reached
}

int
sys_wait(void)
{
  //switchkvm();  
  wait();
  //switchuvm(proc);
  return;
}

int
sys_kill(int pid)
{
  //int pid;
  //switchkvm();
  if(pid < 0)
    {
    switchuvm(proc);
    return -1;
        }
  kill(pid);
  //switchuvm(proc);
  return;
}

int
sys_getpid(void)
{
  cprintf("pid%x\n", proc->pid);
  return proc->pid;
}

int
sys_sbrk(uint n)
{
  int addr;
  //int n;
  //switchkvm();
  if(n < 0)
    {  return -1;
    }
      cprintf("n%x\n", n);
    addr = proc->sz;
  //cprintf("proc->sz%x\n", proc->sz);
  if(growproc(n) < 0)
    {  return -1;
    }
    //addr = proc->sz;
  cprintf("addr%x\n", addr);
  //switchuvm(proc);  
  return addr;
}

int
sys_sleep(uint n)
{
  //int n;
  uint ticks0;
  //switchkvm();
  if(n < 0)
    {  //switchuvm(proc);
    return -1;
    }
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      {  //switchuvm(proc);
    return -1;
    }
    }
      cprintf("cha%x\n", ticks - ticks0);
    sleep(&ticks, &tickslock);
  }

  cprintf("sleep");
  release(&tickslock);
    //switchuvm(proc);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  //switchkvm();
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  //switchuvm(proc);
  return xticks;
}
