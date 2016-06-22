#ifndef _BSP_H_
#define _BSP_H_

#include "regops.h"
#include "cache.h"
#include "intc.h"
#include "mmu.h"
#include "tmu.h"
#include "cgu.h"
#include "wdt.h"
#include "udc.h"

struct pt_regs {
  unsigned long sr;
  unsigned long pc;
  unsigned long r0;
  unsigned long r1;
  unsigned long r2;
  unsigned long r3;
  unsigned long r4;
  unsigned long r5;
  unsigned long r6;
  unsigned long r7;
  unsigned long r8;
  unsigned long r9;
  unsigned long r10;
  unsigned long r11;
  unsigned long r12;
  unsigned long r13;
  unsigned long r14;
  unsigned long r15;
  unsigned long r16;
  unsigned long r17;
  unsigned long r18;
  unsigned long r19;
  unsigned long r20;
  unsigned long r21;
  unsigned long r22;
  unsigned long r23;
  unsigned long r24;
  unsigned long r25;
  unsigned long r26;
  unsigned long r27;
  unsigned long r28;
  unsigned long r29;
  unsigned long r30;
  unsigned long r31;
};

extern void get_pending_int_source_one();
extern void clear_ipr_highest();
extern void fail_();
extern void pass_();
extern int _stack;
extern void i_wcr_ccr(unsigned int);
#define i_rcr_ccr()  (REG32(CCR))
extern void i_wcr_mmucr(unsigned int);
#define i_rcr_mmucr()  (REG32(MMUCR))
#define setvbr(id,func) SetVectorTable(id,(unsigned int)func)
extern unsigned long _VVBR;
#ifndef ISA
extern int unexpected_error;
#define HAVE_UNEXPECTED_ERROR (unexpected_error!=0)
#else
#define HAVE_UNEXPECTED_ERROR 0
#endif

#define user2pri()    i_trap(5)

#define pri2user() \
({                                        \
  register unsigned long _dst_;                    \
  i_wcr_ssr(i_rcr_sr()&~0x8);             \
  __asm__ __volatile__ ("mova\t%0,1f\n\twcr\t%0,2\n\trte\n1:":"=r"(_dst_));  \
  i_nop;  \
})

#define RAM_SYS_DATA  __attribute__ ((__section__ (".sys_data")))
#define RAM_SYS_TEXT  __attribute__ ((__section__ (".sys_text")))
#define ROM_DATA  __attribute__ ((__section__ (".rodata")))

#define set_imask(imask)                        \
({ \
  REG32(IMR) |= imask;                          \
})

#define dis_interrupt_esr() (i_wcr_esr(i_rcr_esr()&~3))
#define dis_interrupt_fsr() (i_wcr_fsr(i_rcr_fsr()&~3))

extern volatile unsigned int interrupt_type;
extern volatile unsigned int int_source;

#define FAIL 1
#define PASS 0

#define enb_interrupt()  			\
({						\
  unsigned long m;				\
  __asm__ __volatile__("rcr %0, 0":"=r"(m));	\
  m |= 0x1;					\
  __asm__ __volatile__("wcr %0, 0"::"r"(m));	\
})

#define dis_interrupt()    		        \
({						\
  unsigned long m;				\
  __asm__ __volatile__("rcr %0, 0":"=r"(m));	\
  m &= ~0x1;					\
  __asm__ __volatile__("wcr %0, 0"::"r"(m));	\
})

#define close_interrupt()			\
({						\
  unsigned long t = i_rcr_esr();		\
  t = t&0xfffe;					\
  i_wcr_esr(t);					\
})

#define printf sio_printf

#define USB_PULLUP_GPIO() {REG32(0x9803F500+16) &= 0xffff0fff;}
#define USB_PULLUP_OUT() {REG32(0x9803F500+4) |= 0xc00000;}
#define USB_PULLUP_LOW() {REG32(0x9803F500) &= 0xff3fffff;}
#define USB_PULLUP_HIGH() {REG32(0x9803F500) |= 0xc00000;}

#define enable_download_flag() ((void (*)())0x80001800)()
#define USB_EP0_SIZE 32

#define ZI1222_FOR_UDISK 1


//pierce.hu add this for udisk test
#define ZI1222_PIERCEHU		1
//#define ZI1222_MI_ENABLE	1
#define enable_boot(x,y,z) ((void (*)())(0x80000800))(x,y,z)

#endif



