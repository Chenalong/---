#ifndef _UDISKDES_H_
#define _UDISKDES_H_

// DES register address
#define DCNTRL  0x9802F800
#define DDAT     0x9802F804
#define DESIV    0x9802F808
#define DKEY     0x9802F80C

#define set_des_data(x,y)		{REG32(DDAT) = x; REG32(DDAT) = y;}
#define get_des_data(x,y)		{x = REG32(DDAT); y = REG32(DDAT);}
//#define set_des_key(x,y)		{REG32(DKEY) = x; REG32(DKEY) = y;}
#define set_des_ctrl(x)	  	{REG32(DCNTRL) = x;}
#define get_des_done_sts()	(REG32(DCNTRL)&0x80)

#define des_ctrl_encrypt		0x280
#define des_ctrl_decrypt		0x2c0
#define des_ctrl_init			0x200


#ifndef _UDISKDES_C_
extern unsigned char buff_des_usb_packet_map;
extern unsigned int * buff_des;
extern unsigned short buff_des_ptr;
extern unsigned char des_get_result;
#endif


extern void des_init(void);
extern void start_des_data_init(void);
extern void start_des_data_crypt(unsigned char *_buff_des);
//extern void des_encrypt(void);
//extern void des_decrypt(void);

#if 1

#define des_encrypt()		{\
	if(TRUE == des_get_result)\
		{\
		if(!get_des_done_sts())\
			{\
			get_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr +1]);\
			buff_des_ptr += 2;\
			des_get_result = FALSE;\
			if(buff_des_ptr < 128)\
				{\
				if((1<<(buff_des_ptr >>4))&buff_des_usb_packet_map)\
					{\
					set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);\
					set_des_ctrl(des_ctrl_encrypt);\
					des_get_result = TRUE;\
					}\
				}\
			}\
		}\
	else if(buff_des_ptr < 128)\
		{\
		if((1<<(buff_des_ptr >>4))&buff_des_usb_packet_map)\
			{\
			set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);\
			set_des_ctrl(des_ctrl_encrypt);\
			des_get_result = TRUE;\
			}\
		}\
						}


#define  des_decrypt()	{\
	if(buff_des_ptr < 128)\
		{\
		if(TRUE == des_get_result)\
			{\
		if(!get_des_done_sts())\
			{\
			get_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr +1]);\
			buff_des_ptr += 2;\
			if(!(buff_des_ptr & 0xF))\
				{\
				buff_des_usb_packet_map = ((buff_des_usb_packet_map<<1)|0x01);\
				}\
			des_get_result = FALSE;\
			if(buff_des_ptr < 128)\
				{\
				set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);\
				set_des_ctrl(des_ctrl_decrypt);\
				des_get_result = TRUE;\
				}\
			}\
			}\
		else\
			{\
			set_des_data(buff_des[buff_des_ptr],buff_des[buff_des_ptr + 1]);\
			set_des_ctrl(des_ctrl_decrypt);\
			des_get_result = TRUE;\
			}\
		}\
					}


#define des_done() 		((buff_des_ptr < 128)?FALSE:TRUE)

#else
#define des_encrypt()  {}
#define  des_decrypt()  {}
#define des_done()   (TRUE)
#endif 

#endif
