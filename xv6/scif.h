#define REGB(x) (*((volatile unsigned char *)(x)))
#define REGS(x) (*((volatile unsigned short *)(x)))
#define REGI(x) (*((volatile unsigned int *)(x)))
extern unsigned char scif_putc(unsigned char c);
extern void scif_init(void);
/* 
 * Pck=33.333MHz
 * N = Pck / (32 * bps) * 10^6 - 1
 */
#define SCIF_BPS_115200 8
#define SCIF_BPS_19200  54
#define SCIF_BPS_9600   108
#define SCIF_BASE   0xFFE80000


#define SCSMR       REGS(SCIF_BASE + 0x0)  /* Serial Mode Register                 */
#define SCBRR       REGB(SCIF_BASE + 0x4)  /* Bit Rate Register                    */
#define SCSCR       REGS(SCIF_BASE + 0x8)  /* Serial Control Register              */
#define SCFTDR      REGB(SCIF_BASE + 0xC)  /* Fifo Transmit Data Regiser         ? */
#define SCFSR       REGS(SCIF_BASE + 0x10) /* Serial Status Register             ? */
#define SCFRDR      REGB(SCIF_BASE + 0x14) /* Fifo Receive Data Register         ? */
#define SCFCR       REGS(SCIF_BASE + 0x18) /* Fifo Control Register                */
//#define SCFDR       REGS(SCIF_BASE + 0x1C) /* Fifo Data-count Register             */
#define SCSPTR      REGS(SCIF_BASE + 0x1C) /* Serial PorT Register                 */
//#define SCLSR       REGS(SCIF_BASE + 0x24) /* Line Status Register                 */

/* SCSMR bit masks */
#define SMR_CKS         0x0003 /* ClocK Select       */
#define SMR_CKS_CLK     0x0000 /*   CLK              */
#define SMR_CKS_CLK4    0x0001 /*   CLK / 4          */
#define SMR_CKS_CLK16   0x0002 /*   CLK / 32         */
#define SMR_CKS_CLK64   0x0003 /*   CLK / 64         */
#define SMR_STOP        0x0008 /* STOP bit length    */
#define SMR_OE          0x0010 /* parity mode        */
#define SMR_PE          0x0020 /* Parity Enable      */
#define SMR_CHR         0x0040 /* CHaRacter Length   */

/* SCSCR bit masks */
#define SCR_CKE         0x0003 /* CLock Enable                  */
#define SCR_CKE_PORT    0x0000 /*   port                        */
#define SCR_CKE_IN      0x0001 /*   internal clock              */
#define SCR_CKE_EX      0x0002 /*   external clock              */
#define SCR_REIE        0x0008 /* Receive Error Interrupt Enable*/
#define SCR_RE          0x0010 /* Receive Enable                */
#define SCR_TE          0x0020 /* Transmit Enable               */
#define SCR_RIE         0x0040 /* Receive Interrupt Enable      */
#define SCR_TIE         0x0080 /* Transmit Interrupt Enable     */

/* SCFSR bit masks */
#define FSR_DR      0x0001 /* receive Data Ready          */
#define FSR_RDF     0x0002 /* Receive fifo Data Full      */
#define FSR_PER     0x0004 /* Parity ERror                */
#define FSR_FER     0x0008 /* Framing ERror               */
#define FSR_BRK     0x0010 /* BReaK detection             */
#define FSR_TDFE    0x0020 /* Transmit Data Fifo Empty    */
#define FSR_TEND    0x0040 /* Transmit END                */
#define FSR_ER      0x0080 /* receive ERror               */
#define FSR_FER_NUM 0x0F00 /* NUMber of Framing ERrors    */
#define FSR_PER_NUM 0xF000 /* NUMber of Parity ERrors     */

/* SCFCR bit masks */
#define FCR_LOOP        0x0001 /* LOOP back test                    */
#define FCR_RFRST       0x0002 /* Receive Fifo data-number ReSeT    */
#define FCR_TFRST       0x0004 /* Transmit Fifo data-number ReSeT   */
#define FCR_MCE         0x0008 /* Modem Control Enable              */
#define FCR_TTRG        0x0030 /* Transmit fifo data-number TRiGger */
#define FCR_TTRG_8      0x0000
#define FCR_TTRG_4      0x0010
#define FCR_TTRG_2      0x0020
#define FCR_TTRG_1      0x0030
#define FCR_RTRG        0x00c0 /* Receive fifo data-number TRiGger  */
#define FCR_RTRG_1      0x0000
#define FCR_RTRG_4      0x0040
#define FCR_RTRG_8      0x0080
#define FCR_RTRG_14     0x00c0
#define FCR_RSTRG       0x0700 /* SCIF0_RTS output TRiGger          */
#define FCR_RSTRG_15    0x0000
#define FCR_RSTRG_1     0x0100
#define FCR_RSTRG_4     0x0200
#define FCR_RSTRG_6     0x0300
#define FCR_RSTRG_8     0x0400
#define FCR_RSTRG_10    0x0500
#define FCR_RSTRG_12    0x0600
#define FCR_RSTRG_14    0x0700

/* SCSPTR bit masks */
#define SPTR_SPB2DT     0x0001
#define SPTR_SPB2IO     0x0002
#define SPTR_SCKDT      0x0004
#define SPTR_SCKIO      0x0008
#define SPTR_CTSDT      0x0010
#define SPTR_CTSIO      0x0020
#define SPTR_RTSDT      0x0040
#define SPTR_RTSIO      0x0080


#define IPRC	REGI(0xffd0000c)

#define IPRC_SCIF0(n) (IPRC = ((IPRC & 0x00ffffff) | (n << 24)))
#define IPRC_SCIF1(n) (IPRC = ((IPRC & 0xffffff0f) | (n << 4)))
