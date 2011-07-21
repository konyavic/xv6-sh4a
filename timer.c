#include <timer.h>

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

