#if !defined(AFX_NETSETUPNEW_H__C6504438_D347_4A1B_A6C0_3D45E719B227__INCLUDED_)
#define AFX_NETSETUPNEW_H__C6504438_D347_4A1B_A6C0_3D45E719B227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNetSetupNew : public CDialog
{
public:
	CNetSetupNew(CWnd* pParent = NULL);   // standard constructor

	CString m_Name;
	CString m_IRCServer;
	CString m_Homepage;


	//{{AFX_DATA(CNetSetupNew)
	enum { IDD = IDD_LAN_NEW };
	CEdit	m_ebName;
	CEdit	m_ebIRC;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CNetSetupNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:



	//{{AFX_MSG(CNetSetupNew)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSETUPNEW_H__C6504438_D347_4A1B_A6C0_3D45E719B227__INCLUDED_)
