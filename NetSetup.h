#if !defined(AFX_NETSETUP_H__FBC4DC0C_3DE9_4356_BA60_DE478D7AD51A__INCLUDED_)
#define AFX_NETSETUP_H__FBC4DC0C_3DE9_4356_BA60_DE478D7AD51A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutNetwork;
class CAutPrefs;


class CNetSetup : public CDialog
{
public:
	CNetSetup(CGnucleusDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	~CNetSetup();

	//{{AFX_DATA(CNetSetup)
	enum { IDD = IDD_LAN_SETUP };
	CListCtrl	m_lstLan;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	CGnucleusDoc*	m_pDoc;
	CAutNetwork*	m_autNetwork;
	CAutPrefs*		m_autPrefs;

	int m_ListSize;

	//{{AFX_MSG(CNetSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonJoin();
	afx_msg void OnButtonNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSETUP_H__FBC4DC0C_3DE9_4356_BA60_DE478D7AD51A__INCLUDED_)
