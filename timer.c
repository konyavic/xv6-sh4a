#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "sh4.h"
#include "proc.h"
#include "spinlock.h"
#include "timer.h"

void timer_init(void)
{
  /* stop count TCNTx (not change other TCNT)*/
  TSTR = (TSTR & ((~TSTR_STR_BIT0) & (TSTR_STR_MASK)));

  /* set timer constant. */

  TCOR = TIMER_RATE;
  TCNT = TIMER_RATE;


  /* enable interrupts, counts Pck/3 clock */
  TCR &= ~(TCR_UNF_BIT|TCR_UNIE_BIT|TCR_CKEG_MASK|TCR_TPSC_MASK);
  
  /* clear under flowflag */
  TCR |= (TCR_UNIE_BIT | TCR_TPSC_PCK4);

  /* set interrupt level */
  IPRA_TMU0(TINTLVL);

  /* set interrupt mask */
  //IMSKC = IMSK_TMU;
  /*IMSK = (1UL << INTORG_TMUCH345_BIT);*/

  //spm_intr_handler_register(tmu_interval, TMU_INTEVT, 0);

  /* start timer */
  TSTR = (TSTR & TSTR_STR_MASK) | TSTR_STR_BIT0;
}

void do_timer(void)
{
  TCR &= ~TCR_UNF_BIT;
  if(cpu->id == 0){
    acquire(&tickslock);
    ticks++;
    wakeup(&ticks);
    release(&tickslock);
  }
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
}
