/*
  Name: 
  Copyright: 
  Author: 
  Date: 15-06-05 15:27
  Description: 
*/

#define _UDISK_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include "udisk.h"
#include "ums.h"
#include "nand.h"
#include "nand_media.h"
//#include "uart.h"

#include "udiskdes.h"
#include "Usb_bulk.h"
#define UART_BASIC_ADDRESS (0x1802e400)
#define UART_DR (UART_BASIC_ADDRESS)
#define UART_RSR_ECT (UART_BASIC_ADDRESS+4)
#define UART_LCR_H (UART_BASIC_ADDRESS+8)
#define UART_LCR_M (UART_BASIC_ADDRESS+0x0c)
#define UART_LCR_L (UART_BASIC_ADDRESS+0x10)
#define UART_CR (UART_BASIC_ADDRESS+0x14)
#define UART_FR (UART_BASIC_ADDRESS+0x18)
#define UART_IIR_ICR (UART_BASIC_ADDRESS+0x1c)
#define UART_REIER (UART_BASIC_ADDRESS+0x24)

const unsigned short PID = 0x04b4;
const unsigned short VID = 0x4612;
//--------------------------------
unsigned char udisk_do_init ;
//--------------------------------
//#ifdef DISK_TEST_MODE_ENABLE
//#endif
unsigned char udisk_rd_wr_self_enable;

//lun variables
unsigned char lun_num ;
unsigned char lun_low_status[4];
unsigned char pudisk_lun_low_ex[4];
unsigned int	lun_low_lba_start_ex[4];
unsigned char lun_security_enable;
const unsigned char security_key = 0xFF;

//#define DEBUG  1
	
#ifdef DISK_TEST_MODE_ENABLE
void udisk_test(void);
#endif

//---------------------------------------------------------------------
//parameter including luns info,password,inquriy data for each lun
#define PARAMETER_FLASH_ADDR			0x0003f000
#define PARAMETER_FLASH_LEN			0x180

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
  unsigned long val;
  /*set timer enable register to 0, stop the timer*/
  REG8(TER) &=0x2 ;
  /*wait while timer is not busy*/
  while(REG16(TCSR0)&0x80);
  val = 0xffffffff-REG32(TCNT0);
  return val;
}

unsigned long check_cbw_usbkey(CBW * pcbw)
{
	if(pcbw->cdb[0] != VENDOROP_ZTEIC)
		return 1;
				
				if((pcbw->cdb[1] == VENDOROP_ZTEIC_CHECK_0)&&
					(pcbw->cdb[2] == VENDOROP_ZTEIC_CHECK_1)&&
					(pcbw->cdb[3] == VENDOROP_ZTEIC_CHECK_2)&&
					(pcbw->cdb[4] == VENDOROP_ZTEIC_CHECK_3)&&
					(pcbw->cdb[5] == VENDOROP_ZTEIC_CHECK_4))
					{
					//zteic 's vendor command
						if(pcbw->cdb[9]  == VENDOROP_ZTEIC_USB_KEY)
							{//usb key
																					
							return 0;																							
							}
					}		
	return 1;	 
}

void udisk_read_parameter(UCHAR * pbuff)
{
	ReadFlash(pbuff,PARAMETER_FLASH_ADDR, PARAMETER_FLASH_LEN);
#if 0	
print_buf(pbuff,PARAMETER_FLASH_LEN);
#endif
	////功能描述：从FLASH指定的地址读数据到Buf，Add是FLASH的虚拟地址。数据长度不大于一个PAGE,允许跨页读取。
}
void udisk_write_parameter(UCHAR * pbuff)
{
	WriteFlash(pbuff,PARAMETER_FLASH_ADDR, PARAMETER_FLASH_LEN);
}
void udisk_check_parameter(UCHAR * pbuff)
{
	unsigned char  FirstTime = FALSE;
	unsigned int  temp32,temp32b;
	unsigned char checkcode[16] = {'Z','T','E','I','C',(UCHAR)('Z'+'T'+'E'+'I'+'C'),0x55,0xAA,(~'Z'),(~'T'),(~'E'),(~'I'),(~'C'),(UCHAR)((~'Z')+(~'T')+(~'E')+(~'I')+(~'C')),0xAA,0x55};
	if( 0 != memcmp(pbuff,checkcode ,16)) 
		{
		FirstTime = TRUE;
		}
	else {
		temp32 = nand_num_zones *nand_logical_blocks_per_zone*nand_pages_per_block*nand_segments_per_page;
		temp32b =(( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET] <<24)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 1] <<16)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 2] <<8)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 3]));
		if(temp32 != temp32b)
			{
			FirstTime = TRUE;
			}
		}
	//FirstTime = TRUE;
	if(FirstTime)
		{
	//printf("FirstTime =true!\n ");	
		//the first time to use this disk
		memset(pbuff,0,PARAMETER_FLASH_LEN);
		memcpy(pbuff,checkcode,16);
		//flash disk parameter
		//total lba
		temp32 = (nand_num_zones *nand_logical_blocks_per_zone*nand_pages_per_block*nand_segments_per_page);
		printf("temp32=%x\n",temp32);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET]
		= ((temp32 >>24) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET+1]
		= ((temp32 >>16) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET+2]
		= ((temp32 >>8) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET+3]
		= ((temp32 ) & 0xFF);
		//lba size
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LBA_SIZE_OFFSET]
		= 0;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LBA_SIZE_OFFSET + 1]
		= 0;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LBA_SIZE_OFFSET + 2]
		= 2;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LBA_SIZE_OFFSET + 3]
		= 0;
		//lun0 lba max
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET + 1]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET + 2]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET +3]
		=  0xFF;
		//lun1 lba max
		temp32 = temp32 -1;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET]
		= ((temp32 >>24) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 1]
		= ((temp32 >>16) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 2]
		= ((temp32 >>8) & 0xFF);
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 3]
		= ((temp32) & 0xFF);
		
		//lun2 lba max
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET + 1]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET + 2]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET +3]
		=  0xFF;
		//lun3 lba max
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN3_LBA_MAX_OFFSET]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN3_LBA_MAX_OFFSET + 1]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN3_LBA_MAX_OFFSET + 2]
		=  0xFF;
		pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN3_LBA_MAX_OFFSET +3]
		=  0xFF;
		//inquriy data
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN0_VENDOR_STRING_OFFSET , "ZTEIC   " , (sizeof("ZTEIC   ") < PARAMETER_INQUIRY_DATA_LUN0_VENDOR_STRING_LEN) ?sizeof("ZTEIC   "):PARAMETER_INQUIRY_DATA_LUN0_VENDOR_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN0_PRODUCT_STRING_OFFSET , "USB SecurityDisk" , (sizeof("USB SecurityDisk") < PARAMETER_INQUIRY_DATA_LUN0_PRODUCT_STRING_LEN) ?sizeof("USB SecurityDisk"):PARAMETER_INQUIRY_DATA_LUN0_PRODUCT_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN0_VERSION_STRING_OFFSET , "1.00" , (sizeof("1.00") < PARAMETER_INQUIRY_DATA_LUN0_VERSION_STRING_LEN) ?sizeof("1.00"):PARAMETER_INQUIRY_DATA_LUN0_VERSION_STRING_LEN);
		//
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN1_VENDOR_STRING_OFFSET , "ZTEIC   " , (sizeof("ZTEIC   ") < PARAMETER_INQUIRY_DATA_LUN1_VENDOR_STRING_LEN) ?sizeof("ZTEIC   "):PARAMETER_INQUIRY_DATA_LUN1_VENDOR_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN1_PRODUCT_STRING_OFFSET , "USB Flash Disk  " , (sizeof("USB Flash Disk  ") < PARAMETER_INQUIRY_DATA_LUN1_PRODUCT_STRING_LEN) ?sizeof("USB Flash Disk  "):PARAMETER_INQUIRY_DATA_LUN1_PRODUCT_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN1_VERSION_STRING_OFFSET , "1.00" , (sizeof("1.00") < PARAMETER_INQUIRY_DATA_LUN1_VERSION_STRING_LEN) ?sizeof("1.00"):PARAMETER_INQUIRY_DATA_LUN1_VERSION_STRING_LEN);
		//
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN2_VENDOR_STRING_OFFSET , "ZTEIC   " , (sizeof("ZTEIC   ") < PARAMETER_INQUIRY_DATA_LUN0_VENDOR_STRING_LEN) ?sizeof("ZTEIC   "):PARAMETER_INQUIRY_DATA_LUN2_VENDOR_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN2_PRODUCT_STRING_OFFSET , "USB SecurityDisk" , (sizeof("USB SecurityDisk") < PARAMETER_INQUIRY_DATA_LUN0_PRODUCT_STRING_LEN) ?sizeof("USB SecurityDisk"):PARAMETER_INQUIRY_DATA_LUN2_PRODUCT_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN2_VERSION_STRING_OFFSET , "1.00" , (sizeof("1.00") < PARAMETER_INQUIRY_DATA_LUN0_VERSION_STRING_LEN) ?sizeof("1.00"):PARAMETER_INQUIRY_DATA_LUN2_VERSION_STRING_LEN);
		//
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN3_VENDOR_STRING_OFFSET , "ZTEIC   " , (sizeof("ZTEIC   ") < PARAMETER_INQUIRY_DATA_LUN0_VENDOR_STRING_LEN) ?sizeof("ZTEIC   "):PARAMETER_INQUIRY_DATA_LUN3_VENDOR_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN3_PRODUCT_STRING_OFFSET , "USB Key         " , (sizeof("USB Key         ") < PARAMETER_INQUIRY_DATA_LUN0_PRODUCT_STRING_LEN) ?sizeof("USB Key         "):PARAMETER_INQUIRY_DATA_LUN3_PRODUCT_STRING_LEN);
		memcpy(pbuff + PARAMETER_INQUIRY_DATA_OFFSET + PARAMETER_INQUIRY_DATA_LUN3_VERSION_STRING_OFFSET , "1.00" , (sizeof("1.00") < PARAMETER_INQUIRY_DATA_LUN0_VERSION_STRING_LEN) ?sizeof("1.00"):PARAMETER_INQUIRY_DATA_LUN3_VERSION_STRING_LEN);
		//
	//print_buf(pbuff,PARAMETER_FLASH_LEN);	
		udisk_write_parameter(pbuff);
	//udisk_read_parameter(pbuff);
	//print_buf(pbuff,PARAMETER_FLASH_LEN);
		}
}
void udisk_get_lun_low_lba_start(unsigned int *lun_low_lba_start,unsigned char * pbuff)
{
	//get information
	//init lun parameters
	lun_low_lba_start[0] = 0;
	
//printf("lun_low_lba_start[0] :%x\n" , lun_low_lba_start[0]);	
	lun_low_lba_start[1] =  (( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET] <<24)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET + 1] <<16)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET + 2] <<8)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN0_LBA_MAX_OFFSET + 3])
			+1) ;
	
//printf("lun_low_lba_start[1] :%x\n" , lun_low_lba_start[1]);		
	lun_low_lba_start[2] = lun_low_lba_start[1] + (( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET] <<24)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 1] <<16)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 2] <<8)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN1_LBA_MAX_OFFSET + 3])
			+1) ;
	
//printf("lun_low_lba_start[2] :%x\n" , lun_low_lba_start[2]);		
	lun_low_lba_start[3] = lun_low_lba_start[2] + (( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET] <<24)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET + 1] <<16)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET + 2] <<8)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_LUN2_LBA_MAX_OFFSET + 3])
			+1) ;
	
//printf("lun_low_lba_start[3] :%x\n" , lun_low_lba_start[3]);		;
}
void udisk_get_luns_lba_start(void)
{
	unsigned char * pbuff = nand_page_buff;
	unsigned int lba_udisk;
	
	udisk_read_parameter(pbuff);
	udisk_check_parameter(pbuff);
	lba_udisk = (( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET] <<24)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 1] <<16)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 2] <<8)
			+( pbuff[PARAMETER_FLASH_DISK_OFFSET + PARAMETER_FLASH_DISK_TOTAL_LBA_OFFSET + 3])) ;
	//
	//printf("\n%8x\n",lba_udisk);
	udisk_get_lun_low_lba_start(lun_low_lba_start_ex,pbuff);
	//total lba maybe lba_udisk,check it
	if(lun_low_lba_start_ex[0]  > (lba_udisk - 1))
		{
		lun_low_lba_start_ex[0]  = (lba_udisk - 1);
		}
	if(lun_low_lba_start_ex[1]  > (lba_udisk - 1))
		{
		lun_low_lba_start_ex[1]  = (lba_udisk - 1);
		}
	if(lun_low_lba_start_ex[2]  > (lba_udisk - 1))
		{
		lun_low_lba_start_ex[2]  = (lba_udisk - 1);
		}
	if(lun_low_lba_start_ex[3]  > (lba_udisk - 1))
		{
		lun_low_lba_start_ex[3]  = (lba_udisk - 1);
		}
	//
	if(lun_low_lba_start_ex[1]  <  lun_low_lba_start_ex[0] )
		{
		lun_low_lba_start_ex[1]  = lun_low_lba_start_ex[0] ;
		}
	if(lun_low_lba_start_ex[2]  <  lun_low_lba_start_ex[1] )
		{
		lun_low_lba_start_ex[2]  = lun_low_lba_start_ex[1] ;
		}
	if(lun_low_lba_start_ex[3]  <  lun_low_lba_start_ex[2] )
		{
		lun_low_lba_start_ex[3]  = lun_low_lba_start_ex[2] ;
		}
}
void udisk_init_lun_parameter(void)
{
	lun_num = 0;
	lun_low_status[0] = 0;
	lun_low_status[1] = 1;
	lun_low_status[2] = 2;
	lun_low_status[3] = 3;

	//
	udisk_get_luns_lba_start();
#if 0	
printf("lun_low_lba_start_ex[0]  = 0x%08x, [1]  = 0x%08x, \n[2]  = 0x%08x, [3]  = 0x%08x, \n",lun_low_lba_start_ex[0],lun_low_lba_start_ex[1],lun_low_lba_start_ex[2],lun_low_lba_start_ex[3]);		
#endif	
	//
		{
		if((lun_low_lba_start_ex[1]  -lun_low_lba_start_ex[0] ) >= LUNLBA_MIN)
			{
			lun_low_status[0] |= (mask_lun_status_enable|mask_lun_status_ready|mask_lun_status_protect);
			pudisk_lun_low_ex[lun_num ++] = 0;	//one lun
			}
		if((lun_low_lba_start_ex[2]  -lun_low_lba_start_ex[1] ) >= LUNLBA_MIN)
			{
			lun_low_status[1] |= (mask_lun_status_enable |mask_lun_status_ready);
			pudisk_lun_low_ex[lun_num ++] = 1;  //one lun
			}
#ifdef DISK_MAX_THREE_ICON
//for max three disk icon
		if((lun_low_lba_start_ex[3]  -lun_low_lba_start_ex[2] ) >= LUNLBA_MIN)
			{
			lun_low_status[2] |= (mask_lun_status_enable |mask_lun_status_ready |mask_lun_status_protect);
			pudisk_lun_low_ex[lun_num ++] = 2;  //one lun
			}
#else
//for max two disk icon
		if(lun_low_status[0] & mask_lun_status_enable)
			{
			if((lun_low_lba_start_ex[3]  -lun_low_lba_start_ex[2] ) >= LUNLBA_MIN)
				{
				lun_low_status[2] |= (mask_lun_status_enable |mask_lun_status_ready |mask_lun_status_protect);
				pudisk_lun_low_ex[0] = 2;	//first using lun2 as lun0
				}
			}
#endif
	//key lun
			{
			lun_low_status[3] |= (mask_lun_status_enable |mask_lun_status_ready|mask_lun_status_protect);
			pudisk_lun_low_ex[lun_num ++] = 3;  //one lun
			}
		}
}
//--------------------------------------------------------------------
//this is using for check write protect before do writing
#define GPIO_BASE_ADDR		0x9803F500
#define GPIO_DATA			(GPIO_BASE_ADDR)
#define GPIO_DIR				(GPIO_BASE_ADDR + 4)
#define GPIO_PROTECT_MASK  ((unsigned int)(1<<14))
#define GPIO_LED_MASK  		((unsigned int)(1<<22))

void udisk_check_protect_init(void)
{
	REG32(GPIO_DIR) &= (~GPIO_PROTECT_MASK);// GPIO_DIR 第14位置0 
}
void udisk_check_protect(void)
{
#if 0
	//check wp pin status
	if(REG32(GPIO_DATA) & GPIO_PROTECT_MASK)
		{//write enable
		nand_media_sts &= (~nand_media_sts_wrprot);
		//printf("no protected \n");
		}
	else
		{
		//protected
		nand_media_sts |= (nand_media_sts_wrprot);
		//nand_media_sts &= (~nand_media_sts_wrprot);
		//printf("protected \n");
		}
#else
	nand_media_sts &= (~nand_media_sts_wrprot);
#endif

}
#if 0
//flash LED
void udisk_led_init(void)
{
	REG32(GPIO_DIR) |= (GPIO_LED_MASK);
}
void udisk_led(unsigned int on)
{
	if(LED_ON == on)
		{
		//turn on led
		//REG32(GPIO_DATA) |= GPIO_LED_MASK;
		REG32(GPIO_DATA) &= (~GPIO_LED_MASK);
		}
	else
		{
		//turn off led
		//REG32(GPIO_DATA) &= (~GPIO_LED_MASK);
		}
	return;
}
#endif
//extern unsigned char nand2k_need_copy_tail;
//build log2phy table,init lun parameter
void udisk_init(void)
{
	unsigned int buff_nand_page[nand_page_size/4];
	unsigned int buff_nand_redt[nand_redt_buffer_size/4];
	// do not need copy data
	nand2k_need_copy_tail = 1;
	nand_page_buff = (unsigned char *)buff_nand_page;
	nand_redt_data = (unsigned char *)buff_nand_redt;
	//do led and write protect init
	//udisk_led_init();
	udisk_check_protect_init();
	//init nand parameter
	nand_media_init_nand_parameter();
	//
	//it is for memory using,maybe is out of using
       udisk_do_init = TRUE;
	//udisk_do_init = FALSE;
	//
	restore_udisk_log2phy_buff();
	udisk_init_lun_parameter();
#if DEBUG	//
printf("lun_num = %d, lun_low_status[0]  = 0x%02x, \n[1]  = 0x%02x, [2]  = 0x%02x, [3]  = 0x%02x, \n",lun_num,lun_low_status[0],lun_low_status[1],lun_low_status[2],lun_low_status[3]);		
printf("pudisk_lun_low_ex[0]  = 0x%02x, [1]  = 0x%02x, \n[2]  = 0x%02x, [3]  = 0x%02x, \n",pudisk_lun_low_ex[0],pudisk_lun_low_ex[1],pudisk_lun_low_ex[2],pudisk_lun_low_ex[3]);		
printf("lun_low_lba_start_ex[0]  = 0x%08x, [1]  = 0x%08x, \n[2]  = 0x%08x, [3]  = 0x%08x, \n",lun_low_lba_start_ex[0],lun_low_lba_start_ex[1],lun_low_lba_start_ex[2],lun_low_lba_start_ex[3]);		
#endif

#ifdef DISK_TEST_MODE_ENABLE	
	udisk_test();
#endif
	return;
}
//build log2phy table if it's info is changed
void restore_udisk_log2phy_buff(void)
{
	if(udisk_do_init == TRUE)
		{
		nand_init_media();
		udisk_do_init = FALSE;
		printf("nand_init_media...\n");
		}
}
unsigned char * release_udisk_log2phy_buff(void)
{
	//udisk_do_init = TRUE;
	return (nand_log2phy_map_buff);
}
//check cbw and process cbw
void udisk_poll(void)
{
	CBW cbw;
	if(TRUE == ums_cbw_probe(&cbw))
  		{
  		
  		//printf("udisk_poll handle...\n");
  		if(check_cbw_usbkey(&cbw))			
  			process_cbw(&cbw);
		else
			process_usbkey(&cbw);
  		}		
}
//lba writing,it is used by cbw processing
//input parameter is the ptr to int,it includes
//index_udisk_lba_start
//index_udisk_lba_xfer_count
//index_udisk_lba_split_max_count
//index_udisk_lba_split_count
unsigned int udisk_write(unsigned int * udisk_lba_param)
{
	if(nand_media_sts&0x40)
		{
		if(K_SUCCES != udisk_write_2k(udisk_lba_param))
			{
			return K_ERROR;
			}
		return K_SUCCES;
		}
	else
		{
		if(K_SUCCES != nand_write_begin_xfer(udisk_lba_param))
			{
			return K_ERROR;
			}
		
		while(udisk_lba_param[index_udisk_lba_xfer_count])
			{
			if(udisk_lba_param[index_udisk_lba_xfer_count] > udisk_lba_param[index_udisk_lba_split_max_count])
				{
				udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_split_max_count];
				}
			else
				{
				udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_xfer_count];
				}
		//
		
			if(K_SUCCES != nand_write_begin_split(udisk_lba_param))
				{
				nand_soft_reset();
				return K_ERROR;
				}
			udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_count];
			udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_count];
		//
			if(K_SUCCES != nand_write_end_split(udisk_lba_param))
				{
				nand_soft_reset();
				return K_ERROR;
				}
			}
		if(K_SUCCES != nand_write_end_xfer(udisk_lba_param))
			{
			nand_soft_reset();
			return K_ERROR;
			}
		}
	return K_SUCCES;
}

unsigned int nand_check_exdata_blank_2k()
{
    	unsigned int i;
	nand_read_extra_data_2k();
	//printf("nand_redt_data[6]=%x",nand_redt_data[6]);
//print_buf(nand_redt_data,16);	
	for(i=6;i<8;i++)
		{
		if(nand_redt_data[i]!=0xff)
			return K_ERROR;
		}
	return K_SUCCES;
}
extern unsigned char nand_addr_segment ;
unsigned int nand_block_check_space_2k(unsigned int * udisk_lba_param)
{
	unsigned int i;
	unsigned char addr_page_save,addr_segment_save;
	
	addr_page_save=nand_addr_page;
	addr_segment_save=nand_addr_segment;
	for(i=0;i<udisk_lba_param[index_udisk_lba_split_max_count];i++)
		{
		if(K_SUCCES != nand_check_exdata_blank_2k())
			{
			//printf("This segment is not blank !!\n");
			nand_addr_page=addr_page_save;
			nand_addr_segment=addr_segment_save;
			return 0;			
			}
		nand_addr_segment++;
		if (nand_addr_segment==4)
			{
			nand_addr_segment=0;
			nand_addr_page++;
			}
		}
	nand_addr_page=addr_page_save;
	nand_addr_segment=addr_segment_save;
	return 1;
	
	
}

unsigned int nand2k_media_copy_block_head(unsigned int * udisk_lba_param)
{
	unsigned int segment_num,copy_num;
	unsigned char bak_nand_addr_page,bak_nand_addr_segment;       
// first copy head
       copy_num = (nand_addr_page<<2)+nand_addr_segment;
	if(copy_num)
		{
#ifdef DISK_TEST_MODE_ENABLE		
printf("copy_block_head, copy_num=0x%02x ,\n",copy_num);	
#endif
		bak_nand_addr_page = nand_addr_page;
		bak_nand_addr_segment = nand_addr_segment;
		nand_addr_page = 0;
		nand_addr_segment=0;
		if(K_SUCCES != nand_media_copy_block_2k(copy_num))
			{
			printf("nand_media_copy_block_2k error!!!\n");
			return K_ERROR;
			}
		nand_addr_page = bak_nand_addr_page;
		nand_addr_segment = bak_nand_addr_segment;
		}  
	return K_SUCCES;
}

// if flag=0:copy tail; flag=1 maybe not copy tail;
unsigned int nand2k_media_copy_block_tail(unsigned int * udisk_lba_param, unsigned char flag)
{
	unsigned int copy_num,temp;
	unsigned short wr_addr_save,rd_addr_save;
	unsigned char bak_nand_addr_page,bak_nand_addr_segment;
	unsigned char *nand_assign_map;
	nand_assign_map = (nand_addr_zone ? (nand_assign_map_buff + nand_assign_map_one_zone_size) : nand_assign_map_buff);
	bak_nand_addr_page = nand_addr_page;
	bak_nand_addr_segment = nand_addr_segment;
	copy_num = (nand_addr_page<<2)+nand_addr_segment;
// second copy tail
	if(flag)
       	copy_num += udisk_lba_param[index_udisk_lba_xfer_count];
	if(0x100>copy_num)
		{
		nand_addr_page = copy_num>>2;
		nand_addr_segment=copy_num&3;
		// set copy len 
              copy_num = 256-copy_num;

#ifdef DISK_TEST_MODE_ENABLE
printf("copy_block_tail, copy_num=0x%02x, nand_addr_page=0x%02x, nand_addr_segment=0x%02x, \n",copy_num,nand_addr_page,nand_addr_segment);		
#endif		
		if(K_SUCCES != nand_media_copy_block_2k(copy_num))
			{
			printf("nand_media_copy_block_2k error!!!\n");
			return K_ERROR;
			}
		nand_addr_page = bak_nand_addr_page;
		nand_addr_segment = bak_nand_addr_segment;	   
		}
	
// erase old block	
	wr_addr_save = nand_addr_wr_phy_blk;
	rd_addr_save = nand_addr_rd_phy_blk;
	nand_addr_wr_phy_blk = nand_addr_rd_phy_blk;
	
#ifdef DISK_TEST_MODE_ENABLE		
printf("erase phy block = 0x%08x, \n",nand_addr_wr_phy_blk);	
#endif

	b_map_phy_blk_free(nand_assign_map, nand_addr_wr_phy_blk);
	if(K_SUCCES != nand_erase_block_2k())
		{
		printf("nand_erase_block_2k error!!!\n");
		return K_ERROR;
		}
	nand_addr_wr_phy_blk = wr_addr_save;
	nand_addr_rd_phy_blk = rd_addr_save;
	return K_SUCCES;	
}

void print_goble()
{
	printf("nand_addr_page = 0x%02x ,\n",nand_addr_page);
	printf("nand_addr_segment = 0x%02x ,\n",nand_addr_segment);
	printf("nand_addr_zone = 0x%02x ,\n",nand_addr_zone);
	printf("nand_addr_log_blk = 0x%04x ,\n",nand_addr_log_blk);
	printf("nand_addr_wr_phy_blk = 0x%04x ,\n",nand_addr_wr_phy_blk);
	printf("nand_addr_rd_phy_blk = 0x%04x ,\n",nand_addr_rd_phy_blk);
}

void nand2k_copy_block_tail(unsigned int * udisk_lba_param)
{
	unsigned short  bak_rd_phy_blk, bak_wr_phy_blk;
	unsigned char bak_addr_page,bak_addr_segment;
	
	bak_rd_phy_blk = nand_addr_rd_phy_blk;
	bak_wr_phy_blk = nand_addr_wr_phy_blk;
	bak_addr_page = nand_addr_page;
	bak_addr_segment = nand_addr_segment;
	
	nand_addr_rd_phy_blk = nand2k_copy_rd_blk;
	nand_addr_wr_phy_blk = nand2k_copy_wr_blk;
	nand_addr_page = nand2k_copy_start_page;
	nand_addr_segment = nand2k_copy_start_segment;		
	nand2k_media_copy_block_tail(udisk_lba_param,0);
	nand2k_need_copy_tail = 1;
	
	nand_addr_rd_phy_blk = bak_rd_phy_blk;
	nand_addr_wr_phy_blk = bak_wr_phy_blk;
	nand_addr_page = bak_addr_page;
	nand_addr_segment = bak_addr_segment;
}



unsigned int nand_write_2k_split(unsigned int * udisk_lba_param)
{
       unsigned int i,temp,wr_cnt,rd_cnt;
	unsigned char qbuf[512];   
	unsigned char sts,pbuf_sts,qbuf_sts,wr_sts;
	// buffer status have 3 state, 0: no data ready; 1: data ready, not used; 2: data ready, used;
	pbuf_sts = qbuf_sts = wr_sts = 0; // no valid
	rd_cnt = wr_cnt = 0;
       nand_media_bind_log2phy();
	
	while(wr_cnt<udisk_lba_param[index_udisk_lba_split_max_count])	
		{
		if(rd_cnt<udisk_lba_param[index_udisk_lba_split_max_count])
			{
			if((0==pbuf_sts)&&(1!=qbuf_sts))
			    	{
			    	#if 1
				if(TRUE == lun_security_enable)
					{
					g_usb_receive(nand_page_buff, 512);
					en_des(nand_page_buff, 128);
					}
				else
					g_usb_receive(nand_page_buff, 512);
					
				#else
				g_usb_receive(nand_page_buff, 512);
				if(TRUE == lun_security_enable)
					en_des(nand_page_buff, 128);
								    
			    #endif	
			    rd_cnt++;
			    pbuf_sts = 1; // set p buf valid
			    }
			}
		if(!wr_sts) // if wr_sts=1 write data not complete
			{
/*****************/
    			nand_set_cmd(0x80);
			nand_set_write_addr_2k();
			//reset ECC here
			nand_reset_ecc();
			if(1==pbuf_sts)
				{
				#if 1
				//start_des_data_crypt(qbuf);
				//udisk_usb_bulk_start_ex(qbuf);
				for(i = 0; i < 512;)
					{
					nand_set_data(nand_page_buff[i]);
					nand_set_data(nand_page_buff[i+1]);
					nand_set_data(nand_page_buff[i+2]);
					nand_set_data(nand_page_buff[i+3]);
					nand_set_data(nand_page_buff[i+4]);
					nand_set_data(nand_page_buff[i+5]);
					nand_set_data(nand_page_buff[i+6]);
					nand_set_data(nand_page_buff[i+7]);
					nand_set_data(nand_page_buff[i+8]);
					nand_set_data(nand_page_buff[i+9]);
					nand_set_data(nand_page_buff[i+10]);
					nand_set_data(nand_page_buff[i+11]);
					nand_set_data(nand_page_buff[i+12]);
					nand_set_data(nand_page_buff[i+13]);
					nand_set_data(nand_page_buff[i+14]);
					nand_set_data(nand_page_buff[i+15]);
					i += 16;
					//udisk_usb_bulk_out_ex();
					}
				#else
				for(i = 0; i <512; i++)				
					nand_set_data(nand_page_buff[i]);
				#endif
				pbuf_sts=2;
				}
			else
				{
				if(1==qbuf_sts)
					{
					#if 1
				//start_des_data_crypt(nand_page_buff);
				//udisk_usb_bulk_start_ex(nand_page_buff);
				for(i = 0; i < 512;)
					{
					nand_set_data(qbuf[i]);
					nand_set_data(qbuf[i+1]);
					nand_set_data(qbuf[i+2]);
					nand_set_data(qbuf[i+3]);
					nand_set_data(qbuf[i+4]);
					nand_set_data(qbuf[i+5]);
					nand_set_data(qbuf[i+6]);
					nand_set_data(qbuf[i+7]);
					nand_set_data(qbuf[i+8]);
					nand_set_data(qbuf[i+9]);
					nand_set_data(qbuf[i+10]);
					nand_set_data(qbuf[i+11]);
					nand_set_data(qbuf[i+12]);
					nand_set_data(qbuf[i+13]);
					nand_set_data(qbuf[i+14]);
					nand_set_data(qbuf[i+15]);
					i += 16;
					//udisk_usb_bulk_out_ex();
					}
				#else
					for(i = 0; i <512; i++)				
						nand_set_data(qbuf[i]);
					#endif
					qbuf_sts=2;
					}	
				}
				
			nand_get_hw_and_set_sw_ecc();
			nand_set_cmd(0x85);
			if(K_SUCCES!= nand_wait_rdy_with_timeout(2))
				{
				nand_reset_device();
				printf("nand not ready!");
		      	 	return K_ERROR;
				}
	
			nand_set_addr(0x00+nand_addr_segment*nand_redt_buffer_size);
			nand_set_addr(0x08); 

			//reset ECC here
			nand_reset_ecc();
			for(i = 0; i <nand_redt_buffer_size; i++)
				{
				nand_set_data(nand_redt_data[i]);
			//printf(" %x ",nand_redt_data[i]);
				}

			CheckWriteProtectInWriting();	
			nand_set_cmd(0x10);	
			}
		if(nand_is_ready())
			{ // ready
			wr_sts = 0; 
			}
		else
			{ // no ready
			wr_sts = 1;  
			}
		if(!wr_sts) // if wr_sts=1 write data not complete
			{
			nand_set_cmd(0x70);
			while(((i=nand_get_data())&(kbm_nand_status_ready))!=kbm_nand_status_ready)								
				CheckWriteProtectInWriting(); //writing protect detecting				
			nand_set_standby();
			if(i & kbm_nand_status_write_failed)
				{
				printf("nand_write_2k_split error\n");
				return K_ERROR;
				}
			if(2==pbuf_sts)
				pbuf_sts=0;
			if(2==qbuf_sts)
				qbuf_sts=0;			
			nand_addr_segment++;
			nand_addr_page +=(nand_addr_segment>>2);
			nand_addr_segment &= 3;			
			wr_cnt++;
			}
/*****************/	
		if(rd_cnt<udisk_lba_param[index_udisk_lba_split_max_count])
			{
			if((0==qbuf_sts)&&(1!=pbuf_sts))
			    {
			    #if 1
			 	if(TRUE == lun_security_enable)
					{
					g_usb_receive(qbuf, 512);
					en_des(qbuf, 128);
					}
				else
					g_usb_receive(qbuf, 512);
				#else
				g_usb_receive(qbuf, 512);
				if(TRUE == lun_security_enable)
					en_des(qbuf, 128);
			    #endif				    
			    rd_cnt++;
			    qbuf_sts = 1; // set p buf valid
			    }
			}
		}
	udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_max_count];
	udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_max_count];
	return K_SUCCES;	
}

unsigned int udisk_write_2k(unsigned int * udisk_lba_param)
{
      unsigned int temp; 
#ifdef DISK_TEST_MODE_ENABLE
printf("\nW nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_write_segment_num = 0x%08x\n",udisk_lba_param[index_udisk_lba_xfer_count]);
#endif
       if(0==nand2k_need_copy_tail)
       	{
       	temp = nand2k_copy_log_blk;
#ifdef DISK_TEST_MODE_ENABLE
printf("\n nand2k_copy_log_blk = 0x%08x\n",temp);
#endif		
		if(temp != (udisk_lba_param[index_udisk_lba_start]>>8))			
			{// copy the last tail				
			nand2k_copy_block_tail(udisk_lba_param);			
			}
       	}       	   
       while(0!=udisk_lba_param[index_udisk_lba_xfer_count])
       	{
       	if(K_SUCCES!=nand_write_log2phy_2k(udisk_lba_param))
			{
			printf("nand_write_log2phy_2k error!!!!");
			return K_ERROR;
			}
	//print_goble();
	//printf("nand_addr_page = 0x%02x ,\n",nand_addr_page);
	//printf("nand_addr_segment = 0x%02x ,\n",nand_addr_segment);
	//printf("nand_addr_zone = 0x%02x ,\n",nand_addr_zone);
	//printf("nand_addr_log_blk = 0x%04x ,\n",nand_addr_log_blk);
	//printf("nand_addr_wr_phy_blk = 0x%04x ,\n\n",nand_addr_wr_phy_blk);
	//printf("nand_addr_rd_phy_blk = 0x%04x ,\n",nand_addr_rd_phy_blk);
#ifdef DISK_TEST_MODE_ENABLE		
print_goble();
printf("index_udisk_lba_split_max_count = 0x%08x ,\n",udisk_lba_param[index_udisk_lba_split_max_count]);
#endif
		if(K_SUCCES!=nand_write_2k_split(udisk_lba_param))
			{
			printf("nand_write_2k_split error!!!!");
			return K_ERROR;
			}
#ifdef DISK_TEST_MODE_ENABLE
printf("start_LBA = 0x%08x, segment_num = 0x%08x, write end...\n\n",udisk_lba_param[index_udisk_lba_start],udisk_lba_param[index_udisk_lba_xfer_count]);
#endif
       	}
	return K_SUCCES;		
}

unsigned int nand2k_read_split(unsigned int * udisk_lba_param)
{
#if 1
       unsigned int i,sd_cnt;
       unsigned char *nand_log2phy_map;
	nand_log2phy_map = (nand_addr_zone ? (nand_log2phy_map_buff + buff_nand_log2phy_map_one_zone_size): nand_log2phy_map_buff);
	if(nand_addr_rd_phy_blk == nand_map_block_free)
		{
		i = nand_addr_wr_phy_blk;
		if(K_SUCCES!= nand_map_alloc_wr_blk())//循环查找空的block
			{
			return K_ERROR;
			}
		nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
		nand_map_log2phy_set(nand_log2phy_map,nand_addr_log_blk, (nand_addr_rd_phy_blk | nand_map_soft_l2p_binding));
		nand_addr_wr_phy_blk = i;
		memset(nand_page_buff,0xff,512);
		for(sd_cnt=0;sd_cnt<udisk_lba_param[index_udisk_lba_split_max_count];sd_cnt++)
			{
			usb_send(nand_page_buff,512);
			}
		}
	else
		{
		//printf("%x \n",udisk_lba_param[index_udisk_lba_split_max_count]);
		//bulk_buff_ex = nand_page_buff;
		//-----------------
		//udisk_bulk_init_ex();
		//start_des_data_init();
		//------------------
		for(sd_cnt=0;sd_cnt<udisk_lba_param[index_udisk_lba_split_max_count];sd_cnt++)
			{			
			#if 0
			nand_read_segment_2k();
			#else			
			nand_set_cmd(0);
			nand_set_read_addr_2k();
			nand_set_cmd(0x30);
			//reset ECC here
			nand_reset_ecc();

			if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
				{
				nand_reset_device();
				printf("nand not ready!");
				return K_ERROR;
				}
			
			#if 1
			//start_des_data_crypt(nand_page_buff);
			//udisk_usb_bulk_start_ex(nand_page_buff);
			for(i = 0; i < 512;)
				{
				nand_page_buff[i]=nand_get_data();
				nand_page_buff[i+1]=nand_get_data();
				nand_page_buff[i+2]=nand_get_data();
				nand_page_buff[i+3]=nand_get_data();
				nand_page_buff[i+4]=nand_get_data();
				nand_page_buff[i+5]=nand_get_data();
				nand_page_buff[i+6]=nand_get_data();
				nand_page_buff[i+7]=nand_get_data();
				nand_page_buff[i+8]=nand_get_data();
				nand_page_buff[i+9]=nand_get_data();
				nand_page_buff[i+10]=nand_get_data();
				nand_page_buff[i+11]=nand_get_data();
				nand_page_buff[i+12]=nand_get_data();
				nand_page_buff[i+13]=nand_get_data();
				nand_page_buff[i+14]=nand_get_data();
				nand_page_buff[i+15]=nand_get_data();
				i +=16;
				//des_decrypt();
				//udisk_usb_bulk_in_ex();
				}
			
			//while(FALSE == udisk_usb_bulk_done_ex())
				//{
				//des_decrypt();
				//udisk_usb_bulk_in_ex();
				//}
			//start usb send
			//
			if(TRUE == lun_security_enable)
				de_des(nand_page_buff, 128);
			usb_send(nand_page_buff,512);
			while(!nand_is_ready());
			//{
			//des_decrypt();
			//udisk_usb_bulk_in_ex();
			//}
			#else
			for(i = 0; i < 0x200; i++)
				{
				nand_page_buff[i] = nand_get_data();
				}
			#endif
			#if 0 
			if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
				{
				nand_reset_device();
				return K_ERROR;
				}			
			#endif			
			#endif				
			nand_addr_segment++;
			nand_addr_page +=(nand_addr_segment>>2);
			nand_addr_segment &= 3;			
			//usb_send(nand_page_buff,512);			
			}
		}
	nand_set_standby();
	udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_max_count];
	udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_max_count]; 
	while(FALSE == udisk_usb_bulk_done_ex())
		{
		des_decrypt();
		udisk_usb_bulk_in_ex();
		}
	return K_SUCCES;
#else	
       unsigned int i,sd_cnt,rd_cnt,pbuf_ptr,qbuf_ptr;
	unsigned char qbuf[512];   
	unsigned char pbuf_sts,qbuf_sts,rd_sts;
	if(nand_addr_rd_phy_blk == nand_map_block_free)
		{
		i = nand_addr_wr_phy_blk;
		if(K_SUCCES!= nand_map_alloc_wr_blk())//循环查找空的block
			{
			return K_ERROR;
			}
		nand_addr_rd_phy_blk = nand_addr_wr_phy_blk;
		nand_map_log2phy_set(nand_log2phy_map_buff,nand_addr_log_blk, (nand_addr_rd_phy_blk | nand_map_soft_l2p_binding));
		nand_addr_wr_phy_blk = i;
		memset(nand_page_buff,0xff,512);
		for(sd_cnt=0;sd_cnt<udisk_lba_param[index_udisk_lba_split_max_count];sd_cnt++)
			{
			usb_send(nand_page_buff,512);
			}
		}
	else
		{
		sd_cnt = rd_cnt = pbuf_ptr = qbuf_ptr = 0;
		pbuf_sts = qbuf_sts  = rd_sts = 0; // 0: no data; 1: half data; 2: data ready; 3: send now;		
		while(sd_cnt<udisk_lba_param[index_udisk_lba_split_max_count])
			{			
			if((0==rd_sts)&&(rd_cnt<udisk_lba_param[index_udisk_lba_split_max_count]))
				{
printf("A sd_cnt = %d,rd_cnt = %d,pbuf_ptr = %d,qbuf_ptr = %d,\n",sd_cnt,rd_cnt,pbuf_ptr,qbuf_ptr);
printf("pbuf_sts = %d,qbuf_sts = %d,rd_sts = %d,\n",pbuf_sts,qbuf_sts,rd_sts);
				if(((0==pbuf_sts)&&(1!=qbuf_sts))||(0==qbuf_sts)&&(1!=pbuf_sts))
					{
					if(0==pbuf_sts)
						{
						pbuf_sts = 1;
						pbuf_ptr = 0;
						}
					else
						{
						qbuf_sts = 1;
						qbuf_ptr = 0;
						}
					nand_set_cmd(0);
					nand_set_read_addr_2k();
					nand_set_cmd(0x30);
					//reset ECC here
					nand_reset_ecc();

					if(K_SUCCES!= nand_wait_rdy_with_timeout(k_nand_busy_read_timeout))
						{
						nand_reset_device();
						printf("nand not ready!");
						return K_ERROR;
						}
					rd_sts =1;
					}
				}
			if((1==pbuf_sts)||(1==qbuf_sts))
				{
				if(1==pbuf_sts)
					{
					if(pbuf_ptr<512)
						nand_page_buff[pbuf_ptr++] = nand_get_data();
					else
						{ // have read 512 bytes data
						pbuf_sts = 2;
						pbuf_ptr = 0;
						rd_cnt++;
						rd_sts =0;
						}
					}
				else
					{
					if(qbuf_ptr<512)
						qbuf[qbuf_ptr++] = nand_get_data();
					else
						{ // have read 512 bytes data
						qbuf_sts = 2;
						qbuf_ptr = 0;
						rd_cnt++;
						rd_sts =0;
						}					
					}			
				}
			if((2==pbuf_sts)||(2==qbuf_sts))
				{
printf("C sd_cnt = %d,rd_cnt = %d,pbuf_ptr = %d,qbuf_ptr = %d,\n",sd_cnt,rd_cnt,pbuf_ptr,qbuf_ptr);
printf("pbuf_sts = %d,qbuf_sts = %d,rd_sts = %d,\n",pbuf_sts,qbuf_sts,rd_sts);				
				if((2==pbuf_sts)&&(3!=qbuf_sts))
					pbuf_sts = 3;
				if((2==qbuf_sts)&&(3!=pbuf_sts))
					qbuf_sts = 3;
					
				}
			if((3==pbuf_sts)||(3==qbuf_sts))
				{
printf("D sd_cnt = %d,rd_cnt = %d,pbuf_ptr = %d,qbuf_ptr = %d,\n",sd_cnt,rd_cnt,pbuf_ptr,qbuf_ptr);
printf("pbuf_sts = %d,qbuf_sts = %d,rd_sts = %d,\n",pbuf_sts,qbuf_sts,rd_sts);				
				if(3==pbuf_sts)
					{
					if(pbuf_ptr<512)
						{
						bulk_buff_ptr = pbuf_ptr>>2;
						bulk_buff_ex = nand_page_buff;
						udisk_usb_bulk_in_2k();
						pbuf_ptr = bulk_buff_ptr<<2;
						}
					else
						{ // have send 512 bytes data
						pbuf_sts = 0;
						sd_cnt++;
						nand_addr_segment++;
						nand_addr_page +=(nand_addr_segment>>2);
						nand_addr_segment &= 3;
						
						}
					}
				else
					{
					if(qbuf_ptr<512)
						{
						bulk_buff_ptr = qbuf_ptr>>2;
						bulk_buff_ex = qbuf;
						udisk_usb_bulk_in_2k();
						qbuf_ptr = bulk_buff_ptr<<2;
						}
					else
						{ // have send 512 bytes data
						qbuf_sts = 0;
						sd_cnt++;
						nand_addr_segment++;
						nand_addr_page +=(nand_addr_segment>>2);
						nand_addr_segment &= 3;
						
						}					
					}			
				}
							
						
			//usb_send(nand_page_buff,512);			
			}
		}
	nand_set_standby();
	udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_max_count];
	udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_max_count];     
	return K_SUCCES;
#endif
}

void udisk_read_2k(unsigned int * udisk_lba_param)
{
#ifdef DISK_TEST_MODE_ENABLE
printf("\nR nand_addr_start = 0x%08x\n",udisk_lba_param[index_udisk_lba_start]);
printf("nand_read_segment_num = 0x%08x\n",udisk_lba_param[index_udisk_lba_xfer_count]);
#endif
       
       while(0!=udisk_lba_param[index_udisk_lba_xfer_count])
       	{
       	if(K_SUCCES!=nand_read_log2phy_2k(udisk_lba_param))
			{
			printf("nand_write_log2phy_2k error!!!!");
			return K_ERROR;
			}
	//print_goble();
	//printf("nand_addr_page = 0x%02x ,\n",nand_addr_page);
	//printf("nand_addr_segment = 0x%02x ,\n",nand_addr_segment);
	//printf("nand_addr_zone = 0x%02x ,\n",nand_addr_zone);
	//printf("nand_addr_log_blk = 0x%04x ,\n",nand_addr_log_blk);
	//printf("nand_addr_wr_phy_blk = 0x%04x ,\n",nand_addr_wr_phy_blk);
	//printf("nand_addr_rd_phy_blk = 0x%04x ,\n\n",nand_addr_rd_phy_blk);
#ifdef DISK_TEST_MODE_ENABLE		
print_goble();
printf("index_udisk_lba_split_max_count = 0x%08x ,\n",udisk_lba_param[index_udisk_lba_split_max_count]);
#endif
		if(K_SUCCES!=nand2k_read_split(udisk_lba_param))
			{
			printf("nand_write_2k_split error!!!!");
			return K_ERROR;
			}
#ifdef DISK_TEST_MODE_ENABLE
printf("start_LBA = 0x%08x, segment_num = 0x%08x, read end...\n\n",udisk_lba_param[index_udisk_lba_start],udisk_lba_param[index_udisk_lba_xfer_count]);
#endif
       	}	
	return K_SUCCES;	
}

#ifdef DISK_TEST_MODE_ENABLE
//this is used in test mode,nomoral mode doesn't use it
unsigned int debug_udisk_write(unsigned int * udisk_lba_param)
{
	if(K_SUCCES != nand_write_begin_xfer(udisk_lba_param))
		{
			return K_ERROR;
		}
	while(udisk_lba_param[index_udisk_lba_xfer_count])
		{
		if(udisk_lba_param[index_udisk_lba_xfer_count] > udisk_lba_param[index_udisk_lba_split_max_count])
			{
			udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_split_max_count];
			}
		else
			{
			udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_xfer_count];
			}
		if(udisk_rd_wr_self_enable == TRUE)
			{
				debug_set_write_data();
			}
		if(nand_media_sts&0x40)
			{
			if(K_SUCCES != nand_write_begin_split_2k(udisk_lba_param))
				{
				nand_soft_reset();
				return K_ERROR;
				}
			}
		else
			{
			if(K_SUCCES != nand_write_begin_split(udisk_lba_param))
				{
				nand_soft_reset();
				return K_ERROR;
				}
			}
		//----------------------------------------------------------------------
		udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_count];
		udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_count];
		//usb residue should do here
		//---------------------------------------------------------------------
		if(K_SUCCES != nand_write_end_split(udisk_lba_param))
			{
			nand_soft_reset();
			return K_ERROR;
			}
		}
	if(K_SUCCES != nand_write_end_xfer(udisk_lba_param))
		{
		nand_soft_reset();
		return K_ERROR;
		}
	return K_SUCCES;
}
#endif
//lba read
unsigned int udisk_read(unsigned int * udisk_lba_param)
{
	unsigned char b_first = TRUE;

	if(K_SUCCES != nand_read_begin_xfer(udisk_lba_param))
		{
			return K_ERROR;
		}
	//----------------------------------------------------------------
	while(udisk_lba_param[index_udisk_lba_xfer_count])
		{
		if(udisk_lba_param[index_udisk_lba_xfer_count] > udisk_lba_param[index_udisk_lba_split_max_count])
			{
			udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_split_max_count];
			}
		else
			{
			udisk_lba_param[index_udisk_lba_split_count] = udisk_lba_param[index_udisk_lba_xfer_count];
			}
		
		//printf("512 flash!!!!!");
		if(K_SUCCES != nand_read_begin_split(udisk_lba_param))
				{
				nand_soft_reset();
				return K_ERROR;
				}
		//----------------------------------------------------------------------
		udisk_lba_param[index_udisk_lba_start] += udisk_lba_param[index_udisk_lba_split_count];
		udisk_lba_param[index_udisk_lba_xfer_count] -= udisk_lba_param[index_udisk_lba_split_count];
		//usb residue should do here
		//
		//---------------------------------------------------------------------
		if(b_first == TRUE)
			{
			nand_read_end_split_first(udisk_lba_param);
			b_first = FALSE;
			}
		else
			{
			nand_read_end_split(udisk_lba_param);
			}	
		}
	//--------------------------------------------------------------------
	nand_read_end_xfer(udisk_lba_param);
	return K_SUCCES;
}

void udisk_send_usb(unsigned char * buff,unsigned short len)
{
#ifdef DISK_TEST_MODE_ENABLE
	if(udisk_rd_wr_self_enable == FALSE)
#endif		
		{
		usb_send(buff, len);
		}
}
void udisk_receive_usb(unsigned char * buff,unsigned short len)
{
#ifdef DISK_TEST_MODE_ENABLE
	if(udisk_rd_wr_self_enable == FALSE)
#endif		
		{
		usb_receive(buff, len);
		}
}
//======================================================
#ifdef DISK_TEST_MODE_ENABLE
void udisk_help()
{
		printf("\n/------------------------------------------------------------------/\n");
		printf("This is only for test.\n");
		printf("Commands List:\n");
		printf("1: NAND Media read ID.\n");
		printf("2: NAND Media Indentify( seek CIS, build map ).\n");
		printf("3: Display NAND Info.\n");
		printf("4: Dispaly LOG2PHY Map.\n");
		printf("5: Display PHY Block Assign Map.\n");
		printf("6: Read LBA.\n");
		printf("7: Write LBA.\n");
		printf("8: Display Nand Page Buff.\n");
		printf("9: Display Nand REDT Buff.\n");
		printf("a: Read PB.\n");
		printf("b: Wrtie PB.\n");
		printf("c: Read PB REDT.\n");
		printf("d: Wrtie PB REDT.\n");
		printf("e: Read nand_2k one page.\n");
		printf("f: Wrtie nand_2k one page.\n");
		printf("g: Erase nand_2k one block.\n");
		printf("h: Test command hlep.\n");
		printf("i: Test Writie a block.\n");
		printf("j: Test Read nand_2k a serise of segments.\n");
		printf("k: Test Writie nand_2k a segment zone.\n");
		printf("l: Test Write nand_2k a serise of segments.\n");
		printf("x: Exit Test Mode and Enter UDisk Mode.\n");
		printf("y: Erase All PHY Blocks.\n");
		printf("z: Boot Mode.\n");
		printf("Please select: 1 or 2 or ......\n");
		printf("/------------------------------------------------------------------/\n");
}

extern unsigned char nand_addr_segment ;
int receive_address(short count)
{
    int x,i;
    unsigned char d;
    x=0;   	
    for(i=0;i<count;i++)
    	{
    	d = uart_receive_byte();
    	
	if(d>0x60)
		d-=0x57;
	else
		d-=0x30;
	x |= (d<<((7-i)*4));
	printf("%01x",d);
    	}
    	printf("\n");
	return x;
}

//used for test mode
void udisk_test(void)
{
    unsigned char my_buf[2048+80];
	int *ptr,i,j;   
	unsigned char cmd;
	unsigned char bfirst = TRUE,mfirst = TRUE;
	unsigned char id_list[6];
	unsigned int udisk_lba_param[index_udisk_lba_parameter_all];
extern unsigned int nand_media_erase_all(void);	
#define Timer_Max		0x3fffff

	udisk_rd_wr_self_enable = TRUE;
	printf("input any char to start test mode");
	while(udisk_rd_wr_self_enable == TRUE)		
		{
		if(bfirst == TRUE)
			{
			unsigned int timer = 0;
			while((timer++) <Timer_Max)
				{
				if((timer &0xffff) ==0)
					printf(".");
				
				if((REG8(UART_FR)&0x10))
					continue;
				cmd = REG8(UART_DR);
				if((cmd == 's') ||(cmd == 'S'))
					break;
				}
			if(timer >= Timer_Max)
				{
				udisk_rd_wr_self_enable = FALSE;
				break;
				}
			bfirst = FALSE;
			}
		if(mfirst)
			{
		       udisk_help();
			mfirst = FALSE;   
			}
		switch(cmd = uart_receive_byte())
			{
				case 'z':
					printf("Please Do Reset and Then Enter Boot Mode.\n");
					enable_download_flag();
					break;
				case 'y':
					printf("Erase All Start....................\n");
					//nand_media_erase_all();
					printf("Erase Done.\n");
					break;
				case '1':
					
					nand_read_id(id_list);
					printf("Nand read ID %02x, %02x, %02x, %02x, ....................\n",id_list[0],id_list[1],id_list[2],id_list[3]);
					break;
				case '2':
					printf("Indentify start.........................\n");
					//nand_media_identify();
					//for zone 1
					//nand_addr_zone = 1;
					//nand_map_build_sector_map();
					//nand_assign_zone = 0;
					//
					//printf("Indentify Done.\n");
					break;
				case '3':
					printf("Display NAND Info Start..............................\n");
					//debug_printf_nand_info();
					printf("Done\n");
					break;
				case '4':
					printf("Dispaly LOG2PHY Map Start..............................\n");
					//debug_printf_l2p_map();
					printf("Done\n");
					break;
				case '5':
					printf("Display PHY Block Assign Map Start..............................\n");
					//debug_printf_assign_map();
					printf("Done\n");
					break;
				case '6':
					
					
					printf("Read Start..............................\n");
					/*debug_get_lba(udisk_lba_param);
					if(K_SUCCES != udisk_read(udisk_lba_param))
						{
						printf("udisk_read() error.s\n");
						}
					printf("Done\n");
					*/
					break;
				case '7':
					
					printf("Write Start..............................\n",cmd);
					/*debug_get_lba(udisk_lba_param);
					debug_get_write_data();
					debug_udisk_write(udisk_lba_param);
					printf("Done\n");
					*/
					break;
				case '8':
					printf("Display Nand Page Buff Start..............................\n");
					//debug_page_buff(nand_page_buff,512);
					printf("Done\n");
					break;
				case '9':
					printf("Display Nand REDT Buff Start..............................\n");
					//debug_page_buff(nand_redt_data,16);
					printf("Done\n");
					break;
				case 'b':
					{
					printf("Write PB start..............................\n");
					//debug_get_lba(udisk_lba_param);
					//debug_get_write_data();
					//nand_write_sector_test(udisk_lba_param[index_udisk_lba_start],udisk_lba_param[index_udisk_lba_xfer_count]);
					printf("Done\n");
					}
					break;
				case 'a':
					{
					printf("Read PB start..............................\n");
					//debug_get_lba(udisk_lba_param);
					//nand_read_sector_test(udisk_lba_param[index_udisk_lba_start],udisk_lba_param[index_udisk_lba_xfer_count]);
					printf("Done\n");
					}
					break;
				case 'c':
					{
					printf("Read PB REDT Start..............................\n");
					//debug_get_lba(udisk_lba_param);
					//nand_read_extra_data_test(udisk_lba_param[index_udisk_lba_start],udisk_lba_param[index_udisk_lba_xfer_count]);
					printf("Done\n");
					}
					break;
				case 'd':
					printf("Command %c..............................\n",cmd);
					printf("Done\n");
					break;
				case 'e':
					//unsigned char my_buf[2048+80];
					printf("Read nand_2k one page..............................\n");
					printf("Input Read address ...\n");
					i = receive_address(8);
					printf("End Input Read address 0x%08x, ...\n",i);
					nand_read_page_test_2k(my_buf,2048+64,i);
					print_buf(my_buf,2048+64);
					printf("Done\n");
					break;	
				case 'f':
					printf("Write nand_2k one page..............................\n");
					ptr = (int *)my_buf;
					for(i=0;i<(512+16);i++)
						ptr[i] = Get_Rand();
					printf("Input Write address ...\n");
					i = receive_address(8);
					printf("End Input Write address 0x%08x, ...\n",i);
					nand_write_page_test_2k(my_buf,2048+64,i);
					print_buf(my_buf,2048+64);
					printf("Done\n");
					break;
				case 'g':
					//i = receive_address(8);
					//printf("Done 0x%08x, \n",i);
					printf("erase nand_2k one block..............................\n");
					printf("Input erase address ...\n");
					i = receive_address(8);
					printf("End Input erase address 0x%08x, ...\n",i);
					nand_erase_page_test_2k(i);
					printf("Done\n");
					break;		
				case 'h':
					udisk_help();
					break;
    		    		case 'i':
                    			printf("Write nand_2k one block..............................\n");
                    			printf("Input Write address ...\n");
					i = receive_address(8);
                    			printf("End Input Write address 0x%08x, ...\n",i);
                   			 for(j=0;j<64;j++)
                      			{nand_cmp_read_write(i);
                       			printf("write address %08x done\n",i);
                       			i+=2048;
                     			 }
                    			//if (error_times==0)
                    			//    printf("Test done OK!!!!!!!!!!!");
                    			//else
                    			//    printf("Have %08x times error!!",error_times);
                   			 break;
                		case 'j':
                    			printf("Test Read nand_2k a serise of segments..............................\n");
                    			//printf("Input Write nand_addr_zone ...\n");
					//nand_addr_zone = receive_address(2);
		      			//printf("Input Write nand_addr_rd_phy_blk ...\n");
					//nand_addr_rd_phy_blk= receive_address(2);
		      			//printf("Input Write nand_addr_page ...\n");
					//nand_addr_page= receive_address(2);
		      			//printf("Input Write nand_addr_segment ...\n");
					//nand_addr_segment= receive_address(2);
		     			printf("Input Read udisk_lba_param[index_udisk_lba_start] ...\n");
					udisk_lba_param[index_udisk_lba_start]= receive_address(8);
		     			printf("Input Read udisk_lba_param[index_udisk_lba_xfer_count] ...\n");
					udisk_lba_param[index_udisk_lba_xfer_count]= receive_address(8);
                    			udisk_read(udisk_lba_param);
                    			break;
                		case 'k':
                    			printf("Write test nand_2k one segment zone..............................\n");
                    			printf("Input Write address ...\n");
					i = receive_address(8);
                    			printf("End Input Write address 0x%08x, ...\n",i);
                    			nand_write_segment_test_2k(i); 
                    			break;  
				case 'l':
					printf("Test Write nand_2k a serise of segments..............................\n");
                    			//printf("Input Write nand_addr_zone ...\n");
					//nand_addr_zone = receive_address(2);
		      			//printf("Input Write nand_addr_rd_phy_blk ...\n");
					//nand_addr_rd_phy_blk= receive_address(2);
		      			//printf("Input Write nand_addr_page ...\n");
					//nand_addr_page= receive_address(2);
		      			//printf("Input Write nand_addr_segment ...\n");
					//nand_addr_segment= receive_address(2);
		     			printf("Input Read udisk_lba_param[index_udisk_lba_start] ...\n");
					udisk_lba_param[index_udisk_lba_start]= receive_address(8);
		     			printf("Input Read udisk_lba_param[index_udisk_lba_xfer_count] ...\n");
					udisk_lba_param[index_udisk_lba_xfer_count]= receive_address(8);
                    			udisk_write(udisk_lba_param);
                    			break;
				case 'm':
					printf("nand log2phy map is   \n");
					for (i=0;i<1500;i++)
						{
						printf("0x%03x=0x%02x, ",i,nand_log2phy_map_buff[i]);
						if((i&0x07)==0x07)
							printf("\n");
						}
					printf("\n nand assign map is   \n");
					for(i=0;i<128;i++)
						{
						printf("0x%02x=0x%02x, ",i,nand_assign_map_buff[i]);
						if((i&0x07)==0x07)
							printf("\n");
						}
					printf("%x\n",nand_assign_zone);
					break;
				case 'v':
					printf("erase all the nand_flash_2k........\n");
					nand_addr_zone=0;
					nand_addr_wr_phy_blk=0;
					for(i=0;i<nand_physical_blocks_per_zone*nand_num_zones;i++)
						{
						if (nand_erase_block_test_2k(i)!=K_SUCCES)
							{
							printf("erase error in %x block\n",i);
							break;
							}
						}
						printf("erase all the nand_flash_2k ok!!!!");
					break;
				case 'x':
					printf("Exit Test Mode......\n Enter UDisk Mode..........");
					udisk_rd_wr_self_enable = FALSE;
					break;
				default:
					printf("Unkonow Command.\n");
					break;
			}
		}

}
#endif	


