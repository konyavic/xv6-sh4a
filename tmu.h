//#include <spumone/spumone.h>


#define REGB(x) (*((volatile unsigned char *)(x)))
#define REGS(x) (*((volatile unsigned short *)(x)))
#define REGI(x) (*((volatile unsigned int *)(x)))

#define TMU_USEC_TO_HZ(t) (t * (12500000ul / 1000 / 1000))
#define TMU_MSEC_TO_HZ(t) (t * (12500000ul / 1000))

#define TIMER_RATE	(12500000UL)  /* use PCK/4 edge.  12.5Mhz 1tick = 0.08us */
#define TIMER_RATE_MHZ  (TIMER_RATE/1000000UL)
#define COUNT_FROM_DEFAULT 150


#define TMU_BASE_012    0xFFD80000
#define TMU_BASE_345    0xFEDC0000

#define TMU_OFFSET0     0x08
#define TMU_OFFSET1     0x14
#define TMU_OFFSET2     0x20
#define TMU_OFFSET3     0x08
#define TMU_OFFSET4     0x14
#define TMU_OFFSET5     0x20

#define TSTR_OFFSET     0x04
#define TCOR_OFFSET     0x00
#define TCNT_OFFSET     0x04
#define TCR_OFFSET      0x08
#define TCPR_OFFSET     0x0C

#define TSTR0        REGB(TMU_BASE_012 + 0x04) /* Timer start register */
#define TSTR1        REGB(TMU_BASE_345 + 0x04) /* Timer start register */

#define TCOR0        REGI(TMU_BASE_012 + TMU_OFFSET0 + TCOR_OFFSET) /* Timer constant register */
#define TCNT0        REGI(TMU_BASE_012 + TMU_OFFSET0  + TCNT_OFFSET) /* Timer counter */
#define TCR0         REGS(TMU_BASE_012 + TMU_OFFSET0  + TCR_OFFSET) /* Timer control register */
#define TCOR1        REGI(TMU_BASE_012 + TMU_OFFSET1 + TCOR_OFFSET) /* Timer constant register */
#define TCNT1        REGI(TMU_BASE_012 + TMU_OFFSET1  + TCNT_OFFSET) /* Timer counter */
#define TCR1         REGS(TMU_BASE_012 + TMU_OFFSET1  + TCR_OFFSET) /* Timer control register */
#define TCOR2        REGI(TMU_BASE_012 + TMU_OFFSET2 + TCOR_OFFSET) /* Timer constant register */
#define TCNT2        REGI(TMU_BASE_012 + TMU_OFFSET2  + TCNT_OFFSET) /* Timer counter */
#define TCR2         REGS(TMU_BASE_012 + TMU_OFFSET2  + TCR_OFFSET) /* Timer control register */

#define TCOR3        REGI(TMU_BASE_345 + TMU_OFFSET3 + TCOR_OFFSET) /* Timer constant register */
#define TCNT3        REGI(TMU_BASE_345 + TMU_OFFSET3  + TCNT_OFFSET) /* Timer counter */
#define TCR3         REGS(TMU_BASE_345 + TMU_OFFSET3  + TCR_OFFSET) /* Timer control register */
#define TCOR4        REGI(TMU_BASE_345 + TMU_OFFSET4 + TCOR_OFFSET) /* Timer constant register */
#define TCNT4        REGI(TMU_BASE_345 + TMU_OFFSET4  + TCNT_OFFSET) /* Timer counter */
#define TCR4         REGS(TMU_BASE_345 + TMU_OFFSET4  + TCR_OFFSET) /* Timer control register */
#define TCOR5        REGI(TMU_BASE_345 + TMU_OFFSET5 + TCOR_OFFSET) /* Timer constant register */
#define TCNT5        REGI(TMU_BASE_345 + TMU_OFFSET5  + TCNT_OFFSET) /* Timer counter */
#define TCR5         REGS(TMU_BASE_345 + TMU_OFFSET5  + TCR_OFFSET) /* Timer control register */

#define WDTCSR      REGI(0xFFCC0004)
#define SWITCH_WDT    0x00000088


/* Default Value for ch-5 */
#define TMU_BASE        TMU_BASE_345
#define TMU_OFFSET      TMU_OFFSET5

#define TSTR        REGB(TMU_BASE + 0x04) /* Timer start register */
#define TCOR        REGI(TMU_BASE + TMU_OFFSET + TCOR_OFFSET) /* Timer constant register */
#define TCNT        REGI(TMU_BASE + TMU_OFFSET  + TCNT_OFFSET) /* Timer counter */
#define TCR         REGS(TMU_BASE + TMU_OFFSET  + TCR_OFFSET) /* Timer control register */


/* bit definitions */
/* TOCR */
#define TOCR_TCOE	        0x01 /* Timer Clockpin Control. TCLK select 0:ext/input-caputre, 1:internal RTC*/
#define TOCR_MASK	        (TOCR_TCOE)

/* TSTR */
#define TSTR_STR_MASK       0x07

#define TCR_UNF_BIT	        0x0100 /* Underflow Flag 1: underflow occured. */
#define TCR_UNIE_BIT        0x0020 /* Underflow Interrupt Control. 1: enable TUNIn */

#define TCR_CKEG_MASK   	0x0018 /* Clock Edge select */
#define TCR_CKEG_UP         0x0000 /*  select Up edge */
#define TCR_CKEG_DOWN   	0x0008 /*  select Down edge */
#define TCR_CKEG_BOTH       0x0010 /*  select UP/Down both edge */

#define TCR_TPSC_MASK   	0x0007 /* Timer PreScaler. select TCNT count clock */
#define TCR_TPSC_PCK4   	0x0000 /* use Pck/4 */
#define TCR_TPSC_PCK16  	0x0001 /* use Pck/16 */
#define TCR_TPSC_PCK64  	0x0002 /* use Pck/64 */
#define TCR_TPSC_PCK256 	0x0003 /* use Pck/256 */
#define TCR_TPSC_PCK1024	0x0004 /* use Pck/1024 */
/*#define TCR_TPSC_NEVER	0x0005 */ /* NEVER SET THIS */
#define TCR_TPSC_RTCCLK 	0x0006 /* use RTCCLK (RTC output clock */
#define TCR_TPSC_EXTCLK 	0x0007 /* use TCLK (External Clock). NEVER SET CH[345] */

#define TSTR_STR_BIT0       1
#define TSTR_STR_BIT1       2
#define TSTR_STR_BIT2       4
#define TSTR_STR_BIT3       1
#define TSTR_STR_BIT4       2
#define TSTR_STR_BIT5       4

#define TMU0_INTEVT         0x400 /* TMU-ch0 timer underflow interrupt */
#define TMU1_INTEVT         0x420 /* TMU-ch1 timer underflow interrupt */
#define TMU2_INTEVT         0x440 /* TMU-ch2 timer underflow interrupt */
/*#define TMU_TICPI2_INTEVT   0x600 *//* TMU-ch2 timer input capture interrupt */
#define TMU3_INTEVT         0xb00 /* TMU-ch3 timer underflow interrupt */
#define TMU4_INTEVT         0xb80 /* TMU-ch4 timer underflow interrupt */
#define TMU5_INTEVT         0xE40 /* TMU-ch5 timer underflow interrupt */

/* for ch-5 */
#define TSTR_STR_BIT        TSTR_STR_BIT5
#define TMU_INTEVT          TMU5_INTEVT 

/* Interrupt Level */
#define TINTLVL        0xd
#define IPRA    REGI(0xffd40000)
//#define IPRO    REGI(0xffd40004)
#define	IPRB	REGI(0xffd40004)


#define IPRA_TMU0(n) (IPRA = ((IPRA & 0x00ffffff) | (n << 24)))
#define IPRA_TMU1(n) (IPRA = ((IPRA & 0xff00ffff) | (n << 16)))
#define IPRA_TMU2(n) (IPRA = ((IPRA & 0xffff00ff) | (n << 8)))
#define IPRA_TMU3(n) (IPRO = ((IPRO & 0x00ffffff) | (n << 24)))//(IPRB = ((IPRB & 0xffffffff) | (n << 4)))
#define IPRA_TMU4(n) (IPRO = ((IPRO & 0xff00ffff) | (n << 16)))//(IPRB = ((IPRB & 0xff00ffff) | (n )))
#define IPRA_TMU5(n) (IPRB = ((IPRB & 0xffff00ff) | (n << 8)))

/* Interrupt Mask */
#define IMSKC   REGI(0xfed40080)
#define IMSK    REGI(0xfed40084)
#define IMSK_TMU012 0x00000001
#define IMSK_TMU345 0x00000002
//#define INTORG_TMUCH345_BIT	1

/* for ch-5 */
#define IMSK_TMU    IMSK_TMU345


//spm_vcpu *tmu_interval(spm_intr_id iid);
void tmu_mask(void)
{
    IMSK |= IMSK_TMU;
}

void tmu_clr(void)
{
 IMSKC |= IMSK_TMU;
}
