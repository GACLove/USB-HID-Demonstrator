
// UsbHidDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UsbHidDemo.h"
#include "UsbHidDemoDlg.h"
#include "afxdialogex.h"
#include "UsbHidDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DWORD WINAPI TreadProc(LPVOID lpThreadParameter)
{
    // ���������߳�
    CUsbHidDemoDlg* dlg = (CUsbHidDemoDlg*)lpThreadParameter;
    while(1)
    {
		dlg->TestReadProc();
    }
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUsbHidDemoDlg �Ի���




CUsbHidDemoDlg::CUsbHidDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUsbHidDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    handle = NULL;
}

void CUsbHidDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_wndWriteEdit);
    DDX_Control(pDX, IDC_RADIO1, m_wndReport);
    DDX_Control(pDX, IDC_STATIC_DEVICE, m_wndDeviceName);
    DDX_Control(pDX, IDC_STATIC_READ, m_wndRead);
    DDX_Control(pDX, IDC_EDIT_FILEPATH, m_wndFilePath);
    DDX_Control(pDX, IDC_STATIC_STATUS, m_wndFileStatus);
    DDX_Control(pDX, IDC_STATIC_LOG, m_openlog);
}

BEGIN_MESSAGE_MAP(CUsbHidDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON2, &CUsbHidDemoDlg::OnBnClickedWriteByte)
    ON_BN_CLICKED(IDC_BUTTON1, &CUsbHidDemoDlg::OnBnClickedReFresh)
    ON_BN_CLICKED(IDC_BUTTON4, &CUsbHidDemoDlg::OnBnClickedOpen)
    ON_BN_CLICKED(IDC_RADIO1, &CUsbHidDemoDlg::OnBnClickedSetReport)
    ON_BN_CLICKED(IDC_RADIO2, &CUsbHidDemoDlg::OnBnClickedSetFeature)
    ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CUsbHidDemoDlg::OnBnClickedButtonOpenfile)
    ON_BN_CLICKED(IDC_BUTTON_WRITEDATA, &CUsbHidDemoDlg::OnBnClickedButtonWritedata)
    ON_BN_CLICKED(IDC_BUTTON_TESTREAD, &CUsbHidDemoDlg::OnBnClickedButtonTestread)
END_MESSAGE_MAP()


// CUsbHidDemoDlg ��Ϣ�������

BOOL CUsbHidDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_wndReport.SetCheck(BST_CHECKED);

    if (USBHIDFindUSBHIDDevice())
    {
        CString deviceName = USBHIDGetDeviceName();
        m_wndDeviceName.SetWindowText(deviceName);
    }

	isWrite = false;

    fileData = NULL;

    handle = NULL;
    hThread = CreateThread(NULL, 0, TreadProc, this, 0, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUsbHidDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUsbHidDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUsbHidDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsbHidDemoDlg::OnBnClickedWriteByte()
{
    CString tmp;
    m_wndWriteEdit.GetWindowTextA(tmp);
    if (tmp.IsEmpty()) return ; // ��ʾ��������
    int len = tmp.GetLength();

    BYTE* pBuffer = (BYTE*)tmp.GetBuffer(len);
    BYTE* buffer = new BYTE[len];

    // ת����16����
	int index = 0; 
	char* token = strtok(tmp.GetBuffer(0), " ");
	char* token2;
	while (token)
	{
		buffer[index] = strtol(token, &token2, 16);
		token = strtok(NULL, " ");
		index++;
	}

	isWrite = true;

    BYTE* tmpBuf = buffer;

    // ѭ��д��
    while (index > caps.OutputReportByteLength)
    {
        // �����豸����д
        USBHIDWriteByte(handle, tmpBuf, caps.OutputReportByteLength);
        
        // ʣ�������
        index -= caps.OutputReportByteLength;
        
        // �����Ѿ�д��
        tmpBuf += caps.OutputReportByteLength;
    }

    if (index > 0)
    {
        USBHIDWriteByte(handle, tmpBuf, index);
    }
   
	isWrite = false;

    delete [] buffer;
}


void CUsbHidDemoDlg::OnBnClickedReFresh()
{
    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    if (USBHIDFindUSBHIDDevice())
    {
        CString deviceName = USBHIDGetDeviceName();
        m_wndDeviceName.SetWindowText(deviceName);
    }
}

// 00 fd 01 00 a0 c0 01 02 00 00 00 00 66 fe
void CUsbHidDemoDlg::OnBnClickedOpen()
{
    // ����Ѿ��򿪣���ر�
    if (handle)
    {
        USBHIDCloseHandle(handle);
        handle = NULL;
        ((CButton*)GetDlgItem(IDC_BUTTON4))->SetWindowTextA("Open");
    }
    else
    {
        handle = USBHIDCreateUsbHid();
        if (NULL == handle)
        {
            ((CButton*)GetDlgItem(IDC_BUTTON4))->SetWindowTextA("Open");
        }
        else
        {
            ((CButton*)GetDlgItem(IDC_BUTTON4))->SetWindowTextA("Close");
            HIDD_ATTRIBUTES attributes;
            // ��ȡ�豸����
            USBHIDGetDeviceCapabilities(handle, &attributes, &caps);
        }
    }
}


void CUsbHidDemoDlg::OnBnClickedSetReport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USBHIDSetType(T_Report);
}


void CUsbHidDemoDlg::OnBnClickedSetFeature()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    USBHIDSetType(T_Feature);
}


void CUsbHidDemoDlg::OnBnClickedButtonOpenfile()
{
    BYTE file1Data[2] ={1, 0};
    FILE* fp = fopen("D:\\test.bin", "wb");
    fwrite(file1Data,1,2,fp);
    fclose(fp);
   CString filePathName;
   CFileDialog dlg(TRUE, 
       NULL, 
       NULL,
       OFN_HIDEREADONLY |  OFN_OVERWRITEPROMPT,
       _T("bin File (*.bin)|*.bin|All Files(*.*)|*.*||"),
       NULL);

   if (dlg.DoModal() == IDOK)
   {
       filePathName = dlg.GetPathName();
       m_wndFilePath.SetWindowText(filePathName);
       FILE* fp = fopen(filePathName.GetBuffer(0), "rb+");

       if (fp == NULL)
       {
           MessageBox("Open file error.");
           return ;
       }

       fseek(fp, 0, SEEK_END);

       // ��ȡ�ļ���С
       int len = ftell(fp);

       // ɾ����һ�εĿռ�
       if (fileData != NULL)
       {
           delete [] fileData;
       }
       // ����ռ��С
       fileData = new BYTE[len];

       // ��ȡ����
       fseek(fp, 0, SEEK_SET);
       fileLen = 0;
       fileLen = fread(fileData, 1, len, fp);

       CString fstatus;
       // ��ʾ��С
       fstatus.Format("�ļ���С:%d",fileLen);
       m_wndFileStatus.SetWindowText(fstatus);

       fclose(fp);
       fp = NULL;
   }
}


void CUsbHidDemoDlg::OnBnClickedButtonWritedata()
{
    if (handle)
    {
        BYTE* tmpbuf = fileData;
        // ѭ��д��
        while (fileLen > caps.OutputReportByteLength)
        {
            // �����豸����д
            USBHIDWriteByte(handle, tmpbuf, caps.OutputReportByteLength);

            // ʣ�������
            fileLen -= caps.OutputReportByteLength;

            // �����Ѿ�д��
            tmpbuf += caps.OutputReportByteLength;
        }

        if (fileLen > 0)
        {
            USBHIDWriteByte(handle, tmpbuf, fileLen);
        }
    }
    delete [] fileData;
}

void CUsbHidDemoDlg::TestReadProc()
{
    if (handle)
    {
		if (isWrite)
		{
			Sleep(50);
		}
		else
		{
			BYTE tmpData[30];
			memset(tmpData, '\0', 30);
			// BYTE tmpData[10] = {1, 2, 3, 4, 5};
			int len = USBHIDReadByte(handle, tmpData, 30);

			int kk = 0;
			char tmp[1024];
			memset(tmp, '\0', 1024);

			if (len <= 0)
			{
				Sleep(50);
				return;
			}
			CString text;
			for (int i = 0; i < len; i++)
			{
				sprintf(tmp, "%02x ", tmpData[i]);  // ��16������ʾ
				text += tmp;
			}
			m_wndRead.SetWindowText(text);
		}
    }
}
void CUsbHidDemoDlg::OnBnClickedButtonTestread()
{
   if (handle)
   {
       BYTE tmpData[18];
	   memset(tmpData, 0, 18);
      // BYTE tmpData[10] = {1, 2, 3, 4, 5};
       int len = USBHIDReadByte(handle, tmpData, 18);
       char tmp[18];
	   memset(tmp, '\0', 18);

       CString text;
       for (int i = 0; i < len; i++)
       {
           sprintf(tmp, "0x%02x ", tmpData[i]);  // ��16������ʾ
           text += tmp;
       }
       m_wndRead.SetWindowText(text);
   }
}


BOOL CUsbHidDemoDlg::DestroyWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    USBHIDCloseHandle(handle);
    if (hThread)
    {
        CloseHandle(hThread);
    }
    return CDialogEx::DestroyWindow();
}
