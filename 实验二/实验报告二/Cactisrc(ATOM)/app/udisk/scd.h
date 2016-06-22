#ifndef __CCORE_SCD_H__
#define __CCORE_SCD_H__


#define SCDDR         0x9803FD00 
#define SCDCR         0x9803FD04 
#define SCDSR         0x9803FD08      
#define SCDTFR        0x9803FD0C      
#define SCDECR        0x9803FD10      
#define IOCSR0        0x9803FD14
#define IOCSR1        0x9803FD18


#define SCDCR_SCDE    0x8000
#define SCDCR_TRS     0x4000
#define SCDCR_T2R     0x2000
#define SCDCR_TRIG    0x0200
#define SCDCR_FLUSH   0x0100
#define SCDCR_TP      0x0080
#define SCDCR_CONV    0x0040
#define SCDCR_SCDIE   0x0001

#define SCDCR_TRIG_1  0x0000
#define SCDCR_TRIG_2  0x0200

#define SCDSR_VRST    0x8000
#define SCDSR_TSEND   0x4000
#define SCDSR_ATRMD   0x2000
#define SCDSR_ORER    0x0400
#define SCDSR_PER     0x0200
#define SCDSR_FFTRG   0x0100
#define SCDSR_FEMP    0x0080
#define SCDSR_FFUL    0x0040
#define SCDSR_RETR_3  0x0010
#define SCDSR_ECNTO   0x0001

#define IOCSR_D      0x40
#define IOCSR_DI     0x20
#define IOCSR_OD     0x10
#define IOCSR_DT     0x0C
#define IOCSR_IE     0x02
#define IOCSR_IF     0x01

#define IOCSR_DT_LOW 0x00
#define IOCSR_DT_HIGH  0x04
#define IOCSR_DT_FALL 0x08
#define IOCSR_DT_RISE 0x0c
/*************** arca2sc1 register********************/

#define set_scddr(val)       i_s8(val, SCDDR ,0)
#define set_scdcr(val)       i_s16(val, SCDCR , 0)
#define set_scdsr(val)       i_s16(val, SCDSR , 0)
#define set_scdtfr(val)      i_s16(val, SCDTFR , 0)
#define set_scdecr(val)      i_s16(val, SCDECR , 0)
#define set_iocsr0(val)      i_s8(val, IOCSR0,0)
#define set_iocsr1(val)      i_s8(val, IOCSR1,0)

#define get_scddr()          i_l8u(SCDDR, 0)
#define get_scdcr()          i_l16(SCDCR, 0)
#define get_scdsr()          i_l16u(SCDSR, 0)
#define get_scdtfr()         i_l16u(SCDTFR, 0)
#define get_scdecr()         i_l16(SCDECR, 0)
#define get_iocsr0()         i_l8u(IOCSR0, 0)
#define get_iocsr1()         i_l8u(IOCSR1, 0)

#define get_scd_fifo()              get_scddr()
#define set_scd_fifo(val)           set_scddr(val)

#define get_scd_rfifo()             get_scd_fifo()
#define set_scd_tfifo(val)          set_scd_fifo(val)


/**************** arca2sc1 control register bit***************/

#define enable_scd()                set_scdcr(get_scdcr() | SCDCR_SCDE)
#define disable_scd()               set_scdcr(get_scdcr() & ~SCDCR_SCDE)

#define set_scdcr_trs()             set_scdcr(get_scdcr() | SCDCR_TRS)
#define clr_scdcr_trs()             set_scdcr(get_scdcr() & ~SCDCR_TRS)
#define get_scdcr_trs()             (get_scdcr() & SCDCR_TRS)

#define set_scdcr_t2r()             set_scdcr(get_scdcr() | SCDCR_T2R)
#define clr_scdcr_t2r()             set_scdcr(get_scdcr() & ~SCDCR_T2R)
#define get_scdcr_t2r()             (get_scdcr() & SCDCR_T2R)

#define flush_scd_fifo()            set_scdcr(get_scdcr() | SCDCR_FLUSH)

#define set_scdcr_tp_to_1()         set_scdcr( get_scdcr() | SCDCR_TP)
#define set_scdcr_tp_to_0()         set_scdcr( get_scdcr() & ~SCDCR_TP)
#define get_scdcr_tp()              ( get_scdcr() & SCDCR_TP)

#define set_scdcr_conv()            set_scdcr( get_scdcr() | SCDCR_CONV)
#define clr_scdcr_conv()            set_scdcr( get_scdcr() & ~SCDCR_CONV)
#define get_scdcr_conv()            ( get_scdcr()& SCDCR_CONV)

#define dis_scd_int()               set_scdcr(get_scdcr() & ~ SCDCR_SCDIE ) 
#define enb_scd_int()               set_scdcr(get_scdcr() |   SCDCR_SCDIE ) 

/*void  set_scd_trig(int val)                                          
{                                                                    
  if(val==1)                                                         
    set_scdcr(get_scdcr() & ~SCDCR_TRIG | SCDCR_TRIG_1);       
  else if(val==2)                                                         
    set_scdcr(get_scdcr() & ~SCDCR_TRIG | SCDCR_TRIG_2);       
}

int  get_scd_trig()                        
{                                                 
  switch (get_scdcr() & SCDCR_TRIG)            
    {                                              
    case SCDCR_TRIG_2:                             
      return 2;                                    
    case SCDCR_TRIG_1:                             
      return 1;                                    
    default:                                       
      return 1; 
    }
}*/
#define enb_scd_trxi()              enb_scd_int()
#define enb_scd_epi()               enb_scd_int()
#define enb_scd_eoi()               enb_scd_int()
#define enb_scd_reti()              enb_scd_int()
#define enb_scd_eci()               enb_scd_int()

#define dis_scd_trxi()              dis_scd_int()
#define dis_scd_epi()               dis_scd_int()
#define dis_scd_eoi()               dis_scd_int()
#define dis_scd_reti()              dis_scd_int()
#define dis_scd_eci()               dis_scd_int()


#define enable_scd_transmit()       set_scdcr(get_scdcr() | SCDCR_TRS  | SCDCR_SCDE)
#define enable_scd_receive()        set_scdcr(get_scdcr() & ~SCDCR_TRS | SCDCR_SCDE)

#define disable_scd_transmit()      set_scdcr(get_scdcr() & ~SCDCR_TRS)
#define disable_scd_receive()       set_scdcr(get_scdcr() | SCDCR_TRS)

/* define interrupt of arca2sc1*/

#define enb_scd_tper_int()          enb_scd_epi()
#define enb_scd_retr_3_int()        enb_scd_reti()
#define enb_scd_ecnto_int()         enb_scd_eci()
#define enb_scd_txi()               enb_scd_trxi()
#define enb_scd_rxi()               enb_scd_trxi()
#define enb_scd_eri()               enb_scd_epi()               
#define dis_scd_tper_int()          dis_scd_epi()
#define dis_scd_retr_3_int()        dis_scd_reti()
#define dis_scd_ecnto_int()         dis_scd_eci()
#define dis_scd_txi()               dis_scd_trxi()
#define dis_scd_rxi()               dis_scd_trxi()
          

/***********arca2sc1 status register bit **************/

#define get_scdsr_vrst()               (get_scdsr() & SCDSR_VRST)
#define get_scdsr_tsend()              (get_scdsr() & SCDSR_TSEND)
#define get_scdsr_atrmd()              (get_scdsr() & SCDSR_ATRMD)
#define get_scdsr_orer()               (get_scdsr() & SCDSR_ORER)
#define get_scdsr_per()                (get_scdsr() & SCDSR_PER)
#define get_scdsr_fftrg()              (get_scdsr() & SCDSR_FFTRG)
#define get_scdsr_femp()               (get_scdsr() & SCDSR_FEMP)
#define get_scdsr_fful()               (get_scdsr() & SCDSR_FFUL)
#define get_scdsr_retr_3()             (get_scdsr() & SCDSR_RETR_3)
#define get_scdsr_ecnto()              (get_scdsr() & SCDSR_ECNTO)

#define get_scd_rper()                 get_scdsr_per()
#define get_scd_tper()                 get_scdsr_per()
#define clr_scd_rper()                 clr_scdsr_per()
#define clr_scd_tper()                 clr_scdsr_per()
#define clr_scdsr_vrst()               set_scdsr(get_scdsr() & ~SCDSR_VRST)
#define clr_scdsr_per()                set_scdsr((get_scdsr() & ~SCDSR_PER) | SCDSR_RETR_3)
#define clr_scdsr_retr_3()             set_scdsr(get_scdsr() & ~SCDSR_RETR_3)
#define clr_scdsr_orer()               set_scdsr(get_scdsr() & ~SCDSR_ORER)
#define clr_scdsr_ecnto()              set_scdsr(get_scdsr() & ~SCDSR_ECNTO)

/******************* iocsr0 register bit ******************/

#define set_iocsr0_d()                   set_iocsr0(get_iocsr0() | IOCSR_D)
#define clr_iocsr0_d()                   set_iocsr0(get_iocsr0() & ~IOCSR_D) 
#define get_iocsr0_d()                   (get_iocsr0() & IOCSR_D)>>6

#define set_iocsr0_di_in()               set_iocsr0(get_iocsr0() & ~IOCSR_DI)
#define set_iocsr0_di_out()              set_iocsr0(get_iocsr0() | IOCSR_DI)
 
#define en_iocsr0_od()                   set_iocsr0(get_iocsr0() | IOCSR_OD)
#define dis_iocsr0_od()                  set_iocsr0(get_iocsr0() & ~IOCSR_OD)

#define set_iocsr0_dt_low()               set_iocsr0(get_iocsr0() & ~IOCSR_DT | IOCSR_DT_LOW)
#define set_iocsr0_dt_high()              set_iocsr0(get_iocsr0() & ~IOCSR_DT | IOCSR_DT_HIGH)
#define set_iocsr0_dt_fall()              set_iocsr0(get_iocsr0() & ~IOCSR_DT | IOCSR_DT_FALL)
#define set_iocsr0_dt_rise()              set_iocsr0(get_iocsr0() & ~IOCSR_DT | IOCSR_DT_RISE)
#define get_iocsr0_dt()                  (get_iocsr0() & IOCSR_DT)

#define en_iocsr0_ie()                    set_iocsr0(get_iocsr0() | IOCSR_IE)
#define dis_iocsr0_ie()                   set_iocsr0(get_iocsr0() & ~IOCSR_IE)
#define get_iocsr0_ie()                   (get_iocsr0() & IOCSR_IE)>>1

#define set_iocsr0_if()                  set_iocsr0(get_iocsr0() | IOCSR_IF)
#define clr_iocsr0_if()                  set_iocsr0(get_iocsr0() | IOCSR_IF)
#define get_iocsr0_if()                  (get_iocsr0() & IOCSR_IF)

/******************* iocsr1 register bit ******************/

#define set_iocsr1_d()                   set_iocsr1(get_iocsr1() | IOCSR_D)
#define clr_iocsr1_d()                   set_iocsr1(get_iocsr1() & ~IOCSR_D) 
#define get_iocsr1_d()                   (get_iocsr1() & IOCSR_D)>>6

#define set_iocsr1_di_in()               set_iocsr1(get_iocsr1() & ~IOCSR_DI)
#define set_iocsr1_di_out()              set_iocsr1(get_iocsr1() | IOCSR_DI)
 
#define en_iocsr1_od()                   set_iocsr1(get_iocsr1() | IOCSR_OD)
#define dis_iocsr1_od()                  set_iocsr1(get_iocsr1() & ~IOCSR_OD)

#define set_iocsr1_dt_low()               set_iocsr1(get_iocsr1() & ~IOCSR_DT | IOCSR_DT_LOW)
#define set_iocsr1_dt_high()              set_iocsr1(get_iocsr1() & ~IOCSR_DT | IOCSR_DT_HIGH)
#define set_iocsr1_dt_fall()              set_iocsr1(get_iocsr1() & ~IOCSR_DT | IOCSR_DT_FALL)
#define set_iocsr1_dt_rise()              set_iocsr1(get_iocsr1() & ~IOCSR_DT | IOCSR_DT_RISE)
#define get_iocsr1_dt()                   (get_iocsr1() & IOCSR_DT)


#define en_iocsr1_ie()                    set_iocsr1(get_iocsr1() | IOCSR_IE)
#define dis_iocsr1_ie()                   set_iocsr1(get_iocsr1() & ~IOCSR_IE)
#define get_iocsr1_ie()                   (get_iocsr1() & IOCSR_IE)>>1

#define set_iocsr1_if()                  set_iocsr1(get_iocsr1() | IOCSR_IF)
#define clr_iocsr1_if()                  set_iocsr1(get_iocsr1() | IOCSR_IF)
#define get_iocsr1_if()                  (get_iocsr1() & IOCSR_IF)

#endif

