/* Typedefs and defines */
#ifndef   __MYDEF_H
#define   __MYDEF_H

#include <types.h>

// 调试信息条件编译定义
#define PRINT_AEmodN_ON 0
#define PRINT_GET_KEY 0

extern long rngctl_data;
//#define RNG_CNTL_VAL 0x3da // 24MHZ
//#define RNG_CNTL_VAL 0x795 // 48MHZ
//#define RNG_CNTL_VAL 0xf15 // 96MHZ

// 命令定义
#define RSATEST     0x35
#define DESTEST     0x34
#define DOWMTEST    0x30
#define RNGTEST     0x3c
#define GETKEYTEST  0x39  
#define MIXEDENTEST   0x36
#define MIXEDDETEST   0x37 
#define DESSELFTEST 0x31
#define USBSPEEDTEST 0x32  //USB speed test
#define DESENDECRYPT  0x64
#define RSAENDECRYPT  0x65
#define GETCHALLENGE 0x84   // get challenge
#define CREATFILE 0xE0      // creat file
#define WRITEKEY 0xD4       // write key
#define READBINARY 0xB0     // read binary
#define WRITEBINARY 0xD0   // write binary
#define UPDATEBINARY 0xD6   // update binary
#define READRECORD 0xB2     // read record
#define APPENDRECORD 0xE2   // append record
#define UPDATERECORD 0xDC   // update record
#define SELECTFILE 0xA4     // select file
#define GETPRIME 0x3a     // get prime
#define FILESYSINIT 0x3e     // file system init
#define GETFILETREE 0x3b    // get file tree
#define EEPROMTEST 0x3f    // eeprom test
#define ECCTEST 0x70    // ECC test

#define RSA_FUNC 0x71    // RSA 演示
#define ECC_FUNC 0x72    // RSA 演示
#define RNG_FUNC 0x73    // RNG 演示
#define USB_FUNC 0x74    // USB 演示

#define SSF33_FUNC 0x75    // 33 演示
#define INT_FUNC 0x76    // 中断 演示
#define ECC_G2M 0x77   
#define BOOT_BACK 0x78
#define Set_ID      0x79
#define Get_ID      0x80
#define MD5_FUNC 0x81 // MD5值的计算 

#define SETCLOCK_COMMAND 0x1A
#define RANDOM_EEPROM_TEST 0x1B
#define FIXED_ADDRESS_TEST 0x1C
#define READ_FIX_ADDRESS   0x1D
#define CHECK_ALL 0x1e


// 各类型变量的最大值
#define BYTE_MAX 0xff
#define WORD_MAX 0xffff
#define DWORD_MAX 0xffffffff

#define DWORD_LOW_HALF(x)  ((x) & 0xffff)
#define DWORD_HIGH_HALF(x)  (((x)>>16) & 0xffff)
#define DWORD_TO_HIGH_HALF(x)  ((((unsigned long int)(x))<<16) & 0xffff0000)

// 大小定义
#define BIGGER    1    //SHOULD BE LARGER THAN EQUAL
#define EQUAL     0
#define SMALLER   -1

#define OK 1
#define TRUE 1
#define YES 1

#define NO 0
#define ERROR 0xff
#define FALSE 0

#define PRIME    10
#define NOTPRIME 11

// 正负定义
#define POSITIVE   1  //Must be
#define NEGATIVE  -1  //Must be

/* in the Residue, Residue[0] stands for the less significant Base Number */
#define CALCBASE     65536
#define CALCBASEBITSLEN 16
#define CALCLENGTH   80//64 ??? test
#define BIGNUMLENGTH 80//64 ??? test

#define BINARYBASE   2
#define BIGNUMBINARYLENGTH 1056 // 1024 ??? test

// 大数结构定义
struct BigNumUnit
	{
		int PosORNegFlag;
		//int LengthCapacity;  //allways is 64
		int Length;
		//DWORD Base;   //base should 2^16=Base; change WORD to DWORD  allways is 2^16
		WORD Residue[BIGNUMLENGTH+1]; //change DWORD to WORD
	};
typedef struct BigNumUnit BIGNUM;

struct BigNumUnitForBinary
	{
		int PosORNegFlag;
		int LengthCapacity;
		int Length;
		DWORD Base;   //base should 2^16>=Base>=2; change WORD to DWORD
		WORD Residue[BIGNUMBINARYLENGTH+1]; //change DWORD to WORD
	};
typedef struct BigNumUnitForBinary BIGBINARYNUM;

#define LRBINARY 0
#define RLBINARY 1

#define ENCRYPT 0
#define DECODE   1

#define PRIMESAFEPARA 10
#define LIMITEDPRIMEUPLIMIT 2000 //10000
#define UPLIMITEDALENGTH 5

#define INTR 0x1c
#define ZHOUQI 182  /* 10秒 */

#define PI 3.1415926

/*
#define ARCA_BUS_CLK 24

#if(ARCA_BUS_CLK==24)
#define RNG_CNTL 0x645
#else if(ARCA_BUS_CLK==48)
#define RNG_CNTL 0xc85
#else if(ARCA_BUS_CLK==48)
#define RNG_CNTL 0xf05
#else
#endif
*/

#endif
