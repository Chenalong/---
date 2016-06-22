
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UKEYDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UKEYDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef UKEYDLL_EXPORTS
#define UKEYDLL_API __declspec(dllexport)
#else
#define UKEYDLL_API __declspec(dllimport)
#endif

typedef unsigned char U8;

struct _DEVICE_CERT
{
	unsigned char Host_ID[16];
	unsigned char PassWord[16];
	unsigned char Install_Time[16]; 
	unsigned char Key_ID[8]; 
	unsigned char Key_Right[8]; 
}; 
typedef struct _DEVICE_CERT DEVICE_CERT;


struct _DEVICE_LOG
{
	unsigned short Event_ID;
    unsigned char  Event_Param[64];
    unsigned char  Log_Time[16];   
	unsigned char  Status;
};
typedef struct _DEVICE_LOG DEVICE_LOG;

struct _ECC_KEYS
{
	unsigned char  PubKey[48];
	unsigned char  PrvKey[24];
};
typedef struct _ECC_KEYS  ECC_KEYS;

UKEYDLL_API HANDLE Enum_Devices(void);
UKEYDLL_API BOOL EnableBoot(HANDLE hUKEY);
UKEYDLL_API BOOL UpdateCOS(HANDLE hUKEY, char *sFileName);
UKEYDLL_API BOOL SetUKEY_ID(HANDLE hUKEY, U8 id[]);
UKEYDLL_API BOOL GetUKEY_ID(HANDLE hUKEY, U8 id[]);
UKEYDLL_API BOOL GetRNG(HANDLE hUKEY, U8 buf[], int byte_count);

UKEYDLL_API BOOL AddUKEYCert(HANDLE hUKEY, int index, DEVICE_CERT *pCert);
UKEYDLL_API BOOL ReadUKEYCert(HANDLE hUKEY, int index, DEVICE_CERT *pCert);
UKEYDLL_API BOOL DeleteUKEYCert(HANDLE hUKEY, int nIndex);


UKEYDLL_API BOOL AddLog(HANDLE hUKEY, int index, DEVICE_LOG *pLog);
UKEYDLL_API BOOL ReadLog(HANDLE hUKEY, int index, DEVICE_LOG *pLog);
UKEYDLL_API BOOL DeleteLog(HANDLE hUKEY, int nIndex);

UKEYDLL_API BOOL SetCert_Count(HANDLE hUKEY, U8 cnt);
UKEYDLL_API BOOL GetCert_Count(HANDLE hUKEY, U8 *pCnt);

UKEYDLL_API BOOL GenECC_KeyPair(HANDLE hUKEY, int *pKeyNo, U8 PrvKey[], U8 PubKey[]);
UKEYDLL_API BOOL ReadECC_Key(HANDLE hUKEY, int KeyNo, ECC_KEYS * pEccKey);
UKEYDLL_API BOOL RestoreECC_Key(HANDLE hUKEY, int KeyNo, ECC_KEYS * pEccKey);
UKEYDLL_API BOOL DeleteECC_Key(HANDLE hUKEY, int KeyNo);

UKEYDLL_API BOOL ECC_Encrypt(HANDLE hUKEY, U8 buf[], int len, U8 PubKey[], U8 ecc_encrypt_buf[]);
UKEYDLL_API BOOL ECC_Decrypt(HANDLE hUKEY, U8 ecc_encrypt_buf[], int len, int keyNo, U8 buf[]);
UKEYDLL_API BOOL ECC_Signature(HANDLE hUKEY, U8 buf[], int len, int KeyNo, U8 ecc_sign_buf[]);
UKEYDLL_API BOOL ECC_Verify(HANDLE hUKEY, U8 ecc_sign_buf[], int len, U8 PubKey[], U8 buf[]);
UKEYDLL_API BOOL MD5_Decrypt(HANDLE hUKEY, U8 plain[],U8 encrypt[]);

UKEYDLL_API BOOL DES_Encrypt(HANDLE hUKEY, U8 buf[], int len, U8 Key[], U8 des_encrypt_buf[]);
UKEYDLL_API BOOL DES_Decrypt(HANDLE hUKEY, U8 des_encrypt_buf[], int len, U8 Key[], U8 buf[]);

UKEYDLL_API void SHA1(unsigned char hash[], unsigned char msg[], unsigned int mbytes);