/* the address of WDT control registers */
#include <config.h>
#define WTCNT   0x1803F404
#define WTCSR   0x1803F400

#define WTCNT_INIT   0x0
#define WTCSR_INIT   0x0

#define WTCSR_RSTS       0x8
#define WTCSR_START      0x10
