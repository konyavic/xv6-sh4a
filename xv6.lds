OUTPUT_FORMAT("elf32-sh-linux", "elf32-sh-linux", "elf32-sh-linux")
OUTPUT_ARCH(sh)

ENTRY(start)

SECTIONS
{
    . = 0x8c800000;
    .start  : { *(.start) }
    .text   : { *(.text) }
    .data   : { *(.data) }
    . = ALIGN(32);
    .bss    : {
        __bss_start = .;
        *(.bss)
        . = ALIGN(32);
        __bss_end = .;
    }
}
