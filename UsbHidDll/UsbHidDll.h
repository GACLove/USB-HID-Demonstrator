#ifndef USBHIDDLL_H
#define USBHIDDLL_H
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 USBHIDDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// USBHIDDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef USBHIDDLL_EXPORTS
#define USBHIDDLL_API __declspec(dllexport)
#else
#define USBHIDDLL_API __declspec(dllimport)
#endif

#include "hidsdi++.h"

#define USBHANDLE void* 

#ifdef __cplusplus
extern "C" {
#endif


enum USBHIDTYPE
{
    T_Report,
    T_Feature,
};
/************************************************************************/
/*                     用于查找设备和识别设备的API                          */
/************************************************************************/
//查找设备 如果找到则返回true，否则返回false
USBHIDDLL_API bool __stdcall USBHIDFindUSBHIDDevice();

//获取DevicePath
USBHIDDLL_API char* __stdcall USBHIDGetDevicePath();

//获取设备名称
USBHIDDLL_API char* __stdcall USBHIDGetDeviceName();

/************************************************************************/
/*                      获取和配置设备信息                                  */
/************************************************************************/

// 配置设备(与写数据相关)
USBHIDDLL_API void __stdcall USBHIDSetType(USBHIDTYPE type);

// 重置设备一些配置信息
USBHIDDLL_API void __stdcall  USBHIDResetConfig();

// 获取设备一些信息
USBHIDDLL_API void __stdcall USBHIDGetDeviceCapabilities(USBHANDLE handle, PHIDD_ATTRIBUTES attributes, PHIDP_CAPS caps);


/************************************************************************/
/*             打开，关闭和读写操作                                         */
/************************************************************************/

// 打开设备
USBHIDDLL_API USBHANDLE __stdcall USBHIDCreateUsbHid();

// 读设备
USBHIDDLL_API int __stdcall USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len);

// 写设备
USBHIDDLL_API int __stdcall USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len);

// 关闭设备
USBHIDDLL_API void __stdcall USBHIDCloseHandle(USBHANDLE handle);

#ifdef __cplusplus
}
#endif

#endif