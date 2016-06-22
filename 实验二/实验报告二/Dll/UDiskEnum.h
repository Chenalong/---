//	XDiskEnum.h
#if !defined(DO_STORAGE_ENUM_H__)
#define DO_STORAGE_ENUM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winioctl.h>
#include <setupapi.h>

#include "Ukey_only.h"

// IOCTL控制码
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define	BUS_TYPE_USB	0x07

// 查询存储设备属性的类型
typedef enum _STORAGE_QUERY_TYPE {
    PropertyStandardQuery = 0,          // 读取描述
    PropertyExistsQuery,                // 测试是否支持
    PropertyMaskQuery,                  // 读取指定的描述
    PropertyQueryMaxDefined		        // 验证数据
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

// 查询存储设备还是适配器属性
typedef enum _STORAGE_PROPERTY_ID {
    StorageDeviceProperty = 0,			// 查询设备属性
    StorageAdapterProperty				// 查询适配器属性
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

// 查询属性输入的数据结构
typedef struct _STORAGE_PROPERTY_QUERY {
	STORAGE_PROPERTY_ID PropertyId;		// 设备/适配器
	STORAGE_QUERY_TYPE QueryType;		// 查询类型 
    UCHAR AdditionalParameters[1];		// 额外的数据(仅定义了象征性的1个字节)
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;


// 查询属性输出的数据结构
typedef struct _STORAGE_DEVICE_DESCRIPTOR {
    ULONG Version;		// 版本
    ULONG Size;			// 结构大小
    UCHAR DeviceType;	// 设备类型
    UCHAR DeviceTypeModifier;	// SCSI-2额外的设备类型
    BOOLEAN RemovableMedia;		// 是否可移动
    BOOLEAN CommandQueueing;	// 是否支持命令队列
    ULONG VendorIdOffset;		// 厂家设定值的偏移
    ULONG ProductIdOffset;		// 产品ID的偏移
    ULONG ProductRevisionOffset;	// 产品版本的偏移
    ULONG SerialNumberOffset;		// 序列号的偏移
    DWORD BusType;		// 总线类型
    ULONG RawPropertiesLength;		// 额外的属性数据长度
    UCHAR RawDeviceProperties[1];	// 额外的属性数据(仅定义了象征性的1个字节)
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;


int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath);
HANDLE OpenDevice(LPCTSTR pszDevicePath);
BOOL GetDriveProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);
BOOL CheckDriveInfo(LPCTSTR pszDevicePath, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);

#endif // !defined(DO_STORAGE_ENUM_H__)
