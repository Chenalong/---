#ifndef __USBDRV_H__
#define __USBDRV_H__

#include <instructions.h>
#include <bsp.h>

#define UDC_BUF_SIZE 64
#ifndef clr_imask_bit
#define clr_imask_bit(int_type)  do{i_s32((1<<int_type),IMCR,0);}while(0)
#endif

#define udc_set_reg(addr, val) (i_s32((val), (addr), 0))
#define udc_get_reg(addr, val) (val = i_l32((addr), 0))


typedef struct usb_devreq {
    unsigned char       type;
    unsigned char       request;
    unsigned char       value_lo;
    unsigned char       value_hi;
    unsigned char       index_lo;
    unsigned char       index_hi;
    unsigned char       length_lo;
    unsigned char       length_hi;
} usb_devreq __attribute__((packed));

typedef struct usb_device_descriptor {
    unsigned char       length;                 // USB_DEVICE_DESCRIPTOR_LENGTH == 18
    unsigned char       type;                   // USB_DEVREQ_DESCRIPTOR_TYPE
    unsigned char       usb_spec_lo;
    unsigned char       usb_spec_hi;
    unsigned char       device_class;
    unsigned char       device_subclass;
    unsigned char       device_protocol;
    unsigned char       max_packet_size;
    unsigned char       vendor_lo;
    unsigned char       vendor_hi;
    unsigned char       product_lo;
    unsigned char       product_hi;
    unsigned char       device_lo;
    unsigned char       device_hi;
    unsigned char       manufacturer_str;
    unsigned char       product_str;
    unsigned char       serial_number_str;
    unsigned char       number_configurations;
} usb_device_descriptor __attribute__((packed));

    
typedef struct usb_configuration_descriptor {
    unsigned char       length;
    unsigned char       type;
    unsigned char       total_length_lo;
    unsigned char       total_length_hi;
    unsigned char       number_interfaces;
    unsigned char       configuration_id;
    unsigned char       configuration_str;
    unsigned char       attributes;
    unsigned char       max_power;
} usb_configuration_descriptor __attribute__((packed));

    
typedef struct usb_interface_descriptor {
    unsigned char       length;
    unsigned char       type;
    unsigned char       interface_id;
    unsigned char       alternate_setting;
    unsigned char       number_endpoints;
    unsigned char       interface_class;
    unsigned char       interface_subclass;
    unsigned char       interface_protocol;
    unsigned char       interface_str;
} usb_interface_descriptor __attribute__((packed));        


typedef struct usb_endpoint_descriptor {
    unsigned char       length;
    unsigned char       type;
    unsigned char       endpoint;
    unsigned char       attributes;
    unsigned char       max_packet_lo;
    unsigned char       max_packet_hi;
    unsigned char       interval;
} usb_endpoint_descriptor __attribute__((packed));

struct USB_INFO{
//pierce.hu add setup_type
unsigned int setup_type;	//setup packe type
unsigned int command;   //request command
unsigned int type;      //request type
unsigned int total_len; //total length to send
unsigned int cur_len;   //now  send length 
unsigned int count;     //reserved
} usb_info ; 



#define GET_DESCRIPTOR    0x6
#define GET_CONFIGURATION 0x8
#define SET_CONFIGURATION 0x9
#define GET_INTERFACE     0x10
#define SET_INTERFACE     0x11
#define SYNCH_FRAME       0x12
#define GET_STATUS        0x0
#define CLEAR_FEATURE       0x1
#define SET_FEATURE       0x3
#define SET_ADDRESS       0x5
#define SET_DESCRIPTOR    0x7

#define Device_Type       0x1
#define Config_Type       0x2
#define String_Type       0x3
#define Interface_Type    0x4
#define Endpoint_Type     0x5

#define set_dev_config(speed,rwcap,slfpwr)		\
({								\
  int tmp = (slfpwr << 3 | rwcap << 2 | speed); \
  i_s32 (tmp|(i_l32(UDC_DevConfig,0)), UDC_DevConfig, 0);	\
})


#define set_udc_devcrs(speed,rwcap,slfpwr,devctrl,devintrmsk,epintrmsk) \
({										 \
  set_dev_config(speed, rwcap, slfpwr);				 \
  udc_set_reg (UDC_DevControl,  devctrl); 					 \
  udc_set_reg (UDC_DevIntrMask, devintrmsk);			                 \
  udc_set_reg (UDC_EPIntrMask,  epintrmsk);					 \
})

#define set_udc_infors()									\
({											\
  udc_set_reg (UDC_EP0Info,0x80 | (USB_EP0_SIZE << 19));  /* endpt 0, out, ctrl, cfg 1, 32 max*/	\
  udc_set_reg (UDC_EP1Info,0xd1 | (64 << 19)); /* endpt 1, in,  bulk, cfg 1, 64 max*/	\
  udc_set_reg (UDC_EP2Info,0xc2 | (64 << 19)); /* endpt 2, out, bulk, cfg 1, 64 max*/	\
  udc_set_reg (UDC_EP3Info,0x8f3 | (16 << 19)); /* endpt 3, in,  interrupt, cfg 1, 16 max*/	\
})

#define get_setup_command(addr) i_l8u(addr + 1, 0)
#define get_dev_type(addr)   i_l8u(addr + 3, 0)

//pierce.hu add////////////
#define HID_Type	          	0x21	// Get descriptor: HID
#define REPORT_Type         	0x22	// Get descriptor: Report
// Local defines from the mass storage class spec
#define SC_MASS_STORAGE_RESET       0xff
#define SC_GET_MAX_LUN              0xfe


#define get_setup_command(addr) i_l8u(addr + 1, 0)
#define get_dev_type(addr)   i_l8u(addr + 3, 0)

#endif
