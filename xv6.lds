OUTPUT_FORMAT("elf32-sh-linux", "elf32-sh-linux", "elf32-sh-linux")
OUTPUT_ARCH(sh)

ENTRY(start)

SECTIONS
{

    . = 0x8c800000;
    .start    : { *(.start) }
    .EXP  : { *(.EXP) }
    .text     : { *(.text) }

    /* . = DATA_START */
    .data     : { *(.data) }

    .bss	:{
        __bss_start = .;
        *(.bss)
        . = ALIGN(32);
    
    }

}

