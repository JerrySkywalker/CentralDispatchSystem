// TCPClient_MFCDlg.h: 头文件
//

#pragma once

#include "tcp_socket.h"

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
	CClientSocket* m_socket;
	CIPAddressCtrl		m_target_ip;
	CEdit				m_target_port;

	CListBox			m_msg_box;

	afx_msg void OnBnClickedButton_Connect();
	afx_msg void OnBnClickedButton_Disconnect();

	afx_msg void OnBnClickedButton_ClearMsg();
	afx_msg void OnBnClickedButton_SaveMsgLog();
	afx_msg void OnBnClickedButton_Send();
	CString m_StrSendMsg;
};
