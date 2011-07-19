#include "param.h"
#include "types.h"
#include "defs.h"
#include "sh4a.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"
//#include "tmu.h"

void tlbmissR()
{
  uint va = *(uint *)TEA;
  //tlb_register(va);
  return;
}

void tlbmissW()
{
  uint va = *(uint *)TEA;
  //tlb_register(va);
  return;
}


void PowerON_Reset(){;}
void Manual_Reset(){;} 
void TBL_Reset(){;}
void firstpagewrite(){;}
void tlbprotecterrorR(){;}
void tlbprotecterrorW(){;}

void addresserrorR()
{
  cprintf("%s:\n", __func__);
  while(1);
}
void addresserrorW(){;}
void fpuexception(){;}
void tlbdoulblehit(){;}
void INT_Reserved140(){;}
void INT_TRAPA(){;}

void illigalinstruction()
{
  cprintf("%s:\n", __func__);
  while(1);
}

void slotilligal(){;}
void nmi(){;}
void userbreak(){;}
void ir115(){;}
void ir114(){;}
void ir113(){;}
void ir112(){;}
void ir111(){;}
void ir110(){;}
void ir19(){;}
void ir18(){;}
void ir17(){;}
void ir16(){;}
void ir15(){;}
void ir14(){;}
void ir13(){;}
void ir12(){;}
void ir11(){;}

//ir10(){;}

void tuni0(){;}
void tuni1(){;}
void tuni2(){;}
void tuni3(){;}
void tuni4()
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
void ati(){;}		//0x480 RTC ATI
void pri(){;}		//0x4a0 PRI
void cui(){;}		//0x4c0 CUI
void eri1(){;}		//0x4e0 SCI1 ERI
void rxi1(){;}		//0x500 RXI
void txi1(){;}		//0x520 TXi
void tei1(){;}		//0x540 TEI
void iti(){;}		//0x560 WDT ITI
void rcmi(){;}		//0x580 BSC RCMI
void rovi(){;}		//0x5a0 ROVI
void hudi(){;}		//0x600 H-UDI
void dei0(){;}		//0x640 DEI0
void dei1(){;}		//0x660 DEI1
void dei2(){;}		//0x680 DEI2
void dei3(){;}		//0x6a0 DEI3
void dmae(){;}		//0x6c0 DMAE
void eri2(){;}		//0x700 SCIF ERI
void rxi2(){;}		//0x720 RXI
void bri2(){;}		//0x740 BRI
void txi2(){;}		//0x760 TXI
void fpudisable(){;}	//0x800 FPU disable
void slotfpudisable(){;}	//0x820 slot FPU disable
void trapa0(){;}	

//INT_Input_Capture(){;}
//INT_RTC_ATI(){;}
//INT_RTC_PRI(){;}
//INT_RTC_CUI(){;}
//INT_SCI_ERI(){;}
//INT_SCI_RXI(){;}
//INT_SCI_TXI(){;}
//INT_SCI_TEI(){;}
//INT_WDT(){;}
//INT_REF_RCMI(){;}
