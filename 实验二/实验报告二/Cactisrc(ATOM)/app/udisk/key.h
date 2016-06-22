

#ifndef _KEY_H_
#define _KEY_H_

#define KEY_IN_BYTE_NUM		0x10
#define KEY_IN_EP				3

#ifndef _KEY_C_
extern unsigned char key_enable;
#endif

extern void key_init(void);
extern void key_poll(void);

#endif

