// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� USBHIDDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// USBHIDDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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


// ���豸
USBHIDDLL_API USBHANDLE USBHIDCreateUsbHid(char* devicePath);

// �����豸(��д�������)
USBHIDDLL_API void USBHIDSetType(USBHIDTYPE type);

// ���豸
USBHIDDLL_API int USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len);

// д�豸
USBHIDDLL_API int USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len);

// ��ȡ�豸һЩ��Ϣ
USBHIDDLL_API void USBHIDGetDeviceCapabilitiess(USBHANDLE handle, HIDP_CAPS* caps);

#ifdef __cplusplus
}
#endif