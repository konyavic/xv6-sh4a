#define NPROC        64  // maximum number of processes
#define KSTACKSIZE 4096  // size of per-process kernel stack
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NBUF         10  // size of disk block cache
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk

#define TRA    0xff000020
#define EXPEVT 0xff000024
#define INTEVT 0xff000028

#define SR_BL_DISABLE   0x10000000
#define SR_BL_ENABLE    0xefffffff
#define SR_MDRBBL_MASK  0x70000000
#define SR_IMASK_MASK   0x000000f0
#define SR_IMASK_CLEAR  0xffffff0f
#define SR_RB_MASK		0x20000000

#define kernel_prel  0x40000000
#define proc_prel    0xbfffffff
#define FL_IF        0x10000000      // Interrupt Disable

#define STACK_SIZE  0x1000

//#define PHYSTOP  0x8d000000 // use phys mem up to here as free pool
#define PHYSTOP  0x8c900000 // use phys mem up to here as free pool
