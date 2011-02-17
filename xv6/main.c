#include "types.h"
#include "defs.h"
#include "param.h"
#include "xv6.h"
#include "stdio.h"
#include "assert.h"
#include "scif.h"
#include "stdarg.h"
#include "mmu.h"
#include "proc.h"

extern char *skstack;
extern char *kstack;
extern struct cpu *cpu;       // This cpu.
extern struct proc *proc;     // Current proc on this cpu.
//static void bootothers(void);
static void mpmain(void);
void jkstack(void)  __attribute__((noreturn));
void mainc(void);

// Bootstrap processor starts running C code here.
int
main(void)
{
  scif_init();        // serial port
  cprintf("0\n");
  //mpinit();        // collect info about this machine
  //lapicinit(mpbcpu());
  ksegment();      // set up segments
  cprintf("ksegment\n");
  //picinit();       // interrupt controller
  //ioapicinit();    // another interrupt controller
  consoleinit();   // I/O devices & their interrupts
  pgtinit();
  stkinit();
  kinit();
  cprintf("1\n");         // initialize memory allocator
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
    cprintf("top%x\n", top);
  __asm__ __volatile__("mov %0, r15\n\t"
                        : : "r" (top)
                       );
  mainc(); 
  //load_r15(*top);                   
  panic("jkstack");
}

void
mainc(void)
{
  //static struct cpu *bcpu;

  cprintf("\ncpu%d: starting xv6\n\n", cpu->id);
  kvmalloc(); 
  cprintf("1\n");    // initialize the kernel page table
  pinit();
  cprintf("2\n");         // process table
  tvinit();        // trap vectors
  binit();
  cprintf("3\n");         // buffer cache
  fileinit();
  cprintf("4\n");      // file table
  iinit();
  cprintf("5\n");         // inode cache
  ideinit();       // disk
  //if(!ismp)
    tmu_init();   // uniprocessor timer
  userinit();
  cprintf("6");      // first user process
  //bootothers();    // start other processors

  // Finish setting up this processor in mpmain.
  mpmain();
}

// Common CPU setup code.
// Bootstrap CPU comes here from mainc().
// Other CPUs jump here from bootother.S.
static void
mpmain(void)
{
  //if(cpunum() != mpbcpu()) {
    //ksegment();
    //lapicinit(cpunum());
 // }
 cprintf("7");  
  vmenable();        // turn on paging
  cprintf("cpu%d: starting\n", cpu->id);
  //idtinit();       // load idt register
  cpu->booted = 1;
  //print_ttb();
  scheduler();     // start running processes
}

//static void
//bootothers(void)
//{
//  extern uchar _binary_bootother_start[], _binary_bootother_size[];
//  uchar *code;
//  struct cpu *c;
//  char *stack;
//
// // Write bootstrap code to unused memory at 0x7000.  The linker has
//  // placed the start of bootother.S there.
//  code = (uchar *) 0x7000;
//  memmove(code, _binary_bootother_start, (uint)_binary_bootother_size);
//
//  for(c = cpus; c < cpus+ncpu; c++){
//    if(c == cpus+cpunum())  // We've started already.
//      continue;
//
    // Fill in %esp, %eip and start code on cpu.
//    stack = kalloc();
//    *(void**)(code-4) = stack + KSTACKSIZE;
//    *(void**)(code-8) = mpmain;
//    lapicstartap(c->id, (uint)code);
//
    // Wait for cpu to finish mpmain()
//    while(c->booted == 0)
//      ;
//  }
//}

//#define TEST 0x8c100000

//extern unsigned char scif_putc(unsigned char c);
//extern void scif_init(void);
//kernel_main()
//{
//int a=1;;
//SCSCR = 0;
//SCFCR = FCR_RFRST | FCR_TFRST | FCR_TTRG_1 | FCR_RTRG_1;
//SCSMR = 0;
//SCBRR = SCIF_BPS_115200;
//SCFCR = 0;
//SCSCR = SCR_RIE | SCR_TE | SCR_RE;
//while(!(SCFSR & FSR_TDFE));
    //SCFTDR = 'd';
    //SCFSR &= ~FSR_TDFE;
    //return 'c';
//uint a= 2;
//uint * b= (uint *)TEST;
//scif_init();

//xv_mmu_init();
//*b = a;
//printf("TEST = %x\n", *b);

//cprintf("Hello World\n");
//scif_putc('\t');
//while(1){};

//}



