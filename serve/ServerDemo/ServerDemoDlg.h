// ServerDemoDlg.h : header file
//

#if !defined(AFX_SERVERDEMODLG_H__5D4FB9A7_40E8_4638_B0EE_85B3A9DDE5E5__INCLUDED_)
#define AFX_SERVERDEMODLG_H__5D4FB9A7_40E8_4638_B0EE_85B3A9DDE5E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCPSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CServerDemoDlg dialog



class CServerDemoDlg : public CDialog
{
// Construction
public:
	CServerDemoDlg(CWnd* pParent = NULL);	// standard constructor

	CTCPSocket* m_psockServer;//Socket服务端指针
	int m_nPortLocal;//服务端端口号
	BOOL m_bServerCreated;//是否开启服务的标记

	//状态回调
	static void OnStatusChange(char *data,int length,DWORD userdata);
	//数据回调
	static void OnDataArrived(char *data,int length,DWORD userdata);

	void DealReceiveData(CString data) ;

	CWnd* pWnd ;
	CDC* pDC ;


// Dialog Data
	//{{AFX_DATA(CServerDemoDlg)
	enum { IDD = IDD_SERVERDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONStartServer();
	afx_msg void OnBUTTONStopServer();
	afx_msg void OnBUTTONSendData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDEMODLG_H__5D4FB9A7_40E8_4638_B0EE_85B3A9DDE5E5__INCLUDED_)
