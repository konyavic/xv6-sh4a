#define NPROC        64  // maximum number of processes
#define PAGE       4096  // granularity of user-space memory allocation
#define KSTACKSIZE PAGE  // size of per-process kernel stack
//#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NBUF         10  // size of disk block cache
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define EXPEVT 0xff000024
#define INTEVT 0xff000028

#define SR_BL_ENABLE    0x10000000
#define SR_BL_DISABLE   0xefffffff
#define SR_MDRBBL_MASK  0x70000000
#define SR_IMASK_MASK   0x000000f0
#define SR_IMASK_CLEAR  0xffffff0f
#define SR_RB_MASK		0x20000000

#define VBR_INIT		0xdeadbeef
#define kernel_prel  0x40000000
#define proc_prel    0xbfffffff
#define FL_IF        0x10000000      // Interrupt Enable

#define PGTSIZE	     0x4000        // pgtable size
#define STKSIZE	     0x1000        // stack size
#define VMOFF	     0x70000000    // virtual stack off
#define KOFF	     0x80000000    // kernel stack off
