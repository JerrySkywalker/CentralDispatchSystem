// TCPClient_MFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "TCPClient_MFCDlg.h"

#include <atlconv.h>
#include <string>

#include "afxdialogex.h"
#include "framework.h"
#include "TCPClient_MFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string GetTimeStamp_Now(void)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	return std::to_string(st.wHour) + ":" + std::to_string(st.wMinute) + ":" + std::to_string(st.wSecond);
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()

// CTCPClientMFCDlg 对话框

CTCPClientMFCDlg::CTCPClientMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPCLIENT_MFC_DIALOG, pParent)
	, m_StrSendMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_target_ip);
	DDX_Control(pDX, IDC_EDIT1, m_target_port);
	DDX_Control(pDX, IDC_LIST2, m_msg_box);
	DDX_Text(pDX, IDC_EDIT2, m_StrSendMsg);
}

BEGIN_MESSAGE_MAP(CTCPClientMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPClientMFCDlg::OnBnClickedButton_Disconnect)
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPClientMFCDlg::OnBnClickedButton_Connect)
	ON_BN_CLICKED(IDC_BUTTON4, &CTCPClientMFCDlg::OnBnClickedButton_ClearMsg)
	ON_BN_CLICKED(IDC_BUTTON3, &CTCPClientMFCDlg::OnBnClickedButton_SaveMsgLog)
	ON_BN_CLICKED(IDC_BUTTON5, &CTCPClientMFCDlg::OnBnClickedButton_Send)
END_MESSAGE_MAP()

// CTCPClientMFCDlg 消息处理程序

BOOL CTCPClientMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CEdit& Edit1 = *((CEdit*)(GetDlgItem(IDC_IPADDRESS1)));
	CEdit& Edit2 = *((CEdit*)(GetDlgItem(IDC_EDIT1)));
	Edit1.SetWindowTextW(CString("127.0.0.1"));
	Edit2.SetWindowTextW(CString("6688"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPClientMFCDlg::OnPaint()
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
HCURSOR CTCPClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCPClientMFCDlg::OnBnClickedButton_Connect()
{
	// TODO: 在此添加控件通知处理程序代码
	try {
		UpdateData(TRUE);

		CString ServerAddress;
		CString ServerPort;

		m_target_ip.GetWindowTextW(ServerAddress);
		m_target_port.GetWindowTextW(ServerPort);

		std::string server_address = (CStringA)ServerAddress;
		std::string server_port = (CStringA)ServerPort;

		BOOL bInit = AfxSocketInit();
		if (!bInit)
		{
			auto msg = "[" + GetTimeStamp_Now() + "] "
				+ "AfxSocketInit Failed!";
			m_msg_box.AddString(CString(msg.c_str()));

			UpdateData(FALSE);
			return;
		}

		if (m_socket == NULL)
		{
			m_socket = new CClientSocket;
			m_socket->Create();

			std::string msg = "[" + GetTimeStamp_Now() + "] "
				+ "Target selected." + server_address + ":" + server_port;

			m_msg_box.AddString(CString(msg.c_str()));

			msg = "[" + GetTimeStamp_Now() + "] "
				+ "Socket created successfully!";
			m_msg_box.AddString(CString(msg.c_str()));
		}
		else
		{
			std::string msg = "[" + GetTimeStamp_Now() + "] "
				+ "Connecting already in progress!";
			m_msg_box.AddString(CString(msg.c_str()));

			m_socket->Close();
		}
		m_socket->Connect(ServerAddress, _ttoi(ServerPort));
		m_socket->SetListBox(&m_msg_box);

		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
	catch (std::exception const& ex) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "std::exception. " + ex.what();
		m_msg_box.AddString(CString(msg.c_str()));

		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
	catch (...) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Unknown exception!";
		m_msg_box.AddString(CString(msg.c_str()));

		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
}

void CTCPClientMFCDlg::OnBnClickedButton_Disconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_socket != NULL) try
	{
		m_socket->ShutDown();
		m_socket->Close();
		delete m_socket;
		m_socket = NULL;

		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Socket closed successfully!";
		m_msg_box.AddString(CString(msg.c_str()));

		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
	catch (std::exception const& ex) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "std::exception. " + ex.what();
		m_msg_box.AddString(CString(msg.c_str()));
		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
	catch (...) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Unknown exception!";
		m_msg_box.AddString(CString(msg.c_str()));
		m_msg_box.SetCurSel(m_msg_box.GetCount() - 1);
	}
}

void CTCPClientMFCDlg::OnBnClickedButton_ClearMsg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_msg_box.ResetContent();
}

void CTCPClientMFCDlg::OnBnClickedButton_SaveMsgLog()
{
	BOOL isOpen = FALSE;
	CString defaultDir = L"D:\\log";

	SYSTEMTIME st;
	GetLocalTime(&st);

	std::string str_log_name = "Log_" + std::to_string(st.wYear) + "-" + std::to_string(st.wMonth) + "-" + std::to_string(st.wDay)
		+ "_" + GetTimeStamp_Now() + ".log";

	CString fileName = (CString)str_log_name.c_str();
	CString filter = L"Log (*.log)|*.log||";
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);

	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\log";

	INT_PTR result = openFileDlg.DoModal();

	CString filePath;

	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();

		CStdioFile log;
		CFileException logException;
		if (log.Open(filePath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate), &logException)
		{
			CString line;

			int Count = m_msg_box.GetCount();
			for (int i = 0; i < Count; i++)
			{
				m_msg_box.GetText(i, line);
				log.WriteString(line + "\n");
			}

			log.Close();
		}
		else
		{
			TRACE("Can't open file %s,error=%u\n", filePath, logException.m_cause);
		}
	}
}

void CTCPClientMFCDlg::OnBnClickedButton_Send()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	std::string buffer_str = (CStringA)m_StrSendMsg;
	char* buffer = const_cast<char*>(buffer_str.c_str());

	if (m_socket != NULL) try
	{
		m_socket->Send(buffer, strlen(buffer));
	}
	catch (std::exception const& ex) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "std::exception. " + ex.what();
		m_msg_box.AddString(CString(msg.c_str()));
	}
	catch (...) {
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Unknown exception!";
		m_msg_box.AddString(CString(msg.c_str()));
	}
}