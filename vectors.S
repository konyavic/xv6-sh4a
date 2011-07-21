.align 4

.global vect_table
vect_table:
.long trap ! 0x000 poweron reset
.long trap ! 0x020 manual reset
.long do_tlb_miss ! 0x040 TLB miss (read)
.long do_tlb_miss ! 0x060 TLB miss (write)
.long trap ! 0x080 first page write
.long trap ! 0x0a0 TLB permission (read)
.long trap ! 0x0c0 TLB permission (write)
.long trap ! 0x0e0 address error(read)
.long trap ! 0x100 address error(write)
.long trap ! 0x120 FPU exception
.long trap ! 0x140 TLB double hits
.long do_syscall ! 0x160 TRAPA
.long trap ! 0x180 illigal instruction
.long trap ! 0x1a0 slot illigal instruction
.long trap ! 0x1c0 NMI
.long trap ! 0x1e0 user break
.long trap ! 0x200 IRL15
.long trap ! 0x220 IRL14
.long trap ! 0x240 IRL13
.long trap ! 0x260 IRL12
.long trap ! 0x280 IRL11
.long trap ! 0x2a0 IRL10
.long trap ! 0x2c0 IRL9
.long trap ! 0x2e0 IRL8
.long trap ! 0x300 IRL7
.long trap ! 0x320 IRL6
.long trap ! 0x340 IRL5
.long trap ! 0x360 IRL4
.long trap ! 0x380 IRL3
.long trap ! 0x3a0 IRL2
.long trap ! 0x3c0 IRL1
.long trap ! 0x3e0 -
.long do_timer ! 0x400 TMU TUNI0
.long trap ! 0x420 TMU TUNI1
.long trap ! 0x440 TMU TUNI2
.long trap ! 0x460 TICPI2
.long trap ! 0x480 RTC ATI
.long trap ! 0x4a0 RTC PRI
.long trap ! 0x4c0 RTC CUI
.long trap ! 0x4e0 SCI1 ERI
.long trap ! 0x500 SCI1 RXI
.long trap ! 0x520 SCI1 TXI
.long trap ! 0x540 SCI1 TEI
.long trap ! 0x560 WDT ITI
.long trap ! 0x580 BSC RCMI
.long trap ! 0x5a0 ROVI
.long trap ! 0x5c0 -
.long trap ! 0x5e0 -
.long trap ! 0x600 H-UDI
.long trap ! 0x620 -
.long trap ! 0x640 DEI0
.long trap ! 0x660 DEI1
.long trap ! 0x680 DEI2
.long trap ! 0x6a0 DEI3
.long trap ! 0x6c0 DMAE
.long trap ! 0x6e0 -
.long trap ! 0x700 SCIF ERI
.long do_scif_read ! 0x720 SCIF RXI
.long trap ! 0x740 SCIF BRI
.long trap ! 0x760 SCIF TXI
.long trap ! 0x780 -
.long trap ! 0x7a0 -
.long trap ! 0x7c0 -
.long trap ! 0x7e0 -
.long trap ! 0x800 FPU disable
.long trap ! 0x820 slot FPU disable
.long trap ! 0x840 -
.long trap ! 0x860 TRAPA #0
.long trap ! 0x880
.long trap ! 0x8a0
.long trap ! 0x8c0
.long trap ! 0x8e0
.long trap ! 0x900
.long trap ! 0x920
.long trap ! 0x940
.long trap ! 0x960
.long trap ! 0x980
.long trap ! 0x9a0
.long trap ! 0x9c0
.long trap ! 0x9e0
.long trap ! 0xa00
.long trap ! 0xa20
.long trap ! 0xa40
.long trap ! 0xa60
.long trap ! 0xa80
.long trap ! 0xaa0
.long trap ! 0xac0
.long trap ! 0xae0
.long trap ! 0xb00 TMU TUNI3
.long trap ! 0xb20
.long trap ! 0xb40
.long trap ! 0xb60
.long trap ! 0xb80 TMU TUNI4
