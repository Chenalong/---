#include "stdafx.h"
#include "UDiskEnum.h"
#include "Setupapi.h"

// SetupDiGetInterfaceDeviceDetail����Ҫ���������
#define INTERFACE_DETAIL_SIZE	(1024)

// ��GUID����豸·��
// lpGuid: GUIDָ��
// pszDevicePath: �豸·��ָ���ָ��
// ����ֵ: �ɹ��õ����豸·������
int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath)
{
	HDEVINFO hDevInfoSet;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	int nCount;
	BOOL bResult;

	// ȡ��һ����GUID��ص��豸��Ϣ�����
	hDevInfoSet = ::SetupDiGetClassDevs(lpGuid, 	// class GUID 
			NULL,									// �޹ؼ��� 
			NULL,									// ��ָ�������ھ�� 
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);	// Ŀǰ���ڵ��豸

	// ʧ��...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	// �����豸�ӿ����ݿռ�
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);
	
	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;
	
	// �豸���=0,1,2... ��һ�����豸�ӿڣ���ʧ��Ϊֹ
	while (bResult)
	{
		ifdata.cbSize=sizeof(ifdata);
	
		// ö�ٷ��ϸ�GUID���豸�ӿ�
		bResult = ::SetupDiEnumDeviceInterfaces(
				hDevInfoSet,			// �豸��Ϣ�����
				NULL,					// ���������豸����
				lpGuid,					// GUID
				(ULONG)nCount,			// �豸��Ϣ������豸���
				&ifdata);				// �豸�ӿ���Ϣ

		if(bResult)
		{
			// ȡ�ø��豸�ӿڵ�ϸ��(�豸·��)
			bResult = SetupDiGetInterfaceDeviceDetail(
					hDevInfoSet,			// �豸��Ϣ�����
					&ifdata,				// �豸�ӿ���Ϣ
					pDetail,				// �豸�ӿ�ϸ��(�豸·��)
					INTERFACE_DETAIL_SIZE,	// �����������С
					NULL,					// ������������������С(ֱ�����趨ֵ)
					NULL);					// ���������豸����

			if(bResult)
			{
				// �����豸·�������������
				::strcpy(pszDevicePath[nCount], pDetail->DevicePath);

				// ��������ֵ
				nCount++;
			}
		}
	}

	// �ͷ��豸�ӿ����ݿռ�
	::GlobalFree(pDetail);

	// �ر��豸��Ϣ�����
	::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}

// ���豸
// pszDevicePath: �豸��·��
HANDLE OpenDevice(LPCTSTR pszDevicePath)
{
	HANDLE hDevice;

	// ���豸
	hDevice= ::CreateFile(pszDevicePath,			// �豸·��
			GENERIC_READ | GENERIC_WRITE,			// ��д��ʽ
			FILE_SHARE_READ | FILE_SHARE_WRITE,		// ����ʽ
			NULL,									// Ĭ�ϵİ�ȫ������
			OPEN_EXISTING,							// ������ʽ
			0,										// ���������ļ�����
			NULL);									// �������ģ���ļ�

	return hDevice;
}

// ȡ�豸������Ϣ
// hDevice -- �豸���
// pDevDesc -- ������豸������������Ϣ������ָ��(����������һ���������)
BOOL GetDriveProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	// ��ѯ�������
	DWORD dwOutBytes;				// IOCTL������ݳ���
	BOOL bResult;					// IOCTL����ֵ

	// ָ����ѯ��ʽ
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// ��IOCTL_STORAGE_QUERY_PROPERTYȡ�豸������Ϣ
	bResult = ::DeviceIoControl(hDevice,			// �豸���
			IOCTL_STORAGE_QUERY_PROPERTY,			// ȡ�豸������Ϣ
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	// �������ݻ�����
			pDevDesc, pDevDesc->Size,				// ������ݻ�����
			&dwOutBytes,							// ������ݳ���
			(LPOVERLAPPED)NULL);					// ��ͬ��I/O	

	return bResult;
}


BOOL CheckDriveInfo(LPCTSTR pszDevicePath, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	char strTmp[256];
	char* p;
	p= (char*)pDevDesc;

	if(pDevDesc->BusType!=BUS_TYPE_USB)
	{
		return FALSE;
	}
	if(!pDevDesc->RemovableMedia)
	{
		//return FALSE;
	}
	//	����Ʒ����
	//strTmp = (pDevDesc->VendorIdOffset ? &p[pDevDesc->VendorIdOffset] : "");
	if(pDevDesc->VendorIdOffset)
		strcpy(strTmp, &p[pDevDesc->VendorIdOffset]);
	else
		strcpy(strTmp, "");
	
	
	if(strcmp(strTmp, "ATOM    ") == 0)
		return TRUE;
	else
		return FALSE;
}


