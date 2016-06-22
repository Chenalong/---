#ifndef __CCORE_INTC_H__
#define __CCORE_INTC_H__

#include <regops.h>
#include <config.h>
/* define types of default vbr table */
#define TP_IU      0
#define TP_EXP     0x100
#define TP_INT     0x200

/* EE FE position in SR */
#define SR_BK_MASK              0x1
#define SR_EE_MASK              0x2
#define SR_DE_MASK              0x4
#define SR_SM_MASK              0x8
#define SR_DM_MASK              0x10
#define SR_VB_MASK              0xe0
#define SR_CAUSE_MASK           0x300

#define SR_BK_SFT               0
#define SR_EE_SFT               1
#define SR_DE_SFT               2
#define SR_SM_SFT               3
#define SR_DM_SFT               4
#define SR_VB_SFT               5
#define SR_CAUSE_SFT            8

/* used as vector table index */
#define EXP_RESET	0x0
#define EXP_ILLINS	0x1
#define EXP_DEBUG	0x2
#define EXP_TRAP	0x5
#define EXP_TLBFAULT	0x6
#define EXP_INT	        0x4

/* Define all intc mapped registers */
#define ISR        0x1803F000
#define IMR        0x1803F004
#define IMSR       0x1803F008
#define IMCR       0x1803F00C
#define IPR        0x1803F010
#define IFLAGR     0x1803F014

// for arca2sc1 only

#define INTC_SEC     30
#define INTC_SCDRST  29
#define INTC_UDCRST  28
#define INTC_RSA     27
#define INTC_DES     26
#define INTC_RNG     25
#define INTC_TMU0    24
#define INTC_TMU1    23
#define INTC_GPIO18    19
#define INTC_GPIO23    18


#define INTC_UDC     12
#define INTC_SCD0    10
#define INTC_CPUCORE  6
#define INTC_PMC     7 /* just for compile */

#define IFLAG_DES    4
#define IFLAG_RSA    2
#define IFLAG_RNG    8
#define IFLAG_SEC    1
#define IFLAG_PPC    0x20
#define IFLAG_SPI    0x10
#define IFLAG_UART   0x40

#define INTC_SPI     7
#define INTC_UART    3
#define INTC_PPC     4
#define INTC_SCC1     8
#define INTC_SCC0     9

#endif /* __CCORE_INTC_H__ */







