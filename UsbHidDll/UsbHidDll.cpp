// UsbHidDll.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "UsbHidDll.h"
#include "SETUPAPI.H"
// 控制当前的模式
USBHIDTYPE GlobalType = T_Report;

char GlobalUSBHIDDevicePath[256];
char GlobalUSBHIDDeviceName[256];

// 用于查找HID设备
USBHIDDLL_API bool __stdcall USBHIDFindUSBHIDDevice()
{
    GUID Guid;
    HidD_GetHidGuid(&Guid);

    void* info;
    info=SetupDiGetClassDevs(&Guid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
    if (info!=INVALID_HANDLE_VALUE) 
    {
        DWORD devIndex;
        SP_INTERFACE_DEVICE_DATA ifData;
        ifData.cbSize=sizeof(ifData);

        for (devIndex=0;SetupDiEnumDeviceInterfaces(info, NULL, &Guid, devIndex, &ifData);++devIndex)
        {
            DWORD needed;

            SetupDiGetDeviceInterfaceDetail(info, &ifData, NULL, 0, &needed, NULL);

            PSP_INTERFACE_DEVICE_DETAIL_DATA detail=(PSP_INTERFACE_DEVICE_DETAIL_DATA)new BYTE[needed];
            detail->cbSize=sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
            SP_DEVINFO_DATA did={sizeof(SP_DEVINFO_DATA)};

            if (SetupDiGetDeviceInterfaceDetail(info, &ifData, detail, needed, NULL, &did))
            {
                // 查找特定设备 查到后则返回。
                if(strstr(detail->DevicePath, "vid_0483") != NULL)
                {
                    memset(GlobalUSBHIDDevicePath, '\0', sizeof(GlobalUSBHIDDevicePath));                 
                   memcpy(GlobalUSBHIDDevicePath, detail->DevicePath, strlen(detail->DevicePath));

                   if (!SetupDiGetDeviceRegistryProperty(info, &did, SPDRP_DEVICEDESC, NULL, 
                       (PBYTE)GlobalUSBHIDDeviceName, sizeof(GlobalUSBHIDDeviceName), NULL))
                   {
                       memset(GlobalUSBHIDDeviceName, '\0', sizeof(GlobalUSBHIDDeviceName));
                       char* hint = "(Unnamed HID device)";
                       memcpy(GlobalUSBHIDDeviceName,  hint, strlen(hint));
                   }

                   delete[] (PBYTE)detail;
                   SetupDiDestroyDeviceInfoList(info);    
                   return true;
                }
            }
            delete[] (PBYTE)detail; 
        }
        SetupDiDestroyDeviceInfoList(info);
    }
    return false;
}

USBHIDDLL_API char* __stdcall USBHIDGetDevicePath()
{
    return GlobalUSBHIDDevicePath;
}

USBHIDDLL_API char* __stdcall USBHIDGetDeviceName()
{
    return GlobalUSBHIDDeviceName;
}

// 打开设备
USBHIDDLL_API USBHANDLE __stdcall USBHIDCreateUsbHid()
{
    USBHANDLE handle = CreateFile (
        (LPCTSTR)GlobalUSBHIDDevicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,                                 //&SecurityAttributes,  //no SECURITY_ATTRIBUTES structure
        OPEN_EXISTING,                        //No special create flags
        FILE_FLAG_OVERLAPPED,                 // No special attributes
        NULL);                                // No template file

    if (handle == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    return handle;
}

// 配置设备(与写数据相关)
USBHIDDLL_API void __stdcall USBHIDSetType(USBHIDTYPE type)
{
    GlobalType = type;
}

USBHIDDLL_API void __stdcall USBHIDResetConfig()
{
    GlobalType = T_Report;
}

// 读设备
USBHIDDLL_API int __stdcall USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len)
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

USBHIDDLL_API int __stdcall USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len)
{
    DWORD numberOfBytesWriten;
    OVERLAPPED	HIDOverlapped;

    if (handle != INVALID_HANDLE_VALUE)
    {
        CancelIo(handle);

        if( GlobalType == T_Feature )
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

USBHIDDLL_API void __stdcall USBHIDCloseHandle(USBHANDLE handle)
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        memset(GlobalUSBHIDDevicePath, '\0', sizeof(GlobalUSBHIDDevicePath));
    }
}

// 获取设备一些能力信息
USBHIDDLL_API void __stdcall USBHIDGetDeviceCapabilities(USBHANDLE handle, PHIDD_ATTRIBUTES attributes, PHIDP_CAPS caps)
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