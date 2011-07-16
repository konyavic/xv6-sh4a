#include "param.h"
#include "types.h"
#include "defs.h"
#include "sh4a.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"
//#include "tmu.h"



//static void (* const vect_table[])(void);
uint i = 0;
//void exphandl(void)
//{
//uint  _INT_Vectors;
//   uint  expno;
//   uint *expevt = (uint *)EXPEVT;
//   expno = *expevt; 
//   (vect_table[expno >> 5])();
// __tlb_term();
//}
//void reserve(void);
tlbmissR()
{
  uint tlbtable[64][3];

  //tlbtable[0][0]= 0x0;
  //tlbtable[0][1]= 0xff001b4;
  //tlbtable[0][2]= 0x0;
  //tlbtable[1][0]= 0x0c1fec00;
  //tlbtable[1][1]= proc->pgdir[PTX(0x0c1fec00)];
  //tlbtable[1][2]= 0x0;
  //set_pteh(tlbtable[1][0]);
  //print_pteh();
  //set_ptel(tlbtable[1][1]);
  //print_ptel();
  //set_ptea(tlbtable[1][2]);
  //print_ptea();
  //ldtlb();
  uint * tea = (uint *) TEA;
  tlbtable[0][0]= *tea & 0xfffffc00;
  tlbtable[0][1]= intpgdir[PTX(*tea)];  // XXX: search in page table
  tlbtable[0][2]= 0x0;
  set_pteh(tlbtable[0][0]);
  print_pteh();
  set_ptel(tlbtable[0][1]);
  print_ptel();
  set_ptea(tlbtable[0][2]);

  set_urc(i);
  i++;
  if (i == 64)
    i = 1;

  ldtlb();
  return;
}


PowerON_Reset(){;}                                                                                                                   


Manual_Reset(){;} 

TBL_Reset(){;}                                                                                                                      




tlbmissW(){
  uint tlbtable[64][3];

  //tlbtable[0][0]= 0x0;
  //tlbtable[0][1]= 0xff001b4;
  //tlbtable[0][2]= 0x0;
  //tlbtable[1][0]= 0x0c1fec00;
  //tlbtable[1][1]= proc->pgdir[PTX(0x0c1fec00)];
  //tlbtable[1][2]= 0x0;
  //set_pteh(tlbtable[1][0]);
  //print_pteh();
  //set_ptel(tlbtable[1][1]);
  //print_ptel();
  //set_ptea(tlbtable[1][2]);
  //print_ptea();
  //ldtlb();
  uint * tea = (uint *) TEA;
  tlbtable[0][0]= *tea & 0xfffffc00;

  tlbtable[0][1]= intpgdir[PTX(*tea)];
  tlbtable[0][2]= 0x0;
  set_pteh(tlbtable[0][0]);
  print_pteh();
  set_ptel(tlbtable[0][1]);
  print_ptel();
  set_ptea(tlbtable[0][2]);

  set_urc(i);
  i++;
  if (i == 64)
    i = 1;

  ldtlb();
  return;
}


firstpagewrite(){;}
tlbprotecterrorR(){;}

tlbprotecterrorW(){uint tlbtable[64][3];

  //tlbtable[0][0]= 0x0;
  //tlbtable[0][1]= 0xff001b4;
  //tlbtable[0][2]= 0x0;
  //tlbtable[1][0]= 0x0c1fec00;
  //tlbtable[1][1]= proc->pgdir[PTX(0x0c1fec00)];
  //tlbtable[1][2]= 0x0;
  //set_pteh(tlbtable[1][0]);
  //print_pteh();
  //set_ptel(tlbtable[1][1]);
  //print_ptel();
  //set_ptea(tlbtable[1][2]);
  //print_ptea();
  //ldtlb();
  uint * tea = (uint *) TEA;
  tlbtable[0][0]= *tea & 0xfffffc00;
  tlbtable[0][1]= intpgdir[PTX(*tea)];
  tlbtable[0][2]= 0x0;
  set_pteh(tlbtable[0][0]);
  print_pteh();
  set_ptel(tlbtable[0][1]);
  print_ptel();
  set_ptea(tlbtable[0][2]);

  set_urc(i);
  i++;
  if (i = 64)
    i = 1;

  ldtlb();
  return;
}

addresserrorR(){;}

addresserrorW(){;}

fpuexception(){;}
tlbdoulblehit(){;}
INT_Reserved140(){;}

INT_TRAPA(){;}

illigalinstruction(){;}

slotilligal(){;}

nmi(){;}

userbreak(){;}

ir115(){;}

ir114(){;}

ir113(){;}

ir112(){;}

ir111(){;}

ir110(){;}

ir19(){;}

ir18(){;}

ir17(){;}

ir16(){;}

ir15(){;}

ir14(){;}

ir13(){;}

ir12(){;}

ir11(){;}

//ir10(){;}

tuni0(){;}

tuni1(){;}

tuni2(){;}
tuni3(){;}
void tuni4(void)
{
  tmu_mask();
  if(cpu->id == 0){
    acquire(&tickslock);
    ticks++;
    wakeup(&ticks);
    release(&tickslock);
  }
  tmu_clr();
  if(proc && proc->killed)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed)
    exit();

  return;
}	//0x460 TUNI3
ati(){;}		//0x480 RTC ATI
pri(){;}		//0x4a0 PRI
cui(){;}		//0x4c0 CUI
eri1(){;}		//0x4e0 SCI1 ERI
rxi1(){;}		//0x500 RXI
txi1(){;}		//0x520 TXi
tei1(){;}		//0x540 TEI
iti(){;}		//0x560 WDT ITI
rcmi(){;}		//0x580 BSC RCMI
rovi(){;}		//0x5a0 ROVI

hudi(){;}		//0x600 H-UDI

dei0(){;}		//0x640 DEI0
dei1(){;}		//0x660 DEI1
dei2(){;}		//0x680 DEI2
dei3(){;}		//0x6a0 DEI3
dmae(){;}		//0x6c0 DMAE

eri2(){;}		//0x700 SCIF ERI
rxi2(){;}		//0x720 RXI
bri2(){;}		//0x740 BRI
txi2(){;}		//0x760 TXI

fpudisable(){;}	//0x800 FPU disable
slotfpudisable(){;}	//0x820 slot FPU disable

trapa0(){;}	

//                 INT_Input_Capture(){;}

//                 INT_RTC_ATI(){;}

//                 INT_RTC_PRI(){;}

//                 INT_RTC_CUI(){;}

//                 INT_SCI_ERI(){;}

//                INT_SCI_RXI(){;}

//                INT_SCI_TXI(){;}

//                 INT_SCI_TEI(){;}

//                 INT_WDT(){;}

//                 INT_REF_RCMI(){;}



