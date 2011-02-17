void (* const vect_table[])(void) = {
	reserve,	//0x000 poweron reset
	reserve,	// 0x020 manual reset
	tlbmissR,	//0x040 tlb miss(read)
	tlbmissW,	//0x060 TLB miss(write)
	firstpagewrite, //0x080 first page write
	tlbprotecterrorR, //0x0a0 tlb permition (read)
	tlbprotecterrorW, //0x0c0 tlb permition(write)
	addresserrorR,	//0x0e0 address error(read)
	addresserrorW,	//0x100 address error(write)
	fpuexception,	//0x120 FPU exception
	tlbdoulblehit,	//0x140 TLB double hits
	reserve,	//0x160 TRAPA
	illigalinstruction, //0x180 illigal instruction
	slotilligal,	//0x1a0 slot illigal instruction
	nmi,		//0x1c0 NMI
	userbreak,	//0x1e0 user break
	ir115,		//0x200 IRL15
	ir114,		//0x220 IRL14
	ir113,		//0x240 IRL13
	ir112,		//0x260 IRL12
	ir111,		//0x280 IRL11
	ir110,		//0x2a0 IRL10
	ir19,		//0x2c0 IRL9
	ir18,		//0x2e0 IRL8
	ir17,		//0x300 IRL7
	ir16,		//0x320 IRL6
	ir15,		//0x340 IRL5
	ir14,		//0x360 IRL4
	ir13,		//0x380 IRL3
	ir12,		//0x3a0 IRL2
	ir11,		//0x3c0 IRL1
	reserve,	//0x3e0 -
	tuni0,		//0x400 TUNI0
	tuni1,		//0x420 TUNI1
	tuni2,		//0x440 TUNI2
	tuni3,		//0x460 TUNI3
	ati,		//0x480 RTC ATI
	pri,		//0x4a0 PRI
	cui,		//0x4c0 CUI
	eri1,		//0x4e0 SCI1 ERI
	rxi1,		//0x500 RXI
	txi1,		//0x520 TXi
	tei1,		//0x540 TEI
	iti,		//0x560 WDT ITI
	rcmi,		//0x580 BSC RCMI
	rovi,		//0x5a0 ROVI
	reserve,		//0x5c0 -
	reserve,		//0x5e0 -
	hudi,		//0x600 H-UDI
	reserve,	//0x620 -
	dei0,		//0x640 DEI0
	dei1,		//0x660 DEI1
	dei2,		//0x680 DEI2
	dei3,		//0x6a0 DEI3
	dmae,		//0x6c0 DMAE
	reserve,	//0x6e0 -
	eri2,		//0x700 SCIF ERI
	rxi2,		//0x720 RXI
	bri2,		//0x740 BRI
	txi2,		//0x760 TXI
	reserve,	//0x780 -
	reserve,	//0x7a0 -
	reserve,	//0x7c0 -
	reserve,	//0x7e0 -
	fpudisable,	//0x800 FPU disable
	slotfpudisable,	//0x820 slot FPU disable
	reserve,	//0x840 -
	trapa0		//0x860 TRAPA #0



};


void int_hdl(void)
{
vect_table[exp.expevt >> 5]();
}

void addressError(void)
{
set_cr(get_cr() & 0xefffffff);
//
//
set_cr(get_cr() | 0x10000000)
}
