/*
  Name: 

  Description: 
*/

#define _USB_BULK_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
//#include <usbdrv.h>
#include "usb_bulk.h"

#include "udiskdes.h"
/*

volatile unsigned char bulk_status;
volatile unsigned int * bulk_buff;
volatile unsigned int bulk_len;

//#define enable_ep1_ep2_bulk_int() {i_s32( ((~0x40002) | i_l32(UDC_EPIntrMask,0)),UDC_EPIntrMask,0);}
//#define disable_ep1_ep2_bulk_int()  {i_s32( (0x40002 | i_l32(UDC_EPIntrMask,0)),UDC_EPIntrMask,0);}
#define enable_ep1_bulk_in_int() {i_s32( 0xfffefffc,UDC_EPIntrMask,0);}
//#define enable_ep1_bulk_in_int() {;}
#define disable_ep1_bulk_in_int()  {i_s32( 0xfffefffe,UDC_EPIntrMask,0);}
#define enable_ep2_bulk_out_int() {i_s32( 0xfffefffc,UDC_EPIntrMask,0);}
#define disable_ep2_bulk_out_int()  {i_s32( 0xfffefffe,UDC_EPIntrMask,0);}


void udisk_bulk_init(void)
{
	bulk_status = 0;
	bulk_buff = NULL;
	bulk_len = 0;
}
unsigned int udisk_usb_bulk_start(unsigned char dir,unsigned char *buff, unsigned int len)
{
	if(!(bulk_status & USB_BULK_EN))
		{
		//must from disable to enable
		if(dir != (USB_BULK_DIR_IN | USB_BULK_DIR_OUT))
		if((dir == USB_BULK_DIR_IN) ||(dir == USB_BULK_DIR_OUT))
			{
			if((NULL != buff ) && (0 != len)&&(!(len&0x3f))&&(len&0xffffff40))
				{
				dis_interrupt();
				bulk_buff = (unsigned int *)buff;
				bulk_len = len;
				bulk_status =(dir|USB_BULK_EN);
				if(dir == USB_BULK_DIR_IN) 
					{
					enable_ep1_bulk_in_int();
					udc_set_reg (UDC_EPIntr,0x02);//clear int flag
					}
				else
					{
					enable_ep2_bulk_out_int();
					udc_set_reg (UDC_EPIntr, 0x40000);//clear int flag
					}
				enb_interrupt();
				return TRUE;
				}
			}
		}
	return FALSE;
}
void udisk_usb_bulk_stop(void)
{
	unsigned char dir = bulk_status&(USB_BULK_DIR_IN | USB_BULK_DIR_OUT);
	if(dir == USB_BULK_DIR_IN) 
		{
		disable_ep1_bulk_in_int();
		udc_set_reg (UDC_EPIntr,0x02);//clear int flag
		}
	else if(dir == USB_BULK_DIR_OUT) 
		{
		disable_ep2_bulk_out_int();
		udc_set_reg (UDC_EPIntr, 0x40000);//clear int flag
		}
	else
		{
		disable_ep1_bulk_in_int();
		disable_ep2_bulk_out_int();
		udc_set_reg (UDC_EPIntr, 0x40002);//clear int flag
		}
	bulk_status = 0;
	bulk_buff = NULL;
	bulk_len = 0;
	return;
}
unsigned int udisk_usb_bulk_done(void)
{
	if(( bulk_buff == NULL) && ( bulk_len == 0) && (!(bulk_status & (USB_BULK_EN|USB_BULK_DIR_IN | USB_BULK_DIR_OUT))))
		{
		return TRUE;
		}
	return FALSE;
}
//this is for int isr
void udisk_usb_bulk_out(void)
{
	if((!(bulk_status & USB_BULK_EN)) ||(NULL == bulk_buff)||(0 == bulk_len))
		{
		udisk_usb_bulk_stop();
		return;
		}
	//-------------------------
	if(bulk_status& USB_BULK_DIR_OUT)
		{
		//check data
  		*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);
  		*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);
  		*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);
  		*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);*bulk_buff++ = i_l32(UDC_OutFifo, 0);
		//clear int flag
  		udc_set_reg (UDC_EPIntr, 0x40000);
		//clear out packet status
		i_s32( ((~UDC_EP_STS_OUT) & i_l32(UDC_EP2OutStatus,0)),UDC_EP2OutStatus,0);
		//
		bulk_len -= 64;
		//check bulk end
		if( bulk_len == 0)
			udisk_usb_bulk_stop();
		}
	//-------------------------
	return;
}
void udisk_usb_bulk_in(void)
{
	if((!(bulk_status & USB_BULK_EN)) ||(NULL == bulk_buff)||(0 == bulk_len))
		{
		udisk_usb_bulk_stop();
		return;
		}
	if(bulk_status& USB_BULK_DIR_IN)
		{
#define fifo_addr		(UDC_InFifo + 8 * 4)
   		i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);
  		i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);
   		i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);
   		i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);i_s32(*bulk_buff++, fifo_addr, 0);
		//confirm in packet
   		udc_set_reg(UDC_WRCNFRM, 0);
   		//clear in packet status
		i_s32( ((~UDC_EP_STS_IN) & i_l32(UDC_EP1InStatus,0)),UDC_EP1InStatus,0);
		//clear int flag
   		udc_set_reg (UDC_EPIntr, 0x02);
		//
		bulk_len -= 64;
		//check bulk end
		if( bulk_len == 0)
			udisk_usb_bulk_stop();
		}
	//-------------------------
	return;
}

*/



//===========================================================
//using query
unsigned int * bulk_buff_ex;
unsigned int bulk_buff_ptr;
void udisk_bulk_init_ex(void)
{
	bulk_buff_ptr = 128;
}
void udisk_usb_bulk_start_ex(unsigned char *buff)
{
	bulk_buff_ex = (unsigned int *)buff;
	bulk_buff_ptr = 0;
	return ;
}







