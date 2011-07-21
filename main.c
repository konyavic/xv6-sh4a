#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sh4.h"
#include "timer.h"
#include "scif.h"

extern struct cpu *cpu;       // This cpu.
extern struct proc *proc;     // Current proc on this cpu.

static void mpmain(void);
void jkstack(void)  __attribute__((noreturn));
void mainc(void);

unsigned char xv6_stack[STACK_SIZE];

// Bootstrap processor starts running C code here.
int
main(void)
{
  scif_init();     // serial port
  ksegment();      // set up segments
  consoleinit();   // I/O devices & their interrupts
  kinit();         // initialize memory allocator
  jkstack();       // call mainc() on a properly-allocated stack 
}

void
jkstack(void)
{
  char *kstack = kalloc();
  if(!kstack)
    panic("jkstack\n");
  char *top = kstack + PGSIZE;
#ifdef DEBUG
  cprintf("%s: kstack=0x%x\n", __func__, kstack);
  cprintf("%s: top=0x%x\n", __func__, top);
#endif
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
  timer_init();    // uniprocessor timer
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
