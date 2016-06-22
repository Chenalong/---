//	XDiskEnum.h
#if !defined(DO_STORAGE_ENUM_H__)
#define DO_STORAGE_ENUM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winioctl.h>
#include <setupapi.h>

#include "Ukey_only.h"

// IOCTL������
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define	BUS_TYPE_USB	0x07

// ��ѯ�洢�豸���Ե�����
typedef enum _STORAGE_QUERY_TYPE {
    PropertyStandardQuery = 0,          // ��ȡ����
    PropertyExistsQuery,                // �����Ƿ�֧��
    PropertyMaskQuery,                  // ��ȡָ��������
    PropertyQueryMaxDefined		        // ��֤����
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

// ��ѯ�洢�豸��������������
typedef enum _STORAGE_PROPERTY_ID {
    StorageDeviceProperty = 0,			// ��ѯ�豸����
    StorageAdapterProperty				// ��ѯ����������
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

// ��ѯ������������ݽṹ
typedef struct _STORAGE_PROPERTY_QUERY {
	STORAGE_PROPERTY_ID PropertyId;		// �豸/������
	STORAGE_QUERY_TYPE QueryType;		// ��ѯ���� 
    UCHAR AdditionalParameters[1];		// ���������(�������������Ե�1���ֽ�)
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;


// ��ѯ������������ݽṹ
typedef struct _STORAGE_DEVICE_DESCRIPTOR {
    ULONG Version;		// �汾
    ULONG Size;			// �ṹ��С
    UCHAR DeviceType;	// �豸����
    UCHAR DeviceTypeModifier;	// SCSI-2������豸����
    BOOLEAN RemovableMedia;		// �Ƿ���ƶ�
    BOOLEAN CommandQueueing;	// �Ƿ�֧���������
    ULONG VendorIdOffset;		// �����趨ֵ��ƫ��
    ULONG ProductIdOffset;		// ��ƷID��ƫ��
    ULONG ProductRevisionOffset;	// ��Ʒ�汾��ƫ��
    ULONG SerialNumberOffset;		// ���кŵ�ƫ��
    DWORD BusType;		// ��������
    ULONG RawPropertiesLength;		// ������������ݳ���
    UCHAR RawDeviceProperties[1];	// �������������(�������������Ե�1���ֽ�)
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;


int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath);
HANDLE OpenDevice(LPCTSTR pszDevicePath);
BOOL GetDriveProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);
BOOL CheckDriveInfo(LPCTSTR pszDevicePath, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);

#endif // !defined(DO_STORAGE_ENUM_H__)
