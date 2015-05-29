// UsbHidDll.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "UsbHidDll.h"

USBHIDTYPE globalType = T_Report;
    
// 打开设备
USBHIDDLL_API USBHANDLE USBHIDCreateUsbHid(char* devicePath)
{
    USBHANDLE handle = CreateFile (
        (LPCTSTR)devicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,                                 //&SecurityAttributes,  //no SECURITY_ATTRIBUTES structure
        OPEN_EXISTING,                        //No special create flags
        FILE_FLAG_OVERLAPPED,                 // No special attributes
        NULL);                                // No template file

    return handle;
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
    DWORD numberOfBytesWriten;
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
                &numberOfBytesWriten,
                (LPOVERLAPPED) &HIDOverlapped);
        }
    }
    return numberOfBytesWriten;
}

// 获取设备一些信息
USBHIDDLL_API void USBHIDGetDeviceCapabilities(USBHANDLE handle, PHIDD_ATTRIBUTES attributes, PHIDP_CAPS caps)
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        HidD_GetAttributes(handle, attributes);

        //Get the Capabilities structure for the device.

        PHIDP_PREPARSED_DATA	PreparsedData;

        /*
        API function: HidD_GetPreparsedData
        Returns: a pointer to a buffer containing the information about the device's capabilities.
        Requires: A handle returned by CreateFile.
        There's no need to access the buffer directly,
        but HidP_GetCaps and other API functions require a pointer to the buffer.
        */

        HidD_GetPreparsedData (handle, &PreparsedData);
        //DisplayLastError("HidD_GetPreparsedData: ");

        /*
        API function: HidP_GetCaps
        Learn the device's capabilities.
        For standard devices such as joysticks, you can find out the specific
        capabilities of the device.
        For a custom device, the software will probably know what the device is capable of,
        and the call only verifies the information.
        Requires: the pointer to the buffer returned by HidD_GetPreparsedData.
        Returns: a Capabilities structure containing the information.
        */

        HidP_GetCaps (PreparsedData, caps);

        //No need for PreparsedData any more, so free the memory it's using.
        HidD_FreePreparsedData(PreparsedData);
        //DisplayLastError("HidD_FreePreparsedData: ") ;
        
    }
}