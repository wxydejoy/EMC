// ServerDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerDemo.h"
#include "ServerDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDemoDlg dialog

CServerDemoDlg::CServerDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_StartServer, OnBUTTONStartServer)
	ON_BN_CLICKED(IDC_BUTTON_StopServer, OnBUTTONStopServer)
	ON_BN_CLICKED(IDC_BUTTON_SendData, OnBUTTONSendData)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_PIC, &CServerDemoDlg::OnStnClickedPic)
	//ON_EN_CHANGE(IDC_EDIT4, &CServerDemoDlg::OnEnChangeEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CServerDemoDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CServerDemoDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON_Help, &CServerDemoDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_TI, &CServerDemoDlg::OnBnClickedButtonTi)
	ON_BN_CLICKED(IDC_BUTTON_AQ, &CServerDemoDlg::OnBnClickedButtonAq)
	ON_BN_CLICKED(IDC_BUTTON_TT, &CServerDemoDlg::OnBnClickedButtonTt)
	ON_BN_CLICKED(IDC_BUTTON_IM, &CServerDemoDlg::OnBnClickedButtonIm)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RF, &CServerDemoDlg::OnBnClickedButtonRf)
	ON_BN_CLICKED(IDC_BUTTON_WV, &CServerDemoDlg::OnBnClickedButtonWv)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDemoDlg message handlers

BOOL CServerDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//��ʼ������
	m_nPortLocal = 8000 ;//����˿ںţ����Ը���Ϊ��������ֵ��
	m_bServerCreated = FALSE ;
	m_psockServer = new CTCPSocket(TCP_SOCKET_SERVER);

	SetDlgItemText(IDC_STATIC_Status,"����δ����") ;

	pWnd = GetDlgItem(IDC_PIC) ;
	pDC = pWnd->GetDC() ;



	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerDemoDlg::OnBUTTONStartServer() 
{
	if(!m_bServerCreated)
	{
		if(!m_psockServer->CreateServer(m_nPortLocal,5))
		{
			AfxMessageBox("��������������!");

			SetDlgItemText(IDC_STATIC_Status,"��������������") ;

			return;
		}
		if(!m_psockServer->StartServer(OnStatusChange,OnDataArrived,(DWORD)this))
		{
			AfxMessageBox("�����������!");

			SetDlgItemText(IDC_STATIC_Status,"�����������!") ;

			m_psockServer->Close();
			return;
		}
		m_bServerCreated = TRUE;

		SetDlgItemText(IDC_STATIC_Status,"�����ѿ���!") ;
	}
}

void CServerDemoDlg::OnStatusChange(char *data,int length,DWORD userdata)
{
	CString ClientIP;//�ͻ��˵�IP��ַ
	CString Status;
	int no;
	int op;//״̬���ֱ��
	char n[3];
	
	if(data[0]=='S')
	{
		ClientIP = CString(data+1).Left(15);
		if(data[16]=='C')
		{
			Status= ClientIP + "����\r\n";
			op=0;
		}
		if(data[16]=='D')
		{
			Status= ClientIP + "�Ͽ�\r\n";
			op=1;
		}
		memcpy(n,data+17,3);
		no=atoi(n);
	}
}

void CServerDemoDlg::OnDataArrived(char *data,int length,DWORD userdata)
{
	CServerDemoDlg *pDlg = (CServerDemoDlg*)userdata ;

	CString ClientIP;
	CString ReceiveData;
	CString Data;

	if(data[0]=='S')
	{
		ClientIP = CString(data+1).Left(15);
		Data = ClientIP + "����:";
		ReceiveData = CString(data+21).Left(length-21);
		Data = Data + ReceiveData ;
		pDlg->DealReceiveData(ReceiveData) ;		
	}
}

void CServerDemoDlg::OnBUTTONStopServer() 
{
	m_psockServer->Close();
	m_bServerCreated = FALSE;

	SetDlgItemText(IDC_STATIC_Status,"����δ����!") ;
}

void CServerDemoDlg::OnBUTTONSendData() 
{
	CString senddata = "Test" ;
	GetDlgItemText(IDC_EDIT_InPut,senddata) ;
	int nRet = m_psockServer->SendServer(0,senddata.GetBuffer(0),senddata.GetLength());
	if(nRet > 0)
	{
		AfxMessageBox("���ͳɹ���") ;
	}

}

void CServerDemoDlg::DealReceiveData(CString data) 
{
	AfxMessageBox("jieshou�ɹ���");
	//��������#��&֮ǰ��������ȡ����
	int iPos1 = data.Find("##");
	int iPos2 = data.Find("&&");

	if(iPos1 >= 0 && (iPos2 > iPos1))
	{
		data = data.Left(iPos2) ;
		int length = data.GetLength() ;
		data = data.Right(length - iPos1 - 2) ;	


		CString csTemp ;
		AfxExtractSubString(csTemp, data, 0, '_') ;//ָ��


		if(csTemp == "Clear")
		{
			Invalidate(FALSE);
		}
		else if(csTemp == "Circle")
		{
			AfxExtractSubString(csTemp, data, 1, '_') ;//X
			int X = atoi(csTemp) ;

			AfxExtractSubString(csTemp, data, 2, '_') ;//Y
			int Y = atoi(csTemp) ;

			AfxExtractSubString(csTemp, data, 3, '_') ;//R
			int R = atoi(csTemp) ;

			//CPen Pen;
			//Pen.CreatePen(PS_SOLID,2,RGB(255, 0, 0));
			//CPen *oldPen;
			//oldPen = pDC->SelectObject(&Pen);


			//pDC->Ellipse(X-R,Y-R,X+R,Y+R) ;


			//pDC->SelectObject(oldPen) ;
			////ɾ�������Ļ���
			//DeleteObject(Pen) ;
		}
		else if(csTemp == "Line")
		{
			AfxExtractSubString(csTemp, data, 1, '_') ;//X
			int StartX = atoi(csTemp) ;

			AfxExtractSubString(csTemp, data, 2, '_') ;//Y
			int StartY = atoi(csTemp) ;

			AfxExtractSubString(csTemp, data, 3, '_') ;//X
			int EndX = atoi(csTemp) ;

			AfxExtractSubString(csTemp, data, 4, '_') ;//Y
			int EndY = atoi(csTemp) ;

			//CPen Pen;
			//Pen.CreatePen(PS_SOLID,2,RGB(255, 0, 0));
			//CPen *oldPen;
			//oldPen = pDC->SelectObject(&Pen);

			//pDC->MoveTo(StartX,StartY) ;
			//pDC->LineTo(EndX,EndY) ;

			//pDC->SelectObject(oldPen) ;
			////ɾ�������Ļ���
			//DeleteObject(Pen) ;
		}

	}

}



void CServerDemoDlg::OnStnClickedPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnBnClickedButtonHelp()
{
	AfxMessageBox("> {����} {����}\n>tt30		temperature threshold �¶���ֵ 30 ���϶�\n> ti30		time interval ʱ���� 30 ����\n> aq30		Air quality threshold ����������ֵ 30 ��λ����\n>ct30		Current temperature��ǰ �¶� 30���϶�\n> ca30		Current Air quality��ǰ �������� 30 ��λ����\n> ci30		Current time interval��ǰ ʱ���� 30 ����\n> wv30		Water volumeʣ��ˮ��30 ��λ����\n> vf30		velocity of flow ��ǰ����30��λ����\n");
	
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnBnClickedButtonTi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_TI, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		AfxMessageBox("���óɹ���");
	}
}


void CServerDemoDlg::OnBnClickedButtonAq()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_AQ, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		AfxMessageBox("���óɹ���");
	}
}


void CServerDemoDlg::OnBnClickedButtonTt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_TT, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		AfxMessageBox("���óɹ���");
	}
}


void CServerDemoDlg::OnBnClickedButtonWv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_TT, senddata);
	senddata = ">wv" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		AfxMessageBox("���óɹ���");
	}
}


void CServerDemoDlg::OnBnClickedButtonIm()
{
	int state_ti = ((CButton*)GetDlgItem(IDC_CHECK_TI))->GetCheck();
	int state_tt = ((CButton*)GetDlgItem(IDC_CHECK_TT))->GetCheck();
	int state_aq = ((CButton*)GetDlgItem(IDC_CHECK_AQ))->GetCheck();
	/*CString str;
	str.Format(_T("%d"), state_aq);

	AfxMessageBox(str);*/

	CString strti,strtt,straq;

	if (state_ti)
	{
		
		CString senddata = "Test";
		GetDlgItemText(IDC_EDIT_AQ, senddata);
		
		senddata = ">aq" + senddata;
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet > 0)
		{
			senddata.Delete(0, 3);
			strti.Format(_T("ʱ����%s����\n"), senddata);
		}
	}

	if (state_tt)
	{
		
		CString senddata = "Test";
		GetDlgItemText(IDC_EDIT_TT, senddata);
		
		senddata = ">tt" + senddata;
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet > 0)
		{
			senddata.Delete(0, 3);
			strtt.Format(_T("�¶���ֵ%s���϶�\n"), senddata);
		}
	}

	if (state_aq)
	{
		
		CString senddata = "Test";
		GetDlgItemText(IDC_EDIT_TT, senddata);
		
		senddata = ">ti" + senddata;
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet > 0)
		{
			senddata.Delete(0, 3);
			straq.Format(_T("��������%s\n"), senddata);
		}
	}


	AfxMessageBox(strtt + strti + straq);


}


void CServerDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	// �ܶ�ʱ��
	if (nIDEvent == 1)
	{
		CString senddata = ">getstate";
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet != 1)
		{	
			KillTimer(1);
			AfxMessageBox("����ʧ��\n���鵱ǰ�������ӣ�");
			
		}
		else
		{
			//����״̬
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CServerDemoDlg::OnBnClickedButtonRf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString i;
	GetDlgItemText(IDC_BUTTON_RF, i); //ȡ��ť����
	if (i == _T("����")) {
		
		KillTimer(1);
		CString senddata = "Test";
		GetDlgItemText(IDC_EDIT_RF, senddata);
		int RefreshTime = _ttoi(senddata);
		if (RefreshTime > 0)
		{
			SetTimer(1, RefreshTime * 1000, NULL);
			GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("ֹͣ"));
		}
		else {
			AfxMessageBox("����ʧ��\n����д��ȷ��ˢ��Ƶ��");
		}
	}
	else {

		GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("����"));
	}
}
	


