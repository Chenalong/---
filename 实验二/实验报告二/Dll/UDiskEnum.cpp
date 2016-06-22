#include "stdafx.h"
#include "UDiskEnum.h"
#include "Setupapi.h"

// SetupDiGetInterfaceDeviceDetail所需要的输出长度
#define INTERFACE_DETAIL_SIZE	(1024)

// 从GUID获得设备路径
// lpGuid: GUID指针
// pszDevicePath: 设备路径指针的指针
// 返回值: 成功得到的设备路径个数
int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath)
{
	HDEVINFO hDevInfoSet;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	int nCount;
	BOOL bResult;

	// 取得一个该GUID相关的设备信息集句柄
	hDevInfoSet = ::SetupDiGetClassDevs(lpGuid, 	// class GUID 
			NULL,									// 无关键字 
			NULL,									// 不指定父窗口句柄 
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);	// 目前存在的设备

	// 失败...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	// 申请设备接口数据空间
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);
	
	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;
	
	// 设备序号=0,1,2... 逐一测试设备接口，到失败为止
	while (bResult)
	{
		ifdata.cbSize=sizeof(ifdata);
	
		// 枚举符合该GUID的设备接口
		bResult = ::SetupDiEnumDeviceInterfaces(
				hDevInfoSet,			// 设备信息集句柄
				NULL,					// 不需额外的设备描述
				lpGuid,					// GUID
				(ULONG)nCount,			// 设备信息集里的设备序号
				&ifdata);				// 设备接口信息

		if(bResult)
		{
			// 取得该设备接口的细节(设备路径)
			bResult = SetupDiGetInterfaceDeviceDetail(
					hDevInfoSet,			// 设备信息集句柄
					&ifdata,				// 设备接口信息
					pDetail,				// 设备接口细节(设备路径)
					INTERFACE_DETAIL_SIZE,	// 输出缓冲区大小
					NULL,					// 不需计算输出缓冲区大小(直接用设定值)
					NULL);					// 不需额外的设备描述

			if(bResult)
			{
				// 复制设备路径到输出缓冲区
				::strcpy(pszDevicePath[nCount], pDetail->DevicePath);

				// 调整计数值
				nCount++;
			}
		}
	}

	// 释放设备接口数据空间
	::GlobalFree(pDetail);

	// 关闭设备信息集句柄
	::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}

// 打开设备
// pszDevicePath: 设备的路径
HANDLE OpenDevice(LPCTSTR pszDevicePath)
{
	HANDLE hDevice;

	// 打开设备
	hDevice= ::CreateFile(pszDevicePath,			// 设备路径
			GENERIC_READ | GENERIC_WRITE,			// 读写方式
			FILE_SHARE_READ | FILE_SHARE_WRITE,		// 共享方式
			NULL,									// 默认的安全描述符
			OPEN_EXISTING,							// 创建方式
			0,										// 不需设置文件属性
			NULL);									// 不需参照模板文件

	return hDevice;
}

// 取设备属性信息
// hDevice -- 设备句柄
// pDevDesc -- 输出的设备描述和属性信息缓冲区指针(包含连接在一起的两部分)
BOOL GetDriveProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	// 查询输入参数
	DWORD dwOutBytes;				// IOCTL输出数据长度
	BOOL bResult;					// IOCTL返回值

	// 指定查询方式
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// 用IOCTL_STORAGE_QUERY_PROPERTY取设备属性信息
	bResult = ::DeviceIoControl(hDevice,			// 设备句柄
			IOCTL_STORAGE_QUERY_PROPERTY,			// 取设备属性信息
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	// 输入数据缓冲区
			pDevDesc, pDevDesc->Size,				// 输出数据缓冲区
			&dwOutBytes,							// 输出数据长度
			(LPOVERLAPPED)NULL);					// 用同步I/O	

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
	//	检测产品名称
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


