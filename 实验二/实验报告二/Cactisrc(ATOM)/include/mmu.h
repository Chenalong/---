#ifndef __CCORE_MMU_H__
#define __CCORE_MMU_H__
#include <regops.h>
#include <config.h>
#define READABLE          1
#define UNREADABLE        0
#define WRITABLE          1
#define UNWRITABLE        0
#define EXECUTABLE        1
#define UNEXECUTABLE      0

#if  (TLB_SIZE==64)
#define SIZE 0
#endif

#if  (TLB_SIZE==256)
#define SIZE 1
#endif

#if  (TLB_SIZE==1000)
#define SIZE 2
#endif

#if  (TLB_SIZE==4000)
#define SIZE 3
#endif

#define ASID      0x5   
#define TLB_ENTRY 32

#define MCR_CAUSE             0xfe000000
#define MCR_ADV               0x00000002
#define MCR_ATE               0x00000001
#define MCR_CAUSE_SHIFT       25
#define MCR_ADV_SHIFT         1
#define MCR_ATE_SHIFT         0

#define MCR_SG_MODE           0x00000000
#define MCR_PM_MODE           0x00000001
#define MCR_AD_MODE           0x00000003
/************************************************/
/*
#define MCR_CAUSE_NONE        0x0
#define MCR_CAUSE_IMISS       0x44
#define MCR_CAUSE_IMISS_ICACHE 0x45
#define MCR_CAUSE_IADDR_FET   0x24
#define MCR_CAUSE_IADDR_INS   0x25   
#define MCR_CAUSE_ICONFIG     0x05
#define MCR_CAUSE_DMISS_RD    0x40
#define MCR_CAUSE_DMISS_WR    0x42
#define MCR_CAUSE_DMISS_INS   0x41
#define MCR_CAUSE_DADDR_RD    0x20
#define MCR_CAUSE_DADDR_WR    0x22
#define MCR_CAUSE_DADDR_INS   0x21
#define MCR_CAUSE_DINIT_WR    0x7f
#define MCR_CAUSE_DCONFIG     0x01
#define MCR_CAUSE_DADDR_TAG_RD 0x10
#define MCR_CAUSE_DADDR_TAG_WR 0x12

*/
/*************************************************/

#define MCR_CAUSE_NONE        0x0
#define MCR_CAUSE_IMISS       0x44
#define MCR_CAUSE_IMISS_ICACHE 0x45
#define MCR_CAUSE_DMISS_RD    0x40
#define MCR_CAUSE_DMISS_WR    0x42
#define MCR_CAUSE_DMISS_INS   0x41
#define MCR_CAUSE_IADDR_FET   0x24   
#define MCR_CAUSE_IADDR_INS   0x25   
#define MCR_CAUSE_DADDR_RD    0x20
#define MCR_CAUSE_DADDR_WR    0x22
#define MCR_CAUSE_DADDR_INS   0x21
#define MCR_CAUSE_IPSPV_FET   0x14   
#define MCR_CAUSE_IPSPV_INS   0x15   
#define MCR_CAUSE_DPSPV_RD    0x10
#define MCR_CAUSE_DPSPV_WR    0x12
#define MCR_CAUSE_DPSPV_INS   0x11
#define MCR_CAUSE_ITLBPV_FET   0x0c   
#define MCR_CAUSE_ITLBPV_INS   0x0d   
#define MCR_CAUSE_DTLBPV_RD    0x08
#define MCR_CAUSE_DTLBPV_WR    0x0a
#define MCR_CAUSE_DTLBPV_INS   0x09
#define MCR_CAUSE_ICONFIG     0x05
#define MCR_CAUSE_DCONFIG     0x01

#define EXP_CAUSE_IMISS       0x0
#define EXP_CAUSE_IMISS_ICACHE 0x1
#define EXP_CAUSE_DMISS_RD    0x2
#define EXP_CAUSE_DMISS_WR    0x3
#define EXP_CAUSE_DMISS_INS   0x4
#define EXP_CAUSE_IADDR_FET   0x5   
#define EXP_CAUSE_IADDR_INS   0x6   
#define EXP_CAUSE_DADDR_RD    0x7
#define EXP_CAUSE_DADDR_WR    0x8
#define EXP_CAUSE_DADDR_INS   0x9
#define EXP_CAUSE_IPSPV_FET   0xa   
#define EXP_CAUSE_IPSPV_INS   0xb   
#define EXP_CAUSE_DPSPV_RD    0xc
#define EXP_CAUSE_DPSPV_WR    0xd
#define EXP_CAUSE_DPSPV_INS   0xe
#define EXP_CAUSE_ITLBPV_FET   0xf   
#define EXP_CAUSE_ITLBPV_INS   0x10   
#define EXP_CAUSE_DTLBPV_RD    0x11
#define EXP_CAUSE_DTLBPV_WR    0x12
#define EXP_CAUSE_DTLBPV_INS   0x13
#define EXP_CAUSE_ICONFIG     0x14
#define EXP_CAUSE_DCONFIG     0x15
/*
#define EXP_CAUSE_IMISS       0x0
#define EXP_CAUSE_IADDR_FET   0x1
#define EXP_CAUSE_IADDR_INS   0x2  
#define EXP_CAUSE_ICONFIG     0x3
#define EXP_CAUSE_DMISS_RD    0x4
#define EXP_CAUSE_DMISS_WR    0x5
#define EXP_CAUSE_DMISS_INS   0x6
#define EXP_CAUSE_DADDR_RD    0x7
#define EXP_CAUSE_DADDR_WR    0x8
#define EXP_CAUSE_DADDR_INS   0x9
#define EXP_CAUSE_DINIT_WR    0xb
#define EXP_CAUSE_DCONFIG     0xc
#define EXP_CAUSE_IMISS_ICACHE       0xd
*/
#define EXP_TLBMISS_I_Fet 0x44
#define EXP_TLBMISS_ICache_Op 0x45
#define EXP_TLBMISS_D_Load 0x40
#define EXP_TLBMISS_D_Store 0x42
#define EXP_TLBMISS_DCache_Op 0x41
#define EXP_ADDRERR_I_Fet	0x24
#define EXP_ADDRERR_ICache_Op	0x25
#define EXP_ADDRERR_D_Load	0x20
#define EXP_ADDRERR_D_Store	0x22
#define EXP_ADDRERR_DCache_Op	0x21
#define EXP_PRVIOLATE_I_Fet	0x14
#define EXP_PRVIOLATE_ICache_Op	0x15
#define EXP_PRVIOLATE_D_Load	0x10
#define EXP_PRVIOLATE_D_Store	0x12
#define EXP_PRVIOLATE_DCache_Op	0x11
#define EXP_TLBVIOLATE_DCache_Op	0xc
#define EXP_TLBVIOLATE_ICache_Op	0xd
#define EXP_TLBVIOLATE_D_Load	0x8
#define EXP_TLBVIOLATE_D_Store	0xa
#define EXP_TLBVIOLATE_DCache_Op	0x9
#define EXP_ILLEGAL_ICACHE_ConfOP	0x5
#define EXP_ILLEGAL_DCACHEDTLB_ConfOp	0x1

#define ID_MMU                0x0
 
#define CR_MCR                0x0
#define CR_TTB                0x1
#define CR_ASI                0x3
#define CR_MEA                0x2
#define CR_CED                0x4
#define CR_CCR                0x5
#define CR_CID                0x7

#define CR_PCR		      0x10
#define CR_MDPR		      0x11
#define CR_ODPR		      0x12

#define CR_PSPR0	      0x18
#define CR_PSPR1              0x19
#define CR_PSPR2              0x1a
#define CR_PSPR3              0x1b
#define CR_PAPR0              0x1c
#define CR_PAPR1              0x1d
#define CR_PAPR2              0x1e
#define CR_PAPR3              0x1f

#define CMD_TLB_INVALIDATE    0xf
#define CMD_TLB_READ          0x2
#define CMD_TLB_WRITE         0x3
#define CMD_TLB_DISCARD       0x1
#define CMD_TLB_LOCK          0x7
#define CMD_TLB_UNLOCK        0xe

#define ASI_ASID              0x0000000f
#define CMD_TLB_DISCARD       0x1
#define CMD_TLB_LOCK          0x7
#define CMD_TLB_UNLOCK        0xe

#define ASI_ASID              0x0000000f
#define ASI_ASID_SHIFT        0

#define CED_PPN               0x000fffc0
#define CED_SIZE              0x00000030
#define CED_MISS              0x00000001
#define CED_PPN_SHIFT         6
#define CED_SIZE_SHIFT        4
#define CED_MISS_SHIFT        0      
/*
             #define CED_READ             0x00000008
             #define CED_WRITE            0x00000004
             #define CED_EXECUTE             0x00000002
             #define CED_READ_SHIFT       3     
             #define CED_WRITE_SHIFT      2     
             #define CED_EXECUTE_SHIFT       1      
*/
#define CED_READ              0x00000008
#define CED_WRITE             0x00000004
#define CED_EXECUTE           0x00000002
#define CED_READ_SHIFT        3     
#define CED_WRITE_SHIFT       2     
#define CED_EXECUTE_SHIFT     1      

#define PSPR_PPN              0xfffc0000
#define PSPR_LEN              0x0000fff0
#define PSPR_R                0x00000008
#define PSPR_W                0x00000004
#define PSPR_E                0x00000002
#define PSPR_PPN_SHIFT        18
#define PSPR_LEN_SHIFT        4
#define PSPR_R_SHIFT          3
#define PSPR_W_SHIFT          2
#define PSPR_E_SHIFT          1

#define PAPR_PPN              0xfffc0000
#define PAPR_MSK              0x000000f0
#define PAPR_R                0x00000008
#define PAPR_W                0x00000004
#define PAPR_E                0x00000002
#define PAPR_V                0x00000001
#define PAPR_PPN_SHIFT        18
#define PAPR_MSK_SHIFT        4
#define PAPR_R_SHIFT          3
#define PAPR_W_SHIFT          2
#define PAPR_E_SHIFT          1
#define PAPR_V_SHIFT          0

#define PAPR_AREA_64          0x40
#define PAPR_AREA_128         0x80
#define PAPR_AREA_256         0x100
#define PAPR_AREA_512         0x200
#define PAPR_AREA_1K          0x400
#define PAPR_AREA_2K          0x800
#define PAPR_AREA_4K          0x1000
#define PAPR_AREA_8K          0x2000
#define PAPR_AREA_16K         0x4000
#define PAPR_AREA_32K         0x8000
#define PAPR_AREA_64K         0x10000
#define PAPR_AREA_128K        0x20000
#define PAPR_AREA_256K        0x40000
#define PAPR_AREA_512K        0x80000

#define i_cld_mcr() i_cld(ID_MMU,CR_MCR)
#define i_cld_ttb() i_cld(ID_MMU,CR_TTB)
#define i_cld_mea() i_cld(ID_MMU,CR_MEA)
#define i_cld_asi() i_cld(ID_MMU,CR_ASI)
#define i_cld_ced() i_cld(ID_MMU,CR_CED)
#define i_cld_ccr() i_cld(ID_MMU,CR_CCR)

#define i_cld_pcr()   i_cld(ID_MMU,CR_PCR)
#define i_cld_mdpr()  i_cld(ID_MMU,CR_MDPR)
#define i_cld_odpr()  i_cld(ID_MMU,CR_ODPR)

#define i_cld_pspr0() i_cld(ID_MMU,CR_PSPR0)
#define i_cld_pspr1() i_cld(ID_MMU,CR_PSPR1)
#define i_cld_pspr2() i_cld(ID_MMU,CR_PSPR2)
#define i_cld_pspr3() i_cld(ID_MMU,CR_PSPR3)
#define i_cld_papr0() i_cld(ID_MMU,CR_PAPR0)
#define i_cld_papr1() i_cld(ID_MMU,CR_PAPR1)
#define i_cld_papr2() i_cld(ID_MMU,CR_PAPR2)
#define i_cld_papr3() i_cld(ID_MMU,CR_PAPR3)

#define i_cst_mcr(val) i_cst(val,ID_MMU,CR_MCR)
#define i_cst_ttb(val) i_cst(val,ID_MMU,CR_TTB)
#define i_cst_mea(val) i_cst(val,ID_MMU,CR_MEA)
#define i_cst_asi(val) i_cst(val,ID_MMU,CR_ASI)
#define i_cst_ced(val) i_cst(val,ID_MMU,CR_CED)
#define i_cst_ccr(val) i_cst(val,ID_MMU,CR_CCR)

#define i_cst_pcr(val)  i_cst(val,ID_MMU,CR_PCR)
#define i_cst_mdpr(val) i_cst(val,ID_MMU,CR_MDPR)
#define i_cst_odpr(val) i_cst(val,ID_MMU,CR_ODPR)

#define i_cst_pspr0(val) i_cst(val,ID_MMU,CR_PSPR0)
#define i_cst_pspr1(val) i_cst(val,ID_MMU,CR_PSPR1)
#define i_cst_pspr2(val) i_cst(val,ID_MMU,CR_PSPR2)
#define i_cst_pspr3(val) i_cst(val,ID_MMU,CR_PSPR3)
#define i_cst_papr0(val) i_cst(val,ID_MMU,CR_PAPR0)
#define i_cst_papr1(val) i_cst(val,ID_MMU,CR_PAPR1)
#define i_cst_papr2(val) i_cst(val,ID_MMU,CR_PAPR2)
#define i_cst_papr3(val) i_cst(val,ID_MMU,CR_PAPR3)

#define i_itlb_invalidate()  i_dtlb(0,CMD_TLB_INVALIDATE)
#define i_itlb_discard(va) i_dtlb(va,CMD_TLB_DISCARD)
#define i_itlb_read(va) i_dtlb(va,CMD_TLB_READ)
#define i_itlb_write(va) i_dtlb(va,CMD_TLB_WRITE)
#define i_itlb_lock(va) i_dtlb(va, CMD_TLB_LOCK)
#define i_itlb_unlock() i_dtlb(0, CMD_TLB_UNLOCK)
#define i_dtlb_invalidate() i_dtlb(0,CMD_TLB_INVALIDATE)
#define i_dtlb_discard(va) i_dtlb(va,CMD_TLB_DISCARD)
#define i_dtlb_read(va) i_dtlb(va,CMD_TLB_READ)
#define i_dtlb_write(va) i_dtlb(va,CMD_TLB_WRITE)
#define i_dtlb_lock(va) i_dtlb(va, CMD_TLB_LOCK)
#define i_dtlb_unlock() i_dtlb(0, CMD_TLB_UNLOCK)

#define primary_2_advanced() (i_cst_mcr(i_cld_mcr() | MCR_ADV))

#define mmu_en()                                 \
({                                               \
  i_cst_mcr(i_cld_mcr() | MCR_ATE);              \
  i_cst_asi(ASID << ASI_ASID_SHIFT);             \
})

#define mmu_ate() i_cst_mcr(i_cld_mcr() | MCR_ATE)
#define mmu_dis() i_cst_mcr(i_cld_mcr() & ~MCR_ATE)

/**************************************************************/
/************************** ced macro *************************/
/**************************************************************/

#define unreadable_2_readable()   \
({                                \
    unsigned int va;              \
    va = i_cld_mea();             \
    i_dtlb_read(va);              \
    i_cst_ced(i_cld_ced() | (READABLE << CED_READ_SHIFT));  \
    i_dtlb_write(va);             \
})

#define unwritable_2_writable()   \
({                                \
    unsigned int va;              \
    va = i_cld_mea();             \
    i_dtlb_read(va);              \
    i_cst_ced(i_cld_ced() | (WRITABLE << CED_WRITE_SHIFT));  \
    i_dtlb_write(va);             \
})

#define unexecutable_2_executable()           \
({                                \
    unsigned int va;              \
    va = i_cld_mea();             \
    i_dtlb_read(va);              \
    i_cst_ced(i_cld_ced() | (EXECUTABLE << CED_EXECUTE_SHIFT));  \
    i_dtlb_write(va);             \
})

#define set_ced(pa,readable,writable,executable,size)  \
    i_cst_ced( ((pa) & CED_PPN)                    |   \
               ((readable) << CED_READ_SHIFT)      |   \
               ((writable) << CED_WRITE_SHIFT)     |   \
               ((executable) << CED_EXECUTE_SHIFT) |   \
               ((size) << CED_SIZE_SHIFT))

/**************************************************************/
/************************* pspr macro *************************/
/**************************************************************/

#define disable_pspr(pspr_num)    \
({                                \
    unsigned int _val;            \
    val = read_pspr(pspr_num);    \
    val = val & (PSPR_LEN ^ 0xFFFFFFFF);       \
    set_pspr(val, pspr_num);          \
)}

#define read_pspr(pspr_num)                             \
({                                                      \
  unsigned int _val;					\
  switch(pspr_num)                                      \
    {                                                   \
      case 0: _val = i_cld_pspr0(); break;              \
      case 1: _val = i_cld_pspr1(); break;              \
      case 2: _val = i_cld_pspr2(); break;              \
      case 3: _val = i_cld_pspr3(); break;              \
      default: i_call(fail_); break;                    \
    }                                                   \
  _val;                                                 \
})

#define set_pspr(val, pspr_num)                         \
({                                                      \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(val); break;                  \
      case 1: i_cst_pspr1(val); break;                  \
      case 2: i_cst_pspr2(val); break;                  \
      case 3: i_cst_pspr3(val); break;                  \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define write_pspr(pspr_num,pa,len,readable,writable,executable) \
({                                                        \
    unsigned int _pa, _len, val;                          \
    if (pa >= 0x00100000) return 1;                       \
    _pa = pa >> 6;                                        \
    if (len >= 0x00040000) return 1;                      \
    _len = len >> 6;                                      \
    val = ((_pa << PSPR_PPN_SHIFT)                         \
            | (_len << PSPR_LEN_SHIFT)                     \
            | (readable << PSPR_R_SHIFT)                \
            | (writable << PSPR_W_SHIFT)               \
            | (executable << PSPR_E_SHIFT));         \
    set_pspr(val, pspr_num);                                  \
})

#define pspr_unread_2_read(pspr_num)   \
({                                     \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() | PSPR_R); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() | PSPR_R); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() | PSPR_R); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() | PSPR_R); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define pspr_unwrite_2_write(pspr_num)   \
({                                       \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() | PSPR_W); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() | PSPR_W); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() | PSPR_W); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() | PSPR_W); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define pspr_unexe_2_exe(pspr_num)   \
({                                   \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() | PSPR_E); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() | PSPR_E); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() | PSPR_E); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() | PSPR_E); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define pspr_read_2_unread(pspr_num)   \
({                                     \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() & ~PSPR_R); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() & ~PSPR_R); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() & ~PSPR_R); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() & ~PSPR_R); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define pspr_write_2_unwrite(pspr_num)   \
({                                       \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() & ~PSPR_W); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() & ~PSPR_W); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() & ~PSPR_W); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() & ~PSPR_W); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define pspr_exe_2_unexe(pspr_num)   \
({                                   \
  switch(pspr_num)                                      \
    {                                                   \
      case 0: i_cst_pspr0(i_cld_pspr0() & ~PSPR_E); break;     \
      case 1: i_cst_pspr1(i_cld_pspr1() & ~PSPR_E); break;     \
      case 2: i_cst_pspr2(i_cld_pspr2() & ~PSPR_E); break;     \
      case 3: i_cst_pspr3(i_cld_pspr3() & ~PSPR_E); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

/**************************************************************/
/************************* papr macro *************************/
/**************************************************************/

#define disable_papr(papr_num)    \
({                                \
    unsigned int _val;            \
    val = read_papr(papr_num);    \
    val = val & (PAPR_V ^ 0xFFFFFFFF); \
    set_papr(val, papr_num);           \
)}

#define enable_papr(papr_num)    \
({                               \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() | PAPR_V); break;     \
      case 1: i_cst_papr1(i_cld_papr1() | PAPR_V); break;     \
      case 2: i_cst_papr2(i_cld_papr2() | PAPR_V); break;     \
      case 3: i_cst_papr3(i_cld_papr3() | PAPR_V); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
)}

#define read_papr(pspr_num)                             \
({                                                      \
  unsigned int _val;					\
  switch(papr_num)                                      \
    {                                                   \
      case 0: _val = i_cld_papr0(); break;              \
      case 1: _val = i_cld_papr1(); break;              \
      case 2: _val = i_cld_papr2(); break;              \
      case 3: _val = i_cld_papr3(); break;              \
      default: i_call(fail_); break;                    \
    }                                                   \
  _val;                                                 \
})

#define set_papr(val, papr_num)                         \
({                                                      \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(val); break;                  \
      case 1: i_cst_papr1(val); break;                  \
      case 2: i_cst_papr2(val); break;                  \
      case 3: i_cst_papr3(val); break;                  \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define write_papr(papr_num,pa,area,readable,writable,executable) \
({                                                         \
    unsigned int _pa, _mask, val;                          \
    if (pa >= 0x00100000) return 1;                        \
    _pa = (pa & 0x000FFFC0) >> 6;                          \
    switch(area)                                           \
    {                                                      \
      case PAPR_AREA_64:     _mask = 0; break;             \
      case PAPR_AREA_128:    _mask = 1; break;             \
      case PAPR_AREA_256:    _mask = 2; break;             \
      case PAPR_AREA_512:    _mask = 3; break;             \
      case PAPR_AREA_1K:     _mask = 4; break;             \
      case PAPR_AREA_2K:     _mask = 5; break;             \
      case PAPR_AREA_4K:     _mask = 6; break;             \
      case PAPR_AREA_8K:     _mask = 7; break;             \
      case PAPR_AREA_16K:    _mask = 8; break;             \
      case PAPR_AREA_32K:    _mask = 9; break;             \
      case PAPR_AREA_64K:    _mask = 0xa; break;           \
      case PAPR_AREA_128K:   _mask = 0xb; break;           \
      case PAPR_AREA_256K:   _mask = 0xc; break;           \
      case PAPR_AREA_512K:   _mask = 0xd; break;           \
      default: i_call(fail_); break;                       \
    }                                                      \
    val = ((_pa << PAPR_PPN_SHIFT)                         \
            | (_mask << PAPR_MSK_SHIFT)                   \
            | (readable << PAPR_R_SHIFT)                \
            | (writable << PAPR_W_SHIFT)               \
            | (executable << PAPR_E_SHIFT)             \
            | 0x1);                                        \
    set_papr(val, papr_num);                               \
})

#define papr_unread_2_read(papr_num)   \
({                                     \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() | PAPR_R); break;     \
      case 1: i_cst_papr1(i_cld_papr1() | PAPR_R); break;     \
      case 2: i_cst_papr2(i_cld_papr2() | PAPR_R); break;     \
      case 3: i_cst_papr3(i_cld_papr3() | PAPR_R); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define papr_unwrite_2_write(papr_num)   \
({                                       \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() | PAPR_W); break;     \
      case 1: i_cst_papr1(i_cld_papr1() | PAPR_W); break;     \
      case 2: i_cst_papr2(i_cld_papr2() | PAPR_W); break;     \
      case 3: i_cst_papr3(i_cld_papr3() | PAPR_W); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define papr_unexe_2_exe(papr_num)   \
({                                   \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() | PAPR_E); break;     \
      case 1: i_cst_papr1(i_cld_papr1() | PAPR_E); break;     \
      case 2: i_cst_papr2(i_cld_papr2() | PAPR_E); break;     \
      case 3: i_cst_papr3(i_cld_papr3() | PAPR_E); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define papr_read_2_unread(papr_num)   \
({                                     \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() & ~PAPR_R); break;     \
      case 1: i_cst_papr1(i_cld_papr1() & ~PAPR_R); break;     \
      case 2: i_cst_papr2(i_cld_papr2() & ~PAPR_R); break;     \
      case 3: i_cst_papr3(i_cld_papr3() & ~PAPR_R); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define papr_write_2_unwrite(papr_num)   \
({                                       \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() & ~PAPR_W); break;     \
      case 1: i_cst_papr1(i_cld_papr1() & ~PAPR_W); break;     \
      case 2: i_cst_papr2(i_cld_papr2() & ~PAPR_W); break;     \
      case 3: i_cst_papr3(i_cld_papr3() & ~PAPR_W); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

#define papr_exe_2_unexe(papr_num)   \
({                                   \
  switch(papr_num)                                      \
    {                                                   \
      case 0: i_cst_papr0(i_cld_papr0() & ~PAPR_E); break;     \
      case 1: i_cst_papr1(i_cld_papr1() & ~PAPR_E); break;     \
      case 2: i_cst_papr2(i_cld_papr2() & ~PAPR_E); break;     \
      case 3: i_cst_papr3(i_cld_papr3() & ~PAPR_E); break;     \
      default: i_call(fail_); break;                    \
    }                                                   \
})

/**************************************************************/
/************************** tlb macro *************************/
/**************************************************************/
#define clean_2_readable()           

/*
 #define clean_2_readable()           \
 ({                                \
     unsigned int va;              \
     va = i_cld_mea();             \
     i_dtlb_read(va);              \
     i_cst_ced(i_cld_ced() | (READABLE << CED_READ_SHIFT));  \
     i_dtlb_write(va);             \
  })
*/

#define write_dtlb(va,readable,writable,executable,size) \
({                                \
    unsigned int _pa;             \
    _pa = va_2_pa(va);            \
    set_ced(_pa,readable,writable,executable,size);       \
    i_dtlb_write(va);             \
})

#define write_dtlb_shadow(va,readable,writable,executable,size) \
({                                \
    unsigned int _pa;             \
    _pa = va_2_pa_shadow(va);            \
    set_ced(_pa,readable,writable,executable,size);       \
    i_dtlb_write(va);             \
})

#define write_itlb(va,executable,size)  \
({                                \
    unsigned int _pa;             \
    _pa = va_2_pa(va);            \
    set_ced(_pa,UNREADABLE,UNWRITABLE,executable,size); \
    i_dtlb_write(va);             \
})

#define lock_dtlb(va,readable,writable,executable,size) \
({                                \
    unsigned int _pa;             \
    _pa = va_2_pa(va);            \
    set_ced(_pa,readable,writable,executable,size);       \
    i_dtlb_lock(va);              \
})

#define lock_itlb(va,executable,size)  \
({                                \
    unsigned int _pa;             \
    _pa = va_2_pa(va);            \
    set_ced(_pa,0,0,executable,size); \
    i_dtlb_lock(va);              \
})

/**************************************************************/
/************************* misc macro *************************/
/**************************************************************/

#define va_2_pa_shadow(va)				\
({						\
    unsigned int _pa_,_tmp_,_err_;	\
    _tmp_ = ((va)>>20) & 0xFFF;		        \
    _pa_  = va & 0x000FFFFF;			\
               _pa_ = _pa_ & 0x3FFFF;           \
               break;                           \
    case 0x100: _tmp_ = 0x80000;                \
               _pa_ = _pa_ & 0x1FFF;           \
               break;                           \
    case 0x080: _tmp_ = 0x40000;                \
               _pa_ = _pa_ & 0x7FFF;           \
               break;                           \
    case 0x000: _tmp_ = 0x00000;                \
               _pa_ = _pa_ & 0x1FFFF;           \
               break;                           \
    default: _err_ = 1; break;                  \
    }                                           \
    _pa_ += _tmp_;                              \
    if(_err_) fail_();                          \
    _pa_;                                       \
}) 

#define va_2_pa(va)				\
({						\
    unsigned int _pa_,_tmp_,_err_;		\
    _tmp_ = ((va)>>20) & 0xFFF;		        \
    _pa_  = va & 0x000FFFFF;			\
    _err_ = 0;			                \
    switch (_tmp_) {                            \
    case 0xD80: _tmp_ = 0xC0000;                \
               if (_pa_ >= 0x40000) _err_ = 1;  \
               break;                           \
    case 0xD00: _tmp_ = 0x80000;                \
               if (_pa_ >= 0x2000) _err_ = 1;   \
               break;                           \
    case 0xC80: _tmp_ = 0x40000;                \
               if (_pa_ >= 0x8000) _err_ = 1;   \
               break;                           \
    case 0xC00: _tmp_ = 0x00000;                \
               if (_pa_ >= 0x20000) _err_ = 1;  \
               break;                           \
    case 0x180: _tmp_ = 0xC0000;                \
               if (_pa_ >= 0x40000) _err_ = 1;  \
               break;                           \
    case 0x100: _tmp_ = 0x80000;                \
               if (_pa_ >= 0x2000) _err_ = 1;  \
               break;                           \
    case 0x080: _tmp_ = 0x40000;                \
               if (_pa_ >= 0x8000) _err_ = 1;   \
               break;                           \
    case 0x000: _tmp_ = 0x00000;                \
               if (_pa_ >= 0x20000) _err_ = 1;  \
               break;                           \
    default: _err_ = 1; break;                  \
    }                                           \
    _pa_ += _tmp_;                              \
    if(_err_) fail_();                          \
    _pa_;                                       \
}) 


#define WORD_ACCESS 0

#if  (ENDIAN==0)
#define BYTE_ACCESS 3
#define HWORD_ACCESS 2
#define ACCESS_HIGH_INC (-1)
#else
#define BYTE_ACCESS 0
#define HWORD_ACCESS 0
#define ACCESS_HIGH_INC 1
#endif
#endif  /* __CCORE_MMU_H__ */

