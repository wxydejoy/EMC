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

	//初始化变量
	m_nPortLocal = 8000 ;//服务端口号（可以更换为其他整型值）
	m_bServerCreated = FALSE ;
	m_psockServer = new CTCPSocket(TCP_SOCKET_SERVER);

	SetDlgItemText(IDC_STATIC_Status,"服务未开启") ;

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
			AfxMessageBox("创建服务器出错!");

			SetDlgItemText(IDC_STATIC_Status,"创建服务器出错！") ;

			return;
		}
		if(!m_psockServer->StartServer(OnStatusChange,OnDataArrived,(DWORD)this))
		{
			AfxMessageBox("启动服务出错!");

			SetDlgItemText(IDC_STATIC_Status,"启动服务出错!") ;

			m_psockServer->Close();
			return;
		}
		m_bServerCreated = TRUE;

		SetDlgItemText(IDC_STATIC_Status,"服务已开启!") ;
	}
}

void CServerDemoDlg::OnStatusChange(char *data,int length,DWORD userdata)
{
	CString ClientIP;//客户端的IP地址
	CString Status;
	int no;
	int op;//状态区分标记
	char n[3];
	
	if(data[0]=='S')
	{
		ClientIP = CString(data+1).Left(15);
		if(data[16]=='C')
		{
			Status= ClientIP + "连接\r\n";
			op=0;
		}
		if(data[16]=='D')
		{
			Status= ClientIP + "断开\r\n";
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
		Data = ClientIP + "发送:";
		ReceiveData = CString(data+21).Left(length-21);
		Data = Data + ReceiveData ;
		pDlg->DealReceiveData(ReceiveData) ;		
	}
}

void CServerDemoDlg::OnBUTTONStopServer() 
{
	m_psockServer->Close();
	m_bServerCreated = FALSE;

	SetDlgItemText(IDC_STATIC_Status,"服务未开启!") ;
}

void CServerDemoDlg::OnBUTTONSendData() 
{
	CString senddata = "Test" ;
	GetDlgItemText(IDC_EDIT_InPut,senddata) ;
	int nRet = m_psockServer->SendServer(0,senddata.GetBuffer(0),senddata.GetLength());
	if(nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("指令%s已发送成功\n"), senddata);
		AfxMessageBox(sendstr) ;
	}
	else
	{
		AfxMessageBox("请检查网络连接！");
	}
}

void CServerDemoDlg::DealReceiveData(CString data) 
{
	
	//将数据中#与&之前的数据提取出来
	int iPos1 = data.Find(">");
	int iPos2 = data.Find("<");

	if(iPos1 >= 0 && (iPos2 > iPos1))
	{
		//>030100100030110010020<



		SetDlgItemText(IDC_STATIC_WV, data.Mid(17, 2) + "%");//显示水量
		//SetDlgItemText(IDC_STATIC_Status_VF, data.Mid(19, 3));//显示流速

		SetDlgItemText(IDC_STATIC_Status_TI, data.Mid(7, 3) + "分钟");//显示时间间隔
		SetDlgItemText(IDC_STATIC_Status_TT, data.Mid(11, 2) + "摄氏度");//温度阈值
		SetDlgItemText(IDC_STATIC_Status_CT, data.Mid(2, 2)+ "摄氏度");//环境温度
		SetDlgItemText(IDC_STATIC_Status_CA, data.Mid(4, 3));//环境空气质量
		SetDlgItemText(IDC_STATIC_Status_AQ, data.Mid(15, 3));//空气质量阈值

		CString rftime = "Test";
		GetDlgItemText(IDC_EDIT_RF, rftime);

		SetDlgItemText(IDC_STATIC_Status_RF, rftime);//刷新频率


		//数据转换
		int ct = _ttoi(data.Mid(1,3));//环境温度
		int ca = _ttoi(data.Mid(4, 3)); //空气质量3位
		int ti = _ttoi(data.Mid(7, 3));//时间间隔
		
		int tt = _ttoi(data.Mid(10, 3));//温度阈值
		int aq = _ttoi(data.Mid(13, 3));//空气质量阈值
		int wv = _ttoi(data.Mid(16, 3));//水量
		int vf = _ttoi(data.Mid(19, 3));//流速

		CString wv2,tempt;
		tempt.Format(_T("%d"),wv / 10);
		wv2 = "预计还可使用" + tempt + "天";//计算水量剩余使用时间
		SetDlgItemText(IDC_STATIC_Status_WV2, wv2);//显示
		//进度条
		// 
		CProgressCtrl* pgwv = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_WV);
		pgwv->SetRange(0, 100);
		pgwv->SetPos(wv);
		

		CProgressCtrl* pgvf = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_VF);
		pgvf->SetRange(0, 100);
		pgvf->SetPos(vf);



		//int RefreshTime = _ttoi(senddata);//转换为整数


		//SetDlgItemText(IDC_STATIC_WV, "启动服务出错!");
		if (data.Mid(19, 1) == "1")
		{
				SetDlgItemText(IDC_STATIC_Status_ON, "系统运行中");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_Status_ON, "系统未运行");
		}



		/*CString str;
		str.Format(_T("%s"), data);
		AfxMessageBox(str);*/
	

	}

}



void CServerDemoDlg::OnStnClickedPic()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnBnClickedButtonHelp()
{
	AfxMessageBox("> {命令} {数据}\n>tt30		temperature threshold 温度阈值 30 摄氏度\n> ti30		time interval 时间间隔 30 分钟\n> aq30		Air quality threshold 空气质量阈值 30 单位再议\n");
	
	
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnBnClickedButtonTi()
{
	// TODO: 在此添加控件通知处理程序代码
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_TI, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("时间间隔%s分钟已设置成功\n"), senddata.Mid(3, 2));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("请检查网络连接");
	}
}


void CServerDemoDlg::OnBnClickedButtonAq()
{
	// 这个不能用了，用Im2，莫名其妙的bug
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_AQ, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("颗粒浓度%s已设置成功\n"), senddata.Mid(3, 3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("请检查网络连接");
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
		sendstr.Format(_T("颗粒浓度%s已设置成功\n"), senddata.Mid(3, 3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("请检查网络连接");
	}
}


void CServerDemoDlg::OnBnClickedButtonTt()
{
	// TODO: 在此添加控件通知处理程序代码
	CString senddata = "Test";
	GetDlgItemText(IDC_EDIT_TT, senddata);
	senddata = ">aq" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("温度阈值%s已设置成功\n"), senddata.Mid(3,3));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("请检查网络连接");
	}
}


void CServerDemoDlg::OnBnClickedButtonWv()
{
	// TODO: 在此添加控件通知处理程序代码
	CString senddata = "Test";
	GetDlgItemText(IDC_Time_WV, senddata);
	senddata = ">wv" + senddata;
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet > 0)
	{
		CString sendstr;
		sendstr.Format(_T("单次水量%sL\n已设置成功\n"), senddata.Mid(3, 2));
		AfxMessageBox(sendstr);
	}
	else
	{
		AfxMessageBox("请检查网络连接");
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
			strti.Format(_T("时间间隔%s分钟\n"), senddata);
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
			strtt.Format(_T("温度阈值%s摄氏度\n"), senddata);
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
			straq.Format(_T("空气质量%s\n"), senddata);
		}
	}
	if (state >= 1)
	{
		 AfxMessageBox(strtt + strti + straq);

	}
	else
	{
		AfxMessageBox("发送失败\n请检查当前网络连接！");
	}

	


}


void CServerDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	// 总定时器
	if (nIDEvent == 1)
	{
		CString senddata = ">getstate";
		int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
		if (nRet <= 0)
		{	
			KillTimer(1);
			AfxMessageBox("发送失败\n请检查当前网络连接！");
			
		}
		else
		{
			//更新状态
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CServerDemoDlg::OnBnClickedButtonRf()
{
	// TODO: 在此添加控件通知处理程序代码

	CString i;
	GetDlgItemText(IDC_BUTTON_RF, i); //取按钮标题
	if (i == _T("启动")) {
		
		KillTimer(1);
		CString senddata = "Test";
		GetDlgItemText(IDC_EDIT_RF, senddata);
		SetDlgItemText(IDC_STATIC_Status_RF,senddata + "秒");//显示时间间隔
		int RefreshTime = _ttoi(senddata);
		if (RefreshTime > 0)
		{
			SetTimer(1, RefreshTime * 1000 , NULL);
			GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("停止"));

			//senddata.Format(_T("刷新频率%s分钟\n已设置成功\n"), senddata);
			//AfxMessageBox(senddata);
			
		}
		else {
			AfxMessageBox("设置失败\n请填写正确的刷新频率");
		}
	}
	else {
		KillTimer(1);
		GetDlgItem(IDC_BUTTON_RF)->SetWindowText(_T("启动"));
	}
}
	




void CServerDemoDlg::OnBnClickedButtonRf2()
{
	CString senddata = ">getstate";
	int nRet = m_psockServer->SendServer(0, senddata.GetBuffer(0), senddata.GetLength());
	if (nRet <= 0)
	{
		AfxMessageBox("发送失败\n请检查当前网络连接！");

	}
	else
	{
		AfxMessageBox("设置成功");
	}
}



void CServerDemoDlg::OnStnClickedStaticStatusWv2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnStnClickedStaticStatusVf()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDemoDlg::OnStnClickedStaticStatus()
{
	// TODO: 在此添加控件通知处理程序代码
}
