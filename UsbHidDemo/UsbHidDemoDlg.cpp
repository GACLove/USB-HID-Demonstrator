
// UsbHidDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UsbHidDemo.h"
#include "UsbHidDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUsbHidDemoDlg 对话框




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
}

BEGIN_MESSAGE_MAP(CUsbHidDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON2, &CUsbHidDemoDlg::OnBnClickedWriteByte)
    ON_BN_CLICKED(IDC_BUTTON1, &CUsbHidDemoDlg::OnBnClickedReFresh)
    ON_BN_CLICKED(IDC_BUTTON3, &CUsbHidDemoDlg::OnBnClickedGetCaps)
    ON_BN_CLICKED(IDC_BUTTON4, &CUsbHidDemoDlg::OnBnClickedOpen)
    ON_BN_CLICKED(IDC_RADIO1, &CUsbHidDemoDlg::OnBnClickedSetReport)
    ON_BN_CLICKED(IDC_RADIO2, &CUsbHidDemoDlg::OnBnClickedSetFeature)
END_MESSAGE_MAP()


// CUsbHidDemoDlg 消息处理程序

BOOL CUsbHidDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_wndReport.SetCheck(BST_CHECKED);

    if (USBHIDFindUSBHIDDevice())
    {
        CString deviceName = USBHIDGetDeviceName();
        m_wndDeviceName.SetWindowText(deviceName);
    }
    
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUsbHidDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUsbHidDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsbHidDemoDlg::OnBnClickedWriteByte()
{
    CString tmp;
    m_wndWriteEdit.GetWindowTextA(tmp);
    if (tmp.IsEmpty()) return ; // 提示输入内容
    BYTE* pBuffer = (BYTE*)tmp.GetBuffer(tmp.GetLength());
    int writtedNum = USBHIDWriteByte(handle, pBuffer, tmp.GetLength());
}


void CUsbHidDemoDlg::OnBnClickedReFresh()
{
    // TODO: 在此添加额外的初始化代码
    if (USBHIDFindUSBHIDDevice())
    {
        CString deviceName = USBHIDGetDeviceName();
        m_wndDeviceName.SetWindowText(deviceName);
    }
}


void CUsbHidDemoDlg::OnBnClickedGetCaps()
{
    HIDD_ATTRIBUTES attributes;
    HIDP_CAPS caps;
    USBHIDGetDeviceCapabilities(handle, &attributes, &caps);
}


void CUsbHidDemoDlg::OnBnClickedOpen()
{
   if (NULL == USBHIDCreateUsbHid())
   {
       MessageBox("打开USB设备失败,请确认设备是否连接！");
   }
}


void CUsbHidDemoDlg::OnBnClickedSetReport()
{
    // TODO: 在此添加控件通知处理程序代码
    USBHIDSetType(T_Report);
}


void CUsbHidDemoDlg::OnBnClickedSetFeature()
{
    // TODO: 在此添加控件通知处理程序代码
    USBHIDSetType(T_Feature);
}
