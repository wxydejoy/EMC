// Login.cpp : implementation file
//

#include "stdafx.h"
#include "ServerDemo.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_strUserName = _T("");
	m_strPassWord = _T("");
	//}}AFX_DATA_INIT
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Text(pDX, IDC_EDIT_UserName, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PassWord, m_strPassWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	//{{AFX_MSG_MAP(CLogin)
	ON_BN_CLICKED(IDC_BUTTON_Reset, OnBUTTONReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

void CLogin::OnOK() 
{
	UpdateData(TRUE) ;//获取编辑输入的内容

	//对固定用户名和密码进行验证
	if(m_strUserName == "张三" && m_strPassWord == "123456")
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("登录失败，请检查用户名或密码是否输入正确！") ;
	}
}

void CLogin::OnBUTTONReset() 
{
	//点击按钮，清空输入的内容
	m_strUserName = "" ;
	m_strPassWord = "" ;

	UpdateData(FALSE) ;//编辑框内容清空
}
