/*Ukey_Only.h*/
/*
   //*************************************************************************
   // basic #defines
   //*************************************************************************
*/

#ifndef __UKEY_ONLY_h__
#define __UKEY_ONLY_h__

typedef char S8;
typedef short S16;
typedef long S32;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;

#define RSA_FUNC 0x71    // RSA 演示
#define ECC_FUNC 0x72    // ECC 演示
#define RNG_FUNC 0x73    // RNG 演示
#define USB_FUNC 0x74    // USB 演示
#define MD5_FUNC 0x81   //计算MD5值
#define SSF33_FUNC 0x75    // 33 演示
#define INT_FUNC 0x76    // 中断 演示
#define ECC_G2M 0x77  
#define BOOT_BACK 0x78
#define Set_ID   0x79
#define Get_ID	 0x80
#define DES_FUNC_ENCRYPT 0x81
#define DES_FUNC_DECRYPT 0x82

#define SETCLOCK_COMMAND 0x1A
#define RANDOM_EEPROM_TEST 0x1B
#define FIXED_ADDRESS_TEST 0x1C
#define READ_FIX_ADDRESS   0x1D
#define CHECK_ALL 0x1e


/***************************************************/
//           USB_KEY 命令包（KCW）结构定义
// 各字段的意义如下：
// dKCWSignature:  数字签名，KCW包的开始标志，值为0xA5，指明这是一个KCW包。   
// dKCWTag:        主机发出的COMMAND块标签。设备需要根据相应的KCW响应，填充dKWTag字段于KSW包，并将CSW包返回给主机。
//                 但在下载COS的命令中用作下载结束标志。 
// dKCWOutLength:  本次KCW希望Bulk-out端点传送的数据长度。如果长度为0，本次传输无数据传送。
// dKCWInLength:   本次COS回应的数据长度。如果长度为0，本次传输无数据传输。
// bKCWFlag :      保留字节，可用于自定义用途。
// bKCWCLA:        7816-3 中的CLA
// bKCWINS:        7816-3 中的INS
// bKCWP1:         7816-3 中的P1
// bKCWP2:         7816-3 中的P2
// dKCWReseved0:   保留字节，可用于自定义用途。   
// dKCWReseved1:   保留字节，可用于自定义用途。
// dKCWReseved2:   保留字节，可用于自定义用途。
// dKCWReseved3:   保留字节，可用于自定义用途。
// dKCWReseved4:   保留字节，可用于自定义用途。
// ads:            在下载COS的命令中用作指定下载到的地址，其他命令可以自定义或保留。
// sum:            在下载COS的命令中用作存放本数据块的和值，其他命令可以自定义或保留。
//
// 说明：
//       KCW包固定长度为16个字节，本结构兼容7816-3。同时还有较多的保留字用于扩展命令。
/***************************************************/
typedef struct _Ukey_KCW
{
	unsigned char dKCWSignature;		// 0
	unsigned char dKCWtag;                  // 1
        unsigned short dKCWOutLength;
        unsigned short dKCWInLength;
	unsigned char bKCWCLA;			// 6
	unsigned char bKCWINS;			// 7
union _flag
  {
  struct _left
  {
	unsigned char bKCWP1;			// 8
	unsigned char bKCWP2;			// 9
        unsigned char bKCWFlag;			// a
	unsigned char dKCWReseved0;		// b
  }left;
  unsigned long ads;
  }flag;
union _Rev
  {
  struct _Res
  {
	unsigned char dKCWReseved1;		// C
	unsigned char dKCWReseved2;		// D
	unsigned char dKCWReseved3;		// E
	unsigned char dKCWReseved4;		// F
  }Res;
  unsigned long sum;
  }Rev;
} Ukey_KCW;

/***************************************************/
//               USB_KEY 状态包结构定义
// 各字段的含义：
// dKSWSignature:    数字签名，CSW包的开始标志，值为0x5A，指明这是一个KBW包。   
// dKSWTag:          设备需要根据相应的KBW响应，填充。可根据命令自定义。
// dKWDataResidue:   为兼容7816-3协议设置。剩余数据长度。用来监视数据传输过程。
//                   对主机－设备的DATA-OUT,该值为dKCWDataTransferlLength与设备实际接收到的数据量之差；
//                   对设备－主机的DATA-IN, 该值为dKCWDataTransferlLength与设备实际发送的数据量之差。该值不得超过dKCWDataTransferLength。
// bKSWStatus：      该字段用来指示KCW命令执行成功或者失败。
// DKSWSW1, DKSWSW1：该字段用来指示COS命令执行成功或者失败。
// bKSWFlag：        保留字节，可用于自定义用途。        
// dKSWReseved0:     保留字节，可用于自定义用途。   
// dKSWReseved1:     保留字节，可用于自定义用途。
// dKSWReseved2:     保留字节，可用于自定义用途。
// dKSWReseved3:     保留字节，可用于自定义用途。
// dKSWReseved4:     保留字节，可用于自定义用途。
// dKSWReseved5:     保留字节，可用于自定义用途。   
// dKSWReseved6:     保留字节，可用于自定义用途。
// dKSWReseved7:     保留字节，可用于自定义用途。
// dKSWReseved8:     保留字节，可用于自定义用途。
// 说明：
//       KSW包固定长度为16个字节，本结构兼容7816-3。同时还有较多的保留字用于扩展命令。
/***************************************************/
typedef struct _Ukey_KSW
{
	unsigned char dKSWSignature;		// 0
	unsigned char dKSWtag;			// 1
	unsigned char dKSWDataResidue;	        // 2
	unsigned char bKSWStatus;		// 3
	unsigned char DKSWSW1;		        // 4
	unsigned char DKSWSW2;			// 5
	unsigned char bKSWFlag;			// 6
	unsigned char dKSWReseved0;		// 7
	unsigned char dKSWReseved1;		// 8
	unsigned char dKSWReseved2;		// 9
	unsigned char dKSWReseved3;		// A
	unsigned char dKSWReseved4;		// B
	unsigned char dKSWReseved5;		// C
	unsigned char dKSWReseved6;		// D
	unsigned char dKSWReseved7;		// E
	unsigned char dKSWReseved8;		// F
} Ukey_KSW;

unsigned char Ukey_transfer(HANDLE hDevice,Ukey_KCW *kcw_p,LPVOID lpInBuffer,Ukey_KSW *ksw_p,LPVOID lpOutBuffer);
unsigned char Ukey_transfer_old(HANDLE hDevice,  Ukey_KCW *kcw_p,  LPVOID lpInBuffer,  Ukey_KSW *ksw_p,  LPVOID lpOutBuffer);
void transfer_buffer_ssx20(U8 *buf,int len);
void hex_to_str(U8 *ptr,U8 *buf,int len);

#endif

