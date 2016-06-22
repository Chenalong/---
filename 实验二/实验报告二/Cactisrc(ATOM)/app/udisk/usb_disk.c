#include <instructions.h>
#include <bsp.h>
//#include "usbdrv.h"
#define udc_set_reg(addr, val) (i_s32((val), (addr), 0))
#define udc_get_reg(addr, val) (val = i_l32((addr), 0))
//=============================================

void usb_send_ep1_64_32_ex(unsigned char *addr)
{
  while(!((i_l32(UDC_EP1InStatus,0)) & UDC_EP_STS_IN));
   unsigned fifo_addr = UDC_InFifo + i_l32(UDC_EP0InBufSize, 0) * 4;
   unsigned int * pbuff = (unsigned int *)addr;
   unsigned int  count = 0; 
   i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);
   i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);
   i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);
   i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);i_s32(pbuff[count++], fifo_addr, 0);
   udc_set_reg(UDC_WRCNFRM, 0);
   clr_ep_sts(UDC_DIR_IN, 1, UDC_EP_STS_IN);
   udc_set_reg (UDC_EPIntr, 0x0ffff);
}
void usb_receive_ep2_64_32_ex(char *buf)
{
  while (!((i_l32(UDC_EP2OutStatus,0)) & UDC_EP_STS_OUT));
  unsigned int *p = (unsigned int *) buf;
  *p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);
  *p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);
  *p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);
  *p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);*p++ = i_l32(UDC_OutFifo, 0);
  udc_set_reg (UDC_EPIntr, 0xffff0000);
  clr_ep_sts(UDC_DIR_OUT, 2, UDC_EP_STS_OUT);
}
//=============================================
//hmx add for nak-stall-nak
void usb_bulk_out_stall_token()
{
    unsigned int pkt_sz=0;
    //wait untill out data
    while(!((get_ep_sts (UDC_DIR_OUT, 2) & UDC_EP_STS_OUT)));
    //stall it
    i_s32(0x01 |(i_l32(UDC_EP2OutControl ,0)),UDC_EP2OutControl,0);  
    //clear fifo then clear feature can been recieve
    //read out ep2 data and wait ep0 out data
    dis_interrupt();
    //while( (!(get_ep_sts (UDC_DIR_OUT, 0) & UDC_EP_STS_OUT)) ||((get_ep_sts (UDC_DIR_OUT, 2) & UDC_EP_STS_OUT)))
    while(((get_ep_sts (UDC_DIR_OUT, 2) & UDC_EP_STS_OUT)))
    	{
    	if((get_ep_sts (UDC_DIR_OUT, 2) & UDC_EP_STS_OUT))
    		{
    		unsigned int buf[64/4];
    		pkt_sz = get_pkt_sz(2);
		usb_fifo_in((unsigned char *)buf,pkt_sz,2);
		udc_set_reg (UDC_EPIntr, 0xffff0000);
		clr_ep_sts(UDC_DIR_OUT, 2, UDC_EP_STS_OUT);
    		}
    	}
    if(0 == pkt_sz)
    	{
    	pkt_sz = 0x80;
    	}
    else
    	{
    	pkt_sz = 0x200;
    	}
    while(pkt_sz--);
    enb_interrupt();
    //set nak
   i_s32(0xFE &(i_l32(UDC_EP2OutControl ,0)),UDC_EP2OutControl,0);  
}
void usb_bulk_in_stall_token()
{
#if 1
    unsigned status;
    unsigned long count;
    //wait it fifo empty
     do
    {
     status = get_ep_sts (UDC_DIR_IN, 1);
    }
   while (!(status & UDC_EP_STS_IN));
    //stall it
    clr_ep_sts(UDC_DIR_IN, 1, UDC_EP_STS_IN);
    udc_get_reg(UDC_EP1InControl , status );
    udc_set_reg(UDC_EP1InControl , status | 0x01);
    //wait stall send to host
    count = 0;
    do
    {
     status = get_ep_sts (UDC_DIR_IN, 1);
	 //if((++count)>0xfff) //48M ok nec2.0 hub ok one stall
	 //if((++count)>0x3000)//48M ok maybe error
	 if((++count)>0xff) //48M ok nec2.0 ok one stall ,maybe it is the best
	 	break;
    }
   while (!(status & UDC_EP_STS_IN));
   //set nak
   udc_get_reg(UDC_EP1InControl , status );
   udc_set_reg(UDC_EP1InControl , status & 0xFE);
#else
//send zero packet for ep1 bulk in
	//wait in packet
	//while (!(UDC_EP_STS_IN&get_ep_sts (UDC_DIR_IN, 1)));
	_usb_send(0,0);
#endif
   
}
//
int usb_receive(char *buf, int len)
{
 unsigned int out_pack,i,len1;
 out_pack = 0;
 while(len>out_pack)
 {
    len1=_usb_receive(buf+out_pack); 
    out_pack+=len1;
 }
 return out_pack;
}

int usb_send(char *buf,unsigned short len)
{
    unsigned short mylen,i;
	short length=len,sendlen=0;
	mylen=0;
	if(length>0)
	{
	  while(length>0)
           {     
	         if(length>64) 
                	sendlen=64;
                 else
                    sendlen=length;
		_usb_send(buf+mylen,sendlen);
			length-=sendlen;	
		    mylen+=sendlen;
           }		 	
    	}
	return mylen;
}

//pierce.hu add this 
int usb_probe_ep3_in_send(char *buf,unsigned short len)
{
	if(len>32)
		{
		return 0;
		}
	if(!(get_ep_sts (UDC_DIR_IN, 3) & UDC_EP_STS_IN))
		{//no host's in packet
		return 0;
		} 
	
   usb_fifo_out(buf,len,3);
   udc_set_reg(UDC_WRCNFRM, 0);
   clr_ep_sts(UDC_DIR_IN, 3, UDC_EP_STS_IN);
   udc_set_reg (UDC_EPIntr, 0x0ffff);
   return 1;
}
int usb_probe_ep2_out_recieve(char *buf)
{
  unsigned pkt_sz=0,status=0,i=0,count=0;
  char tempbuf[65];
  if(!(get_ep_sts (UDC_DIR_OUT, 2) & UDC_EP_STS_OUT))
  	{
  	       // set clk to 24Mhz
  	       //if(0x55!=(REG32(0x1803F100)&0xff))
		    //REG32(0x1803F100)=0x31e755;
		return 0;
  	}
  //start_timer();
  // set clk to 96Mhz
  //if(0x11!=(REG32(0x1803F100)&0xff))
     //REG32(0x1803F100)=0x31e711;
  pkt_sz = get_pkt_sz(2);
  usb_fifo_in((unsigned char *)buf,pkt_sz,2);
  udc_set_reg (UDC_EPIntr, 0xffff0000);
  clr_ep_sts(UDC_DIR_OUT, 2, UDC_EP_STS_OUT);
  return 1; 
}
																				
int g_usb_receive(char *buf, int len)
{
 unsigned int out_pack,i,len1;
 out_pack = 0;
 while(len>out_pack)
 {
    len1=_usb_receive(buf+out_pack); 
    out_pack+=len1;
 }
 return out_pack;
}	
