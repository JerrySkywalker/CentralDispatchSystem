#include "pch.h"
#include "tcp_socket.h"

#include <boost/exception/to_string.hpp>


#include "TCPClient_MFCDlg.h"

extern std::string GetTimeStamp_Now();

CClientSocket::CClientSocket(void)
{
}


CClientSocket::~CClientSocket(void)
{
}



void CClientSocket::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		CString szBuf;
		INT nReceiveCounts = Receive((VOID*)szBuf.GetBuffer(1000), 1000);
		m_ListBox->AddString(szBuf);
	}
}
void CClientSocket::OnClose(int nErrorCode)
{

}

void CClientSocket::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Connected successfully!." ;

		m_ListBox->InsertString(0, CString(msg.c_str()));
	}
	else
	{
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Connection Failure. Error code:" + std::to_string(nErrorCode);

		m_ListBox->InsertString(0, CString(msg.c_str()));
	}
}