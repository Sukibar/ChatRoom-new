
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CClientDlg �Ի���



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
	, m_usrName(_T(""))
	, m_serverIP(_T("127.0.0.1"))
	, m_serverPort(2537)
	, m_sendMsg(_T(""))
	, m_publicChannel(_T(""))
	, m_privateChannel(_T(""))
	, m_strPeopleToTalk(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_clientSocket.m_pClientDlg = this;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_USERNAME, m_usrName);
	DDX_Text(pDX, IDC_SERVERPORT, m_serverPort);
	DDV_MinMaxUInt(pDX, m_serverPort, 1024, 65535);
	DDX_Control(pDX, IDC_SERVERIP, m_ctrlServerIP);
	DDX_Text(pDX, IDC_USERNAME, m_usrName);
	DDV_MaxChars(pDX, m_usrName, 8);
	DDX_Text(pDX, IDC_MESSAGE, m_sendMsg);
	DDV_MaxChars(pDX, m_sendMsg, 800);
	DDX_Control(pDX, IDC_MESSAGE, m_ctrlSendMsg);
	DDX_Control(pDX, IDC_MSGSTYTLE, m_ctrlMsgStyle);
	DDX_Control(pDX, IDC_SELECTUSER, m_ctrlPeopleToTalk);
	DDX_Text(pDX, IDC_PUBLICAREA, m_publicChannel);
	DDX_Text(pDX, IDC_PRIVATEAREA, m_privateChannel);
	DDX_Control(pDX, IDC_USERONLINE, m_ctrlUserOnline);
	DDX_CBString(pDX, IDC_SELECTUSER, m_strPeopleToTalk);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SENDMSG, &CClientDlg::OnBnClickedSendmsg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
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
	((CButton *)GetDlgItem(IDC_MALE))->SetCheck(TRUE);
	GetDlgItem(IDC_USERNAME)->SetFocus();

	m_ctrlServerIP.SetWindowTextW(m_serverIP);

	m_editFont.CreatePointFont(120, _T("����"));
	m_ctrlSendMsg.SetFont(&m_editFont); // ����������
	
	m_ctrlMsgStyle.AddString(_T("΢Ц��"));
	m_ctrlMsgStyle.AddString(_T("���ĵ�"));
	m_ctrlMsgStyle.AddString(_T("��Ц��"));
	m_ctrlMsgStyle.AddString(_T("��ŭ��"));
	m_ctrlMsgStyle.AddString(_T("���˵�"));
	m_ctrlMsgStyle.AddString(_T("�����"));
	m_ctrlMsgStyle.AddString(_T("�˸߲��ҵ�"));
	m_ctrlMsgStyle.AddString(_T("ŭ���ɶ���"));
	m_ctrlMsgStyle.AddString(_T("����������"));
	m_ctrlMsgStyle.AddString(_T("ϲЦ�տ���"));
	m_ctrlMsgStyle.AddString(_T("���������"));
	m_ctrlMsgStyle.SetCurSel(0);

	m_ctrlPeopleToTalk.AddString(_T("������"));
	m_ctrlPeopleToTalk.SetCurSel(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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

	   //CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CClientDlg::OnBnClickedSendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("δ���ӵ���������"),_T("����"),MB_ICONERROR);
		return;
	}
	unsigned char buf[MAX_BUFSIZE]={0};
	CString strStyle;
	GetDlgItem(IDC_MSGSTYTLE)->GetWindowTextW(strStyle);
	if (m_ctrlPeopleToTalk.GetCurSel() == 0)	//ѡ���˵�0�Ҳ���Ƿ��͸�������
	{
		buf[0] = MESSAGE_ALL;	//������Ϣ
		
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16�ֽ��û���
		memcpy(buf+17,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10�ֽ���Ϣ��ʽ
		memcpy(buf+27,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//��Ϣ����
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+27);	//������Ϣ
	}
	else
	{
		UpdateData();
		buf[0] = MESSAGE_ONE;	//˽����Ϣ

		memcpy(buf+1,m_strPeopleToTalk.GetBuffer(0),m_strPeopleToTalk.GetLength()*2);	//16�ֽ�Ŀ���û���
		memcpy(buf+17,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16�ֽ�Դ�û���
		memcpy(buf+33,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10�ֽ���Ϣ��ʽ
		memcpy(buf+43,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//��Ϣ����
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+43);	//������Ϣ

		m_privateChannel+=_T("���㡿");
		m_privateChannel+=strStyle;
		m_privateChannel+=_T("* �� ��");
		m_privateChannel+=m_strPeopleToTalk;
		m_privateChannel+=_T("�� ˵��");
		m_privateChannel+=m_sendMsg;
		m_privateChannel+=_T("\r\n");
		UpdateData(FALSE);
	}
}


HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID=pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_PUBLICAREA:
	case IDC_PRIVATEAREA:
		pDC->SetBkColor(RGB(255, 255, 255));
		return ::CreateSolidBrush(RGB(255, 255, 255));
	case IDC_STATIC:
	case IDC_MALE:
	case IDC_FEMALE:
		pDC->SetBkMode(TRANSPARENT);
//e		pDC->SetTextColor(RGB(255,255,255));  //������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}


void CClientDlg::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_BTN_CONNECT)->GetWindowTextW(str);
	if (str == "���ӷ�����")	//��ǰ���ڶϿ�״̬
	{
		UpdateData();
		GetDlgItem(IDC_SERVERIP)->GetWindowTextW(m_serverIP);

		if (m_usrName.IsEmpty())
		{
			MessageBox(_T("�������û�����"),_T("��ʾ"),MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return ;
		}
		if (m_serverIP.IsEmpty())
		{
			MessageBox(_T("�����������IP��"),_T("��ʾ"),MB_ICONWARNING);
			GetDlgItem(IDC_SERVERIP)->SetFocus();
			return ;
		}
		if (!m_clientSocket.Create())	//����Socket
		{
			MessageBox(_T("�����׽���ʧ�ܣ�"),_T("����"),MB_ICONERROR);
			return;
		}
		if (!m_clientSocket.Connect(m_serverIP,m_serverPort))
		{
			MessageBox(_T("���ӷ�����ʧ�ܣ����Ժ����ԣ�"),_T("����"),MB_ICONERROR);
			m_clientSocket.Close();
			return;
		}
//		MessageBox(_T("���ӷ������ɹ���"),_T("��ʾ"),MB_ICONINFORMATION);
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("�Ͽ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//�����Ϸ������󲻿ɸ����û���

		//�����Ϸ��������Լ���������Ϣ���͵�������
		unsigned char buf[MAX_BUFSIZE];
		buf[0]=NEWCLIENT;
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
	}
	else		//��ǰ��������״̬
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//�Ͽ�����
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);		//�˳��û����û���
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();	//��������û��б�
	}
}


void CClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(IDYES==MessageBox(_T("ȷ���˳���"),_T("��ʾ"),MB_ICONQUESTION|MB_YESNO))
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//�Ͽ�����
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
		m_clientSocket.Close();
		CDialogEx::OnClose();
	}
}

void CClientDlg::ProcessRecvData()
{
	CString str;
	unsigned char buf[MAX_BUFSIZE]={0};
	int len = m_clientSocket.Receive(buf,MAX_BUFSIZE);
	wchar_t msg[MAX_BUFSIZE]={0};
	wchar_t style[MAX_BUFSIZE]={0};
	wchar_t usrName[MAX_BUFSIZE]={0};
	srand((unsigned int)time(NULL));
	if (NEWCLIENT == buf[0])		//���û�������Ϣ
	{
		memcpy(usrName,buf+1,len-1);		//�û���
		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))		//�����û�����
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))	//����˽�ĵ�������
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
		str.Empty();
		str+=_T("\t\t��");
		str+=usrName;
		str+=_T("��");
		str+=enterStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("�����ң���һ�ӭ��\r\n");
		m_publicChannel+=str;
	}
	else if (ONLINECLIENT == buf[0])		//��ǰ�����û���Ϣ
	{
		memcpy(usrName,buf+1,len-1);		//�û���
		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
	}
	else if (MESSAGE_ALL == buf[0])		//����Ƶ������Ϣ
	{
		str.Empty();
		memcpy(usrName,buf+1,16);		//ǰʮ�����ֽڴ��û���
		memcpy(style,buf+17,10);		//ʮ���ֽڵ���Ϣ��ʽ
		memcpy(msg,buf+27,len-27);		//��Ϣ����
		str+=_T("��");
		str+=usrName;
		str+=_T("�� *");
		str+=style;
		str+=_T("* �� �������ˡ� ˵��");
		str+=msg;
		m_publicChannel+=str;
		m_publicChannel+=_T("\r\n");
	}
	else if (MESSAGE_ONE == buf[0])		//˽��Ƶ������Ϣ
	{
		wchar_t srcUsrName[16]={0},dstUsrName[16]={0};
		memcpy(dstUsrName,buf+1,16);		//Ŀ���û���
		memcpy(srcUsrName,buf+17,16);		//Դ�û���
		memcpy(style,buf+33,10);			//��Ϣ��ʽ
		memcpy(msg,buf+43,len-43);		//��Ϣ����
		str.Empty();
		if (m_usrName == dstUsrName)		//���˷��������Ϣ
		{
			str+=_T("��");
			str+=srcUsrName;
			str+=_T("�� *");
			str+=style;
			str+=_T("* �� ���㡿 ˵��");
			str+=msg;
			m_privateChannel+=str;
			m_privateChannel+=_T("\r\n");
		}
	}
	else if (EXIT == buf[0])		//�뿪����Ϣ
	{
		memcpy(usrName,buf+1,len-1);		//�û���
		m_ctrlUserOnline.AddString(usrName);
		str.Empty();
		str+=_T("\t\t��");
		str+=usrName;
		str+=_T("��");
		str+=leaveStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("�����ң�\r\n");
		m_publicChannel+=str;

		int n=m_ctrlUserOnline.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlUserOnline.DeleteString((UINT)n);		//�������д����ɾ������ɾ������֪��Ϊʲô
			m_ctrlUserOnline.DeleteString((UINT)n);
		}
		n=m_ctrlPeopleToTalk.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlPeopleToTalk.DeleteString((UINT)n);
		}
	}
	else if (SERVERCLOSE == buf[0])		//�������ر���Ϣ
	{
		MessageBox(_T("�������ѹرգ�"),_T("��ʾ"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();
	}
	else if (REPEATNAME == buf[0])
	{
		MessageBox(_T("�û����Ѵ��ڣ������ԣ�"),_T("��ʾ"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();
	}
	UpdateData(FALSE);
}

void CClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	unsigned char buf[MAX_BUFSIZE];
	buf[0] = EXIT;	//�Ͽ�����
	memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
	m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
	m_clientSocket.Close();
	// TODO: �ڴ˴������Ϣ����������
}
