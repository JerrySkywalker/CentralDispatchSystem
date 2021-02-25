// TCPClient_MFCDlg.h: 头文件
//

#pragma once

#include "tcp_client.h"

#include <boost/asio/io_context.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

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

	//Custom for this client
protected:
	boost::scoped_ptr< boost::asio::io_context>		io_context_ptr_;			
	boost::recursive_mutex							mtx_reset_connection_;	    // recursive_mutex for thread-safe using of the unique pointer to the client object
	volatile bool									connecting_in_progress_;	// volatile flag - for thread-safe detecting, that connecting in progress

	// Try to connect to the server
	void create_new_connection(std::string server_address, unsigned int server_port, unsigned int timeout);																

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
	CListBox m_msg_box;

	afx_msg void OnBnClickedButton_Connect();
	afx_msg void OnBnClickedButton_Disconnect();

	afx_msg void OnBnClickedButton_ClearMsg();
	afx_msg void OnBnClickedButton_SaveMsgLog();
};
