#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "sh4a.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern struct context *new_context, *old_context;
//extern struct context new_context, old_context;
extern struct trapframe *ktf;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

static int
mappages(pde_t *pgdir, void *la, uint size, uint pa, int perm)
{
  char *a = PGROUNDDOWN(la);
  char *last = PGROUNDDOWN(la + size - 1);

  while(1){
    pte_t *pte = &pgdir[PTX(a)];
    //if(pte == 0)
    //  return 0;
    //if(*pte & PTE_P)
    //  panic("remap");
    *pte = pa | perm | PTE_P | PTE_D | PTE_PS;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
    //a+= PGSIZE;
  //while(last <= 0x400000)
  //{
  //pte_t *pte = &pgdir[PTX(a)];
  //*pte &= ~PTE_P;
  //last += PGSIZE;
  //}
  return 1;
}

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];
  
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->r15+2, pc);
    }
  }
}


// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and return it.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto found;
  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;
  release(&ptable.lock);

  // Allocate kernel stack if possible.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;
  
  // Leave room for trap frame.
  //sp -= sizeof *p->tf;
  //p->tf = (struct trapframe*)sp;
  
  // Set up new context to start executing at forkret,
  // which returns to trapret (see below).
  //sp -= 4;
  //*(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->pr = (uint)forkret;
  p->context->sr = 0x40000000;
  return p;
}

// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];
  
  p = allocproc();
  initproc = p;
  if(!(p->pgdir = setupkvm()))
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  mappages(p->pgdir, PADDR(p->context) , PGSIZE, PADDR(p->context), PTE_W|PTE_U|PTE_PWT|PTE_P);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));

  //p->context->spc = 0x0;
  //p->context->sgr = p->kstack;
  //p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  //p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  //p->tf->es = p->tf->ds;
  //p->tf->ss = p->tf->ds;
  //p->tf->eflags = FL_IF;
  //p->tf->esp = PGSIZE;
  p->context->ssr = 0x00000000;
  p->context->spc = 0;  // beginning of initcode.S
  //p->context->sgr = PGSIZE; // forbidden in r2dplus
  p->context->r15 = PGSIZE;

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  p->state = RUNNABLE;

#ifdef DEBUG
  cprintf("%s:\n", __func__);
  dump_proc(p);
#endif
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz = proc->sz;
  if(n > 0){
    if(!(sz = allocuvm(proc->pgdir, sz, sz + n)))
      return -1;
  } else if(n < 0){
    if(!(sz = deallocuvm(proc->pgdir, sz, sz + n)))
      return -1;
  }
  proc->sz = sz;
  //switchuvm(proc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;

  // Allocate process.
  //release(&ptable.lock);
  if((np = allocproc()) == 0)
    return -1;

  // Copy process state from p.
  if(!(np->pgdir = copyuvm(proc->pgdir, proc->sz))){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  mappages(np->pgdir, PADDR(np->context), PGSIZE, PADDR(np->context), PTE_W|PTE_U|PTE_PWT|PTE_P);
  np->sz = proc->sz;
  np->parent = proc;
  *np->tf = *ktf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->r0 = 0;
  //np->context->r0 = 0;

  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      np->ofile[i] = filedup(proc->ofile[i]);
  np->cwd = idup(proc->cwd);
 
  pid = np->pid;
  np->state = RUNNABLE;
  safestrcpy(np->name, proc->name, sizeof(proc->name));
  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *p;
  int fd;

  if(proc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(proc->ofile[fd]){
      fileclose(proc->ofile[fd]);
      proc->ofile[fd] = 0;
    }
  }

  iput(proc->cwd);
  proc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(proc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == proc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  proc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for zombie children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != proc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->state = UNUSED;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;

  for(;;){
#ifdef DEBUG
    cprintf("%s: for loop\n", __func__);
#endif
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;
#ifdef DEBUG
      cprintf("%s: found pid=%d\n", __func__, p->pid);
#endif

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      proc = p;
      switchuvm(p);
      p->state = RUNNING;
#ifdef DEBUG
      cprintf("%s: before swtch\n", __func__);
      cprintf("%s: proc->context:\n", __func__);
      debug_context(proc->context);
#endif
      swtch(&cpu->scheduler, proc->context);
#ifdef DEBUG
      cprintf("%s: after swtch\n", __func__);
#endif
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      proc = 0;
    }
    release(&ptable.lock);
    while(1); // XXX

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state.
void
sched(void)
{
  int intena;

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(cpu->ncli != 1)
    panic("sched locks");
  if(proc->state == RUNNING)
    panic("sched running");
  if(read_sr()&FL_IF)
    panic("sched interruptible");
  intena = cpu->intena;
  cstack = proc->kstack + PGSIZE - 4;
  cstack = cstack - sizeof *proc->tf - 4;
      new_context= cpu->scheduler;
      old_context= &proc->context;
  swtch(old_context, new_context);
  cpu->intena = intena;
  return;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  proc->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
#ifdef DEBUG
  cprintf("%s:\n", __func__);
#endif
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  if(proc == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }

  // Go to sleep.
  proc->chan = chan;
  proc->state = SLEEPING;
  sched();

  // Tidy up.
  proc->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

// Wake up all processes sleeping on chan.
// The ptable lock must be held.
void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  //acquire(&ptable.lock);
  wakeup1(chan);
  //release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}


void dump_proc(struct proc* p) 
{
  cprintf("--- %s start ---\n", __func__);
  cprintf(
      "name=%s, "
      "sz=%d, "
      "pgdir=0x%x, "
      "kstack=0x%x, "
      "state=%d, "
      "pid=%d, "
      "\n",
      p->name,
      p->sz,
      p->pgdir,
      p->kstack,
      p->state,
      p->pid
      );
  dump_pde(p->pgdir, 0, 2);
  dump_pgd(p->pgdir, 2);
  cprintf("--- %s end ---\n", __func__);
}

void debug_context(struct context *cx)
{
  cprintf("--- %s start ---\n", __func__);
  cprintf("r0: 0x%x r1: 0x%x r2: 0x%x r3: 0x%x\n",
      cx->r0,
      cx->r1,
      cx->r2,
      cx->r3);
  cprintf("r4: 0x%x r5: 0x%x r6: 0x%x r7: 0x%x\n",
      cx->r4,
      cx->r5,
      cx->r6,
      cx->r7);
  cprintf("r8: 0x%x r9: 0x%x r10: 0x%x r11: 0x%x\n",
      cx->r8,
      cx->r9,
      cx->r10,
      cx->r11);
  cprintf("r12: 0x%x r13: 0x%x r14: 0x%x r15: 0x%x\n",
      cx->r12,
      cx->r13,
      cx->r14,
      cx->r15);
  cprintf("sr: 0x%x pc: 0x%x\n",
      cx->sr,
      cx->pc);
  cprintf("ssr: 0x%x spc: 0x%x sgr: 0x%x\n",
      cx->ssr,
      cx->spc,
      cx->sgr);
  cprintf("gbr: 0x%x mach: 0x%x macl: 0x%x pr: 0x%x\n",
      cx->gbr,
      cx->mach,
      cx->macl,
      cx->pr);
  cprintf("r0: 0x%x r1: 0x%x r2: 0x%x r3: 0x%x\n",
      cx->r0_bank,
      cx->r1_bank,
      cx->r2_bank,
      cx->r3_bank);
  cprintf("r4: 0x%x r5: 0x%x r6: 0x%x r7: 0x%x\n",
      cx->r4_bank,
      cx->r5_bank,
      cx->r6_bank,
      cx->r7_bank);
  cprintf("dbr: 0x%x\n",
      cx->dbr);
  cprintf("--- %s end ---\n", __func__);
}

char debug_str[] = "debug: %x\n";
