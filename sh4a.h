// Routines to let C code use special x86 instructions.
#include "param.h"

static inline uint
read_sr()
{
  uint sr;
  asm volatile(
      "stc sr, %0"
      :"=r"(sr)
      );
  return sr;
}
  static inline void
load_sr(uint val)
{
  asm volatile(
      "ldc %0, sr"
      :
      :"r"(val)
      );
}

static inline void cli(void)
{
  unsigned long __srval;
  asm volatile(
      "stc   sr,%0\n\t"
      "or    %1,%0\n\t"
      "ldc   %0,sr\n\t"
      :"+&z"(__srval)
      :"r"(SR_BL_DISABLE)
      );

}

static inline void sti(void)
{
  unsigned long __srval;
  asm volatile(
      "stc  sr,%0\n\t"
      "and   %1,%0\n\t"
      "ldc   %0,sr\n\t"
      :"+&z"(__srval)
      :"r"(SR_BL_ENABLE)
      );
}

static inline void set_proc_prel(void)
{
  unsigned long __srval;
  asm volatile(
      "stc  sr,%0\n\t"
      "and   %1,%0\n\t"
      "ldc   %0,sr\n\t"
      :"+&z"(__srval)
      :"r"(proc_prel)
      );
}

static inline void set_kerenl_prel(void)
{
  unsigned long __srval;
  asm volatile(
      "stc  sr,%0\n\t"
      "or   %1,%0\n\t"
      "ldc   %0,sr\n\t"
      :"+&z"(__srval)
      :"r"(kernel_prel)
      );
}
