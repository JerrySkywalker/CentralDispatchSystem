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
		char buffer[1024] = { 0 };
		int recvLen = this->Receive(buffer, sizeof(buffer));

		std::string str = buffer;

		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Reply: "+ str.c_str();

		m_ListBox->AddString(CString(msg.c_str()));

		int count = 0;
		count = m_ListBox->GetCount();

		m_ListBox->SetCurSel(count - 1);
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

		m_ListBox->AddString(CString(msg.c_str()));
	}
	else
	{
		std::string msg = "[" + GetTimeStamp_Now() + "] "
			+ "Connection Failure. Error code:" + std::to_string(nErrorCode);

		m_ListBox->AddString(CString(msg.c_str()));
	}
}