
#define _KEY_C_

#include <instructions.h>
#include <bsp.h>
#include <types.h>
#include "key.h"
#include <usb.h>

#ifdef ZI1222_MI_ENABLE
unsigned char key_enable = TRUE;
#else
unsigned char key_enable = FALSE;
#endif

#ifdef ZI1222_MI_ENABLE
static unsigned int key_timer = 0;
static struct _KEY_IN_DATA{
	unsigned long long	Data0;
	unsigned long long  Data1;
}key_in_data;
#endif

void key_init(void)
{
#ifdef ZI1222_MI_ENABLE
	key_in_data.Data0 = 0;
	key_in_data.Data1 = 0xffffffff;
#endif	

}
void key_poll(void)
{
#ifdef ZI1222_MI_ENABLE
	//static unsigned int int_packet_num=0;
    if(key_enable)
    {
		if (key_timer >= 0xfffff)
			{
			if(usb_probe_ep3_in_send((char *)(&key_in_data),KEY_IN_BYTE_NUM))
				{
				//printf("hid in ok\n");
				key_in_data.Data0 ++;
				key_in_data.Data1 ++;
				//if((++int_packet_num)==4)
					{
					key_timer = 0;
					//int_packet_num = 0;
					}
				}
			}
		else
			{
			key_timer ++;
			}		
    }
#endif	
}
