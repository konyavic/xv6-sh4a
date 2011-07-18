#define reserve 0

.align 4
.global vect_table
vect_table:
.long	reserve	//0x000 poweron reset
.long	reserve	// 0x020 manual reset
.long	tlbmissR	//0x040 tlb miss(read)
.long	tlbmissW	//0x060 TLB miss(write)
.long	firstpagewrite //0x080 first page write
.long	tlbprotecterrorR //0x0a0 tlb permition (read)
.long	tlbprotecterrorW //0x0c0 tlb permition(write)
.long	addresserrorR	//0x0e0 address error(read)
.long	addresserrorW	//0x100 address error(write)
.long	fpuexception	//0x120 FPU exception
.long	tlbdoulblehit	//0x140 TLB double hits
.long	syscall	//0x160 TRAPA
.long	illigalinstruction //0x180 illigal instruction
.long	slotilligal	//0x1a0 slot illigal instruction
.long	nmi		//0x1c0 NMI
.long	userbreak	//0x1e0 user break
.long	ir115		//0x200 IRL15
.long	ir114		//0x220 IRL14
.long	ir113		//0x240 IRL13
.long	ir112		//0x260 IRL12
.long	ir111		//0x280 IRL11
.long	ir110		//0x2a0 IRL10
.long	ir19		//0x2c0 IRL9
.long	ir18		//0x2e0 IRL8
.long	ir17		//0x300 IRL7
.long	ir16		//0x320 IRL6
.long	ir15		//0x340 IRL5
.long	ir14		//0x360 IRL4
.long	ir13		//0x380 IRL3
.long	ir12		//0x3a0 IRL2
.long	ir11		//0x3c0 IRL1
.long	reserve		//0x3e0 -
.long	tuni0		//0x400 TUNI0
.long	tuni1		//0x420 TUNI1
.long	tuni2		//0x440 TUNI2
.long	reserve		//0x460 TUNI3
.long	ati		//0x480 RTC ATI
.long	pri		//0x4a0 PRI
.long	cui		//0x4c0 CUI
.long	eri1		//0x4e0 SCI1 ERI
.long	rxi1		//0x500 RXI
.long	txi1		//0x520 TXi
.long	tei1		//0x540 TEI
.long	iti		//0x560 WDT ITI
.long	rcmi		//0x580 BSC RCMI
.long	rovi		//0x5a0 ROVI
.long	reserve		//0x5c0 -
.long	reserve		//0x5e0 -
.long	hudi		//0x600 H-UDI
.long	reserve		//0x620 -
.long	dei0		//0x640 DEI0
.long	dei1		//0x660 DEI1
.long	dei2		//0x680 DEI2
.long	dei3		//0x6a0 DEI3
.long	dmae		//0x6c0 DMAE
.long	reserve		//0x6e0 -
.long	eri2		//0x700 SCIF ERI
.long	do_scif1_read	//0x720 RXI
.long	bri2		//0x740 BRI
.long	txi2		//0x760 TXI
.long	reserve	//0x780 -
.long	reserve	//0x7a0 -
.long	reserve	//0x7c0 -
.long	reserve	//0x7e0 -
.long	fpudisable	//0x800 FPU disable
.long	slotfpudisable	//0x820 slot FPU disable
.long	reserve	//0x840 -
.long	trapa0		//0x860 TRAPA #0
.long	reserve //880
.long	reserve	//8a0
.long	reserve	//8c0
.long	reserve	//8e0
.long	reserve //900
.long	reserve	//920
.long	reserve //940
.long	reserve	//960
.long	reserve	//980
.long	reserve	//9a0
.long	reserve	//9c0
.long	reserve	//9e0
.long	reserve	//a00
.long	reserve	//a20
.long	reserve	//a40
.long	reserve	//a60
.long	reserve	//a80
.long	reserve	//aa0
.long	reserve	//ac0
.long	reserve	//ae0
.long	tuni3	//b00
.long	reserve	//b20
.long	reserve	//b40
.long	reserve	//b60
.long	tuni4	//b80
