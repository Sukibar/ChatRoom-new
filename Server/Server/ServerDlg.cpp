
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "afxdialogex.h"
#include "ServerDlg.h"
#include "ClientSocket.h"
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<CClientSocket*> clientList;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg �Ի���



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_clientCnt(0)
	, m_listenPort(2537)
	, m_maxClient(50)
	, m_srvrName(_T(""))
	, m_srvrStatus(_T("��ֹͣ"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverSocket.m_pSerDlg=this;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENTCNT, m_clientCnt);
	DDX_Text(pDX, IDC_LISTENPORT, m_listenPort);
	//  DDX_CBIndex(pDX, IDC_MAXCLIENT, m_maxClient);
	DDX_Control(pDX, IDC_SERVERIP, m_ctrlServerIP);
	DDX_Text(pDX, IDC_SERVERNAME, m_srvrName);
	DDX_Text(pDX, IDC_STATUS, m_srvrStatus);
	DDX_Control(pDX, IDC_MAXCLIENT, m_ctrlMaxCnt);
	DDX_Control(pDX, IDC_CHATROOMINFO, m_ctrlChatRoomInfo);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_START_STOP, &CServerDlg::OnBnClickedBtnStartStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CServerDlg::OnBnClickedBtnClear)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CServerDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!AfxSocketInit())
	{
	   AfxMessageBox(_T("Failed to Initialize Sockets"), MB_OK | MB_ICONSTOP);
	   exit(1);
	}
	char hostname[256];
	PHOSTENT hostinfo;
	CString IP;
	gethostname(hostname,sizeof(hostname));		//��ȡ������
	hostinfo=gethostbyname(hostname);			//������������ȡ����IP

	m_srvrName=hostname;	//���÷���������
	IP=inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
	m_ctrlServerIP.SetWindowTextW(IP);	//���÷�����IP
	
	m_ctrlMaxCnt.AddString(_T("50"));
	m_ctrlMaxCnt.AddString(_T("40"));
	m_ctrlMaxCnt.AddString(_T("30"));
	m_ctrlMaxCnt.AddString(_T("20"));
	m_ctrlMaxCnt.AddString(_T("10"));
	m_ctrlMaxCnt.AddString(_T("5"));
	m_ctrlMaxCnt.SetCurSel(0);

	m_serverSocket.m_pSerDlg=this;
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap bmpBackground;   

       bmpBackground.LoadBitmap(IDB_BACKGROUND); 

       BITMAP bitmap;   
       bmpBackground.GetBitmap(&bitmap);   
       CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);   

       dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

//		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialogEx::OnOK();
}


void CServerDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IDYES==MessageBox(_T("ȷ�Ϲرշ�������"),_T("��ʾ"),MB_ICONQUESTION|MB_YESNO))
	{
		CDialogEx::OnCancel();
	}
}


void CServerDlg::OnBnClickedBtnStartStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_srvrStatus=="��ֹͣ")	//״̬Ϊ��ֹͣ����Ҫ���������������˿�
	{
		UpdateData();
		WCHAR str[3];
		m_ctrlMaxCnt.GetWindowTextW(str,3);
		m_maxClient=_ttoi(str);
		if (m_listenPort<1024)
		{
			MessageBox(_T("��ѡ�����1024�Ķ˿ڣ�"),_T("��ʾ"),MB_ICONWARNING);
			return;
		}
		if (!m_serverSocket.Create(m_listenPort))
		{
			MessageBox(_T("�����׽��ֳ���"),_T("����"),MB_ICONERROR);
			return;
		}
		if (!m_serverSocket.Listen())
		{
			MessageBox(_T("����ʧ�ܣ�"),_T("����"),MB_ICONERROR);
			return;
		}
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("ֹͣ"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(FALSE);
	}
	else		//״̬Ϊ���ڼ�����Ҫֹͣ����
	{
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("����"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(TRUE);
		m_clientCnt = 0;
		vector<CClientSocket *>::iterator iter;
		for (iter = clientList.begin(); iter != clientList.end(); )
		{
			unsigned char buf[2]={0};
			buf[0]=SERVERCLOSE;
			(*iter)->Send(buf,1);
			(*iter)->Close();
			delete (*iter);
			iter=clientList.erase(iter);
		}
		clientList.clear();
		m_serverSocket.Close();
	}

	m_srvrStatus=m_srvrStatus=="���ڼ�������"?"��ֹͣ":"���ڼ�������";
	GetDlgItem(IDC_STATUS)->SetWindowTextW(m_srvrStatus);
	UpdateData(FALSE);
}



void CServerDlg::OnBnClickedBtnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlChatRoomInfo.ResetContent();
}


HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID=pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_SERVERNAME:
	case IDC_STATUS:
	case IDC_CLIENTCNT:
		pDC->SetBkColor(RGB(255, 255, 255));
		return ::CreateSolidBrush(RGB(255, 255, 255));
	case IDC_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));  //������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}


void CServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_serverSocket.Close();
	CDialogEx::OnClose();
}

void CServerDlg::ProcessData(CClientSocket *clientSocket)
{
	unsigned char buf[MAX_BUFSIZE]={0};
	wchar_t wbuf[MAX_BUFSIZE]={0};
	wchar_t srcName[16]={0},dstName[16]={0};
	int len = clientSocket->Receive(buf,MAX_BUFSIZE);
	vector<CClientSocket *>::iterator iter; 
	CString log,usrName;
	CTime tm;
	switch(buf[0])
	{
	case NEWCLIENT:		//�յ��������û�������Ϣ��ת���������ˣ��������û�����ȫ�������û���Ϣ
		memcpy(wbuf,buf+1,len - 1);		//��ȡ�û���
		usrName = wbuf;
		clientSocket->m_userName = usrName;		//�����û���
		for (iter= clientList.begin(); iter < clientList.end()-1; iter++)
		{
			if((*iter) ->m_userName == usrName)
			{
				buf[0]=REPEATNAME;
				clientSocket->Send(buf,1);		//�û����ظ�
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if(*iter != NULL)
			{
				(*iter)->Send(buf,len);		//ת����ÿ���ͻ���
			}
		}
		Sleep(100);
		for (iter= clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if(*iter != NULL)
			{
				buf[0] = ONLINECLIENT;
				memcpy(wbuf,(*iter)->m_userName.GetBuffer(0),(*iter)->m_userName.GetLength()*2);
				memcpy(buf+1,wbuf,(*iter)->m_userName.GetLength()*2);
				clientSocket->Send(buf,(*iter)->m_userName.GetLength()*2+1); //���������û�����Ϣ�������ߵ��û�
				Sleep(100);
			}
		}
		break;

	case MESSAGE_ALL:	//�յ����Ƿ��͸������˵�������Ϣ
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if(*iter != NULL)
			{
				(*iter)->Send(buf,len);		//ת����ÿ���ͻ���
			}
		}
		break;
	case MESSAGE_ONE:	//�յ����Ƿ��͸�ĳ���˵�������Ϣ
		memcpy(dstName,buf+1,16);
		memcpy(srcName,buf+17,16);
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if((*iter) -> m_userName == dstName)	//����Ŀ���û�
			{
				(*iter)->Send(buf,len);		//ת��
				break;
			}
		}
		break;
	case EXIT:		//�Ͽ����ӵĴ���

		tm=CTime::GetCurrentTime();
		log.Format(_T("��%s : %u����������Ͽ���"),clientSocket->m_userIP,clientSocket->m_userPort);
		log=tm.Format("%Y-%m-%d %X : ")+log;
		m_ctrlChatRoomInfo.AddString(log);
		m_clientCnt--;
		UpdateData(FALSE);
		clientSocket->Close();
		for (iter = clientList.begin(); iter != clientList.end();)
		{
			if (*iter == clientSocket)		//���ӶϿ���ɾ���׽���
			{
				iter=clientList.erase(iter);
			}
			else		//�������ͻ���ת�����ߵ���Ϣ
			{
				(*iter)->Send(buf,len);		
				iter++;
			}
		}
		delete clientSocket;
		break;
	default:
		break;
	}
}

void CServerDlg::OnBnClickedBtnExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Text Files(*.txt)|*.txt||"),AfxGetMainWnd());
	CString strPath,strLog;
	if (dlg.DoModal() == IDOK)
	{
		strPath=dlg.GetPathName();
		if (strPath.Right(4)!=".txt")
			strPath+=".txt";
	}
	CFile file(strPath,CFile::modeCreate|CFile::modeWrite);
	for (int i = 0; i < m_ctrlChatRoomInfo.GetCount(); i++)
	{
		CString str;
		m_ctrlChatRoomInfo.GetText(i,str);
		strLog+=str;
		strLog+=_T("\r\n");
	}
	file.Write(strLog,strLog.GetLength()*2);
	file.Close();
	MessageBox(_T("�����ɹ���"),_T("��ʾ"),MB_ICONINFORMATION);
}
