#ifndef __CCORE_REGOPS_H__
#define __CCORE_REGOPS_H__

/* Define macros for access memory mapped ccore register */
#define REG32(addr)     (*((unsigned long  volatile *)(addr)))
#define REG16(addr)     (*((unsigned short volatile *)(addr)))
#define REG8(addr)      (*((unsigned char  volatile *)(addr)))
#define reg32(addr)     (*((unsigned long  volatile *)(addr)))
#define reg16(addr)     (*((unsigned short volatile *)(addr)))
#define reg8(addr)      (*((unsigned char  volatile *)(addr)))
#endif /* __CCORE_REGOPS_H__ */
