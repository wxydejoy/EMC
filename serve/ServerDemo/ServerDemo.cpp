// ServerDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerDemo.h"
#include "ServerDemoDlg.h"

#include "Login.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDemoApp

BEGIN_MESSAGE_MAP(CServerDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CServerDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDemoApp construction

CServerDemoApp::CServerDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerDemoApp object

CServerDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerDemoApp initialization

BOOL CServerDemoApp::InitInstance()
{
	//添加一个登录对话框对象
	CLogin loginDlg ;

	//弹出对话框窗口
	if(loginDlg.DoModal() == IDOK)
	{
		//点击的确定按钮，并通过用户名和密码验证，程序可以进去
	}
	else
	{
		//否则直接退出
		exit(0) ;
	}



	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CServerDemoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
