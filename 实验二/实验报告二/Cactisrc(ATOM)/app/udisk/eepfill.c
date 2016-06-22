#include<instructions.h>
#include<bsp.h>
#include <types.h>
#include "scd.h"
#define set_iocsr0_d()                   set_iocsr0(get_iocsr0() | IOCSR_D)
#define clr_iocsr0_d()                   set_iocsr0(get_iocsr0() & ~IOCSR_D) 
#define get_iocsr0_d()                   (get_iocsr0() & IOCSR_D)>>6

#define set_iocsr0_di_in()               set_iocsr0(get_iocsr0() & ~IOCSR_DI)
#define set_iocsr0_di_out()              set_iocsr0(get_iocsr0() | IOCSR_DI)

#define set_iocsr1_d()                   set_iocsr1(get_iocsr1() | IOCSR_D)
#define clr_iocsr1_d()                   set_iocsr1(get_iocsr1() & ~IOCSR_D) 
#define get_iocsr1_d()                   (get_iocsr1() & IOCSR_D)>>6

#define set_iocsr1_di_in()               set_iocsr1(get_iocsr1() & ~IOCSR_DI)
#define set_iocsr1_di_out()              set_iocsr1(get_iocsr1() | IOCSR_DI)
#define EEPROM_CNTRL 0x98011240
/* 
 eeprom mapped base address
*/
#define EEPROM_BASE  0x88000000
#define EEPROM_SIZE  0x8000
#define TRUE  1
#define FALSE 0


void my_tmu_handler1()
{ 
 /* clear the UF to 0 */
 REG16(TCSR1)&=0xBF;
 //printf("TMU1 handler");
 if(get_iocsr1_d())
    clr_iocsr1_d();
 else  
    set_iocsr1_d();
}

void  test_my_tmu1()
{			
  /*install an interrupt handler */
  enable_int(INTC_TMU0);
  set_iocsr1_di_out();
  install_irq_handler(INTC_TMU1,my_tmu_handler1); 
  /*set timer enable register to 0, stop the timer*/
  REG8(TER) =0 ;	
  /*set timer count to 0x90000,reload data register =0x90000*/
  REG32(TCNT1) = 0x5b8d8;   
  REG32(TRDR1) = 0x5b8d8;  	
  /*enable intterupt ,set internal clock to pclk/256*/
  REG16(TCSR1) = 0x03|0x20;
  /*wait while timer is not busy*/
  while(REG16(TCSR1)&0x80); 
  /* enable timer0, start count */
  REG8(TER) = 0x02;
  /*enable timer0 interrupt*/
  enable_int(INTC_TMU1);
  
}
#if 0
void start_timer0()
{
  /*set timer enable register to 0, stop the timer*/
  REG8(TER) &=0x2 ;	
  /*set timer count to 0xffffffff,reload data register =0x90000*/
  REG32(TCNT0) = 0xffffffff;   
  REG32(TRDR0) = 0xffffffff;  	
  /*enable intterupt ,set internal clock to pclk/256*/
  REG16(TCSR0) = 0x03|0x00;
  /*wait while timer is not busy*/
  while(REG16(TCSR0)&0x80); 
  /* enable timer0, start count */
  REG8(TER) |= 1;

}

unsigned long stop_timer0()
{
  /*set timer enable register to 0, stop the timer*/
  REG8(TER) &=0x2 ;
  /*wait while timer is not busy*/
  while(REG16(TCSR0)&0x80);
  return REG32(TCNT0);
}
#endif

