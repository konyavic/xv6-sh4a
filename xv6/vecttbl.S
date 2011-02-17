.section        .VECTTBL
.global _RESET_Vectors
_RESET_Vectors:
!<<VECTOR DATA START (POWER ON RESET)>>
        !0x000 Power On Reset (Hitachi-UDI RESET)
        .long	PowerON_Reset                                                                                                                  
!<<VECTOR DATA END (POWER ON RESET)>>
!<<VECTOR DATA START (MANUAL RESET)>>
        !0x020 Manual Reset
        .long	Manual_Reset                                                                                                                   
!<<VECTOR DATA END (MANUAL RESET)>>
! Reserved
        .long        8,0x00000000
!<<VECTOR DATA START (TBL RESET)>>
        !0x140 TBL Reset (DATA TBL Reset)
        .long         TBL_Reset                                                                                                                      
!<<VECTOR DATA END (TBL RESET)>>

        .section    .INTTBL
.global _INT_Vectors
_INT_Vectors:
        !0x040 TLB miss/invalid (load)
        .long         INT_TLBMiss_Load
        !0x060 TLB miss/invalid (store)
        .long         INT_TLBMiss_Store
        !0x080 Initial page write
        .long         INT_TLBInitial_Page
        !0x0A0 TLB protect (load)
        .long         INT_TLBProtect_Load
        !0x0C0 TLB protect (store)
        .long         INT_TLBProtect_Store
        !0x0E0 Address error (load)
        .long         INT_Address_load
        !0x100 Address error (store)
        .long         INT_Address_store
        !0x120 FPU invalid
        .long         INT_FPU
        !0x140 Reserved
        .long         INT_Reserved140
        !0x160 TRAPA
        .long         INT_TRAPA
        !0x180 Illegal code
        .long         INT_Illegal_code
        !0x1A0 Illegal slot
        .long         INT_Illegal_slot
        !0x1C0 NMI
        .long         INT_NMI
        !0x1E0 User Break
        .long         INT_User_Break
        !0x200 External hardware interrupt
        .long         INT_Extern_0000
        !0x220 External hardware interrupt
        .long         INT_Extern_0001
        !0x240 External hardware interrupt
        .long         INT_Extern_0010
        !0x260 External hardware interrupt
        .long         INT_Extern_0011
        !0x280 External hardware interrupt
        .long         INT_Extern_0100
        !0x2A0 External hardware interrupt
        .long         INT_Extern_0101
        !0x2C0 External hardware interrupt
        .long         INT_Extern_0110
        !0x2E0 External hardware interrupt
        .long         INT_Extern_0111
        !0x300 External hardware interrupt
        .long         INT_Extern_1000
        !0x320 External hardware interrupt
        .long         INT_Extern_1001
        !0x340 External hardware interrupt
        .long         INT_Extern_1010
        !0x360 External hardware interrupt
        .long         INT_Extern_1011
        !0x380 External hardware interrupt
        .long         INT_Extern_1100
        !0x3A0 External hardware interrupt
        .long         INT_Extern_1101
        !0x3C0 External hardware interrupt
        .long         INT_Extern_1110
        !0x3E0 Reserved
        .long         INT_Reserved3E0
        !0x400 TMU0 TUNI0
        .long         INT_Timer_Under_0
        !0x420 TMU1 TUNI1
        .long         INT_Timer_Under_1
        !0x440 TMU2 TUNI2
        .long         INT_Timer_Under_2
        !0x460 TMU2 TICPI2
        .long         INT_Input_Capture
        !0x480 RTC ATI
        .long         INT_RTC_ATI
        !0x4A0 RTC PRI
        .long         INT_RTC_PRI
        !0x4C0 RTC CUI
        .long         INT_RTC_CUI
        !0x4E0 SCI ERI
        .long         INT_SCI_ERI
        !0x500 SCI RXI
        .long         INT_SCI_RXI
        !0x520 SCI TXI
        .long         INT_SCI_TXI
        !0x540 SCI TEI
        .long         INT_SCI_TEI
        !0x560 WDT ITI
        .long         INT_WDT
        !0x580 REF RCMI
        .long         INT_REF_RCMI
        !0x5A0 REF ROVI
        .long         !INT_REF_ROVI
        !0x5C0 Reserved
        .long         !INT_Reserved5C0
        !0x5E0 Reserved
        .long         !INT_Reserved5E0
        !0x600 Hitachi-UDI
        .long         !INT_HUDI
        !0x620 Reserved
        .long         !INT_Reserved620
        !0x640 DMAC DMTE0
        .long         !INT_DMAC_DMTE0
        !0x660 DMAC DMTE1
        .long         !INT_DMAC_DMTE1
        !0x680 DMAC DMTE2
        .long         !INT_DMAC_DMTE2
        !0x6A0 DMAC DMTE3
        .long         !INT_DMAC_DMTE3
        !0x6C0 DMAC DMAE
        .long         !INT_DMAC_DMAE
        !0x6E0 Reserved
        .long         !INT_Reserved6E0
        !0x700 SCIF ERI
        .long         !INT_SCIF_ERI
        !0x720 SCIF RXI
        .long         !INT_SCIF_RXI
        !0x740 SCIF BRI
        .long         !INT_SCIF_BRI
        !0x760 SCIF TXI
        .long         !INT_SCIF_TXI
        !0x780 Reserved
        .long         !INT_DMAC_DMTE4
        !0x7A0 Reserved
        .long         !INT_DMAC_DMTE5
        !0x7C0 Reserved
        .long         !INT_DMAC_DMTE6
        !0x7E0 Reserved
        .long         !INT_DMAC_DMTE7
        !0x800 Illegal FPU
        .long         !INT_Illegal_FPU
        !0x820 Illegal slot FPU
        .long         !NT_Illegal_slot_FPU
        !0x840 Reserved
        .long         !INT_Reserved840
        !0x860 Reserved
        .long         !INT_Reserved860
        !0x880 Reserved
        .long         !INT_Reserved880
        !0x8A0 Reserved
        .long         !INT_Reserved8A0
        !0x8C0 Reserved
        .long         !INT_Reserved8C0
        !0x8E0 Reserved
        .long         !INT_Reserved8E0
        !0x900 Reserved
        .long         !INT_Reserved900
        !0x920 Reserved
        .long         !INT_Reserved920
        !0x940 Reserved
        .long         !INT_Reserved940
        !0x960 Reserved
        .long         !INT_Reserved960
        !0x980 Reserved
        .long         !INT_Reserved980
        !0x9A0 Reserved
        .long         !INT_Reserved9A0
        !0x9C0 Reserved
        .long         !INT_Reserved9C0
        !0x9E0 Reserved
        .long         !INT_Reserved9E0
        !0xA00 PCIC PCISERR
        .long         !INT_PCIC_PCISERR
        !0xA20 PCIC PCIDMA3
        .long         !INT_PCIC_PCIDMA3
        !0xA40 PCIC PCIDMA2
        .long         !INT_PCIC_PCIDMA2
        !0xA60 PCIC PCIDMA1
        .long         !INT_PCIC_PCIDMA1
        !0xA80 PCIC PCIDMA0
        .long         !INT_PCIC_PCIDMA0
        !0xAA0 PCIC PCIPWON
        .long         !INT_PCIC_PCIPWON
        !0xAC0 PCIC PCIPWDWN
        .long         !INT_PCIC_PCIPWDWN
        !0xAE0 PCIC PCIERR
        .long         !INT_PCIC_PCIERR
        !0xB00 TMU3 TUNI3
        .long         !INT_Timer_Under_3
        !0xB20 Reserved
        .long         !INT_ReservedB20
        !0xB40 Reserved
        .long         !INT_ReservedB40
        !0xB60 Reserved
        .long         !INT_ReservedB60
        !0xB80 TMU4 TUNI4
        .long         !INT_Timer_Under_4


_INT_MASK:
        !0x040 TLB miss/invalid (load)
        .byte         0xF0
        !0x060 TLB miss/invalid (store)
        .byte         0xF0
        !0x080 Initial page write
        .byte         0xF0
        !0x0A0 TLB protect (load)
        .byte         0xF0
        !0x0C0 TLB protect (store)
        .byte         0xF0
        !0x0E0 Address error (load)
        .byte         0xF0
        !0x100 Address error (store)
        .byte         0xF0
        !0x120 FPU invalid
        .byte         0xF0
        !0x140 Reserved
        .byte         0xF0
        !0x160 TRAPA
        .byte         0xF0
        !0x180 Illegal code
        .byte         0xF0
        !0x1A0 Illegal slot
        .byte         0xF0
        !0x1C0 NMI
        .byte         0xF0
        !0x1E0 User Break
        .byte         0xF0
        !0x200 External hardware interrupt
        .byte         0xF0
        !0x220 External hardware interrupt
        .byte         0xF0
        !0x240 External hardware interrupt
        .byte         0xF0
        !0x260 External hardware interrupt
        .byte         0xF0
        !0x280 External hardware interrupt
        .byte         0xF0
        !0x2A0 External hardware interrupt
        .byte         0xF0
        !0x2C0 External hardware interrupt
        .byte         0xF0
        !0x2E0 External hardware interrupt
        .byte         0xF0
        !0x300 External hardware interrupt
        .byte         0xF0
        !0x320 External hardware interrupt
        .byte         0xF0
        !0x340 External hardware interrupt
        .byte         0xF0
        !0x360 External hardware interrupt
        .byte         0xF0
        !0x380 External hardware interrupt
        .byte         0xF0
        !0x3A0 External hardware interrupt
        .byte         0xF0
        !0x3C0 External hardware interrupt
        .byte         0xF0
        !0x3E0 Reserved
        .byte         0xF0
        !0x400 TMU0 TUNI0
        .byte         0xF0
        !0x420 TMU1 TUNI1
        .byte         0xF0
        !0x440 TMU2 TUNI2
        .byte         0xF0
        !0x460 TMU2 TICPI2
        .byte         0xF0
        !0x480 RTC ATI
        .byte         0xF0
        !0x4A0 RTC PRI
        .byte         0xF0
        !0x4C0 RTC CUI
        .byte         0xF0
        !0x4E0 SCI ERI
        .byte         0xF0
        !0x500 SCI RXI
        .byte         0xF0
        !0x520 SCI TXI
        .byte         0xF0
        !0x540 SCI TEI
        .byte         0xF0
        !0x560 WDT ITI
        .byte         0xF0
        !0x580 REF RCMI
        .byte         0xF0
        !0x5A0 REF ROVI
        .byte         0xF0
        !0x5C0 Reserved
        .byte         0xF0
        !0x5E0 Reserved
        .byte         0xF0
        !0x600 Hitachi-UDI
        .byte         0xF0
        !0x620 Reserved
        .byte         0xF0
        !0x640 DMAC DMTE0
        .byte         0xF0
        !0x660 DMAC DMTE1
        .byte         0xF0
        !0x680 DMAC DMTE2
        .byte         0xF0
        !0x6A0 DMAC DMTE3
        .byte         0xF0
        !0x6C0 DMAC DMAE
        .byte         0xF0
        !0x6E0 Reserved
        .byte         0xF0
        !0x700 SCIF ERI
        .byte         0xF0
        !0x720 SCIF RXI
        .byte         0xF0
        !0x740 SCIF BRI
        .byte         0xF0
        !0x760 SCIF TXI
        .byte         0xF0
        !0x780 DMAC DMTE4
        .byte         0xF0
        !0x7A0 DMAC DMTE5
        .byte         0xF0
        !0x7C0 DMAC DMTE6
        .byte         0xF0
        !0x7E0 DMAC DMTE7
        .byte         0xF0
        !0x800 Illegal FPU
        .byte         0xF0
        !0x820 Illegal slot FPU
        .byte         0xF0
        !0x840 Reserved
        .byte         0xF0
        !0x860 Reserved
        .byte         0xF0
        !0x880 Reserved
        .byte         0xF0
        !0x8A0 Reserved
        .byte         0xF0
        !0x8C0 Reserved
        .byte         0xF0
        !0x8E0 Reserved
        .byte         0xF0
        !0x900 Reserved
        .byte         0xF0
        !0x920 Reserved
        .byte         0xF0
        !0x940 Reserved
        .byte         0xF0
        !0x960 Reserved
        .byte         0xF0
        !0x980 Reserved
        .byte         0xF0
        !0x9A0 Reserved
        .byte         0xF0
        !0x9C0 Reserved
        .byte         0xF0
        !0x9E0 Reserved
        .byte         0xF0
        !0xA00 PCIC PCISERR
        .byte         0xF0
        !0xA20 PCIC PCIDMA3
        .byte         0xF0
        !0xA40 PCIC PCIDMA2
        .byte         0xF0
        !0xA60 PCIC PCIDMA1
        .byte         0xF0
        !0xA80 PCIC PCIDMA0
        .byte         0xF0
        !0xAA0 PCIC PCIPWON
        .byte         0xF0
        !0xAC0 PCIC PCIPWDWN
        .byte         0xF0
        !0xAE0 PCIC PCIERR
        .byte         0xF0
        !0xB00 TMU3 TUNI3
        .byte         0xF0
        !0xB20 Reserved
        .byte         0xF0
        !0xB40 Reserved
        .byte         0xF0
        !0xB60 Reserved
        .byte         0xF0
        !0xB80 TMU4 TUNI4
        .byte         0xF0
