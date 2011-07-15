#include "types.h"
#include "sh4a.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"


int
sys_test(void)
{
  //switchkvm();
  //while(1){;}
  //switchuvm(proc);
  return;
}

int
sys_fork(void)
{
  int pid;
 // switchkvm();
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
    addr = proc->sz;
  if(growproc(n) < 0)
    {  return -1;
    }
    //addr = proc->sz;
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
  uint xticks;
  //switchkvm();
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  //switchuvm(proc);
  return xticks;
}
