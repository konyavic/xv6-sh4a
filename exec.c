#include "types.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "xv6.h"
#include "elf.h"


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

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, argc, arglen, len, off;
  uint sz, spbottom, argp, esp;
  char *sp;
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir,*mem;
  char *eargv[20];

  pgdir = 0;
  sz = 0;
  memset(eargv, 0, sizeof(eargv));
  for (i=0; argv[i]; i++)
  {
	len = strlen(argv[i]);	
	eargv[i] = argv[i];
	memmove(eargv[i], argv[i], len);
  }
  
  //**eargv = **argv;
  //*eargv = *argv;

  if((ip = namei(path)) == 0)
    return -1;
  ilock(ip);
  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) < sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;
  if(!(pgdir = pgtalloc()))
    goto bad;

  // Load program into memory.
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if(!(sz = allocuvm(pgdir, sz, ph.va + ph.memsz)))
      goto bad;
    if(!loaduvm(pgdir, (char *)ph.va, ip, ph.offset, ph.filesz))
      goto bad;
  }
  iunlockput(ip);
  // Allocate and initialize stack at sz
  sz = spbottom = PGROUNDUP(sz);
  //if(!(sz = allocuvm(pgdir, sz, sz + PGSIZE)))
  //  goto bad;
  //mem = uva2ka(pgdir, (char *)spbottom);
  //proc->kstack = stkalloc();
  //proc->kstack = mem;
  //sp=0;
  sp = proc->kstack + STKSIZE - 4;
   mappages(pgdir, PADDR(proc->kstack) , PGSIZE, PADDR(proc->kstack), PTE_W|PTE_U|PTE_PWT|PTE_P);
  arglen = 0;
  for(argc=0; eargv[argc]; argc++)
    {
   arglen += strlen(*argv) + 1;
  }
  arglen = (arglen+3) & ~3;
//arglen += strlen(*eargv) + 1;
  //for(argc=0; argv[argc]; argc++)
    //{
   //arglen += strlen(argv[argc]) + 1;
 // }
  //arglen = (arglen+3) & ~3;
  
  //sp = sz;
  //sp = proc->tf->r7_rank;
//  argp = sp - arglen - 100;
  //mem = sp - 80;
    //memset(eargv, 0, sizeof(eargv));
  //for(i=0;; i++){
    //if(i >= NELEM(eargv))
      //return -1;
  //if (argv[i] != 0)
  //{
  //*mem = argv[i];
  //sp = sp + 1;}
  //else{
  //*mem = 0;
  //break;
 // }
//}
  sp -= sizeof *proc->tf;
  //proc->tf = (struct trapframe*)sp;
  
  // Set up new context to start executing at forkret,
  // which returns to trapret (see below).
  sp -= 4;
  //*(uint*)sp = (uint)trapret;

   sp -= sizeof *proc->context;
   //proc->context = (struct context*)sp;
   argp = sp - arglen - 2048;
   //argp= argp + 1;
  memset(argp, 0, 1024);
  sp-=1024;
  // argp = sp − arglen − 4*(argc+1);
  // Copy argv strings and pointers to stack.
  //*(uint*)(argp+4*argc) = 0;  // argv[argc]
  for(i=argc-1; i>=0; i--){
    len = strlen(eargv[i]) + 1;
    sp -= len;
    memmove(sp, eargv[i], len);
    *(uint*)(argp + 4*i) = sp;  // argv[i]
  }
  //len = strlen(*argv+1) + 1;
  //sp -= len;
  //memmove(sp, *argv+1, len);
  //*(uint*)(argp + 4) = sp;
  //len = strlen(*argv) + 1;
  
  //sp -= len;
  //memmove(sp, *argv, len); 
  //*(uint*)argp = sp;
  
  ktf->r5 = argp;
  ktf->r4 = argc;
  // Stack frame for main(argc, argv), below arguments.
  //sp = argp;
  //sp -= 4;
  //*(uint*)(mem+sp-spbottom) = argp;
  //sp -= 4;
  //*(uint*)(mem+sp-spbottom) = argc;
  //sp -= 4;
  //*(uint*)(mem+sp-spbottom) = 0xffffffff;   // fake return pc

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(proc->name, last, sizeof(proc->name));
  // Commit to the user image.
  oldpgdir = proc->pgdir;
    //sp -= sizeof *p->context;
  //proc->kstack = mem;
  //p->context = (struct context*)sp;
  //memset(p->context, 0, sizeof *p->context);
  proc->pgdir = pgdir;
  proc->sz = sz;
  proc->tf->spc = elf.entry;  // main
  ktf->spc = elf.entry;
  //proc->context->r15 = sp;
  //proc->tf->r7_rank = argp -4;
  freevm(oldpgdir);
  switchuvm(proc);
  //swtch_stack(proc->context->r15); 
  return 0;

 bad:
  if(pgdir) freevm(pgdir);
  iunlockput(ip);
  return -1;
}
