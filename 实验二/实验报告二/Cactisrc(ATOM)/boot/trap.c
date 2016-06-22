/* Name : trap.c
 *  exception  handler module
 */
/*
 * CHANGELOG:
 * 26-08-2005	copied and modified by Stanley.Zhu from Zi1221src <zhu.shan@zte.com.cn>
 * 16-08-2003	modified by Stanley.Zhu<zhu.shan@zte.com.cn>
 * 22-07-2003	created by ARCA TECH
  
*-------------------------------------------------------------------------*/

#include <instructions.h>
#include <bsp.h>
#include <mmu.h>
#include <config.h>
volatile unsigned int RAM_SYS_DATA interrupt_type = 0;
volatile unsigned int RAM_SYS_DATA int_source = 0;

int RAM_SYS_DATA  unexpected_error = 0;

/* place in sys_data section */
RAM_SYS_DATA unsigned int vector_intc_handler[32];
RAM_SYS_DATA unsigned int vector_exp_handler[16];

/*A dummy print function ,if you will print some message, add code here*/
void myprint_long(unsigned int x){}

/*install exception handler 
  INPUT :
         * type : vector type (TP_IU = all vector table)
	                   (TP_EXP = exception vector table)
			   (TP_INT = interrupt vector table)
	 * id   : id number
	 * func : pointer of handler function
  OUTPUT:
         none; 
  RETURN:
         none; 
*/
void setvbr_default(int type, int id, unsigned int func)
{
  unsigned long vbr;
  switch(type)
    {
    case TP_IU:
      vbr = i_rcr_vbr();
      REG32(vbr+id*4) = func;
      break;
    case TP_EXP:
      vector_exp_handler[id] = func;
      break;
    case TP_INT:
      vector_intc_handler[id] = func;
      break;
    }
}
/*install irq handler 
  INPUT :
	 * irqnum   : irq number
	 * func     : pointer of handler function
  OUTPUT:
         none; 
  RETURN:
         none; 
*/
void install_irq_handler(int irqnum, unsigned long func)
{
  dis_interrupt();
  setvbr_default(TP_INT , irqnum, func);
} 

/*disable irq (irqnum)
  INPUT :
	 * irqnum   : irq number
 OUTPUT:
         none; 
  RETURN:
         none; 
*/
void disable_int(int irqnum)
{
  REG32(IMR)|=1<<irqnum;
}
/*enable irq (irqnum)
  INPUT :
	 * irqnum   : irq number
  OUTPUT:
         none; 
  RETURN:
         none; 
*/
void enable_int(int irqnum)
{
  REG32(IMCR)|=1<<irqnum;
  enb_interrupt();
}

void SetVectorTable(int offset,unsigned long handler)
{
  int type,id;
  type = offset&0xF00;
  id   = offset&0xFF;
  setvbr_default(type, id, handler);
}
/* clear ipr */
void clear_ipr_highest() 
{
  REG32(IPR) = 1 << int_source;                
}

/*get interrupt source number*/
  void get_pending_int_source_one()           
{
  unsigned long ipr, csb;                      
  ipr = REG32(IPR);                            
  csb = i_csb(ipr);                            
  int_source = 30 - csb;                       
}

int do_entry0(struct pt_regs * regs)  /* reset or dboot */
{
  unexpected_error = (i_rcr_sr() & SR_CAUSE_MASK) >> SR_CAUSE_SFT;
  myprint_long(unexpected_error);
  myprint_long(i_rcr_spc());
  return 0;
}
  int do_entry1(struct pt_regs * regs)  /* ill_insn */
{
  myprint_long(i_rcr_spc());
}
  int do_entry2(struct pt_regs * regs)  /* debug */
{
   myprint_long(2);
   
  myprint_long(i_rcr_spc());
}

  int do_entry3(struct pt_regs * regs)  /* reserved */
{
  myprint_long(i_rcr_spc());
}

  int do_entry5(struct pt_regs * regs)  /* trap */
{
   print_string("trap");
 if ( regs->r16 == 5 )   
    {
      regs->sr |= SR_SM_MASK;
    }
  else
  if ( regs->r16 == 100) /* show_long r1 */
    {
    }
  else if (regs->r16 == 101) /* show_str r1 */
    ;
  else 
    {
      myprint_long(i_rcr_spc());
    }
  return 0;
}

unsigned int pa_protect_flag=0;
  int do_entry6(struct pt_regs * regs)  /* TLBFAULT */
{
  unsigned int i, cause,va;
  void (*f)(struct pt_regs * regs);
  int fail_p = 0;

  print_string("exception \n");
  cause = ((i_cld_mcr() & MCR_CAUSE) >> MCR_CAUSE_SHIFT);
  unexpected_error = cause;
  va = i_cld_mea();
  print_long(va);
  print_long(regs->pc);
  print_long(cause);
  f = (void (*)()) 0;
  switch(cause)
      {
      case MCR_CAUSE_IMISS:
	if (vector_exp_handler[EXP_CAUSE_IMISS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IMISS];
        else write_itlb(va,EXECUTABLE,SIZE);
	break;
      case MCR_CAUSE_IMISS_ICACHE:
	if (vector_exp_handler[EXP_CAUSE_IMISS_ICACHE])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IMISS_ICACHE];
        else write_itlb(va,EXECUTABLE,SIZE);
	break;
      case MCR_CAUSE_DMISS_RD:
	if (vector_exp_handler[EXP_CAUSE_DMISS_RD])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DMISS_RD];
        else write_dtlb(va,READABLE,WRITABLE,EXECUTABLE,SIZE);
	break;
      case MCR_CAUSE_DMISS_WR:
	if (vector_exp_handler[EXP_CAUSE_DMISS_WR])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DMISS_WR];
        else write_dtlb(va,READABLE,WRITABLE,EXECUTABLE,SIZE);
	break;
      case MCR_CAUSE_DMISS_INS:
	if (vector_exp_handler[EXP_CAUSE_DMISS_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DMISS_INS];
        else write_dtlb(va,READABLE,WRITABLE,EXECUTABLE,SIZE);
	break;
      case MCR_CAUSE_IADDR_FET:
	if (vector_exp_handler[EXP_CAUSE_IADDR_FET])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IADDR_FET];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_IADDR_INS:
	if (vector_exp_handler[EXP_CAUSE_IADDR_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IADDR_INS];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_ICONFIG:
	if (vector_exp_handler[EXP_CAUSE_ICONFIG])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_ICONFIG];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DADDR_RD:
	if (vector_exp_handler[EXP_CAUSE_DADDR_RD])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DADDR_RD];
        else
	{ regs->pc = regs->pc+4;}
	break;
      case MCR_CAUSE_DADDR_WR:
	if (vector_exp_handler[EXP_CAUSE_DADDR_WR])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DADDR_WR];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DADDR_INS:
	if (vector_exp_handler[EXP_CAUSE_DADDR_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DADDR_INS];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DCONFIG:
	if (vector_exp_handler[EXP_CAUSE_DCONFIG])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DCONFIG];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_IPSPV_FET:
	if (vector_exp_handler[EXP_CAUSE_IPSPV_FET])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IPSPV_FET];
        else  {
	  	pa_protect_flag=1;
         	regs->sr |= 0x8; 
	  	regs->pc = regs->pc +4;
		}
	break;
      case MCR_CAUSE_IPSPV_INS:
	if (vector_exp_handler[EXP_CAUSE_IPSPV_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_IPSPV_INS];
        else  {
	  	pa_protect_flag=1;
         	regs->sr |= 0x8; 
	  	regs->pc = regs->pc +4;
		}
        
	break;
      case MCR_CAUSE_DPSPV_INS:
	if (vector_exp_handler[EXP_CAUSE_DPSPV_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DPSPV_INS];
        else  {
	  	pa_protect_flag=1;
         	regs->sr |= 0x8; 
	  	regs->pc = regs->pc +4;
		}
	break;
      case MCR_CAUSE_DPSPV_RD:
	if (vector_exp_handler[EXP_CAUSE_DPSPV_RD])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DPSPV_RD];
        else  {
	  	pa_protect_flag=1;
         	regs->sr |= 0x8; 
	  	regs->pc = regs->pc +4;
		}
	break;
      case MCR_CAUSE_DPSPV_WR:
	if (vector_exp_handler[EXP_CAUSE_DPSPV_WR])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DPSPV_WR];
        else  {
	  	pa_protect_flag=1;
         	regs->sr |= 0x8; 
	  	regs->pc = regs->pc +4;
		}
	break;

      case MCR_CAUSE_ITLBPV_FET:
	if (vector_exp_handler[EXP_CAUSE_ITLBPV_FET])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_ITLBPV_FET];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_ITLBPV_INS:
	if (vector_exp_handler[EXP_CAUSE_ITLBPV_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_ITLBPV_INS];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DTLBPV_RD:
	if (vector_exp_handler[EXP_CAUSE_DTLBPV_RD])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DTLBPV_RD];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DTLBPV_WR:
	if (vector_exp_handler[EXP_CAUSE_DTLBPV_WR])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DTLBPV_WR];
        else  fail_p = 1;
	break;
      case MCR_CAUSE_DTLBPV_INS:
	if (vector_exp_handler[EXP_CAUSE_DTLBPV_INS])
	  f = (void (*)())vector_exp_handler[EXP_CAUSE_DTLBPV_INS];
        else  fail_p = 1;
	break;
      default:
	fail_p = 1;
	break;
      }
  
 
  if (fail_p)
    {
    myprint_long(i_rcr_spc());
    }
  else if (f)
    f(regs);
  //printf("epc=%x",regs->pc);
  //print_string("leave exception handler");  
  return 0;
}

int do_entry4(struct pt_regs * regs)  /* Int */
{
  void (*f)(struct pt_regs * regs);
  int fail_p = 0;
  get_pending_int_source_one();                
  //printf("int source =0x%08x\n",int_source);
  switch(int_source)
    {                                          
      case INTC_SEC:                          
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_SEC;                          
        if (vector_intc_handler[INTC_SEC])
          f = (void (*)())(vector_intc_handler[INTC_SEC]);
        else fail_p = 1;
        break;                                 
      case INTC_TMU0:                           
        if (vector_intc_handler[INTC_TMU0])
	      f = (void (*)())(vector_intc_handler[INTC_TMU0]);
     	else fail_p = 1;
          break;                                 
      case INTC_TMU1:                           
        if (vector_intc_handler[INTC_TMU1])
	       f = (void (*)())(vector_intc_handler[INTC_TMU1]);
	    else fail_p = 1;
           break;                                 
      case INTC_UDC:
        if (vector_intc_handler[INTC_UDC])
    	  f = (void (*)())(vector_intc_handler[INTC_UDC]);
    	else 
     	   fail_p = 1;
 	    break;
      case INTC_SCD0:                           
        if (vector_intc_handler[INTC_SCD0])
          f = (void (*)())(vector_intc_handler[INTC_SCD0]);
      	else 
           fail_p = 1;
           break;                                 
      case INTC_GPIO18:                           
        if (vector_intc_handler[INTC_GPIO18])
          f = (void (*)())(vector_intc_handler[INTC_GPIO18]);
      	else 
           fail_p = 1;
           break;                                 
      case INTC_GPIO23:                           
        if (vector_intc_handler[INTC_GPIO23])
          f = (void (*)())(vector_intc_handler[INTC_GPIO23]);
      	else 
           fail_p = 1;
           break;                                 
      case INTC_SCC0:                           
        if (vector_intc_handler[INTC_SCC0])
           f = (void (*)())(vector_intc_handler[INTC_SCC0]);
     	else fail_p = 1;
           break;                                 
      case INTC_SCC1:                           
        if (vector_intc_handler[INTC_SCC1])
           f = (void (*)())(vector_intc_handler[INTC_SCC1]);
     	else fail_p = 1;
           break;                                 
      case INTC_DES:                          
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_DES;                          
        if (vector_intc_handler[INTC_DES])
          f = (void (*)())(vector_intc_handler[INTC_DES]);
        else fail_p = 1;
          break;
      case INTC_RSA:                          
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_RSA;                          
        if (vector_intc_handler[INTC_RSA])
          f = (void (*)())(vector_intc_handler[INTC_RSA]);
        else fail_p = 1;
          break;
      case INTC_RNG:
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_RNG;                          
        if (vector_intc_handler[INTC_RNG])
          f = (void (*)())(vector_intc_handler[INTC_RNG]);
        else fail_p = 1;
        break;
      case INTC_SCDRST:                          
        if (vector_intc_handler[INTC_SCDRST])
          f = (void (*)())(vector_intc_handler[INTC_SCDRST]);
        else fail_p = 1;
        break;
      case INTC_UDCRST:                          
        if (vector_intc_handler[INTC_UDCRST])
          f = (void (*)())(vector_intc_handler[INTC_UDCRST]);
        else fail_p = 1;
        break;
      case INTC_SPI:                      
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_SPI;                          
        if (vector_intc_handler[INTC_SPI])
          f = (void (*)())(vector_intc_handler[INTC_SPI]);
        else fail_p = 1;
        break;  
      case INTC_UART:                          
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_UART;                          
        if (vector_intc_handler[INTC_UART])
          f = (void (*)())(vector_intc_handler[INTC_UART]);
        else fail_p = 1;
          break;
      case INTC_PPC:                      
        /*clear the flag register*/
        REG16(IFLAGR)|=IFLAG_PPC;                          
        if (vector_intc_handler[INTC_PPC])
          f = (void (*)())(vector_intc_handler[INTC_PPC]);
        else fail_p = 1;
        break;  
 
    default:
      fail_p = 1;
      return 0; 
    }
  if (fail_p)
    {
     myprint_long(int_source);
     myprint_long(i_rcr_spc());
    }
  else
    f(regs);
  clear_ipr_highest();
  return 0;
}

 int do_entry7(struct pt_regs * regs)  /* Reserved */
{
  myprint_long(i_rcr_spc());
}



