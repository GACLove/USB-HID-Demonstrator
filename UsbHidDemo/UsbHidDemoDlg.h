
// UsbHidDemoDlg.h : 头文件
//

#pragma once


// CUsbHidDemoDlg 对话框
class CUsbHidDemoDlg : public CDialogEx
{
// 构造
public:
	CUsbHidDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_USBHIDDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    USBHANDLE handle;

    HIDP_CAPS caps;

    BYTE* fileData;

	bool isWrite;

    int fileLen;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedWriteByte();
    afx_msg void OnBnClickedReFresh();
    CEdit m_wndWriteEdit;                                      // 输入
    CButton m_wndReport;
    CStatic m_wndDeviceName;
    CStatic m_wndRead;                                         // 读取到数据
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedSetReport();
    afx_msg void OnBnClickedSetFeature();
    CEdit m_wndFilePath;
    afx_msg void OnBnClickedButtonOpenfile();
    CStatic m_wndFileStatus;
    afx_msg void OnBnClickedButtonWritedata();
    afx_msg void OnBnClickedButtonTestread();
    HANDLE hThread;
    void TestReadProc();
    virtual BOOL DestroyWindow();
    CStatic m_openlog;
};
