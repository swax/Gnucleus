#if !defined(AFX_CONNECTINFO_H__A84B9583_DFFE_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_CONNECTINFO_H__A84B9583_DFFE_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;


class CConnectInfo : public CDialog
{
public:
	CConnectInfo(CWnd* pParent = NULL, CGnucleusDoc* pDoc = NULL);   // standard constructor
	~CConnectInfo();

	CString m_Host;
	UINT	m_Port;
	COleDateTime   m_Time;
	CString m_Handshake;

	//Makslane: asynchronous host name resolving
	char bufHost[MAXGETHOSTSTRUCT];
	HANDLE hAsync;


	//{{AFX_DATA(CConnectInfo)
	enum { IDD = IDD_CONNECT_INFO };
	CEdit	m_ebHostName;
	CEdit	m_ebHost;
	CStatic	m_stcTime;
	CEdit	m_ebHandshake;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CConnectInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;


	// Generated message map functions
	//{{AFX_MSG(CConnectInfo)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAsyncGetHostByAddr(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTINFO_H__A84B9583_DFFE_11D4_ACF2_00A0CC533D52__INCLUDED_)
