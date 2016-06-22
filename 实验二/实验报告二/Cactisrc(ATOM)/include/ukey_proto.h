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
//           USB_KEY �������KCW���ṹ����
// ���ֶε��������£�
// dKCWSignature:  ����ǩ����KCW���Ŀ�ʼ��־��ֵΪ0xA5��ָ������һ��KCW����   
// dKCWTag:        ����������COMMAND���ǩ���豸��Ҫ������Ӧ��KCW��Ӧ�����dKWTag�ֶ���KSW��������CSW�����ظ�������
//                 ��������COS���������������ؽ�����־�� 
// dKCWOutLength:  ����KCWϣ��Bulk-out�˵㴫�͵����ݳ��ȡ��������Ϊ0�����δ��������ݴ��͡�
// dKCWInLength:   ����COS��Ӧ�����ݳ��ȡ��������Ϊ0�����δ��������ݴ��䡣
// bKCWFlag :      �����ֽڣ��������Զ�����;��
// bKCWCLA:        7816-3 �е�CLA
// bKCWINS:        7816-3 �е�INS
// bKCWP1:         7816-3 �е�P1
// bKCWP2:         7816-3 �е�P2
// dKCWReseved0:   �����ֽڣ��������Զ�����;��   
// dKCWReseved1:   �����ֽڣ��������Զ�����;��
// dKCWReseved2:   �����ֽڣ��������Զ�����;��
// dKCWReseved3:   �����ֽڣ��������Զ�����;��
// dKCWReseved4:   �����ֽڣ��������Զ�����;��
// ads:            ������COS������������ָ�����ص��ĵ�ַ��������������Զ��������
// sum:            ������COS��������������ű����ݿ�ĺ�ֵ��������������Զ��������
//
// ˵����
//       KCW���̶�����Ϊ16���ֽڣ����ṹ����7816-3��ͬʱ���н϶�ı�����������չ���    
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
//               USB_KEY ״̬���ṹ����
// ���ֶεĺ��壺
// dKSWSignature:    ����ǩ����CSW���Ŀ�ʼ��־��ֵΪ0x5A��ָ������һ��KBW����   
// dKSWTag:          �豸��Ҫ������Ӧ��KBW��Ӧ����䡣�ɸ��������Զ��塣
// dKWDataResidue:   Ϊ����7816-3Э�����á�ʣ�����ݳ��ȡ������������ݴ�����̡�
//                   ���������豸��DATA-OUT,��ֵΪdKCWDataTransferlLength���豸ʵ�ʽ��յ���������֮�
//                   ���豸��������DATA-IN, ��ֵΪdKCWDataTransferlLength���豸ʵ�ʷ��͵�������֮���ֵ���ó���dKCWDataTransferLength��
// bKSWStatus��      ���ֶ�����ָʾKCW����ִ�гɹ�����ʧ�ܡ�
// DKSWSW1, DKSWSW1�����ֶ�����ָʾCOS����ִ�гɹ�����ʧ�ܡ�
// bKSWFlag��        �����ֽڣ��������Զ�����;��        
// dKSWReseved0:     �����ֽڣ��������Զ�����;��   
// dKSWReseved1:     �����ֽڣ��������Զ�����;��
// dKSWReseved2:     �����ֽڣ��������Զ�����;��
// dKSWReseved3:     �����ֽڣ��������Զ�����;��
// dKSWReseved4:     �����ֽڣ��������Զ�����;��
// dKSWReseved5:     �����ֽڣ��������Զ�����;��   
// dKSWReseved6:     �����ֽڣ��������Զ�����;��
// dKSWReseved7:     �����ֽڣ��������Զ�����;��
// dKSWReseved8:     �����ֽڣ��������Զ�����;��
// ˵����
//       KSW���̶�����Ϊ16���ֽڣ����ṹ����7816-3��ͬʱ���н϶�ı�����������չ���
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

