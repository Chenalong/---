#ifndef __CONFIG_H__
#define __CONFIG_H__

#define FLASH_RUN_MODE 1     //run in flash or other?
#define BOOTROM_MODE   0     //bootrom mode  
#define TLB_SIZE      4000   //tlb page size
#define ENDIAN         1     //little =1 big=0

#if(FLASH_RUN_MODE == 1)
#define HOST_MODE_EN  0      //host mode or no 
#else
#define HOST_MODE_EN  1      //host mode or no
#endif



#endif 
