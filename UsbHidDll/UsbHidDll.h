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
//#include "hid.h"
//#include "setupapi.h"
//#include "usb100.h"

#define USBHANDLE void* 

#ifdef __cplusplus
extern "C" {
#endif


enum USBHIDTYPE
{
    T_Report,
    T_Feature,
};

//获取DevicePath（利用文件查找方式返回多个设备path）

// 打开设备
USBHIDDLL_API USBHANDLE USBHIDCreateUsbHid(char* devicePath);

// 配置设备(与写数据相关)
USBHIDDLL_API void USBHIDSetType(USBHIDTYPE type);

// 读设备
USBHIDDLL_API int USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len);

// 写设备
USBHIDDLL_API int USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len);

// 获取设备一些信息
USBHIDDLL_API void USBHIDGetDeviceCapabilities(USBHANDLE handle, HIDP_CAPS* caps);

#ifdef __cplusplus
}
#endif