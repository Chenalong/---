#ifndef __CCORE_CACHE_H__
#define __CCORE_CACHE_H__

#include <regops.h>
#include <config.h>
#define CCR_ICE         0x00000040
#define CCR_DCE         0x00000020
#define CCR_A3C         0x00000010
#define CCR_A3B         0x00000008
#define CCR_A2B         0x00000004
#define CCR_A1B         0x00000002
#define CCR_A0B         0x00000001

#define CCR_P2B         CCR_A2B
#define CCR_P1B         CCR_A1B
#define CCR_B           CCR_A0B
#define CCR_P3B         CCR_A3B
#define CCR_P3C         CCR_A3C

#define CACHE_ENTRIES     32
#define CACHE_SETS        16  // the same as arca2c6
#define CACHE_LINE_WORDS  8
#define ENTRY_OFFSET      0x200  //the lest offset between contiguous entry within the same set

#ifdef CACHE_2K
#define SET_SIZE 4
#else
#define SET_SIZE 32
#endif

#define CMD_CACHE_INVILIDATE  0xf
#define CMD_CACHE_DISCARD     0x1
#define CMD_CACHE_PREFETCH    0x0
#define CMD_CACHE_PLOCK       0x7
#define CMD_CACHE_UNLOCK      0xe
#define CMD_DCACHE_FLUSH      0x3
#define CMD_DCACHE_WB_UNREADABLE   0x2
#define CMD_DCACHE_ALLOCATE   0x4
#define CMD_DCACHE_FLUSH_BUF  0x9
#define CMD_DCACHE_ALOCK      0x6
#define CMD_DCACHE_PREFETCH    0x0

#define i_icache_discard(va,offset) i_icache(CMD_CACHE_DISCARD,va,offset)
#define i_icache_invalidate()                 \
({ unsigned int r0=0;                           \
   i_icache(CMD_CACHE_INVILIDATE,r0,0);       \
})

#define i_icache_prefetch(va,offset) i_icache(CMD_CACHE_PREFETCH, va, offset)
#define i_icache_plock(va,offset) i_icache(CMD_CACHE_PLOCK, va, offset)
#define i_icache_unlock()                     \
({ unsigned int r0=0;                           \
   i_icache(CMD_CACHE_UNLOCK, r0, 0);         \
})

#define i_dcache_prefetch(va,offset) i_dcache(CMD_CACHE_PREFETCH,va,offset)
#define i_dcache_discard(va,offset) i_dcache(CMD_CACHE_DISCARD,va,offset)
#define i_dcache_invalidate()                 \
({ unsigned int r0=0;                           \
   i_dcache(CMD_CACHE_INVILIDATE,r0,0);       \
})

#define i_dcache_flush(va,offset) i_dcache(CMD_DCACHE_FLUSH,va,offset)
#define i_dcache_wb_clean(va,offset) i_dcache(CMD_DCACHE_WB_UNREADABLE,va,offset)
#define i_dcache_allocate(va,offset) i_dcache(CMD_DCACHE_ALLOCATE,va,offset)
#define i_dcache_flush_buf()                  \
({ unsigned int r0=0;                           \
   i_dcache(CMD_DCACHE_FLUSH_BUF,r0,0);       \
})

#define i_dcache_plock(va,offset) i_dcache(CMD_CACHE_PLOCK, va, offset)
#define i_dcache_alock(va,offset) i_dcache(CMD_DCACHE_ALOCK, va, offset)
#define i_dcache_unlock()                     \
({ unsigned int r0=0;                           \
   i_dcache(CMD_CACHE_UNLOCK, r0, 0);         \
})

#define cache_px_wb()  (i_cst_ccr(i_cld_ccr() | CCR_B))
#define cache_px_wt()  (i_cst_ccr(i_cld_ccr() & ~CCR_B))
#define cache_p1_wb()  (i_cst_ccr(i_cld_ccr() | CCR_P1B))
#define cache_p1_wt()  (i_cst_ccr(i_cld_ccr() & ~CCR_P1B))

#define cache_p2_buf()  (i_cst_ccr(i_cld_ccr() | CCR_P2B))
#define cache_p2_nbuf() (i_cst_ccr(i_cld_ccr() & ~CCR_P2B))

#define cache_p3_wb()   (i_cst_ccr(i_cld_ccr() | CCR_P3B))
#define cache_p3_wt()   (i_cst_ccr(i_cld_ccr() & ~CCR_P3B))
#define cache_p3_c()    (i_cst_ccr(i_cld_ccr() | CCR_P3C))
#define cache_p3_nc()   (i_cst_ccr(i_cld_ccr() & ~CCR_P3C))

#define cache_wb()  (i_cst_ccr(i_cld_ccr() | CCR_B | CCR_P1B))
#define cache_wt()  (i_cst_ccr(i_cld_ccr() & ~CCR_B & ~CCR_P1B))

#define write_alias_dtlb(vaddr1, vaddr2, readable, writable)	\
({							\
  unsigned int _pa;					\
  _pa = va_2_pa(vaddr1);				\
  set_ced(_pa,READABLE, writable, UNEXECUTABLE, SIZE);	\
  i_dtlb_write(vaddr1);					\
  i_dtlb_write(vaddr2);					\
})

#define protect_data_in_dcache()				\
({								\
  register unsigned int r1 asm ("r1");			        \
  int addr;							\
  for ( addr = r1; addr < ((unsigned int)&_stack + 4); addr = addr + 32)	\
  i_dcache_flush(addr,0);					\
  i_dcache_flush(i_rcr_vbr(),0);				\
})

#endif  /* __CCORE_CACHE_H__ */
