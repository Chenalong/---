#include<instructions.h>
#include<bsp.h>
#include <option.h>
#include "key.h"
#include "udisk.h"
#include "nand_media.h"
#include "udiskdes.h"

void start_timer()
{
  /*set timer enable register to 0, stop the timer*/
  REG8(TER) &=0x2 ;	
  /*set timer count to 0xffffffff,reload data register =0x90000*/
  REG32(TCNT0) = 0xffffffff;   
  REG32(TRDR0) = 0xffffffff;  	
  /*enable intterupt ,set internal clock to pclk/256*/
  REG16(TCSR0) = 0x03;
  /*wait while timer is not busy*/
  while(REG16(TCSR0)&0x80); 
  /* enable timer0, start count */
  REG8(TER) |= 1;
}


unsigned long get_timer()
{
  unsigned long nlen;
  nlen = 0xffffffff-REG32(TCNT0);
  return nlen;  
}


void stop_timer()
{
  REG8(TER) &=0x2 ;
}

int main()
{
	udisk_init();
	usbkey_initialize();//Пе 
	key_init();
	des_init();

#ifdef DISK_TEST_MODE_ENABLE		
	printf("pull up USB, enable USB... \n");
#endif

       // pull up USB, enable USB
	USB_PULLUP_HIGH();

#ifdef DISK_TEST_MODE_ENABLE		
	printf("start USB... \n");	
#endif	
	while(1)
	{  
	 	udisk_poll();
		key_poll();
	}
	return 0;
}




