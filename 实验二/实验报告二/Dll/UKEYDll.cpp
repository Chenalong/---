// UKEYDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UKEYDll.h"
#include "ukey_only.h"
#include "string.h"
#include "UDiskEnum.h"
#include <stdio.h>
#include "sh1.h"


#define USB_DEVICE_NAME		"\\\\.\\Cacti_Device"
#define START_ADDRESS		0x80008000
#define PAGE_SIZE			1024
#define BUF_SIZE			2048
#define MAX_DEVICE			16

static const GUID DiskClassGuid =
		{0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b};

Ukey_KCW	m_kcw;
Ukey_KSW	m_ksw;
U8			data_send_buf[BUF_SIZE] = {0};
U8			data_receive_buf[BUF_SIZE] = {0};



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



//=================================================================
UKEYDLL_API HANDLE Enum_Devices(void)
{
	BOOL bFound = FALSE;
	HANDLE hDevice;
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
	int nDevice;
	int i;
	char* szDevicePath[MAX_DEVICE];		// 设备路径
	LPGUID lpGuid = (LPGUID)&DiskClassGuid;
    // 分配需要的空间
	for(i=0;i<MAX_DEVICE;i++) szDevicePath[i]=new char[256];

	pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR) new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
	pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
    
	// 对感兴趣的类型的存储设备，逐一获取设备信息
	// 取设备路径
	nDevice=::GetDevicePath(lpGuid, szDevicePath);
	// 对同一类的存储设备，逐一获取设备信息
	for(i=0; i<nDevice; i++)
	{
		// 打开设备
		hDevice=OpenDevice(szDevicePath[i]);
		if(hDevice != INVALID_HANDLE_VALUE)
		{
			// 取设备信息
			GetDriveProperty(hDevice, pDevDesc);
			if(CheckDriveInfo(szDevicePath[i], pDevDesc))
			{
				//MessageBox( "设备已经找到！", "OK", MB_OK);
				bFound = TRUE;
				break;
			}
			// 关闭设备
			CloseHandle(hDevice);
		}
	}
	// 释放空间
	delete pDevDesc;
	for(i=0;i<MAX_DEVICE;i++) delete []szDevicePath[i];
	
	if(bFound) 
		return hDevice;
	else
		return NULL;
}



UKEYDLL_API BOOL EnableBoot(HANDLE hUKEY)
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = BOOT_BACK;

	// 初始化ksw包
    memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	return TRUE;
}


UKEYDLL_API BOOL UpdateCOS(HANDLE hUKEY, char *sFileName)
{
	HANDLE hCurUKEY;
	char sLinkName[256]={'\0'};
    for(int i=0; i<MAX_DEVICE; i++)
    {
       sprintf(sLinkName, "%s%d", USB_DEVICE_NAME, i);
       hCurUKEY = CreateFile(sLinkName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
       if(hCurUKEY != NULL && hCurUKEY != INVALID_HANDLE_VALUE)
           break;

    }
	if((hCurUKEY == NULL) || (hCurUKEY == INVALID_HANDLE_VALUE))
	{
		MessageBox(NULL, "Can not find device!", "Update", MB_OK);
		CloseHandle(hCurUKEY);
		return FALSE;
	}
	
	// 打开文件
    HANDLE hSrcFile = CreateFile(sFileName,          
								  GENERIC_READ,       
								  FILE_SHARE_READ,           
								  NULL,
								  OPEN_EXISTING,  
								  FILE_ATTRIBUTE_NORMAL,  
								  NULL         
							     );
    if(hSrcFile == NULL)
    {
        MessageBox(NULL, "无法打开指定的文件!", "提示", MB_OK);
        CloseHandle(hSrcFile);
        return FALSE;
    }

    unsigned long dwSrcFileSize = GetFileSize(hSrcFile,NULL);
    if(dwSrcFileSize <= 0)
	{
		MessageBox(NULL, "获取文件长度出错", "提示", MB_OK);
		return FALSE;
	}

	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包  
    memset(&m_kcw,0,sizeof(Ukey_KCW));
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWCLA = 0x80;
    m_kcw.bKCWINS = 0x30;

    // 初始化ksw包
    memset(&m_ksw,0,sizeof(Ukey_KSW));
   
    // 初始化buffer
    unsigned char datain_buf[PAGE_SIZE+4] = {0};
    unsigned char dataout_buf[PAGE_SIZE+4] = {0};
 
    unsigned int m_start_address = START_ADDRESS;
    unsigned long total_down_data=0;
    DWORD read_len;

    // download
    while((!m_kcw.dKCWtag)) //GetFileSize
    {
	    m_kcw.flag.ads = m_start_address;
		ReadFile(hSrcFile, datain_buf, PAGE_SIZE, &read_len, NULL);
        total_down_data += read_len;
        if(total_down_data >= dwSrcFileSize)
		    m_kcw.dKCWtag = 1;

        m_kcw.dKCWOutLength = (unsigned short)read_len;
        unsigned long page_sum = 0;
        for(DWORD i=0; i<read_len; i++)
        {
		    page_sum += datain_buf[i];
        }
        m_kcw.Rev.sum = page_sum;

        Ukey_transfer_old(hCurUKEY, &m_kcw, datain_buf, &m_ksw, dataout_buf);
        if((m_ksw.dKSWSignature != 0x5a) || (m_ksw.bKSWStatus != 0xfb))
        {
		    CloseHandle(hCurUKEY);
            return FALSE;
        }
       
        m_start_address += read_len;
    }

    CloseHandle(hSrcFile);
	CloseHandle(hCurUKEY);
    return TRUE;
}


UKEYDLL_API BOOL SetUKEY_ID(HANDLE hUKEY, U8 id[])
{
	int start_addr = 0x88000000-8;
    int d_len = 8;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, id, &m_ksw, data_receive_buf);

	if(memcmp(id, data_receive_buf, d_len))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}	
}



UKEYDLL_API BOOL GetUKEY_ID(HANDLE hUKEY, U8 id[])
{
	int start_addr = 0x88000000-8;
    int d_len = 8;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = READ_FIX_ADDRESS;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;

    // 初始化ksw包
	memset(&m_ksw,0,16);

	//通过id查找证书
	// m_kcw.XXX = host_id
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	memcpy(id, data_receive_buf, 8);

	return TRUE;
}




UKEYDLL_API BOOL SetCert_Count(HANDLE hUKEY, U8 cnt)
{
	int start_addr = 0x88000000-16;
    int d_len = 1;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, &cnt, &m_ksw, data_receive_buf);

	if(memcmp(&cnt, data_receive_buf, d_len))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}	
}






UKEYDLL_API BOOL GetCert_Count(HANDLE hUKEY, U8 *pCnt)
{
	int start_addr = 0x88000000-16;
    int d_len = 1;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = READ_FIX_ADDRESS;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;

    // 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	memcpy(pCnt, data_receive_buf, d_len);

	return TRUE;
}



UKEYDLL_API BOOL GetRNG(HANDLE hUKEY, U8 buf[], int byte_count)
{  
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = RNG_FUNC;
    m_kcw.flag.ads = byte_count;
    m_kcw.dKCWtag = 0;
    m_kcw.dKCWInLength =  byte_count;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	memcpy(buf, data_receive_buf, byte_count);

	return TRUE;
}


UKEYDLL_API BOOL AddUKEYCert(HANDLE hUKEY, int index, DEVICE_CERT *pCert)
{
    int d_len = sizeof(DEVICE_CERT);
	int start_addr = 0x88000000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, pCert, &m_ksw, data_receive_buf);

	if(memcmp(pCert, data_receive_buf, d_len))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}	
}


UKEYDLL_API BOOL ReadUKEYCert(HANDLE hUKEY, int index, DEVICE_CERT *pCert)
{
    int d_len = sizeof(DEVICE_CERT);
	int start_addr = 0x88000000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = READ_FIX_ADDRESS;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;

    // 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	memcpy(pCert, data_receive_buf, sizeof(DEVICE_CERT));

	return TRUE;
        
}



UKEYDLL_API BOOL DeleteUKEYCert(HANDLE hUKEY, int index)
{
    int d_len = sizeof(DEVICE_CERT);
	int start_addr = 0x88000000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	return TRUE;

}


UKEYDLL_API BOOL AddLog(HANDLE hUKEY, int index, DEVICE_LOG *pLog)
{
    int d_len = sizeof(DEVICE_LOG);
	int start_addr = 0x88002000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, pLog, &m_ksw, data_receive_buf);

	if(memcmp(pLog, data_receive_buf, d_len))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}	
}


UKEYDLL_API BOOL ReadLog(HANDLE hUKEY, int index, DEVICE_LOG *pLog)
{
    int d_len = sizeof(DEVICE_LOG);
	int start_addr = 0x88002000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = READ_FIX_ADDRESS;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;

    // 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	memcpy(pLog, data_receive_buf, sizeof(DEVICE_LOG));

	return TRUE;
        
}


UKEYDLL_API BOOL DeleteLog(HANDLE hUKEY, int index)
{
    int d_len = sizeof(DEVICE_LOG);
	int start_addr = 0x88002000 + index * d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = d_len;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);

	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

	return TRUE;

}



//密钥产生
UKEYDLL_API BOOL GenECC_KeyPair(HANDLE hUKEY, int *pKeyNo, U8 PrvKey[], U8 PubKey[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = ECC_FUNC;
    m_kcw.dKCWtag = 4;
    m_kcw.dKCWInLength =  72;

	// 初始化ksw包
	memset(&m_ksw,0,16);

    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

    //Private key
    memcpy(PrvKey, data_receive_buf, 24);
    //Public key
    memcpy(PubKey, data_receive_buf+24, 48);

	//暂时置为0
	*pKeyNo = 0;

	return TRUE;
}


//根据密钥索引号读取ECC密钥
BOOL ReadECC_Key(HANDLE hUKEY, int KeyNo, ECC_KEYS *pEccKey)
{
	// 初始化buffer
    int d_len = sizeof(ECC_KEYS);
	int start_addr = 0x88000000+4096+KeyNo*d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = READ_FIX_ADDRESS;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = 4;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, &KeyNo, 4);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);
	memcpy(pEccKey, data_receive_buf, sizeof(ECC_KEYS));

	return TRUE;
}




//将其他设备的公钥导入UKEY，以便认证其签名或将数据加密后发送
BOOL RestoreECC_Key(HANDLE hUKEY, int KeyNo, ECC_KEYS *pEccKey)
{
	// 初始化buffer
    int d_len = sizeof(ECC_KEYS);
	int start_addr = 0x88000000+4096+KeyNo*d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = 4;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, &KeyNo, 4);
    Ukey_transfer(hUKEY, &m_kcw, pEccKey, &m_ksw, data_receive_buf);

	return TRUE;
}


BOOL DeleteECC_Key(HANDLE hUKEY, int KeyNo)
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

	// 初始化buffer
    int d_len = sizeof(ECC_KEYS);
	int start_addr = 0x88000000+4096+KeyNo*d_len;

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = FIXED_ADDRESS_TEST;
    m_kcw.flag.ads = start_addr;
    m_kcw.dKCWInLength = 4;
    m_kcw.dKCWOutLength = d_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, &KeyNo, 4);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);


	return TRUE;
}


//ECC加密，直接使用公钥，将来可以通过软件在内存进行
UKEYDLL_API BOOL ECC_Encrypt(HANDLE hUKEY, U8 buf[], int len, U8 PubKey[], U8 ecc_encrypt_buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = ECC_FUNC;

	m_kcw.dKCWtag = 2;
    m_kcw.dKCWOutLength = len;
    m_kcw.dKCWInLength =  72+1;
    
	// 初始化ksw包
    memset(&m_ksw,0,16);

    memcpy(data_send_buf, buf, len);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

    memcpy(ecc_encrypt_buf,data_receive_buf,73);

	return TRUE;
    
}


//ECC解密，使用密钥号，必须在芯片内部进行
UKEYDLL_API BOOL ECC_Decrypt(HANDLE hUKEY, U8 ecc_encrypt_buf[], int len, int keyNo, U8 buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = ECC_FUNC;

	m_kcw.dKCWtag = 3;
    m_kcw.dKCWOutLength = 73;
    m_kcw.dKCWInLength =  100;
    
	// 初始化ksw包
    memset(&m_ksw,0,16);

    memcpy(data_send_buf, ecc_encrypt_buf,73);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

    memcpy(buf,data_receive_buf+1,data_receive_buf[0]);

	return TRUE;
}


//ECC签名，使用密钥号，必须在芯片内部进行
UKEYDLL_API BOOL ECC_Signature(HANDLE hUKEY, U8 buf[], int len, int KeyNo, U8 ecc_sign_buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = ECC_FUNC;

	m_kcw.dKCWtag = 0;
    m_kcw.dKCWOutLength = len;
    m_kcw.dKCWInLength =  48;
    
	// 初始化ksw包
    memset(&m_ksw,0,16);

    memcpy(data_send_buf, buf, len);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);
    memcpy(ecc_sign_buf, data_receive_buf, 48);
   
	return TRUE;
}


//ECC认证，直接使用公钥，将来可以通过软件在内存进行
UKEYDLL_API BOOL ECC_Verify(HANDLE hUKEY, U8 ecc_sign_buf[], int len, U8 PubKey[], U8 buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);

    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
    m_kcw.bKCWINS = ECC_FUNC;

	m_kcw.dKCWtag = 1;
    m_kcw.dKCWOutLength = len + 48;
    //m_kcw.dKCWInLength =  48;
    
	// 初始化ksw包
    memset(&m_ksw,0,16);

    memcpy(data_send_buf, buf, len);
    memcpy(data_send_buf+len, ecc_sign_buf, 48);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);

    if(m_ksw.DKSWSW1 != 0xfb)
		return FALSE; //Verification failed!
    else
        return TRUE;  //Verification passed!

}



//DES加密，8字节密钥
UKEYDLL_API BOOL DES_Encrypt(HANDLE hUKEY, U8 buf[], int len, U8 Key[], U8 des_encrypt_buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);
    
    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = DES_FUNC_ENCRYPT;
    m_kcw.dKCWInLength = len+8;
    m_kcw.dKCWOutLength = len+8;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, Key, 8);
	memcpy(data_send_buf, buf, len);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);
	memcpy(des_encrypt_buf, data_receive_buf, len);


	return TRUE;
}

//DES解密，8字节密钥
UKEYDLL_API BOOL DES_Decrypt(HANDLE hUKEY, U8 des_encrypt_buf[], int len, U8 Key[], U8 buf[])
{
	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);
    
    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = DES_FUNC_DECRYPT;
    m_kcw.dKCWInLength = len+8;
    m_kcw.dKCWOutLength = len+8;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, Key, 8);
	memcpy(data_send_buf, des_encrypt_buf, len);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);
	memcpy(buf, data_receive_buf, len);

	return TRUE;
}

//MD5加密算法实现
UKEYDLL_API BOOL MD5_Decrypt(HANDLE hUKEY, U8 plain[],U8 encrypt[])
{
	//MessageBox("hello_world");
	int plain_len = strlen((char*)plain);

	// 初始化buffer
	memset(data_send_buf, 0, BUF_SIZE);
    memset(data_receive_buf, 0, BUF_SIZE);
    
    // 初始化kcw包
    memset(&m_kcw,0,16);
    m_kcw.dKCWSignature = 0xa5;
	m_kcw.bKCWINS = MD5_FUNC;
    m_kcw.dKCWInLength = 16;   //加密完用字符串表示长度为16
    m_kcw.dKCWOutLength = plain_len;

	// 初始化ksw包
	memset(&m_ksw,0,16);
	memcpy(data_send_buf, plain, plain_len);
    Ukey_transfer(hUKEY, &m_kcw, data_send_buf, &m_ksw, data_receive_buf);
	data_receive_buf[17] = '\0';
	memcpy(encrypt, data_receive_buf, 17);
	printf("============m_ksw.DKSWSW1 is %c================\n",m_ksw.DKSWSW1);
	if(m_ksw.DKSWSW1 == 0x01)
		return TRUE; //Verification failed!
    else
        return FALSE;  //Verification passed!
}



//SHA_1算法，芯片外部执行
UKEYDLL_API void SHA1(unsigned char hash[], unsigned char msg[], unsigned int mbytes)
{
	unsigned int i;
	sha sh;
	shs_init(&sh);
	for(i=0; i<mbytes; i++) shs_process(&sh, msg[i]);
	shs_hash(&sh,(char*)hash);
}
