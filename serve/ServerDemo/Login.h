#if !defined(AFX_LOGIN_H__47292C2C_A33C_4BE3_A5E4_B4C61D2B79D0__INCLUDED_)
#define AFX_LOGIN_H__47292C2C_A33C_4BE3_A5E4_B4C61D2B79D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class CLogin : public CDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_DIALOG_Login };
	CString	m_strUserName;
	CString	m_strPassWord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogin)
	virtual void OnOK();
	afx_msg void OnBUTTONReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__47292C2C_A33C_4BE3_A5E4_B4C61D2B79D0__INCLUDED_)
