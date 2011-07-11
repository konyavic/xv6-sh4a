#include "syscall.h"   
 void open(char *path, int omode)
{
    __asm__ __volatile__ (
    "mov %0, r3\n\t"
    "trapa #10\n\t"
    "nop\n\t"
    "nop\n\t"
	::"r" (SYS_open));
return;

}

    void mknod(char *path, int major, int minor)
{
    __asm__ __volatile__ ("mov #11, r3\n\t"
    "trapa #11\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void exec(char *path, char *argv[20])
{
    __asm__ __volatile__ ("mov #9, r3\n\t"
    "trapa #9\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void exit(void)
{
    __asm__ __volatile__ ("mov #2, r3\n\t"
    "trapa #2\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void dup(void)
{
    __asm__ __volatile__ ("mov #17, r3\n\t"
    "trapa #17\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void fork(void)
{
    __asm__ __volatile__ ("mov #1, r3\n\t"
    "trapa #1\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void wait(void)
{
    __asm__ __volatile__ ("mov #3, r3\n\t"
    "trapa #3\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}

    void write(void)
{
    __asm__ __volatile__ ("mov #5, r3\n\t"
    "trapa #5\n\t"
    "nop\n\t"
    "nop\n\t");
return;

}
