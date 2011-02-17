// Segments in proc->gdt.
// Also known to bootasm.S and trapasm.S
//#define SEG_KCODE 1  // kernel code
//#define SEG_KDATA 2  // kernel data+stack
//#define SEG_KCPU  3  // kernel per-cpu data
//#define SEG_UCODE 4  // user code
//#define SEG_UDATA 5  // user data+stack
//#define SEG_TSS   6  // this process's task state
//#define NSEGS     7
//void ksegment(void);
// Per-CPU state
struct cpu {
  uchar id;                    // Local APIC ID; index into cpus[] below
  struct context *scheduler;   // Switch here to enter scheduler
  //struct taskstate ts;         // Used by x86 to find stack for interrupt
  //struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint booted;        // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  
  // Cpu-local storage variables; see below
  struct cpu *cpu;
  struct proc *proc;
};

struct context *new_context, *old_context;
struct trapframe *ktf;
struct trapframe *etf;
uint *midtf;
struct cpu cpus[1];
struct cpu *bcpu;
uint *glock;
char *kstack, *cstack;
char *skstack;
extern pde_t *intpgdir;
extern uint *midtf;
extern char *cstack;
//char firfs[10] = "/init\0";
//extern char inits[];

//extern int ncpu;

// Per-CPU variables, holding pointers to the
// current cpu and to the current process.
// The asm suffix tells gcc to use "%gs:0" to refer to cpu
// and "%gs:4" to refer to proc.  ksegment sets up the
// %gs segment register so that %gs refers to the memory
// holding those two variables in the local cpu's struct cpu.
// This is similar to how thread-local variables are implemented
// in thread libraries such as Linux pthreads.
struct cpu *cpu;       // This cpu.
struct proc *proc;     // Current proc on this cpu.

// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {

    _reg_gp r0;
    _reg_gp r1;
    _reg_gp r2;
    _reg_gp r3;
    _reg_gp r4;
    _reg_gp r5;
    _reg_gp r6;
    _reg_gp r7;
    /* not-banked registers */
    _reg_gp r8;
    _reg_gp r9;
    _reg_gp r10;
    _reg_gp r11;
    _reg_gp r12;
    _reg_gp r13;
    _reg_gp r14;
    /* control registers */
    _reg_gp ssr;
    _reg_gp spc;
    _reg_gp sgr;
    _reg_gp mach;
    _reg_gp macl;
    _reg_vt dbr;
    
    /* general purpose registers (bank1) */
    _reg_gp r0_bank;
    _reg_gp r1_bank;
    _reg_gp r2_bank;
    _reg_gp r3_bank;
    _reg_gp r4_bank;
    _reg_gp r5_bank;
    _reg_gp r6_bank;
    _reg_gp r7_bank;
    /* system registers */
    _reg_gp gbr;
    _reg_gp pr;
    _reg_gp r15;
    //_reg_gp sr;
    /* dummy registers and vbr */
    //_reg_gp pc;
    //_reg_vt vbr600;
   // _reg_vt vbr400;
   // _reg_vt vbr100;
    //_reg_vt vbr;
    //_reg_gp intevt;  
//uint edi;
  //uint esi;
  //uint ebx;
  //uint ebp;
  //uint eip;
};

struct trapframe {

    _reg_gp r0;
    _reg_gp r1;
    _reg_gp r2;
    _reg_gp r3;
    _reg_gp r4;
    _reg_gp r5;
    _reg_gp r6;
    _reg_gp r7;
    /* not-banked registers */
    _reg_gp r8;
    _reg_gp r9;
    _reg_gp r10;
    _reg_gp r11;
    _reg_gp r12;
    _reg_gp r13;
    _reg_gp r14;
    /* control registers */
    _reg_gp spc;
    _reg_gp pr;
    _reg_gp ssr;
    _reg_gp gbr;
    _reg_gp mach;
    _reg_gp macl;
   /* general purpose registers (bank1) */
    _reg_gp r4_rank;
    _reg_gp r7_rank;

};
//struct trapframe {
//    _reg_gp r7_rank;
//    _reg_gp r4_rank;
//    _reg_gp macl;
//    _reg_gp mach;
//    _reg_gp gbr;
//    _reg_gp ssr;
//    _reg_gp pr;
//    _reg_gp spc;
//    _reg_gp r14;
//    _reg_gp r13;
//    _reg_gp r12;
//    _reg_gp r11;
//    _reg_gp r10;
//    _reg_gp r9;
//    _reg_gp r8;
//    _reg_gp r7;
//    _reg_gp r6;
//    _reg_gp r5;
//    _reg_gp r4;
//    _reg_gp r3;
//    _reg_gp r2;
//    _reg_gp r1;
//    _reg_gp r0;
//
//};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc{
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Linear address of proc's pgdir
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  volatile int pid;            // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // Switch here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
