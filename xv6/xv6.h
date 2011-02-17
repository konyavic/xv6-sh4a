// Routines to let C code use special x86 instructions.
#include "param.h"


//static inline uchar
//inb(ushort port)
//{
//  uchar data;

//  asm volatile("mov.w @%1,%0" : "=r" (data) : "r" (port)
//               );
//  return data;
//}

//static inline void
//insl(int port, void *addr, int cnt)
//{
//  asm volatile(
//               "mov.l #cnt, r1"
//               "mov.l #addr, r2"
//               "mov.l #port, r3"
//               "loop:"
//               "mov.l @r3, @r2+"
//               "DT r1"
//               "BF loop"
//                );
//}

//static inline void
//outb(ushort port, uchar data)
//{
//  asm volatile("mov.w %1, @%0"
//               : "=r"(port)
//		: "r"(data)
//               );
//}

//static inline void
//outw(ushort port, ushort data)
//{
//  asm volatile("mov.w #data, r1"
//               "mov.w #port, r2"
//               "mov.w r1, @r2"
//               );
//}

//static inline void
//outsl(int port, const void *addr, int cnt)
//{
//  asm volatile("mov.l #cnt, r1"
//               "mov.l #addr, r2"
//               "mov.l #port, r3"
//               "loop:"
//               "mov.l @r2+, @r3"
//               "DT r1"
 //              "BF loop"
//               );
//}

static inline uint
read_sr()
{
uint sr;
__asm__ __volatile__("stc sr, %0"
			:"=r" (sr));
return sr;
}
static inline void
load_sr(uint val)
{
__asm__ __volatile__("ldc %0, sr"::"r"(val)
                     );
}

//static inline void load_r15(char *val)
//{
    //unsigned long __srval;
 //   __asm__ __volatile__("mov	%0, r15\n\t"
 //           "mov.l    _mainc, r0\n\t"
  //         "jmp   @r0\n\t"
 //          :
   //        :"r" (*val)
	   
  //        );
 //return;
//}
static inline void sti(void)
{
    unsigned long __srval;
    __asm__ __volatile__("stc   sr,%0\n\t"
            "or    %1,%0\n\t"
           "ldc   %0,sr\n\t"
           :"+&z" (__srval)
          :"r"   (SR_BL_ENABLE)
          );

}

static inline void cli(void)
{
   unsigned long __srval;
   __asm__ __volatile__("stc  sr,%0\n\t"
            "and   %1,%0\n\t"
            "ldc   %0,sr\n\t"
            :"+&z" (__srval)
            :"r"   (SR_BL_DISABLE)
            );
}

static inline void set_proc_prel(void)
{
   unsigned long __srval;
   __asm__ __volatile__("stc  sr,%0\n\t"
            "and   %1,%0\n\t"
            "ldc   %0,sr\n\t"
            :"+&z" (__srval)
            :"r"   (proc_prel)
            );
}

static inline void set_kerenl_prel(void)
{
   unsigned long __srval;
   __asm__ __volatile__("stc  sr,%0\n\t"
            "or   %1,%0\n\t"
            "ldc   %0,sr\n\t"
            :"+&z" (__srval)
            :"r"   (kernel_prel)
            );
}


//static uint
//xchg(uint *addr, uint newval)
//{
//  uint result;
//  result = *addr;
//  *addr = newval;
  // The + in "+m" denotes a read-modify-write operand.
 // __asm__ __volatile__("mov %0, %2"
 //              "mov %1, %0"
               //"mov %2, %1,"
 //              : "+m" (*addr), "+2"(result)
 //              : "1"(newval)
 //              : "cc"
  //             );
//   return result;
//}

//static inline void
//cli(void)
//{
//  asm volatile("ldc sr, r1"
//               "and #0xefffffff, r1"
//               "ldc r1, sr"
//               );
//}

//static inline void
//sti(void)
//{
//  asm volatile("ldc sr, r1"
//               "or #0x10000000, r1"
 //              "ldc r1, sr"
 //              );
//}




