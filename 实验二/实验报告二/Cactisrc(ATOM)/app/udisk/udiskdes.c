//#define  _UDISKDES_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
//#include <usbdrv.h>
#include "udisk.h"
#include "udiskdes.h"

unsigned int * buff_des;
unsigned short buff_des_ptr;
unsigned char buff_des_usb_packet_map;	//it is important ,because of it the bridge to USB bulk transfer
unsigned char des_get_result;

void set_des_key(unsigned long x,unsigned long y)
{
    REG32(DKEY) = x; 
    REG32(DKEY) = y;
}

void des_init(void)
{
       unsigned long key[2];
	//set key
	set_des_ctrl(des_ctrl_init);
	//ReadFlash(key,0x3f000+PARAMETER_SECURITY_DISK_PWD_OFFSET+PARAMETER_SECURITY_DISK_PWD_LEN, 8);
	//set_des_key(key[0],key[1]);
	
}
void start_des_data_init(void)
{
		buff_des_ptr = 128;
		des_get_result = FALSE;
}
void start_des_data_crypt(unsigned char *_buff_des)
{
	if(TRUE == lun_security_enable)
		{
		buff_des = (unsigned int *)_buff_des;
		buff_des_ptr = 0;
		buff_des_usb_packet_map = 0;
		}
	else
		{
		buff_des_ptr = 128;
		buff_des_usb_packet_map = 0xFF;
		}
	//
	des_get_result = FALSE;
}
#if 0
void des_encrypt(void)
{
	if(buff_des_ptr < 128)
		{
		if((1<<(buff_des_ptr >>4))&buff_des_usb_packet_map)
			{
			//printf("ec map %x",buff_des_usb_packet_map);
			//set data
			set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);
			set_des_ctrl(des_ctrl_encrypt);
			while(get_des_done_sts());
			//get des data
			get_des_data(buff_des[buff_des_ptr++],buff_des[buff_des_ptr ++]);
			//printf(" %x ",buff_des_ptr);
			}
		}
}
void des_decrypt(void)
{
	if(buff_des_ptr < 128)
		{
		//set data
		set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);
		set_des_ctrl(des_ctrl_decrypt);
		while(get_des_done_sts());
		//get des data
		get_des_data(buff_des[buff_des_ptr++],buff_des[buff_des_ptr ++]);

		//printf(" %x ",buff_des_ptr);
		if(!(buff_des_ptr & 0xF))
			{
			//it is the 64 BYTES packet
			buff_des_usb_packet_map = ((buff_des_usb_packet_map<<1)|0x01);
			//printf("dc map %x",buff_des_usb_packet_map);
			}
		}
}
#endif
#if 0
void des_encrypt(void)
{
	if(TRUE == des_get_result)
		{
		if(get_des_done_sts())
			{
			return;
			}
		get_des_data(buff_des[buff_des_ptr++],buff_des[buff_des_ptr ++]);
		des_get_result = FALSE;
		}

	
	if(buff_des_ptr < 128)
		{
		if((1<<(buff_des_ptr >>4))&buff_des_usb_packet_map)
			{
			set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);
			set_des_ctrl(des_ctrl_encrypt);
			des_get_result = TRUE;
			}
		}
}
void des_decrypt(void)
{
	if(TRUE == des_get_result)
		{
		if(get_des_done_sts())
			{
			return;
			}
		get_des_data(buff_des[buff_des_ptr++],buff_des[buff_des_ptr ++]);
		if(!(buff_des_ptr & 0xF))
			{
			buff_des_usb_packet_map = ((buff_des_usb_packet_map<<1)|0x01);
			}
		des_get_result = FALSE;
		}
	if(buff_des_ptr < 128)
		{
		set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);
		set_des_ctrl(des_ctrl_decrypt);
		des_get_result = TRUE;
		}
}
#endif

int en_des(unsigned int *buff_des,unsigned int j)
{
#if 1
    unsigned int i;
    for(i=0;i<j;i+=8)
        {
        set_des_data(buff_des[i],buff_des[i+4]);
        set_des_ctrl(des_ctrl_encrypt);
        while(get_des_done_sts());
        get_des_data(buff_des[i],buff_des[i+4]);
        
        }
#else
    set_des_data(buff_des[0],buff_des[1]);
    set_des_ctrl(des_ctrl_encrypt);
    while(get_des_done_sts());
    get_des_data(buff_des[0],buff_des[1]);
#endif
        
}

int de_des(unsigned int *buff_des,unsigned int j)
{
#if 1
    unsigned int i;
    for(i=0;i<j;i+=8)
        {
        set_des_data(buff_des[i],buff_des[i+4]);
        set_des_ctrl(des_ctrl_decrypt);
        while(get_des_done_sts());
        get_des_data(buff_des[i],buff_des[i+4]);
        }
#else
    set_des_data(buff_des[0],buff_des[1]);
    set_des_ctrl(des_ctrl_decrypt);
    while(get_des_done_sts());
    get_des_data(buff_des[0],buff_des[1]);
#endif
}








