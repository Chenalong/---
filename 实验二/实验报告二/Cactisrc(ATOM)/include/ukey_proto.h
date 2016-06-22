#ifndef _UKEY_PROTO_H_
#define _UKEY_PROTO_H_
#if 0
/****** command define table start ******/
  // UART command start with 0xff..
#define UART_SET_CMD 0xff00
#define UART_START_CMD 0xff01
#define UART_END_CMD 0xff02
#define UART_LOOP_CMD 0xff03

  // UART command start with 0xfe..
#define UART_TEST_CMD 0xfe00

#define RNG_GET_RANDOM 0xfe01

  // RSA command start with 0xfd..
#define RSA_TEST_CMD 0xfd00
#define RSA_MODMUL_CMD 0xfd01
#define RSA_MUL_CMD 0xfd02
#define RSA_MODE_CMD 0xfd03
#define RSA_GEN_KEY_CMD 0xfd04

#define RSA_ECC_ABmodF_CMD 0xfd05
#define RSA_ECC_ABmul_CMD 0xfd06
#define RSA_ECC_CmodF_CMD 0xfd07

#define RSA_CLEAR_MEM 0xfd08

  // DES command start with 0xfc..
#define DES_TEST_CMD 0xfc00

 // CPU sleep command
#define CPU_SLEEP 0x1111
#endif
/****** command define table end ******/

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
        //unsigned short bKCWINS;
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
} Ukey_KCW __attribute__((packed));

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
} Ukey_KSW ;//__attribute__((packed));

#define KCW_SIGN 0xa5
#define KCW_SIGN_ERR 2
#define KSW_SIGN 0x5a

#endif

