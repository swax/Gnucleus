#if !defined(AFX_PREFCONNECTSERVERS_H__2DEAFAE0_EA73_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFCONNECTSERVERS_H__2DEAFAE0_EA73_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefConnectServers : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefConnectServers)

public:
	CPrefConnectServers();
	~CPrefConnectServers();


	//{{AFX_DATA(CPrefConnectServers)
	enum { IDD = IDD_PREF_CONNECT_SERVERS };
	CListCtrl	m_lstServers;
	CEdit	m_ebPort;
	CEdit	m_ebHost;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefConnectServers)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*	  m_autPrefs;
	

	//{{AFX_MSG(CPrefConnectServers)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnRclickListServers(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFCONNECTSERVERS_H__2DEAFAE0_EA73_11D4_ACF2_00A0CC533D52__INCLUDED_)
