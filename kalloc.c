// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "spinlock.h"

#define mem_start1 0x8ce00000
#define mem_end1   0x90000000
//#define mem_start2 0x0ca00000
//#define mem_end2   0x10000000
#define pgt_start 0x8c000000
#define pgt_end   0x8c100000
#define stk_start 0x0c200000
#define stk_end   0x0c300000


struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  struct run *pgtfreelist;
} pgtmem;

struct {
  struct spinlock lock;
  struct run *stkfreelist;
} stkmem;

// Initialize free list of physical pages.
void
kinit(void)
{
  //extern char end[];
  struct run *r;
  //char *mem_s = mem_start;
  //char *men_e = mem_end;  

  initlock(&kmem.lock, "kmem");
     char *p= (char*) mem_start1;
  for( ; p + PGSIZE - 1 < (char*) mem_end1; p += PGSIZE)
    kfree(p);
//    p = mem_start2;
//  for( ; p + PGSIZE - 1 < (char*) mem_end2; p += PGSIZE)
//    kfree(p);
  
}

// Initialize free list of pgtabels.
void
pgtinit(void)
{
  //extern char end[];
  //char *mem_s = mem_start;
  //char *men_e = mem_end;  

  initlock(&pgtmem.lock, "pgtmem");
     char *g= (char*) pgt_start;
  for( ; g + PGTSIZE - 1 < (char*) pgt_end; g += PGTSIZE)
    pgtfree(g);
}

// Initialize free list of stktabels.
void
stkinit(void)
{
  //extern char end[];
  //char *mem_s = mem_start;
  //char *men_e = mem_end;  

  initlock(&stkmem.lock, "stkmem");
     char *k= (char*) stk_start;
  for( ; k + STKSIZE - 1 < (char*) stk_end; k += STKSIZE)
    stkfree(k);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;
  if(0xce00000 <= (uint)v && (uint)v <= 0x10000000)
  v += 0x80000000;

  if(((uint) v) % PGSIZE || (uint)v < mem_start1 || (uint)v >= mem_end1) 
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 0, PGSIZE);

  acquire(&kmem.lock);
  r = (struct run *) v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

void
pgtfree(char *v)
{
  struct run *p;

  if(((uint) v) % PGTSIZE || (uint)v < pgt_start || (uint)v >= pgt_end) 
    panic("pgtfree");

  // Fill with junk to catch dangling refs.
  memset(v, 0, PGTSIZE);

  acquire(&pgtmem.lock);
  p = (struct run *) v;
  p->next = pgtmem.pgtfreelist;
  pgtmem.pgtfreelist = p;
  release(&pgtmem.lock);
}

void
stkfree(char *v)
{
  struct run *r;

  if(((uint) v) % STKSIZE || (uint)v < stk_start || (uint)v >= stk_end) 
    panic("stkfree");

  // Fill with junk to catch dangling refs.
  memset(v, 0, STKSIZE);

  acquire(&stkmem.lock);
  r = (struct run *) v;
  r->next = stkmem.stkfreelist;
  stkmem.stkfreelist = r;
  release(&stkmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc()
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);
  return (char*) r;
}

char*
pgtalloc()
{
  struct run *r;

  acquire(&pgtmem.lock);
  r = pgtmem.pgtfreelist;
  if(r)
    pgtmem.pgtfreelist = r->next;
  release(&pgtmem.lock);
  return (char*) r;
}

char*
stkalloc()
{
  struct run *r;

  acquire(&stkmem.lock);
  r = stkmem.stkfreelist;
  if(r)
    stkmem.stkfreelist = r->next;
  release(&stkmem.lock);
  return (char*) r;
}


