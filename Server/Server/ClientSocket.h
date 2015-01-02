#pragma once
#include "afxsock.h"

class CClientSocket :
	public CSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);
	virtual void OnReceive(int nErrorCode);

	CString m_userName;		//�û���
	CString m_userIP;		//�û�IP
	UINT m_userPort;		//�û��˿�
	CServerDlg *m_pSerDlg;	//������ָ��
};

