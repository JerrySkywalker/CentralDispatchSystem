
// TCPClient_MFCDlg.h: 头文件
//

#pragma once


// CTCPClientMFCDlg 对话框
class CTCPClientMFCDlg : public CDialogEx
{
// 构造
public:
	CTCPClientMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPCLIENT_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	// Target IP Address
	CIPAddressCtrl m_target_ip;
	// Target IP Port
	CEdit m_target_port;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton_Disconnect();
};
