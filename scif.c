#include "scif.h"
int xv6_scif_init=0;
void scif_init(void)
{
  xv6_scif_init = 1;

  SCSCR = 0; /* disable everything */

  SCFCR = FCR_RFRST | FCR_TFRST |    /* clear FIFO */
    FCR_TTRG_1 | FCR_RTRG_1; /* set FIFO trigger */

  SCSMR = 0; /* 8 bits data, No parity, 1 stop bit */

  SCBRR = SCIF_BPS_115200; /* set baudrate to 115200bps */

  //SCFCR = FCR_TTRG_1 | FCR_RTRG_1;
  SCFCR = 0;
  IPRC_SCIF1(0xa);

  SCSCR = SCR_RIE | SCR_TE | SCR_RE; /* enable transmit */

}

unsigned char scif_putc(unsigned char c)
{
  while(!(SCFSR & FSR_TDFE));
  SCFTDR = c;
  SCFSR &= ~FSR_TDFE;
  return c;
}

/* interrupt handler */
scif1_get(void)
{
  unsigned char c;
  unsigned int s;
  s = SCFDR & 0xFF;
  if (!s)
    return -1;

  c = SCFRDR;
  //cprintf("%c\n",c);
  //SCFSR &= ~(FSR_RDF | FSR_DR);
  return c;
}
void do_scif1_read(void)
{
  consoleintr(scif1_get);
  SCFSR &= ~(FSR_RDF | FSR_DR);
  return;
}

int putc(int c)
{
  scif_putc(c);
  if(c == '\n') {
    scif_putc('\r');
  }
  return c;
}

int putchar(int c)
{
  scif_putc(c);
  if(c == '\n') {
    scif_putc('\r');
  }
  return c;
}
