#if !defined(AFX_PREFCONNECT_H__A6C36B21_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFCONNECT_H__A6C36B21_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefConnect : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefConnect)

public:
	CPrefConnect();
	~CPrefConnect();


	//{{AFX_DATA(CPrefConnect)
	enum { IDD = IDD_PREF_CONNECT };
	CEdit	m_ebMinConnects;
	CEdit	m_ebMaxConnects;
	CEdit	m_ebLeafConnects;
	CButton	m_chkMinConnects;
	CButton	m_chkMaxConnects;
	CButton	m_chkAutoConnect;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefConnect)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;


	//{{AFX_MSG(CPrefConnect)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckAutoconnect();
	afx_msg void OnCheckMinConnects();
	afx_msg void OnChangeEditMinConnects();
	afx_msg void OnCheckMaxConnects();
	afx_msg void OnChangeEditMaxConnects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditLeafConnects();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFCONNECT_H__A6C36B21_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
