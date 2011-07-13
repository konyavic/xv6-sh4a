#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sh.h"

#include "stdio.h"
#include "assert.h"
#include "scif.h"

extern char *skstack;
extern char *kstack;
extern struct cpu *cpu;       // This cpu.
extern struct proc *proc;     // Current proc on this cpu.

static void mpmain(void);
void jkstack(void)  __attribute__((noreturn));
void mainc(void);

// Bootstrap processor starts running C code here.
int
main(void)
{
  scif_init();     // serial port
  ksegment();      // set up segments
  consoleinit();   // I/O devices & their interrupts
  pgtinit();
  stkinit();
  kinit();         // initialize memory allocator
  jkstack();       // call mainc() on a properly-allocated stack 
}

void
jkstack(void)
{
  kstack = stkalloc();
  skstack = stkalloc() + STKSIZE + KOFF - 4;
  if(!kstack)
    panic("jkstack\n");
  char *top = kstack + STKSIZE + KOFF -4;
  asm volatile(
      "mov %0, r15\n"
      :
      : "r"(top)
      );
  mainc(); 
  panic("jkstack");
}

void
mainc(void)
{
  cprintf("\ncpu%d: starting xv6\n\n", cpu->id);
  kvmalloc();      // initialize the kernel page table
  pinit();         // process table
  tvinit();        // trap vectors
  binit();         // buffer cache
  fileinit();      // file table
  iinit();         // inode cache
  ideinit();       // disk
  tmu_init();      // uniprocessor timer
  userinit();      // first user process

  // Finish setting up this processor in mpmain.
  mpmain();
}

// Common CPU setup code.
// Bootstrap CPU comes here from mainc().
// Other CPUs jump here from bootother.S.
static void
mpmain(void)
{
  vmenable();         // turn on paging
  cprintf("cpu%d: starting\n", cpu->id);
  cpu->booted = 1;
  scheduler();        // start running processes
}
