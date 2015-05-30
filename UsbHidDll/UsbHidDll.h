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
/************************************************************************/
/*                     ���ڲ����豸��ʶ���豸��API                           */
/************************************************************************/
//�����豸 ����ҵ��򷵻�true�����򷵻�false
USBHIDDLL_API bool __stdcall USBHIDFindUSBHIDDevice();

//��ȡDevicePath
USBHIDDLL_API char* __stdcall USBHIDGetDevicePath();

//��ȡ�豸����
USBHIDDLL_API char* __stdcall USBHIDGetDeviceName();

/************************************************************************/
/*                      ��ȡ�������豸��Ϣ                                  */
/************************************************************************/

// �����豸(��д�������)
USBHIDDLL_API void __stdcall USBHIDSetType(USBHIDTYPE type);

// �����豸һЩ������Ϣ
USBHIDDLL_API void __stdcall  USBHIDResetConfig();

// ��ȡ�豸һЩ��Ϣ
USBHIDDLL_API void __stdcall USBHIDGetDeviceCapabilities(USBHANDLE handle, PHIDD_ATTRIBUTES attributes, PHIDP_CAPS caps);


/************************************************************************/
/*             �򿪣��رպͶ�д����                                         */
/************************************************************************/

// ���豸
USBHIDDLL_API USBHANDLE __stdcall USBHIDCreateUsbHid();

// ���豸
USBHIDDLL_API int __stdcall USBHIDReadByte(USBHANDLE handle, BYTE* byte, int len);

// д�豸
USBHIDDLL_API int __stdcall USBHIDWriteByte(USBHANDLE handle, BYTE* byte, int len);

// �ر��豸
USBHIDDLL_API void __stdcall USBHIDCloseHandle(USBHANDLE handle);

#ifdef __cplusplus
}
#endif