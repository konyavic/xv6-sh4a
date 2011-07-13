#include "param.h"
#include "types.h"
#include "defs.h"
#include "xv6.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"

#define USERTOP  0x3600000
#define PGDIRFIX    0x8c000000

static pde_t *kpgdir;  // for use in scheduler()
extern struct cpu *bcpu;
pde_t *intpgdir;
// Set up CPU's kernel segment descriptors.
// Run once at boot time on each CPU.
void tlbinit()
{
uint tlbtable[64][3];
//tlbtable[0][0]= 0x0;
//tlbtable[0][1]= 0xff001b4;
//tlbtable[0][2]= 0x0;
tlbtable[1][0]= 0x0c1fec00;
tlbtable[1][1]= proc->pgdir[PTX(0x0c1fec00)];
tlbtable[1][2]= 0x0;
set_pteh(tlbtable[1][0]);
print_pteh();
set_ptel(tlbtable[1][1]);
print_ptel();
set_ptea(tlbtable[1][2]);
//print_ptea();
ldtlb();
//tlbtable[0][0]= 0x0;
//tlbtable[0][1]= proc->pgdir[0];
//tlbtable[0][2]= 0x0;
//set_pteh(tlbtable[0][0]);
//print_pteh();
//set_ptel(tlbtable[0][1]);
//print_ptel();
//set_ptea(tlbtable[0][2]);
//set_urc(1);
//ldtlb();
//return;
}

void
ksegment(void)
{
  //extern struct cpu cpus[1];
  struct cpu *c;
  uint sr;

  //set_proc_prel();
  // Map virtual addresses to linear addresses using identity map.
  // Cannot share a CODE descriptor for both kernel and user
  // because it would have to have DPL_USR, but the CPU forbids
  // an interrupt from CPL=0 to DPL=3.
  bcpu = &cpus[0];
  c = &cpus[0];
  //c->scheduler->ssr= 0;
  //sr= read_sr();
  //sr|=kernel_prel;
  //c->scheduler->ssr = sr;
  //load_sr(c->scheduler->ssr);

  //c->gdt[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
  //c->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
  //c->gdt[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, DPL_USER);
  //c->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);

  // Map cpu, and curproc
  //c->gdt[SEG_KCPU] = SEG(STA_W, &c->cpu, 8, 0);

  //lgdt(c->gdt, sizeof(c->gdt));
  //loadgs(SEG_KCPU << 3);
  
  // Initialize cpu-local storage.
  cpu = bcpu;
  proc = 0;
}

// Return the address of the PTE in page table pgdir
// that corresponds to linear address va.  If create!=0,
// create any required page table pages.
//static pte_t *
//walkpgdir(pde_t *pgdir, const void *va, int create)
//{
//  uint r;
//  pte_t *pde;
  //pte_t *pgtab;

//  return pde = &pgdir[PTX(va)];
  //if(*pde & PTE_P){
  //  pgtab = (pte_t*) PTE_ADDR(*pde);
  //} else if(!create || !(r = (uint) kalloc()))
  //  return 0;
  //else {
    //pgtab = (pte_t*) r;
    // Make sure all those PTE_P bits are zero.
    //memset(pgtab, 0, PGSIZE);
  //  // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table 
    // entries, if necessary.
  //  *pde = PADDR(r) | PTE_P | PTE_W | PTE_U;
  //}
  //return pde;
//}

// Create PTEs for linear addresses starting at la that refer to
// physical addresses starting at pa. la and size might not
// be page-aligned.
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
    //*pte &= PTE_PCD;
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


// The mappings from logical to linear are one to one (i.e.,
// segmentation doesn't do anything).
// There is one page table per process, plus one that's used
// when a CPU is not running any process (kpgdir).
// A user process uses the same page table as the kernel; the
// page protection bits prevent it from using anything other
// than its memory.
// 
// setupkvm() and exec() set up every page table like this:
//   0..640K          : user memory (text, data, stack, heap)
//   640K..1M         : mapped direct (for IO space)
//   1M..end          : mapped direct (for the kernel's text and data)
//   end..PHYSTOP     : mapped direct (kernel heap and user pages)
//   0xfe000000..0    : mapped direct (devices such as ioapic)
//
// The kernel allocates memory for its heap and for user memory
// between kernend and the end of physical memory (PHYSTOP).
// The virtual address space of each user program includes the kernel
// (which is inaccessible in user mode).  The user program addresses
// range from 0 till 640KB (USERTOP), which where the I/O hole starts
// (both in physical memory and in the kernel's virtual address
// space).

// Allocate one page table for the machine for the kernel address
// space for scheduler processes.
void
kvmalloc(void)
{
  kpgdir = setupkvm();
  intpgdir = kpgdir;
}

// Set up kernel part of a page table.
pde_t*
setupkvm(void)
{
  pde_t *pgdir;

  // Allocate page directory
  //pgdir = PGDIRFIX;
  if(!(pgdir = (pde_t *) pgtalloc()))
    return 0;
  memset(pgdir, 0, PGSIZE);
  if(// Map IO space from 640K to 1Mbyte
     !mappages(pgdir, (void *)0x80000000, 0x20000000, 0x00000000, PTE_W) ||
     // Map kernel and free memory pool
     !mappages(pgdir, (void *)0xe0000000, 0x20000000, 0x00000000, PTE_W) ||
     // Map devices such as ioapic, lapic, ...
     !mappages(pgdir, (void *)0x00000000, 0x20000000, 0x00000000, PTE_W) ||
     !mappages(pgdir, (void *)0xa0000000, 0x20000000, 0x00000000, PTE_W) ||   
     !mappages(pgdir, (void *)0xc0000000, 0x20000000, 0x00000000, PTE_W)
)
    return 0;
  return pgdir;
}

// Turn on paging.
void
vmenable(void)
{
  //uint cr0;
  switchkvm();
  enable_mmu();
   // load kpgdir into cr3
  //cr0 = rcr0();
  //cr0 |= CR0_PG;
  //lcr0(cr0);
}

// Switch h/w page table register to the kernel-only page table,
// for when no process is running.
void
switchkvm()
{
  pushcli();
  //clear_tlb();
  //disable_mmu();
  //clear_tlb();
  intpgdir= kpgdir;
    //enable_mmu();
//set_ttb(PADDR(kpgdir));
//lcr3(PADDR(kpgdir));   // switch to the kernel page table
  popcli();
}

// Switch h/w page table and TSS registers to point to process p.
void
switchuvm(struct proc *p)
{
  pushcli();
  // Setup TSS
  //cpu->gdt[SEG_TSS] = SEG16(STS_T32A, &cpu->ts, sizeof(cpu->ts)-1, 0);
  //cpu->gdt[SEG_TSS].s = 0;
  //cpu->ts.ss0 = SEG_KDATA << 3;
  //cpu->ts.esp0 = (uint)proc->kstack + KSTACKSIZE;
  //ltr(SEG_TSS << 3);
  if(p->pgdir == 0)
    panic("switchuvm: no pgdir\n");
  disable_mmu();
  clear_tlb();
  //set_val_in_p2();
  intpgdir = p->pgdir;
  enable_mmu();
  //set_ttb(0);
  //set_ttb(PADDR(p->pgdir));  // switch to new address space
  popcli();
}

// Return the physical address that a given user address
// maps to.  The result is also a kernel logical address,
// since the kernel maps the physical memory allocated to user
// processes directly.
char*
uva2ka(pde_t *pgdir, char *uva)
{    
  pte_t *pte = &pgdir[PTX(uva)];
  if(pte == 0) return 0;
  uint pa = PTE_ADDR(*pte);
  return (char *)pa;
}

// Load the initcode into address 0 of pgdir.
// sz must be less than a page.
void
inituvm(pde_t *pgdir, char *init, uint sz)
{
  char *mem = kalloc();
  if (sz >= PGSIZE)
    panic("inituvm: more than a page");
  memset(mem, 0, PGSIZE);
  mappages(pgdir, 0, PGSIZE, PADDR(mem) - KOFF, PTE_W|PTE_U|PTE_PWT|PTE_P);
  memmove(mem, init, sz);
}

// Load a program segment into pgdir.  addr must be page-aligned
// and the pages from addr to addr+sz must already be mapped.
//int
loaduvm(pde_t *pgdir, char *addr, struct inode *ip, uint offset, uint sz)
{
  uint i, pa, n;
  pte_t *pte;

  if((uint)addr % PGSIZE != 0)
    panic("loaduvm: addr must be page aligned\n");
  for(i = 0; i < sz; i += PGSIZE){
    if(!(pte = &pgdir[PTX(addr+i)]))
      panic("loaduvm: address should exist\n");
    pa = PTE_ADDR(*pte);
    if(sz - i < PGSIZE) n = sz - i;
    else n = PGSIZE;
    if(readi(ip, (char *)pa, offset+i, n) != n)
      return 0;
  }
  return 1;
}

// Allocate memory to the process to bring its size from oldsz to
// newsz. Allocates physical memory and page table entries. oldsz and
// newsz need not be page-aligned, nor does newsz have to be larger
// than oldsz.  Returns the new process size or 0 on error.
int
allocuvm(pde_t *pgdir, uint oldsz, uint newsz)
{
  //if(newsz > USERTOP)
    //return 0;
  char *a = (char *)PGROUNDUP(oldsz);
  char *last = PGROUNDDOWN(newsz - 1);
  for (; a <= last; a += PGSIZE){
    char *mem = kalloc();
    if(mem == 0){
      deallocuvm(pgdir, newsz, oldsz);
      return 0;
    }
    memset(mem, 0, PGSIZE);
    mappages(pgdir, a, PGSIZE, PADDR(mem) - KOFF, PTE_W|PTE_U);
  }
  return newsz > oldsz ? newsz : oldsz;
}

// Deallocate user pages to bring the process size from oldsz to
// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
// need to be less than oldsz.  oldsz can be larger than the actual
// process size.  Returns the new process size.
int
deallocuvm(pde_t *pgdir, uint oldsz, uint newsz)
{
  char *a = (char *)PGROUNDUP(newsz);
  char *last = PGROUNDDOWN(oldsz - 1);
  for(; a <= last; a += PGSIZE){
    pte_t *pte = &pgdir[PTX(a)];
    if(pte && (*pte & PTE_P) != 0){
      uint pa = PTE_ADDR(*pte);
      if(pa == 0)
        panic("kfree");
      kfree((void *) pa);
      *pte = 0;
    }
  }
  return newsz < oldsz ? newsz : oldsz;
}

// Free a page table and all the physical memory pages
// in the user part.
void
freevm(pde_t *pgdir)
{
  uint i;
  if(!pgdir)
    panic("freevm: no pgdir");
  //deallocuvm(pgdir, USERTOP, 0);
  for(i = 0; i < NPDENTRIES; i++){
    if(pgdir[i] & PTE_P)
      {
      if ((PTE_ADDR(pgdir[i]) >= 0x8ce00000 && PTE_ADDR(pgdir[i]) < 0x90000000) || (PTE_ADDR(pgdir[i]) >= 0xce00000 && PTE_ADDR(pgdir[i]) < 0x10000000))
      kfree((void *) PTE_ADDR(pgdir[i]));
      //else if (PTE_ADDR(pgdir[i]) >= 0xc200000 && PTE_ADDR(pgdir[i]) < 0xc300000)
      //stkfree((void *) (pgdir[i]&0xfffff000));
  }
  }
  pgtfree((void *) pgdir);
}



// Given a parent process's page table, create a copy
// of it for a child.
pde_t*
copyuvm(pde_t *pgdir, uint sz)
{
  pde_t *d = pgtalloc();
  pte_t *pte;
  uint pa, i;
  char *mem;

  if(!d) return 0;
  for(i = 0; i < sz; i += PGSIZE){
    if(!(pte = &pgdir[PTX(i)]))
      panic("copyuvm: pte should exist\n");
    if(!(*pte & PTE_P))
      panic("copyuvm: page not present\n");
    pa = PTE_ADDR(*pte);
    if(!(mem = kalloc()))
          goto bad;
    memmove(mem, (char *)pa + KOFF, PGSIZE);
    if(!mappages(d, (void *)i, PGSIZE, PADDR(mem) - KOFF, PTE_W|PTE_U))
     goto bad;
  }
  return d;

bad:
  freevm(d);
  return 0;
}



