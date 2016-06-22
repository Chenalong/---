/*
  Name: 
  Description: 
*/

#ifndef _USB_BULK_H_
#define _USB_BULK_H_

//#include "usbdrv.h"

#define USB_BULK_EN			0x01
#define USB_BULK_DIR_IN		0x80
#define USB_BULK_DIR_OUT	0x40

/*
extern void udisk_bulk_init(void);
extern unsigned int udisk_usb_bulk_start(unsigned char dir,unsigned char *buff, unsigned int len);
extern void udisk_usb_bulk_stop(void);
extern unsigned int udisk_usb_bulk_done(void);
extern void udisk_usb_bulk_out(void);
extern void udisk_usb_bulk_in(void);

//---------------------------------------------------------
extern void udisk_bulk_init_ex(void);
void udisk_usb_bulk_start_ex(unsigned char *buff);
*/


#ifndef _USB_BULK_C_
extern unsigned int * bulk_buff_ex;
extern unsigned int bulk_buff_ptr;
#endif

#define fifo_addr		(UDC_InFifo + 8 * 4)
#if 0
#define udisk_usb_bulk_in_ex()			\
{										\
	if(bulk_buff_ptr <128)					\
	if(buff_des_usb_packet_map & (1<<(bulk_buff_ptr >>4)))\
	if ((i_l32(UDC_EP1InStatus,0)) & UDC_EP_STS_IN)	\
		{										\
   		i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);			\
  		i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr++], fifo_addr, 0);			\
   		udc_set_reg(UDC_WRCNFRM, 0);			\
		i_s32( ((~UDC_EP_STS_IN) & i_l32(UDC_EP1InStatus,0)),UDC_EP1InStatus,0);		\
		udc_set_reg (UDC_EPIntr, 0x0ffff);		\
		}									\
}
#define udisk_usb_bulk_out_ex()			\
{										\
	if(bulk_buff_ptr < 128)					\
	if ((i_l32(UDC_EP2OutStatus,0)) & UDC_EP_STS_OUT)	\
		{											\
  		bulk_buff_ex[bulk_buff_ptr++]=  i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr++] = i_l32(UDC_OutFifo, 0);	\
  		udc_set_reg (UDC_EPIntr, 0xffff0000);			\
		i_s32( ((~UDC_EP_STS_OUT) & i_l32(UDC_EP2OutStatus,0)),UDC_EP2OutStatus,0);	\
		buff_des_usb_packet_map = ((buff_des_usb_packet_map <<1)|0x01);\
		}											\
}
#else
#define udisk_usb_bulk_in_ex()			\
{										\
	if(bulk_buff_ptr <128)					\
	if(buff_des_usb_packet_map & (1<<(bulk_buff_ptr >>4)))\
	if ((i_l32(UDC_EP1InStatus,0)) & UDC_EP_STS_IN)	\
		{										\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+0], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+1], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+2], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+3], fifo_addr, 0);			\
  		i_s32(bulk_buff_ex[bulk_buff_ptr+4], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+5], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+6], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+7], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+8], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+9], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+10], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+11], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+12], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+13], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+14], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+15], fifo_addr, 0);			\
   		bulk_buff_ptr +=16;\
   		udc_set_reg(UDC_WRCNFRM, 0);			\
		i_s32( ((~UDC_EP_STS_IN) & i_l32(UDC_EP1InStatus,0)),UDC_EP1InStatus,0);		\
		udc_set_reg (UDC_EPIntr, 0x0ffff);		\
		}									\
}
#define udisk_usb_bulk_out_ex()			\
{										\
	if(bulk_buff_ptr < 128)					\
	if ((i_l32(UDC_EP2OutStatus,0)) & UDC_EP_STS_OUT)	\
		{											\
  		bulk_buff_ex[bulk_buff_ptr+0]=  i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+1] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+2] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+3] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr+4] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+5] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+6] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+7] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr+8] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+9] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+10] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+11] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ex[bulk_buff_ptr+12] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+13] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+14] = i_l32(UDC_OutFifo, 0);bulk_buff_ex[bulk_buff_ptr+15] = i_l32(UDC_OutFifo, 0);	\
  		bulk_buff_ptr +=16;\
  		udc_set_reg (UDC_EPIntr, 0xffff0000);			\
		i_s32( ((~UDC_EP_STS_OUT) & i_l32(UDC_EP2OutStatus,0)),UDC_EP2OutStatus,0);	\
		buff_des_usb_packet_map = ((buff_des_usb_packet_map <<1)|0x01);\
		}											\
}
#if 1
#define udisk_usb_bulk_in_2k()			\
{										\
	if(buff_des_usb_packet_map & (1<<(bulk_buff_ptr >>4)))\
	while(!((i_l32(UDC_EP1InStatus,0)) & UDC_EP_STS_IN));	\
		{										\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+0], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+1], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+2], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+3], fifo_addr, 0);			\
  		i_s32(bulk_buff_ex[bulk_buff_ptr+4], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+5], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+6], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+7], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+8], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+9], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+10], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+11], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+12], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+13], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+14], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+15], fifo_addr, 0);			\
   		bulk_buff_ptr +=16;\
   		udc_set_reg(UDC_WRCNFRM, 0);			\
		i_s32( ((~UDC_EP_STS_IN) & i_l32(UDC_EP1InStatus,0)),UDC_EP1InStatus,0);		\
		udc_set_reg (UDC_EPIntr, 0x0ffff);		\
		}									\
}
#else
#define udisk_usb_bulk_in_2k()			\
{										\
	if ((i_l32(UDC_EP1InStatus,0)) & UDC_EP_STS_IN)	\
		{										\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+0], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+1], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+2], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+3], fifo_addr, 0);			\
  		i_s32(bulk_buff_ex[bulk_buff_ptr+4], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+5], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+6], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+7], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+8], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+9], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+10], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+11], fifo_addr, 0);			\
   		i_s32(bulk_buff_ex[bulk_buff_ptr+12], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+13], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+14], fifo_addr, 0);i_s32(bulk_buff_ex[bulk_buff_ptr+15], fifo_addr, 0);			\
   		bulk_buff_ptr +=16;\
   		udc_set_reg(UDC_WRCNFRM, 0);			\
		i_s32( ((~UDC_EP_STS_IN) & i_l32(UDC_EP1InStatus,0)),UDC_EP1InStatus,0);		\
		udc_set_reg (UDC_EPIntr, 0x0ffff);		\
		}									\
}
#endif
#endif
#define udisk_usb_bulk_done_ex()  ( (bulk_buff_ptr <128) ? FALSE:TRUE)

#endif

