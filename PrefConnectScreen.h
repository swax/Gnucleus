#if !defined(AFX_PREFCONNECTSCREEN_H__066322C1_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFCONNECTSCREEN_H__066322C1_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefConnectScreen : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefConnectScreen)

public:
	CPrefConnectScreen();
	~CPrefConnectScreen();


	//{{AFX_DATA(CPrefConnectScreen)
	enum { IDD = IDD_PREF_CONNECT_SCREEN };
	CEdit	m_ebHost;
	CComboBox	m_cmbRule;
	CListCtrl	m_lstScreen;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefConnectScreen)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;


	//{{AFX_MSG(CPrefConnectScreen)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFCONNECTSCREEN_H__066322C1_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
