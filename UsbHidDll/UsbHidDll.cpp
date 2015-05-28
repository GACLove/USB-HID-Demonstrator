// UsbHidDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//
#include "stdafx.h"
#include "UsbHidDll.h"

USBHIDTYPE globalType = T_Report;
    
// ���豸
USBHIDDLL_API USBHANDLE USBHIDCreateUsbHid(char* devicePath)
{
    return NULL;
}

// �����豸(��д�������)
USBHIDDLL_API void USBHIDSetType(USBHIDTYPE type)
{
    globalType = type;
}

// ���豸
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

// д�豸
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

// ��ȡ�豸һЩ��Ϣ
USBHIDDLL_API void USBHIDGetDeviceCapabilitiess(USBHANDLE handle, HIDP_CAPS* caps)
{

}