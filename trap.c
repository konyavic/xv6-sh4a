#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sh4.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
//struct gatedesc idt[256];
//extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  initlock(&tickslock, "time");
}

void
trap()
{
  cprintf("unexpected trap\n");
  while(1);
  return;
}
