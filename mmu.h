//void xv_mmu_init();
//static void print_ttb();
/* registers */
#define PTEH 0xff000000  // page-table entry high
#define PTEL 0xff000004  // page-table entry low
#define TTB  0xff000008  // transfer-table base
#define TEA  0xff00000c  // TLB exception address
#define MMUCR 0xff000010 // MMU control register
//#define PASCR 0xff000070 // phisical-address-space control register
//#define IRMCR 0xff000078 // instruction re-fetch control register
#define PTEA  0xff000034 //Page table entry assistance register 


//#define FL_CF           0x00000001      // Carry Flag
//#define FL_PF           0x00000004      // Parity Flag
//#define FL_AF           0x00000010      // Auxiliary carry Flag
//#define FL_ZF           0x00000040      // Zero Flag
//#define FL_SF           0x00000080      // Sign Flag
//#define FL_TF           0x00000100      // Trap Flag
//#define FL_IF           0x00000200      // Interrupt Enable
//#define FL_DF           0x00000400      // Direction Flag
//#define FL_OF           0x00000800      // Overflow Flag
//#define FL_IOPL_MASK    0x00003000      // I/O Privilege Level bitmask
//#define FL_IOPL_0       0x00000000      //   IOPL == 0
//#define FL_IOPL_1       0x00001000      //   IOPL == 1
//#define FL_IOPL_2       0x00002000      //   IOPL == 2
//#define FL_IOPL_3       0x00003000      //   IOPL == 3
//#define FL_NT           0x00004000      // Nested Task
//#define FL_RF           0x00010000      // Resume Flag
//#define FL_VM           0x00020000      // Virtual 8086 mode
//#define FL_AC           0x00040000      // Alignment Check
//#define FL_VIF          0x00080000      // Virtual Interrupt Flag
//#define FL_VIP          0x00100000      // Virtual Interrupt Pending
//#define FL_ID           0x00200000      // ID flag

// Control Register flags
//#define CR0_PE		0x00000001	// Protection Enable
//#define CR0_MP		0x00000002	// Monitor coProcessor
//#define CR0_EM		0x00000004	// Emulation
//#define CR0_TS		0x00000008	// Task Switched
//#define CR0_ET		0x00000010	// Extension Type
//#define CR0_NE		0x00000020	// Numeric Errror
//#define CR0_WP		0x00010000	// Write Protect
//#define CR0_AM		0x00040000	// Alignment Mask
//#define CR0_NW		0x20000000	// Not Writethrough
//#define CR0_CD		0x40000000	// Cache Disable
//#define CR0_PG		0x80000000	// Paging



//#define DPL_USER    0x3     // User DPL



// A linear address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(la) --/ \--- PTX(la) --/ 

// page directory index
#define PDX(la)		((((uint) (la)) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(la)		((((uint) (la)) >> PTXSHIFT) & 0x3FF)

// construct linear address from indexes and offset
#define PGADDR(d, t, o)	((uint) ((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// turn a kernel linear address into a physical address.
// all of the kernel data structures have linear and
// physical addresses that are equal.
#define PADDR(a)       ((uint) a)

// Page directory and page table constants.
#define NPDENTRIES	1024		// page directory entries per page directory
#define NPTENTRIES	1024		// page table entries per page table

#define PGSIZE		4096		// bytes mapped by a page
#define PGSHIFT		12		// log2(PGSIZE)

#define PTXSHIFT	12		// offset of PTX in a linear address
#define PDXSHIFT	22		// offset of PDX in a linear address

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) ((char*)((((unsigned int)(a)) & ~(PGSIZE-1))))

// PTEH
// +-----------+--+----------+
// | VPN 31:10 |xx| ASID 7:0 |
// +-----------+--+----------+
//
// PTEL
// +---+-----------+-+-+--+--+--+--+-+-+--+--+
// |xxx| PPN 28:10 |x|V|SZ|PR|PR|SZ|C|D|SH|WT|
// +---+-----------+-+-+--+--+--+--+-+-+--+--+
//
// pte
// +-----------+---+-+--+--+--+--+-+-+--+--+
// | PPN 31:12 |xxx|V|SZ|PR|PR|SZ|C|D|SH|WT|
// +-----------+---+-+--+--+--+--+-+-+--+--+

// Page table/directory entry flags.
#define	PTEL_WT         0x001 /* WT-bit on SH-4, 0 on SH-3 */
#define PTEL_HW_SHARED  0x002 /* SH-bit  : shared among processes */
#define PTEL_DIRTY      0x004 /* D-bit   : page changed */
#define PTEL_CACHABLE   0x008 /* C-bit   : cachable */
#define PTEL_SZ0        0x010 /* SZ0-bit : Size of page */
#define PTEL_RW         0x020 /* PR0-bit : write access allowed */
#define PTEL_USER       0x040 /* PR1-bit : user space access allowed */
#define PTEL_SZ1        0x080 /* SZ1-bit : Size of page (on SH-4) */
#define PTEL_V          0x100 /* V-bit   : page is valid */

#define PTEL_DEFAULT    ( PTEL_WT | PTEL_SZ0 | PTEL_RW | PTEL_USER | PTEL_V | PTEL_DIRTY )
// XXX: why causing miltiple hit without dirty?

//#define PTE_PWT  0x001	// Write-Through
//#define PTE_D		0x004	// Dirty
//#define PTE_P		0x100	// Present
//#define PTE_W		0x020	// Writeable
//#define PTE_U		0x040	// User
//#define PTE_PCD		0xfffffff7	// Cache-Disable
////#define PTE_A		0x020	// Accessed
//#define PTE_PS		0x090	// Page Size
////#define PTE_MBZ		0x180	// Bits must be zero

// Address in page table or page directory entry
#define PTE_ADDR(pte)	((uint) (pte) & ~0xFFF)
#define PTE_PERM(pte)	((uint) (pte) & 0x1FF)

void tlb_register(uint);

inline static void enable_mmu()
{
    uint * mmucr = (uint *)MMUCR;
    *mmucr |= 0x00000001;
    return;
}

inline static void disable_mmu()
{
    uint * mmucr = (uint *)MMUCR;
    *mmucr &= 0xfffffffe;
    return;
}

inline static void clear_tlb()
{
    uint * mmucr = (uint *)MMUCR;
    *mmucr |= 0x00000004;
    return;
}

inline static void set_urb(uint val)
{
    uint * mmucr = (uint *)MMUCR;
    if(val >= 64)
    {
       val = 63;
    }
    val <<= 18;
    val &= 0x00fc0000;
    *mmucr &= 0xff03ffff;
    *mmucr |= val;
    return;

}

inline static void set_urc(uint val)
{
    uint * mmucr = (uint *)MMUCR;
    if(val >= 64)
    {
        val = 63;
    }
    val <<= 10;
    val &= 0x0000fc00;
    *mmucr &= 0xffff03ff;
    *mmucr |= val;
    return;

}


inline static void print_pteh()
{
    uint * pteh = (uint *)PTEH;
    return;
}

inline static void set_pteh(uint val)
{
    uint * pteh = (uint *)PTEH;
    *pteh = (val & 0xffffffff);
    return;
}

inline static void print_ptea()
{
    uint * ptea = (uint *)PTEA;
    return;
}

inline static void set_ptea(uint val)
{
    uint * ptea = (uint *)PTEA;
    *ptea = (val & 0xffffffff);
    return;
}

inline static void set_ptel(uint val)
{
    uint * ptel = (uint *)PTEL;
    *ptel = (val & 0xffffffff);
    return;
}

inline static void print_ptel()
{
    uint * ptel = (uint *)PTEL;
    return;
}

inline static void ldtlb()
{
    __asm__ __volatile__("ldtlb\n\t");
    return;
}

inline static void set_ttb(uint val)
{
    uint * ttb = (uint *)TTB;
    * ttb = val;
    return;
}

inline static void print_ttb()
{
    uint * ttb = (uint *)TTB;
    return;
}

//static void set_mmucr(uint val)
//{
//    uint * mmucr = (uint *)MMUCR;
//    *mmucr = val;
//    return;
//}

//static void set_pascr(uint val)
//{
//    uint * pascr = (uint *)PASCR;
//    *pascr = val;
//    return;
//}

//static void set_irmcr(uint val)
//{
//    uint * irmcr = (uint *)IRMCR;
//   *irmcr = val;
//   return;
//}

//static void ocbp(uint val)
//{
//    __asm__ __volatile__("mov %0,%1\n\t"
//                         "ocbp @%1\n\t"
//                     :"+z" (val)
//                             :"r"  (0)
//                            );
//    return;
//}

//static void icbi(uint val)
//{
//    __asm__ __volatile__("mov %0,%1\n\t"
//                         "icbi @%1\n\t"
//                     :"+z" (val)
//                             :"r"  (0)
//                            );
//    return;
//}

//void xv_mmu_init()
//{
//    clear_tlb();
//    disable_mmu();
//    set_urc(0);
//    set_urb(0x3f);
//    set_ttb(0x8c000000);
//    print_ttb();
//    enable_mmu();
//    return;
//}

//static inline void sti(void)
//{
//    unsigned long __srval;
//    __asm__ __volatile__("stc   sr,%0\n\t"
//            "or    %1,%0\n\t"
//           "ldc   %0,sr\n\t"
//           :"+&z" (__srval)
//          :"r"   (SR_BL_ENABLE)
//          );

//}

//static inline void cli(void)
//{
//   unsigned long __srval;
//   __asm__ __volatile__("stc  sr,%0\n\t"
//            "and   %1,%0\n\t"
//            "ldc   %0,sr\n\t"
//            :"+&z" (__srval)
//            :"r"   (SR_BL_DISABLE)
//            );
//}

//static inline void set_proc_prel(void)
//{
//   unsigned long __srval;
//   __asm__ __volatile__("stc  sr,%0\n\t"
//            "and   %1,%0\n\t"
//            "ldc   %0,sr\n\t"
//            :"+&z" (__srval)
//            :"r"   (proc_prel)
//            );
//}

//static inline void set_kerenl_prel(void)
//{
//   unsigned long __srval;
//   __asm__ __volatile__("stc  sr,%0\n\t"
//            "or   %1,%0\n\t"
//            "ldc   %0,sr\n\t"
//            :"+&z" (__srval)
//            :"r"   (kernel_prel)
//            );
//}

inline static void set_val_in_p2(unsigned int addr, unsigned int val)
{
	unsigned int __tempval;
	__asm__ __volatile__(
		"mov.l 1f, %0\n\t"
		"or %1, %0\n\t"
		"jmp	@%0\n\t"
		"nop\n\t"
		".align 4\n"
		"1: .long 2f\n"
		"2:\n\t"

		"mov.l 3f, %0\n\t"
		//"synco\n\t"
		//"ocbi @%0\n\t"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//"mov.l 6f, r3\n\t"
		//"mov.l 5f, r1\n\t"
		//"mov.l r1, @r0\n\t"
		//"mov.l @r3, r3\n\t"
		"mov.l %2, @%3\n\t "
		//"and r2, r3\n\t"
		//"mov.l r1, @r0\n\t"		
		//"synco\n\t"
		//"ocbi @r0\n\t"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"jmp @%0\n\t"
		"nop\n"
		".align 4\n"
		"3: .long 4f\n"
		//"5: .long 0xfffffe00\n"
		//"6: .long 0xf2000000\n"
		"4:"

			: "=&r" (__tempval)
			: "r"	(0x20000000), "r" (val), "r" (addr)
		);
return;
}


