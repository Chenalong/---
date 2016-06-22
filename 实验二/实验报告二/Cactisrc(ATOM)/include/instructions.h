#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_
#include <config.h>
/*
 The names of registers are 
 REGISTER_NAMES						
 {  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",       
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",	
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",	
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31",	
  }
  The content of r0 is always 0.
*/
#define OER3 "%0,%1,%2"
#define OER4 "%0,%1,%2,%3"
#define LSOER3 "%0,[%1,%2]"
#define OER2U5 "%0,%1,%2"
#define OER2S10 "%0,%1,%2"
#define OER2S10_BCC "%1,%2,%0"
#define OER2S16 "%0,%1,%2"
#define OER1U21 "%0,%1"
#define OER1U10 "%0,%1"
#define OER1S5S10_BCCI "%1,%2,%0"
#define OER2 "%0,%1"
#define OER1 "%0"
#define OER1U10_TRAP "r16,%0"
#define OE ""


#define OER3_R0 "%0,%1,r0"
#define OER3_R0_3 "r0,r0,r0"

#define OER2_WCR_SR  "%0,0"
#define OER2_WCR_SSR "%0,1"
#define OER2_WCR_SPC "%0,2"
#define OER2_WCR_DSR "%0,5"
#define OER2_WCR_DPC "%0,6"

#define OER2_RCR_SR  "%0,0"
#define OER2_RCR_SSR "%0,1"
#define OER2_RCR_SPC "%0,2"
#define OER2_RCR_DSR "%0,5"
#define OER2_RCR_DPC "%0,6"

#define OER1_RCR     "%0"
#define OER2_WCR     "%0,%1"
#define OER3_CLD_CST "%0,%1,%2"
#define OER3_CMD     "%0,%1,%2"
#define OER2_CMD     "%1,%0"
/*
In Ccore,  the format of constant in the instruction is the following:
`I' is unsigned 5-bit constants.   
`J' is signed 5-bit constants.    
`K' is signed 10-bit constants.    
`L' is signed 16-bit constants.  
`' is signed 21-bit constants.    
`' is unsigned 10-bit constants.    
`O' is the constant zero.
    */
//#define C_U21 ""
#define C_S16 "L"
#define C_S10 "X"  /* instead of "K" */
#define C_U10 "X"  /* "X" is used for all constants */
#define C_S5  "J"
#define C_U5  "I"
#define C_0 "O"

#define i_prefetch(src) \
        ({__asm__ __volatile__("lx32 r0, [%0, r0]"::"r" (src));})
#define i_set(_dst_) 	\
	({__asm__ __volatile__("!set %0":"=r"(_dst_)::"memory"); })

#define i_use(src) 	   \
	({__asm__ __volatile__("!use %0"::"r"(src):"memory"); })

#define i_sbrk             \
        ({__asm__ __volatile__("sbrk"::);  })

#define i_set_use(src__dst_) 	\
	({__asm__ __volatile__("!set & use %0":"+r"(src__dst_)::"memory"); })

#define i_add(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("add\t"OER3"\t!i_add":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_interfere(src0)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("add\t"OER3_R0"\t!i_mov":"=r"(_dst_) :"r" (src0));	\
 _dst_;})
    
#define i_addi(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("addi\t"OER2S10"\t!i_addi":"=r"(_dst_) :"r" (src0),  C_S10 (src1));	\
 _dst_;})

#define i_andi(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("andi\t"OER2S16"\t!i_andi":"=r"(_dst_) :"r" (src0), C_S16 (src1));	\
 _dst_;})
    
#define i_ori(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("ori\t"OER2S16"\t!i_ori":"=r"(_dst_) :"r" (src0), C_S16 (src1));	\
 _dst_;})
    
#define i_xori(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("xori\t"OER2S16"\t!i_xori":"=r"(_dst_) :"r" (src0), C_S16 (src1));	\
 _dst_;})
    
#define i_and(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("and\t"OER3"\t!i_and":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_or(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("or\t"OER3"\t!i_or":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_xor(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("xor\t"OER3"\t!i_xor":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_andn(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("andn\t"OER3"\t!i_andn":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_seq(src0,src1)					\
({unsigned long _dst_;						\
   __asm__ __volatile__("seq\t"OER3"\t!i_seq":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_sne(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("sne\t"OER3"\t!i_sne":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
//#define i_sle(src0,src1)									\
//({unsigned long _dst_;										\
//   __asm__ __volatile__("sle\t"OER3"\t!i_sle":"=r"(_dst_) :"r" (src0), "r" (src1));	\
// _dst_;})
    
//#define i_sleu(src0,src1)									\
//({unsigned long _dst_;										\
//   __asm__ __volatile__("sleu\t"OER3"\t!i_sleu":"=r"(_dst_) :"r" (src0), "r" (src1));	\
// _dst_;})

#define i_sge(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("sge\t"OER3"\t!i_sge":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_sgeu(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("sgeu\t"OER3"\t!i_sgeu":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_slt(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("slt\t"OER3"\t!i_slt":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_sltu(src0,src1)									\
({unsigned long _dst_;										\
   __asm__ __volatile__("sltu\t"OER3"\t!i_sltu":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})
    
#define i_slli(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("slli\t"OER2U5"\t!i_slli":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 _dst_;})

#define i_slri(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("slri\t"OER2U5"\t!i_slri":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 _dst_;})

#define i_sari(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("sari\t"OER2U5"\t!i_sari":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 _dst_;})

#define i_sll(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("sll\t"OER3"\t!i_sll":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_slr(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("slr\t"OER3"\t!i_slr":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_sar(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("sar\t"OER3"\t!i_sar":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_mvz(_dst_1,src0,src1)								\
({unsigned long _dst_;										\
   __asm__ __volatile__("add\t"OER3"\t!add":"=r"(_dst_) :"r" (_dst_1), "r" (0));	\
   __asm__ __volatile__("mvz\t"OER3"\t!i_mvz":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_mvnz(_dst_1,src0,src1)								\
({unsigned long _dst_=src0;									\
   __asm__ __volatile__("add\t"OER3"\t!add":"=r"(_dst_) :"r" (_dst_1), "r" (0));	\
   __asm__ __volatile__("mvnz\t"OER3"\t!i_mvnz":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_jump(label)({ __asm__ __volatile__("j\t r0,%0"::"X"(label):"memory"); })
#define i_j(ret,label) ({ __asm__ __volatile__("j\t %0,%1":"=r"(ret):"X"(label):"memory"); })
#define i_jrl0(src) ({ __asm__ __volatile__("ja\t r0,%0,0"::"r"(src):"memory"); })
#define i_jrlr(_dst_,src0) ({ __asm__ __volatile__("ja\t %0,%1,0":"=r"(_dst_):"r"(src0):"memory"); })
#define i_ja(_dst_,src0,src1) ({ __asm__ __volatile__("ja\t %0,%1,%2":"=r"(_dst_):"r"(src0),C_U10 (src1):"memory"); })

#define i_call(src) ({ __asm__ __volatile__("ja\t r18,%0,0"::"r"(src):"memory"); })

#define i_label(id) ({  __asm__ __volatile__(".LL%0:"::C_U5(id)); })
#define i_mark(id) ({  __asm__ __volatile__("MARK%0:"::"X"(id)); })
#define i_align(num) ({  __asm__ __volatile__(".align\t%0"::C_U5(num)); })
#define i_long(op) ({ __asm__ __volatile__(".long\t%0"::"X"(op));}) 
#define i_long0(x) ({ __asm__ __volatile__(".long\t%0"::"X"(x));}) 
#define i_org(num) ({  __asm__ __volatile__(".org\t%0"::"X"(num)); })

#define i_rcr_vbr() \
({unsigned long _dst_;	  \
   __asm__ __volatile__("rcr\t"OER2_RCR_SR"\t\t!rcr sr":"=r"(_dst_));	\
 ((_dst_&0x70)<<22)|0x80000000;})

#define i_rcr_spc()								\
({unsigned long _dst_;									\
   __asm__ __volatile__("rcr\t"OER2_RCR_SPC"\t\t!rcr spc":"=r"(_dst_));	\
 _dst_;})

#define i_rcr_epc() i_rcr_spc()
#define i_rcr_esr() i_rcr_ssr()

#define i_wcr_epc(x) i_wcr_spc(x)
#define i_wcr_esr(x) i_wcr_ssr(x)

#define i_rcr_dpc()								\
({unsigned long _dst_;									\
   __asm__ __volatile__("rcr\t"OER2_RCR_DPC"\t\t!rcr dpc":"=r"(_dst_));	\
 _dst_;})

#define i_rcr_sr()								\
({unsigned long _dst_;									\
   __asm__ __volatile__("rcr\t"OER2_RCR_SR"\t\t!rcr sr":"=r"(_dst_));	\
 _dst_;})

#define i_rcr_ssr()										\
({unsigned long _dst_;									\
   __asm__ __volatile__("rcr\t"OER2_RCR_SSR"\t\t!rcr ssr":"=r"(_dst_));	\
 _dst_;})

#define i_rcr_dsr()										\
({unsigned long _dst_;									\
   __asm__ __volatile__("rcr\t"OER2_RCR_DSR"\t\t!rcr dsr":"=r"(_dst_));	\
 _dst_;})

#define i_wcr_spc(src)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR_SPC"\t\t!wcr spc":: "r"(src));  })

#define i_wcr_dpc(src)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR_DPC"\t\t!wcr dpc":: "r"(src));  })

#define i_wcr_sr(src)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR_SR"\t\t!wcr sr":: "r"(src));  })

#define i_wcr_ssr(src)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR_SSR"\t\t!wcr ssr":: "r"(src));  })

#define i_wcr_dsr(src)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR_DSR"\t\t!wcr dsr":: "r"(src));  })

#define i_rcr(src)										\
({unsigned long _dst_; \
   __asm__ __volatile__("rcr\t"OER1_RCR"\t\t!rcr ":"=r"(_dst_) : "X"(src));  })

#define i_wcr(src0,src1)										\
({   __asm__ __volatile__("wcr\t"OER2_WCR"\t\t!wcr ":: "r"(src0), "X"(src1));  })

#define i_cld(id,src0)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("cld\t"OER3_CLD_CST"\t!cld":"=r"(_dst_) :C_U5 (id), C_U10 (src0));	\
 _dst_;})

#define i_cst(src0,id,src1)										\
({   __asm__ __volatile__("cst\t"OER3_CLD_CST"\t!cst":: "r"(src0), C_U5 (id), C_U10 (src1));  })

#define i_itlb(src0,cmd)										\
({   __asm__ __volatile__("itlb\t"OER2_CMD"\t!itlb":: "r" (src0), C_U5 (cmd));  })

#define i_dtlb(src0,cmd)										\
({   __asm__ __volatile__("dtlb\t"OER2_CMD"\t!dtlb":: "r" (src0), C_U5 (cmd));  })

#define i_icache(cmd,src0,src1)										\
({   __asm__ __volatile__("icache\t"OER3_CMD"\t!icache":: C_U5 (cmd), "r" (src0), C_S10 (src1));  })

#define i_dcache(cmd,src0,src1)										\
({   __asm__ __volatile__("dcache\t"OER3_CMD"\t!dcache":: C_U5 (cmd), "r" (src0), C_S10 (src1));  })

#define i_lhi(src)										\
({  unsigned long _dst_;									\
	__asm__ __volatile__("lhi\t"OER1U21"\t!lhi":"=r"(_dst_) : "X" (src)); \
	_dst_;})

#define i_nop								\
({   __asm__ __volatile__("nop\t\t\t!or r0,r0,r0"::);  })

#define i_comm                     \
({   __asm__ __volatile__("!comment"::);  })

#define i_trap(src0)										\
({  __asm__ __volatile__("trap\t"OER1U10_TRAP"\t!trap r16,":: C_U10 (src0): "r16"); })

#define i_sleep		({  __asm__ __volatile__("sleep"); })

#define i_rte		({  __asm__ __volatile__("rte"); })

//#define i_rtf		({  __asm__ __volatile__("rtf"); })

#define i_barrier 	({  __asm__ __volotile__("":::"memory"); })

#define i_beq(_dst_,src0,src1)						\
({   __asm__ __volatile__("beq\t"OER2S10_BCC"\t!i_beq":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_bne(_dst_,src0,src1)						\
({   __asm__ __volatile__("bne\t"OER2S10_BCC"\t!i_bne":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_blt(_dst_,src0,src1)						\
({   __asm__ __volatile__("blt\t"OER2S10_BCC"\t!i_blt":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_bltu(_dst_,src0,src1)						\
({   __asm__ __volatile__("bltu\t"OER2S10_BCC"\t!i_bltu":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

//#define i_ble(_dst_,src0,src1)						\
//({   __asm__ __volatile__("ble\t"OER2S10_BCC"\t!i_ble":: "X"(_dst_),"r"(src0),"r"//(src1): "memory"); })

//#define i_bleu(_dst_,src0,src1)						\
//({   __asm__ __volatile__("bleu\t"OER2S10_BCC"\t!i_bleu":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_bge(_dst_,src0,src1)						\
({   __asm__ __volatile__("bge\t"OER2S10_BCC"\t!i_bge":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_bgeu(_dst_,src0,src1)						\
({   __asm__ __volatile__("bgeu\t"OER2S10_BCC"\t!i_bgeu":: "X"(_dst_),"r"(src0),"r"(src1): "memory"); })

#define i_beqi(_dst_,src0,src1)						\
({   __asm__ __volatile__("beqi\t"OER1S5S10_BCCI"\t!i_beqi":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })

#define i_bnei(_dst_,src0,src1)						\
({   __asm__ __volatile__("bnei\t"OER1S5S10_BCCI"\t!i_bnei":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })

#define i_blti(_dst_,src0,src1)						\
({   __asm__ __volatile__("blti\t"OER1S5S10_BCCI"\t!i_blti":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })

#define i_bltui(_dst_,src0,src1)						\
({   __asm__ __volatile__("bltui\t"OER1S5S10_BCCI"\t!i_bltui":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })

#define i_bgei(_dst_,src0,src1)						\
({   __asm__ __volatile__("bgei\t"OER1S5S10_BCCI"\t!i_bgei":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })

#define i_bgeui(_dst_,src0,src1)						\
({   __asm__ __volatile__("bgeui\t"OER1S5S10_BCCI"\t!i_bgeui":: "X"(_dst_),"r"(src0),C_S5 (src1): "memory"); })


/* Load-Store Instructions */

#define i_lx8(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lr8\t"LSOER3"\t!i_lx8":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx8u(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lr8u\t"LSOER3"\t!i_lx8u":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx16(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lr16\t"LSOER3"\t!i_lx16":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx16u(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lr16u\t"LSOER3"\t!i_lx16u":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx32(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lr32\t"LSOER3"\t!i_lx32":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_l8(src0,src1)									\
({unsigned long _dst_;									\
   __asm__ __volatile__("ld8\t"LSOER3"\t!i_l8":"=r"(_dst_) :"r" (src0), C_S10(src1));	\
 _dst_;})

#define i_l8u(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("ld8u\t"LSOER3"\t!i_l8u":"=r"(_dst_) :"r" (src0), C_S10(src1));	\
 _dst_;})

#define i_l16(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("ld16\t"LSOER3"\t!i_l16":"=r"(_dst_) :"r" (src0), C_S10(src1));	\
 _dst_;})

#define i_l16u(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("ld16u\t"LSOER3"\t!i_l16u":"=r"(_dst_) :"r" (src0), C_S10(src1));	\
 _dst_;})

#define i_l32(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("ld32\t"LSOER3"\t!i_l32":"=r"(_dst_) :"r" (src0), C_S10(src1));	\
 _dst_;})

#define i_sx8(src0,src1,src2)						\
({   __asm__ __volatile__("sr8\t"LSOER3"\t!i_sx8"			\
			  ::"r"(src0), "r"(src1), "r"(src2):"memory");	\
 })

#define i_sx16(src0,src1,src2)						\
({   __asm__ __volatile__("sr16\t"LSOER3"\t!i_sx16"			\
			  ::"r"(src0), "r"(src1), "r"(src2):"memory");	\
 })

#define i_sx32(src0,src1,src2)						\
({   __asm__ __volatile__("sr32\t"LSOER3"\t!i_sx32"			\
			  ::"r"(src0), "r"(src1), "r"(src2):"memory");	\
 })

#define i_s8(src0,src1,src2)							\
({   __asm__ __volatile__("sd8\t"LSOER3"\t!i_s8"				\
			  ::"r"(src0), "r"(src1), C_S10(src2):"memory");	\
 })

#define i_s16(src0,src1,src2)							\
({   __asm__ __volatile__("sd16\t"LSOER3"\t!i_s16"				\
			  ::"r"(src0), "r"(src1), C_S10(src2):"memory");	\
 })

#define i_s32(src0,src1,src2)							\
({   __asm__ __volatile__("sd32\t"LSOER3"\t!i_s32"				\
			  ::"r"(src0), "r"(src1), C_S10(src2):"memory");	\
 })

#define i_lx16s(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lx16\t"LSOER3"\t!i_lx16s":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx16su(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lx16u\t"LSOER3"\t!i_lx16su":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_lx32s(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("lx32\t"LSOER3"\t!i_lx32s":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

#define i_sx16s(src0,src1,src2)						\
({   __asm__ __volatile__("sx16\t"LSOER3"\t!i_sx16s"		\
			  ::"r"(src0), "r"(src1), "r"(src2):"memory");	\
 })

#define i_sx32s(src0,src1,src2)						\
({   __asm__ __volatile__("sx32\t"LSOER3"\t!i_sx32s"		\
			  ::"r"(src0), "r"(src1), "r"(src2):"memory");	\
 })

/* SUB */
#define i_sub(src0,src1)								\
({unsigned long _dst_;									\
   __asm__ __volatile__("sub\t"OER3"\t!i_sub":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})


/* MUL */
#define i_mul(src0,src1)	\
({unsigned long _dst_;		\
   __asm__ __volatile__("mulu\t"OER3"\t!i_mul":"=r"(_dst_) :"r" (src0), "r" (src1));	\
 _dst_;})

//signed multiply
#define i_muls(_dst_,src0,src1)								\
({unsigned long _dst_h,_dst_l;									\
   __asm__ __volatile__("mul\t"OER4"\t!i_mul":"=r"(_dst_h),"=r" (_dst_l) :"r" (src0), "r" (src1));	\
  _dst_ = _dst_h;       \
  _dst_l;})

//unsigned multiply
#define i_mulu(_dst_,src0,src1)								\
({unsigned long _dst_h,_dst_l;									\
   __asm__ __volatile__("mulu\t"OER4"\t!i_mulu":"=r"(_dst_h),"=r" (_dst_l) :"r" (src0), "r" (src1));	\
  _dst_ = _dst_h;       \
  _dst_l;})

#define i_csb(src)								\
({unsigned long __dst__;							\
   __asm__ __volatile__("csb\t"OER2"\t!i_csb":"=r"(__dst__) :"r" (src));	\
 __dst__;})

#define i_swap(src0,src1,src2)						\
({   __asm__ __volatile__("swap\t"OER2S10"\t!i_swap"                    \
                          :"+r" (src0) : "r" (src1), C_S10 (src2):"memory");	\
 })

#define i_brev(dst, src)				\
({   __asm__ __volatile__("brev\t"OER2"\t!i_brev"	\
			  ::"r"(dst), "r"(src));	\
 })

#endif
