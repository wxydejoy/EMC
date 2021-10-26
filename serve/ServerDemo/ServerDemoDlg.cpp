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
	ON_BN_CLICKED(IDC_BUTTON_RF2, &CServerDemoDlg::OnBnClickedButtonRf2)
	ON_BN_CLICKED(IDC_BUTTON_IM2, &CServerDemoDlg::OnBnClickedButtonIm2)
	ON_STN_CLICKED(IDC_STATIC_Status_WV2, &CServerDemoDlg::OnStnClickedStaticStatusWv2)
	ON_STN_CLICKED(IDC_STATIC_Status_VF, &CServerDemoDlg::OnStnClickedStaticStatusVf)
	ON_STN_CLICKED(IDC_STATIC_Status, &CServerDemoDlg::OnStnClickedStaticStatus)
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
		CString sendstr;
		sendstr.Format(_T("ָ��%s�ѷ��ͳɹ�\n"), senddata);
		AfxMessageBox(sendstr) ;
	}
	else
	{
		AfxMessageBox("�����������ӣ�");
	}
}

void CServerDemoDlg::DealReceiveData(CString data) 
{
	
	//��������#��&֮ǰ��������ȡ����
	int iPos1 = data.Find(">");
	int iPos2 = data.Find("<");

	if(iPos1 >= 0 && (iPos2 > iPos1))
	{
		//>030100100030110010020<



		SetDlgItemText(IDC_STATIC_WV, data.Mid(17, 2) + "%");//��ʾˮ��
		//SetDlgItemText(IDC_STATIC_Status_VF, data.Mid(19, 3));//��ʾ����

		SetDlgItemText(IDC_STATIC_Status_TI, data.Mid(7, 3) + "����");//��ʾʱ����
		SetDlgItemText(IDC_STATIC_Status_TT, data.Mid(11, 2) + "���϶�");//�¶���ֵ
		SetDlgItemText(IDC_STATIC_Status_CT, data.Mid(2, 2)+ "���϶�");//�����¶�
		SetDlgItemText(IDC_STATIC_Status_CA, data.Mid(4, 3));//������������
		SetDlgItemText(IDC_STATIC_Status_AQ, data.Mid(15, 3));//����������ֵ

		CString rftime = "Test";
		GetDlgItemText(IDC_EDIT_RF, rftime);

		SetDlgItemText(IDC_STATIC_Status_RF, rftime);//ˢ��Ƶ��


		//����ת��
		int ct = _ttoi(data.Mid(1,3));//�����¶�
		int ca = _ttoi(data.Mid(4, 3)); //��������3λ
		int ti = _ttoi(data.Mid(7, 3));//ʱ����
		
		int tt = _ttoi(data.Mid(10, 3));//�¶���ֵ
		int aq = _ttoi(data.Mid(13, 3));//����������ֵ
		int wv = _ttoi(data.Mid(16, 3));//ˮ��
		int vf = _ttoi(data.Mid(19, 3));//����

		CString wv2,tempt;
		tempt.Format(_T("%d"),wv / 10);
		wv2 = "Ԥ�ƻ���ʹ��" + tempt + "��";//����ˮ��ʣ��ʹ��ʱ��
		SetDlgItemText(IDC_STATIC_Status_WV2, wv2);//��ʾ
		//������
		// 
		CProgressCtrl* pgwv = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_WV);
		pgwv->SetRange(0, 100);
		pgwv->SetPos(wv);
		

		CProgressCtrl* pgvf = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_VF);
		pgvf->SetRange(0, 100);
		pgvf->SetPos(vf);



		//int RefreshTime = _ttoi(senddata);//ת��Ϊ����


		//SetDlgItemText(IDC_STATIC_WV, "�����������!");
		if (data.Mid(19, 1) == "1")
		{
				SetDlgItemText(IDC_STATIC_Status_ON, "ϵͳ������");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_Status_ON, "ϵͳδ����");
		}



		/*CString str;
		str.Format(_T("%s"), data);
		AfxMessageBox(str);*/
	

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
	AfxMessageBox("> {����} {����}\n>tt30		temperature threshold �¶���ֵ 30 ���϶�\n> ti30		time interval ʱ���� 30 ����\n> aq30		Air quality threshold ����������ֵ 30 ��λ����\n");
	
	
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
		CString sendstr;
		sendstr.Format(_T("ʱ����%s���������óɹ�\n"), senddata.Mid(3, 2));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("������������");
	}
}


void CServerDemoDlg::OnBnClickedButtonAq()
{
	// ����������ˣ���Im2��Ī�������bug
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_AQ, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("����Ũ��%s�����óɹ�\n"), senddata.Mid(3, 3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("������������");
	}
}

void CServerDemoDlg::OnBnClickedButtonIm2()
{
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_AQ, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("����Ũ��%s�����óɹ�\n"), senddata.Mid(3, 3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("������������");
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
		CString sendstr;
		sendstr.Format(_T("�¶���ֵ%s�����óɹ�\n"), senddata.Mid(3,3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("������������");
	}
}


void CServerDemoDlg::OnBnClickedButtonWv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString senddata = "Test";
	GetDlgItemText(IDC_Time_WV, senddata);
	senddata = ">wv" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("����ˮ��%sL\n�����óɹ�\n"), senddata.Mid(3, 2));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("������������");
	}
}


void CServerDemoDlg::OnBnClickedButtonIm()
{
	int state = 0;
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
			state += 1;
			senddata.Delete(0, 3);
			strti.Format(_T("ʱ����%s����\n"), senddata);
		}
	}

	if (state_tt)
	{
		
		state += 1;
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
		
		state += 1;
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
	if (state >= 1)
	{
		 AfxMessageBox(strtt + strti + straq);

	}
	else
	{
		AfxMessageBox("����ʧ��\n���鵱ǰ�������ӣ�");
	}

	


}


void CServerDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	// �ܶ�ʱ��
	if (nIDEvent == 1)
	{
		CString senddata = ">getstate";
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet <= 0)
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
		SetDlgItemText(IDC_STATIC_Status_RF,senddata + "��");//��ʾʱ����
		int RefreshTime = _ttoi(senddata);
		if (RefreshTime > 0)
		{
			SetTimer(1, RefreshTime * 1000 , NULL);
			GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("ֹͣ"));

			//senddata.Format(_T("ˢ��Ƶ��%s����\n�����óɹ�\n"), senddata);
			//AfxMessageBox(senddata);
			
		}
		else {
			AfxMessageBox("����ʧ��\n����д��ȷ��ˢ��Ƶ��");
		}
	}
	else {
		KillTimer(1);
		GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("����"));
	}
}
	




void CServerDemoDlg::OnBnClickedButtonRf2()
{
	CString senddata = ">getstate";
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet <= 0)
	{
		AfxMessageBox("����ʧ��\n���鵱ǰ�������ӣ�");

	}
	else
	{
		AfxMessageBox("���óɹ�");
	}
}



void CServerDemoDlg::OnStnClickedStaticStatusWv2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnStnClickedStaticStatusVf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CServerDemoDlg::OnStnClickedStaticStatus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
