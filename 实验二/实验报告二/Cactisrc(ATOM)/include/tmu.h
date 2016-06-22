#ifndef __CCORE_TMU_H__
#define __CCORE_TMU_H__

#include <regops.h>
#include <config.h>
/* Define all TMU memory mapped registers */
#define TER             0x1803F200  /* Initial Value 0x00 */
#define TRDR0           0x1803F204  /* Initial Value 0xFFFFFFFF */
#define TCNT0           0x1803F208  /* Initial Value 0xFFFFFFFF */
#define TCSR0           0x1803F20C  /* Initial Value 0x0000 */
#define TRDR1           0x1803F210  /* Initial Value 0xFFFFFFFF */
#define TCNT1           0x1803F214  /* Initial Value 0xFFFFFFFF */
#define TCSR1           0x1803F218  /*  Value 0x0000 */
#define TCRB0           0x1803F228  /* Read-only Initial Value 0x0000 */
#define TCRB1           0x1803F22C  /* Read-only Initial Value 0x0000 */


/* Define tclk driver memory mapped register */
//#define TMU_TCLK        0xa012F000

/*
 * Define macros for TER
 * Timer Enable Register
 */
#define TER_TE0         0x01    /* Starts channel 0 count */
#define TER_TE1         0x02    /* Starts channel 1 count */
#define TER_TE2         0x04    /* Starts channel 2 count */




/*   TCSR clock define */
#define  TMU_DCLK_4      0x0
#define  TMU_DCLK_16     0x1
#define  TMU_DCLK_64     0x2
#define  TMU_DCLK_256    0x3
#define  TMU_RTCCLK      0x4
#define  TMU_EXTRAL      0x5
#define  TMU_EXTAL       0x5

#define  TMU_PCLK_4      0x0
#define  TMU_PCLK_16     0x1
#define  TMU_PCLK_64     0x2
#define  TMU_PCLK_256    0x3

#define TCSR_UIE        0x0020  /* Underflow interrupt is enabled */
#define TCSR_UF         0x0040  /* TCNT underflow has occurred */
#define TCSR_BUSY       0x0080  /* TCNT underflow has occurred */

#endif  /* __CCORE_TMU_H__ */
