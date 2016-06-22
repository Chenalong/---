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

#define RSA_FUNC 0x71    // RSA ��ʾ
#define ECC_FUNC 0x72    // ECC ��ʾ
#define RNG_FUNC 0x73    // RNG ��ʾ
#define USB_FUNC 0x74    // USB ��ʾ
#define MD5_FUNC 0x81   //����MD5ֵ
#define SSF33_FUNC 0x75    // 33 ��ʾ
#define INT_FUNC 0x76    // �ж� ��ʾ
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
} Ukey_KSW;

unsigned char Ukey_transfer(HANDLE hDevice,Ukey_KCW *kcw_p,LPVOID lpInBuffer,Ukey_KSW *ksw_p,LPVOID lpOutBuffer);
unsigned char Ukey_transfer_old(HANDLE hDevice,  Ukey_KCW *kcw_p,  LPVOID lpInBuffer,  Ukey_KSW *ksw_p,  LPVOID lpOutBuffer);
void transfer_buffer_ssx20(U8 *buf,int len);
void hex_to_str(U8 *ptr,U8 *buf,int len);

#endif

