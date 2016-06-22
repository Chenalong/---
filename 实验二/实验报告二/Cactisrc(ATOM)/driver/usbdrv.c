/*
 * USB Device Driver module 
 */
/*
 * CHANGELOG:
   * 02-sep-2003	created by Stanley.zhu<zhu.shan@zte.com.cn> 
14-06-2005: pierce.hu modify,
05-07-2005 add bulk int isr
*-------------------------------------------------------------------------*/

#include "usbdrv.h"
#include <ukey_proto.h>
#include<config.h>

//#define DEBUG 1
#undef DEBUG
#ifdef DEBUG
#define DPRINTS(x) print_string(x)
#define DPRINTL(x) print_long(x)
#else
#define DPRINTS(x)
#define DPRINTL(x)
#endif

#define USB_DEV_LEN 0x12
#define USB_CFG_LEN (4*9+3*7)

usb_devreq  g_req;
/*UDC_BUF_SIZE define in usbdrv.h */
volatile unsigned char udc_data_buf[UDC_BUF_SIZE];


//pierce.hu change
#ifdef ZI1222_PIERCEHU
#define DSCR_DEVICE_LEN     18
#define DSCR_CONFIG_LEN     9
#define DSCR_INTRFC_LEN     9
#define DSCR_ENDPNT_LEN     7
#define DSCR_HID_LEN        9

#define DSCR_DEVICE         1   // Descriptor type: Device
#define DSCR_CONFIG         2   // Descriptor type: Configuration
#define DSCR_STRING         3   // Descriptor type: String
#define DSCR_INTRFC         4   // Descriptor type: Interface
#define DSCR_ENDPNT         5   // Descriptor type: Endpoint

#define ET_CONTROL          0   // Endpoint type: Control
#define ET_ISO              1   // Endpoint type: Isochronous
#define ET_BULK             2   // Endpoint type: Bulk
#define ET_INT              3   // Endpoint type: Interrupt

#define SETUP_MASK				0x60	//Used to mask off request type
#define SETUP_STANDARD_REQUEST	0		//Standard Request
#define SETUP_CLASS_REQUEST		0x20	//Class Request
#define SETUP_VENDOR_REQUEST	0x40	//Vendor Request
#define SETUP_RESERVED_REQUEST 	0x60	//Reserved or illegal request

#ifdef ZI1222_MI_ENABLE
const unsigned char zteic_hid_report [] = 
       {
	//HIDReportDscr:
	0x05, 0x01,			// Usage page (generic desktop)
       0x06, 0xA0, 0xFF,         //Usage Page (vendor defined)
       0x09, 0xA5,                  //Usage (Vendor Defined)
       	0xA1, 0x01,           		//collection (application)
       	0x09, 0xA6,           		//usage (vendor defined)
       		0x09, 0xA7,              		//usage (vendor defined)
       		0x15, 0x80,                      	// logical min (-127)        
       		0x25, 0x7F,                      	//logical max (128)
       		0x75, 0x08,                      	//report size (8 bits)
       		0x95, 0x10,                     	//report count (16 field)
       		0x81, 0x02,                     	//input (data,variable,absolute)
       	0xC0                         		//end collection
	//HIDReportDscrEnd:
	};

#define HID_REPORT_LEN	24
#define HID_DESC_OFFSET	(DSCR_CONFIG_LEN+DSCR_INTRFC_LEN*2+DSCR_ENDPNT_LEN*2)
#endif
extern unsigned short PID;
extern unsigned short VID;
const usb_device_descriptor zteic_dev_desc=
{DSCR_DEVICE_LEN,DSCR_DEVICE,0x10,0x01,0x00,0x00,0x00,USB_EP0_SIZE,0x5F,0x73,0x95,0x81,0,0x01,4,3,2,1};
const unsigned char zteic_cfg_desc [] = 
       {
       //configuration desc                    
       DSCR_CONFIG_LEN, DSCR_CONFIG,
#ifdef ZI1222_MI_ENABLE
       DSCR_CONFIG_LEN + DSCR_INTRFC_LEN + DSCR_ENDPNT_LEN + DSCR_ENDPNT_LEN +
       DSCR_INTRFC_LEN + DSCR_HID_LEN + DSCR_ENDPNT_LEN,
       0x00,
       0x02,
#else       
       DSCR_CONFIG_LEN + DSCR_INTRFC_LEN + DSCR_ENDPNT_LEN +DSCR_ENDPNT_LEN,
       0x00,
       0x01,
#endif
       0x01, 0x00, 0xc0, 0xf0,
       //interface desc for mass storage bulk only
       DSCR_INTRFC_LEN, DSCR_INTRFC, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,
       //ep1 desc
       DSCR_ENDPNT_LEN, DSCR_ENDPNT, 0x81, ET_BULK, 0x40, 0x00, 0x00,
       //ep2 desc
       DSCR_ENDPNT_LEN, DSCR_ENDPNT, 0x02, ET_BULK, 0x40, 0x00, 0x00,
       
 #ifdef ZI1222_MI_ENABLE
       //interface desc for hid
       DSCR_INTRFC_LEN, DSCR_INTRFC, 0x01, 0x00, 0x01, 0x03, 0x01, 0x02, 0x00,
       //HID desc
       DSCR_HID_LEN, 0x21, 0x10, 0x01, 0x00, 0x01, 0x22, HID_REPORT_LEN, 0x00,
       //ep3 desc
       DSCR_ENDPNT_LEN, DSCR_ENDPNT, 0x83, ET_INT, 0x10, 0x00, 0x01
 #endif
       }; 
//=====================================================
#endif
/*get the num of endpoint
 INPUT:
       * in :  
             ( 16 bits of) endpoint interrupt register value : 
	       high 16 bits --> out_ep_num
	       low  16 bits --> in_ep_num
 OUTPUT:
       none;
 RETURN:
        in or out endpoint num;     

*/
void usb_fifo_out(unsigned char * buffer,unsigned pkt_sz, unsigned epnum);
void usb_fifo_in( unsigned char *buffer, unsigned pkt_sz,unsigned ep_num);
unsigned get_ep_num(unsigned in)
{
  if (in == 1) return 0;
  else if (in == 2) return 1;
  else if (in == 4) return 2;
  else if (in == 8) return 3;
  else return UDC_MAX_ENDPT_NUM + 1;
}

/*get status register addr from endpoint number
 INPUT:
      * dir : direction (in or out);
      * ep_num: endpoint num;
 OUTPUT:
      none;
 RETURN:
      * address: status register addr; 
*/

  unsigned get_ep_sts_addr(unsigned dir, unsigned ep_num)
{
  unsigned int address;
  if (dir == UDC_DIR_OUT)
    address = UDC_BASE_ADDR + ep_num * 0x20 + 0x204;
  else
    address = UDC_BASE_ADDR + ep_num * 0x20 + 0x04;
  return address;
}

/*get status of endpoint 
 INPUT:
      * direction : direction (in or out);
      * ep_num: endpoint num;
 OUTPUT:
      none;
 RETURN:
      * status: endpoint status; 
*/

  unsigned get_ep_sts(unsigned direction, unsigned ep_num)
{
  unsigned int address, status;

  address = get_ep_sts_addr(direction, ep_num);
  udc_get_reg(address, status);
  return status;
}

/*clear endpoint status register 
 INPUT:
      * direction : direction (in or out);
      * ep_num:     endpoint num;
      * mask  :     mask bits
 OUTPUT:
      none;
 RETURN:
      none; 
*/

  void clr_ep_sts(unsigned direction, unsigned ep_num, unsigned mask)
{
  unsigned int address, status;
  address = get_ep_sts_addr (direction, ep_num);
  udc_get_reg (address, status);
  udc_set_reg (address, (status&(~mask)));
  return;
}
/* get length of packet 
 INPUT:
      * ep_num: endpoint num;
 OUTPUT:
      none;
 RETURN:
      * pktsz: packet length; 
*/
 unsigned get_pkt_sz(unsigned ep_num)
{
  unsigned status;
  status = get_ep_sts(UDC_DIR_OUT, ep_num);
  return ((status & 0x03ff800) >> 11);
}

/* when host setup the usb device, interrupt calls this function
 INPUT:
      * ep_num: endpoint num;
 OUTPUT:
      none;
 RETURN:
      none; 
*/

//pierce.hu add for string descriptor
const unsigned char manufacturerString[12] = {12,String_Type,'Z',0,'T',0,'E',0,'I',0,'C',0};
const unsigned char productString[26] = {26,String_Type,'U',0,'S',0,'B',0,' ',0,'D',0,'I',0,'S',0,'K',0,'-',0,'K',0,'E',0,'Y',0};
const unsigned char srialnumString[22+20] = {22+20,String_Type,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0
	,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0};
const unsigned char languageString[4]={4,String_Type,0x09,0x04};
unsigned char * stringdesc = (unsigned char *)languageString;
//
void usb_setup_handler(unsigned ep_num)
 {
  unsigned dev_type;
  unsigned tmp, len,i;
  unsigned command_type=0;
  /*get request length*/
  if(g_req.request==0xb5)
    { 
     enable_download_flag();
     return;
    }
    
  len = g_req.length_hi;
  len<<=8 ;
  len+= g_req.length_lo;
  
  usb_info.total_len = len;
  usb_info.cur_len = 0;
  /* get request */
  usb_info.command = g_req.request;
  /* get request type*/
  usb_info.type = g_req.value_hi;
  usb_info.count = 0;
  
  //pierce.hu add setup paket type dealing
  usb_info.setup_type = g_req.type;
  
  
  command_type = g_req.request;
  
  switch (command_type)
    {
     case GET_DESCRIPTOR:
       dev_type = g_req.value_hi;
       if(dev_type==0)
         {dev_type=2;
          usb_info.type = dev_type;
	 }
       switch (dev_type)
	{
	 case Device_Type:
	   if(usb_info.total_len > USB_DEV_LEN)
	        usb_info.total_len=USB_DEV_LEN;	 
	  break;
	 case Config_Type:
	   if(usb_info.total_len > USB_CFG_LEN)
	        usb_info.total_len=USB_CFG_LEN;
	  break;
#ifdef ZI1222_MI_ENABLE	  
	  //pierce.hu add this
	  case HID_Type:
	  	if(usb_info.total_len > DSCR_HID_LEN)
	        usb_info.total_len=DSCR_HID_LEN;
		break;
	 case REPORT_Type:
	 	if(usb_info.total_len > HID_REPORT_LEN)
	        usb_info.total_len=HID_REPORT_LEN;
		break;
#endif		
	  //
	 case String_Type:
	 	switch(g_req.value_lo)
	 		{
	 		case 4:
	 			{
	 			stringdesc = (unsigned char *)manufacturerString;
				break;
	 			}
	 		case 3:
	 			{
	 			stringdesc = (unsigned char *)productString;
				break;
	 			}
			case 2:
				{
				stringdesc = (unsigned char *)srialnumString;
				break;
				}
	 		default: 
				{//language
				stringdesc = (unsigned char *)languageString;
				//break;
				}
	 		}
		if(usb_info.total_len > stringdesc[0])
	        	usb_info.total_len=stringdesc[0];
	   	break;
	 case Interface_Type:
	   break;
	 case Endpoint_Type:
	   break;
	}
      break;
   //pierce.hu mask
   /*
   case GET_MAX_LUN:
	        usb_info.total_len=1;
	  break;      */
    case GET_CONFIGURATION:
      break;
    case GET_INTERFACE:
      break;
    case GET_STATUS:
      break;
    default:
      {
      	break;
      }
    }
 }






/*main handler of usb interrupt
 INPUT:
      none; 
 OUTPUT:
      none;
 RETURN:
      none; 
*/
void usb_int_handler()
 {
  unsigned int device_intr_data;
  unsigned int ep_intr_data;
  unsigned int out_ep_num, in_ep_num;
  unsigned int status ,dev_ctrl;
  unsigned int pkt_sz ;
  int left_len=0;
  unsigned char temp;
  status = 0; 
  
  udc_get_reg(UDC_DevIntr, device_intr_data);
  udc_get_reg(UDC_EPIntr,  ep_intr_data);
  DPRINTS("enter usb int ");
  DPRINTL(device_intr_data);
  DPRINTL(ep_intr_data);
  /*device interrupt */
  if (device_intr_data)
    {
      /*clear the register */
      udc_set_reg(UDC_DevIntr, device_intr_data);
    }
  /*end point interrupt*/     
  if (ep_intr_data)
    {
      /*get the out endpoit num*/
      out_ep_num = get_ep_num (ep_intr_data >> 16);
      /*get the in endpoit num*/      
      in_ep_num = get_ep_num (ep_intr_data & 0x0000ffff);
      DPRINTS("enter ep interrupt");
      DPRINTL(out_ep_num);
      DPRINTL(in_ep_num);
     
    if (out_ep_num <= UDC_MAX_ENDPT_NUM)
      {
      //------------------
      //humingxin add this for bulk int
     /* if(out_ep_num == 2)
      	{
      	printf("udisk_usb_bulk_out\n");
      	udisk_usb_bulk_out();
      	return;
      	}*/
      //-------------------
       // if(out_ep_num>0)
	//     return;
        status = get_ep_sts(UDC_DIR_OUT, out_ep_num);
	//clear ep interrupt register
	udc_set_reg (UDC_EPIntr, ep_intr_data & 0xffff0000);
        
	DPRINTS("enter ep out");
     }
    else if (in_ep_num <= UDC_MAX_ENDPT_NUM)
    {
      DPRINTS("enter ep in");
      //------------------
      //humingxin add this for bulk int
      /*if(in_ep_num == 1)
      	{
      	//printf("udisk_usb_bulk_in\n");
      	udisk_usb_bulk_in();
      	return;
      	}*/
      //-------------------
      status = get_ep_sts(UDC_DIR_IN, in_ep_num);
     }  
    if (status & UDC_EP_STS_IN) //in
       {
         //if ep0 in 
	 if(in_ep_num == 0 )
	 {
	   //pierce.hu add
	   if ((usb_info.setup_type & SETUP_MASK) == SETUP_STANDARD_REQUEST)
	   {
	   //
          switch(usb_info.command)
	    {
             case GET_DESCRIPTOR:  
      	                  left_len = usb_info.total_len - usb_info.cur_len;
			 //pierce.hu add
			 if(left_len<=0)	break;
			 //
                    	  switch (usb_info.type)
  	                       {
        	 		case Device_Type:
					{
						unsigned char ddscr[18];
						memcpy(ddscr,(unsigned char *)(&zteic_dev_desc),18);
						ddscr[8] = (unsigned char)(VID & 0xFF);
						ddscr[9] = (unsigned char)((VID >> 8) & 0xFF);
						ddscr[10] = (unsigned char)(PID & 0xFF);
						ddscr[11] = (unsigned char)((PID >> 8) & 0xFF);
                                              	  /*send out device descriptor*/
                                          if (left_len > USB_EP0_SIZE) // USB_EP0_SIZE == max packet size
		                        	    usb_fifo_out((unsigned char *)(ddscr) + usb_info.cur_len,USB_EP0_SIZE,0); 
                        			else
                           			    usb_fifo_out((unsigned char *)(ddscr) + usb_info.cur_len, left_len, 0);
                         			 usb_info.cur_len += USB_EP0_SIZE; 
	 		                      break;
        	 			}
              	 		case Config_Type:
	                         		  /*send out configuration descriptors*/
                        	  		 if (left_len > USB_EP0_SIZE) // USB_EP0_SIZE == max packet size
			                              usb_fifo_out((unsigned char *)(&zteic_cfg_desc) + usb_info.cur_len,USB_EP0_SIZE,0); 
                         			 else
                            			      usb_fifo_out((unsigned char *)(&zteic_cfg_desc) + usb_info.cur_len, left_len, 0);
                            			 usb_info.cur_len += USB_EP0_SIZE; 
                        			 break;
#ifdef ZI1222_MI_ENABLE									 
					//pierce.hu add this \
					case HID_Type:
						if (left_len > USB_EP0_SIZE) // USB_EP0_SIZE == max packet size
			                              usb_fifo_out((unsigned char *)(&zteic_cfg_desc[HID_DESC_OFFSET]) + usb_info.cur_len,USB_EP0_SIZE,0); 
                         			 else
                            			      usb_fifo_out((unsigned char *)(&zteic_cfg_desc[HID_DESC_OFFSET]) + usb_info.cur_len, left_len, 0);
                            			 usb_info.cur_len += USB_EP0_SIZE; 
						break;
					case REPORT_Type:
						if (left_len > USB_EP0_SIZE) // USB_EP0_SIZE == max packet size
			                              usb_fifo_out((unsigned char *)(&zteic_hid_report) + usb_info.cur_len,USB_EP0_SIZE,0); 
                         			 else
                            			      usb_fifo_out((unsigned char *)(&zteic_hid_report) + usb_info.cur_len, left_len, 0);
                            			 usb_info.cur_len += USB_EP0_SIZE; 
						break;
#endif						
					//
				//else do nothing 		 
              	 		case String_Type:
						if (left_len > USB_EP0_SIZE) // USB_EP0_SIZE == max packet size
			                              usb_fifo_out((unsigned char *)(stringdesc) + usb_info.cur_len,USB_EP0_SIZE,0); 
                         			 else
                            			  usb_fifo_out((unsigned char *)(stringdesc) + usb_info.cur_len, left_len, 0);
                            			 usb_info.cur_len += USB_EP0_SIZE; 
	                         		 break;
         	 		case Interface_Type:
	   		                         break;
                     		case Endpoint_Type:
	  		                         break;
	 	               }
		               break;
	     //else do nothing 		       
             default: 
             		printf("setup command\n");
	             break;		 
  	   }  
	   //pierce.hu add
	   }
	   else if ((usb_info.setup_type & SETUP_MASK) == SETUP_CLASS_REQUEST)
	   	{
	   	//
	   	extern unsigned char lun_num;
		unsigned char lun_num_ex = lun_num - 1;
		//lun_num_ex = 0;
	   	//
	   		switch(usb_info.command)
			         {
				         case SC_MASS_STORAGE_RESET:
						//printf("^^^^^bulk reset^^^^^\n");
				               break;			      
				         case SC_GET_MAX_LUN:
	 		                     //printf("SC_GET_MAX_LUN %x\n",lun_num);
						usb_fifo_out(&lun_num_ex,1,0);
				              break;
				         default:                            // *** Invalid Command
				            break;
			         } // End Switch
	   	}
	   //end pierce.hu add
	   /*clear status register*/ 
	   clr_ep_sts(UDC_DIR_IN, in_ep_num, UDC_EP_STS_IN);
	   //clear interrupt register
 	   udc_set_reg (UDC_EPIntr, ep_intr_data & 0x0ffff);
	  }
      }
      if (status & UDC_EP_STS_OUT)
	  {
	   //if host error
	   if (status & UDC_EP_STS_HE)
	    {
	      udc_get_reg(UDC_DevControl, dev_ctrl);
              /*software disconnect */
	      udc_set_reg (UDC_DevControl, dev_ctrl & 0x400);
	    }
	  // Recieve setup request
	  if ((status & UDC_EP_STS_OUT) == UDC_EP_OUT_SETUP_DAT)
	    {
	      //clear ep out status 
	      clr_ep_sts(UDC_DIR_OUT, out_ep_num, UDC_EP_STS_OUT);
	      //read out packet to g_req
	      usb_fifo_in((unsigned char *)(&g_req),8,out_ep_num);
              //start setup handler
	      usb_setup_handler(out_ep_num);
	    }
	  else // Receive normal data
	    {
	      //clear ep out status 
	      clr_ep_sts(UDC_DIR_OUT, out_ep_num, UDC_EP_STS_OUT);
	      //get the out packet size
	      pkt_sz = get_pkt_sz(out_ep_num);
	      //read out packet to udc_data_buf
              usb_fifo_in((unsigned char *)udc_data_buf, pkt_sz,out_ep_num);
     	   }
       }
   }    
 }


void init_usb_info_struct()
{
  usb_info.command = 0;
  usb_info.type = 0;
  usb_info.total_len = 0;
  usb_info.cur_len = 0;
  usb_info.count = 0;
}
/* usb initialize function ,if you will use usb module, you must call this function 
   in your main function
   INPUT:
      none; 
   OUTPUT:
      none;
   RETURN:
      none; 
*/

void usb_initialize()
{
//pierce.hu add this for bulk int
//udisk_bulk_init();
//
  //disable interrupt
  dis_interrupt();
  //init device control registers
  set_udc_devcrs(UDC_FULL_SPEED, UDC_REMOTE_WAKEUP_ENB, UDC_BUS_POWERED,
                 0, 0x7f, 0xfffefffe);
  //-------------
  //humingxin add
  //udisk_bulk_init();
  //-------------
   //init ep control registers		 
  //set_udc_epcrs();
  //init epinfo registers
  set_udc_infors();
  //clear ep1 ep2 status
  clr_ep_sts(UDC_DIR_OUT, 2, UDC_EP_STS_OUT);
  clr_ep_sts(UDC_DIR_IN, 1, UDC_EP_STS_IN);
  //clear Endpoints int flag
  udc_set_reg (UDC_EPIntr, 0xffffffff);
  //install a irq handler for usb interrupt

/*********************************************/
// 清除中断标志，防止芯片启动时usb中断已经置位  
udc_set_reg(UDC_DevIntr, 0xffffffff);
/*********************************************/
  
  setvbr(TP_INT | INTC_UDC, usb_int_handler);
  // clear int mask bit 
  clr_imask_bit(INTC_UDC);
  init_usb_info_struct();
  //enable interrupt
  enb_interrupt();
  //init_gpio_led();
  
}
/* usb fifo write
   fifo register only supports word-wide;
   INPUT:
         * buffer : buffer to be send ;
	 * pkt_sz : packet size;
	 * epnum  : endpoint num; 
   OUTPUT:
         none;
   RETURN:
         none; 
*/
void usb_fifo_out(unsigned char * buffer,unsigned pkt_sz, unsigned epnum)
{
  unsigned int counter, fifo_addr = UDC_InFifo;
  unsigned int quad_pkts, nquad_pkts,*ibuf;
  unsigned int i=0;
  /*if epnum==1 , get ep1 fifo address*/
  if (epnum == 1) 
  	fifo_addr += i_l32(UDC_EP0InBufSize, 0) * 4;
  //pierce.hu add
  else if(epnum == 3)
  	fifo_addr = fifo_addr + i_l32(UDC_EP0InBufSize, 0) * 4+ i_l32(UDC_EP1InBufSize, 0) * 4;
  //
  /*if will send a null packet */     
  if (pkt_sz == 0 )
    {
      /*write zerolen register*/
      udc_set_reg(UDC_WRZeroLen, 0);
      /*write any data to fifo*/
      udc_set_reg(fifo_addr, 0);
     /*confirm send */
      udc_set_reg(UDC_WRCNFRM,0);
      return;
    }
  /*===================start put the data into fifo ===============*/    
  ibuf=(unsigned int *) buffer;
  quad_pkts = (pkt_sz >> 2);
  nquad_pkts = (pkt_sz & 3);
  for (counter = 0; counter < quad_pkts; counter++)
    {
     i_s32(ibuf[counter], fifo_addr, 0);
     i+=4;
    }
  for (counter = 0; counter < nquad_pkts; counter++)
    {
      i_s8(buffer[i],fifo_addr, 0);
      i++;
    }
  /*===================end put the data into fifo ===============*/    
   /*confirm send */
  udc_set_reg(UDC_WRCNFRM, 0);
}

/* usb fifo read
   fifo register only supports word-wide;
   INPUT:
	 * pkt_sz : packet size;
	 * epnum  : endpoint num; 
   OUTPUT:
         * buffer : output buffer;
   RETURN:
         none; 
 */
   
  void usb_fifo_in( unsigned char *buffer, unsigned pkt_sz,unsigned ep_num)
{
  unsigned int *p = (unsigned int *) buffer;
  unsigned int quad_pkts, nquad_pkts, counter;
  /*if will receive a null packet */ 
  if (pkt_sz == 0)
    {
      /*write a data to confirm register */
      i_l32(UDC_WRCNFRM, 0);
      return;
    }
  
  /*================start read data from fifo===============*/
  quad_pkts = (pkt_sz >>2);
  nquad_pkts = (pkt_sz & 3);

  for (counter = 0; counter < quad_pkts; counter++, p++)
    *p = i_l32(UDC_OutFifo, 0);

  if (nquad_pkts)
    *p = i_l32(UDC_OutFifo, 0);
  /*================end read data from fifo===============*/
}
/* receive a packet from usb  : query mode 
   INPUT:
         none;
   OUTPUT:
         * buf : output buffer;
   RETURN:
         none; 
*/
int _usb_receive(char *buf)
{
  unsigned pkt_sz=0,status=0,i=0,count=0;
  char tempbuf[65];
  do
   {
     status = get_ep_sts (UDC_DIR_OUT, 2);
   }
  while (!(status & UDC_EP_STS_OUT));
      pkt_sz = get_pkt_sz(2);
  usb_fifo_in((unsigned char *)buf,pkt_sz,2);
  udc_set_reg (UDC_EPIntr, 0xffff0000);
  clr_ep_sts(UDC_DIR_OUT, 2, UDC_EP_STS_OUT);
  return pkt_sz; 
}


/* send out a packet  : query mode 
   INPUT:
         * buf : input buffer;
   OUTPUT:
         none;
   RETURN:
         none; 
*/
int _usb_send(char *buf,unsigned len)
{
    unsigned pkt_sz=0,status=0;
    unsigned long count=0;
    do
    {
     status = get_ep_sts (UDC_DIR_IN, 1);
    }
   while (!(status & UDC_EP_STS_IN));
   if(len>64)
     len=64; 
   usb_fifo_out(buf,len,1);
   clr_ep_sts(UDC_DIR_IN, 1, UDC_EP_STS_IN);
   udc_set_reg (UDC_EPIntr, 0x0ffff);
   return len;
}
																   
