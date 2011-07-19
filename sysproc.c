#include "types.h"
#include "sh4a.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill()
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

#include "defs.h"

int
sys_sleep(uint n)
{
  cprintf("%s: not working yet\n", __func__);
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
    sleep(&ticks, &tickslock);
  }

  release(&tickslock);
    //switchuvm(proc);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  cprintf("%s: not working yet\n", __func__);
  uint xticks;
  //switchkvm();
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  //switchuvm(proc);
  return xticks;
}
