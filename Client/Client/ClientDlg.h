
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ClientSocket.h"

const int MAX_BUFSIZE = 1024;
#define EXIT			255
#define SERVERCLOSE		254
#define REPEATNAME		253
#define NEWCLIENT		1
#define ONLINECLIENT	2
#define MESSAGE_ALL		5
#define MESSAGE_ONE		6

#define MAX_ENTERSTYLE  5
#define MAX_LEAVESTYLE  5
const static wchar_t enterStyle[][20]={_T("һ����Ʒ���"),
										_T("͵͵���������"),
										_T("��ҡ��ڵ��߽�"),
										_T("��������ؽ���"),
										_T("��ͥ�Ų����߽�")};
const static wchar_t leaveStyle[][20]={_T("����������뿪��"),
										_T("�������뿪��"),
										_T("��į��ת���뿪��"),
										_T("�Ų�������뿪��"),
										_T("ͷҲ���ص��뿪��")};
// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSendmsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString m_usrName;
	CString m_serverIP;
	CIPAddressCtrl m_ctrlServerIP;
	UINT m_serverPort;
	CClientSocket m_clientSocket;
	CString m_sendMsg;
	CFont m_editFont;
	CEdit m_ctrlSendMsg;
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnClose();

	void ProcessRecvData();
	CComboBox m_ctrlMsgStyle;
	CComboBox m_ctrlPeopleToTalk;
	CString m_publicChannel;
	CString m_privateChannel;
	CListBox m_ctrlUserOnline;
	afx_msg void OnDestroy();
	CString m_strPeopleToTalk;
};
