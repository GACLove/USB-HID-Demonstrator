// UsbHidDll.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "UsbHidDll.h"

USBHIDTYPE globalType = T_Report;
    
// 打开设备
USBHIDDLL_API USBHANDLE USBHIDCreateUsbHid(char* devicePath)
{
    return NULL;
}

// 配置设备(与写数据相关)
USBHIDDLL_API void USBHIDSetType(USBHIDTYPE type)
{
    globalType = type;
}

// 读设备
USBHIDDLL_API int USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len)
{
    DWORD numberOfByteRead = 0;
    OVERLAPPED	HIDOverlapped;

    if (handle != INVALID_HANDLE_VALUE)
    {
        CancelIo(handle);

        int Result = ReadFile 
            (handle, 
            &byte, 
            len, 
            &numberOfByteRead,
            (LPOVERLAPPED) &HIDOverlapped);
    }

    return numberOfByteRead;
}

// 写设备
USBHIDDLL_API int USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len)
{
    DWORD NumberOfBytesWriten;
    OVERLAPPED	HIDOverlapped;

    if (handle != INVALID_HANDLE_VALUE)
    {
        CancelIo(handle);

        if( globalType == T_Feature )
        {
            HidD_SetFeature 
                (handle,
                byte,
               len); 
        }
        else
        {
            WriteFile 
                (handle, 
                &byte, 
                len, 
                &NumberOfBytesWriten,
                (LPOVERLAPPED) &HIDOverlapped);
        }
    }
    return NumberOfBytesWriten;
}

// 获取设备一些信息
USBHIDDLL_API void USBHIDGetDeviceCapabilitiess(USBHANDLE handle, HIDP_CAPS* caps)
{

}