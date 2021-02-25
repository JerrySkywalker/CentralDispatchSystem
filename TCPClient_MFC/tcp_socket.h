#pragma once
#include "afxsock.h"
class CClientSocket final :
	public CAsyncSocket
{
public:
	CClientSocket(void);
	virtual ~CClientSocket(void);

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual  void OnConnect(int nErrorCode);

	void SetListBox(CListBox* ListBox) { m_ListBox = ListBox; }
	CListBox* m_ListBox;
};
