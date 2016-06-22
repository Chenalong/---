#ifndef __USB_H__
#define __USB_H__
#include <ukey_proto.h>
#include <config.h>
void usb_initialize();
int usb_receive(char *buf);
int usb_send(char *buf,unsigned len);
int usb_receivepack(Ukey_KCW *kcw_p,char *buf);
int usb_sendpack(Ukey_KSW *ksw_p,char *buf,unsigned short len);
#endif
