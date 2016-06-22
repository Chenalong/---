//Ukey_Only.c
#include "stdafx.h"
#include "UKey_Only.h"
#include <stdio.h>
#include <stddef.h>
#include <winioctl.h>
#include "NtScsi.h"
#include "ZTEIC.h"



void transfer_buffer_ssx20(U8 *buf,int len)
{
    int i;
    U32 temp,*ptr;
    U8 xxx;
    ptr = (U32 *)buf;
    for(i=0;i<(len>>3);i++)
    {
        temp = ptr[i];
        ptr[i] = ptr[(len>>2)-1-i];
        ptr[(len>>2)-1-i] = temp;
    }
    for(i=0;i<len;i+=4)
    {
        xxx = buf[i];
        buf[i] = buf[i+3];
        buf[i+3] = xxx;
        xxx = buf[i+1];
        buf[i+1] = buf[i+2];
        buf[i+2] = xxx;
    }
}

void hex_to_str(U8 *ptr,U8 *buf,int len)
{
        for(int i=0;i<len;i++)
        {
            //do it by using stream
            sprintf((char *)ptr, "%02x",buf[i]);
            ptr += 2;
            if(!((i+1)&0x1f))
            {
               ptr[0] = 0x0d;
               ptr[1] = 0x0a;
               ptr += 2;
            }
        }
}


BOOL VendorUSBKeyDataInOut(HANDLE hDevice,UCHAR * dataBuffer,UINT DataLen,BOOL Derection)
{

    BOOL                                status;
    ULONG                               returned = 0,length = 0;
    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;

    ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));
    sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
    sptdwb.sptd.PathId = 0;
    sptdwb.sptd.TargetId = 1;
    sptdwb.sptd.Lun = 0;
    sptdwb.sptd.CdbLength = CDB10GENERIC_LENGTH;
    sptdwb.sptd.SenseInfoLength = 24;
    if(Derection)
        sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_OUT;
    else
        sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_IN;
    sptdwb.sptd.DataTransferLength = DataLen;
    sptdwb.sptd.TimeOutValue = 2000;
    sptdwb.sptd.DataBuffer = dataBuffer;
    sptdwb.sptd.SenseInfoOffset =
       offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER,ucSenseBuf);
    sptdwb.sptd.Cdb[0] = VENDOROP_ZTEIC;
    sptdwb.sptd.Cdb[1] = VENDOROP_ZTEIC_CHECK_0;
    sptdwb.sptd.Cdb[2] = VENDOROP_ZTEIC_CHECK_1;
    sptdwb.sptd.Cdb[3] = VENDOROP_ZTEIC_CHECK_2;
    sptdwb.sptd.Cdb[4] = VENDOROP_ZTEIC_CHECK_3;
    sptdwb.sptd.Cdb[5] = VENDOROP_ZTEIC_CHECK_4;
    if(Derection)
        sptdwb.sptd.Cdb[6] = VENDOROP_ZTEIC_USB_KEY_DATA_OUT;
    else
        sptdwb.sptd.Cdb[6] = VENDOROP_ZTEIC_USB_KEY_DATA_IN;
    sptdwb.sptd.Cdb[7] = (DataLen & 0xFF);
    sptdwb.sptd.Cdb[8] = ((DataLen>>8) & 0xFF);
    sptdwb.sptd.Cdb[9] = VENDOROP_ZTEIC_USB_KEY;
    length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
    status = DeviceIoControl(hDevice,
                             IOCTL_SCSI_PASS_THROUGH_DIRECT,
                             &sptdwb,
                             length,
                             &sptdwb,
                             length,
                             &returned,
                             FALSE);
    if ((sptdwb.sptd.ScsiStatus == 0) && (status != 0)) {
       return TRUE;
       }
    return FALSE;
}




unsigned char Ukey_transfer(
   HANDLE hDevice,		// usb设备的句柄
   Ukey_KCW *kcw_p,		// 指向输入kcw包的指针
   LPVOID lpInBuffer,		// 输入buffer指针
   Ukey_KSW *ksw_p,		// 指向输出kcw包的指针
   LPVOID lpOutBuffer		// 输出buffer指针
)
{
	unsigned long nInput = 0;
	unsigned long nOutput = 0;
	VOID *  lpInputBuffer = (void *)kcw_p;
	VOID *  lpOutputBuffer = (void *)ksw_p;

    UINT *int_ptr = new unsigned int[512*128/4];
    unsigned char *ptr;
    ptr = (unsigned char *)int_ptr;
    memcpy(ptr,kcw_p,VENDOROP_ZTEIC_USB_KEY_KCW_LEN);
    if(kcw_p->dKCWOutLength != 0)
        memcpy(ptr+16,lpInBuffer,kcw_p->dKCWOutLength);
    VendorUSBKeyDataInOut(hDevice,(unsigned char *)int_ptr,kcw_p->dKCWOutLength+16,1);
 
    VendorUSBKeyDataInOut(hDevice,(unsigned char *)int_ptr,kcw_p->dKCWInLength+16,0);
    memcpy(lpOutBuffer,ptr,kcw_p->dKCWInLength);
    memcpy(ksw_p,ptr+kcw_p->dKCWInLength,VENDOROP_ZTEIC_USB_KEY_KCW_LEN);
    delete int_ptr;

	return(1);
}

unsigned char Ukey_transfer_old(
   HANDLE hDevice,		// usb设备的句柄
   Ukey_KCW *kcw_p,		// 指向输入kcw包的指针
   LPVOID lpInBuffer,	// 输入buffer指针
   Ukey_KSW *ksw_p,		// 指向输出kcw包的指针
   LPVOID lpOutBuffer	// 输出buffer指针
)
{
	unsigned long nInput = 0;
	unsigned long nOutput = 0;
	VOID *  lpInputBuffer = (void *)kcw_p;
	VOID *  lpOutputBuffer = (void *)ksw_p;

	//写usb设备
	WriteFile(hDevice, lpInputBuffer, 16, &nInput, NULL);
    
	if(kcw_p->dKCWOutLength != 0)  // 如果有数据需要发送，则发送之
		WriteFile(hDevice, lpInBuffer, kcw_p->dKCWOutLength, &nInput, NULL);
    
	if(kcw_p->dKCWInLength != 0)   // 如果有数据需要接收，则接收之
        ReadFile(hDevice, lpOutBuffer, kcw_p->dKCWInLength, &nOutput, NULL);
	
	lpOutputBuffer = (void *)ksw_p;
	
	ReadFile(hDevice, lpOutputBuffer, 16, &nInput, NULL);

	return(1);
}



